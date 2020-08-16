/**
 * File:   check_button.h
 * Author: AWTK Develop Team
 * Brief:  check_button
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

END_C_DECLS

#endif /*TK_CHECK_BUTTON_H*/
