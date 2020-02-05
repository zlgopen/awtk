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

#ifndef PINYINIME_INCLUDE_SPELLINGTRIE_H__
#define PINYINIME_INCLUDE_SPELLINGTRIE_H__

#include <stdio.h>
#include <stdlib.h>
#include "./dictdef.h"

namespace ime_pinyin {

static const unsigned short kFullSplIdStart = kHalfSpellingIdNum + 1;

// Node used for the trie of spellings
struct SpellingNode {
  SpellingNode *first_son;
  // The spelling id for each node. If you need more bits to store
  // spelling id, please adjust this structure.
  uint16 spelling_idx:11;
  uint16  num_of_son:5;
  char char_this_node;
  unsigned char score;
};

class SpellingTrie {
 private:
  static const int kMaxYmNum = 64;
  static const unsigned kValidSplCharNum = 26;

  static const uint16 kHalfIdShengmuMask = 0x01;
  static const uint16 kHalfIdYunmuMask = 0x02;
  static const uint16 kHalfIdSzmMask = 0x04;

  // Map from half spelling id to single char.
  // For half ids of Zh/Ch/Sh, map to z/c/s (low case) respectively.
  // For example, 1 to 'A', 2 to 'B', 3 to 'C', 4 to 'c', 5 to 'D', ...,
  // 28 to 'Z', 29 to 'z'.
  // [0] is not used to achieve better efficiency.
  static const char kHalfId2Sc_[kFullSplIdStart + 1];

  static unsigned char char_flags_[];
  static SpellingTrie* instance_;

  // The spelling table
  char *spelling_buf_;

  // The size of longest spelling string, includes '\0' and an extra char to
  // store score. For example, "zhuang" is the longgest item in Pinyin list,
  // so spelling_size_ is 8.
  // Structure: The string ended with '\0' + score char.
  // An item with a lower score has a higher probability.
  unsigned spelling_size_;

  // Number of full spelling ids.
  unsigned spelling_num_;

  float score_amplifier_;
  unsigned char average_score_;

  // The Yunmu id list for the spelling ids (for half ids of Shengmu,
  // the Yunmu id is 0).
  // The length of the list is spelling_num_ + kFullSplIdStart,
  // so that spl_ym_ids_[splid] is the Yunmu id of the splid.
  uint8 *spl_ym_ids_;

  // The Yunmu table.
  // Each Yunmu will be assigned with Yunmu id from 1.
  char *ym_buf_;
  unsigned ym_size_;  // The size of longest Yunmu string, '\0'included.
  unsigned ym_num_;

  // The spelling string just queried
  char *splstr_queried_;

  // The spelling string just queried
  char16 *splstr16_queried_;

  // The root node of the spelling tree
  SpellingNode* root_;

  // If a none qwerty key such as a fnction key like ENTER is given, this node
  // will be used to indicate that this is not a QWERTY node.
  SpellingNode* dumb_node_;

  // If a splitter key is pressed, this node will be used to indicate that this
  // is a splitter key.
  SpellingNode* splitter_node_;

  // Used to get the first level sons.
  SpellingNode* level1_sons_[kValidSplCharNum];

  // The full spl_id range for specific half id.
  // h2f means half to full.
  // A half id can be a ShouZiMu id (id to represent the first char of a full
  // spelling, including Shengmu and Yunmu), or id of zh/ch/sh.
  // [1..kFullSplIdStart-1] is the arrange of half id.
  uint16 h2f_start_[kFullSplIdStart];
  uint16 h2f_num_[kFullSplIdStart];

  // Map from full id to half id.
  uint16 *f2h_;

#ifdef ___BUILD_MODEL___
  // How many node used to build the trie.
  unsigned node_num_;
#endif

  SpellingTrie();

  void free_son_trie(SpellingNode* node);

  // Construct a subtree using a subset of the spelling array (from
  // item_star to item_end).
  // Member spelliing_buf_ and spelling_size_ should be valid.
  // parent is used to update its num_of_son and score.
  SpellingNode* construct_spellings_subset(unsigned item_start, unsigned item_end,
                                           unsigned level, SpellingNode *parent);
  bool build_f2h();

  // The caller should guarantee ch >= 'A' && ch <= 'Z'
  bool is_shengmu_char(char ch) const;

  // The caller should guarantee ch >= 'A' && ch <= 'Z'
  bool is_yunmu_char(char ch) const;

#ifdef ___BUILD_MODEL___
  // Given a spelling string, return its Yunmu string.
  // The caller guaratees spl_str is valid.
  const char* get_ym_str(const char *spl_str);

