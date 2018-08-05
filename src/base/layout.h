/**
 * File:   layout.h
 * Author: AWTK Develop Team
 * Brief:  layout
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
 * 2018-03-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_LAYOUT_H
#define TK_LAYOUT_H

#include "base/rect.h"
#include "base/widget.h"
#include "base/layout_def.h"

BEGIN_C_DECLS

ret_t widget_layout_calc(const widget_layout_t* layout, rect_t* r, wh_t parent_w, wh_t parent_h);
widget_layout_t* widget_layout_parse(widget_layout_t* layout, const char* x, const char* y,
                                     const char* w, const char* h);

children_layout_t* children_layout_parser(children_layout_t* layout, const char* params);

ret_t widget_set_self_layout(widget_t* widget, const widget_layout_t* layout);

ret_t widget_set_children_layout(widget_t* widget, const children_layout_t* cl);

ret_t widget_layout_self(widget_t* widget);
ret_t widget_layout_children_default(widget_t* widget);

END_C_DECLS

#endif /*TK_LAYOUT_H*/
