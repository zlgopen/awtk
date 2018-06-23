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

#ifndef PINYINIME_INCLUDE_SPLPARSER_H__
#define PINYINIME_INCLUDE_SPLPARSER_H__

#include "./dictdef.h"
#include "./spellingtrie.h"

namespace ime_pinyin {

class SpellingParser {
 protected:
  const SpellingTrie *spl_trie_;

 public:
  SpellingParser();

  // Given a string, parse it into a spelling id stream.
  // If the whole string are sucessfully parsed, last_is_pre will be true;
  // if the whole string is not fullly parsed, last_is_pre will return whether
  // the last part of the string is a prefix of a full spelling string. For
  // example, given string "zhengzhon", "zhon" is not a valid speling, but it is
  // the prefix of "zhong".
  //
  // If splstr starts with a character not in ['a'-z'] (it is a split char),
  // return 0.
  // Split char can only appear in the middle of the string or at the end.
  uint16 splstr_to_idxs(const char *splstr, uint16 str_len, uint16 splidx[],
                        uint16 start_pos[], uint16 max_size, bool &last_is_pre);

  // Similar to splstr_to_idxs(), the only difference is that splstr_to_idxs()
  // convert single-character Yunmus into half ids, while this function converts
  // them into full ids.
  uint16 splstr_to_idxs_f(const char *splstr, uint16 str_len, uint16 splidx[],
          uint16 start_pos[], uint16 max_size, bool &last_is_pre);

  // Similar to splstr_to_idxs(), the only difference is that this function
  // uses char16 instead of char8.
  uint16 splstr16_to_idxs(const char16 *splstr, uint16 str_len, uint16 splidx[],
                        uint16 start_pos[], uint16 max_size, bool &last_is_pre);

  // Similar to splstr_to_idxs_f(), the only difference is that this function
  // uses char16 instead of char8.
  uint16 splstr16_to_idxs_f(const char16 *splstr16, uint16 str_len,
                            uint16 splidx[], uint16 start_pos[],
                            uint16 max_size, bool &last_is_pre);

  // If the given string is a spelling, return the id, others, return 0.
  // If the give string is a single char Yunmus like "A", and the char is
  // enabled in ShouZiMu mode, the returned spelling id will be a half id.
  // When the returned spelling id is a half id, *is_pre returns whether it
  // is a prefix of a full spelling string.
  uint16 get_splid_by_str(const char *splstr, uint16 str_len, bool *is_pre);

  // If the given string is a spelling, return the id, others, return 0.
  // If the give string is a single char Yunmus like "a", no matter the char
  // is enabled in ShouZiMu mode or not, the returned spelling id will be
  // a full id.
  // When the returned spelling id is a half id, *p_is_pre returns whether it
  // is a prefix of a full spelling string.
  uint16 get_splid_by_str_f(const char *splstr, uint16 str_len, bool *is_pre);

  // Splitter chars are not included.
  bool is_valid_to_parse(char ch);

  // When auto-correction is not enabled, get_splid_by_str() will be called to
  // return the single result. When auto-correction is enabled, this function
  // will be called to get the results. Auto-correction is not ready.
  // full_id_num returns number of full spelling ids.
  // is_pre returns whether the given string is the prefix of a full spelling
  // string.
  // If splstr starts with a character not in [a-zA-Z] (it is a split char),
  // return 0.
  // Split char can only appear in the middle of the string or at the end.
  // The caller should guarantee NULL != splstr && str_len > 0 && NULL != splidx
  uint16 get_splids_parallel(const char *splstr, uint16 str_len,
                             uint16 splidx[], uint16 max_size,
                             uint16 &full_id_num, bool &is_pre);
};
}

#endif  // PINYINIME_INCLUDE_SPLPARSER_H__
