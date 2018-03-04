/**
 * File:   dialog.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  dialog
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#ifndef LFTK_DIALOG_H
#define LFTK_DIALOG_H

#include "base/wstr.h"
#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class dialog_t 
 * @parent widget_t
 * @scriptable
 * 对话框控件。
 */
typedef struct _dialog_t {
  widget_t widget;
  bitmap_t icon;
  wstr_t title;
  widget_t* client;
  uint32_t quit_code;
}dialog_t;

/**
 * @method dialog_create
 * @constructor
 * 创建dialog对象
 * @param {widget_t*} parent 父控件
 * @param {xy_t} x x坐标
 * @param {xy_t} y y坐标
 * @param {wh_t} w 宽度
 * @param {wh_t} h 高度
 *
 * @return {widget_t*} 对象。
 */
widget_t* dialog_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method dialog_set_icon
 * 设置对话框的标题图标。
 * @param {widget_t*} widget dialog对象。
 * @param {char*}  name 图标名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_set_icon(widget_t* widget, const char* name);

/**
 * @method dialog_set_title
 * 设置对话框的标题文本。
 * @param {widget_t*} widget dialog对象。
 * @param {wchar_t*}  title 标题。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_set_title(widget_t* widget, const wchar_t* title);

/**
 * @method dialog_modal
 * 模态显示对话框。
 * @param {widget_t*} widget dialog对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
uint32_t dialog_modal(widget_t* widget);

/**
 * @method dialog_quit
 * 退出模态显示。
 * @param {widget_t*} widget dialog对象。
 * @param {uint32_t} code 退出码，作为dialog_modal的返回值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_quit(widget_t* widget, uint32_t code);

#define DIALOG(widget) ((dialog_t*)(widget))

END_C_DECLS

#endif/*LFTK_DIALOG_H*/

