/**
 * File:   tab_button.h
 * Author: AWTK Develop Team
 * Brief:  tab_button
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

#ifndef TK_TAB_BUTTON_H
#define TK_TAB_BUTTON_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class tab_button_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 标签按钮控件。
 */
typedef struct _tab_button_t {
  widget_t widget;
  /**
   * @property {bool_t} value
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 值。
   */
  bool_t value;
  /**
   * @property {char*}active_icon
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 当前项的图标的名称。
   */
  char* active_icon;
  /**
   * @property {char*} icon
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 非当前项的图标的名称。
   */
  char* icon;
} tab_button_t;

/**
 * @method tab_button_create
 * 创建tab_button对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* tab_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method tab_button_cast
 * 转换tab_button对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget tab_button对象。
 *
 * @return {widget_t*} tab_button对象。
 */
widget_t* tab_button_cast(widget_t* widget);

/**
 * @method tab_button_set_value
 * 设置控件的值。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget tab_button对象。
 * @param {uint32_t}  value 值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tab_button_set_value(widget_t* widget, bool_t value);

/**
 * @method tab_button_set_icon
 * 设置控件的图标。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget tab_button对象。
 * @param {char*}  name 当前项的图标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tab_button_set_icon(widget_t* widget, const char* name);

/**
 * @method tab_button_set_active_icon
 * 设置控件的active图标。
 * @annotation ["scriptable"]
 * @param {widget_t*} widget tab_button对象。
 * @param {char*}  name 当前项的图标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tab_button_set_active_icon(widget_t* widget, const char* name);

#define TAB_BUTTON(widget) ((tab_button_t*)(widget))

END_C_DECLS

#endif /*TK_TAB_BUTTON_H*/
