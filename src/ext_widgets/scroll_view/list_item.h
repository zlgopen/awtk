/**
 * File:   list_item.h
 * Author: AWTK Develop Team
 * Brief:  list_item
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
 * 2018-07-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LIST_ITEM_H
#define TK_LIST_ITEM_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class list_item_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * list item控件。
 */
typedef struct _list_item_t {
  widget_t widget;

  /*private*/
  int32_t timer_id;
  bool_t dragged;
  point_t down;
} list_item_t;

/**
 * @method list_item_create
 * @annotation ["constructor", "scriptable"]
 * 创建list_item对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* list_item_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method list_item_cast
 * 转换为list_item对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget list_item对象。
 *
 * @return {widget_t*} list_item对象。
 */
widget_t* list_item_cast(widget_t* widget);

#define LIST_ITEM(widget) ((list_item_t*)(widget))

END_C_DECLS

#endif /*TK_LIST_ITEM_H*/
