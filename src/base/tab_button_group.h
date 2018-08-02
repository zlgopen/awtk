/**
 * File:   tab_button_group.h
 * Author: AWTK Develop Team
 * Brief:  tab_button_group
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

#ifndef TK_TAB_BUTTON_GROUP_H
#define TK_TAB_BUTTON_GROUP_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class tab_button_group_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * Tab Button Group控件。
 */
typedef struct _tab_button_group_t {
  widget_t widget;

  /**
   * @property {bool_t} compact
   * @annotation ["set_prop","get_prop","readable","persitent","design","scriptable"]
   * 紧凑型排版子控件。
   */
  bool_t compact;
} tab_button_group_t;

/**
 * @method tab_button_group_create
 * 创建tab_button_group对象
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* tab_button_group_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define TAB_BUTTON_GROUP(widget) ((tab_button_group_t*)(widget))

END_C_DECLS

#endif /*TK_TAB_BUTTON_GROUP_H*/
