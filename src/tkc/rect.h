/**
 * File:   rect.h
 * Author: AWTK Develop Team
 * Brief:  rect struct and utils functions.
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

#ifndef TK_RECT_H
#define TK_RECT_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class point_t
 * @annotation ["scriptable"]
 * @order -10
 * 点。包括一个x坐标和一个y坐标。
 */
typedef struct _point_t {
  /**
   * @property {xy_t} x
   * @annotation ["readable"]
   * x坐标。
   */
  xy_t x;
  /**
   * @property {xy_t} y
   * @annotation ["readable"]
   * y坐标。
   */
  xy_t y;
} point_t;

/**
 * @class pointf_t
 * @order -10
 * @annotation ["scriptable"]
 * 点(浮点数)。包括一个x坐标和一个y坐标。
 */
typedef struct _pointf_t {
  /**
   * @property {float_t} x
   * @annotation ["readable"]
   * x坐标。
   */
  float_t x;
  /**
   * @property {float_t} y
   * @annotation ["readable"]
   * y坐标。
   */
  float_t y;
} pointf_t;

/**
 * @class rect_t
 * @order -10
 * @annotation ["scriptable"]
 * 矩形。包括一个x坐标、y坐标、宽度和高度。
 */
typedef struct _rect_t {
  /**
   * @property {xy_t} x
   * @annotation ["readable", "scriptable"]
   * x坐标。
   */
  xy_t x;
  /**
   * @property {xy_t} y
   * @annotation ["readable", "scriptable"]
   * y坐标。
   */
  xy_t y;
  /**
   * @property {wh_t} w
   * @annotation ["readable", "scriptable"]
   * 宽度。
   */
  wh_t w;
  /**
   * @property {wh_t} h
   * @annotation ["readable", "scriptable"]
   * 高度。
   */
  wh_t h;
} rect_t;

/**
 * @method rect_create
 * 创建rect对象。
 *
 * > 主要供脚本语言使用。
 *
 * @annotation ["constructor", "scriptable", "gc"]
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 * @param {wh_t} w 宽度。
 * @param {wh_t} h 高度。
 *
 * @return {rect_t*} rect对象。
 */
rect_t* rect_create(xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method rect_set
 * 设置rect对象的xywh。
 *
 * > 主要供脚本语言使用。
 *
 * @annotation ["scriptable"]
 * @param {rect_t*} rect rect对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 * @param {wh_t} w 宽度。
 * @param {wh_t} h 高度。
 *
 * @return {rect_t*} rect对象。
 */
rect_t* rect_set(rect_t* rect, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method rect_cast
 * 转换为rect对象。
 *
 * > 供脚本语言使用。
 * @annotation ["cast", "scriptable"]
 * @param {rect_t*} rect rect对象。
 *
 * @return {rect_t*} rect对象。
 */
rect_t* rect_cast(rect_t* rect);

/**
 * @method rect_destroy
 * 销毁rect对象。
 *
 * > 主要供脚本语言使用。
 *
 * @annotation ["deconstructor", "scriptable", "gc"]
 * @param {rect_t*} r rect对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rect_destroy(rect_t* r);

/**
 * @method rect_scale
 * 缩放rect对象。
 *
 * @param {rect_t*} r rect对象。
 * @param {float_t} scale 缩放比例。
 *
 * @return {rect_t*} 返回rect对象。
 */
rect_t* rect_scale(rect_t* r, float_t scale);

/**
 * @method rect_init
 * 初始化rect对象。
 *
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 * @param {wh_t} w 宽度。
 * @param {wh_t} h 高度。
 *
 * @return {rect_t} 返回rect对象。
 */
rect_t rect_init(xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method rect_merge
 * 合并两个rect对象。
 *
 * @param {rect_t*} dst_r rect对象。
 * @param {const rect_t*} r rect对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rect_merge(rect_t* dst_r, const rect_t* r);

/**
 * @method rect_contains
 * 判断指定的点在rect范围内。
 *
 * @param {rect_t*} r rect对象。
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 *
 * @return {bool_t} 返回在rect范围内。
 */
bool_t rect_contains(rect_t* r, xy_t x, xy_t y);

/**
 * @method rect_fix
 * 确保rect在指定的大小范围内。
 *
 * @param {rect_t*} r rect对象。
 *
 * @return {rect_t} 返回修复之后的rect对象。
 */
rect_t rect_fix(rect_t* r, wh_t max_w, wh_t max_h);

/**
 * @method rect_intersect
 * 求两个rect的交集。
 *
 * @param {const rect_t*} r1 rect对象。
 * @param {const rect_t*} r2 rect对象。
 *
 * @return {rect_t} 返回交集。
 */
rect_t rect_intersect(const rect_t* r1, const rect_t* r2);

END_C_DECLS

#endif /*TK_RECT_H*/
