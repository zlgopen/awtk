/**
 * File:   spin_box.h
 * Author: AWTK Develop Team
 * Brief:  spin_box
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
 * 2018-06-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SPIN_BOX_H
#define TK_SPIN_BOX_H

#include "base/edit.h"

BEGIN_C_DECLS

/**
 * @class spin_box_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * spinbox控件。
 */
typedef edit_t spin_box_t;

/**
 * @method spin_box_create
 * @annotation ["constructor"]
 * 创建spin_box对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* spin_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define SPIN_BOX(widget) ((spin_box_t*)(widget))

END_C_DECLS

#endif /*TK_SPIN_BOX_H*/
