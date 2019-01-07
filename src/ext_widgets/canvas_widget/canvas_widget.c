/**
 * File:   canvas_widget.h
 * Author: AWTK Develop Team
 * Brief:  canvas_widget
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static const widget_vtable_t s_canvas_widget_vtable = {.size = sizeof(canvas_widget_t),
                                                       .type = WIDGET_TYPE_CANVAS_WIDGET,
                                                       .create = canvas_widget_create};

widget_t* canvas_widget_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  canvas_widget_t* canvas_widget = TKMEM_ZALLOC(canvas_widget_t);
  widget_t* widget = WIDGET(canvas_widget);
  return_value_if_fail(canvas_widget != NULL, NULL);

  return widget_init(widget, parent, &s_canvas_widget_vtable, x, y, w, h);
}

widget_t* canvas_widget_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_canvas_widget_vtable, NULL);

  return widget;
}
