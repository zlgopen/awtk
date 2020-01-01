/**
 * File:   combo_box_item.h
 * Author: AWTK Develop Team
 * Brief:  combo_box_item
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_COMBO_BOX_ITEM_H
#define TK_COMBO_BOX_ITEM_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class combo_box_item_t
 * @parent widget_t
 * @annotation ["scriptable","design","widget"]
 * ComboBox Item控件。
 *
 * 本类仅供combo\_box控件内部使用。
 *
 */
typedef struct _combo_box_item_t {
  widget_t widget;

  /**
   * @property {int32_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值。
   */
  int32_t value;
  /**
   * @property {bool_t} checked
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 是否被选中。
   */
  bool_t checked;

  /*private*/
  bool_t pressed;
} combo_box_item_t;

/**
 * @event {event_t} EVT_VALUE_WILL_CHANGE
 * 值(勾选状态)即将改变事件。
 */

/**
 * @event {event_t} EVT_VALUE_CHANGED
 * 值(勾选状态)改变事件。
 */

/**
 * @method combo_box_item_create
 * 创建combo_box_item对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* combo_box_item_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method combo_box_item_cast
 * 转换combo_box_item对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget combo_box_item对象。
 *
 * @return {widget_t*} combo_box_item对象。
 */
widget_t* combo_box_item_cast(widget_t* widget);

/**
 * @method combo_box_item_set_checked
 * 设置控件是否被选中。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box_item对象。
 * @param {bool_t}  checked 是否被选中。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_item_set_checked(widget_t* widget, bool_t checked);

/**
 * @method combo_box_item_set_value
 * 设置控件的值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget combo_box_item对象。
 * @param {int32_t}  value 值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t combo_box_item_set_value(widget_t* widget, int32_t value);

#define COMBO_BOX_ITEM(widget) ((combo_box_item_t*)(combo_box_item_cast(WIDGET(widget))))

/*public for subclass and runtime type check*/
TK_EXTERN_VTABLE(combo_box_item);

END_C_DECLS

#endif /*TK_COMBO_BOX_ITEM_H*/
