/**
 * File:   button.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  button
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_BUTTON_H
#define LFTK_BUTTON_H

#include "base/wstr.h"
#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _button_t {
  widget_t widget;
  wstr_t text;
}button_t;

widget_t* button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

ret_t button_set_text(widget_t* widget, const wchar_t* text);

#define BUTTON(widget) ((button_t*)(widget))

END_C_DECLS

#endif/*LFTK_BUTTON_H*/

