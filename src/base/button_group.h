/**
 * File:   button_group.h
 * Author: AWTK Develop Team
 * Brief:  button_group
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BUTTON_GROUP_H
#define TK_BUTTON_GROUP_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class button_group_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * Button Group控件。
 */
typedef struct _button_group_t {
  widget_t widget;
} button_group_t;

/**
 * @method button_group_create
 * 创建button_group对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* button_group_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define BUTTON_GROUP(widget) ((button_group_t*)(widget))

END_C_DECLS

#endif /*TK_BUTTON_GROUP_H*/