  // Build the Yunmu list, and the mapping relation between the full ids and the
  // Yunmu ids. This functin is called after the spelling trie is built.
  bool build_ym_info();
#endif

  friend class SpellingParser;
  friend class SmartSplParser;
  friend class SmartSplParser2;

 public:
  ~SpellingTrie();

  inline static bool is_valid_spl_char(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
  }

  // The caller guarantees that the two chars are valid spelling chars.
  inline static bool is_same_spl_char(char ch1, char ch2) {
    return ch1 == ch2 || ch1 - ch2 == 'a' - 'A' || ch2 - ch1 == 'a' - 'A';
  }

  // Construct the tree from the input pinyin array
  // The given string list should have been sorted.
  // score_amplifier is used to convert a possibility value into score.
  // average_score is the average_score of all spellings. The dumb node is
  // assigned with this score.
  bool construct(const char* spelling_arr, unsigned item_size, unsigned item_num,
                 float score_amplifier, unsigned char average_score);

  // Test if the given id is a valid spelling id.
  // If function returns true, the given splid may be updated like this:
  // When 'A' is not enabled in ShouZiMu mode, the parsing result for 'A' is
  // first given as a half id 1, but because 'A' is a one-char Yunmu and
  // it is a valid id, it needs to updated to its corresponding full id.
  bool if_valid_id_update(uint16 *splid) const;

  // Test if the given id is a half id.
  bool is_half_id(uint16 splid) const;

  bool is_full_id(uint16 splid) const;

  // Test if the given id is a one-char Yunmu id (obviously, it is also a half
  // id), such as 'A', 'E' and 'O'.
  bool is_half_id_yunmu(uint16 splid) const;

  // Test if this char is a ShouZiMu char. This ShouZiMu char may be not enabled.
  // For Pinyin, only i/u/v is not a ShouZiMu char.
  // The caller should guarantee that ch >= 'A' && ch <= 'Z'
  bool is_szm_char(char ch) const;

  // Test If this char is enabled in ShouZiMu mode.
  // The caller should guarantee that ch >= 'A' && ch <= 'Z'
  bool szm_is_enabled(char ch) const;

  // Enable/disable Shengmus in ShouZiMu mode(using the first char of a spelling
  // to input).
  void szm_enable_shm(bool enable);

  // Enable/disable Yunmus in ShouZiMu mode.
  void szm_enable_ym(bool enable);

  // Test if this char is enabled in ShouZiMu mode.
  // The caller should guarantee ch >= 'A' && ch <= 'Z'
  bool is_szm_enabled(char ch) const;

  // Return the number of full ids for the given half id.
  uint16 half2full_num(uint16 half_id) const;

  // Return the number of full ids for the given half id, and fill spl_id_start
  // to return the first full id.
  uint16 half_to_full(uint16 half_id, uint16 *spl_id_start) const;

  // Return the corresponding half id for the given full id.
  // Not frequently used, low efficient.
  // Return 0 if fails.
  uint16 full_to_half(uint16 full_id) const;

  // To test whether a half id is compatible with a full id.
  // Generally, when half_id == full_to_half(full_id), return true.
  // But for "Zh, Ch, Sh", if fussy mode is on, half id for 'Z' is compatible
  // with a full id like "Zhe". (Fussy mode is not ready).
  bool half_full_compatible(uint16 half_id, uint16 full_id) const;

  static const SpellingTrie* get_cpinstance();

  static SpellingTrie& get_instance();

  // Save to the file stream
  bool save_spl_trie(FILE *fp);

  // Load from the file stream
  bool load_spl_trie(FILE *fp);

  // Get the number of spellings
  unsigned get_spelling_num();

  // Return the Yunmu id for the given Yunmu string.
  // If the string is not valid, return 0;
  uint8 get_ym_id(const char* ym_str);

  // Get the readonly Pinyin string for a given spelling id
  const char* get_spelling_str(uint16 splid);

  // Get the readonly Pinyin string for a given spelling id
  const char16* get_spelling_str16(uint16 splid);

  // Get Pinyin string for a given spelling id. Return the length of the
  // string, and fill-in '\0' at the end.
  unsigned get_spelling_str16(uint16 splid, char16 *splstr16,
                            unsigned splstr16_len);
};
}

#endif  // PINYINIME_INCLUDE_SPELLINGTRIE_H__
