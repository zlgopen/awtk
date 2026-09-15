/**
 * File:   font.h
 * Author: AWTK Develop Team
 * Brief:  font interface
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FONT_H
#define TK_FONT_H

#include "base/types_def.h"

BEGIN_C_DECLS

/**
 * @enum font_bidi_type_t
 * @annotation ["scriptable"]
 * @prefix FONT_BIDI_TYPE_
 * 字库 bidi 类型常量定义。
 */
typedef enum _font_bidi_type_t {
  /**
   * @const FONT_BIDI_TYPE_AUTO
   * 自动检查。
   */
  FONT_BIDI_TYPE_AUTO = 0,
  /**
   * @const FONT_BIDI_TYPE_LTR
   * Left-To-Right letter。
   */
  FONT_BIDI_TYPE_LTR,
  /**
   * @const FONT_BIDI_TYPE_RTL
   * Right-To-Left letter。
   */
  FONT_BIDI_TYPE_RTL,
  /**
   * @const FONT_BIDI_TYPE_LRO
   * Left-To-Right letter Override。
   */
  FONT_BIDI_TYPE_LRO,
  /**
   * @const FONT_BIDI_TYPE_RLO
   * Right-To-Left letter Override。
   */
  FONT_BIDI_TYPE_RLO,
  /**
   * @const FONT_BIDI_TYPE_WLTR
   * Weak Left To Right paragraph。
   */
  FONT_BIDI_TYPE_WLTR,
  /**
   * @const FONT_BIDI_TYPE_WRTL
   * Weak Right To Left paragraph。
   */
  FONT_BIDI_TYPE_WRTL
} font_bidi_type_t;

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
  GLYPH_FMT_RGBA,
  /**
   * @const GLYPH_FMT_ALPHA2
   * 每个像素占用2bit。
   */
  GLYPH_FMT_ALPHA2,
  /**
   * @const GLYPH_FMT_ALPHA4
   * 每个像素占用4bit。
   */
  GLYPH_FMT_ALPHA4
} glyph_format_t;

struct _font_t;
typedef struct _font_t font_t;

typedef struct _font_vmetrics_t {
  int16_t ascent;
  int16_t descent;
  int32_t line_gap;
  uint16_t units_per_em;
  int16_t font_ascender;
  int16_t font_descender;
} font_vmetrics_t;

typedef struct _glyph_t glyph_t;
/**
 * @class glyph_t
 * 代表一个字符的字模。
 *
 */
#pragma pack(push, 1)
struct _glyph_t {
  /**
   * @property {uint32_t} glyph_index
   * @annotation ["readable"]
   * 字模序号。
   */
  uint32_t glyph_index;
  /**
   * @property {wchar_t} chr
   * @annotation ["readable"]
   * 字符。
   */
  wchar_t chr;
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
  int16_t advance : 16;
  /**
   * @property {int16_t} bidi_type
   * @annotation ["readable"]
   * bidi 类型。
   */
  int16_t bidi_type;
  /**
   * @property {uint16_t} str_count
   * @annotation ["readable"]
   * 该字模簇的字符串组成数量。
   */
  uint16_t str_count;
  /**
   * @property {uint32_t} glyph_count
   * @annotation ["readable"]
   * 该字模簇的字模组成数量。
   */
  uint16_t glyph_count;
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
  /**
   * @property {const glyph_t*} next_glyph
   * @annotation ["readable"]
   * 关联的下一个字模。（有一些字符整形后会由一个字符变成多个字模）
   */
  const glyph_t* next_glyph;
};
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
 * 克隆glyph对象。
 * @annotation ["constructor"]
 * @param {glyph_t*} g glyph对象。
 *
 * @return {glyph_t*} glyph对象。
 */
glyph_t* glyph_clone(glyph_t* g);

/**
 * @method glyph_destroy
 * 销毁glyph对象。
 * @annotation ["deconstructor"]
 * @param {glyph_t*} g glyph对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t glyph_destroy(glyph_t* g);

/**
 * @class glyphs_t
 * 字模列表。
 * 备注：在 OpenGL 模式下，会跟随 system_info 对象中的 device_pixel_ratio 来缩放
 */
