/**
 * File:   font.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  font interface
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_FONT_H
#define LFTK_FONT_H

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
}glyph_t;

typedef ret_t (*font_find_glyph_t)(font_t* f, wchar_t chr, glyph_t* g);
typedef ret_t (*font_destroy_t)(font_t* f);

struct _font_t {
  uint16_t size;
  uint16_t scalable;
  const char* name;
  font_find_glyph_t find_glyph;
  font_destroy_t destroy;
};

ret_t font_find_glyph(font_t* f, wchar_t chr, glyph_t* g);
ret_t font_destroy(font_t* f);

END_C_DECLS

#endif/*LFTK_FONT_H*/

