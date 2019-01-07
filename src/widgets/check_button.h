/**
 * File:   check_button.h
 * Author: AWTK Develop Team
 * Brief:  check_button
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
 * 2018-02-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CHECK_BUTTON_H
#define TK_CHECK_BUTTON_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class check_button_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 勾选控件。
 */
typedef struct _check_button_t {
  widget_t widget;
  /**
   * @property {bool_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值。
   */
  bool_t value;

  /*private*/
  bool_t radio;
  bool_t pressed;
  bool_t point_down_aborted;
} check_button_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值(勾选状态)即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值(勾选状态)改变事件。
 */

/**
 * @method check_button_create
 * 创建check_button对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* check_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method check_button_create_radio
 * 创建check_button对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* check_button_create_radio(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method check_button_set_value
 * 设置控件的值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget check_button对象。
 * @param {bool_t}  value 值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t check_button_set_value(widget_t* widget, bool_t value);

/**
 * @method check_button_cast
 * 转换check_button对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget check_button对象。
 *
 * @return {widget_t*} check_button对象。
 */
widget_t* check_button_cast(widget_t* widget);

#define CHECK_BUTTON(widget) ((check_button_t*)(widget))

END_C_DECLS

#endif /*TK_CHECK_BUTTON_H*/
