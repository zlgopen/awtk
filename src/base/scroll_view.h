/**
 * File:   scroll_view.h
 * Author: AWTK Develop Team
 * Brief:  scroll_view
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
 * 2018-07-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SCROLL_VIEW_H
#define TK_SCROLL_VIEW_H

#include "base/widget.h"
#include "base/velocity.h"
#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class scroll_view_t
 * @parent widget_t
 * @scriptable
 * 滚动视图。
 */
typedef struct _scroll_view_t {
  widget_t widget;

  wh_t virtual_w;
  wh_t virtual_h;

  /*private*/
  point_t down;
  int32_t xoffset;
  int32_t yoffset;
  int32_t xoffset_save;
  int32_t yoffset_save;
  uint32_t timer_id;

  velocity_t velocity;
  widget_animator_t* wa;

} scroll_view_t;

/**
 * @method scroll_view_create
 * @constructor
 * 创建scroll_view对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* scroll_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method scroll_view_set_virtual_w
 * 设置虚拟宽度。
 * @param {widget_t*} widget 控件对象。
 * @param {wh_t} w 虚拟宽度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t scroll_view_set_virtual_w(widget_t* widget, wh_t w);

/**
 * @method scroll_view_set_virtual_h
 * 设置虚拟高度。
 * @param {widget_t*} widget 控件对象。
 * @param {wh_t} h 虚拟高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t scroll_view_set_virtual_h(widget_t* widget, wh_t h);

#define SCROLL_VIEW(widget) ((scroll_view_t*)(widget))

END_C_DECLS

#endif /*TK_SCROLL_VIEW_H*/
