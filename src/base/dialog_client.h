/**
 * File:   dialog_client.h
 * Author: AWTK Develop Team
 * Brief:  dialog client
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

#ifndef TK_DIALOG_CLIENT_H
#define TK_DIALOG_CLIENT_H

#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class dialog_client_t
 * @parent widget_t
 * @annotation ["scriptable"]
 * 对话框客户区控件。
 */
typedef struct _dialog_client_t {
  widget_t widget;
} dialog_client_t;

/**
 * @method dialog_client_create
 * 创建dialog对象。
 * @annotation ["constructor", "scriptable"]
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} dialog对象。
 */
widget_t* dialog_client_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method dialog_client_cast
 * 转换为dialog_client对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 * @param {widget_t*} widget dialog_client对象。
 *
 * @return {widget_t*} dialog_client对象。
 */
widget_t* dialog_client_cast(widget_t* widget);

#define DIALOG_CLIENT(widget) ((dialog_client_t*)(widget))

END_C_DECLS

#endif /*TK_DIALOG_CLIENT_H*/
