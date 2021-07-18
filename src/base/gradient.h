
/**
 * File: gradient.h   
 * Author: AWTK Develop Team
 * Brief:  vector graphic gradient
 *
 * Copyright (c) 2021 - 2021 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-07-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GRADIENT_H
#define TK_GRADIENT_H

#include "tkc/color.h"
#include "tkc/str.h"
#include "tkc/buffer.h"

BEGIN_C_DECLS

#ifndef TK_GRADIENT_MAX_STOP_NR
#define TK_GRADIENT_MAX_STOP_NR 8
#endif /*TK_GRADIENT_MAX_STOP_NR*/

/**
 * @enum gradient_type_t
 * 渐变色类型。
 */
typedef enum _gradient_type_t {
  /**
   * @const GRADIENT_NONE
   * 无效类型。
   */
  GRADIENT_NONE,
  /**
   * @const GRADIENT_LINEAR
   * 线性渐变。
   */
  GRADIENT_LINEAR,
  /**
   * @const GRADIENT_RADIAL
   * 放射渐变。
   */
  GRADIENT_RADIAL,
} gradient_type_t;

struct _gradient_stop_t;
typedef struct _gradient_stop_t gradient_stop_t;

/**
 * @class gradient_stop_t
 * 渐变关键点。
 */
struct _gradient_stop_t {
  /**
   * @property {color_t} color
   * @annotation ["readable"]
   * 颜色。
   */
  color_t color;
  /**
   * @property {float} offset
   * @annotation ["readable"]
   * 偏移量(0-1)。
   */
  float offset;
};

struct _gradient_t;
typedef struct _gradient_t gradient_t;

/**
 * @class gradient_t
 * 渐变。
 */
struct _gradient_t {
  /**
   * @property {gradient_type_t} type
   * @annotation ["readable"]
   * 类型。
   */
  gradient_type_t type;

  /**
   * @property {uint32_t} degree
   * @annotation ["readable"]
   * 线性渐变的角度。
   */
  uint32_t degree;

  /**
   * @property {uint32_t} nr
   * @annotation ["readable"]
   * stop个数。
   */
  uint32_t nr;

  /*private*/
  gradient_stop_t stops[TK_GRADIENT_MAX_STOP_NR];
};

/**
 * @method gradient_init
 * 初始化gradient对象。
 * @param {gradient_t*} gradient gradient对象。
 *
 * @return {gradient_t*} 返回gradient对象。
 */
gradient_t* gradient_init(gradient_t* gradient);

/**
 * @method gradient_init_simple
 * 初始化gradient对象。
 * @param {gradient_t*} gradient gradient对象。
 * @param {uint32_t} color 颜色。
 *
 * @return {gradient_t*} 返回gradient对象。
 */
gradient_t* gradient_init_simple(gradient_t* gradient, uint32_t color);

/**
 * @method gradient_init_from_str
 * 从字符串初始化gradient对象。
 *
 * > 字符串的格式兼容CSS，请参考相关文档。
 * @param {gradient_t*} gradient gradient对象。
 * @param {const char*} str 字符串。
 *
 * @return {gradient_t*} 返回gradient对象。
 */
gradient_t* gradient_init_from_str(gradient_t* gradient, const char* str);

/**
 * @method gradient_init_from_binary
 * 从二进制数据初始化gradient对象。
 *
 * > 二进制数据必须由gradient_to_binary生成。
 * @param {gradient_t*} gradient gradient对象。
 * @param {const uint8_t*} data 数据。
 * @param {uint32_t} size 数据长度。
 *
 * @return {gradient_t*} 返回gradient对象。
 */
gradient_t* gradient_init_from_binary(gradient_t* gradient, const uint8_t* data, uint32_t size);

/**
 * @method gradient_set_type
 * 设置类型。
 *
 * @param {gradient_t*} gradient gradient对象。
 * @param {gradient_type_t} type 类型。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t gradient_set_type(gradient_t* gradient, gradient_type_t type);

/**
 * @method gradient_set_linear_degree
 * 设置线性渐变的角度。
 *
 * @param {gradient_t*} gradient gradient对象。
 * @param {uint32_t} degree 角度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t gradient_set_linear_degree(gradient_t* gradient, uint32_t degree);

/**
 * @method gradient_to_str
 * 转换成字符串格式。
 *
 * @param {gradient_t*} gradient gradient对象。
 * @param {str_t*} str 字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t gradient_to_str(gradient_t* gradient, str_t* str);

/**
 * @method gradient_to_binary
 * 转成二进制格式。
 *
 * @param {gradient_t*} gradient gradient对象。
 * @param {wbuffer_t*} wb 二进制数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t gradient_to_binary(gradient_t* gradient, wbuffer_t* wb);

/**
 * @method gradient_add_stop
 * 增加关键点。
 *
 * @param {gradient_t*} gradient gradient对象。
 * @param {color_t} color 颜色。
 * @param {float} stop 位置。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t gradient_add_stop(gradient_t* gradient, color_t color, float stop);

/**
 * @method gradient_get_stop
 * 获取关键点。
 *
 * @param {gradient_t*} gradient gradient对象。
 * @param {uint32_t} index 序数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
gradient_stop_t* gradient_get_stop(gradient_t* gradient, uint32_t index);

/**
 * @method gradient_get_first_color
 * 获取开始的颜色。
 *
 * @param {gradient_t*} gradient gradient对象。
 *
 * @return {color_t} 返回颜色。
 */
color_t gradient_get_first_color(gradient_t* gradient);

/**
 * @method gradient_get_last_color
 * 获取结束的颜色。
 *
 * @param {gradient_t*} gradient gradient对象。
 *
 * @return {color_t} 返回颜色。
 */
color_t gradient_get_last_color(gradient_t* gradient);

/**
 * @method gradient_get_color
 * 获取颜色。
 *
 * @param {gradient_t*} gradient gradient对象。
 * @param {float} offset 偏移(0-1)。
 *
 * @return {color_t} 返回颜色。
 */
color_t gradient_get_color(gradient_t* gradient, float offset);

/**
 * @method gradient_deinit
 * 释放gradient对象。
 *
 * @param {gradient_t*} gradient gradient对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t gradient_deinit(gradient_t* gradient);

END_C_DECLS

#endif /*TK_GRADIENT_H*/
