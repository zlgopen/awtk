﻿/**
 * File:   check_button.h
 * Author: AWTK Develop Team
 * Brief:  check_button
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * @annotation ["scriptable","design","widget"]
 * 勾选按钮控件(单选/多选)。
 *
 * check\_button\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于check\_button\_t控件。
 *
 * 在xml中使用"check_button"标签创建多选按钮控件。如：
 *
 * ```xml
 * <check_button name="c1" text="Book"/>
 * ```
 *
 * 在xml中使用"radio_button"标签创建单选按钮控件。如：
 *
 * ```xml
 * <radio_button name="r1" text="Book"/>
 * ```
 *
 * > 更多用法请参考：
 * [button.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/basic.xml)
 *
 * 在c代码中使用函数check\_button\_create创建多选按钮控件。如：
 *
 * ```c
 *  widget_t* button = check_button_create(win, x, y, w, h);
 *  widget_set_text(button, L"OK");
 *  widget_on(button, EVT_VALUE_CHANGED, on_changed, NULL);
 * ```
 *
 * 在c代码中使用函数check\_button\_create\_radio创建单选按钮控件。如：
 *
 * ```c
 *  widget_t* button = check_button_create_radio(win, x, y, w, h);
 *  widget_set_text(button, L"OK");
 *  widget_on(button, EVT_VALUE_CHANGED, on_changed, NULL);
 * ```
 *
 * > 完整示例请参考：
 * [button demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/check_button.c)
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 * <style name="default" icon_at="left">
 *   <normal  icon="unchecked" />
 *   <pressed icon="unchecked" />
 *   <over    icon="unchecked" text_color="green"/>
 *   <normal_of_checked icon="checked" text_color="blue"/>
 *   <pressed_of_checked icon="checked" text_color="blue"/>
 *   <over_of_checked icon="checked" text_color="green"/>
 * </style>
 * ```
 *
 * > 更多用法请参考：
 * [theme
 * default](https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L227)
 *
 */
typedef struct _check_button_t {
  widget_t widget;
  /**
   * @property {bool_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值(勾选为TRUE，非勾选为FALSE)。
   */
  bool_t value;

  /**
   * @property {bool_t} indeterminate
   * @annotation ["set_prop","get_prop"]
   * 复选框是否是为不确定状态。（该值为TRUE的话，value 值存于不确定状态，该值为FALSE的话，value 值存于确定状态）
   */
  bool_t indeterminate;

  /**
   * @property {bool_t} radio
   * @annotation ["set_prop","get_prop"]
   * 是否是单选按钮。
   */
  bool_t radio;

  /*private*/
  bool_t pressed;
  bool_t point_down_aborted;
  const char* type;
} check_button_t;

/**
 * @event {value_change_event_t} EVT_VALUE_WILL_CHANGE
 * 值(勾选状态)即将改变事件。
 */

/**
 * @event {value_change_event_t} EVT_VALUE_CHANGED
 * 值(勾选状态)改变事件。
 */

/**
 * @event {pointer_event_t} EVT_CLICK
 * 点击事件。
 */

/**
 * @method check_button_create
 * 创建多选按钮对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} widget对象。
 */
widget_t* check_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method check_button_create_radio
 * 创建单选按钮对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} widget对象。
 */
widget_t* check_button_create_radio(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method check_button_set_value
 * 设置控件的值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget check_button对象。
 * @param {bool_t}  value 值(勾选为TRUE，非勾选为FALSE)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t check_button_set_value(widget_t* widget, bool_t value);

/**
 * @method check_button_get_checked_button
 * 用于radio button获取同组中勾选的radio button。
 *
 * @param {widget_t*} widget radio_button对象。
 *
 * @return {widget_t*} 同组中勾选的radio button对象。
 */
widget_t* check_button_get_checked_button(widget_t* widget);

/**
 * @method check_button_set_indeterminate
 * 设置控件的不确定状态。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget check_button对象。
 * @param {bool_t}  indeterminate 不确定状态。（该值为TRUE的话，value 值存于不确定状态，该值为FALSE的话，value 值存于确定状态）
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t check_button_set_indeterminate(widget_t* widget, bool_t indeterminate);

/**
 * @method check_button_get_indeterminate
 * 获取控件的是否存于不确定状态。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget check_button对象。
 *
 * @return {bool_t} 返回控件的是否存于不确定状态。
 */
bool_t check_button_get_indeterminate(widget_t* widget);

/**
 * @method check_button_cast
 * 转换check_button对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget check_button对象。
 *
 * @return {widget_t*} check_button对象。
 */
widget_t* check_button_cast(widget_t* widget);

#define CHECK_BUTTON(widget) ((check_button_t*)(check_button_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(check_button);

/**
 * @method check_button_create_ex
 * 创建check button对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 * @param {const char*} type 类型。
 * @param {bool_t} radio 是否单选。
 *
 * @return {widget_t*} widget对象。
 */
widget_t* check_button_create_ex(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h, const char* type,
                                 bool_t radio);

/**
 * @enum widget_state_t
 * @annotation ["string"]
 * @prefix WIDGET_STATE_
 * 控件的状态。
 */

/**
 * @const WIDGET_STATE_NORMAL_OF_INDETERMINATE
 * 正常状态(选项不确定)。
 */
#define WIDGET_STATE_NORMAL_OF_INDETERMINATE "normal_of_indeterminate"

/**
 * @const WIDGET_STATE_PRESSED_OF_INDETERMINATE
 * 指针按下状态(选项不确定)。
 */
#define WIDGET_STATE_PRESSED_OF_INDETERMINATE "pressed_of_indeterminate"

/**
 * @const WIDGET_STATE_OVER_OF_INDETERMINATE
 * 指针悬浮状态(选项不确定)。
 */
#define WIDGET_STATE_OVER_OF_INDETERMINATE "over_of_indeterminate"

/**
 * @const WIDGET_STATE_DISABLE_OF_INDETERMINATE
 * 禁用状态(选项不确定)。
 */
#define WIDGET_STATE_DISABLE_OF_INDETERMINATE "disable_of_indeterminate"

/**
 * @const WIDGET_STATE_FOCUSED_OF_INDETERMINATE
 * 焦点状态(选项不确定)。
 */
#define WIDGET_STATE_FOCUSED_OF_INDETERMINATE "focused_of_indeterminate"

#define CHECK_BUTTON_PROP_INDETERMINATE "indeterminate"

END_C_DECLS

#endif /*TK_CHECK_BUTTON_H*/
