/**
 * File:   progress_bar.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  progress_bar
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

#ifndef LFTK_PROGRESS_BAR_H
#define LFTK_PROGRESS_BAR_H

#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _progress_bar_t {
  widget_t widget;
  uint8_t value; /*0-100*/
  bool_t vertical;
  bool_t show_text;
}progress_bar_t;

widget_t* progress_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

ret_t progress_bar_set_value(widget_t* widget, uint8_t value);
ret_t progress_bar_set_vertical(widget_t* widget, bool_t vertical);
ret_t progress_bar_set_show_text(widget_t* widget, bool_t show_text);

#define PROGRESS_BAR(widget) ((progress_bar_t*)(widget))

END_C_DECLS

#endif/*LFTK_PROGRESS_BAR_H*/

