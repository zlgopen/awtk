/**
 * File:   color_picker.h
 * Author: AWTK Develop Team
 * Brief:  color_picker
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
 * 2018-08-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COLOR_PICKER_H
#define TK_COLOR_PICKER_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class color_picker_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 颜色选择器。
 */
typedef struct _color_picker_t {
  widget_t widget;

  color_t color;
} color_picker_t;

/**
 * @method color_picker_create
 * 创建color_picker对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* color_picker_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method color_picker_set_color
 * 设置进度条的进度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {uint8_t}  value 进度
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t color_picker_set_color(widget_t* widget, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

/**
 * @method color_picker_cast
 * 转换为color_picker对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget color_picker对象。
 *
 * @return {widget_t*} color_picker对象。
 */
widget_t* color_picker_cast(widget_t* widget);

#define COLOR_PICKER(widget) ((color_picker_t*)(widget))

END_C_DECLS

#endif /*TK_COLOR_PICKER_H*/
