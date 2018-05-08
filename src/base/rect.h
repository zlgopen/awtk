/**
 * File:   rect.h
 * Author: Li XianJing <xianjimli@hotmail.com>
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
 * @scriptable
 * 点。包括一个x坐标和一个y坐标。
 */
typedef struct _point_t {
  /**
   * @property {xy_t} x
   * @readonly
   * x坐标。
   */
  xy_t x;
  /**
   * @property {xy_t} y
   * @readonly
   * y坐标。
   */
  xy_t y;
} point_t;

/**
 * @class rect_t
 * @scriptable
 * 矩形。包括一个x坐标、y坐标、宽度和高度。
 */
typedef struct _rect_t {
  /**
   * @property {xy_t} x
   * @readonly
   * x坐标。
   */
  xy_t x;
  /**
   * @property {xy_t} y
   * @readonly
   * y坐标。
   */
  xy_t y;
  /**
   * @property {wh_t} w
   * @readonly
   * 宽度。
   */
  wh_t w;
  /**
   * @property {wh_t} h
   * @readonly
   * 高度。
   */
  wh_t h;
} rect_t;

#define rect_init(r, xx, yy, ww, hh) \
  r.x = (xx);                        \
  r.y = (yy);                        \
  r.w = (ww);                        \
  r.h = (hh);
#define rectp_init(r, xx, yy, ww, hh) \
  r->x = (xx);                        \
  r->y = (yy);                        \
  r->w = (ww);                        \
  r->h = (hh);

ret_t rect_merge(rect_t* dst_r, rect_t* r);
bool_t rect_contains(rect_t* r, xy_t x, xy_t y);

END_C_DECLS

#endif /*TK_RECT_H*/
