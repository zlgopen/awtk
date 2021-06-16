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
#include <math.h>
#include <string.h>

#include "file.h"
#include "../include/lpicache.h"
#include "../include/matrixsearch.h"
#include "../include/mystdlib.h"
#include "../include/ngram.h"

namespace ime_pinyin {

#define PRUMING_SCORE 8000.0f

MatrixSearch::MatrixSearch() {
  inited_ = false;
  spl_trie_ = SpellingTrie::get_cpinstance();

  reset_pointers_to_null();

  pys_decoded_len_ = 0;
  mtrx_nd_pool_used_ = 0;
  dmi_pool_used_ = 0;
  xi_an_enabled_ = false;
  dmi_c_phrase_ = false;

  assert(kMaxSearchSteps > 0);
  max_sps_len_ = kMaxSearchSteps - 1;
  max_hzs_len_ = kMaxSearchSteps;
}

MatrixSearch::~MatrixSearch() {
  LpiCache::free_instance();
  free_resource();
}

void MatrixSearch::reset_pointers_to_null() {
  dict_trie_ = NULL;
  spl_parser_ = NULL;

  share_buf_ = NULL;

  // The following four buffers are used for decoding, and they are based on
  // share_buf_, no need to delete them.
  mtrx_nd_pool_ = NULL;
  dmi_pool_ = NULL;
  matrix_ = NULL;
  dep_ = NULL;

  // Based on share_buf_, no need to delete them.
  npre_items_ = NULL;
}

bool MatrixSearch::alloc_resource() {
  free_resource();

  dict_trie_ = new DictTrie();
  spl_parser_ = new SpellingParser();

  unsigned mtrx_nd_size = sizeof(MatrixNode) * kMtrxNdPoolSize;
  mtrx_nd_size = align_to_unsigned(mtrx_nd_size) / sizeof(unsigned);
  unsigned dmi_size = sizeof(DictMatchInfo) * kDmiPoolSize;
  dmi_size = align_to_unsigned(dmi_size) / sizeof(unsigned);
  unsigned matrix_size = sizeof(MatrixRow) * kMaxRowNum;
  matrix_size = align_to_unsigned(matrix_size) / sizeof(unsigned);
  unsigned dep_size = sizeof(DictExtPara);
  dep_size = align_to_unsigned(dep_size) / sizeof(unsigned);

  // share_buf's size is determined by the buffers for search.
  share_buf_ = new unsigned[mtrx_nd_size + dmi_size + matrix_size + dep_size];

  if (NULL == dict_trie_ || NULL == spl_parser_ || NULL == share_buf_) return false;

  // The buffers for search are based on the share buffer
  mtrx_nd_pool_ = reinterpret_cast<MatrixNode*>(share_buf_);
  dmi_pool_ = reinterpret_cast<DictMatchInfo*>(share_buf_ + mtrx_nd_size);
  matrix_ = reinterpret_cast<MatrixRow*>(share_buf_ + mtrx_nd_size + dmi_size);
  dep_ = reinterpret_cast<DictExtPara*>(share_buf_ + mtrx_nd_size + dmi_size + matrix_size);

  // The prediction buffer is also based on the share buffer.
  npre_items_ = reinterpret_cast<NPredictItem*>(share_buf_);
  npre_items_len_ =
      (mtrx_nd_size + dmi_size + matrix_size + dep_size) * sizeof(unsigned) / sizeof(NPredictItem);
  return true;
}

void MatrixSearch::free_resource() {
  if (NULL != dict_trie_) delete dict_trie_;

  if (NULL != spl_parser_) delete spl_parser_;

  if (NULL != share_buf_) delete[] share_buf_;

  reset_pointers_to_null();
}

bool MatrixSearch::init(const char* fn_sys_dict, const char* fn_usr_dict) {
  if (NULL == fn_sys_dict || NULL == fn_usr_dict) return false;

  if (!alloc_resource()) return false;

  if (!dict_trie_->load_dict(fn_sys_dict, 1, kSysDictIdEnd)) return false;

  reset_search0();

  inited_ = true;
  return true;
}

bool MatrixSearch::init_fd(int sys_fd, long start_offset, long length, const char* fn_usr_dict) {
  if (NULL == fn_usr_dict) return false;

  if (!alloc_resource()) return false;

  if (!dict_trie_->load_dict_fd(sys_fd, start_offset, length, 1, kSysDictIdEnd)) return false;

  reset_search0();

  inited_ = true;
  return true;
}

void MatrixSearch::set_max_lens(unsigned max_sps_len, unsigned max_hzs_len) {
  if (0 != max_sps_len) max_sps_len_ = max_sps_len;
  if (0 != max_hzs_len) max_hzs_len_ = max_hzs_len;
}

void MatrixSearch::close() {
  flush_cache();
  free_resource();
  inited_ = false;
}

void MatrixSearch::flush_cache() {
}

void MatrixSearch::set_xi_an_switch(bool xi_an_enabled) {
  xi_an_enabled_ = xi_an_enabled;
}

bool MatrixSearch::get_xi_an_switch() {
  return xi_an_enabled_;
}

bool MatrixSearch::reset_search() {
  if (!inited_) return false;
  return reset_search0();
}

bool MatrixSearch::reset_search0() {
  if (!inited_) return false;

  pys_decoded_len_ = 0;
  mtrx_nd_pool_used_ = 0;
  dmi_pool_used_ = 0;

  // Get a MatrixNode from the pool
  matrix_[0].mtrx_nd_pos = mtrx_nd_pool_used_;
  matrix_[0].mtrx_nd_num = 1;
  mtrx_nd_pool_used_ += 1;

  // Update the node, and make it to be a starting node
  MatrixNode* node = mtrx_nd_pool_ + matrix_[0].mtrx_nd_pos;
  node->id = 0;
  node->score = 0;
  node->from = NULL;
  node->step = 0;
  node->dmi_fr = (PoolPosType)-1;

  matrix_[0].dmi_pos = 0;
  matrix_[0].dmi_num = 0;
  matrix_[0].dmi_has_full_id = 1;
  matrix_[0].mtrx_nd_fixed = node;

  lma_start_[0] = 0;
  fixed_lmas_ = 0;
  spl_start_[0] = 0;
  fixed_hzs_ = 0;

  dict_trie_->reset_milestones(0, 0);

  return true;
}

bool MatrixSearch::reset_search(unsigned ch_pos, bool clear_fixed_this_step,
                                bool clear_dmi_this_step, bool clear_mtrx_this_step) {
  if (!inited_ || ch_pos > pys_decoded_len_ || ch_pos >= kMaxRowNum) return false;

  if (0 == ch_pos) {
    reset_search0();
  } else {
    // Prepare mile stones of this step to clear.
    MileStoneHandle* dict_handles_to_clear = NULL;
    if (clear_dmi_this_step && matrix_[ch_pos].dmi_num > 0) {
      dict_handles_to_clear = dmi_pool_[matrix_[ch_pos].dmi_pos].dict_handles;
    }

    // If there are more steps, and this step is not allowed to clear, find
    // milestones of next step.
    if (pys_decoded_len_ > ch_pos && !clear_dmi_this_step) {
      dict_handles_to_clear = NULL;
      if (matrix_[ch_pos + 1].dmi_num > 0) {
        dict_handles_to_clear = dmi_pool_[matrix_[ch_pos + 1].dmi_pos].dict_handles;
      }
    }

    if (NULL != dict_handles_to_clear) {
      dict_trie_->reset_milestones(ch_pos, dict_handles_to_clear[0]);
    }

    pys_decoded_len_ = ch_pos;

    if (clear_dmi_this_step) {
      dmi_pool_used_ = matrix_[ch_pos - 1].dmi_pos + matrix_[ch_pos - 1].dmi_num;
      matrix_[ch_pos].dmi_num = 0;
    } else {
      dmi_pool_used_ = matrix_[ch_pos].dmi_pos + matrix_[ch_pos].dmi_num;
    }

    if (clear_mtrx_this_step) {
      mtrx_nd_pool_used_ = matrix_[ch_pos - 1].mtrx_nd_pos + matrix_[ch_pos - 1].mtrx_nd_num;
      matrix_[ch_pos].mtrx_nd_num = 0;
    } else {
      mtrx_nd_pool_used_ = matrix_[ch_pos].mtrx_nd_pos + matrix_[ch_pos].mtrx_nd_num;
    }

    // Modify fixed_hzs_
    if (fixed_hzs_ > 0 &&
        ((kLemmaIdComposing != lma_id_[0]) ||
         (kLemmaIdComposing == lma_id_[0] && spl_start_[c_phrase_.length] <= ch_pos))) {
      unsigned fixed_ch_pos = ch_pos;
      if (clear_fixed_this_step) fixed_ch_pos = fixed_ch_pos > 0 ? fixed_ch_pos - 1 : 0;
      while (NULL == matrix_[fixed_ch_pos].mtrx_nd_fixed && fixed_ch_pos > 0) fixed_ch_pos--;

      fixed_lmas_ = 0;
      fixed_hzs_ = 0;
      if (fixed_ch_pos > 0) {
        while (spl_start_[fixed_hzs_] < fixed_ch_pos) fixed_hzs_++;
        assert(spl_start_[fixed_hzs_] == fixed_ch_pos);

        while (lma_start_[fixed_lmas_] < fixed_hzs_) fixed_lmas_++;
        assert(lma_start_[fixed_lmas_] == fixed_hzs_);
      }

      // Re-search the Pinyin string for the unlocked lemma
      // which was previously fixed.
      //
      // Prepare mile stones of this step to clear.
      MileStoneHandle* dict_handles_to_clear = NULL;
      if (clear_dmi_this_step && ch_pos == fixed_ch_pos && matrix_[fixed_ch_pos].dmi_num > 0) {
        dict_handles_to_clear = dmi_pool_[matrix_[fixed_ch_pos].dmi_pos].dict_handles;
      }

      // If there are more steps, and this step is not allowed to clear, find
      // milestones of next step.
      if (pys_decoded_len_ > fixed_ch_pos && !clear_dmi_this_step) {
        dict_handles_to_clear = NULL;
        if (matrix_[fixed_ch_pos + 1].dmi_num > 0) {
          dict_handles_to_clear = dmi_pool_[matrix_[fixed_ch_pos + 1].dmi_pos].dict_handles;
        }
      }

      if (NULL != dict_handles_to_clear) {
        dict_trie_->reset_milestones(fixed_ch_pos, dict_handles_to_clear[0]);
      }

      pys_decoded_len_ = fixed_ch_pos;

      if (clear_dmi_this_step && ch_pos == fixed_ch_pos) {
        dmi_pool_used_ = matrix_[fixed_ch_pos - 1].dmi_pos + matrix_[fixed_ch_pos - 1].dmi_num;
        matrix_[fixed_ch_pos].dmi_num = 0;
      } else {
        dmi_pool_used_ = matrix_[fixed_ch_pos].dmi_pos + matrix_[fixed_ch_pos].dmi_num;
      }

      if (clear_mtrx_this_step && ch_pos == fixed_ch_pos) {
        mtrx_nd_pool_used_ =
            matrix_[fixed_ch_pos - 1].mtrx_nd_pos + matrix_[fixed_ch_pos - 1].mtrx_nd_num;
        matrix_[fixed_ch_pos].mtrx_nd_num = 0;
      } else {
        mtrx_nd_pool_used_ = matrix_[fixed_ch_pos].mtrx_nd_pos + matrix_[fixed_ch_pos].mtrx_nd_num;
      }

      for (uint16 re_pos = fixed_ch_pos; re_pos < ch_pos; re_pos++) {
        add_char(pys_[re_pos]);
      }
    } else if (fixed_hzs_ > 0 && kLemmaIdComposing == lma_id_[0]) {
      for (uint16 subpos = 0; subpos < c_phrase_.sublma_num; subpos++) {
        uint16 splpos_begin = c_phrase_.sublma_start[subpos];
        uint16 splpos_end = c_phrase_.sublma_start[subpos + 1];
        for (uint16 splpos = splpos_begin; splpos < splpos_end; splpos++) {
          // If ch_pos is in this spelling
          uint16 spl_start = c_phrase_.spl_start[splpos];
          uint16 spl_end = c_phrase_.spl_start[splpos + 1];
          if (ch_pos >= spl_start && ch_pos < spl_end) {
            // Clear everything after this position
            c_phrase_.chn_str[splpos] = static_cast<char16>('\0');
            c_phrase_.sublma_start[subpos + 1] = splpos;
            c_phrase_.sublma_num = subpos + 1;
            c_phrase_.length = splpos;

            if (splpos == splpos_begin) {
              c_phrase_.sublma_num = subpos;
            }
          }
        }
      }

      // Extend the composing phrase.
      reset_search0();
      dmi_c_phrase_ = true;
      uint16 c_py_pos = 0;
      while (c_py_pos < spl_start_[c_phrase_.length]) {
        bool b_ac_tmp = add_char(pys_[c_py_pos]);
        assert(b_ac_tmp);
        c_py_pos++;
      }
      dmi_c_phrase_ = false;

      lma_id_num_ = 1;
      fixed_lmas_ = 1;
      fixed_lmas_no1_[0] = 0;  // A composing string is always modified.
      fixed_hzs_ = c_phrase_.length;
      lma_start_[1] = fixed_hzs_;
      lma_id_[0] = kLemmaIdComposing;
      matrix_[spl_start_[fixed_hzs_]].mtrx_nd_fixed =
          mtrx_nd_pool_ + matrix_[spl_start_[fixed_hzs_]].mtrx_nd_pos;
    }
  }

  return true;
}

void MatrixSearch::del_in_pys(unsigned start, unsigned len) {
  while (start < kMaxRowNum - len && '\0' != pys_[start]) {
    pys_[start] = pys_[start + len];
    start++;
  }
}

unsigned MatrixSearch::search(const char* py, unsigned py_len) {
  if (!inited_ || NULL == py) return 0;

  // If the search Pinyin string is too long, it will be truncated.
  if (py_len > kMaxRowNum - 1) py_len = kMaxRowNum - 1;

  // Compare the new string with the previous one. Find their prefix to
  // increase search efficiency.
  unsigned ch_pos = 0;
  for (ch_pos = 0; ch_pos < pys_decoded_len_; ch_pos++) {
    if ('\0' == py[ch_pos] || py[ch_pos] != pys_[ch_pos]) break;
  }

  bool clear_fix = true;
  if (ch_pos == pys_decoded_len_) clear_fix = false;

  reset_search(ch_pos, clear_fix, false, false);

  memcpy(pys_ + ch_pos, py + ch_pos, py_len - ch_pos);
  pys_[py_len] = '\0';

  while ('\0' != pys_[ch_pos]) {
    if (!add_char(py[ch_pos])) {
      pys_decoded_len_ = ch_pos;
      break;
    }
    ch_pos++;
  }

  // Get spelling ids and starting positions.
  get_spl_start_id();

  // If there are too many spellings, remove the last letter until the spelling
  // number is acceptable.
  while (spl_id_num_ > 9) {
    py_len--;
    reset_search(py_len, false, false, false);
    pys_[py_len] = '\0';
    get_spl_start_id();
  }

  prepare_candidates();

  return ch_pos;
}

unsigned MatrixSearch::delsearch(unsigned pos, bool is_pos_in_splid, bool clear_fixed_this_step) {
  if (!inited_) return 0;

  unsigned reset_pos = pos;

  // Out of range for both Pinyin mode and Spelling id mode.
  if (pys_decoded_len_ <= pos) {
    del_in_pys(pos, 1);

    reset_pos = pys_decoded_len_;
    // Decode the string after the un-decoded position
    while ('\0' != pys_[reset_pos]) {
      if (!add_char(pys_[reset_pos])) {
        pys_decoded_len_ = reset_pos;
        break;
      }
      reset_pos++;
    }
    get_spl_start_id();
    prepare_candidates();
    return pys_decoded_len_;
  }

  // Spelling id mode, but out of range.
  if (is_pos_in_splid && spl_id_num_ <= pos) return pys_decoded_len_;

  // Begin to handle two modes respectively.
  // Pinyin mode by default
  unsigned c_py_len = 0;  // The length of composing phrase's Pinyin
  unsigned del_py_len = 1;
  if (!is_pos_in_splid) {
    // Pinyin mode is only allowed to delete beyond the fixed lemmas.
    if (fixed_lmas_ > 0 && pos < spl_start_[lma_start_[fixed_lmas_]]) return pys_decoded_len_;

    del_in_pys(pos, 1);

    // If the deleted character is just the one after the last fixed lemma
    if (pos == spl_start_[lma_start_[fixed_lmas_]]) {
      // If all fixed lemmas have been merged, and the caller of the function
      // request to unlock the last fixed lemma.
      if (kLemmaIdComposing == lma_id_[0] && clear_fixed_this_step) {
        // Unlock the last sub lemma in the composing phrase. Because it is not
        // easy to unlock it directly. Instead, we re-decode the modified
        // composing phrase.
        c_phrase_.sublma_num--;
        c_phrase_.length = c_phrase_.sublma_start[c_phrase_.sublma_num];
        reset_pos = spl_start_[c_phrase_.length];
        c_py_len = reset_pos;
      }
    }
  } else {
    del_py_len = spl_start_[pos + 1] - spl_start_[pos];

    del_in_pys(spl_start_[pos], del_py_len);

    if (pos >= lma_start_[fixed_lmas_]) {
      c_py_len = 0;
      reset_pos = spl_start_[pos + 1] - del_py_len;
    } else {
      c_py_len = spl_start_[lma_start_[fixed_lmas_]] - del_py_len;
      reset_pos = c_py_len;
      if (c_py_len > 0) merge_fixed_lmas(pos);
    }
  }

  if (c_py_len > 0) {
    assert(c_phrase_.length > 0 &&
           c_py_len == c_phrase_.spl_start[c_phrase_.sublma_start[c_phrase_.sublma_num]]);
    // The composing phrase is valid, reset all search space,
    // and begin a new search which will only extend the composing
    // phrase.
    reset_search0();

    dmi_c_phrase_ = true;
    // Extend the composing phrase.
    uint16 c_py_pos = 0;
    while (c_py_pos < c_py_len) {
      bool b_ac_tmp = add_char(pys_[c_py_pos]);
      assert(b_ac_tmp);
      c_py_pos++;
    }
    dmi_c_phrase_ = false;

    // Fixd the composing phrase as the first choice.
    lma_id_num_ = 1;
    fixed_lmas_ = 1;
    fixed_lmas_no1_[0] = 0;  // A composing string is always modified.
    fixed_hzs_ = c_phrase_.length;
    lma_start_[1] = fixed_hzs_;
    lma_id_[0] = kLemmaIdComposing;
    matrix_[spl_start_[fixed_hzs_]].mtrx_nd_fixed =
        mtrx_nd_pool_ + matrix_[spl_start_[fixed_hzs_]].mtrx_nd_pos;
  } else {
    // Reseting search only clear pys_decoded_len_, but the string is kept.
    reset_search(reset_pos, clear_fixed_this_step, false, false);
  }

  // Decode the string after the delete position.
  while ('\0' != pys_[reset_pos]) {
    if (!add_char(pys_[reset_pos])) {
      pys_decoded_len_ = reset_pos;
      break;
    }
    reset_pos++;
  }

  get_spl_start_id();
  prepare_candidates();
  return pys_decoded_len_;
}

unsigned MatrixSearch::get_candidate_num() {
  if (!inited_ || 0 == pys_decoded_len_ || 0 == matrix_[pys_decoded_len_].mtrx_nd_num) return 0;

  return 1 + lpi_total_;
}

char16* MatrixSearch::get_candidate(unsigned cand_id, char16* cand_str, unsigned max_len) {
  if (!inited_ || 0 == pys_decoded_len_ || NULL == cand_str) return NULL;

  if (0 == cand_id) {
    return get_candidate0(cand_str, max_len, NULL, false);
  } else {
    cand_id--;
  }

  // For this case: the current sentence is a word only, and the user fixed it,
  // so the result will be fixed to the sentence space, and
  // lpi_total_ will be set to 0.
  if (0 == lpi_total_) {
    return get_candidate0(cand_str, max_len, NULL, false);
  }

  LemmaIdType id = lpi_items_[cand_id].id;
  char16 s[kMaxLemmaSize + 1];

  uint16 s_len = lpi_items_[cand_id].lma_len;
  if (s_len > 1) {
    s_len = get_lemma_str(id, s, kMaxLemmaSize + 1);
  } else {
    // For a single character, Hanzi is ready.
    s[0] = lpi_items_[cand_id].hanzi;
    s[1] = static_cast<char16>(0);
  }

  if (s_len > 0 && max_len > s_len) {
    utf16_strncpy(cand_str, s, s_len);
    cand_str[s_len] = (char16)'\0';
    return cand_str;
  }

  return NULL;
}

void MatrixSearch::update_dict_freq() {
}

bool MatrixSearch::add_lma_to_userdict(uint16 lma_fr, uint16 lma_to, float score) {
  return true;
}

void MatrixSearch::debug_print_dmi(PoolPosType dmi_pos, uint16 nest_level) {
}

bool MatrixSearch::try_add_cand0_to_userdict() {
  unsigned new_cand_num = get_candidate_num();
  if (fixed_hzs_ > 0 && 1 == new_cand_num) {
    float score_from = 0;
    uint16 lma_id_from = 0;
    uint16 pos = 0;
    bool modified = false;
    while (pos < fixed_lmas_) {
      if (lma_start_[pos + 1] - lma_start_[lma_id_from] > static_cast<uint16>(kMaxLemmaSize)) {
        float score_to_add =
            mtrx_nd_pool_[matrix_[spl_start_[lma_start_[pos]]].mtrx_nd_pos].score - score_from;
        if (modified) {
          score_to_add += 1.0f;
          if (score_to_add > NGram::kMaxScore) {
            score_to_add = NGram::kMaxScore;
          }
          add_lma_to_userdict(lma_id_from, pos, score_to_add);
        }
        lma_id_from = pos;
        score_from += score_to_add;

        // Clear the flag for next user lemma.
        modified = false;
      }

      if (0 == fixed_lmas_no1_[pos]) {
        modified = true;
      }
      pos++;
    }

    // Single-char word is not allowed to add to userdict.
    if (lma_start_[pos] - lma_start_[lma_id_from] > 1) {
      float score_to_add =
          mtrx_nd_pool_[matrix_[spl_start_[lma_start_[pos]]].mtrx_nd_pos].score - score_from;
      if (modified) {
        score_to_add += 1.0f;
        if (score_to_add > NGram::kMaxScore) {
          score_to_add = NGram::kMaxScore;
        }
        add_lma_to_userdict(lma_id_from, pos, score_to_add);
      }
    }
  }
  return true;
}

// Choose a candidate, and give new candidates for next step.
// If user finishes selection, we will try to communicate with user dictionary
// to add new items or update score of some existing items.
//
// Basic rule:
// 1. If user selects the first choice:
//    1.1. If the first choice is not a sentence, instead, it is a lemma:
//         1.1.1. If the first choice is a user lemma, notify the user
//                dictionary that a user lemma is hit, and add occuring count
//                by 1.
//         1.1.2. If the first choice is a system lemma, do nothing.
//    1.2. If the first choice is a sentence containing more than one lemma:
//         1.2.1. The whole sentence will be added as a user lemma. If the
//                sentence contains user lemmas, -> hit, and add occuring count
//                by 1.
unsigned MatrixSearch::choose(unsigned cand_id) {
  if (!inited_ || 0 == pys_decoded_len_) return 0;

  if (0 == cand_id) {
    fixed_hzs_ = spl_id_num_;
    matrix_[spl_start_[fixed_hzs_]].mtrx_nd_fixed =
        mtrx_nd_pool_ + matrix_[spl_start_[fixed_hzs_]].mtrx_nd_pos;
    for (unsigned pos = fixed_lmas_; pos < lma_id_num_; pos++) {
      fixed_lmas_no1_[pos] = 1;
    }
    fixed_lmas_ = lma_id_num_;
    lpi_total_ = 0;  // Clean all other candidates.

    // 1. It is the first choice
    if (1 == lma_id_num_) {
      // 1.1. The first choice is not a sentence but a lemma
    } else {
      // 1.2. The first choice is a sentence.
      // 1.2.1 Try to add the whole sentence to user dictionary, the whole
      // sentence may be splitted into many items.
    }
    update_dict_freq();
    return 1;
  } else {
    cand_id--;
  }

  // 2. It is not the full sentence candidate.
  // Find the length of the candidate.
  LemmaIdType id_chosen = lpi_items_[cand_id].id;
  LmaScoreType score_chosen = lpi_items_[cand_id].psb;
  unsigned cand_len = lpi_items_[cand_id].lma_len;

  assert(cand_len > 0);

  // Notify the atom dictionary that this item is hit.
  if (is_user_lemma(id_chosen)) {
    update_dict_freq();
  }

  // 3. Fixed the chosen item.
  // 3.1 Get the steps number.
  unsigned step_fr = spl_start_[fixed_hzs_];
  unsigned step_to = spl_start_[fixed_hzs_ + cand_len];

  // 3.2 Save the length of the original string.
  unsigned pys_decoded_len = pys_decoded_len_;

  // 3.2 Reset the space of the fixed part.
  reset_search(step_to, false, false, true);

  // 3.3 For the last character of the fixed part, the previous DMI
  // information will be kept, while the MTRX information will be re-extended,
  // and only one node will be extended.
  matrix_[step_to].mtrx_nd_num = 0;

  LmaPsbItem lpi_item;
  lpi_item.psb = score_chosen;
  lpi_item.id = id_chosen;

  PoolPosType step_to_dmi_fr = match_dmi(step_to, spl_id_ + fixed_hzs_, cand_len);
  assert(step_to_dmi_fr != static_cast<PoolPosType>(-1));

  extend_mtrx_nd(matrix_[step_fr].mtrx_nd_fixed, &lpi_item, 1, step_to_dmi_fr, step_to);

  matrix_[step_to].mtrx_nd_fixed = mtrx_nd_pool_ + matrix_[step_to].mtrx_nd_pos;
  mtrx_nd_pool_used_ = matrix_[step_to].mtrx_nd_pos + matrix_[step_to].mtrx_nd_num;

  if (id_chosen == lma_id_[fixed_lmas_])
    fixed_lmas_no1_[fixed_lmas_] = 1;
  else
    fixed_lmas_no1_[fixed_lmas_] = 0;
  lma_id_[fixed_lmas_] = id_chosen;
  lma_start_[fixed_lmas_ + 1] = lma_start_[fixed_lmas_] + cand_len;
  fixed_lmas_++;
  fixed_hzs_ = fixed_hzs_ + cand_len;

  while (step_to != pys_decoded_len) {
    bool b = add_char(pys_[step_to]);
    assert(b);
    step_to++;
  }

  if (fixed_hzs_ < spl_id_num_) {
    prepare_candidates();
  } else {
    lpi_total_ = 0;
  }

  return get_candidate_num();
}

unsigned MatrixSearch::cancel_last_choice() {
  if (!inited_ || 0 == pys_decoded_len_) return 0;

  unsigned step_start = 0;
  if (fixed_hzs_ > 0) {
    unsigned step_end = spl_start_[fixed_hzs_];
    MatrixNode* end_node = matrix_[step_end].mtrx_nd_fixed;
    assert(NULL != end_node);

    step_start = end_node->from->step;

    if (step_start > 0) {
      DictMatchInfo* dmi = dmi_pool_ + end_node->dmi_fr;
      fixed_hzs_ -= dmi->dict_level;
    } else {
      fixed_hzs_ = 0;
    }

    reset_search(step_start, false, false, false);

    while (pys_[step_start] != '\0') {
      bool b = add_char(pys_[step_start]);
      assert(b);
      step_start++;
    }

    prepare_candidates();
  }
  return get_candidate_num();
}

unsigned MatrixSearch::get_fixedlen() {
  if (!inited_ || 0 == pys_decoded_len_) return 0;
  return fixed_hzs_;
}

bool MatrixSearch::prepare_add_char(char ch) {
  if (pys_decoded_len_ >= kMaxRowNum - 1 || (!spl_parser_->is_valid_to_parse(ch) && ch != '\''))
    return false;

  if (dmi_pool_used_ >= kDmiPoolSize) return false;

  pys_[pys_decoded_len_] = ch;
  pys_decoded_len_++;

  MatrixRow* mtrx_this_row = matrix_ + pys_decoded_len_;
  mtrx_this_row->mtrx_nd_pos = mtrx_nd_pool_used_;
  mtrx_this_row->mtrx_nd_num = 0;
  mtrx_this_row->dmi_pos = dmi_pool_used_;
  mtrx_this_row->dmi_num = 0;
  mtrx_this_row->dmi_has_full_id = 0;

  return true;
}

bool MatrixSearch::is_split_at(uint16 pos) {
  return !spl_parser_->is_valid_to_parse(pys_[pos - 1]);
}

void MatrixSearch::fill_dmi(DictMatchInfo* dmi, MileStoneHandle* handles, PoolPosType dmi_fr,
                            uint16 spl_id, uint16 node_num, unsigned char dict_level,
                            bool splid_end_split, unsigned char splstr_len,
                            unsigned char all_full_id) {
  dmi->dict_handles[0] = handles[0];
  dmi->dict_handles[1] = handles[1];
  dmi->dmi_fr = dmi_fr;
  dmi->spl_id = spl_id;
  dmi->dict_level = dict_level;
  dmi->splid_end_split = splid_end_split ? 1 : 0;
  dmi->splstr_len = splstr_len;
  dmi->all_full_id = all_full_id;
  dmi->c_phrase = 0;
}

bool MatrixSearch::add_char(char ch) {
  if (!prepare_add_char(ch)) return false;
  return add_char_qwerty();
}

bool MatrixSearch::add_char_qwerty() {
  matrix_[pys_decoded_len_].mtrx_nd_num = 0;

  bool spl_matched = false;
  uint16 longest_ext = 0;
  // Extend the search matrix, from the oldest unfixed row. ext_len means
  // extending length.
  for (uint16 ext_len = kMaxPinyinSize + 1; ext_len > 0; ext_len--) {
    if (ext_len > pys_decoded_len_ - spl_start_[fixed_hzs_]) continue;

    // Refer to the declaration of the variable dmi_has_full_id for the
    // explanation of this piece of code. In one word, it is used to prevent
    // from the unwise extending of "shoud ou" but allow the reasonable
    // extending of "heng ao", "lang a", etc.
    if (ext_len > 1 && 0 != longest_ext &&
        0 == matrix_[pys_decoded_len_ - ext_len].dmi_has_full_id) {
      if (xi_an_enabled_)
        continue;
      else
        break;
    }

    uint16 oldrow = pys_decoded_len_ - ext_len;

    // 0. If that row is before the last fixed step, ignore.
    if (spl_start_[fixed_hzs_] > oldrow) continue;

    // 1. Check if that old row has valid MatrixNode. If no, means that row is
    // not a boundary, either a word boundary or a spelling boundary.
    // If it is for extending composing phrase, it's OK to ignore the 0.
    if (0 == matrix_[oldrow].mtrx_nd_num && !dmi_c_phrase_) continue;

    // 2. Get spelling id(s) for the last ext_len chars.
    uint16 spl_idx;
    bool is_pre = false;
    spl_idx = spl_parser_->get_splid_by_str(pys_ + oldrow, ext_len, &is_pre);
    if (is_pre) spl_matched = true;

    if (0 == spl_idx) continue;

    bool splid_end_split = is_split_at(oldrow + ext_len);

    // 3. Extend the DMI nodes of that old row
    // + 1 is to extend an extra node from the root
    for (PoolPosType dmi_pos = matrix_[oldrow].dmi_pos;
         dmi_pos < matrix_[oldrow].dmi_pos + matrix_[oldrow].dmi_num + 1; dmi_pos++) {
      DictMatchInfo* dmi = dmi_pool_ + dmi_pos;
      if (dmi_pos == matrix_[oldrow].dmi_pos + matrix_[oldrow].dmi_num) {
        dmi = NULL;  // The last one, NULL means extending from the root.
      } else {
        // If the dmi is covered by the fixed arrange, ignore it.
        if (fixed_hzs_ > 0 &&
            pys_decoded_len_ - ext_len - dmi->splstr_len < spl_start_[fixed_hzs_]) {
          continue;
        }
        // If it is not in mode for composing phrase, and the source DMI node
        // is marked for composing phrase, ignore this node.
        if (dmi->c_phrase != 0 && !dmi_c_phrase_) {
          continue;
        }
      }

      // For example, if "gao" is extended, "g ao" is not allowed.
      // or "zh" has been passed, "z h" is not allowed.
      // Both word and word-connection will be prevented.
      if (longest_ext > ext_len) {
        if (NULL == dmi && 0 == matrix_[oldrow].dmi_has_full_id) {
          continue;
        }

        // "z h" is not allowed.
        if (NULL != dmi && spl_trie_->is_half_id(dmi->spl_id)) {
          continue;
        }
      }

      dep_->splids_extended = 0;
      if (NULL != dmi) {
        uint16 prev_ids_num = dmi->dict_level;
        if ((!dmi_c_phrase_ && prev_ids_num >= kMaxLemmaSize) ||
            (dmi_c_phrase_ && prev_ids_num >= kMaxRowNum)) {
          continue;
        }

        DictMatchInfo* d = dmi;
        while (d) {
          dep_->splids[--prev_ids_num] = d->spl_id;
          if ((PoolPosType)-1 == d->dmi_fr) break;
          d = dmi_pool_ + d->dmi_fr;
        }
        assert(0 == prev_ids_num);
        dep_->splids_extended = dmi->dict_level;
      }
      dep_->splids[dep_->splids_extended] = spl_idx;
      dep_->ext_len = ext_len;
      dep_->splid_end_split = splid_end_split;

      dep_->id_num = 1;
      dep_->id_start = spl_idx;
      if (spl_trie_->is_half_id(spl_idx)) {
        // Get the full id list
        dep_->id_num = spl_trie_->half_to_full(spl_idx, &(dep_->id_start));
        assert(dep_->id_num > 0);
      }

      uint16 new_dmi_num;

      new_dmi_num = extend_dmi(dep_, dmi);

      if (new_dmi_num > 0) {
        if (dmi_c_phrase_) {
          dmi_pool_[dmi_pool_used_].c_phrase = 1;
        }
        matrix_[pys_decoded_len_].dmi_num += new_dmi_num;
        dmi_pool_used_ += new_dmi_num;

        if (!spl_trie_->is_half_id(spl_idx)) matrix_[pys_decoded_len_].dmi_has_full_id = 1;
      }

      // If get candiate lemmas, try to extend the path
      if (lpi_total_ > 0) {
        uint16 fr_row;
        if (NULL == dmi) {
          fr_row = oldrow;
        } else {
          assert(oldrow >= dmi->splstr_len);
          fr_row = oldrow - dmi->splstr_len;
        }
        for (PoolPosType mtrx_nd_pos = matrix_[fr_row].mtrx_nd_pos;
             mtrx_nd_pos < matrix_[fr_row].mtrx_nd_pos + matrix_[fr_row].mtrx_nd_num;
             mtrx_nd_pos++) {
          MatrixNode* mtrx_nd = mtrx_nd_pool_ + mtrx_nd_pos;

          extend_mtrx_nd(mtrx_nd, lpi_items_, lpi_total_, dmi_pool_used_ - new_dmi_num,
                         pys_decoded_len_);
          if (longest_ext == 0) longest_ext = ext_len;
        }
      }
    }  // for dmi_pos
  }    // for ext_len
  mtrx_nd_pool_used_ += matrix_[pys_decoded_len_].mtrx_nd_num;

  if (dmi_c_phrase_) return true;

  return (matrix_[pys_decoded_len_].mtrx_nd_num != 0 || spl_matched);
}

void MatrixSearch::prepare_candidates() {
  // Get candiates from the first un-fixed step.
  uint16 lma_size_max = kMaxLemmaSize;
  if (lma_size_max > spl_id_num_ - fixed_hzs_) lma_size_max = spl_id_num_ - fixed_hzs_;

  uint16 lma_size = lma_size_max;

  // If the full sentense candidate's unfixed part may be the same with a normal
  // lemma. Remove the lemma candidate in this case.
  char16 fullsent[kMaxLemmaSize + 1];
  char16* pfullsent = NULL;
  uint16 sent_len;
  pfullsent = get_candidate0(fullsent, kMaxLemmaSize + 1, &sent_len, true);

  // If the unfixed part contains more than one ids, it is not necessary to
  // check whether a lemma's string is the same to the unfixed part of the full
  // sentence candidate, so, set it to NULL;
  if (sent_len > kMaxLemmaSize) pfullsent = NULL;

  lpi_total_ = 0;
  unsigned lpi_num_full_match = 0;  // Number of items which are fully-matched.
  while (lma_size > 0) {
    unsigned lma_num;
    lma_num = get_lpis(spl_id_ + fixed_hzs_, lma_size, lpi_items_ + lpi_total_,
                       unsigned(kMaxLmaPsbItems - lpi_total_), pfullsent, lma_size == lma_size_max);

    if (lma_num > 0) {
      lpi_total_ += lma_num;
      // For next lemma candidates which are not the longest, it is not
      // necessary to compare with the full sentence candiate.
      pfullsent = NULL;
    }
    if (lma_size == lma_size_max) {
      lpi_num_full_match = lpi_total_;
    }
    lma_size--;
  }

  // Sort those partially-matched items by their unified scores.
  myqsort(lpi_items_ + lpi_num_full_match, lpi_total_ - lpi_num_full_match, sizeof(LmaPsbItem),
          cmp_lpi_with_unified_psb);
}

const char* MatrixSearch::get_pystr(unsigned* decoded_len) {
  if (!inited_ || NULL == decoded_len) return NULL;

  *decoded_len = pys_decoded_len_;
  return pys_;
}

void MatrixSearch::merge_fixed_lmas(unsigned del_spl_pos) {
  if (fixed_lmas_ == 0) return;
  // Update spelling segmentation information first.
  spl_id_num_ -= 1;
  uint16 del_py_len = spl_start_[del_spl_pos + 1] - spl_start_[del_spl_pos];
  for (unsigned pos = del_spl_pos; pos <= spl_id_num_; pos++) {
    spl_start_[pos] = spl_start_[pos + 1] - del_py_len;
    if (pos == spl_id_num_) break;
    spl_id_[pos] = spl_id_[pos + 1];
  }

  // Begin to merge.
  uint16 phrase_len = 0;

  // Update the spelling ids to the composing phrase.
  // We need to convert these ids into full id in the future.
  memcpy(c_phrase_.spl_ids, spl_id_, spl_id_num_ * sizeof(uint16));
  memcpy(c_phrase_.spl_start, spl_start_, (spl_id_num_ + 1) * sizeof(uint16));

  // If composing phrase has not been created, first merge all fixed
  //  lemmas into a composing phrase without deletion.
  if (fixed_lmas_ > 1 || kLemmaIdComposing != lma_id_[0]) {
    uint16 bp = 1;  // Begin position of real fixed lemmas.
    // There is no existing composing phrase.
    if (kLemmaIdComposing != lma_id_[0]) {
      c_phrase_.sublma_num = 0;
      bp = 0;
    }

    uint16 sub_num = c_phrase_.sublma_num;
    for (uint16 pos = bp; pos <= fixed_lmas_; pos++) {
      c_phrase_.sublma_start[sub_num + pos - bp] = lma_start_[pos];
      if (lma_start_[pos] > del_spl_pos) {
        c_phrase_.sublma_start[sub_num + pos - bp] -= 1;
      }

      if (pos == fixed_lmas_) break;

      uint16 lma_len;
      char16* lma_str = c_phrase_.chn_str + c_phrase_.sublma_start[sub_num] + phrase_len;

      lma_len = get_lemma_str(lma_id_[pos], lma_str, kMaxRowNum - phrase_len);
      assert(lma_len == lma_start_[pos + 1] - lma_start_[pos]);
      phrase_len += lma_len;
    }
    assert(phrase_len == lma_start_[fixed_lmas_]);
    c_phrase_.length = phrase_len;  // will be deleted by 1
    c_phrase_.sublma_num += fixed_lmas_ - bp;
  } else {
    for (uint16 pos = 0; pos <= c_phrase_.sublma_num; pos++) {
      if (c_phrase_.sublma_start[pos] > del_spl_pos) {
        c_phrase_.sublma_start[pos] -= 1;
      }
    }
    phrase_len = c_phrase_.length;
  }

  assert(phrase_len > 0);
  if (1 == phrase_len) {
    // After the only one is deleted, nothing will be left.
    fixed_lmas_ = 0;
    return;
  }

  // Delete the Chinese character in the merged phrase.
  // The corresponding elements in spl_ids and spl_start of the
  // phrase have been deleted.
  char16* chn_str = c_phrase_.chn_str + del_spl_pos;
  for (uint16 pos = 0; pos < c_phrase_.sublma_start[c_phrase_.sublma_num] - del_spl_pos; pos++) {
    chn_str[pos] = chn_str[pos + 1];
  }
  c_phrase_.length -= 1;

  // If the deleted spelling id is in a sub lemma which contains more than
  // one id, del_a_sub will be false; but if the deleted id is in a sub lemma
  // which only contains 1 id, the whole sub lemma needs to be deleted, so
  // del_a_sub will be true.
  bool del_a_sub = false;
  for (uint16 pos = 1; pos <= c_phrase_.sublma_num; pos++) {
    if (c_phrase_.sublma_start[pos - 1] == c_phrase_.sublma_start[pos]) {
      del_a_sub = true;
    }
    if (del_a_sub) {
      c_phrase_.sublma_start[pos - 1] = c_phrase_.sublma_start[pos];
    }
  }
  if (del_a_sub) c_phrase_.sublma_num -= 1;

  return;
}

void MatrixSearch::get_spl_start_id() {
  lma_id_num_ = 0;
  lma_start_[0] = 0;

  spl_id_num_ = 0;
  spl_start_[0] = 0;
  if (!inited_ || 0 == pys_decoded_len_ || 0 == matrix_[pys_decoded_len_].mtrx_nd_num) return;

  // Calculate number of lemmas and spellings
  // Only scan those part which is not fixed.
  lma_id_num_ = fixed_lmas_;
  spl_id_num_ = fixed_hzs_;

  MatrixNode* mtrx_nd = mtrx_nd_pool_ + matrix_[pys_decoded_len_].mtrx_nd_pos;
  while (mtrx_nd != mtrx_nd_pool_) {
    if (fixed_hzs_ > 0) {
      if (mtrx_nd->step <= spl_start_[fixed_hzs_]) break;
    }

    // Update the spelling segamentation information
    unsigned char word_splstr_len = 0;
    PoolPosType dmi_fr = mtrx_nd->dmi_fr;
    if ((PoolPosType)-1 != dmi_fr) word_splstr_len = dmi_pool_[dmi_fr].splstr_len;

    while ((PoolPosType)-1 != dmi_fr) {
      spl_start_[spl_id_num_ + 1] =
          mtrx_nd->step - (word_splstr_len - dmi_pool_[dmi_fr].splstr_len);
      spl_id_[spl_id_num_] = dmi_pool_[dmi_fr].spl_id;
      spl_id_num_++;
      dmi_fr = dmi_pool_[dmi_fr].dmi_fr;
    }

    // Update the lemma segmentation information
    lma_start_[lma_id_num_ + 1] = spl_id_num_;
    lma_id_[lma_id_num_] = mtrx_nd->id;
    lma_id_num_++;

    mtrx_nd = mtrx_nd->from;
  }

  // Reverse the result of spelling info
  for (unsigned pos = fixed_hzs_; pos < fixed_hzs_ + (spl_id_num_ - fixed_hzs_ + 1) / 2; pos++) {
    if (spl_id_num_ + fixed_hzs_ - pos != pos + 1) {
      spl_start_[pos + 1] ^= spl_start_[spl_id_num_ - pos + fixed_hzs_];
      spl_start_[spl_id_num_ - pos + fixed_hzs_] ^= spl_start_[pos + 1];
      spl_start_[pos + 1] ^= spl_start_[spl_id_num_ - pos + fixed_hzs_];

      spl_id_[pos] ^= spl_id_[spl_id_num_ + fixed_hzs_ - pos - 1];
      spl_id_[spl_id_num_ + fixed_hzs_ - pos - 1] ^= spl_id_[pos];
      spl_id_[pos] ^= spl_id_[spl_id_num_ + fixed_hzs_ - pos - 1];
    }
  }

  // Reverse the result of lemma info
  for (unsigned pos = fixed_lmas_; pos < fixed_lmas_ + (lma_id_num_ - fixed_lmas_ + 1) / 2; pos++) {
    assert(lma_id_num_ + fixed_lmas_ - pos - 1 >= pos);

    if (lma_id_num_ + fixed_lmas_ - pos > pos + 1) {
      lma_start_[pos + 1] ^= lma_start_[lma_id_num_ - pos + fixed_lmas_];
      lma_start_[lma_id_num_ - pos + fixed_lmas_] ^= lma_start_[pos + 1];
      lma_start_[pos + 1] ^= lma_start_[lma_id_num_ - pos + fixed_lmas_];

      lma_id_[pos] ^= lma_id_[lma_id_num_ - 1 - pos + fixed_lmas_];
      lma_id_[lma_id_num_ - 1 - pos + fixed_lmas_] ^= lma_id_[pos];
      lma_id_[pos] ^= lma_id_[lma_id_num_ - 1 - pos + fixed_lmas_];
    }
  }

  for (unsigned pos = fixed_lmas_ + 1; pos <= lma_id_num_; pos++) {
    if (pos < lma_id_num_)
      lma_start_[pos] = lma_start_[pos - 1] + (lma_start_[pos] - lma_start_[pos + 1]);
    else
      lma_start_[pos] = lma_start_[pos - 1] + lma_start_[pos] - lma_start_[fixed_lmas_];
  }

  // Find the last fixed position
  fixed_hzs_ = 0;
  for (unsigned pos = spl_id_num_; pos > 0; pos--) {
    if (NULL != matrix_[spl_start_[pos]].mtrx_nd_fixed) {
      fixed_hzs_ = pos;
      break;
    }
  }

  return;
}

unsigned MatrixSearch::get_spl_start(const uint16*& spl_start) {
  get_spl_start_id();
  spl_start = spl_start_;
  return spl_id_num_;
}

unsigned MatrixSearch::extend_dmi(DictExtPara* dep, DictMatchInfo* dmi_s) {
  if (dmi_pool_used_ >= kDmiPoolSize) return 0;

  if (dmi_c_phrase_) return extend_dmi_c(dep, dmi_s);

  LpiCache& lpi_cache = LpiCache::get_instance();
  uint16 splid = dep->splids[dep->splids_extended];

  bool cached = false;
  if (0 == dep->splids_extended) cached = lpi_cache.is_cached(splid);

  // 1. If this is a half Id, get its corresponding full starting Id and
  // number of full Id.
  unsigned ret_val = 0;
  //  PoolPosType mtrx_dmi_fr = (PoolPosType)-1;  // From which dmi node

  lpi_total_ = 0;

  MileStoneHandle from_h[3];
  from_h[0] = 0;
  from_h[1] = 0;

  if (0 != dep->splids_extended) {
    from_h[0] = dmi_s->dict_handles[0];
    from_h[1] = dmi_s->dict_handles[1];
  }

  // 2. Begin exgtending in the system dictionary
  unsigned lpi_num = 0;
  MileStoneHandle handles[2];
  handles[0] = handles[1] = 0;
  if (from_h[0] > 0 || NULL == dmi_s) {
    handles[0] = dict_trie_->extend_dict(from_h[0], dep, lpi_items_, kMaxLmaPsbItems, &lpi_num);
  }
  if (handles[0] > 0) lpi_total_ = lpi_num;

  if (NULL == dmi_s) {  // from root
    assert(0 != handles[0]);
    //    mtrx_dmi_fr = dmi_pool_used_;
  }

  if (0 != handles[0] || 0 != handles[1]) {
    if (dmi_pool_used_ >= kDmiPoolSize) return 0;

    DictMatchInfo* dmi_add = dmi_pool_ + dmi_pool_used_;
    if (NULL == dmi_s) {
      fill_dmi(dmi_add, handles, (PoolPosType)-1, splid, 1, 1, dep->splid_end_split, dep->ext_len,
               spl_trie_->is_half_id(splid) ? 0 : 1);
    } else {
      fill_dmi(dmi_add, handles, dmi_s - dmi_pool_, splid, 1, dmi_s->dict_level + 1,
               dep->splid_end_split, dmi_s->splstr_len + dep->ext_len,
               spl_trie_->is_half_id(splid) ? 0 : dmi_s->all_full_id);
    }

    ret_val = 1;
  }

  if (!cached) {
    if (0 == lpi_total_) return ret_val;

    myqsort(lpi_items_, lpi_total_, sizeof(LmaPsbItem), cmp_lpi_with_psb);
    if (NULL == dmi_s && spl_trie_->is_half_id(splid))
      lpi_total_ = lpi_cache.put_cache(splid, lpi_items_, lpi_total_);
  } else {
    assert(spl_trie_->is_half_id(splid));
    lpi_total_ = lpi_cache.get_cache(splid, lpi_items_, kMaxLmaPsbItems);
  }

  return ret_val;
}

unsigned MatrixSearch::extend_dmi_c(DictExtPara* dep, DictMatchInfo* dmi_s) {
  lpi_total_ = 0;

  uint16 pos = dep->splids_extended;
  assert(dmi_c_phrase_);
  if (pos >= c_phrase_.length) return 0;

  uint16 splid = dep->splids[pos];
  if (splid == c_phrase_.spl_ids[pos]) {
    DictMatchInfo* dmi_add = dmi_pool_ + dmi_pool_used_;
    MileStoneHandle handles[2];  // Actually never used.
    if (NULL == dmi_s)
      fill_dmi(dmi_add, handles, (PoolPosType)-1, splid, 1, 1, dep->splid_end_split, dep->ext_len,
               spl_trie_->is_half_id(splid) ? 0 : 1);
    else
      fill_dmi(dmi_add, handles, dmi_s - dmi_pool_, splid, 1, dmi_s->dict_level + 1,
               dep->splid_end_split, dmi_s->splstr_len + dep->ext_len,
               spl_trie_->is_half_id(splid) ? 0 : dmi_s->all_full_id);

    if (pos == c_phrase_.length - 1) {
      lpi_items_[0].id = kLemmaIdComposing;
      lpi_items_[0].psb = 0;  // 0 is bigger than normal lemma score.
      lpi_total_ = 1;
    }
    return 1;
  }
  return 0;
}

unsigned MatrixSearch::extend_mtrx_nd(MatrixNode* mtrx_nd, LmaPsbItem lpi_items[], unsigned lpi_num,
                                      PoolPosType dmi_fr, unsigned res_row) {
  assert(NULL != mtrx_nd);
  matrix_[res_row].mtrx_nd_fixed = NULL;

  if (mtrx_nd_pool_used_ >= kMtrxNdPoolSize - kMaxNodeARow) return 0;

  if (0 == mtrx_nd->step) {
    // Because the list is sorted, if the source step is 0, it is only
    // necessary to pick up the first kMaxNodeARow items.
    if (lpi_num > kMaxNodeARow) lpi_num = kMaxNodeARow;
  }

  MatrixNode* mtrx_nd_res_min = mtrx_nd_pool_ + matrix_[res_row].mtrx_nd_pos;
  for (unsigned pos = 0; pos < lpi_num; pos++) {
    float score = mtrx_nd->score + lpi_items[pos].psb;
    if (pos > 0 && score - PRUMING_SCORE > mtrx_nd_res_min->score) break;

    // Try to add a new node
    unsigned mtrx_nd_num = matrix_[res_row].mtrx_nd_num;
    MatrixNode* mtrx_nd_res = mtrx_nd_res_min + mtrx_nd_num;
    bool replace = false;
    // Find its position
    while (mtrx_nd_res > mtrx_nd_res_min && score < (mtrx_nd_res - 1)->score) {
      if (static_cast<unsigned>(mtrx_nd_res - mtrx_nd_res_min) < kMaxNodeARow)
        *mtrx_nd_res = *(mtrx_nd_res - 1);
      mtrx_nd_res--;
      replace = true;
    }
    if (replace || (mtrx_nd_num < kMaxNodeARow &&
                    matrix_[res_row].mtrx_nd_pos + mtrx_nd_num < kMtrxNdPoolSize)) {
      mtrx_nd_res->id = lpi_items[pos].id;
      mtrx_nd_res->score = score;
      mtrx_nd_res->from = mtrx_nd;
      mtrx_nd_res->dmi_fr = dmi_fr;
      mtrx_nd_res->step = res_row;
      if (matrix_[res_row].mtrx_nd_num < kMaxNodeARow) matrix_[res_row].mtrx_nd_num++;
    }
  }
  return matrix_[res_row].mtrx_nd_num;
}

PoolPosType MatrixSearch::match_dmi(unsigned step_to, uint16 spl_ids[], uint16 spl_id_num) {
  if (pys_decoded_len_ < step_to || 0 == matrix_[step_to].dmi_num) {
    return static_cast<PoolPosType>(-1);
  }

  for (PoolPosType dmi_pos = 0; dmi_pos < matrix_[step_to].dmi_num; dmi_pos++) {
    DictMatchInfo* dmi = dmi_pool_ + matrix_[step_to].dmi_pos + dmi_pos;

    if (dmi->dict_level != spl_id_num) continue;

    bool matched = true;
    for (uint16 spl_pos = 0; spl_pos < spl_id_num; spl_pos++) {
      if (spl_ids[spl_id_num - spl_pos - 1] != dmi->spl_id) {
        matched = false;
        break;
      }

      dmi = dmi_pool_ + dmi->dmi_fr;
    }
    if (matched) {
      return matrix_[step_to].dmi_pos + dmi_pos;
    }
  }

  return static_cast<PoolPosType>(-1);
}

char16* MatrixSearch::get_candidate0(char16* cand_str, unsigned max_len, uint16* retstr_len,
                                     bool only_unfixed) {
  if (pys_decoded_len_ == 0 || matrix_[pys_decoded_len_].mtrx_nd_num == 0) return NULL;

  LemmaIdType idxs[kMaxRowNum];
  unsigned id_num = 0;

  MatrixNode* mtrx_nd = mtrx_nd_pool_ + matrix_[pys_decoded_len_].mtrx_nd_pos;

  while (mtrx_nd != NULL) {
    idxs[id_num] = mtrx_nd->id;
    id_num++;

    mtrx_nd = mtrx_nd->from;
  }

  unsigned ret_pos = 0;
  do {
    id_num--;
    if (0 == idxs[id_num]) continue;

    char16 str[kMaxLemmaSize + 1];
    uint16 str_len = get_lemma_str(idxs[id_num], str, kMaxLemmaSize + 1);
    if (str_len > 0 && ((!only_unfixed && max_len - ret_pos > str_len) ||
                        (only_unfixed && max_len - ret_pos + fixed_hzs_ > str_len))) {
      if (!only_unfixed)
        utf16_strncpy(cand_str + ret_pos, str, str_len);
      else if (ret_pos >= fixed_hzs_)
        utf16_strncpy(cand_str + ret_pos - fixed_hzs_, str, str_len);

      ret_pos += str_len;
    } else {
      return NULL;
    }
  } while (id_num != 0);

  if (!only_unfixed) {
    if (NULL != retstr_len) *retstr_len = ret_pos;
    cand_str[ret_pos] = (char16)'\0';
  } else {
    if (NULL != retstr_len) *retstr_len = ret_pos - fixed_hzs_;
    cand_str[ret_pos - fixed_hzs_] = (char16)'\0';
  }
  return cand_str;
}

unsigned MatrixSearch::get_lpis(const uint16* splid_str, unsigned splid_str_len,
                                LmaPsbItem* lma_buf, unsigned max_lma_buf, const char16* pfullsent,
                                bool sort_by_psb) {
  if (splid_str_len > kMaxLemmaSize) return 0;

  unsigned num1 = dict_trie_->get_lpis(splid_str, splid_str_len, lma_buf, max_lma_buf);
  unsigned num2 = 0;

  unsigned num = num1 + num2;

  if (0 == num) return 0;

  // Remove repeated items.
  if (splid_str_len > 1) {
    LmaPsbStrItem* lpsis = reinterpret_cast<LmaPsbStrItem*>(lma_buf + num);
    unsigned lpsi_num = (max_lma_buf - num) * sizeof(LmaPsbItem) / sizeof(LmaPsbStrItem);
    assert(lpsi_num > num);
    if (num > lpsi_num) num = lpsi_num;
    lpsi_num = num;

    for (unsigned pos = 0; pos < lpsi_num; pos++) {
      lpsis[pos].lpi = lma_buf[pos];
      get_lemma_str(lma_buf[pos].id, lpsis[pos].str, kMaxLemmaSize + 1);
    }

    myqsort(lpsis, lpsi_num, sizeof(LmaPsbStrItem), cmp_lpsi_with_str);

    unsigned remain_num = 0;
    for (unsigned pos = 0; pos < lpsi_num; pos++) {
      if (pos > 0 && utf16_strcmp(lpsis[pos].str, lpsis[pos - 1].str) == 0) {
        if (lpsis[pos].lpi.psb < lpsis[pos - 1].lpi.psb) {
          //
          // Jim: input nishis will trigger this assert.
          // https://github.com/zlgopen/awtk/issues/563
          //
          //assert(remain_num > 0);
          //lma_buf[remain_num - 1] = lpsis[pos].lpi;
          //
          if (remain_num > 0) {
            lma_buf[remain_num - 1] = lpsis[pos].lpi;
          }
        }
        continue;
      }
      if (NULL != pfullsent && utf16_strcmp(lpsis[pos].str, pfullsent) == 0) continue;

      lma_buf[remain_num] = lpsis[pos].lpi;
      remain_num++;
    }

    // Update the result number
    num = remain_num;
  } else {
    // For single character, some characters have more than one spelling, for
    // example, "de" and "di" are all valid for a Chinese character, so when
    // the user input  "d", repeated items are generated.
    // For single character lemmas, Hanzis will be gotten
    for (unsigned pos = 0; pos < num; pos++) {
      char16 hanzis[2];
      get_lemma_str(lma_buf[pos].id, hanzis, 2);
      lma_buf[pos].hanzi = hanzis[0];
    }

    myqsort(lma_buf, num, sizeof(LmaPsbItem), cmp_lpi_with_hanzi);

    unsigned remain_num = 0;
    for (unsigned pos = 0; pos < num; pos++) {
      if (pos > 0 && lma_buf[pos].hanzi == lma_buf[pos - 1].hanzi) {
        if (NULL != pfullsent && static_cast<char16>(0) == pfullsent[1] &&
            lma_buf[pos].hanzi == pfullsent[0])
          continue;

        if (lma_buf[pos].psb < lma_buf[pos - 1].psb) {
          assert(remain_num > 0);
          assert(lma_buf[remain_num - 1].hanzi == lma_buf[pos].hanzi);
          lma_buf[remain_num - 1] = lma_buf[pos];
        }
        continue;
      }
      if (NULL != pfullsent && static_cast<char16>(0) == pfullsent[1] &&
          lma_buf[pos].hanzi == pfullsent[0])
        continue;

      lma_buf[remain_num] = lma_buf[pos];
      remain_num++;
    }

    num = remain_num;
  }

  if (sort_by_psb) {
    myqsort(lma_buf, num, sizeof(LmaPsbItem), cmp_lpi_with_psb);
  }
  return num;
}

uint16 MatrixSearch::get_lemma_str(LemmaIdType id_lemma, char16* str_buf, uint16 str_max) {
  uint16 str_len = 0;

  if (is_system_lemma(id_lemma)) {
    str_len = dict_trie_->get_lemma_str(id_lemma, str_buf, str_max);
  } else if (is_user_lemma(id_lemma)) {
    str_len = 0;
    str_buf[0] = static_cast<char16>('\0');
  } else if (is_composing_lemma(id_lemma)) {
    if (str_max <= 1) return 0;
    str_len = c_phrase_.sublma_start[c_phrase_.sublma_num];
    if (str_len > str_max - 1) str_len = str_max - 1;
    utf16_strncpy(str_buf, c_phrase_.chn_str, str_len);
    str_buf[str_len] = (char16)'\0';
    return str_len;
  }

  return str_len;
}

uint16 MatrixSearch::get_lemma_splids(LemmaIdType id_lemma, uint16* splids, uint16 splids_max,
                                      bool arg_valid) {
  uint16 splid_num = 0;

  if (arg_valid) {
    for (splid_num = 0; splid_num < splids_max; splid_num++) {
      if (spl_trie_->is_half_id(splids[splid_num])) break;
    }
    if (splid_num == splids_max) return splid_num;
  }

  if (is_system_lemma(id_lemma)) {
    splid_num = dict_trie_->get_lemma_splids(id_lemma, splids, splids_max, arg_valid);
  } else if (is_user_lemma(id_lemma)) {
    splid_num = 0;
  } else if (is_composing_lemma(id_lemma)) {
    if (c_phrase_.length > splids_max) {
      return 0;
    }
    for (uint16 pos = 0; pos < c_phrase_.length; pos++) {
      splids[pos] = c_phrase_.spl_ids[pos];
      if (spl_trie_->is_half_id(splids[pos])) {
        return 0;
      }
    }
  }
  return splid_num;
}

unsigned MatrixSearch::inner_predict(const char16* fixed_buf, uint16 fixed_len,
                                     char16 predict_buf[][kMaxPredictSize + 1], unsigned buf_len) {
  unsigned res_total = 0;
  memset(npre_items_, 0, sizeof(NPredictItem) * npre_items_len_);
  // In order to shorten the comments, j-character candidates predicted by
  // i-character prefix are called P(i,j). All candiates predicted by
  // i-character prefix are called P(i,*)
  // Step 1. Get P(kMaxPredictSize, *) and sort them, here
  // P(kMaxPredictSize, *) == P(kMaxPredictSize, 1)
  for (unsigned len = fixed_len; len > 0; len--) {
    // How many blank items are available
    unsigned this_max = npre_items_len_ - res_total;
    unsigned res_this;
    // If the history is longer than 1, and we can not get prediction from
    // lemmas longer than 2, in this case, we will add lemmas with
    // highest scores as the prediction result.
    if (fixed_len > 1 && 1 == len && 0 == res_total) {
      // Try to find if recent n (n>1) characters can be a valid lemma in system
      // dictionary.
      bool nearest_n_word = false;
      for (unsigned nlen = 2; nlen <= fixed_len; nlen++) {
        if (dict_trie_->get_lemma_id(fixed_buf + fixed_len - nlen, nlen) > 0) {
          nearest_n_word = true;
          break;
        }
      }
      res_this = dict_trie_->predict_top_lmas(nearest_n_word ? len : 0, npre_items_ + res_total,
                                              this_max, res_total);
      res_total += res_this;
    }

    // How many blank items are available
    this_max = npre_items_len_ - res_total;
    res_this = 0;
    if (!kOnlyUserDictPredict) {
      res_this = dict_trie_->predict(fixed_buf + fixed_len - len, len, npre_items_ + res_total,
                                     this_max, res_total);
    }

    if (kPredictLimitGt1) {
      myqsort(npre_items_ + res_total, res_this, sizeof(NPredictItem), cmp_npre_by_score);

      if (len > 3) {
        if (res_this > kMaxPredictNumByGt3) res_this = kMaxPredictNumByGt3;
      } else if (3 == len) {
        if (res_this > kMaxPredictNumBy3) res_this = kMaxPredictNumBy3;
      } else if (2 == len) {
        if (res_this > kMaxPredictNumBy2) res_this = kMaxPredictNumBy2;
      }
    }

    res_total += res_this;
  }

  res_total = remove_duplicate_npre(npre_items_, res_total);

  if (kPreferLongHistoryPredict) {
    myqsort(npre_items_, res_total, sizeof(NPredictItem), cmp_npre_by_hislen_score);
  } else {
    myqsort(npre_items_, res_total, sizeof(NPredictItem), cmp_npre_by_score);
  }

  if (buf_len < res_total) {
    res_total = buf_len;
  }

  for (unsigned i = 0; i < res_total; i++) {
    utf16_strncpy(predict_buf[i], npre_items_[i].pre_hzs, kMaxPredictSize);
    predict_buf[i][kMaxPredictSize] = '\0';
  }

  return res_total;
}

unsigned MatrixSearch::get_predicts(const char16 fixed_buf[],
                                    char16 predict_buf[][kMaxPredictSize + 1], unsigned buf_len) {
  unsigned fixed_len = utf16_strlen(fixed_buf);
  if (0 == fixed_len || fixed_len > kMaxPredictSize || 0 == buf_len) return 0;

  return inner_predict(fixed_buf, fixed_len, predict_buf, buf_len);
}

}  // namespace ime_pinyin
