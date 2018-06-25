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
    .type_name = WIDGET_TYPE_VIEW
};

widget_t* view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  view_t* view = TKMEM_ZALLOC(view_t);
  return_value_if_fail(view != NULL, NULL);

  widget = WIDGET(view);
  widget_init(widget, parent, WIDGET_VIEW);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_view_vtable;

  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return widget;
}
