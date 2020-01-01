/**
 * File:   app_bar.h
 * Author: AWTK Develop Team
 * Brief:  app_bar
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_APP_BAR_H
#define TK_APP_BAR_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class app_bar_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * app_bar控件。
 *
 * 一个简单的容器控件，一般在窗口的顶部，用于显示本窗口的状态和信息。
 *
 * 它本身不提供布局功能，仅提供具有语义的标签，让xml更具有可读性。
 * 子控件的布局可用layout\_children属性指定。
 * 请参考[布局参数](https://github.com/zlgopen/awtk/blob/master/docs/layout.md)。
 *
 * app\_bar\_t是[widget\_t](widget_t.md)的子类控件，widget\_t的函数均适用于app\_bar\_t控件。
 *
 * 在xml中使用"app\_bar"标签创建app\_bar。如：
 *
 * ```xml
 * <app_bar x="0" y="0" w="100%" h="30" >
 *   <label x="0" y="0" w="100%" h="100%" text="Basic Controls" />
 * </app_bar>
 * ```
 *
 * 在c代码中使用函数app\_bar\_create创建app\_bar。如：
 *
 * ```c
 *  widget_t* app_bar = app_bar_create(win, 0, 0, 320, 30);
 * ```
 *
 * 可用通过style来设置控件的显示风格，如背景颜色等。如：
 *
 * ```xml
 * <style name="default" border_color="#a0a0a0">
 *   <normal     bg_color="#f0f0f0" />
 * </style>
 * ```
 *
 */
typedef struct _app_bar_t {
  widget_t widget;
} app_bar_t;

/**
 * @method app_bar_create
 * 创建app_bar对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* app_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method app_bar_cast
 * 转换为app_bar对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget app_bar对象。
 *
 * @return {widget_t*} app_bar对象。
 */
widget_t* app_bar_cast(widget_t* widget);

#define APP_BAR(w) ((app_bar_t*)(app_bar_cast(w))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(app_bar);

END_C_DECLS

#endif /*TK_APP_BAR_H*/
