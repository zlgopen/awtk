/**
 * File:   label.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  label
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
 * 2018-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_LABEL_H
#define LFTK_LABEL_H

#include "base/wstr.h"
#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _label_t {
  widget_t widget;
  wstr_t text;
  uint8_t border;
  uint8_t text_align_v;
  uint8_t text_align_h;
}label_t;

widget_t* label_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

ret_t label_set_text(widget_t* widget, const wchar_t* text);
ret_t label_set_border(widget_t* widget, uint8_t border);
ret_t label_set_text_align_h(widget_t* widget, uint8_t text_align_h);
ret_t label_set_text_align_v(widget_t* widget, uint8_t text_align_v);

#define LABEL(widget) ((label_t*)(widget))

END_C_DECLS

#endif/*LFTK_LABEL_H*/

