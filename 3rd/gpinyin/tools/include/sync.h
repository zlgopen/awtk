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

#ifndef PINYINIME_INCLUDE_SYNC_H__
#define PINYINIME_INCLUDE_SYNC_H__

#define ___SYNC_ENABLED___

#ifdef ___SYNC_ENABLED___

#include "userdict.h"

namespace ime_pinyin {

// Class for user dictionary synchronization
// This class is not thread safe
// Normal invoking flow will be
//   begin() ->
//   put_lemmas() x N ->
//   {
//     get_lemmas() ->
//     [ get_last_got_count() ] ->
//     clear_last_got() ->
//   } x N ->
//   finish()
class Sync {
 public:
  Sync();
  ~Sync();

  static const int kUserDictMaxLemmaCount = 5000;
  static const int kUserDictMaxLemmaSize = 200000;
  static const int kUserDictRatio = 20;

  bool begin(const char * filename);

  // Merge lemmas downloaded from sync server into local dictionary
  // lemmas, lemmas string encoded in UTF16LE
  // len, length of lemmas string
  // Return how many lemmas merged successfully
  int put_lemmas(char16 * lemmas, int len);

  // Get local new user lemmas into UTF16LE string
  // str, buffer ptr to store new user lemmas
  // size, size of buffer
  // Return length of returned buffer in measure of UTF16LE
  int get_lemmas(char16 * str, int size);

  // Return lemmas count in last get_lemmas()
  int get_last_got_count();

  // Return total lemmas count need get_lemmas()
  int get_total_count();

  // Clear lemmas got by recent get_lemmas()
  void clear_last_got();

  void finish();

  int get_capacity();

 private:
  UserDict * userdict_;
  char * dictfile_;
  int last_count_;
};

}

#endif

#endif  // PINYINIME_INCLUDE_SYNC_H__
