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
#include <string.h>
#include <assert.h>
#include "../include/dictdef.h"

#include "../include/spellingtrie.h"

namespace ime_pinyin {

SpellingTrie* SpellingTrie::instance_ = NULL;

// z/c/s is for Zh/Ch/Sh
const char SpellingTrie::kHalfId2Sc_[kFullSplIdStart + 1] = "0ABCcDEFGHIJKLMNOPQRSsTUVWXYZz";

// Bit 0 : is it a Shengmu char?
// Bit 1 : is it a Yunmu char? (one char is a Yunmu)
// Bit 2 : is it enabled in ShouZiMu(first char) mode?
unsigned char SpellingTrie::char_flags_[] = {
    // a    b      c     d     e     f     g
    0x02, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01,
    // h    i     j      k     l     m    n
    0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01,
    // o    p     q      r     s     t
    0x02, 0x01, 0x01, 0x01, 0x01, 0x01,
    // u    v     w      x     y     z
    0x00, 0x00, 0x01, 0x01, 0x01, 0x01};

int compare_spl(const void* p1, const void* p2) {
  return strcmp((const char*)(p1), (const char*)(p2));
}

SpellingTrie::SpellingTrie() {
  spelling_buf_ = NULL;
  spelling_size_ = 0;
  spelling_num_ = 0;
  spl_ym_ids_ = NULL;
  splstr_queried_ = NULL;
  splstr16_queried_ = NULL;
  root_ = NULL;
  dumb_node_ = NULL;
  splitter_node_ = NULL;
  instance_ = NULL;
  ym_buf_ = NULL;
  f2h_ = NULL;

  szm_enable_shm(true);
  szm_enable_ym(true);
}

SpellingTrie::~SpellingTrie() {
	if (NULL != spelling_buf_) {
		delete[] spelling_buf_;
    spelling_buf_ = NULL;
	}

	if (NULL != splstr_queried_) {
		delete[] splstr_queried_;
    splstr_queried_ = NULL;
	}

	if (NULL != splstr16_queried_) {
		delete[] splstr16_queried_;
		splstr16_queried_ = NULL;
	}

	if (NULL != spl_ym_ids_) {
		delete[] spl_ym_ids_;
    spl_ym_ids_ = NULL;
	}

  if (NULL != root_) {
    free_son_trie(root_);
    delete root_;
    root_ = NULL;
  }

  if (NULL != dumb_node_) {
    delete[] dumb_node_;
    dumb_node_ = NULL;
  }

  if (NULL != splitter_node_) {
    delete[] splitter_node_;
    splitter_node_ = NULL;
  }

  if (NULL != ym_buf_) {
	  delete[] ym_buf_;
    ym_buf_ = NULL;
  }

  if (NULL != f2h_) {
	  delete[] f2h_;
    f2h_ = NULL;
  }
}

bool SpellingTrie::if_valid_id_update(uint16* splid) const {
  if (NULL == splid || 0 == *splid) return false;

  if (*splid >= kFullSplIdStart) return true;
  if (*splid < kFullSplIdStart) {
    char ch = kHalfId2Sc_[*splid];
    if (ch > 'Z') {
      return true;
    } else {
      if (szm_is_enabled(ch)) {
        return true;
      } else if (is_yunmu_char(ch)) {
        assert(h2f_num_[*splid] > 0);
        *splid = h2f_start_[*splid];
        return true;
      }
    }
  }
  return false;
}

bool SpellingTrie::is_half_id(uint16 splid) const {
  if (0 == splid || splid >= kFullSplIdStart) return false;

  return true;
}

bool SpellingTrie::is_full_id(uint16 splid) const {
  if (splid < kFullSplIdStart || splid >= kFullSplIdStart + spelling_num_) return false;
  return true;
}

bool SpellingTrie::half_full_compatible(uint16 half_id, uint16 full_id) const {
  uint16 half_fr_full = full_to_half(full_id);

  if (half_fr_full == half_id) return true;

  // &~0x20 is used to conver the char to upper case.
  // So that Zh/Ch/Sh(whose char is z/c/s) can be matched with Z/C/S.
  char ch_f = (kHalfId2Sc_[half_fr_full] & (~0x20));
  char ch_h = kHalfId2Sc_[half_id];
  if (ch_f == ch_h) return true;

  return false;
}

bool SpellingTrie::is_half_id_yunmu(uint16 splid) const {
  if (0 == splid || splid >= kFullSplIdStart) return false;

  char ch = kHalfId2Sc_[splid];
  // If ch >= 'a', that means the half id is one of Zh/Ch/Sh
  if (ch >= 'a') {
    return false;
  }

  return char_flags_[ch - 'A'] & kHalfIdYunmuMask;
}

bool SpellingTrie::is_shengmu_char(char ch) const {
  return char_flags_[ch - 'A'] & kHalfIdShengmuMask;
}

bool SpellingTrie::is_yunmu_char(char ch) const {
  return char_flags_[ch - 'A'] & kHalfIdYunmuMask;
}

bool SpellingTrie::is_szm_char(char ch) const {
  return is_shengmu_char(ch) || is_yunmu_char(ch);
}

bool SpellingTrie::szm_is_enabled(char ch) const {
  return char_flags_[ch - 'A'] & kHalfIdSzmMask;
}

void SpellingTrie::szm_enable_shm(bool enable) {
  if (enable) {
    for (char ch = 'A'; ch <= 'Z'; ch++) {
      if (is_shengmu_char(ch)) char_flags_[ch - 'A'] = char_flags_[ch - 'A'] | kHalfIdSzmMask;
    }
  } else {
    for (char ch = 'A'; ch <= 'Z'; ch++) {
      if (is_shengmu_char(ch))
        char_flags_[ch - 'A'] = char_flags_[ch - 'A'] & (kHalfIdSzmMask ^ 0xff);
    }
  }
}

void SpellingTrie::szm_enable_ym(bool enable) {
  if (enable) {
    for (char ch = 'A'; ch <= 'Z'; ch++) {
      if (is_yunmu_char(ch)) char_flags_[ch - 'A'] = char_flags_[ch - 'A'] | kHalfIdSzmMask;
    }
  } else {
    for (char ch = 'A'; ch <= 'Z'; ch++) {
      if (is_yunmu_char(ch))
        char_flags_[ch - 'A'] = char_flags_[ch - 'A'] & (kHalfIdSzmMask ^ 0xff);
    }
  }
}

bool SpellingTrie::is_szm_enabled(char ch) const {
  return char_flags_[ch - 'A'] & kHalfIdSzmMask;
}

const SpellingTrie* SpellingTrie::get_cpinstance() {
  return &get_instance();
}

SpellingTrie& SpellingTrie::get_instance() {
  if (NULL == instance_) instance_ = new SpellingTrie();

  return *instance_;
}

uint16 SpellingTrie::half2full_num(uint16 half_id) const {
  if (NULL == root_ || half_id >= kFullSplIdStart) return 0;
  return h2f_num_[half_id];
}

uint16 SpellingTrie::half_to_full(uint16 half_id, uint16* spl_id_start) const {
  if (NULL == spl_id_start || NULL == root_ || half_id >= kFullSplIdStart) return 0;

  *spl_id_start = h2f_start_[half_id];
  return h2f_num_[half_id];
}

uint16 SpellingTrie::full_to_half(uint16 full_id) const {
  if (NULL == root_ || full_id < kFullSplIdStart || full_id > spelling_num_ + kFullSplIdStart)
    return 0;

  return f2h_[full_id - kFullSplIdStart];
}

void SpellingTrie::free_son_trie(SpellingNode* node) {
  if (NULL == node) return;

  for (unsigned pos = 0; pos < node->num_of_son; pos++) {
    free_son_trie(node->first_son + pos);
  }

  if (NULL != node->first_son) delete[] node->first_son;
}

bool SpellingTrie::construct(const char* spelling_arr, unsigned item_size, unsigned item_num,
                             float score_amplifier, unsigned char average_score) {
  if (spelling_arr == NULL) return false;

  memset(h2f_start_, 0, sizeof(uint16) * kFullSplIdStart);
  memset(h2f_num_, 0, sizeof(uint16) * kFullSplIdStart);

  // If the arr is the same as the buf, means this function is called by
  // load_table(), the table data are ready; otherwise the array should be
  // saved.
  if (spelling_arr != spelling_buf_) {
    if (NULL != spelling_buf_) delete[] spelling_buf_;
    spelling_buf_ = new char[item_size * item_num];
    if (NULL == spelling_buf_) return false;
    memcpy(spelling_buf_, spelling_arr, sizeof(char) * item_size * item_num);
  }

  spelling_size_ = item_size;
  spelling_num_ = item_num;

  score_amplifier_ = score_amplifier;
  average_score_ = average_score;

  if (NULL != splstr_queried_) delete[] splstr_queried_;
  splstr_queried_ = new char[spelling_size_];
  if (NULL == splstr_queried_) return false;

  if (NULL != splstr16_queried_) delete[] splstr16_queried_;
  splstr16_queried_ = new char16[spelling_size_];
  if (NULL == splstr16_queried_) return false;

  // First, sort the buf to ensure they are in ascendant order
  qsort(spelling_buf_, spelling_num_, spelling_size_, compare_spl);

  root_ = new SpellingNode();
  memset(root_, 0, sizeof(SpellingNode));

  dumb_node_ = new SpellingNode();
  memset(dumb_node_, 0, sizeof(SpellingNode));
  dumb_node_->score = average_score_;

  splitter_node_ = new SpellingNode();
  memset(splitter_node_, 0, sizeof(SpellingNode));
  splitter_node_->score = average_score_;

  memset(level1_sons_, 0, sizeof(SpellingNode*) * kValidSplCharNum);

  root_->first_son = construct_spellings_subset(0, spelling_num_, 0, root_);

  // Root's score should be cleared.
  root_->score = 0;

  if (NULL == root_->first_son) return false;

  h2f_start_[0] = h2f_num_[0] = 0;

  if (!build_f2h()) return false;

  return true;
}

SpellingNode* SpellingTrie::construct_spellings_subset(unsigned item_start, unsigned item_end,
                                                       unsigned level, SpellingNode* parent) {
  if (level >= spelling_size_ || item_end <= item_start || NULL == parent) return NULL;

  SpellingNode* first_son = NULL;
  uint16 num_of_son = 0;
  unsigned char min_son_score = 255;

  const char* spelling_last_start = spelling_buf_ + spelling_size_ * item_start;
  char char_for_node = spelling_last_start[level];
  assert((char_for_node >= 'A' && char_for_node <= 'Z') || 'h' == char_for_node);

  // Scan the array to find how many sons
  for (unsigned i = item_start + 1; i < item_end; i++) {
    const char* spelling_current = spelling_buf_ + spelling_size_ * i;
    char char_current = spelling_current[level];
    if (char_current != char_for_node) {
      num_of_son++;
      char_for_node = char_current;
    }
  }
  num_of_son++;

  first_son = new SpellingNode[num_of_son];
  memset(first_son, 0, sizeof(SpellingNode) * num_of_son);

  // Now begin construct tree
  unsigned son_pos = 0;

  spelling_last_start = spelling_buf_ + spelling_size_ * item_start;
  char_for_node = spelling_last_start[level];

  bool spelling_endable = true;
  if (spelling_last_start[level + 1] != '\0') spelling_endable = false;

  unsigned item_start_next = item_start;

  for (unsigned i = item_start + 1; i < item_end; i++) {
    const char* spelling_current = spelling_buf_ + spelling_size_ * i;
    char char_current = spelling_current[level];
    assert(is_valid_spl_char(char_current));

    if (char_current != char_for_node) {
      // Construct a node
      SpellingNode* node_current = first_son + son_pos;
      node_current->char_this_node = char_for_node;

      // For quick search in the first level
      if (0 == level) level1_sons_[char_for_node - 'A'] = node_current;

      if (spelling_endable) {
        node_current->spelling_idx = kFullSplIdStart + item_start_next;
      }

      if (spelling_last_start[level + 1] != '\0' || i - item_start_next > 1) {
        unsigned real_start = item_start_next;
        if (spelling_last_start[level + 1] == '\0') real_start++;

        node_current->first_son =
            construct_spellings_subset(real_start, i, level + 1, node_current);

        if (real_start == item_start_next + 1) {
          uint16 score_this = static_cast<unsigned char>(spelling_last_start[spelling_size_ - 1]);
          if (score_this < node_current->score) node_current->score = score_this;
        }
      } else {
        node_current->first_son = NULL;
        node_current->score = static_cast<unsigned char>(spelling_last_start[spelling_size_ - 1]);
      }

      if (node_current->score < min_son_score) min_son_score = node_current->score;

      bool is_half = false;
      if (level == 0 && is_szm_char(char_for_node)) {
        node_current->spelling_idx = static_cast<uint16>(char_for_node - 'A' + 1);

        if (char_for_node > 'C') node_current->spelling_idx++;
        if (char_for_node > 'S') node_current->spelling_idx++;

        h2f_num_[node_current->spelling_idx] = i - item_start_next;
        is_half = true;
      } else if (level == 1 && char_for_node == 'h') {
        char ch_level0 = spelling_last_start[0];
        uint16 part_id = 0;
        if (ch_level0 == 'C')
          part_id = 'C' - 'A' + 1 + 1;
        else if (ch_level0 == 'S')
          part_id = 'S' - 'A' + 1 + 2;
        else if (ch_level0 == 'Z')
          part_id = 'Z' - 'A' + 1 + 3;
        if (0 != part_id) {
          node_current->spelling_idx = part_id;
          h2f_num_[node_current->spelling_idx] = i - item_start_next;
          is_half = true;
        }
      }

      if (is_half) {
        if (h2f_num_[node_current->spelling_idx] > 0)
          h2f_start_[node_current->spelling_idx] = item_start_next + kFullSplIdStart;
        else
          h2f_start_[node_current->spelling_idx] = 0;
      }

      // for next sibling
      spelling_last_start = spelling_current;
      char_for_node = char_current;
      item_start_next = i;
      spelling_endable = true;
      if (spelling_current[level + 1] != '\0') spelling_endable = false;

      son_pos++;
    }
  }

  // the last one
  SpellingNode* node_current = first_son + son_pos;
  node_current->char_this_node = char_for_node;

  // For quick search in the first level
  if (0 == level) level1_sons_[char_for_node - 'A'] = node_current;

  if (spelling_endable) {
    node_current->spelling_idx = kFullSplIdStart + item_start_next;
  }

  if (spelling_last_start[level + 1] != '\0' || item_end - item_start_next > 1) {
    unsigned real_start = item_start_next;
    if (spelling_last_start[level + 1] == '\0') real_start++;

    node_current->first_son =
        construct_spellings_subset(real_start, item_end, level + 1, node_current);

    if (real_start == item_start_next + 1) {
      uint16 score_this = static_cast<unsigned char>(spelling_last_start[spelling_size_ - 1]);
      if (score_this < node_current->score) node_current->score = score_this;
    }
  } else {
    node_current->first_son = NULL;
    node_current->score = static_cast<unsigned char>(spelling_last_start[spelling_size_ - 1]);
  }

  if (node_current->score < min_son_score) min_son_score = node_current->score;

  assert(son_pos + 1 == num_of_son);

  bool is_half = false;
  if (level == 0 && szm_is_enabled(char_for_node)) {
    node_current->spelling_idx = static_cast<uint16>(char_for_node - 'A' + 1);

    if (char_for_node > 'C') node_current->spelling_idx++;
    if (char_for_node > 'S') node_current->spelling_idx++;

    h2f_num_[node_current->spelling_idx] = item_end - item_start_next;
    is_half = true;
  } else if (level == 1 && char_for_node == 'h') {
    char ch_level0 = spelling_last_start[0];
    uint16 part_id = 0;
    if (ch_level0 == 'C')
      part_id = 'C' - 'A' + 1 + 1;
    else if (ch_level0 == 'S')
      part_id = 'S' - 'A' + 1 + 2;
    else if (ch_level0 == 'Z')
      part_id = 'Z' - 'A' + 1 + 3;
    if (0 != part_id) {
      node_current->spelling_idx = part_id;
      h2f_num_[node_current->spelling_idx] = item_end - item_start_next;
      is_half = true;
    }
  }
  if (is_half) {
    if (h2f_num_[node_current->spelling_idx] > 0)
      h2f_start_[node_current->spelling_idx] = item_start_next + kFullSplIdStart;
    else
      h2f_start_[node_current->spelling_idx] = 0;
  }

  parent->num_of_son = num_of_son;
  parent->score = min_son_score;
  return first_son;
}

bool SpellingTrie::save_spl_trie(FILE* fp) {
  if (NULL == fp || NULL == spelling_buf_) return false;

  if (fwrite(&spelling_size_, sizeof(unsigned), 1, fp) != 1) return false;

  if (fwrite(&spelling_num_, sizeof(unsigned), 1, fp) != 1) return false;

  if (fwrite(&score_amplifier_, sizeof(float), 1, fp) != 1) return false;

  if (fwrite(&average_score_, sizeof(unsigned char), 1, fp) != 1) return false;

  if (fwrite(spelling_buf_, sizeof(char) * spelling_size_, spelling_num_, fp) != spelling_num_)
    return false;

  return true;
}

bool SpellingTrie::load_spl_trie(FILE* fp) {
  if (NULL == fp) return false;

  if (fread(&spelling_size_, sizeof(unsigned), 1, fp) != 1) return false;

  if (fread(&spelling_num_, sizeof(unsigned), 1, fp) != 1) return false;

  if (fread(&score_amplifier_, sizeof(float), 1, fp) != 1) return false;

  if (fread(&average_score_, sizeof(unsigned char), 1, fp) != 1) return false;

  if (NULL != spelling_buf_) delete[] spelling_buf_;

  spelling_buf_ = new char[spelling_size_ * spelling_num_];
  if (NULL == spelling_buf_) return false;

  if (fread(spelling_buf_, sizeof(char) * spelling_size_, spelling_num_, fp) != spelling_num_)
    return false;

  return construct(spelling_buf_, spelling_size_, spelling_num_, score_amplifier_, average_score_);
}

bool SpellingTrie::build_f2h() {
  if (NULL != f2h_) delete[] f2h_;
  f2h_ = new uint16[spelling_num_];
  if (NULL == f2h_) return false;

  for (uint16 hid = 0; hid < kFullSplIdStart; hid++) {
    for (uint16 fid = h2f_start_[hid]; fid < h2f_start_[hid] + h2f_num_[hid]; fid++)
      f2h_[fid - kFullSplIdStart] = hid;
  }

  return true;
}

unsigned SpellingTrie::get_spelling_num() {
  return spelling_num_;
}

uint8 SpellingTrie::get_ym_id(const char* ym_str) {
  if (NULL == ym_str || NULL == ym_buf_) return 0;

  for (uint8 pos = 0; pos < ym_num_; pos++)
    if (strcmp(ym_buf_ + ym_size_ * pos, ym_str) == 0) return pos + 1;

  return 0;
}

const char* SpellingTrie::get_spelling_str(uint16 splid) {
  splstr_queried_[0] = '\0';

  if (splid >= kFullSplIdStart) {
    splid -= kFullSplIdStart;
    snprintf(splstr_queried_, spelling_size_, "%s", spelling_buf_ + splid * spelling_size_);
  } else {
    if (splid == 'C' - 'A' + 1 + 1) {
      snprintf(splstr_queried_, spelling_size_, "%s", "Ch");
    } else if (splid == 'S' - 'A' + 1 + 2) {
      snprintf(splstr_queried_, spelling_size_, "%s", "Sh");
    } else if (splid == 'Z' - 'A' + 1 + 3) {
      snprintf(splstr_queried_, spelling_size_, "%s", "Zh");
    } else {
      if (splid > 'C' - 'A' + 1) splid--;
      if (splid > 'S' - 'A' + 1) splid--;
      splstr_queried_[0] = 'A' + splid - 1;
      splstr_queried_[1] = '\0';
    }
  }
  return splstr_queried_;
}

const char16* SpellingTrie::get_spelling_str16(uint16 splid) {
  splstr16_queried_[0] = '\0';

  if (splid >= kFullSplIdStart) {
    splid -= kFullSplIdStart;
    for (unsigned pos = 0; pos < spelling_size_; pos++) {
      splstr16_queried_[pos] = static_cast<char16>(spelling_buf_[splid * spelling_size_ + pos]);
    }
  } else {
    if (splid == 'C' - 'A' + 1 + 1) {
      splstr16_queried_[0] = static_cast<char16>('C');
      splstr16_queried_[1] = static_cast<char16>('h');
      splstr16_queried_[2] = static_cast<char16>('\0');
    } else if (splid == 'S' - 'A' + 1 + 2) {
      splstr16_queried_[0] = static_cast<char16>('S');
      splstr16_queried_[1] = static_cast<char16>('h');
      splstr16_queried_[2] = static_cast<char16>('\0');
    } else if (splid == 'Z' - 'A' + 1 + 3) {
      splstr16_queried_[0] = static_cast<char16>('Z');
      splstr16_queried_[1] = static_cast<char16>('h');
      splstr16_queried_[2] = static_cast<char16>('\0');
    } else {
      if (splid > 'C' - 'A' + 1) splid--;
      if (splid > 'S' - 'A' + 1) splid--;
      splstr16_queried_[0] = 'A' + splid - 1;
      splstr16_queried_[1] = '\0';
    }
  }
  return splstr16_queried_;
}

unsigned SpellingTrie::get_spelling_str16(uint16 splid, char16* splstr16, unsigned splstr16_len) {
  if (NULL == splstr16 || splstr16_len < kMaxPinyinSize + 1) return 0;

  if (splid >= kFullSplIdStart) {
    splid -= kFullSplIdStart;
    for (unsigned pos = 0; pos <= kMaxPinyinSize; pos++) {
      splstr16[pos] = static_cast<char16>(spelling_buf_[splid * spelling_size_ + pos]);
      if (static_cast<char16>('\0') == splstr16[pos]) {
        return pos;
      }
    }
  } else {
    if (splid == 'C' - 'A' + 1 + 1) {
      splstr16[0] = static_cast<char16>('C');
      splstr16[1] = static_cast<char16>('h');
      splstr16[2] = static_cast<char16>('\0');
      return 2;
    } else if (splid == 'S' - 'A' + 1 + 2) {
      splstr16[0] = static_cast<char16>('S');
      splstr16[1] = static_cast<char16>('h');
      splstr16[2] = static_cast<char16>('\0');
      return 2;
    } else if (splid == 'Z' - 'A' + 1 + 3) {
      splstr16[0] = static_cast<char16>('Z');
      splstr16[1] = static_cast<char16>('h');
      splstr16[2] = static_cast<char16>('\0');
      return 2;
    } else {
      if (splid > 'C' - 'A' + 1) splid--;
      if (splid > 'S' - 'A' + 1) splid--;
      splstr16[0] = 'A' + splid - 1;
      splstr16[1] = '\0';
      return 1;
    }
  }

  // Not reachable.
  return 0;
}

}  // namespace ime_pinyin