typedef struct _glyphs_t {
  wchar_t* str;
  uint32_t str_length;
  glyph_t* glyphs;
  uint32_t length;
  int32_t* l2v_maps;
  int32_t* v2l_maps;
  font_size_t font_size;
  font_t* font;
  int16_t ascent;
  int16_t descent;
  bool_t valid;
} glyphs_t;

/**
 * @method glyphs_create
 * 创建glyphs对象。
 * 备注：内部函数
 * @annotation ["constructor"]
 * @param {wchar_t*} str 字符串。
 * @param {uint32_t} glyph_len 字模长度。
 * @param {uint32_t} str_length 字符串长度。
 * @param {font_size_t} font_size 字号。
 * @param {const font_vmetrics_t*} vmetrics 字体度量信息。
 *
 * @return {glyphs_t*} glyph对象。
 */
glyphs_t* glyphs_create(wchar_t* str, uint32_t glyph_len, uint32_t str_length, bool_t has_maps,
                        font_size_t font_size, const font_vmetrics_t* vmetrics);

/**
 * @method glyphs_destroy
 * 销毁glyphs对象。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t glyphs_destroy(glyphs_t* glyphs);

/**
 * @method glyphs_get_str
 * 获取 glyphs 对象的字符串。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 *
 * @return {const wchar_t*} 返回字符串。
 */
const wchar_t* glyphs_get_str(glyphs_t* glyphs);

/**
 * @method glyphs_get_str_length
 * 获取 glyphs 对象的字符串个数。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 *
 * @return {uint32_t} 返回个数。
 */
uint32_t glyphs_get_str_length(glyphs_t* glyphs);

/**
 * @method glyphs_get_length
 * 获取 glyphs 对象的字模个数。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 *
 * @return {uint32_t} 返回个数。
 */
uint32_t glyphs_get_length(glyphs_t* glyphs);

/**
 * @method glyphs_get_valid
 * 获取 glyphs 对象的是否合法。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 *
 * @return {bool_t} 返回是否合法。
 */
bool_t glyphs_get_valid(glyphs_t* glyphs);

/**
 * @method glyphs_get_glyph_index_from_str_index
 * 根据字符串序号获取 glyphs 对象的对应字模的序号。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 * @param {uint32_t} str_index 字符串序号。
 *
 * @return {int32_t} 返回字号。
 */
int32_t glyphs_get_glyph_index_from_str_index(glyphs_t* glyphs, uint32_t str_index);

/**
 * @method glyphs_get_glyph_indexs_from_str_indexs
 * 根据子串获取 glyphs 对象的对应字模的序号集合。
 * 要求 dst 为 NULL 或者数组首地址，调用后 dst 可能无效，请保留返回的地址。
 * 该函数会自动申请内存，调用后需要使用TKMEM_FREE释放。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 * @param {uint32_t} str_start 字符串序号。
 * @param {uint32_t} len 字符串长度。
 * @param {int32_t*} dst 目标数组。
 * @param {uint32_t} dst_cap 目标数组容量。
 * @param {int32_t*} ret_cap 实际返回字模序号集合容量。
 *
 * @return {int32_t*} 返回目标数组指针。
*/
int32_t* glyphs_get_glyph_indexs_from_str_indexs(glyphs_t* glyphs, uint32_t str_start, uint32_t len,
                                                 int32_t* dst, uint32_t dst_cap, int32_t* ret_cap);

/**
 * @method glyphs_get_str_index_from_glyph_index
 * 根据字模序号获取 glyphs 对象的对应字符串的序号。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 * @param {uint32_t} glyph_index 字模序号。
 *
 * @return {int32_t} 返回字号。
 */
int32_t glyphs_get_str_index_from_glyph_index(glyphs_t* glyphs, uint32_t glyph_index);

/**
 * @method glyphs_get_font_size
 * 获取 glyphs 对象的逻辑字号。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 *
 * @return {font_size_t} 返回字号。
 */
font_size_t glyphs_get_font_size(glyphs_t* glyphs);

/**
 * @method glyphs_get_physical_font_size
 * 获取 glyphs 对象的物理字号。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 *
 * @return {font_size_t} 返回字号。
 */
font_size_t glyphs_get_physical_font_size(glyphs_t* glyphs);

/**
 * @method glyphs_get
 * 根据 index 获取字模。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 * @param {uint32_t} index 字模序号。
 *
 * @return {const glyph_t*} 成功返回字模对象，失败返回 NULL。
 */
