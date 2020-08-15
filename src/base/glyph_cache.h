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

/**
 * @class glyph_cache_t
 * glyph cache
 */
typedef struct _glyph_cache_t {
  uint32_t size;
  uint32_t capacity;
  glyph_cache_item_t* items;
  tk_destroy_t destroy_glyph;
} glyph_cache_t;

/**
 * @method glyph_cache_init
 * 初始化glyph_cache对象。
 * @param {glyph_cache_t*} cache cache对象。
 * @param {uint32_t} capacity 最大容量。
 * @param {tk_destroy_t} destroy_glyph 销毁函数。
 * 
 * @return {glyph_cache_t*} 返回cache对象。
 */
glyph_cache_t* glyph_cache_init(glyph_cache_t* cache, uint32_t capacity,
                                tk_destroy_t destroy_glyph);

/**
 * @method glyph_cache_add
 * 增加一个glyph对象到cache。
 * 
 * @param {glyph_cache_t*} cache cache对象。
 * @param {wchar_t} code 字符。
 * @param {font_size_t} size 字体大小。
 * @param {glyph_t} g glyph对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t glyph_cache_add(glyph_cache_t* cache, wchar_t code, font_size_t size, glyph_t* g);

/**
 * @method glyph_cache_lookup
 * 查找glyph对象。
 * 
 * @param {glyph_cache_t*} cache cache对象。
 * @param {wchar_t} code 字符。
 * @param {font_size_t} size 字体大小。
 * @param {glyph_t} g glyph对象(用于返回glyph)。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t glyph_cache_lookup(glyph_cache_t* cache, wchar_t code, font_size_t size, glyph_t* g);

/**
 * @method glyph_cache_shrink
 * 释放部分glyph。
 * 
 * @param {glyph_cache_t*} cache cache对象。
 * @param {uint32_t} cache_size 保留cache个数。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t glyph_cache_shrink(glyph_cache_t* cache, uint32_t cache_size);

/**
 * @method glyph_cache_deinit
 * 释放全部cache。
 * 
 * @param {glyph_cache_t*} cache cache对象。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t glyph_cache_deinit(glyph_cache_t* cache);

END_C_DECLS

#endif /*TK_GLYPH_CACHE_H*/
