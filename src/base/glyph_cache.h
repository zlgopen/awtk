/**
 * File:   glyph_cache.h
 * Author: AWTK Develop Team
 * Brief:  glyph cache
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-06-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GLYPH_CACHE_H
#define TK_GLYPH_CACHE_H

#include "base/font.h"

BEGIN_C_DECLS

typedef struct _glyph_cache_item_t {
  uint32_t last_access_time;
  font_size_t size;
  wchar_t code;
  glyph_t* g;
} glyph_cache_item_t;

typedef struct _glyph_cache_t {
  uint32_t size;
  uint32_t capacity;
  glyph_cache_item_t* items;
  tk_destroy_t destroy_glyph;
} glyph_cache_t;

glyph_cache_t* glyph_cache_init(glyph_cache_t* cache, uint32_t capacity,
                                tk_destroy_t destroy_glyph);
ret_t glyph_cache_add(glyph_cache_t* cache, wchar_t code, font_size_t size, glyph_t* g);
ret_t glyph_cache_lookup(glyph_cache_t* cache, wchar_t code, font_size_t size, glyph_t* g);
ret_t glyph_cache_deinit(glyph_cache_t* cache);

END_C_DECLS

#endif /*TK_GLYPH_CACHE_H*/
