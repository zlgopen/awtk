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
#include "../include/splparser.h"

namespace ime_pinyin {

SpellingParser::SpellingParser() {
  spl_trie_ = SpellingTrie::get_cpinstance();
}

bool SpellingParser::is_valid_to_parse(char ch) {
  return SpellingTrie::is_valid_spl_char(ch);
}

uint16 SpellingParser::splstr_to_idxs(const char *splstr, uint16 str_len,
                                      uint16 spl_idx[], uint16 start_pos[],
                                      uint16 max_size, bool &last_is_pre) {
  if (NULL == splstr || 0 == max_size || 0 == str_len)
    return 0;

  if (!SpellingTrie::is_valid_spl_char(splstr[0]))
    return 0;

  last_is_pre = false;

  const SpellingNode *node_this = spl_trie_->root_;

  uint16 str_pos = 0;
  uint16 idx_num = 0;
  if (NULL != start_pos)
    start_pos[0] = 0;
  bool last_is_splitter = false;

  while (str_pos < str_len) {
    char char_this = splstr[str_pos];
    // all characters outside of [a, z] are considered as splitters
    if (!SpellingTrie::is_valid_spl_char(char_this)) {
      // test if the current node is endable
      uint16 id_this = node_this->spelling_idx;
      if (spl_trie_->if_valid_id_update(&id_this)) {
        spl_idx[idx_num] = id_this;

        idx_num++;
        str_pos++;
        if (NULL != start_pos)
          start_pos[idx_num] = str_pos;
        if (idx_num >= max_size)
          return idx_num;

        node_this = spl_trie_->root_;
        last_is_splitter = true;
        continue;
      } else {
        if (last_is_splitter) {
          str_pos++;
          if (NULL != start_pos)
            start_pos[idx_num] = str_pos;
          continue;
        } else {
          return idx_num;
        }
      }
    }

    last_is_splitter = false;

    SpellingNode *found_son = NULL;

    if (0 == str_pos) {
      if (char_this >= 'a')
        found_son = spl_trie_->level1_sons_[char_this - 'a'];
      else
        found_son = spl_trie_->level1_sons_[char_this - 'A'];
    } else {
      SpellingNode *first_son = node_this->first_son;
      // Because for Zh/Ch/Sh nodes, they are the last in the buffer and
      // frequently used, so we scan from the end.
      for (int i = 0; i < node_this->num_of_son; i++) {
        SpellingNode *this_son = first_son + i;
        if (SpellingTrie::is_same_spl_char(
            this_son->char_this_node, char_this)) {
          found_son = this_son;
          break;
        }
      }
    }

    // found, just move the current node pointer to the the son
    if (NULL != found_son) {
      node_this = found_son;
    } else {
      // not found, test if it is endable
      uint16 id_this = node_this->spelling_idx;
      if (spl_trie_->if_valid_id_update(&id_this)) {
        // endable, remember the index
        spl_idx[idx_num] = id_this;

        idx_num++;
        if (NULL != start_pos)
          start_pos[idx_num] = str_pos;
        if (idx_num >= max_size)
          return idx_num;
        node_this = spl_trie_->root_;
        continue;
      } else {
        return idx_num;
      }
    }

    str_pos++;
  }

  uint16 id_this = node_this->spelling_idx;
  if (spl_trie_->if_valid_id_update(&id_this)) {
    // endable, remember the index
    spl_idx[idx_num] = id_this;

    idx_num++;
    if (NULL != start_pos)
      start_pos[idx_num] = str_pos;
  }

  last_is_pre = !last_is_splitter;

  return idx_num;
}

uint16 SpellingParser::splstr_to_idxs_f(const char *splstr, uint16 str_len,
                                        uint16 spl_idx[], uint16 start_pos[],
                                        uint16 max_size, bool &last_is_pre) {
  uint16 idx_num = splstr_to_idxs(splstr, str_len, spl_idx, start_pos,
                                  max_size, last_is_pre);
  for (uint16 pos = 0; pos < idx_num; pos++) {
    if (spl_trie_->is_half_id_yunmu(spl_idx[pos])) {
      spl_trie_->half_to_full(spl_idx[pos], spl_idx + pos);
      if (pos == idx_num - 1) {
        last_is_pre = false;
      }
    }
  }
  return idx_num;
}

uint16 SpellingParser::splstr16_to_idxs(const char16 *splstr, uint16 str_len,
                                        uint16 spl_idx[], uint16 start_pos[],
                                        uint16 max_size, bool &last_is_pre) {
  if (NULL == splstr || 0 == max_size || 0 == str_len)
    return 0;

  if (!SpellingTrie::is_valid_spl_char(splstr[0]))
    return 0;

  last_is_pre = false;

  const SpellingNode *node_this = spl_trie_->root_;

  uint16 str_pos = 0;
  uint16 idx_num = 0;
  if (NULL != start_pos)
    start_pos[0] = 0;
  bool last_is_splitter = false;

  while (str_pos < str_len) {
    char16 char_this = splstr[str_pos];
    // all characters outside of [a, z] are considered as splitters
    if (!SpellingTrie::is_valid_spl_char(char_this)) {
      // test if the current node is endable
      uint16 id_this = node_this->spelling_idx;
      if (spl_trie_->if_valid_id_update(&id_this)) {
        spl_idx[idx_num] = id_this;

        idx_num++;
        str_pos++;
        if (NULL != start_pos)
          start_pos[idx_num] = str_pos;
        if (idx_num >= max_size)
          return idx_num;

        node_this = spl_trie_->root_;
        last_is_splitter = true;
        continue;
      } else {
        if (last_is_splitter) {
          str_pos++;
          if (NULL != start_pos)
            start_pos[idx_num] = str_pos;
          continue;
        } else {
          return idx_num;
        }
      }
    }

    last_is_splitter = false;

    SpellingNode *found_son = NULL;

    if (0 == str_pos) {
      if (char_this >= 'a')
        found_son = spl_trie_->level1_sons_[char_this - 'a'];
      else
        found_son = spl_trie_->level1_sons_[char_this - 'A'];
    } else {
      SpellingNode *first_son = node_this->first_son;
      // Because for Zh/Ch/Sh nodes, they are the last in the buffer and
      // frequently used, so we scan from the end.
      for (int i = 0; i < node_this->num_of_son; i++) {
        SpellingNode *this_son = first_son + i;
        if (SpellingTrie::is_same_spl_char(
            this_son->char_this_node, char_this)) {
          found_son = this_son;
          break;
        }
      }
    }

    // found, just move the current node pointer to the the son
    if (NULL != found_son) {
      node_this = found_son;
    } else {
      // not found, test if it is endable
      uint16 id_this = node_this->spelling_idx;
      if (spl_trie_->if_valid_id_update(&id_this)) {
        // endable, remember the index
        spl_idx[idx_num] = id_this;

        idx_num++;
        if (NULL != start_pos)
          start_pos[idx_num] = str_pos;
        if (idx_num >= max_size)
          return idx_num;
        node_this = spl_trie_->root_;
        continue;
      } else {
        return idx_num;
      }
    }

    str_pos++;
  }

  uint16 id_this = node_this->spelling_idx;
  if (spl_trie_->if_valid_id_update(&id_this)) {
    // endable, remember the index
    spl_idx[idx_num] = id_this;

    idx_num++;
    if (NULL != start_pos)
      start_pos[idx_num] = str_pos;
  }

  last_is_pre = !last_is_splitter;

  return idx_num;
}

uint16 SpellingParser::splstr16_to_idxs_f(const char16 *splstr, uint16 str_len,
                                          uint16 spl_idx[], uint16 start_pos[],
                                          uint16 max_size, bool &last_is_pre) {
  uint16 idx_num = splstr16_to_idxs(splstr, str_len, spl_idx, start_pos,
                                    max_size, last_is_pre);
  for (uint16 pos = 0; pos < idx_num; pos++) {
    if (spl_trie_->is_half_id_yunmu(spl_idx[pos])) {
      spl_trie_->half_to_full(spl_idx[pos], spl_idx + pos);
      if (pos == idx_num - 1) {
        last_is_pre = false;
      }
    }
  }
  return idx_num;
}

uint16 SpellingParser::get_splid_by_str(const char *splstr, uint16 str_len,
                                        bool *is_pre) {
  if (NULL == is_pre)
    return 0;

  uint16 spl_idx[2];
  uint16 start_pos[3];

  if (splstr_to_idxs(splstr, str_len, spl_idx, start_pos, 2, *is_pre) != 1)
    return 0;

  if (start_pos[1] != str_len)
    return 0;
  return spl_idx[0];
}

uint16 SpellingParser::get_splid_by_str_f(const char *splstr, uint16 str_len,
                                          bool *is_pre) {
  if (NULL == is_pre)
    return 0;

  uint16 spl_idx[2];
  uint16 start_pos[3];

  if (splstr_to_idxs(splstr, str_len, spl_idx, start_pos, 2, *is_pre) != 1)
    return 0;

  if (start_pos[1] != str_len)
    return 0;
  if (spl_trie_->is_half_id_yunmu(spl_idx[0])) {
    spl_trie_->half_to_full(spl_idx[0], spl_idx);
    *is_pre = false;
  }

  return spl_idx[0];
}

uint16 SpellingParser::get_splids_parallel(const char *splstr, uint16 str_len,
    uint16 splidx[], uint16 max_size,
    uint16 &full_id_num, bool &is_pre) {
  if (max_size <= 0 || !is_valid_to_parse(splstr[0]))
    return 0;

  splidx[0] = get_splid_by_str(splstr, str_len, &is_pre);
  full_id_num = 0;
  if (0 != splidx[0]) {
    if (splidx[0] >= kFullSplIdStart)
      full_id_num = 1;
    return 1;
  }
  return 0;
}

}  // namespace ime_pinyin
