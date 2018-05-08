/**
 * File:   group_box.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  group_box
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
 * 2018-02-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GROUP_BOX_H
#define TK_GROUP_BOX_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class group_box_t
 * @parent widget_t
 * @scriptable
 * 分组控件。
 */
typedef struct _group_box_t {
  widget_t widget;
} group_box_t;

/**
 * @method group_box_create
 * @constructor
 * 创建group_box对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* group_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

END_C_DECLS

#endif /*TK_GROUP_BOX_H*/
