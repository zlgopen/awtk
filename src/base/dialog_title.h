/**
 * File:   dialog_title.h
 * Author: AWTK Develop Team
 * Brief:  dialog title
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DIALOG_TITLE_H
#define TK_DIALOG_TITLE_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class dialog_title_t
 * @parent widget_t
 * @scriptable
 * 对话框标题控件。
 */
typedef struct _dialog_title_t {
  widget_t widget;
} dialog_title_t;

/**
 * @method dialog_title_create
 * @constructor
 * 创建dialog对象。
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} dialog对象。
 */
widget_t* dialog_title_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

#define DIALOG_TITLE(widget) ((dialog_title_t*)(widget))

END_C_DECLS

#endif /*TK_DIALOG_TITLE_H*/
