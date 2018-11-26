/**
 * File:   grid_item.h
 * Author: AWTK Develop Team
 * Brief:  grid_item
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
 * 2018-11-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_GRID_ITEM_H
#define TK_GRID_ITEM_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class grid_item_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * grid_item。一个简单的容器控件，一般作为grid的子控件。
 */
typedef struct _grid_item_t {
  widget_t widget;
} grid_item_t;

/**
 * @method grid_item_create
 * 创建grid_item对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* grid_item_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method grid_item_cast
 * 转换为grid_item对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget grid_item对象。
 *
 * @return {widget_t*} grid_item对象。
 */
widget_t* grid_item_cast(widget_t* widget);

END_C_DECLS

#endif /*TK_GRID_ITEM_H*/
