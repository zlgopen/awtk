/**
 * File:   font.h
 * Author: AWTK Develop Team
 * Brief:  font interface
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FONT_H
#define TK_FONT_H

#include "base/types_def.h"

BEGIN_C_DECLS

struct _font_t;
typedef struct _font_t font_t;

typedef struct _glyph_t {
  int8_t x;
  int8_t y;
  uint8_t w;
  uint8_t h;
  const uint8_t* data;
} glyph_t;

typedef int32_t (*font_get_baseline_t)(font_t* f, uint16_t font_size);
typedef bool_t (*font_match_t)(font_t* f, const char* name, uint16_t font_size);
typedef ret_t (*font_find_glyph_t)(font_t* f, wchar_t chr, glyph_t* g, uint16_t font_size);

typedef ret_t (*font_destroy_t)(font_t* f);

struct _font_t {
  const char* name;
  font_match_t match;
  font_get_baseline_t get_baseline;
  font_find_glyph_t find_glyph;
  font_destroy_t destroy;
};

int32_t font_get_baseline(font_t* f, uint16_t font_size);
bool_t font_match(font_t* f, const char* name, uint16_t font_size);
ret_t font_find_glyph(font_t* f, wchar_t chr, glyph_t* g, uint16_t font_size);
ret_t font_destroy(font_t* f);

END_C_DECLS

#endif /*TK_FONT_H*/
