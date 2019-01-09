/**
 * File:   rect.h
 * Author: AWTK Develop Team
 * Brief:  rect struct and utils functions.
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

#ifndef TK_RECT_H
#define TK_RECT_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class point_t
 * @annotation ["scriptable"]
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

typedef struct _pointf_t {
  float_t x;
  float_t y;
} pointf_t;

/**
 * @class rect_t
 * @annotation ["scriptable"]
 * 矩形。包括一个x坐标、y坐标、宽度和高度。
 */
typedef struct _rect_t {
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
  /**
   * @property {wh_t} w
   * @annotation ["readable"]
   * 宽度。
   */
  wh_t w;
  /**
   * @property {wh_t} h
   * @annotation ["readable"]
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
 * @annotation ["constructor", "scriptable"]
 * @param {xy_t} x x坐标。
 * @param {xy_t} y y坐标。
 * @param {wh_t} w 宽度。
 * @param {wh_t} h 高度。
 *
 * @return {rect_t*} rect对象。
 */
rect_t* rect_create(xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method rect_destroy
 *
 * 销毁rect对象。
 * > 主要供脚本语言使用。
 *
 * @annotation ["deconstructor", "scriptable"]
 * @param {rect_t*} r rect对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t rect_destroy(rect_t* r);

rect_t rect_init(xy_t x, xy_t y, wh_t w, wh_t h);
ret_t rect_merge(rect_t* dst_r, rect_t* r);
bool_t rect_contains(rect_t* r, xy_t x, xy_t y);
rect_t rect_fix(rect_t* r, wh_t max_w, wh_t max_h);
rect_t rect_intersect(const rect_t* r1, const rect_t* r2);

END_C_DECLS

#endif /*TK_RECT_H*/
