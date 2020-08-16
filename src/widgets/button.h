/**
 * File:   button.h
 * Author: AWTK Develop Team
 * Brief:  button
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
 * @annotation ["scriptable","design","widget"]
 * 按钮控件。
 *
 * 点击按钮之后会触发EVT\_CLICK事件，注册EVT\_CLICK事件以执行特定操作。
 *
 * 按钮控件也可以作为容器使用，使用图片和文本作为其子控件，可以实现很多有趣的效果。
 *
 * button\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于button\_t控件。
 *
 * 在xml中使用"button"标签创建按钮控件。如：
 *
 * ```xml
 * <button x="c" y="m" w="80" h="30" text="OK"/>
 * ```
 *
 * > 更多用法请参考：
 * [button.xml](https://github.com/zlgopen/awtk/blob/master/design/default/ui/button.xml)
 *
 * 在c代码中使用函数button\_create创建按钮控件。如：
 *
 * ```c
 *  widget_t* button = button_create(win, 10, 10, 128, 30);
 *  widget_set_text(button, L"OK");
 *  widget_on(button, EVT_CLICK, on_click, NULL);
 * ```
 *
 * > 创建之后，需要用widget\_set\_text或widget\_set\_text\_utf8设置文本内容。
 *
 * > 完整示例请参考：
 * [button demo](https://github.com/zlgopen/awtk-c-demos/blob/master/demos/button.c)
 *
 * 可用通过style来设置控件的显示风格，如字体的大小和颜色等等。如：
 *
 * ```xml
 *  <style name="default" border_color="#a0a0a0"  text_color="black">
 *   <normal     bg_color="#f0f0f0" />
 *   <pressed    bg_color="#c0c0c0" x_offset="1" y_offset="1"/>
 *   <over       bg_color="#e0e0e0" />
 *   <disable    bg_color="gray" text_color="#d0d0d0" />
 * </style>
 * ```
 *
 * > 更多用法请参考：
 * [theme
 * default](https://github.com/zlgopen/awtk/blob/master/design/default/styles/default.xml#L31)
 *
 */
typedef struct _button_t {
  widget_t widget;
  /**
   * @property {int32_t} repeat
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 重复触发EVT\_CLICK事件的时间间隔。
   *
   * 为0则不重复触发EVT\_CLICK事件。
   */
  int32_t repeat;

  /**
   * @property {bool_t} enable_long_press
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否启用长按事件，为true时才触发长按事件。
   *
   * 触发长按事件后不再触发点击事件。
   * 缺省不启用。
   *
   */
  bool_t enable_long_press;

  /*private*/
  int32_t timer_id;
  int32_t repeat_nr;
  bool_t pressed;
} button_t;

/**
 * @event {pointer_event_t} EVT_CLICK
 * 点击事件。
 */

/**
 * @event {pointer_event_t} EVT_LONG_PRESS
 * 长按事件。
 */

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
 * 设置触发EVT\_CLICK事件的时间间隔。为0则不重复触发EVT\_CLICK事件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t}  repeat 触发EVT_CLICK事件的时间间隔(毫秒)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t button_set_repeat(widget_t* widget, int32_t repeat);

/**
 * @method button_set_enable_long_press
 * 设置是否启用长按事件。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t}  enable_long_press 是否启用长按事件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t button_set_enable_long_press(widget_t* widget, bool_t enable_long_press);

#define BUTTON(widget) ((button_t*)(button_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(button);

END_C_DECLS

#endif /*TK_BUTTON_H*/
