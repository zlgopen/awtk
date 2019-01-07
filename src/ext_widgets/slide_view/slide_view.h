/**
 * File:   slide_view.h
 * Author: AWTK Develop Team
 * Brief:  slide_view
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
 * @annotation ["scriptable"]
 * slide view。
 */
typedef struct _slide_view_t {
  widget_t widget;
  /**
   * @property {bool_t} vertical
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否为上下滑动模式。
   */
  bool_t vertical;

  /**
   * @property {uint16_t} auto_play
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 自动播放。0表示禁止自动播放，非0表示自动播放时每一页播放的时间。
   */
  uint16_t auto_play;

  /*private*/
  point_t down;
  int32_t xoffset;
  int32_t yoffset;
  uint32_t active;
  bool_t animating;
  uint32_t timer_id;
  bool_t dragged;
  velocity_t velocity;
} slide_view_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值(当前页)即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值(当前页)改变事件。
 */

/**
 * @method slide_view_create
 * 创建slide_view对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* slide_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method slide_view_cast
 * 转换为slide_view对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget slide_view对象。
 *
 * @return {widget_t*} slide_view对象。
 */
widget_t* slide_view_cast(widget_t* widget);

/**
 * @method slide_view_set_auto_play
 * 设置为自动播放模式。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_view对象。
 * @param {uint16_t} auto_play 0表示禁止自动播放，非0表示自动播放时每一页播放的时间。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_view_set_auto_play(widget_t* widget, uint16_t auto_play);

/**
 * @method slide_view_set_active
 * 设置当前页的序号。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_view对象。
 * @param {uint32_t} index 当前页的序号。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_view_set_active(widget_t* widget, uint32_t index);

/**
 * @method slide_view_set_vertical
 * 设置为上下滑动(缺省为左右滑动)。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget slide_view对象。
 * @param {bool_t} vertical TRUE表示上下滑动，FALSE表示左右滑动。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t slide_view_set_vertical(widget_t* widget, bool_t vertical);

/*public for test only*/
widget_t* slide_view_get_prev(slide_view_t* slide_view);
widget_t* slide_view_get_next(slide_view_t* slide_view);
ret_t slide_view_activate_prev(slide_view_t* slide_view);
ret_t slide_view_activate_next(slide_view_t* slide_view);

#define SLIDE_VIEW(widget) ((slide_view_t*)(widget))

END_C_DECLS

#endif /*TK_SLIDE_VIEW_H*/
