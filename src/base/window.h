/**
 * File:   window.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  window
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

#ifndef LFTK_WINDOW_H
#define LFTK_WINDOW_H

#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _window_t {
  widget_t widget;
}window_t;

widget_t* window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

END_C_DECLS

#endif/*LFTK_WINDOW_H*/

