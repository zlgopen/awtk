/**
 * File:   font.h
 * Author: AWTK Develop Team
 * Brief:  font interface
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FONT_H
#define TK_FONT_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @enum glyph_format_t
 * @prefix GLYPH_FMT_
 * @annotation ["scriptable"]
 * 字模格式常量定义。
 */
typedef enum _glyph_format_t {
  /**
   * @const GLYPH_FMT_ALPHA
   * 每个像素占用1个字节(缺省)。
   */
  GLYPH_FMT_ALPHA,
  /**
   * @const GLYPH_FMT_MONO
   * 每个像素占用1个比特。
   */
  GLYPH_FMT_MONO,
  /**
   * @const GLYPH_FMT_RGBA
   * 每个像素占用4个字节。
   */
  GLYPH_FMT_RGBA
} glyph_format_t;

struct _font_t;
typedef struct _font_t font_t;

/**
 * @class glyph_t
 * 代表一个字符的字模。
 *
 */
#pragma pack(push, 1)
typedef struct _glyph_t {
  /**
   * @property {int16_t} x
   * @annotation ["readable"]
   * x坐标。
   */
  int16_t x;
  /**
   * @property {int16_t} y
   * @annotation ["readable"]
   * y坐标。
   */
  int16_t y;
  /**
   * @property {uint16_t} w
   * @annotation ["readable"]
   * 宽度。
   */
  uint16_t w;
  /**
   * @property {uint16_t} h
   * @annotation ["readable"]
   * 高度。
   */
  uint16_t h;
  /**
   * @property {uint16_t} advance
   * @annotation ["readable"]
   * 占位宽度。
   */
  uint16_t advance : 16;
  /**
   * @property {uint8_t} format
   * @annotation ["readable"]
   * 格式。
   */
  uint8_t format : 8;
  /**
   * @property {uint8_t} pitch
   * @annotation ["readable"]
   * pitch。
   */
  uint8_t pitch : 8;
  /**
   * @property {const uint8_t*} data
   * @annotation ["readable"]
   * 数据。
   */
  const uint8_t* data;
} glyph_t;
#pragma pack(pop)

/**
 * @method glyph_create
 * @annotation ["constructor"]
 * 创建glyph对象。
 *
 * @return {glyph_t*} glyph对象。
 */
glyph_t* glyph_create(void);

/**
 * @method glyph_clone
 * @annotation ["constructor"]
 * 克隆glyph对象。
 *
 * @return {glyph_t*} glyph对象。
 */
glyph_t* glyph_clone(glyph_t* g);

/**
 * @method glyph_destroy
 * 销毁glyph对象。
 * @annotation ["deconstructor"]
 * @param {glyph_t*} glyph glyph对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t glyph_destroy(glyph_t* g);

typedef struct _font_vmetrics_t {
  int16_t ascent;
  int16_t descent;
  int32_t line_gap;
} font_vmetrics_t;

typedef font_vmetrics_t (*font_get_vmetrics_t)(font_t* f, font_size_t font_size);
typedef bool_t (*font_match_t)(font_t* f, const char* name, font_size_t font_size);
typedef ret_t (*font_get_glyph_t)(font_t* f, wchar_t chr, font_size_t font_size, glyph_t* g);
typedef ret_t (*font_shrink_cache_t)(font_t* f, uint32_t cache_size);

typedef ret_t (*font_destroy_t)(font_t* f);

/**
 * @class font_t
 * 代表一种字体。
 *
 */
struct _font_t {
  char name[TK_NAME_LEN + 1];
  font_match_t match;
  font_get_glyph_t get_glyph;
  font_get_vmetrics_t get_vmetrics;
  font_shrink_cache_t shrink_cache;
  font_destroy_t destroy;
  const char* desc;
};

/**
 * @method font_get_vmetrics
 * 获取字体的高度信息。
 * @param {font_t*} font font对象。
 * @param {font_size_t} font_size 字体大小。
 *
 * @return {font_vmetrics_t} 返回字体的高度信息。
 */
font_vmetrics_t font_get_vmetrics(font_t* font, font_size_t font_size);

/**
 * @method font_match
 * 检查当前字体是否与指定名称和大小相匹配。
 *
 * @param {font_t*} font font对象。
 * @param {const char*} font_name 字体名称。
 * @param {font_size_t} font_size 字体大小。
 *
 * @return {int32_t} 返回TRUE表示成功，FALSE表示失败。
 */
bool_t font_match(font_t* font, const char* font_name, font_size_t font_size);

/**
 * @method font_get_glyph
 * 获取指定字符和大小的字模。
 *
 * @param {font_t*} font font对象。
 * @param {wchar_t} chr 字符。
 * @param {font_size_t} font_size 字体大小。
 * @param {glyph_t*} glyph 返回字模数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_get_glyph(font_t* font, wchar_t chr, font_size_t font_size, glyph_t* glyph);

/**
 * @method font_shrink_cache
 * 清除最近没使用的字模。
 *
 * @param {font_t*} font font对象。
 * @param {uint32_t} cache_size 保留缓存字模的个数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_shrink_cache(font_t* font, uint32_t cache_size);

/**
 * @method font_destroy
 * 销毁font对象。
 * @annotation ["deconstructor"]
 * @param {font_t*} font font对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_destroy(font_t* font);

END_C_DECLS

#endif /*TK_FONT_H*/
