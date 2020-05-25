/**
 * File:   color.h
 * Author: AWTK Develop Team
 * Brief:  color structs.
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

#ifndef TK_COLOR_H
#define TK_COLOR_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class rgba_t
 * 颜色的四个通道。
 *
 */
typedef struct _rgba_t {
  /**
   * @property {uint8_t} r
   * @annotation ["readable","writable"]
   * 红色。
   */
  uint8_t r;
  /**
   * @property {uint8_t} g
   * @annotation ["readable","writable"]
   * 绿色。
   */
  uint8_t g;
  /**
   * @property {uint8_t} b
   * @annotation ["readable","writable"]
   * 蓝色。
   */
  uint8_t b;
  /**
   * @property {uint8_t} a
   * @annotation ["readable","writable"]
   * alpha。
   */
  uint8_t a;
} rgba_t;

/**
 * @class color_t
 * 颜色。
 * @annotation ["scriptable"]
 *
 */
typedef union _color_t {
  /**
   * @property {rgba_t} rgba
   * @annotation ["readable", "writable"]
   * 颜色的RGBA值。
   */
  rgba_t rgba;
  /**
   * @property {uint32_t} color
   * @annotation ["readable", "writable", "scriptable"]
   * 颜色的数值。
   */
  uint32_t color;
} color_t;

/**
 * @method color_init
 * 初始化颜色对象。
 * @annotation ["constructor"]
 * @param {uint8_t} r 红色。
 * @param {uint8_t} g 绿色。
 * @param {uint8_t} b 蓝色。
 * @param {uint8_t} a alpha。
 *
 * @return {color_t} 颜色对象。
 */
color_t color_init(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/**
 * @method color_create
 * 创建color对象。
 *
 * > 主要供脚本语言使用。
 *
 * @annotation ["constructor", "scriptable", "gc"]
 * @param {uint8_t} r 红色通道。
 * @param {uint8_t} b 蓝色通道。
 * @param {uint8_t} g 绿色通道。
 * @param {uint8_t} a alpha通道。
 *
 * @return {color_t*} color对象。
 */
color_t* color_create(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/**
 * @method color_from_str
 * 创建color对象。
 *
 * > 主要供脚本语言使用。
 *
 * @annotation ["scriptable"]
 * @param {color_t*} c color对象。
 * @param {const char*} str css类似的颜色值。
 *
 * @return {color_t*} color对象。
 */
color_t* color_from_str(color_t* c, const char* str);

/**
 * @method color_r
 *
 * 获取红色通道的值。
 *
 * > 主要供脚本语言使用。
 *
 * @annotation ["scriptable"]
 * @param {color_t*} c color对象。
 *
 * @return {uint8_t} 返回红色通道的值。
 *
 */
uint8_t color_r(color_t* c);

/**
 * @method color_g
 *
 * 获取绿色通道的值。
 *
 * > 主要供脚本语言使用。
 *
 * @annotation ["scriptable"]
 * @param {color_t*} c color对象。
 *
 * @return {uint8_t} 返回绿色通道的值。
 *
 */
uint8_t color_g(color_t* c);

/**
 * @method color_b
 *
 * 获取蓝色通道的值。
 *
 * > 主要供脚本语言使用。
 *
 * @annotation ["scriptable"]
 * @param {color_t*} c color对象。
 *
 * @return {uint8_t} 返回蓝色通道的值。
 *
 */
uint8_t color_b(color_t* c);

/**
 * @method color_a
 *
 * 获取alpha通道的值。
 *
 * > 主要供脚本语言使用。
 *
 * @annotation ["scriptable"]
 * @param {color_t*} c color对象。
 *
 * @return {uint8_t} 返回alpha通道的值。
 *
 */
uint8_t color_a(color_t* c);

/**
 * @method color_cast
 * 转换为color对象。
 *
 * > 供脚本语言使用。
 * @annotation ["cast", "scriptable"]
 * @param {color_t*} color color对象。
 *
 * @return {color_t*} color对象。
 */
color_t* color_cast(color_t* color);

/**
 * @method color_destroy
 *
 * 销毁color对象。
 * > 主要供脚本语言使用。
 *
 * @annotation ["deconstructor", "scriptable", "gc"]
 * @param {color_t*} c color对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t color_destroy(color_t* c);

#define TK_COLOR_HEX_LEN 11
#define TK_COLOR_RGBA_LEN 31

/**
 * @method color_hex_str
 * 获取16进制格式表示的颜色
 * @param {color_t*} c color对象。
 * @param {char*} str 内存。
 *
 * @return {char*} 获取16进制字符串表示的颜色。
 */
const char* color_hex_str(color_t c, char str[TK_COLOR_HEX_LEN + 1]);

/**
 * @method color_rgba_str
 * 获取rgba格式表示的颜色
 * @param {color_t*} c color对象。
 * @param {char*} str 内存。
 *
 * @return {char*} 获取rgba格式表示的颜色。
 */
const char* color_rgba_str(color_t c, char str[TK_COLOR_RGBA_LEN + 1]);

END_C_DECLS

#endif /*TK_COLOR_H*/
