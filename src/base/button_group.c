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
                                                      .type_name = WIDGET_TYPE_BUTTON_GROUP,
                                                      .create = button_group_create,
                                                      .on_paint_self = button_group_on_paint_self};

widget_t* button_group_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  button_group_t* button_group = TKMEM_ZALLOC(button_group_t);
  return_value_if_fail(button_group != NULL, NULL);

  widget = WIDGET(button_group);
  widget->vt = &s_button_group_vtable;
  widget_init(widget, parent, WIDGET_BUTTON_GROUP);
  widget_move_resize(widget, x, y, w, h);

  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return widget;
}
