/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/dictbuilder.h"
#include "../include/dicttrie.h"
#include "../include/mystdlib.h"
#include "../include/ngram.h"
#include "../include/searchutility.h"
#include "../include/spellingtable.h"
#include "../include/spellingtrie.h"
#include "../include/splparser.h"
#include "../include/utf16reader.h"

namespace ime_pinyin {

#ifdef ___BUILD_MODEL___

static const unsigned kReadBufLen = 512;
static const unsigned kSplTableHashLen = 2000;

// Compare a SingleCharItem, first by Hanzis, then by spelling ids, then by
// frequencies.
int cmp_scis_hz_splid_freq(const void* p1, const void* p2) {
  const SingleCharItem *s1, *s2;
  s1 = static_cast<const SingleCharItem*>(p1);
  s2 = static_cast<const SingleCharItem*>(p2);

  if (s1->hz < s2->hz)
    return -1;
  if (s1->hz > s2->hz)
    return 1;

  if (s1->splid.half_splid < s2->splid.half_splid)
    return -1;
  if (s1->splid.half_splid > s2->splid.half_splid)
    return 1;

  if (s1->splid.full_splid < s2->splid.full_splid)
    return -1;
  if (s1->splid.full_splid > s2->splid.full_splid)
    return 1;

  if (s1->freq > s2->freq)
    return -1;
  if (s1->freq < s2->freq)
    return 1;
  return 0;
}

int cmp_scis_hz_splid(const void* p1, const void* p2) {
  const SingleCharItem *s1, *s2;
  s1 = static_cast<const SingleCharItem*>(p1);
  s2 = static_cast<const SingleCharItem*>(p2);

  if (s1->hz < s2->hz)
    return -1;
  if (s1->hz > s2->hz)
    return 1;

  if (s1->splid.half_splid < s2->splid.half_splid)
    return -1;
  if (s1->splid.half_splid > s2->splid.half_splid)
    return 1;

  if (s1->splid.full_splid < s2->splid.full_splid)
    return -1;
  if (s1->splid.full_splid > s2->splid.full_splid)
    return 1;

  return 0;
}

int cmp_lemma_entry_hzs(const void* p1, const void* p2) {
  unsigned size1 = utf16_strlen(((const LemmaEntry*)p1)->hanzi_str);
  unsigned size2 = utf16_strlen(((const LemmaEntry*)p2)->hanzi_str);
  if (size1 < size2)
    return -1;
  else if (size1 > size2)
    return 1;

  return utf16_strcmp(((const LemmaEntry*)p1)->hanzi_str,
                      ((const LemmaEntry*)p2)->hanzi_str);
}

int compare_char16(const void* p1, const void* p2) {
  if (*((const char16*)p1) < *((const char16*)p2))
    return -1;
  if (*((const char16*)p1) > *((const char16*)p2))
    return 1;
  return 0;
}

int compare_py(const void* p1, const void* p2) {
  int ret = utf16_strcmp(((const LemmaEntry*)p1)->spl_idx_arr,
                         ((const LemmaEntry*)p2)->spl_idx_arr);

  if (0 != ret)
    return ret;

  return static_cast<int>(((const LemmaEntry*)p2)->freq) -
         static_cast<int>(((const LemmaEntry*)p1)->freq);
}

// First hanzi, if the same, then Pinyin
int cmp_lemma_entry_hzspys(const void* p1, const void* p2) {
  unsigned size1 = utf16_strlen(((const LemmaEntry*)p1)->hanzi_str);
  unsigned size2 = utf16_strlen(((const LemmaEntry*)p2)->hanzi_str);
  if (size1 < size2)
    return -1;
  else if (size1 > size2)
    return 1;
  int ret = utf16_strcmp(((const LemmaEntry*)p1)->hanzi_str,
                         ((const LemmaEntry*)p2)->hanzi_str);

  if (0 != ret)
    return ret;

  ret = utf16_strcmp(((const LemmaEntry*)p1)->spl_idx_arr,
                     ((const LemmaEntry*)p2)->spl_idx_arr);
  return ret;
}

int compare_splid2(const void* p1, const void* p2) {
  int ret = utf16_strcmp(((const LemmaEntry*)p1)->spl_idx_arr,
                         ((const LemmaEntry*)p2)->spl_idx_arr);
  return ret;
}

DictBuilder::DictBuilder() {
  lemma_arr_ = NULL;
  lemma_num_ = 0;

  scis_ = NULL;
  scis_num_ = 0;

  lma_nodes_le0_ = NULL;
  lma_nodes_ge1_ = NULL;

  lma_nds_used_num_le0_ = 0;
  lma_nds_used_num_ge1_ = 0;

  homo_idx_buf_ = NULL;
  homo_idx_num_eq1_ = 0;
  homo_idx_num_gt1_ = 0;

  top_lmas_ = NULL;
  top_lmas_num_ = 0;

  spl_table_ = NULL;
  spl_parser_ = NULL;
}

DictBuilder::~DictBuilder() {
  free_resource();
}

bool DictBuilder::alloc_resource(unsigned lma_num) {
  if (0 == lma_num)
    return false;

  free_resource();

  lemma_num_ = lma_num;
  lemma_arr_ = new LemmaEntry[lemma_num_];

  top_lmas_num_ = 0;
  top_lmas_ = new LemmaEntry[kTopScoreLemmaNum];

  // New the scis_ buffer to the possible maximum size.
  scis_num_ = lemma_num_ * kMaxLemmaSize;
  scis_ = new SingleCharItem[scis_num_];

  // The root and first level nodes is less than kMaxSpellingNum + 1
  lma_nds_used_num_le0_ = 0;
  lma_nodes_le0_ = new LmaNodeLE0[kMaxSpellingNum + 1];

  // Other nodes is less than lemma_num
  lma_nds_used_num_ge1_ = 0;
  lma_nodes_ge1_ = new LmaNodeGE1[lemma_num_];

  homo_idx_buf_ = new LemmaIdType[lemma_num_];
  spl_table_ = new SpellingTable();
  spl_parser_ = new SpellingParser();

  if (NULL == lemma_arr_ || NULL == top_lmas_ ||
      NULL == scis_ || NULL == spl_table_ ||
      NULL == spl_parser_ || NULL == lma_nodes_le0_ ||
      NULL == lma_nodes_ge1_ || NULL == homo_idx_buf_) {
    free_resource();
    return false;
  }

  memset(lemma_arr_, 0, sizeof(LemmaEntry) * lemma_num_);
  memset(scis_, 0, sizeof(SingleCharItem) * scis_num_);
  memset(lma_nodes_le0_, 0, sizeof(LmaNodeLE0) * (kMaxSpellingNum + 1));
  memset(lma_nodes_ge1_, 0, sizeof(LmaNodeGE1) * lemma_num_);
  memset(homo_idx_buf_, 0, sizeof(LemmaIdType) * lemma_num_);
  spl_table_->init_table(kMaxPinyinSize, kSplTableHashLen, true);

  return true;
}

char16* DictBuilder::read_valid_hanzis(const char *fn_validhzs, unsigned *num) {
  if (NULL == fn_validhzs || NULL == num)
    return NULL;

  *num = 0;
  FILE *fp = fopen(fn_validhzs, "rb");
  if (NULL == fp)
    return NULL;

  char16 utf16header;
  if (fread(&utf16header, sizeof(char16), 1, fp) != 1 ||
      0xfeff != utf16header) {
    fclose(fp);
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  *num = ftell(fp) / sizeof(char16);
  assert(*num >= 1);
  *num -= 1;

  char16 *hzs = new char16[*num];
  if (NULL == hzs) {
    fclose(fp);
    return NULL;
  }

  fseek(fp, 2, SEEK_SET);

  if (fread(hzs, sizeof(char16), *num, fp) != *num) {
    fclose(fp);
    delete [] hzs;
    return NULL;
  }
  fclose(fp);

  myqsort(hzs, *num, sizeof(char16), compare_char16);
  return hzs;
}

bool DictBuilder::hz_in_hanzis_list(const char16 *hzs, unsigned hzs_len,
                                    char16 hz) {
  if (NULL == hzs)
    return false;

  char16 *found;
  found = static_cast<char16*>(
      mybsearch(&hz, hzs, hzs_len, sizeof(char16), compare_char16));
  if (NULL == found)
    return false;

  assert(*found == hz);
  return true;
}

// The caller makes sure that the parameters are valid.
bool DictBuilder::str_in_hanzis_list(const char16 *hzs, unsigned hzs_len,
                                     const char16 *str, unsigned str_len) {
  if (NULL == hzs || NULL == str)
    return false;

  for (unsigned pos = 0; pos < str_len; pos++) {
    if (!hz_in_hanzis_list(hzs, hzs_len, str[pos]))
      return false;
  }
  return true;
}

void DictBuilder::get_top_lemmas() {
  top_lmas_num_ = 0;
  if (NULL == lemma_arr_)
    return;

  for (unsigned pos = 0; pos < lemma_num_; pos++) {
    if (0 == top_lmas_num_) {
      top_lmas_[0] = lemma_arr_[pos];
      top_lmas_num_ = 1;
      continue;
    }

    if (lemma_arr_[pos].freq > top_lmas_[top_lmas_num_ - 1].freq) {
      if (kTopScoreLemmaNum > top_lmas_num_)
        top_lmas_num_ += 1;

      unsigned move_pos;
      for (move_pos = top_lmas_num_ - 1; move_pos > 0; move_pos--) {
        top_lmas_[move_pos] = top_lmas_[move_pos - 1];
        if (0 == move_pos - 1 ||
            (move_pos - 1 > 0 &&
             top_lmas_[move_pos - 2].freq > lemma_arr_[pos].freq)) {
          break;
        }
      }
      assert(move_pos > 0);
      top_lmas_[move_pos - 1] = lemma_arr_[pos];
    } else if (kTopScoreLemmaNum > top_lmas_num_) {
      top_lmas_[top_lmas_num_] = lemma_arr_[pos];
      top_lmas_num_ += 1;
    }
  }

  if (kPrintDebug0) {
    printf("\n------Top Lemmas------------------\n");
    for (unsigned pos = 0; pos < top_lmas_num_; pos++) {
      printf("--%d, idx:%06d, score:%.5f\n", pos, top_lmas_[pos].idx_by_hz,
             top_lmas_[pos].freq);
    }
  }
}

void DictBuilder::free_resource() {
  if (NULL != lemma_arr_)
    delete [] lemma_arr_;

  if (NULL != scis_)
    delete [] scis_;

  if (NULL != lma_nodes_le0_)
    delete [] lma_nodes_le0_;

  if (NULL != lma_nodes_ge1_)
    delete [] lma_nodes_ge1_;

  if (NULL != homo_idx_buf_)
    delete [] homo_idx_buf_;

  if (NULL != spl_table_)
    delete spl_table_;

  if (NULL != spl_parser_)
    delete spl_parser_;

  lemma_arr_ = NULL;
  scis_ = NULL;
  lma_nodes_le0_ = NULL;
  lma_nodes_ge1_ = NULL;
  homo_idx_buf_ = NULL;
  spl_table_ = NULL;
  spl_parser_ = NULL;

  lemma_num_ = 0;
  lma_nds_used_num_le0_ = 0;
  lma_nds_used_num_ge1_ = 0;
  homo_idx_num_eq1_ = 0;
  homo_idx_num_gt1_ = 0;
}

unsigned DictBuilder::read_raw_dict(const char* fn_raw,
                                  const char *fn_validhzs,
                                  unsigned max_item) {
  if (NULL == fn_raw) return 0;

  Utf16Reader utf16_reader;
  if (!utf16_reader.open(fn_raw, kReadBufLen * 10))
    return false;

  char16 read_buf[kReadBufLen];

  // Read the number of lemmas in the file
  unsigned lemma_num = 240000;

  // allocate resource required
  if (!alloc_resource(lemma_num)) {
    utf16_reader.close();
  }

  // Read the valid Hanzi list.
  char16 *valid_hzs = NULL;
  unsigned valid_hzs_num = 0;
  valid_hzs = read_valid_hanzis(fn_validhzs, &valid_hzs_num);

  // Begin reading the lemma entries
  for (unsigned i = 0; i < max_item; i++) {
    // read next entry
    if (!utf16_reader.readline(read_buf, kReadBufLen)) {
      lemma_num = i;
      break;
    }

    unsigned token_size;
    char16 *token;
    char16 *to_tokenize = read_buf;

    // Get the Hanzi string
    token = utf16_strtok(to_tokenize, &token_size, &to_tokenize);
    if (NULL == token) {
      free_resource();
      utf16_reader.close();
      return false;
    }

    unsigned lemma_size = utf16_strlen(token);

    if (lemma_size > kMaxLemmaSize) {
      i--;
      continue;
    }

    if (lemma_size > 4) {
      i--;
      continue;
    }

    // Copy to the lemma entry
    utf16_strcpy(lemma_arr_[i].hanzi_str, token);

    lemma_arr_[i].hz_str_len = token_size;

    // Get the freq string
    token = utf16_strtok(to_tokenize, &token_size, &to_tokenize);
    if (NULL == token) {
      free_resource();
      utf16_reader.close();
      return false;
    }
    lemma_arr_[i].freq = utf16_atof(token);

    if (lemma_size > 1 && lemma_arr_[i].freq < 60) {
      i--;
      continue;
    }

    // Get GBK mark, if no valid Hanzi list available, all items which contains
    // GBK characters will be discarded. Otherwise, all items which contains
    // characters outside of the valid Hanzi list will be discarded.
    token = utf16_strtok(to_tokenize, &token_size, &to_tokenize);
    assert(NULL != token);
    int gbk_flag = utf16_atoi(token);
    if (NULL == valid_hzs || 0 == valid_hzs_num) {
      if (0 != gbk_flag) {
        i--;
        continue;
      }
    } else {
      if (!str_in_hanzis_list(valid_hzs, valid_hzs_num,
          lemma_arr_[i].hanzi_str, lemma_arr_[i].hz_str_len)) {
        i--;
        continue;
      }
    }

    // Get spelling String
    bool spelling_not_support = false;
    for (unsigned hz_pos = 0; hz_pos < (unsigned)lemma_arr_[i].hz_str_len;
         hz_pos++) {
      // Get a Pinyin
      token = utf16_strtok(to_tokenize, &token_size, &to_tokenize);
      if (NULL == token) {
        free_resource();
        utf16_reader.close();
        return false;
      }

      assert(utf16_strlen(token) <= kMaxPinyinSize);

      utf16_strcpy_tochar(lemma_arr_[i].pinyin_str[hz_pos], token);

      format_spelling_str(lemma_arr_[i].pinyin_str[hz_pos]);

      // Put the pinyin to the spelling table
      if (!spl_table_->put_spelling(lemma_arr_[i].pinyin_str[hz_pos],
                                    lemma_arr_[i].freq)) {
        spelling_not_support = true;
        break;
      }
    }

    // The whole line must have been parsed fully, otherwise discard this one.
    token = utf16_strtok(to_tokenize, &token_size, &to_tokenize);
    if (spelling_not_support || NULL != token) {
      i--;
      continue;
    }
  }

  delete [] valid_hzs;
  utf16_reader.close();

  printf("read succesfully, lemma num: %d\n", lemma_num);

  return lemma_num;
}

bool DictBuilder::build_dict(const char *fn_raw,
                             const char *fn_validhzs,
                             DictTrie *dict_trie) {
  if (NULL == fn_raw || NULL == dict_trie)
    return false;

  lemma_num_ = read_raw_dict(fn_raw, fn_validhzs, 240000);
  if (0 == lemma_num_)
    return false;

  // Arrange the spelling table, and build a spelling tree
  // The size of an spelling. '\0' is included. If the spelling table is
  // initialized to calculate the spelling scores, the last char in the
  // spelling string will be score, and it is also included in spl_item_size.
  unsigned spl_item_size;
  unsigned spl_num;
  const char* spl_buf;
  spl_buf = spl_table_->arrange(&spl_item_size, &spl_num);
  if (NULL == spl_buf) {
    free_resource();
    return false;
  }

  SpellingTrie &spl_trie = SpellingTrie::get_instance();

  if (!spl_trie.construct(spl_buf, spl_item_size, spl_num,
                          spl_table_->get_score_amplifier(),
                          spl_table_->get_average_score())) {
    free_resource();
    return false;
  }

  printf("spelling tree construct successfully.\n");

  // Convert the spelling string to idxs
  for (unsigned i = 0; i < lemma_num_; i++) {
    for (unsigned hz_pos = 0; hz_pos < (unsigned)lemma_arr_[i].hz_str_len;
         hz_pos++) {
      uint16 spl_idxs[2];
      uint16 spl_start_pos[3];
      bool is_pre = true;
      int spl_idx_num =
        spl_parser_->splstr_to_idxs(lemma_arr_[i].pinyin_str[hz_pos],
                                    strlen(lemma_arr_[i].pinyin_str[hz_pos]),
                                    spl_idxs, spl_start_pos, 2, is_pre);
      assert(1 == spl_idx_num);

      if (spl_trie.is_half_id(spl_idxs[0])) {
        uint16 num = spl_trie.half_to_full(spl_idxs[0], spl_idxs);
        assert(0 != num);
      }
      lemma_arr_[i].spl_idx_arr[hz_pos] = spl_idxs[0];
    }
  }

  // Sort the lemma items according to the hanzi, and give each unique item a
  // id
  sort_lemmas_by_hz();

  scis_num_ = build_scis();

  // Construct the dict list
  dict_trie->dict_list_ = new DictList();
  bool dl_success = dict_trie->dict_list_->init_list(scis_, scis_num_,
                                                     lemma_arr_, lemma_num_);
  assert(dl_success);

  // Construct the NGram information
  NGram& ngram = NGram::get_instance();
  ngram.build_unigram(lemma_arr_, lemma_num_,
                      lemma_arr_[lemma_num_ - 1].idx_by_hz + 1);

  // sort the lemma items according to the spelling idx string
  myqsort(lemma_arr_, lemma_num_, sizeof(LemmaEntry), compare_py);

  get_top_lemmas();

#ifdef ___DO_STATISTICS___
  stat_init();
#endif

  lma_nds_used_num_le0_ = 1;  // The root node
  bool dt_success = construct_subset(static_cast<void*>(lma_nodes_le0_),
                                     lemma_arr_, 0, lemma_num_, 0);
  if (!dt_success) {
    free_resource();
    return false;
  }

#ifdef ___DO_STATISTICS___
  stat_print();
#endif

  // Move the node data and homo data to the DictTrie
  dict_trie->root_ = new LmaNodeLE0[lma_nds_used_num_le0_];
  dict_trie->nodes_ge1_ = new LmaNodeGE1[lma_nds_used_num_ge1_];
  unsigned lma_idx_num = homo_idx_num_eq1_ + homo_idx_num_gt1_ + top_lmas_num_;
  dict_trie->lma_idx_buf_ = new unsigned char[lma_idx_num * kLemmaIdSize];
  assert(NULL != dict_trie->root_);
  assert(NULL != dict_trie->lma_idx_buf_);
  dict_trie->lma_node_num_le0_ = lma_nds_used_num_le0_;
  dict_trie->lma_node_num_ge1_ = lma_nds_used_num_ge1_;
  dict_trie->lma_idx_buf_len_ = lma_idx_num * kLemmaIdSize;
  dict_trie->top_lmas_num_ = top_lmas_num_;

  memcpy(dict_trie->root_, lma_nodes_le0_,
         sizeof(LmaNodeLE0) * lma_nds_used_num_le0_);
  memcpy(dict_trie->nodes_ge1_, lma_nodes_ge1_,
         sizeof(LmaNodeGE1) * lma_nds_used_num_ge1_);

  for (unsigned pos = 0; pos < homo_idx_num_eq1_ + homo_idx_num_gt1_; pos++) {
    id_to_charbuf(dict_trie->lma_idx_buf_ + pos * kLemmaIdSize,
                  homo_idx_buf_[pos]);
  }

  for (unsigned pos = homo_idx_num_eq1_ + homo_idx_num_gt1_;
       pos < lma_idx_num; pos++) {
    LemmaIdType idx =
        top_lmas_[pos - homo_idx_num_eq1_ - homo_idx_num_gt1_].idx_by_hz;
    id_to_charbuf(dict_trie->lma_idx_buf_ + pos * kLemmaIdSize, idx);
  }

  if (kPrintDebug0) {
    printf("homo_idx_num_eq1_: %d\n", homo_idx_num_eq1_);
    printf("homo_idx_num_gt1_: %d\n", homo_idx_num_gt1_);
    printf("top_lmas_num_: %d\n", top_lmas_num_);
  }

  free_resource();

  if (kPrintDebug0) {
    printf("Building dict succeds\n");
  }
  return dt_success;
}

void DictBuilder::id_to_charbuf(unsigned char *buf, LemmaIdType id) {
  if (NULL == buf) return;
  for (unsigned pos = 0; pos < kLemmaIdSize; pos++) {
    (buf)[pos] = (unsigned char)(id >> (pos * 8));
  }
}

void DictBuilder::set_son_offset(LmaNodeGE1 *node, unsigned offset) {
  node->son_1st_off_l = static_cast<uint16>(offset);
  node->son_1st_off_h = static_cast<unsigned char>(offset >> 16);
}

void DictBuilder:: set_homo_id_buf_offset(LmaNodeGE1 *node, unsigned offset) {
  node->homo_idx_buf_off_l = static_cast<uint16>(offset);
  node->homo_idx_buf_off_h = static_cast<unsigned char>(offset >> 16);

}

// All spelling strings will be converted to upper case, except that
// spellings started with "ZH"/"CH"/"SH" will be converted to
// "Zh"/"Ch"/"Sh"
void DictBuilder::format_spelling_str(char *spl_str) {
  if (NULL == spl_str)
    return;

  uint16 pos = 0;
  while ('\0' != spl_str[pos]) {
    if (spl_str[pos] >= 'a' && spl_str[pos] <= 'z')
      spl_str[pos] = spl_str[pos] - 'a' + 'A';

    if (1 == pos && 'H' == spl_str[pos]) {
      if ('C' == spl_str[0] || 'S' == spl_str[0] || 'Z' == spl_str[0]) {
        spl_str[pos] = 'h';
      }
    }
    pos++;
  }
}

LemmaIdType DictBuilder::sort_lemmas_by_hz() {
  if (NULL == lemma_arr_ || 0 == lemma_num_)
    return 0;

  myqsort(lemma_arr_, lemma_num_, sizeof(LemmaEntry), cmp_lemma_entry_hzs);

  lemma_arr_[0].idx_by_hz = 1;
  LemmaIdType idx_max = 1;
  for (unsigned i = 1; i < lemma_num_; i++) {
    if (utf16_strcmp(lemma_arr_[i].hanzi_str, lemma_arr_[i-1].hanzi_str)) {
      idx_max++;
      lemma_arr_[i].idx_by_hz = idx_max;
    } else {
      idx_max++;
      lemma_arr_[i].idx_by_hz = idx_max;
    }
  }
  return idx_max + 1;
}

unsigned DictBuilder::build_scis() {
  if (NULL == scis_ || lemma_num_ * kMaxLemmaSize > scis_num_)
    return 0;

  SpellingTrie &spl_trie = SpellingTrie::get_instance();

  // This first one is blank, because id 0 is invalid.
  scis_[0].freq = 0;
  scis_[0].hz = 0;
  scis_[0].splid.full_splid = 0;
  scis_[0].splid.half_splid = 0;
  scis_num_ = 1;

  // Copy the hanzis to the buffer
  for (unsigned pos = 0; pos < lemma_num_; pos++) {
    unsigned hz_num = lemma_arr_[pos].hz_str_len;
    for (unsigned hzpos = 0; hzpos < hz_num; hzpos++) {
      scis_[scis_num_].hz = lemma_arr_[pos].hanzi_str[hzpos];
      scis_[scis_num_].splid.full_splid = lemma_arr_[pos].spl_idx_arr[hzpos];
      scis_[scis_num_].splid.half_splid =
          spl_trie.full_to_half(scis_[scis_num_].splid.full_splid);
      if (1 == hz_num)
        scis_[scis_num_].freq = lemma_arr_[pos].freq;
      else
        scis_[scis_num_].freq = 0.000001;
      scis_num_++;
    }
  }

  myqsort(scis_, scis_num_, sizeof(SingleCharItem), cmp_scis_hz_splid_freq);

  // Remove repeated items
  unsigned unique_scis_num = 1;
  for (unsigned pos = 1; pos < scis_num_; pos++) {
    if (scis_[pos].hz == scis_[pos - 1].hz &&
        scis_[pos].splid.full_splid == scis_[pos - 1].splid.full_splid)
      continue;
    scis_[unique_scis_num] = scis_[pos];
    scis_[unique_scis_num].splid.half_splid =
        spl_trie.full_to_half(scis_[pos].splid.full_splid);
    unique_scis_num++;
  }

  scis_num_ = unique_scis_num;

  // Update the lemma list.
  for (unsigned pos = 0; pos < lemma_num_; pos++) {
    unsigned hz_num = lemma_arr_[pos].hz_str_len;
    for (unsigned hzpos = 0; hzpos < hz_num; hzpos++) {
      SingleCharItem key;
      key.hz = lemma_arr_[pos].hanzi_str[hzpos];
      key.splid.full_splid = lemma_arr_[pos].spl_idx_arr[hzpos];
      key.splid.half_splid = spl_trie.full_to_half(key.splid.full_splid);

      SingleCharItem *found;
      found = static_cast<SingleCharItem*>(mybsearch(&key, scis_,
                                                     unique_scis_num,
                                                     sizeof(SingleCharItem),
                                                     cmp_scis_hz_splid));

      assert(found);

      lemma_arr_[pos].hanzi_scis_ids[hzpos] =
          static_cast<uint16>(found - scis_);
      lemma_arr_[pos].spl_idx_arr[hzpos] = found->splid.full_splid;
    }
  }

  return scis_num_;
}

bool DictBuilder::construct_subset(void* parent, LemmaEntry* lemma_arr,
                                   unsigned item_start, unsigned item_end,
                                   unsigned level) {
  if (level >= kMaxLemmaSize || item_end <= item_start)
    return false;

  // 1. Scan for how many sons
  unsigned parent_son_num = 0;
  // LemmaNode *son_1st = NULL;
  // parent.num_of_son = 0;

  LemmaEntry *lma_last_start = lemma_arr_ + item_start;
  uint16 spl_idx_node = lma_last_start->spl_idx_arr[level];

  // Scan for how many sons to be allocaed
  for (unsigned i = item_start + 1; i< item_end; i++) {
    LemmaEntry *lma_current = lemma_arr + i;
    uint16 spl_idx_current = lma_current->spl_idx_arr[level];
    if (spl_idx_current != spl_idx_node) {
      parent_son_num++;
      spl_idx_node = spl_idx_current;
    }
  }
  parent_son_num++;

#ifdef ___DO_STATISTICS___
  // Use to indicate whether all nodes of this layer have no son.
  bool allson_noson = true;

  assert(level < kMaxLemmaSize);
  if (parent_son_num > max_sonbuf_len_[level])
    max_sonbuf_len_[level] = parent_son_num;

  total_son_num_[level] += parent_son_num;
  total_sonbuf_num_[level] += 1;

  if (parent_son_num == 1)
    sonbufs_num1_++;
  else
    sonbufs_numgt1_++;
  total_lma_node_num_ += parent_son_num;
#endif

  // 2. Update the parent's information
  //    Update the parent's son list;
  LmaNodeLE0 *son_1st_le0 = NULL;  // only one of le0 or ge1 is used
  LmaNodeGE1 *son_1st_ge1 = NULL;  // only one of le0 or ge1 is used.
  if (0 == level) {                 // the parent is root
    (static_cast<LmaNodeLE0*>(parent))->son_1st_off =
      lma_nds_used_num_le0_;
    son_1st_le0 = lma_nodes_le0_ + lma_nds_used_num_le0_;
    lma_nds_used_num_le0_ += parent_son_num;

    assert(parent_son_num <= 65535);
    (static_cast<LmaNodeLE0*>(parent))->num_of_son =
      static_cast<uint16>(parent_son_num);
  } else if (1 == level) {  // the parent is a son of root
    (static_cast<LmaNodeLE0*>(parent))->son_1st_off =
      lma_nds_used_num_ge1_;
    son_1st_ge1 = lma_nodes_ge1_ + lma_nds_used_num_ge1_;
    lma_nds_used_num_ge1_ += parent_son_num;

    assert(parent_son_num <= 65535);
    (static_cast<LmaNodeLE0*>(parent))->num_of_son =
      static_cast<uint16>(parent_son_num);
  } else {
    set_son_offset((static_cast<LmaNodeGE1*>(parent)),
                   lma_nds_used_num_ge1_);
    son_1st_ge1 = lma_nodes_ge1_ + lma_nds_used_num_ge1_;
    lma_nds_used_num_ge1_ += parent_son_num;

    assert(parent_son_num <= 255);
    (static_cast<LmaNodeGE1*>(parent))->num_of_son =
      (unsigned char)parent_son_num;
  }

  // 3. Now begin to construct the son one by one
  unsigned son_pos = 0;

  lma_last_start = lemma_arr_ + item_start;
  spl_idx_node = lma_last_start->spl_idx_arr[level];

  unsigned homo_num = 0;
  if (lma_last_start->spl_idx_arr[level + 1] == 0)
    homo_num = 1;

  unsigned item_start_next = item_start;

  for (unsigned i = item_start + 1; i < item_end; i++) {
    LemmaEntry* lma_current = lemma_arr_ + i;
    uint16 spl_idx_current = lma_current->spl_idx_arr[level];

    if (spl_idx_current == spl_idx_node) {
      if (lma_current->spl_idx_arr[level + 1] == 0)
        homo_num++;
    } else {
      // Construct a node
      LmaNodeLE0 *node_cur_le0 = NULL;  // only one of them is valid
      LmaNodeGE1 *node_cur_ge1 = NULL;
      if (0 == level) {
        node_cur_le0 = son_1st_le0 + son_pos;
        node_cur_le0->spl_idx = spl_idx_node;
        node_cur_le0->homo_idx_buf_off = homo_idx_num_eq1_ + homo_idx_num_gt1_;
        node_cur_le0->son_1st_off = 0;
        homo_idx_num_eq1_ += homo_num;
      } else {
        node_cur_ge1 = son_1st_ge1 + son_pos;
        node_cur_ge1->spl_idx = spl_idx_node;

        set_homo_id_buf_offset(node_cur_ge1,
                               (homo_idx_num_eq1_ + homo_idx_num_gt1_));
        set_son_offset(node_cur_ge1, 0);
        homo_idx_num_gt1_ += homo_num;
      }

      if (homo_num > 0) {
        LemmaIdType* idx_buf = homo_idx_buf_ + homo_idx_num_eq1_ +
              homo_idx_num_gt1_ - homo_num;
        if (0 == level) {
          assert(homo_num <= 65535);
          node_cur_le0->num_of_homo = static_cast<uint16>(homo_num);
        } else {
          assert(homo_num <= 255);
          node_cur_ge1->num_of_homo = (unsigned char)homo_num;
        }

        for (unsigned homo_pos = 0; homo_pos < homo_num; homo_pos++) {
          idx_buf[homo_pos] = lemma_arr_[item_start_next + homo_pos].idx_by_hz;
        }

#ifdef ___DO_STATISTICS___
        if (homo_num > max_homobuf_len_[level])
          max_homobuf_len_[level] = homo_num;

        total_homo_num_[level] += homo_num;
#endif
      }

      if (i - item_start_next > homo_num) {
        void *next_parent;
        if (0 == level)
          next_parent = static_cast<void*>(node_cur_le0);
        else
          next_parent = static_cast<void*>(node_cur_ge1);
        construct_subset(next_parent, lemma_arr,
                         item_start_next + homo_num, i, level + 1);
#ifdef ___DO_STATISTICS___

        total_node_hasson_[level] += 1;
        allson_noson = false;
#endif
      }

      // for the next son
      lma_last_start = lma_current;
      spl_idx_node = spl_idx_current;
      item_start_next = i;
      homo_num = 0;
      if (lma_current->spl_idx_arr[level + 1] == 0)
        homo_num = 1;

      son_pos++;
    }
  }

  // 4. The last one to construct
  LmaNodeLE0 *node_cur_le0 = NULL;  // only one of them is valid
  LmaNodeGE1 *node_cur_ge1 = NULL;
  if (0 == level) {
    node_cur_le0 = son_1st_le0 + son_pos;
    node_cur_le0->spl_idx = spl_idx_node;
    node_cur_le0->homo_idx_buf_off = homo_idx_num_eq1_ + homo_idx_num_gt1_;
    node_cur_le0->son_1st_off = 0;
    homo_idx_num_eq1_ += homo_num;
  } else {
    node_cur_ge1 = son_1st_ge1 + son_pos;
    node_cur_ge1->spl_idx = spl_idx_node;

    set_homo_id_buf_offset(node_cur_ge1,
                           (homo_idx_num_eq1_ + homo_idx_num_gt1_));
    set_son_offset(node_cur_ge1, 0);
    homo_idx_num_gt1_ += homo_num;
  }

  if (homo_num > 0) {
    LemmaIdType* idx_buf = homo_idx_buf_ + homo_idx_num_eq1_ +
          homo_idx_num_gt1_ - homo_num;
    if (0 == level) {
      assert(homo_num <= 65535);
      node_cur_le0->num_of_homo = static_cast<uint16>(homo_num);
    } else {
      assert(homo_num <= 255);
      node_cur_ge1->num_of_homo = (unsigned char)homo_num;
    }

    for (unsigned homo_pos = 0; homo_pos < homo_num; homo_pos++) {
      idx_buf[homo_pos] = lemma_arr[item_start_next + homo_pos].idx_by_hz;
    }

#ifdef ___DO_STATISTICS___
    if (homo_num > max_homobuf_len_[level])
      max_homobuf_len_[level] = homo_num;

    total_homo_num_[level] += homo_num;
#endif
  }

  if (item_end - item_start_next > homo_num) {
    void *next_parent;
    if (0 == level)
      next_parent = static_cast<void*>(node_cur_le0);
    else
      next_parent = static_cast<void*>(node_cur_ge1);
    construct_subset(next_parent, lemma_arr,
                     item_start_next + homo_num, item_end, level + 1);
#ifdef ___DO_STATISTICS___

    total_node_hasson_[level] += 1;
    allson_noson = false;
#endif
  }

#ifdef ___DO_STATISTICS___
  if (allson_noson) {
    total_sonbuf_allnoson_[level] += 1;
    total_node_in_sonbuf_allnoson_[level] += parent_son_num;
  }
#endif

  assert(son_pos + 1 == parent_son_num);
  return true;
}

#ifdef ___DO_STATISTICS___
void DictBuilder::stat_init() {
  memset(max_sonbuf_len_, 0, sizeof(unsigned) * kMaxLemmaSize);
  memset(max_homobuf_len_, 0, sizeof(unsigned) * kMaxLemmaSize);
  memset(total_son_num_, 0, sizeof(unsigned) * kMaxLemmaSize);
  memset(total_node_hasson_, 0, sizeof(unsigned) * kMaxLemmaSize);
  memset(total_sonbuf_num_, 0, sizeof(unsigned) * kMaxLemmaSize);
  memset(total_sonbuf_allnoson_, 0, sizeof(unsigned) * kMaxLemmaSize);
  memset(total_node_in_sonbuf_allnoson_, 0, sizeof(unsigned) * kMaxLemmaSize);
  memset(total_homo_num_, 0, sizeof(unsigned) * kMaxLemmaSize);

  sonbufs_num1_ = 0;
  sonbufs_numgt1_ = 0;
  total_lma_node_num_ = 0;
}

void DictBuilder::stat_print() {
  printf("\n------------STAT INFO-------------\n");
  printf("[root is layer -1]\n");
  printf(".. max_sonbuf_len per layer(from layer 0):\n   ");
  for (unsigned i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", max_sonbuf_len_[i]);
  printf("-, \n");

  printf(".. max_homobuf_len per layer:\n   -, ");
  for (unsigned i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", max_homobuf_len_[i]);
  printf("\n");

  printf(".. total_son_num per layer:\n   ");
  for (unsigned i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_son_num_[i]);
  printf("-, \n");

  printf(".. total_node_hasson per layer:\n   1, ");
  for (unsigned i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_node_hasson_[i]);
  printf("\n");

  printf(".. total_sonbuf_num per layer:\n   ");
  for (unsigned i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_sonbuf_num_[i]);
  printf("-, \n");

  printf(".. total_sonbuf_allnoson per layer:\n   ");
  for (unsigned i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_sonbuf_allnoson_[i]);
  printf("-, \n");

  printf(".. total_node_in_sonbuf_allnoson per layer:\n   ");
  for (unsigned i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_node_in_sonbuf_allnoson_[i]);
  printf("-, \n");

  printf(".. total_homo_num per layer:\n   0, ");
  for (unsigned i = 0; i < kMaxLemmaSize; i++)
    printf("%d, ", total_homo_num_[i]);
  printf("\n");

  printf(".. son buf allocation number with only 1 son: %d\n", sonbufs_num1_);
  printf(".. son buf allocation number with more than 1 son: %d\n",
         sonbufs_numgt1_);
  printf(".. total lemma node number: %d\n", total_lma_node_num_ + 1);
}
#endif  // ___DO_STATISTICS___

#endif  // ___BUILD_MODEL___
}  // namespace ime_pinyin
