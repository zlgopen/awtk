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

#ifndef PINYINIME_INCLUDE_DICTLIST_H__
#define PINYINIME_INCLUDE_DICTLIST_H__

#include <stdlib.h>
#include <stdio.h>
#include "./dictdef.h"
#include "./searchutility.h"
#include "./spellingtrie.h"
#include "./utf16char.h"

namespace ime_pinyin {

class DictList {
 private:
  bool initialized_;

  const SpellingTrie *spl_trie_;

  // Number of SingCharItem. The first is blank, because id 0 is invalid.
  unsigned scis_num_;
  char16 *scis_hz_;
  SpellingId *scis_splid_;

  // The large memory block to store the word list.
  char16 *buf_;

  // Starting position of those words whose lengths are i+1, counted in
  // char16
  unsigned start_pos_[kMaxLemmaSize + 1];

  unsigned start_id_[kMaxLemmaSize + 1];

  int (*cmp_func_[kMaxLemmaSize])(const void *, const void *);

  bool alloc_resource(unsigned buf_size, unsigned scim_num);

  void free_resource();

#ifdef ___BUILD_MODEL___
  // Calculate the requsted memory, including the start_pos[] buffer.
  unsigned calculate_size(const LemmaEntry *lemma_arr, unsigned lemma_num);

  void fill_scis(const SingleCharItem *scis, unsigned scis_num);

  // Copy the related content to the inner buffer
  // It should be called after calculate_size()
  void fill_list(const LemmaEntry *lemma_arr, unsigned lemma_num);

  // Find the starting position for the buffer of those 2-character Chinese word
  // whose first character is the given Chinese character.
  char16* find_pos2_startedbyhz(char16 hz_char);
#endif

  // Find the starting position for the buffer of those words whose lengths are
  // word_len. The given parameter cmp_func decides how many characters from
  // beginning will be used to compare.
  char16* find_pos_startedbyhzs(const char16 last_hzs[],
                                unsigned word_Len,
                                int (*cmp_func)(const void *, const void *));

 public:

  DictList();
  ~DictList();

  bool save_list(FILE *fp);
  bool load_list(FILE *fp);

#ifdef ___BUILD_MODEL___
  // Init the list from the LemmaEntry array.
  // lemma_arr should have been sorted by the hanzi_str, and have been given
  // ids from 1
  bool init_list(const SingleCharItem *scis, unsigned scis_num,
                 const LemmaEntry *lemma_arr, unsigned lemma_num);
#endif

  // Get the hanzi string for the given id
  uint16 get_lemma_str(LemmaIdType id_hz, char16 *str_buf, uint16 str_max);

  void convert_to_hanzis(char16 *str, uint16 str_len);

  void convert_to_scis_ids(char16 *str, uint16 str_len);

  // last_hzs stores the last n Chinese characters history, its length should be
  // less or equal than kMaxPredictSize.
  // hzs_len specifies the length(<= kMaxPredictSize).
  // predict_buf is used to store the result.
  // buf_len specifies the buffer length.
  // b4_used specifies how many items before predict_buf have been used.
  // Returned value is the number of newly added items.
  unsigned predict(const char16 last_hzs[], uint16 hzs_len,
                 NPredictItem *npre_items, unsigned npre_max,
                 unsigned b4_used);

  // If half_splid is a valid half spelling id, return those full spelling
  // ids which share this half id.
  uint16 get_splids_for_hanzi(char16 hanzi, uint16 half_splid,
                              uint16 *splids, uint16 max_splids);

  LemmaIdType get_lemma_id(const char16 *str, uint16 str_len);
};
}

#endif  // PINYINIME_INCLUDE_DICTLIST_H__
