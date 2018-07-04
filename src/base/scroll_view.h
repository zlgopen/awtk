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

typedef ret_t (*scroll_view_fix_end_offset_t)(widget_t* widget);

/**
 * @class scroll_view_t
 * @parent widget_t
 * @scriptable
 * 滚动视图。
 */
typedef struct _scroll_view_t {
  widget_t widget;

  /**
   * @property {wh_t} virtual_w
   * @readonly
   * 虚拟宽度。
   */
  wh_t virtual_w;
  /**
   * @property {wh_t} virtual_h
   * @readonly
   * 虚拟高度。
   */
  wh_t virtual_h;
  /**
   * @property {int32_t_t} xoffset
   * @readonly
   * x偏移量。
   */
  int32_t xoffset;
  /**
   * @property {int32_t_t} yoffset
   * @readonly
   * y偏移量。
   */
  int32_t yoffset;

  /*private*/
  point_t down;
  int32_t xoffset_end;
  int32_t yoffset_end;
  int32_t xoffset_save;
  int32_t yoffset_save;

  velocity_t velocity;
  widget_animator_t* wa;
  scroll_view_fix_end_offset_t fix_end_offset;
  widget_on_layout_children_t on_layout_children;
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

/**
 * @method scroll_view_set_offset
 * 设置偏移量。
 * @param {widget_t*} widget 控件对象。
 * @param {xy_t} xoffset x偏移量。
 * @param {xy_t} yoffset x偏移量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t scroll_view_set_offset(widget_t* widget, int32_t xoffset, int32_t yoffset);

#define SCROLL_VIEW(widget) ((scroll_view_t*)(widget))

END_C_DECLS

#endif /*TK_SCROLL_VIEW_H*/
