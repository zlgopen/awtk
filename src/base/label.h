/**
 * File:   label.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  label
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
 * 2018-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LABEL_H
#define TK_LABEL_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class label_t
 * @parent widget_t
 * @scriptable
 * 文本控件。
 */
typedef struct _label_t {
  widget_t widget;
} label_t;

/**
 * @method label_create
 * @constructor
 * 创建label对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* label_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define LABEL(widget) ((label_t*)(widget))

END_C_DECLS

#endif /*TK_LABEL_H*/
