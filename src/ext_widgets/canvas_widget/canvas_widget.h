/**
 * File:   canvas_widget.h
 * Author: AWTK Develop Team
 * Brief:  canvas_widget
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
 * 2018-12-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CANVAS_WIDGET_H
#define TK_CANVAS_WIDGET_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class canvas_widget_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 画布控件。
 */
typedef struct _canvas_widget_t {
  widget_t widget;
} canvas_widget_t;

/**
 * @method canvas_widget_create
 * 创建canvas_widget对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* canvas_widget_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method canvas_widget_cast
 * 转换为canvas_widget对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget canvas_widget对象。
 *
 * @return {widget_t*} canvas_widget对象。
 */
widget_t* canvas_widget_cast(widget_t* widget);

#define WIDGET_TYPE_CANVAS_WIDGET "canvas"

#define CANVAS_WIDGET(widget) ((canvas_widget_t*)(widget))

END_C_DECLS

#endif /*TK_CANVAS_WIDGET_H*/
