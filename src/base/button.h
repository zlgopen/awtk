/**
 * File:   button.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  button
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_BUTTON_H
#define LFTK_BUTTON_H

#include "base/wstr.h"
#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class button_t 
 * @parent widget_t
 * @scriptable
 * 按钮控件。
 */
typedef struct _button_t {
  widget_t widget;
  wstr_t text;
}button_t;

/**
 * @method button_create
 * @constructor
 * 创建button对象
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
 * @method button_set_text
 * 设置控件的文本。
 * @param {widget_t*} widget button对象。
 * @param {wchar_t*}  text 文本。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t button_set_text(widget_t* widget, const wchar_t* text);

#define BUTTON(widget) ((button_t*)(widget))

END_C_DECLS

#endif/*LFTK_BUTTON_H*/

