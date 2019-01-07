/**
 * File:   list_view.h
 * Author: AWTK Develop Team
 * Brief:  list_view
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
 * 2018-07-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LIST_VIEW_H
#define TK_LIST_VIEW_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class list_view_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * ListView控件。
 */
typedef struct _list_view_t {
  widget_t widget;
  /**
   * @property {int32_t} item_height
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 列表项的高度。如果 item_height > 0，所有列表项使用固定高度，否则使用列表项自身的高度。
   */
  int32_t item_height;
  /**
   * @property {int32_t} default_item_height
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 列表项的缺省高度。如果item_height <= 0 而且列表项自身的高度 <= 0，则使用缺省高度。
   */
  int32_t default_item_height;
  /**
   * @property {bool_t} auto_hide_scroll_bar
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 如果不需要滚动条时，自动隐藏滚动条。
   */
  bool_t auto_hide_scroll_bar;

  /*private*/
  widget_t* scroll_view;
  widget_t* scroll_bar;
} list_view_t;

/**
 * @method list_view_create
 * 创建list_view对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* list_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method list_view_set_item_height
 * 设置列表项的高度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} item_height 列表项的高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_view_set_item_height(widget_t* widget, int32_t item_height);

/**
 * @method list_view_set_default_item_height
 * 设置列表项的缺省高度。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {int32_t} default_item_height 列表项的高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_view_set_default_item_height(widget_t* widget, int32_t default_item_height);

/**
 * @method list_view_set_auto_hide_scroll_bar
 * 设置是否自动隐藏滚动条。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget 控件对象。
 * @param {bool_t} auto_hide_scroll_bar 列表项的高度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t list_view_set_auto_hide_scroll_bar(widget_t* widget, bool_t auto_hide_scroll_bar);

/**
 * @method list_view_cast
 * 转换为list_view对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget list_view对象。
 *
 * @return {widget_t*} list_view对象。
 */
widget_t* list_view_cast(widget_t* widget);

#define LIST_VIEW(widget) ((list_view_t*)(widget))

END_C_DECLS

#endif /*TK_LIST_VIEW_H*/
