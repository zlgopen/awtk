/**
 * File:   scroll_bar.h
 * Author: AWTK Develop Team
 * Brief:  scroll_bar
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

#ifndef TK_SCROLL_BAR_H
#define TK_SCROLL_BAR_H

#include "base/widget.h"
#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class scroll_bar_t
 * @parent widget_t
 * @scriptable
 * scroll_bar控件。
 */
typedef struct _scroll_bar_t {
  widget_t widget;
  uint32_t max;
  uint32_t value;
  uint32_t row;
  widget_t* dragger;
  widget_animator_t* wa_value;
  widget_animator_t* wa_opactiy;
} scroll_bar_t;

/**
 * @method scroll_bar_create
 * @constructor
 * 创建scroll_bar对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* scroll_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
widget_t* scroll_bar_create_mobile(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);
widget_t* scroll_bar_create_desktop(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method scroll_bar_set_range
 * 创建scroll_bar对象
 * @param {widget_t*} widget scroll_bar控件。
 * @param {uint32_t} max 最大值。
 * @param {uint32_t} row 每一行的高度。
 *
 * @return {widget_t*} 对象。
 */
ret_t scroll_bar_set_params(widget_t* widget, uint32_t max, uint32_t row);

ret_t scroll_bar_scroll_to(widget_t* widget, uint32_t value, uint32_t duration);
ret_t scroll_bar_set_value(widget_t* widget, int32_t value);

#define SCROLL_BAR(widget) ((scroll_bar_t*)(widget))

END_C_DECLS

#endif /*TK_SCROLL_BAR_H*/
