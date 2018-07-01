/**
 * File:   dragger.h
 * Author: AWTK Develop Team
 * Brief:  dragger
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
 * 2018-07-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DRAGGER_H
#define TK_DRAGGER_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class dragger_t
 * @parent widget_t
 * @scriptable
 * dragger控件。
 */
typedef struct _dragger_t {
  widget_t widget;
  xy_t x_min;
  xy_t y_min;
  xy_t x_max;
  xy_t y_max;

  xy_t save_x;
  xy_t save_y;
  xy_t down_x;
  xy_t down_y;
  bool_t dragging;
} dragger_t;

/**
 * @method dragger_create
 * @constructor
 * 创建dragger对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* dragger_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method dragger_set_range
 * 创建dragger对象
 * @param {widget_t*} widget dragger控件。
 * @param {xy_t} x_min x坐标最小值。
 * @param {xy_t} y_min y坐标最小值。
 * @param {xy_t} x_max x坐标最大值。
 * @param {xy_t} y_max y坐标最大值。
 *
 * @return {widget_t*} 对象。
 */
ret_t dragger_set_range(widget_t* widget, xy_t x_min, xy_t y_min, xy_t x_max, xy_t y_max);

#define DRAGGER(widget) ((dragger_t*)(widget))

END_C_DECLS

#endif /*TK_DRAGGER_H*/
