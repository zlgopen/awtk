/**
 * File:   slide_view.h
 * Author: AWTK Develop Team
 * Brief:  slide_view
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
 * 2018-06-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SLIDE_VIEW_H
#define TK_SLIDE_VIEW_H

#include "base/widget.h"
#include "base/velocity.h"

BEGIN_C_DECLS

/**
 * @class slide_view_t
 * @parent widget_t
 * @scriptable
 * slide view。
 */
typedef struct _slide_view_t {
  widget_t widget;
  bool_t vertical;
  int32_t xoffset;
  int32_t yoffset;
  uint32_t active;
  point_t down;

  velocity_t velocity;
  bitmap_t prev_image;
  bitmap_t active_image;
  bitmap_t next_image;
} slide_view_t;

/**
 * @method slide_view_create
 * @constructor
 * 创建slide_view对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* slide_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

ret_t slide_view_set_active(widget_t* widget, uint32_t active);
ret_t slide_view_set_vertical(widget_t* widget, bool_t vertical);

#define SLIDE_VIEW(widget) ((slide_view_t*)(widget))

END_C_DECLS

#endif /*TK_SLIDE_VIEW_H*/
