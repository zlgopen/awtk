/**
 * File:   app_bar.h
 * Author: AWTK Develop Team
 * Brief:  app_bar
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
 * @annotation ["scriptable"]
 * app_bar。一个简单的容器控件，一般在窗口的顶部。
 * 它本身不提供布局功能(可用layout参数指定)，和view功能一致，仅提供具有语义的标签，让xml更具有可读性。
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

END_C_DECLS

#endif /*TK_APP_BAR_H*/
