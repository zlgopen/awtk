/**
 * File:   button_group.h
 * Author: AWTK Develop Team
 * Brief:  button_group
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/button_group.h"

static ret_t button_group_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static const widget_vtable_t s_button_group_vtable = {.size = sizeof(button_group_t),
                                                      .type = WIDGET_TYPE_BUTTON_GROUP,
                                                      .create = button_group_create,
                                                      .on_paint_self = button_group_on_paint_self};

widget_t* button_group_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  button_group_t* button_group = TKMEM_ZALLOC(button_group_t);
  widget_t* widget = WIDGET(button_group);
  return_value_if_fail(button_group != NULL, NULL);

  return widget_init(widget, parent, &s_button_group_vtable, x, y, w, h);
}

widget_t* button_group_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_button_group_vtable, NULL);

  return widget;
}
