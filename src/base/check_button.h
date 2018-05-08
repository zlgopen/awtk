/**
 * File:   check_button.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  check_button
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
 * 2018-02-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CHECK_BUTTON_H
#define TK_CHECK_BUTTON_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class check_button_t
 * @parent widget_t
 * @scriptable
 * 勾选控件。
 */
typedef struct _check_button_t {
  widget_t widget;
  /**
   * @property {bool_t} value
   * @readonly
   * 值。
   */
  bool_t value;
  bool_t radio;
} check_button_t;

/**
 * @method check_button_create
 * @constructor
 * 创建check_button对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* check_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method check_button_create_radio
 * @constructor
 * 创建check_button对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* check_button_create_radio(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method check_button_set_value
 * 设置控件的值。
 * @param {widget_t*} widget check_button对象。
 * @param {uint32_t}  value 值
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t check_button_set_value(widget_t* widget, bool_t value);

#define CHECK_BUTTON(widget) ((check_button_t*)(widget))

END_C_DECLS

#endif /*TK_CHECK_BUTTON_H*/
