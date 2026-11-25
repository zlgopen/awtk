/**
 * File:   glyph_cache.h
 * Author: AWTK Develop Team
 * Brief:  glyph cache
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

/**
 * @class glyph_cache_t
 * glyph cache
 */
typedef struct _glyph_cache_t glyph_cache_t;

/**
 * @method glyph_cache_create
 * 创建glyph_cache对象。
 * @param {uint32_t} capacity 最大容量，必须大于0。
 * @param {tk_destroy_t} destroy_glyph glyph的销毁函数，可以为NULL。
 *
 * @return {glyph_cache_t*} 返回cache对象，失败返回NULL。
 */
glyph_cache_t* glyph_cache_create(uint32_t capacity, tk_destroy_t destroy_glyph);

/**
 * @method glyph_cache_add
 * 增加一个glyph对象到cache，缓存满了会先淘汰一个旧的。
 *
 * @param {glyph_cache_t*} cache cache对象。
 * @param {uint32_t} code 字符。
 * @param {font_size_t} size 字体大小。
 * @param {glyph_t*} g glyph对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t glyph_cache_add(glyph_cache_t* cache, uint32_t code, font_size_t size, glyph_t* g);

/**
 * @method glyph_cache_lookup
 * 查找glyph对象，命中时把glyph拷贝到g。
 *
 * @param {glyph_cache_t*} cache cache对象。
 * @param {uint32_t} code 字符。
 * @param {font_size_t} size 字体大小。
 * @param {glyph_t*} g 用于返回glyph。
 *
 * @return {ret_t} 返回RET_OK表示命中，RET_NOT_FOUND表示没有。
 */
ret_t glyph_cache_lookup(glyph_cache_t* cache, uint32_t code, font_size_t size, glyph_t* g);

/**
 * @method glyph_cache_shrink
 * 只保留cache_size个glyph，其余的释放掉。
 *
 * @param {glyph_cache_t*} cache cache对象。
 * @param {uint32_t} cache_size 保留的个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t glyph_cache_shrink(glyph_cache_t* cache, uint32_t cache_size);

/**
 * @method glyph_cache_destroy
 * 释放cache对象和里面所有的glyph。
 *
 * @param {glyph_cache_t*} cache cache对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t glyph_cache_destroy(glyph_cache_t* cache);

END_C_DECLS

#endif /*TK_GLYPH_CACHE_H*/
