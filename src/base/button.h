/**
 * File:   button.h
 * Author: AWTK Develop Team
 * Brief:  button
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BUTTON_H
#define TK_BUTTON_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class button_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 按钮控件。
 */
typedef struct _button_t {
  widget_t widget;
  /**
   * @property {int32_t} repeat
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 重复释放EVT_CLICK事件的时间间隔。为0则不重复释放EVT_CLICK事件。
   */
  int32_t repeat;

  /*private*/
  int32_t timer_id;
  int32_t repeat_nr;
  bool_t pressed;
  bool_t point_down_aborted;
} button_t;

/**
 * @method button_create
 * 创建button对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method button_cast
 * 转换为button对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget button对象。
 *
 * @return {widget_t*} button对象。
 */
widget_t* button_cast(widget_t* widget);

/**
 * @method button_set_repeat
 * 设置释放EVT_CLICK事件的时间间隔。为0则不重复释放EVT_CLICK事件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  repeat 释放EVT_CLICK事件的时间间隔(毫秒)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t button_set_repeat(widget_t* widget, int32_t repeat);

#define BUTTON(widget) ((button_t*)(widget))

END_C_DECLS

#endif /*TK_BUTTON_H*/
