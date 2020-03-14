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
#include <stdio.h>
#include <time.h>
#define  WIN32_LEAN_AND_MEAN 1

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "../include/dicttrie.h"

using namespace ime_pinyin;

/**
 * Build binary dictionary model. Make sure that ___BUILD_MODEL___ is defined
 * in dictdef.h.
 */
int main(int argc, char* argv[]) {
  const char* rawdict = "./3rd/gpinyin/data/rawdict_utf16_65105_freq.txt";
  const char* validchars = "./3rd/gpinyin/data/valid_utf16.txt";
  const char* output = "./3rd/gpinyin/data/gpinyin.dat";
  DictTrie* dict_trie = new DictTrie();

  bool success;
  if (argc > 3) {
    rawdict = argv[1];
    validchars = argv[2];
    output = argv[3];
  } else if(argc == 2) {
    printf("Usage: %s rawdict validchars output\n", argv[0]);
    return -1;
  } else {
    printf("%s %s => %s\n", rawdict, validchars, output);
  }

  success = dict_trie->build_dict(rawdict, validchars);

  if (success) {
    printf("Build dictionary successfully.\n");
  } else {
    printf("Build dictionary unsuccessfully.\n");
    return -1;
  }

  success = dict_trie->save_dict(output);

  if (success) {
    printf("Save dictionary successfully.\n");
  } else {
    printf("Save dictionary unsuccessfully.\n");
    return -1;
  }

  return 0;
}
