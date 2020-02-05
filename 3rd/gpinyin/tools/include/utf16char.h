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

#ifndef PINYINIME_INCLUDE_UTF16CHAR_H__
#define PINYINIME_INCLUDE_UTF16CHAR_H__

#include <stdlib.h>

namespace ime_pinyin {

#ifdef __cplusplus
extern "C" {
#endif

  typedef unsigned short char16;

  // Get a token from utf16_str,
  // Returned pointer is a '\0'-terminated utf16 string, or NULL
  // *utf16_str_next returns the next part of the string for further tokenizing
  char16* utf16_strtok(char16 *utf16_str, unsigned *token_size,
                       char16 **utf16_str_next);

  int utf16_atoi(const char16 *utf16_str);

  float utf16_atof(const char16 *utf16_str);

  unsigned utf16_strlen(const char16 *utf16_str);

  int utf16_strcmp(const char16 *str1, const char16 *str2);
  int utf16_strncmp(const char16 *str1, const char16 *str2, unsigned size);

  char16* utf16_strcpy(char16 *dst, const char16 *src);
  char16* utf16_strncpy(char16 *dst, const char16 *src, unsigned size);


  char* utf16_strcpy_tochar(char *dst, const char16 *src);

#ifdef __cplusplus
}
#endif
}

#endif  // PINYINIME_INCLUDE_UTF16CHAR_H__
