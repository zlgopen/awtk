/**
 * File:   list_view_h.h
 * Author: AWTK Develop Team
 * Brief:  list_view_h
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
 * 2018-07-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LIST_VIEW_H_H
#define TK_LIST_VIEW_H_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class list_view_h_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 水平ListView控件。
 */
typedef struct _list_view_h_t {
  widget_t widget;
  /**
   * @property {int32_t} item_width
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 列表项的宽度。
   */
  int32_t item_width;
  /**
   * @property {int32_t} spacing
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 间距。
   */
  int32_t spacing;

  /*private*/
  widget_t* scroll_view;
} list_view_h_t;

/**
 * @method list_view_h_create
 * 创建list_view_h对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* list_view_h_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method list_view_h_set_item_width
 * 设置列表项的宽度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} item_width 列表项的宽度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_view_h_set_item_width(widget_t* widget, int32_t item_width);

/**
 * @method list_view_h_set_spacing
 * 设置列表项的间距。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} spacing 列表项的间距。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_view_h_set_spacing(widget_t* widget, bool_t spacing);

/**
 * @method list_view_h_cast
 * 转换为list_view_h对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget list_view_h对象。
 *
 * @return {widget_t*} list_view_h对象。
 */
widget_t* list_view_h_cast(widget_t* widget);

#define LIST_VIEW_H(widget) ((list_view_h_t*)(widget))

END_C_DECLS

#endif /*TK_LIST_VIEW_H_H*/