const glyph_t* glyphs_get(glyphs_t* glyphs, uint32_t index);

/**
 * @method glyphs_get_font_ascent
 * 获取字体逻辑 ascent。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 *
 * @return {int16_t} 返回 ascent 。
 */
int16_t glyphs_get_font_ascent(glyphs_t* glyphs);

/**
 * @method glyphs_get_physical_font_ascent
 * 获取字体物理 ascent。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 *
 * @return {int16_t} 返回 ascent 。
 */
int16_t glyphs_get_physical_font_ascent(glyphs_t* glyphs);

/**
 * @method glyphs_measure
 * 计算字模长度。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 * @param {uint32_t} start 字模开始序号。
 * @param {uint32_t} len 字模长度。
 *
 * @return {float_t} 成功返回长度，失败返回 0。
 */
float_t glyphs_measure(glyphs_t* glyphs, uint32_t start, uint32_t len);

/**
 * @method glyphs_get_height
 * 计算字模高度。
 * @annotation ["deconstructor"]
 * @param {glyphs_t*} glyphs glyphs对象。
 *
 * @return {int32_t} 成功返回高度，失败返回 -1。
 */
int32_t glyphs_get_height(glyphs_t* glyphs);

typedef struct _font_raster_params_t {
  /*加粗*/
  uint32_t bold : 1;
  /*斜体*/
  uint32_t italic : 1;
  /*描边*/
  uint32_t stroker : 1;
  /*阴影*/
  uint32_t shadow : 1;
  /*下划线*/
  uint32_t underline : 1;
  /*整形*/
  uint32_t shaping : 1;
  /*对齐模式*/
  font_bidi_type_t bidi_type;
} font_raster_params_t;

typedef font_vmetrics_t (*font_get_vmetrics_t)(font_t* f, font_size_t font_size);
typedef bool_t (*font_match_t)(font_t* f, const char* name, font_size_t font_size);
typedef ret_t (*font_get_glyph_t)(font_t* f, wchar_t chr, font_size_t font_size, glyph_t* g);
typedef ret_t (*font_shrink_cache_t)(font_t* f, uint32_t cache_size);
typedef glyphs_t* (*font_create_glyphs_t)(font_t* f, const wchar_t* str, uint32_t len,
                                          font_size_t font_size, font_raster_params_t* params);

typedef ret_t (*font_destroy_t)(font_t* f);

/**
 * @class font_t
 * 代表一种字体。
 *
 */
struct _font_t {
  char name[MAX_PATH + 1];
  font_match_t match;
  font_get_glyph_t get_glyph;
  font_get_vmetrics_t get_vmetrics;
  font_shrink_cache_t shrink_cache;
  font_create_glyphs_t create_glyphs;
  font_destroy_t destroy;
  const char* desc;
  font_raster_params_t params;
  font_manager_t* fm;
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
 * @return {bool_t} 返回TRUE表示成功，FALSE表示失败。
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
 * @method font_create_glyphs
 * 创建字模列表对象（根据指定字符）。
 * 备注：
 * 1.该函数会根据 font_raster_params_t 对象来处理和提取字模列表（包括字体整形等操作）
 * 2.返回的 glyphs_t 对象，需要用户调用 glyphs_destroy 函数释放。
 * @param {font_t*} font font对象。
 * @param {const wchar_t*} str 字符串。
 * @param {uint32_t} len 字符串长度。
 * @param {font_size_t} font_size 字体大小。
 * @param {font_raster_params_t*} params 属性。（如果该参数为 NULL，就使用 font_t 对象的 params 属性）
 *
 * @return {glyphs_t*} 成功返回字模列表对象，失败返回NULL。
 */
glyphs_t* font_create_glyphs(font_t* f, const wchar_t* str, uint32_t len, font_size_t font_size,
                             font_raster_params_t* params);

/**
 * @method font_set_raster_params
 * 设置 font 对象的字模光栅属性。
 *
 * @param {font_t*} font font对象。
 * @param {const font_raster_params_t*} params 属性。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_set_raster_params(font_t* font, const font_raster_params_t* params);

/**
 * @method font_get_raster_params
 * 获取 font 对象的字模光栅属性。
 *
 * @param {font_t*} font font对象。
 * @param {font_raster_params_t*} params 返回字模光栅属性。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t font_get_raster_params(font_t* font, font_raster_params_t* params);

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
