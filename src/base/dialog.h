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

typedef struct _dialog_t {
  widget_t widget;
  bitmap_t icon;
  wstr_t title;
  widget_t* client;
  uint32_t quit_code;
}dialog_t;

widget_t* dialog_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

ret_t dialog_set_icon(widget_t* widget, const char* name);
ret_t dialog_set_title(widget_t* widget, const wchar_t* title);

uint32_t dialog_modal(widget_t* widget);
ret_t dialog_quit(widget_t* widget, uint32_t code);

#define DIALOG(widget) ((dialog_t*)(widget))

END_C_DECLS

#endif/*LFTK_DIALOG_H*/

