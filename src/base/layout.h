/**
 * File:   layout.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  layout
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
 * 2018-03-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_LAYOUT_H
#define LFTK_LAYOUT_H

#include "base/rect.h"
#include "base/widget.h"
#include "base/layout_def.h"

BEGIN_C_DECLS

ret_t widget_layout_calc(const widget_layout_t* layout, rect_t* r, wh_t parent_w, wh_t parent_h);
widget_layout_t* widget_layout_parse(widget_layout_t* layout, const char* x, const char* y, const char* w, const char* h);
children_layout_t* children_layout_parser(children_layout_t* layout, const char* params);

ret_t widget_set_parsed_self_layout_params(widget_t* widget, const widget_layout_t* layout);
ret_t widget_set_self_layout_params(widget_t* widget, const char* x, const char* y, const char* w, const char* h);
ret_t widget_set_children_layout_params(widget_t* widget, uint8_t rows, uint8_t cols, uint8_t margin, uint8_t cell_spacing);

ret_t widget_layout(widget_t* widget);
ret_t widget_layout_self(widget_t* widget);
ret_t widget_layout_children(widget_t* widget);

END_C_DECLS

#endif/*LFTK_LAYOUT_H*/

