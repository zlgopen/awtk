/**
 * File:   color.h
 * Author: AWTK Develop Team
 * Brief:  color structs.
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 *
 */
typedef union _color_t {
  /**
   * @property {rgba_t} rgba
   * @annotation ["readable","writable"]
   * 颜色的RGBA值。
   */
  rgba_t rgba;
  /**
   * @property {uint32_t} color
   * @annotation ["readable","writable"]
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

typedef uint8_t gray_t;
const char* color_hex_str(color_t c, char str[8]);

END_C_DECLS

#endif /*TK_COLOR_H*/
