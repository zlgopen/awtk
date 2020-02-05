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
#include <stdlib.h>
#include <string.h>
#include "../include/dictlist.h"
#include "../include/mystdlib.h"
#include "../include/ngram.h"
#include "../include/searchutility.h"

namespace ime_pinyin {

DictList::DictList() {
  initialized_ = false;
  scis_num_ = 0;
  scis_hz_ = NULL;
  scis_splid_ = NULL;
  buf_ = NULL;
  spl_trie_ = SpellingTrie::get_cpinstance();

  assert(kMaxLemmaSize == 8);
  cmp_func_[0] = cmp_hanzis_1;
  cmp_func_[1] = cmp_hanzis_2;
  cmp_func_[2] = cmp_hanzis_3;
  cmp_func_[3] = cmp_hanzis_4;
  cmp_func_[4] = cmp_hanzis_5;
  cmp_func_[5] = cmp_hanzis_6;
  cmp_func_[6] = cmp_hanzis_7;
  cmp_func_[7] = cmp_hanzis_8;
}

DictList::~DictList() {
  free_resource();
}

bool DictList::alloc_resource(unsigned buf_size, unsigned scis_num) {
  // Allocate memory
  buf_ = static_cast<char16*>(malloc(buf_size * sizeof(char16)));
  if (NULL == buf_)
    return false;

  scis_num_ = scis_num;

  scis_hz_ = static_cast<char16*>(malloc(scis_num_ * sizeof(char16)));
  if (NULL == scis_hz_)
    return false;

  scis_splid_ = static_cast<SpellingId*>
      (malloc(scis_num_ * sizeof(SpellingId)));

  if (NULL == scis_splid_)
    return false;

  return true;
}

void DictList::free_resource() {
  if (NULL != buf_)
    free(buf_);
  buf_ = NULL;

  if (NULL != scis_hz_)
    free(scis_hz_);
  scis_hz_ = NULL;

  if (NULL != scis_splid_)
    free(scis_splid_);
  scis_splid_ = NULL;
}

#ifdef ___BUILD_MODEL___
bool DictList::init_list(const SingleCharItem *scis, unsigned scis_num,
                         const LemmaEntry *lemma_arr, unsigned lemma_num) {
  if (NULL == scis || 0 == scis_num || NULL == lemma_arr || 0 == lemma_num)
    return false;

  initialized_ = false;

  if (NULL != buf_)
    free(buf_);

  // calculate the size
  unsigned buf_size = calculate_size(lemma_arr, lemma_num);
  if (0 == buf_size)
    return false;

  if (!alloc_resource(buf_size, scis_num))
    return false;

  fill_scis(scis, scis_num);

  // Copy the related content from the array to inner buffer
  fill_list(lemma_arr, lemma_num);

  initialized_ = true;
  return true;
}

unsigned DictList::calculate_size(const LemmaEntry* lemma_arr, unsigned lemma_num) {
  unsigned last_hz_len = 0;
  unsigned list_size = 0;
  unsigned id_num = 0;

  for (unsigned i = 0; i < lemma_num; i++) {
    if (0 == i) {
      last_hz_len = lemma_arr[i].hz_str_len;

      assert(last_hz_len > 0);
      assert(lemma_arr[0].idx_by_hz == 1);

      id_num++;
      start_pos_[0] = 0;
      start_id_[0] = id_num;

      last_hz_len = 1;
      list_size += last_hz_len;
    } else {
      unsigned current_hz_len = lemma_arr[i].hz_str_len;

      assert(current_hz_len >= last_hz_len);

      if (current_hz_len == last_hz_len) {
          list_size += current_hz_len;
          id_num++;
      } else {
        for (unsigned len = last_hz_len; len < current_hz_len - 1; len++) {
          start_pos_[len] = start_pos_[len - 1];
          start_id_[len] = start_id_[len - 1];
        }

        start_pos_[current_hz_len - 1] = list_size;

        id_num++;
        start_id_[current_hz_len - 1] = id_num;

        last_hz_len = current_hz_len;
        list_size += current_hz_len;
      }
    }
  }

  for (unsigned i = last_hz_len; i <= kMaxLemmaSize; i++) {
    if (0 == i) {
      start_pos_[0] = 0;
      start_id_[0] = 1;
    } else {
      start_pos_[i] = list_size;
      start_id_[i] = id_num;
    }
  }

  return start_pos_[kMaxLemmaSize];
}

void DictList::fill_scis(const SingleCharItem *scis, unsigned scis_num) {
  assert(scis_num_ == scis_num);

  for (unsigned pos = 0; pos < scis_num_; pos++) {
    scis_hz_[pos] = scis[pos].hz;
    scis_splid_[pos] = scis[pos].splid;
  }
}

void DictList::fill_list(const LemmaEntry* lemma_arr, unsigned lemma_num) {
  unsigned current_pos = 0;

  utf16_strncpy(buf_, lemma_arr[0].hanzi_str,
                lemma_arr[0].hz_str_len);

  current_pos = lemma_arr[0].hz_str_len;

  unsigned id_num = 1;

  for (unsigned i = 1; i < lemma_num; i++) {
    utf16_strncpy(buf_ + current_pos, lemma_arr[i].hanzi_str,
                  lemma_arr[i].hz_str_len);

    id_num++;
    current_pos += lemma_arr[i].hz_str_len;
  }

  assert(current_pos == start_pos_[kMaxLemmaSize]);
  assert(id_num == start_id_[kMaxLemmaSize]);
}

char16* DictList::find_pos2_startedbyhz(char16 hz_char) {
  char16 *found_2w = static_cast<char16*>
                     (mybsearch(&hz_char, buf_ + start_pos_[1],
                                (start_pos_[2] - start_pos_[1]) / 2,
                                sizeof(char16) * 2, cmp_hanzis_1));
  if (NULL == found_2w)
    return NULL;

  while (found_2w > buf_ + start_pos_[1] && *found_2w == *(found_2w - 1))
    found_2w -= 2;

  return found_2w;
}
#endif  // ___BUILD_MODEL___

char16* DictList::find_pos_startedbyhzs(const char16 last_hzs[],
    unsigned word_len, int (*cmp_func)(const void *, const void *)) {
  char16 *found_w = static_cast<char16*>
                    (mybsearch(last_hzs, buf_ + start_pos_[word_len - 1],
                               (start_pos_[word_len] - start_pos_[word_len - 1])
                               / word_len,
                               sizeof(char16) * word_len, cmp_func));

  if (NULL == found_w)
    return NULL;

  while (found_w > buf_ + start_pos_[word_len -1] &&
         cmp_func(found_w, found_w - word_len) == 0)
    found_w -= word_len;

  return found_w;
}

unsigned DictList::predict(const char16 last_hzs[], uint16 hzs_len,
                         NPredictItem *npre_items, unsigned npre_max,
                         unsigned b4_used) {
  assert(hzs_len <= kMaxPredictSize && hzs_len > 0);

  // 1. Prepare work
  int (*cmp_func)(const void *, const void *) = cmp_func_[hzs_len - 1];

  NGram& ngram = NGram::get_instance();

  unsigned item_num = 0;

  // 2. Do prediction
  for (uint16 pre_len = 1; pre_len <= kMaxPredictSize + 1 - hzs_len;
       pre_len++) {
    uint16 word_len = hzs_len + pre_len;
    char16 *w_buf = find_pos_startedbyhzs(last_hzs, word_len, cmp_func);
    if (NULL == w_buf)
      continue;
    while (w_buf < buf_ + start_pos_[word_len] &&
           cmp_func(w_buf, last_hzs) == 0 &&
           item_num < npre_max) {
      memset(npre_items + item_num, 0, sizeof(NPredictItem));
      utf16_strncpy(npre_items[item_num].pre_hzs, w_buf + hzs_len, pre_len);
      npre_items[item_num].psb =
        ngram.get_uni_psb((unsigned)(w_buf - buf_ - start_pos_[word_len - 1])
                          / word_len + start_id_[word_len - 1]);
      npre_items[item_num].his_len = hzs_len;
      item_num++;
      w_buf += word_len;
    }
  }

  unsigned new_num = 0;
  for (unsigned i = 0; i < item_num; i++) {
    // Try to find it in the existing items
    unsigned e_pos;
    for (e_pos = 1; e_pos <= b4_used; e_pos++) {
      if (utf16_strncmp((*(npre_items - e_pos)).pre_hzs, npre_items[i].pre_hzs,
                        kMaxPredictSize) == 0)
        break;
    }
    if (e_pos <= b4_used)
      continue;

    // If not found, append it to the buffer
    npre_items[new_num] = npre_items[i];
    new_num++;
  }

  return new_num;
}

uint16 DictList::get_lemma_str(LemmaIdType id_lemma, char16 *str_buf,
                               uint16 str_max) {
  if (!initialized_ || id_lemma >= start_id_[kMaxLemmaSize] || NULL == str_buf
      || str_max <= 1)
    return 0;

  // Find the range
  for (uint16 i = 0; i < kMaxLemmaSize; i++) {
    if (i + 1 > str_max - 1)
      return 0;
    if (start_id_[i] <= id_lemma && start_id_[i + 1] > id_lemma) {
      unsigned id_span = id_lemma - start_id_[i];

      uint16 *buf = buf_ + start_pos_[i] + id_span * (i + 1);
      for (uint16 len = 0; len <= i; len++) {
        str_buf[len] = buf[len];
      }
      str_buf[i+1] = (char16)'\0';
      return i + 1;
    }
  }
  return 0;
}

uint16 DictList::get_splids_for_hanzi(char16 hanzi, uint16 half_splid,
                                      uint16 *splids, uint16 max_splids) {
  char16 *hz_found = static_cast<char16*>
      (mybsearch(&hanzi, scis_hz_, scis_num_, sizeof(char16), cmp_hanzis_1));
  assert(NULL != hz_found && hanzi == *hz_found);

  // Move to the first one.
  while (hz_found > scis_hz_ && hanzi == *(hz_found - 1))
    hz_found--;

  // First try to found if strict comparison result is not zero.
  char16 *hz_f = hz_found;
  bool strict = false;
  while (hz_f < scis_hz_ + scis_num_ && hanzi == *hz_f) {
    uint16 pos = hz_f - scis_hz_;
    if (0 == half_splid || scis_splid_[pos].half_splid == half_splid) {
      strict = true;
    }
    hz_f++;
  }

  uint16 found_num = 0;
  while (hz_found < scis_hz_ + scis_num_ && hanzi == *hz_found) {
    uint16 pos = hz_found - scis_hz_;
    if (0 == half_splid ||
        (strict && scis_splid_[pos].half_splid == half_splid) ||
        (!strict && spl_trie_->half_full_compatible(half_splid,
        scis_splid_[pos].full_splid))) {
      assert(found_num + 1 < max_splids);
      splids[found_num] = scis_splid_[pos].full_splid;
      found_num++;
    }
    hz_found++;
  }

  return found_num;
}

LemmaIdType DictList::get_lemma_id(const char16 *str, uint16 str_len) {
  if (NULL == str || str_len > kMaxLemmaSize)
    return 0;

  char16 *found = find_pos_startedbyhzs(str, str_len, cmp_func_[str_len - 1]);
  if (NULL == found)
    return 0;

  assert(found > buf_);
  assert(static_cast<unsigned>(found - buf_) >= start_pos_[str_len - 1]);
  return static_cast<LemmaIdType>
      (start_id_[str_len - 1] +
       (found - buf_ - start_pos_[str_len - 1]) / str_len);
}

void DictList::convert_to_hanzis(char16 *str, uint16 str_len) {
  assert(NULL != str);

  for (uint16 str_pos = 0; str_pos < str_len; str_pos++) {
    str[str_pos] = scis_hz_[str[str_pos]];
  }
}

void DictList::convert_to_scis_ids(char16 *str, uint16 str_len) {
  assert(NULL != str);

  for (uint16 str_pos = 0; str_pos < str_len; str_pos++) {
    str[str_pos] = 0x100;
  }
}

bool DictList::save_list(FILE *fp) {
  if (!initialized_ || NULL == fp)
    return false;

  if (NULL == buf_ || 0 == start_pos_[kMaxLemmaSize] ||
      NULL == scis_hz_ || NULL == scis_splid_ || 0 == scis_num_)
    return false;

  if (fwrite(&scis_num_, sizeof(unsigned), 1, fp) != 1)
    return false;

  if (fwrite(start_pos_, sizeof(unsigned), kMaxLemmaSize + 1, fp) !=
      kMaxLemmaSize + 1)
    return false;

  if (fwrite(start_id_, sizeof(unsigned), kMaxLemmaSize + 1, fp) !=
      kMaxLemmaSize + 1)
    return false;

  if (fwrite(scis_hz_, sizeof(char16), scis_num_, fp) != scis_num_)
    return false;

  if (fwrite(scis_splid_, sizeof(SpellingId), scis_num_, fp) != scis_num_)
    return false;

  if (fwrite(buf_, sizeof(char16), start_pos_[kMaxLemmaSize], fp) !=
      start_pos_[kMaxLemmaSize])
    return false;

  return true;
}

bool DictList::load_list(FILE *fp) {
  if (NULL == fp)
    return false;

  initialized_ = false;

  if (fread(&scis_num_, sizeof(unsigned), 1, fp) != 1)
    return false;

  if (fread(start_pos_, sizeof(unsigned), kMaxLemmaSize + 1, fp) !=
      kMaxLemmaSize + 1)
    return false;

  if (fread(start_id_, sizeof(unsigned), kMaxLemmaSize + 1, fp) !=
      kMaxLemmaSize + 1)
    return false;

  free_resource();

  if (!alloc_resource(start_pos_[kMaxLemmaSize], scis_num_))
    return false;

  if (fread(scis_hz_, sizeof(char16), scis_num_, fp) != scis_num_)
    return false;

  if (fread(scis_splid_, sizeof(SpellingId), scis_num_, fp) != scis_num_)
    return false;

  if (fread(buf_, sizeof(char16), start_pos_[kMaxLemmaSize], fp) !=
      start_pos_[kMaxLemmaSize])
    return false;

  initialized_ = true;
  return true;
}
}  // namespace ime_pinyin
