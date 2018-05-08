/**
 * File:   group_box.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  group_box
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
 * 2018-02-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/group_box.h"
#include "base/mem.h"

static ret_t group_box_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static const widget_vtable_t s_group_box_vtable = {.on_paint_self = group_box_on_paint_self};

widget_t* group_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  group_box_t* group_box = TKMEM_ZALLOC(group_box_t);
  return_value_if_fail(group_box != NULL, NULL);

  widget = WIDGETP(group_box);
  widget_init(widget, parent, WIDGET_GROUP_BOX);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_group_box_vtable;

  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return widget;
}
