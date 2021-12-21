/**
 * File:   preprocess_text.c
 * Author: AWTK Develop Team
 * Brief:  preprocess_text
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-12-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "tkc/str.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"

const char* font_gen_expand_one(const char* in, str_t* out) {
  const char* p = in;

  if (p[0] == '\0' || p[1] == '\0' || p[2] == '\0' || p[3] == '\0') {
    return p += strlen(p);
  }

  if (p[1] == '-' && p[3] == ']' && p[4] == ']') {
    /*[[a-z]]*/
    char c = p[0];
    char end = p[2];
    while (c <= end) {
      str_append_char(out, c);
      c++;
    }
    p += 5;
  } else {
    /* [[1000-2000]] */
    char buff[32];
    int c = 0;
    int end = 0;
    int n = 0;
    if (strncmp(p, "0x", 2) == 0) {
      n = tk_sscanf(p, "0x%x-0x%x]]", &c, &end);
    } else if (strncmp(p, "0X", 2) == 0) {
      n = tk_sscanf(p, "0X%x-0X%x]]", &c, &end);
    } else {
      n = tk_sscanf(p, "%d-%d]]", &c, &end);
    }
    return_value_if_fail(n == 2, in);
    p = strstr(p, "]]");
    return_value_if_fail(p != NULL, in);
    p += 2;

    while (c <= end) {
      tk_utf8_from_utf16_ex(&c, 1, buff, sizeof(buff));
      str_append(out, buff);
      c++;
    }
  }

  return p;
}

const char* font_gen_expand_text(const char* in, str_t* out) {
  const char* p = in;

  while (*p) {
    if (*p == '[' && p[1] == '[') {
      p = font_gen_expand_one(p + 2, out);
    } else {
      str_append_char(out, *p);
      p++;
    }
  }

  return out->str;
}
