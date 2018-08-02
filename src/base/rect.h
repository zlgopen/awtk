/**
 * File:   rect.h
 * Author: AWTK Develop Team
 * Brief:  rect struct and utils functions.
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/types_def.h"

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

rect_t rect_init(xy_t x, xy_t y, wh_t w, wh_t h);

ret_t rect_merge(rect_t* dst_r, rect_t* r);
bool_t rect_contains(rect_t* r, xy_t x, xy_t y);
rect_t rect_fix(rect_t* r, wh_t max_w, wh_t max_h);
rect_t rect_intersect(const rect_t* r1, const rect_t* r2);

END_C_DECLS

#endif /*TK_RECT_H*/
