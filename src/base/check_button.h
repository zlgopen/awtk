/**
 * File:   check_button.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  check_button
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
 * 2018-02-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_CHECK_BUTTON_H
#define LFTK_CHECK_BUTTON_H

#include "base/wstr.h"
#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _check_button_t {
  widget_t widget;
  bool_t value;
  bool_t radio;
  wstr_t text;
}check_button_t;

widget_t* check_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

widget_t* radio_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

ret_t check_button_set_text(widget_t* widget, const wchar_t* text);
ret_t check_button_set_value(widget_t* widget, bool_t value);

#define CHECK_BUTTON(widget) ((check_button_t*)(widget))

END_C_DECLS

#endif/*LFTK_CHECK_BUTTON_H*/

