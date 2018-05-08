/**
 * File:   button.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  button
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
 * @scriptable
 * 按钮控件。
 */
typedef struct _button_t {
  widget_t widget;
} button_t;

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

#define BUTTON(widget) ((button_t*)(widget))

END_C_DECLS

#endif /*TK_BUTTON_H*/
