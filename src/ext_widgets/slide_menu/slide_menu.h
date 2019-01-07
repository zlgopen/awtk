/**
 * File:   slide_menu.h
 * Author: AWTK Develop Team
 * Brief:  slide_menu
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
 * 2018-12-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SLIDE_MENU_H
#define TK_SLIDE_MENU_H

#include "base/widget.h"
#include "base/velocity.h"
#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class slide_menu_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * slide menu。
 * 一般用一组按钮作为子控件，通过左右滑动改变当前的项。
 * 除了当菜单使用外，也可以用来切换页面。
 */
typedef struct _slide_menu_t {
  widget_t widget;

  /**
   * @property {int32_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值。代表当前选中项的索引。
   */
  int32_t value;

  /**
   * @property {align_v_t} align_v
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 垂直对齐方式。
   */
  align_v_t align_v;

  /**
   * @property {float_t} min_scale
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 最小缩放比例。
   */
  float_t min_scale;

  /*private*/
  int32_t xdown;
  int32_t xoffset;
  bool_t dragged;
  velocity_t velocity;
  widget_animator_t* wa;
} slide_menu_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值(当前项)即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值(当前项)改变事件。
 */

/**
 * @method slide_menu_create
 * 创建slide_menu对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* slide_menu_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method slide_menu_cast
 * 转换slide_menu对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget slide_menu对象。
 *
 * @return {widget_t*} slide_menu对象。
 */
widget_t* slide_menu_cast(widget_t* widget);

/**
 * @method slide_menu_set_value
 * 设置当前项。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_menu对象。
 * @param {uint32_t} value 当前项的索引。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_menu_set_value(widget_t* widget, uint32_t value);

/**
 * @method slide_menu_set_align_v
 * 设置垂直对齐方式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_menu对象。
 * @param {align_v_t} align_v 对齐方式。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_menu_set_align_v(widget_t* widget, align_v_t align_v);

/**
 * @method slide_menu_set_min_scale
 * 设置最小缩放比例。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_menu对象。
 * @param {float_t} min_scale 最小缩放比例，范围[0.5-1]。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_menu_set_min_scale(widget_t* widget, float_t min_scale);

#define SLIDE_MENU_PROP_MIN_SCALE "min_scale"

#define WIDGET_TYPE_SLIDE_MENU "slide_menu"
#define SLIDE_MENU(widget) ((slide_menu_t*)(widget))

/*public for test*/
int32_t slide_menu_fix_index(widget_t* widget, int32_t index);

END_C_DECLS

#endif /*TK_SLIDE_MENU_H*/
