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

#ifndef PINYINIME_INCLUDE_SPELLINGTABLE_H__
#define PINYINIME_INCLUDE_SPELLINGTABLE_H__

#include <stdlib.h>
#include "./dictdef.h"

namespace ime_pinyin {

#ifdef ___BUILD_MODEL___

const unsigned kMaxSpellingSize = kMaxPinyinSize;

typedef struct {
  char str[kMaxSpellingSize + 1];
  double freq;
} RawSpelling, *PRawSpelling;

// This class is used to store the spelling strings
// The length of the input spelling string should be less or equal to the
// spelling_size_ (set by init_table). If the input string is too long,
// we only keep its first spelling_size_ chars.
class SpellingTable {
 private:
  static const unsigned kNotSupportNum = 3;
  static const char kNotSupportList[kNotSupportNum][kMaxSpellingSize + 1];

  bool need_score_;

  unsigned spelling_max_num_;

  RawSpelling *raw_spellings_;

  // Used to store spelling strings. If the spelling table needs to calculate
  // score, an extra char after each spelling string is the score.
  // An item with a lower score has a higher probability.
  char *spelling_buf_;
  unsigned spelling_size_;

  double total_freq_;

  unsigned spelling_num_;

  double score_amplifier_;

  unsigned char average_score_;

  // If frozen is true, put_spelling() and contain() are not allowed to call.
  bool frozen_;

  unsigned get_hash_pos(const char* spelling_str);
  unsigned hash_pos_next(unsigned hash_pos);
  void free_resource();
 public:
  SpellingTable();
  ~SpellingTable();

  // pure_spl_size is the pure maximum spelling string size. For example,
  // "zhuang" is the longgest item in Pinyin, so pure_spl_size should be 6.
  // spl_max_num is the maximum number of spelling strings to store.
  // need_score is used to indicate whether the caller needs to calculate a
  // score for each spelling.
  bool init_table(unsigned pure_spl_size, unsigned spl_max_num, bool need_score);

  // Put a spelling string to the table.
  // It always returns false if called after arrange() withtout a new
  // init_table() operation.
  // freq is the spelling's occuring count.
  // If the spelling has been in the table, occuring count will accumulated.
  bool put_spelling(const char* spelling_str, double spl_count);

  // Test whether a spelling string is in the table.
  // It always returns false, when being called after arrange() withtout a new
  // init_table() operation.
  bool contain(const char* spelling_str);

  // Sort the spelling strings and put them from the begin of the buffer.
  // Return the pointer of the sorted spelling strings.
  // item_size and spl_num return the item size and number of spelling.
  // Because each spelling uses a '\0' as terminator, the returned item_size is
  // at least one char longer than the spl_size parameter specified by
  // init_table(). If the table is initialized to calculate score, item_size
  // will be increased by 1, and current_spl_str[item_size - 1] stores an
  // unsinged char score.
  // An item with a lower score has a higher probability.
  // Do not call put_spelling() and contains() after arrange().
  const char* arrange(unsigned *item_size, unsigned *spl_num);

  float get_score_amplifier();

  unsigned char get_average_score();
};
#endif  // ___BUILD_MODEL___
}

#endif  // PINYINIME_INCLUDE_SPELLINGTABLE_H__
