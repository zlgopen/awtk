/**
 * File:   tab_control.h
 * Author: AWTK Develop Team
 * Brief:  tab_control
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

#ifndef TK_TAB_CONTROL_H
#define TK_TAB_CONTROL_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class tab_control_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 标签控件。
 */
typedef struct _tab_control_t {
  widget_t widget;
} tab_control_t;

/**
 * @method tab_control_create
 * 创建tab_control对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* tab_control_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

END_C_DECLS

#endif /*TK_TAB_CONTROL_H*/
