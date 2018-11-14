/**
 * File:   view.h
 * Author: AWTK Develop Team
 * Brief:  view
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
 * 2018-04-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/view.h"

static const widget_vtable_t s_view_vtable = {
    .size = sizeof(view_t), .type = WIDGET_TYPE_VIEW, .create = view_create};

widget_t* view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  view_t* view = TKMEM_ZALLOC(view_t);
  widget_t* widget = WIDGET(view);
  return_value_if_fail(view != NULL, NULL);

  return widget_init(widget, parent, &s_view_vtable, x, y, w, h);
}

widget_t* view_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_view_vtable, NULL);

  return widget;
}
