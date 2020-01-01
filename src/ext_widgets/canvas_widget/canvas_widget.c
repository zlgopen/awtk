/**
 * File:   canvas_widget.h
 * Author: AWTK Develop Team
 * Brief:  canvas_widget
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-12-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "canvas_widget/canvas_widget.h"

TK_DECL_VTABLE(canvas_widget) = {.size = sizeof(canvas_widget_t),
                                 .type = WIDGET_TYPE_CANVAS_WIDGET,
                                 .parent = TK_PARENT_VTABLE(widget),
                                 .create = canvas_widget_create};

widget_t* canvas_widget_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(canvas_widget), x, y, w, h);
}

widget_t* canvas_widget_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, canvas_widget), NULL);

  return widget;
}
