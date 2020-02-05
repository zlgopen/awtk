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

#include <stdlib.h>
#include "../include/utf16char.h"

namespace ime_pinyin {

#ifdef __cplusplus
extern "C" {
#endif

  char16* utf16_strtok(char16 *utf16_str, unsigned *token_size,
                       char16 **utf16_str_next) {
    if (NULL == utf16_str || NULL == token_size || NULL == utf16_str_next) {
      return NULL;
    }

    // Skip the splitters
    unsigned pos = 0;
    while ((char16)' ' == utf16_str[pos] || (char16)'\n' == utf16_str[pos]
           || (char16)'\t' == utf16_str[pos])
      pos++;

    utf16_str += pos;
    pos = 0;

    while ((char16)'\0' != utf16_str[pos] && (char16)' ' != utf16_str[pos]
           && (char16)'\n' != utf16_str[pos]
           && (char16)'\t' != utf16_str[pos]) {
      pos++;
    }

    char16 *ret_val = utf16_str;
    if ((char16)'\0' == utf16_str[pos]) {
      *utf16_str_next = NULL;
      if (0 == pos)
        return NULL;
    } else {
      *utf16_str_next = utf16_str + pos + 1;
    }

    utf16_str[pos] = (char16)'\0';
    *token_size = pos;

    return ret_val;
  }

  int utf16_atoi(const char16 *utf16_str) {
    if (NULL == utf16_str)
      return 0;

    int value = 0;
    int sign = 1;
    unsigned pos = 0;

    if ((char16)'-' == utf16_str[pos]) {
      sign = -1;
      pos++;
    }

    while ((char16)'0' <=  utf16_str[pos] &&
           (char16)'9' >= utf16_str[pos]) {
      value = value * 10 + static_cast<int>(utf16_str[pos] - (char16)'0');
      pos++;
    }

    return value*sign;
  }

  float utf16_atof(const char16 *utf16_str) {
    // A temporary implemetation.
    char char8[256];
    if (utf16_strlen(utf16_str) >= 256) return 0;

    utf16_strcpy_tochar(char8, utf16_str);
    return atof(char8);
  }

  unsigned utf16_strlen(const char16 *utf16_str) {
    if (NULL == utf16_str)
      return 0;

    unsigned size = 0;
    while ((char16)'\0' != utf16_str[size])
      size++;
    return size;
  }

  int utf16_strcmp(const char16* str1, const char16* str2) {
    unsigned pos = 0;
    while (str1[pos] == str2[pos] && (char16)'\0' != str1[pos])
      pos++;

    return static_cast<int>(str1[pos]) - static_cast<int>(str2[pos]);
  }

  int utf16_strncmp(const char16 *str1, const char16 *str2, unsigned size) {
    unsigned pos = 0;
    while (pos < size && str1[pos] == str2[pos] && (char16)'\0' != str1[pos])
      pos++;

    if (pos == size)
      return 0;

    return static_cast<int>(str1[pos]) - static_cast<int>(str2[pos]);
  }

  // we do not consider overlapping
  char16* utf16_strcpy(char16 *dst, const char16 *src) {
    if (NULL == src || NULL == dst)
      return NULL;

    char16* cp = dst;

    while ((char16)'\0' != *src) {
      *cp = *src;
      cp++;
      src++;
    }

    *cp = *src;

    return dst;
  }

  char16* utf16_strncpy(char16 *dst, const char16 *src, unsigned size) {
    if (NULL == src || NULL == dst || 0 == size)
      return NULL;

    if (src == dst)
      return dst;

    char16* cp = dst;

    if (dst < src || (dst > src && dst >= src + size)) {
      while (size-- && (*cp++ = *src++))
        ;
    } else {
      cp += size - 1;
      src += size - 1;
      while (size-- && (*cp-- == *src--))
        ;
    }
    return dst;
  }

  // We do not handle complicated cases like overlapping, because in this
  // codebase, it is not necessary.
  char* utf16_strcpy_tochar(char *dst, const char16 *src) {
    if (NULL == src || NULL == dst)
      return NULL;

    char* cp = dst;

    while ((char16)'\0' != *src) {
      *cp = static_cast<char>(*src);
      cp++;
      src++;
    }
    *cp = *src;

    return dst;
  }

#ifdef __cplusplus
}
#endif
}  // namespace ime_pinyin
