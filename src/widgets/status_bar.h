/**
 * File:   status_bar.h
 * Author: qianfan Zhao <qianfanguijin@163.com>
 * Brief:  status_bar
 *
 * Copyright (c) 2018 - 2019  qianfan Zhao <qianfanguijin@163.com>
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
 * 2019-10-30 qianfan Zhao <qianfanguijin@163.com> created
 *
 */

#ifndef TK_status_bar_H
#define TK_status_bar_H

#include "base/window_base.h"

BEGIN_C_DECLS

/**
 * @class status_bar_t
 * @parent window_base_t
 * @annotation ["scriptable","design","widget","window"]
 * status\_bar窗口。
 *
 * status\_bar窗口是一种特殊的窗口，类似于system\_bar，不同的是，它独占LCD底部区域，
 * 用来显示额外的信息。
 *
 *> status\_bar窗口需要在打开第一个应用程序窗口之前打开。
 *
 * status\_bar对两个子控件会做特殊处理：
 *
 * * 1.名为"title"的label控件，自动显示当前主窗口的name或text。
 * * 2.名为"close"的button控件，点击之后向当前主窗口发送EVT\_REQUEST\_CLOSE\_WINDOW消息。
 *
 *status\_bar\_t是[window\_base\_t](window_base_t.md)的子类控件，
 * window\_base\_t的函数均适用于status\_bar\_t控件。
 *
 * 在xml中使用"status\_bar"标签创建status\_bar窗口。如：
 *
 * ```xml
 * <status_bar h="30">
 * <column x="0" y="0" w="-40" h="100%">
 *   <label style="title" x="10" y="m" w="55%" h="100%" name="title"/>
 *   <digit_clock style="time" x="r" y="m" w="40%" h="100%" format="hh:mm"/>
 * </column>
 * <button style="close" x="r:5" y="m" w="26" h="26" name="close" text="x"/>
 * </status_bar>
 * ```
 *
 * > 更多用法请参考：
 * [status_bar](https://github.com/zlgopen/awtk/blob/master/demos/assets/default/raw/ui/status_bar.xml)
 *
 * 在c代码中使用函数status\_bar\_create创建status\_bar窗口。如：
 *
 * ```c
 * widget_t* win = status_bar_create(NULL, 0, 0, 320, 30);
 * ```
 *
 * > 创建之后，和使用普通窗口是一样的。
 *
 * 可用通过style来设置控件的显示风格，如背景颜色等等。如：
 *
 * ```xml
 * <status_bar>
 *  <style name="default">
 *    <normal bg_color="#a0a0a0"/>
 *  </style>
 * </status_bar>
 * ```
 *
 *> 更多用法请参考：
 * [status_bar.xml](https://github.com/zlgopen/awtk/blob/master/demos/assets/default/raw/styles/status_bar.xml)
 *
 */
typedef struct _status_bar_t {
  window_base_t window;
} status_bar_t;

/**
 * @method status_bar_create
 * @annotation ["constructor", "scriptable"]
 * 创建status_bar对象。
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} status_bar对象。
 */
widget_t* status_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method status_bar_cast
 * 转换为status_bar对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget status_bar对象。
 *
 * @return {widget_t*} status_bar对象。
 */
widget_t* status_bar_cast(widget_t* widget);

#define STATUS_BAR(widget) ((status_bar_t*)(status_bar_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(status_bar);

END_C_DECLS

#endif /*TK_status_bar_H*/
