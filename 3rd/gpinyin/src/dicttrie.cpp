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

#include "file.h"
#include <assert.h>
#include <string.h>
#include "../include/dicttrie.h"
#include "../include/lpicache.h"
#include "../include/mystdlib.h"
#include "../include/ngram.h"

namespace ime_pinyin {

DictTrie::DictTrie() {
  spl_trie_ = SpellingTrie::get_cpinstance();

  root_ = NULL;
  splid_le0_index_ = NULL;
  lma_node_num_le0_ = 0;
  nodes_ge1_ = NULL;
  lma_node_num_ge1_ = 0;
  lma_idx_buf_ = NULL;
  lma_idx_buf_len_ = 0;
  total_lma_num_ = 0;
  top_lmas_num_ = 0;
  dict_list_ = NULL;

  parsing_marks_ = NULL;
  mile_stones_ = NULL;
  reset_milestones(0, kFirstValidMileStoneHandle);
}

DictTrie::~DictTrie() {
  NGram& ngram = NGram::get_instance();
  SpellingTrie& spl_trie = SpellingTrie::get_instance();
  free_resource(true);
  TKMEM_FREE(lma_idx_buf_);
  delete &ngram;
  delete &spl_trie;
}

void DictTrie::free_resource(bool free_dict_list) {
  if (NULL != root_) TKMEM_FREE(root_);
  root_ = NULL;

  if (NULL != splid_le0_index_) TKMEM_FREE(splid_le0_index_);
  splid_le0_index_ = NULL;

  if (NULL != nodes_ge1_) TKMEM_FREE(nodes_ge1_);
  nodes_ge1_ = NULL;

  if (NULL != nodes_ge1_) TKMEM_FREE(nodes_ge1_);
  nodes_ge1_ = NULL;

  if (free_dict_list) {
    if (NULL != dict_list_) {
      delete dict_list_;
    }
    dict_list_ = NULL;
  }

  if (parsing_marks_) delete[] parsing_marks_;
  parsing_marks_ = NULL;

  if (mile_stones_) delete[] mile_stones_;
  mile_stones_ = NULL;

  reset_milestones(0, kFirstValidMileStoneHandle);
}

inline unsigned DictTrie::get_son_offset(const LmaNodeGE1* node) {
  return ((unsigned)node->son_1st_off_l + ((unsigned)node->son_1st_off_h << 16));
}

inline unsigned DictTrie::get_homo_idx_buf_offset(const LmaNodeGE1* node) {
  return ((unsigned)node->homo_idx_buf_off_l + ((unsigned)node->homo_idx_buf_off_h << 16));
}

inline LemmaIdType DictTrie::get_lemma_id(unsigned id_offset) {
  LemmaIdType id = 0;
  for (uint16 pos = kLemmaIdSize - 1; pos > 0; pos--)
    id = (id << 8) + lma_idx_buf_[id_offset * kLemmaIdSize + pos];
  id = (id << 8) + lma_idx_buf_[id_offset * kLemmaIdSize];
  return id;
}

bool DictTrie::load_dict(FILE* fp) {
  if (NULL == fp) return false;

  if (fread(&lma_node_num_le0_, sizeof(unsigned), 1, fp) != 1) return false;

  if (fread(&lma_node_num_ge1_, sizeof(unsigned), 1, fp) != 1) return false;

  if (fread(&lma_idx_buf_len_, sizeof(unsigned), 1, fp) != 1) return false;

  if (fread(&top_lmas_num_, sizeof(unsigned), 1, fp) != 1 || top_lmas_num_ >= lma_idx_buf_len_)
    return false;

  free_resource(false);

  root_ = static_cast<LmaNodeLE0*>(TKMEM_ALLOC(lma_node_num_le0_ * sizeof(LmaNodeLE0)));
  nodes_ge1_ = static_cast<LmaNodeGE1*>(TKMEM_ALLOC(lma_node_num_ge1_ * sizeof(LmaNodeGE1)));
  lma_idx_buf_ = (unsigned char*)TKMEM_ALLOC(lma_idx_buf_len_);
  total_lma_num_ = lma_idx_buf_len_ / kLemmaIdSize;

  unsigned buf_size = SpellingTrie::get_instance().get_spelling_num() + 1;
  assert(lma_node_num_le0_ <= buf_size);
  splid_le0_index_ = static_cast<uint16*>(TKMEM_ALLOC(buf_size * sizeof(uint16)));

  // Init the space for parsing.
  parsing_marks_ = new ParsingMark[kMaxParsingMark];
  mile_stones_ = new MileStone[kMaxMileStone];
  reset_milestones(0, kFirstValidMileStoneHandle);

  if (NULL == root_ || NULL == nodes_ge1_ || NULL == lma_idx_buf_ || NULL == splid_le0_index_ ||
      NULL == parsing_marks_ || NULL == mile_stones_) {
    free_resource(false);
    return false;
  }

  if (fread(root_, sizeof(LmaNodeLE0), lma_node_num_le0_, fp) != lma_node_num_le0_) return false;

  if (fread(nodes_ge1_, sizeof(LmaNodeGE1), lma_node_num_ge1_, fp) != lma_node_num_ge1_)
    return false;

  if (fread(lma_idx_buf_, sizeof(unsigned char), lma_idx_buf_len_, fp) != lma_idx_buf_len_)
    return false;

  // The quick index for the first level sons
  uint16 last_splid = kFullSplIdStart;
  unsigned last_pos = 0;
  for (unsigned i = 1; i < lma_node_num_le0_; i++) {
    for (uint16 splid = last_splid; splid < root_[i].spl_idx; splid++)
      splid_le0_index_[splid - kFullSplIdStart] = last_pos;

    splid_le0_index_[root_[i].spl_idx - kFullSplIdStart] = static_cast<uint16>(i);
    last_splid = root_[i].spl_idx;
    last_pos = i;
  }

  for (uint16 splid = last_splid + 1; splid < buf_size + kFullSplIdStart; splid++) {
    assert(static_cast<unsigned>(splid - kFullSplIdStart) < buf_size);
    splid_le0_index_[splid - kFullSplIdStart] = last_pos + 1;
  }

  return true;
}

bool DictTrie::load_dict(const char* filename, LemmaIdType start_id, LemmaIdType end_id) {
  if (NULL == filename || end_id <= start_id) return false;

  FILE* fp = fopen(filename, "rb");
  if (NULL == fp) return false;

  free_resource(true);

  dict_list_ = new DictList();
  if (NULL == dict_list_) {
    fclose(fp);
    return false;
  }

  SpellingTrie& spl_trie = SpellingTrie::get_instance();
  NGram& ngram = NGram::get_instance();

  if (!spl_trie.load_spl_trie(fp) || !dict_list_->load_list(fp) || !load_dict(fp) ||
      !ngram.load_ngram(fp) || total_lma_num_ > end_id - start_id + 1) {
    free_resource(true);
    fclose(fp);
    return false;
  }

  fclose(fp);
  return true;
}

bool DictTrie::load_dict_fd(int sys_fd, long start_offset, long length, LemmaIdType start_id,
                            LemmaIdType end_id) {
  return true;
}

unsigned DictTrie::fill_lpi_buffer(LmaPsbItem lpi_items[], unsigned lpi_max, LmaNodeLE0* node) {
  unsigned lpi_num = 0;
  NGram& ngram = NGram::get_instance();
  for (unsigned homo = 0; homo < (unsigned)node->num_of_homo; homo++) {
    lpi_items[lpi_num].id = get_lemma_id(node->homo_idx_buf_off + homo);
    lpi_items[lpi_num].lma_len = 1;
    lpi_items[lpi_num].psb = static_cast<LmaScoreType>(ngram.get_uni_psb(lpi_items[lpi_num].id));
    lpi_num++;
    if (lpi_num >= lpi_max) break;
  }

  return lpi_num;
}

unsigned DictTrie::fill_lpi_buffer(LmaPsbItem lpi_items[], unsigned lpi_max, unsigned homo_buf_off,
                                   LmaNodeGE1* node, uint16 lma_len) {
  unsigned lpi_num = 0;
  NGram& ngram = NGram::get_instance();
  for (unsigned homo = 0; homo < (unsigned)node->num_of_homo; homo++) {
    lpi_items[lpi_num].id = get_lemma_id(homo_buf_off + homo);
    lpi_items[lpi_num].lma_len = lma_len;
    lpi_items[lpi_num].psb = static_cast<LmaScoreType>(ngram.get_uni_psb(lpi_items[lpi_num].id));
    lpi_num++;
    if (lpi_num >= lpi_max) break;
  }

  return lpi_num;
}

void DictTrie::reset_milestones(uint16 from_step, MileStoneHandle from_handle) {
  if (0 == from_step) {
    parsing_marks_pos_ = 0;
    mile_stones_pos_ = kFirstValidMileStoneHandle;
  } else {
    if (from_handle > 0 && from_handle < mile_stones_pos_) {
      mile_stones_pos_ = from_handle;

      MileStone* mile_stone = mile_stones_ + from_handle;
      parsing_marks_pos_ = mile_stone->mark_start;
    }
  }
}

MileStoneHandle DictTrie::extend_dict(MileStoneHandle from_handle, const DictExtPara* dep,
                                      LmaPsbItem* lpi_items, unsigned lpi_max, unsigned* lpi_num) {
  if (NULL == dep) return 0;

  // from LmaNodeLE0 (root) to LmaNodeLE0
  if (0 == from_handle) {
    assert(0 == dep->splids_extended);
    return extend_dict0(from_handle, dep, lpi_items, lpi_max, lpi_num);
  }

  // from LmaNodeLE0 to LmaNodeGE1
  if (1 == dep->splids_extended) return extend_dict1(from_handle, dep, lpi_items, lpi_max, lpi_num);

  // From LmaNodeGE1 to LmaNodeGE1
  return extend_dict2(from_handle, dep, lpi_items, lpi_max, lpi_num);
}

MileStoneHandle DictTrie::extend_dict0(MileStoneHandle from_handle, const DictExtPara* dep,
                                       LmaPsbItem* lpi_items, unsigned lpi_max, unsigned* lpi_num) {
  assert(NULL != dep && 0 == from_handle);
  *lpi_num = 0;
  MileStoneHandle ret_handle = 0;

  uint16 splid = dep->splids[dep->splids_extended];
  uint16 id_start = dep->id_start;
  uint16 id_num = dep->id_num;

  LpiCache& lpi_cache = LpiCache::get_instance();
  bool cached = lpi_cache.is_cached(splid);

  // 2. Begin exgtending
  // 2.1 Get the LmaPsbItem list
  LmaNodeLE0* node = root_;
  unsigned son_start = splid_le0_index_[id_start - kFullSplIdStart];
  unsigned son_end = splid_le0_index_[id_start + id_num - kFullSplIdStart];
  for (unsigned son_pos = son_start; son_pos < son_end; son_pos++) {
    assert(1 == node->son_1st_off);
    LmaNodeLE0* son = root_ + son_pos;
    assert(son->spl_idx >= id_start && son->spl_idx < id_start + id_num);

    if (!cached && *lpi_num < lpi_max) {
      bool need_lpi = true;
      if (spl_trie_->is_half_id_yunmu(splid) && son_pos != son_start) need_lpi = false;

      if (need_lpi) *lpi_num += fill_lpi_buffer(lpi_items + (*lpi_num), lpi_max - *lpi_num, son);
    }

    // If necessary, fill in a new mile stone.
    if (son->spl_idx == id_start) {
      if (mile_stones_pos_ < kMaxMileStone && parsing_marks_pos_ < kMaxParsingMark) {
        parsing_marks_[parsing_marks_pos_].node_offset = son_pos;
        parsing_marks_[parsing_marks_pos_].node_num = id_num;
        mile_stones_[mile_stones_pos_].mark_start = parsing_marks_pos_;
        mile_stones_[mile_stones_pos_].mark_num = 1;
        ret_handle = mile_stones_pos_;
        parsing_marks_pos_++;
        mile_stones_pos_++;
      }
    }

    if (son->spl_idx >= id_start + id_num - 1) break;
  }

  //  printf("----- parsing marks: %d, mile stone: %d \n", parsing_marks_pos_,
  //      mile_stones_pos_);
  return ret_handle;
}

MileStoneHandle DictTrie::extend_dict1(MileStoneHandle from_handle, const DictExtPara* dep,
                                       LmaPsbItem* lpi_items, unsigned lpi_max, unsigned* lpi_num) {
  assert(NULL != dep && from_handle > 0 && from_handle < mile_stones_pos_);

  MileStoneHandle ret_handle = 0;

  // 1. If this is a half Id, get its corresponding full starting Id and
  // number of full Id.
  unsigned ret_val = 0;

  uint16 id_start = dep->id_start;
  uint16 id_num = dep->id_num;

  // 2. Begin extending.
  MileStone* mile_stone = mile_stones_ + from_handle;

  for (uint16 h_pos = 0; h_pos < mile_stone->mark_num; h_pos++) {
    ParsingMark p_mark = parsing_marks_[mile_stone->mark_start + h_pos];
    uint16 ext_num = p_mark.node_num;
    for (uint16 ext_pos = 0; ext_pos < ext_num; ext_pos++) {
      LmaNodeLE0* node = root_ + p_mark.node_offset + ext_pos;
      unsigned found_start = 0;
      unsigned found_num = 0;
      for (unsigned son_pos = 0; son_pos < (unsigned)node->num_of_son; son_pos++) {
        assert(node->son_1st_off <= lma_node_num_ge1_);
        LmaNodeGE1* son = nodes_ge1_ + node->son_1st_off + son_pos;
        if (son->spl_idx >= id_start && son->spl_idx < id_start + id_num) {
          if (*lpi_num < lpi_max) {
            unsigned homo_buf_off = get_homo_idx_buf_offset(son);
            *lpi_num +=
                fill_lpi_buffer(lpi_items + (*lpi_num), lpi_max - *lpi_num, homo_buf_off, son, 2);
          }

          // If necessary, fill in the new DTMI
          if (0 == found_num) {
            found_start = son_pos;
          }
          found_num++;
        }
        if (son->spl_idx >= id_start + id_num - 1 || son_pos == (unsigned)node->num_of_son - 1) {
          if (found_num > 0) {
            if (mile_stones_pos_ < kMaxMileStone && parsing_marks_pos_ < kMaxParsingMark) {
              parsing_marks_[parsing_marks_pos_].node_offset = node->son_1st_off + found_start;
              parsing_marks_[parsing_marks_pos_].node_num = found_num;
              if (0 == ret_val) mile_stones_[mile_stones_pos_].mark_start = parsing_marks_pos_;
              parsing_marks_pos_++;
            }

            ret_val++;
          }
          break;
        }  // for son_pos
      }    // for ext_pos
    }      // for h_pos
  }

  if (ret_val > 0) {
    mile_stones_[mile_stones_pos_].mark_num = ret_val;
    ret_handle = mile_stones_pos_;
    mile_stones_pos_++;
    ret_val = 1;
  }

  //  printf("----- parsing marks: %d, mile stone: %d \n", parsing_marks_pos_,
  //         mile_stones_pos_);
  return ret_handle;
}

MileStoneHandle DictTrie::extend_dict2(MileStoneHandle from_handle, const DictExtPara* dep,
                                       LmaPsbItem* lpi_items, unsigned lpi_max, unsigned* lpi_num) {
  assert(NULL != dep && from_handle > 0 && from_handle < mile_stones_pos_);

  MileStoneHandle ret_handle = 0;

  // 1. If this is a half Id, get its corresponding full starting Id and
  // number of full Id.
  unsigned ret_val = 0;

  uint16 id_start = dep->id_start;
  uint16 id_num = dep->id_num;

  // 2. Begin extending.
  MileStone* mile_stone = mile_stones_ + from_handle;

  for (uint16 h_pos = 0; h_pos < mile_stone->mark_num; h_pos++) {
    ParsingMark p_mark = parsing_marks_[mile_stone->mark_start + h_pos];
    uint16 ext_num = p_mark.node_num;
    for (uint16 ext_pos = 0; ext_pos < ext_num; ext_pos++) {
      LmaNodeGE1* node = nodes_ge1_ + p_mark.node_offset + ext_pos;
      unsigned found_start = 0;
      unsigned found_num = 0;

      for (unsigned son_pos = 0; son_pos < (unsigned)node->num_of_son; son_pos++) {
        assert(node->son_1st_off_l > 0 || node->son_1st_off_h > 0);
        LmaNodeGE1* son = nodes_ge1_ + get_son_offset(node) + son_pos;
        if (son->spl_idx >= id_start && son->spl_idx < id_start + id_num) {
          if (*lpi_num < lpi_max) {
            unsigned homo_buf_off = get_homo_idx_buf_offset(son);
            *lpi_num += fill_lpi_buffer(lpi_items + (*lpi_num), lpi_max - *lpi_num, homo_buf_off,
                                        son, dep->splids_extended + 1);
          }

          // If necessary, fill in the new DTMI
          if (0 == found_num) {
            found_start = son_pos;
          }
          found_num++;
        }
        if (son->spl_idx >= id_start + id_num - 1 || son_pos == (unsigned)node->num_of_son - 1) {
          if (found_num > 0) {
            if (mile_stones_pos_ < kMaxMileStone && parsing_marks_pos_ < kMaxParsingMark) {
              parsing_marks_[parsing_marks_pos_].node_offset = get_son_offset(node) + found_start;
              parsing_marks_[parsing_marks_pos_].node_num = found_num;
              if (0 == ret_val) mile_stones_[mile_stones_pos_].mark_start = parsing_marks_pos_;
              parsing_marks_pos_++;
            }

            ret_val++;
          }
          break;
        }
      }  // for son_pos
    }    // for ext_pos
  }      // for h_pos

  if (ret_val > 0) {
    mile_stones_[mile_stones_pos_].mark_num = ret_val;
    ret_handle = mile_stones_pos_;
    mile_stones_pos_++;
  }

  // printf("----- parsing marks: %d, mile stone: %d \n", parsing_marks_pos_,
  //        mile_stones_pos_);
  return ret_handle;
}

bool DictTrie::try_extend(const uint16* splids, uint16 splid_num, LemmaIdType id_lemma) {
  if (0 == splid_num || NULL == splids) return false;

  void* node = root_ + splid_le0_index_[splids[0] - kFullSplIdStart];

  for (uint16 pos = 1; pos < splid_num; pos++) {
    if (1 == pos) {
      LmaNodeLE0* node_le0 = reinterpret_cast<LmaNodeLE0*>(node);
      LmaNodeGE1* node_son;
      uint16 son_pos;
      for (son_pos = 0; son_pos < static_cast<uint16>(node_le0->num_of_son); son_pos++) {
        assert(node_le0->son_1st_off <= lma_node_num_ge1_);
        node_son = nodes_ge1_ + node_le0->son_1st_off + son_pos;
        if (node_son->spl_idx == splids[pos]) break;
      }
      if (son_pos < node_le0->num_of_son)
        node = reinterpret_cast<void*>(node_son);
      else
        return false;
    } else {
      LmaNodeGE1* node_ge1 = reinterpret_cast<LmaNodeGE1*>(node);
      LmaNodeGE1* node_son;
      uint16 son_pos;
      for (son_pos = 0; son_pos < static_cast<uint16>(node_ge1->num_of_son); son_pos++) {
        assert(node_ge1->son_1st_off_l > 0 || node_ge1->son_1st_off_h > 0);
        node_son = nodes_ge1_ + get_son_offset(node_ge1) + son_pos;
        if (node_son->spl_idx == splids[pos]) break;
      }
      if (son_pos < node_ge1->num_of_son)
        node = reinterpret_cast<void*>(node_son);
      else
        return false;
    }
  }

  if (1 == splid_num) {
    LmaNodeLE0* node_le0 = reinterpret_cast<LmaNodeLE0*>(node);
    unsigned num_of_homo = (unsigned)node_le0->num_of_homo;
    for (unsigned homo_pos = 0; homo_pos < num_of_homo; homo_pos++) {
      LemmaIdType id_this = get_lemma_id(node_le0->homo_idx_buf_off + homo_pos);
      char16 str[2];
      get_lemma_str(id_this, str, 2);
      if (id_this == id_lemma) return true;
    }
  } else {
    LmaNodeGE1* node_ge1 = reinterpret_cast<LmaNodeGE1*>(node);
    unsigned num_of_homo = (unsigned)node_ge1->num_of_homo;
    for (unsigned homo_pos = 0; homo_pos < num_of_homo; homo_pos++) {
      unsigned node_homo_off = get_homo_idx_buf_offset(node_ge1);
      if (get_lemma_id(node_homo_off + homo_pos) == id_lemma) return true;
    }
  }

  return false;
}

unsigned DictTrie::get_lpis(const uint16* splid_str, uint16 splid_str_len, LmaPsbItem* lma_buf,
                            unsigned max_lma_buf) {
  if (splid_str_len > kMaxLemmaSize) return 0;

#define MAX_EXTENDBUF_LEN 200

  unsigned* node_buf1[MAX_EXTENDBUF_LEN];  // use unsigned for data alignment
  unsigned* node_buf2[MAX_EXTENDBUF_LEN];
  LmaNodeLE0** node_fr_le0 = reinterpret_cast<LmaNodeLE0**>(node_buf1);  // Nodes from.
  LmaNodeLE0** node_to_le0 = reinterpret_cast<LmaNodeLE0**>(node_buf2);  // Nodes to.
  LmaNodeGE1** node_fr_ge1 = NULL;
  LmaNodeGE1** node_to_ge1 = NULL;
  unsigned node_fr_num = 1;
  unsigned node_to_num = 0;
  node_fr_le0[0] = root_;
  if (NULL == node_fr_le0[0]) return 0;

  unsigned spl_pos = 0;

  while (spl_pos < splid_str_len) {
    uint16 id_num = 1;
    uint16 id_start = splid_str[spl_pos];
    // If it is a half id
    if (spl_trie_->is_half_id(splid_str[spl_pos])) {
      id_num = spl_trie_->half_to_full(splid_str[spl_pos], &id_start);
      assert(id_num > 0);
    }

    // Extend the nodes
    if (0 == spl_pos) {  // From LmaNodeLE0 (root) to LmaNodeLE0 nodes
      for (unsigned node_fr_pos = 0; node_fr_pos < node_fr_num; node_fr_pos++) {
        LmaNodeLE0* node = node_fr_le0[node_fr_pos];
        assert(node == root_ && 1 == node_fr_num);
        unsigned son_start = splid_le0_index_[id_start - kFullSplIdStart];
        unsigned son_end = splid_le0_index_[id_start + id_num - kFullSplIdStart];
        for (unsigned son_pos = son_start; son_pos < son_end; son_pos++) {
          assert(1 == node->son_1st_off);
          LmaNodeLE0* node_son = root_ + son_pos;
          assert(node_son->spl_idx >= id_start && node_son->spl_idx < id_start + id_num);
          if (node_to_num < MAX_EXTENDBUF_LEN) {
            node_to_le0[node_to_num] = node_son;
            node_to_num++;
          }
          // id_start + id_num - 1 is the last one, which has just been
          // recorded.
          if (node_son->spl_idx >= id_start + id_num - 1) break;
        }
      }

      spl_pos++;
      if (spl_pos >= splid_str_len || node_to_num == 0) break;
      // Prepare the nodes for next extending
      // next time, from LmaNodeLE0 to LmaNodeGE1
      LmaNodeLE0** node_tmp = node_fr_le0;
      node_fr_le0 = node_to_le0;
      node_to_le0 = NULL;
      node_to_ge1 = reinterpret_cast<LmaNodeGE1**>(node_tmp);
    } else if (1 == spl_pos) {  // From LmaNodeLE0 to LmaNodeGE1 nodes
      for (unsigned node_fr_pos = 0; node_fr_pos < node_fr_num; node_fr_pos++) {
        LmaNodeLE0* node = node_fr_le0[node_fr_pos];
        for (unsigned son_pos = 0; son_pos < (unsigned)node->num_of_son; son_pos++) {
          assert(node->son_1st_off <= lma_node_num_ge1_);
          LmaNodeGE1* node_son = nodes_ge1_ + node->son_1st_off + son_pos;
          if (node_son->spl_idx >= id_start && node_son->spl_idx < id_start + id_num) {
            if (node_to_num < MAX_EXTENDBUF_LEN) {
              node_to_ge1[node_to_num] = node_son;
              node_to_num++;
            }
          }
          // id_start + id_num - 1 is the last one, which has just been
          // recorded.
          if (node_son->spl_idx >= id_start + id_num - 1) break;
        }
      }

      spl_pos++;
      if (spl_pos >= splid_str_len || node_to_num == 0) break;
      // Prepare the nodes for next extending
      // next time, from LmaNodeGE1 to LmaNodeGE1
      node_fr_ge1 = node_to_ge1;
      node_to_ge1 = reinterpret_cast<LmaNodeGE1**>(node_fr_le0);
      node_fr_le0 = NULL;
      node_to_le0 = NULL;
    } else {  // From LmaNodeGE1 to LmaNodeGE1 nodes
      for (unsigned node_fr_pos = 0; node_fr_pos < node_fr_num; node_fr_pos++) {
        LmaNodeGE1* node = node_fr_ge1[node_fr_pos];
        for (unsigned son_pos = 0; son_pos < (unsigned)node->num_of_son; son_pos++) {
          assert(node->son_1st_off_l > 0 || node->son_1st_off_h > 0);
          LmaNodeGE1* node_son = nodes_ge1_ + get_son_offset(node) + son_pos;
          if (node_son->spl_idx >= id_start && node_son->spl_idx < id_start + id_num) {
            if (node_to_num < MAX_EXTENDBUF_LEN) {
              node_to_ge1[node_to_num] = node_son;
              node_to_num++;
            }
          }
          // id_start + id_num - 1 is the last one, which has just been
          // recorded.
          if (node_son->spl_idx >= id_start + id_num - 1) break;
        }
      }

      spl_pos++;
      if (spl_pos >= splid_str_len || node_to_num == 0) break;
      // Prepare the nodes for next extending
      // next time, from LmaNodeGE1 to LmaNodeGE1
      LmaNodeGE1** node_tmp = node_fr_ge1;
      node_fr_ge1 = node_to_ge1;
      node_to_ge1 = node_tmp;
    }

    // The number of node for next extending
    node_fr_num = node_to_num;
    node_to_num = 0;
  }  // while

  if (0 == node_to_num) return 0;

  NGram& ngram = NGram::get_instance();
  unsigned lma_num = 0;

  // If the length is 1, and the splid is a one-char Yunmu like 'a', 'o', 'e',
  // only those candidates for the full matched one-char id will be returned.
  if (1 == splid_str_len && spl_trie_->is_half_id_yunmu(splid_str[0]))
    node_to_num = node_to_num > 0 ? 1 : 0;

  for (unsigned node_pos = 0; node_pos < node_to_num; node_pos++) {
    unsigned num_of_homo = 0;
    if (spl_pos <= 1) {  // Get from LmaNodeLE0 nodes
      LmaNodeLE0* node_le0 = node_to_le0[node_pos];
      num_of_homo = (unsigned)node_le0->num_of_homo;
      for (unsigned homo_pos = 0; homo_pos < num_of_homo; homo_pos++) {
        unsigned ch_pos = lma_num + homo_pos;
        lma_buf[ch_pos].id = get_lemma_id(node_le0->homo_idx_buf_off + homo_pos);
        lma_buf[ch_pos].lma_len = 1;
        lma_buf[ch_pos].psb = static_cast<LmaScoreType>(ngram.get_uni_psb(lma_buf[ch_pos].id));

        if (lma_num + homo_pos >= max_lma_buf - 1) break;
      }
    } else {  // Get from LmaNodeGE1 nodes
      LmaNodeGE1* node_ge1 = node_to_ge1[node_pos];
      num_of_homo = (unsigned)node_ge1->num_of_homo;
      for (unsigned homo_pos = 0; homo_pos < num_of_homo; homo_pos++) {
        unsigned ch_pos = lma_num + homo_pos;
        unsigned node_homo_off = get_homo_idx_buf_offset(node_ge1);
        lma_buf[ch_pos].id = get_lemma_id(node_homo_off + homo_pos);
        lma_buf[ch_pos].lma_len = splid_str_len;
        lma_buf[ch_pos].psb = static_cast<LmaScoreType>(ngram.get_uni_psb(lma_buf[ch_pos].id));

        if (lma_num + homo_pos >= max_lma_buf - 1) break;
      }
    }

    lma_num += num_of_homo;
    if (lma_num >= max_lma_buf) {
      lma_num = max_lma_buf;
      break;
    }
  }
  return lma_num;
}

uint16 DictTrie::get_lemma_str(LemmaIdType id_lemma, char16* str_buf, uint16 str_max) {
  return dict_list_->get_lemma_str(id_lemma, str_buf, str_max);
}

uint16 DictTrie::get_lemma_splids(LemmaIdType id_lemma, uint16* splids, uint16 splids_max,
                                  bool arg_valid) {
  char16 lma_str[kMaxLemmaSize + 1];
  uint16 lma_len = get_lemma_str(id_lemma, lma_str, kMaxLemmaSize + 1);
  assert((!arg_valid && splids_max >= lma_len) || lma_len == splids_max);

  uint16 spl_mtrx[kMaxLemmaSize * 5];
  uint16 spl_start[kMaxLemmaSize + 1];
  spl_start[0] = 0;
  uint16 try_num = 1;

  for (uint16 pos = 0; pos < lma_len; pos++) {
    uint16 cand_splids_this = 0;
    if (arg_valid && spl_trie_->is_full_id(splids[pos])) {
      spl_mtrx[spl_start[pos]] = splids[pos];
      cand_splids_this = 1;
    } else {
      cand_splids_this = dict_list_->get_splids_for_hanzi(lma_str[pos], arg_valid ? splids[pos] : 0,
                                                          spl_mtrx + spl_start[pos],
                                                          kMaxLemmaSize * 5 - spl_start[pos]);
      assert(cand_splids_this > 0);
    }
    spl_start[pos + 1] = spl_start[pos] + cand_splids_this;
    try_num *= cand_splids_this;
  }

  for (uint16 try_pos = 0; try_pos < try_num; try_pos++) {
    uint16 mod = 1;
    for (uint16 pos = 0; pos < lma_len; pos++) {
      uint16 radix = spl_start[pos + 1] - spl_start[pos];
      splids[pos] = spl_mtrx[spl_start[pos] + try_pos / mod % radix];
      mod *= radix;
    }

    if (try_extend(splids, lma_len, id_lemma)) return lma_len;
  }

  return 0;
}

void DictTrie::set_total_lemma_count_of_others(unsigned count) {
  NGram& ngram = NGram::get_instance();
  ngram.set_total_freq_none_sys(count);
}

void DictTrie::convert_to_hanzis(char16* str, uint16 str_len) {
  return dict_list_->convert_to_hanzis(str, str_len);
}

void DictTrie::convert_to_scis_ids(char16* str, uint16 str_len) {
  return dict_list_->convert_to_scis_ids(str, str_len);
}

LemmaIdType DictTrie::get_lemma_id(const char16 lemma_str[], uint16 lemma_len) {
  if (NULL == lemma_str || lemma_len > kMaxLemmaSize) return 0;

  return dict_list_->get_lemma_id(lemma_str, lemma_len);
}

unsigned DictTrie::predict_top_lmas(unsigned his_len, NPredictItem* npre_items, unsigned npre_max,
                                    unsigned b4_used) {
  NGram& ngram = NGram::get_instance();

  unsigned item_num = 0;
  unsigned top_lmas_id_offset = lma_idx_buf_len_ / kLemmaIdSize - top_lmas_num_;
  unsigned top_lmas_pos = 0;
  while (item_num < npre_max && top_lmas_pos < top_lmas_num_) {
    memset(npre_items + item_num, 0, sizeof(NPredictItem));
    LemmaIdType top_lma_id = get_lemma_id(top_lmas_id_offset + top_lmas_pos);
    top_lmas_pos += 1;
    if (dict_list_->get_lemma_str(top_lma_id, npre_items[item_num].pre_hzs, kMaxLemmaSize - 1) ==
        0) {
      continue;
    }
    npre_items[item_num].psb = ngram.get_uni_psb(top_lma_id);
    npre_items[item_num].his_len = his_len;
    item_num++;
  }
  return item_num;
}

unsigned DictTrie::predict(const char16* last_hzs, uint16 hzs_len, NPredictItem* npre_items,
                           unsigned npre_max, unsigned b4_used) {
  return dict_list_->predict(last_hzs, hzs_len, npre_items, npre_max, b4_used);
}
}  // namespace ime_pinyin
