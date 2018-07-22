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
 * @scriptable
 * 标签按钮控件。
 */
typedef struct _tab_button_t {
  widget_t widget;
  /**
   * @property {bool_t} value
   * @readonly
   * 值。
   */
  bool_t value;

  /**
   * @property {char*} active_icon
   * @fake
   * 当前项的图标的名称(只能通过widget_get_prop/set_prop访问)。
   */

  /**
   * @property {char*} deactive_icon
   * @fake
   * 非当前项的图标的名称(只能通过widget_get_prop/set_prop访问)。
   */
  /*private*/
  bitmap_t active_icon;
  bitmap_t deactive_icon;
} tab_button_t;

/**
 * @method tab_button_create
 * @constructor
 * 创建tab_button对象
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
 * @method tab_button_set_value
 * 设置控件的值。
 * @param {widget_t*} widget tab_button对象。
 * @param {uint32_t}  value 值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tab_button_set_value(widget_t* widget, bool_t value);

/**
 * @method tab_button_set_icons
 * 设置控件的图标。
 * @param {widget_t*} widget tab_button对象。
 * @param {char*}  active 当前项的图标。
 * @param {char*}  deactive 非当前项的图标。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tab_button_set_icons(widget_t* widget, const char* active, const char* deactive);

#define TAB_BUTTON(widget) ((tab_button_t*)(widget))

END_C_DECLS

#endif /*TK_TAB_BUTTON_H*/
