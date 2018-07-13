/**
 * File:   spin_box.h
 * Author: AWTK Develop Team
 * Brief:  spin_box
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
 * 2018-06-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/button.h"
#include "base/layout.h"
#include "base/spin_box.h"

extern const char* s_edit_properties[];

static const widget_vtable_t s_edit_vtable = {.size = sizeof(edit_t),
                                              .type_name = WIDGET_TYPE_SPIN_BOX,
                                              .properties = s_edit_properties,
                                              .create = spin_box_create,
                                              .on_paint_self = edit_on_paint_self,
                                              .set_prop = edit_set_prop,
                                              .get_prop = edit_get_prop,
                                              .on_event = edit_on_event};

widget_t* spin_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* inc = NULL;
  widget_t* dec = NULL;
  widget_t* spin_box = edit_create_ex(parent, x, y, w, h, &s_edit_vtable);
  edit_t* edit = EDIT(spin_box);
  return_value_if_fail(spin_box != NULL, NULL);

  edit->right_margin = 21;
  inc = button_create(spin_box, 0, 0, 0, 0);
  button_set_repeat(inc, 300);
  widget_set_name(inc, "inc");
  widget_use_style(inc, "3:spinbox_up");
  widget_set_self_layout_params(inc, "right", "0", "20", "50%");

  dec = button_create(spin_box, 0, 0, 0, 0);
  button_set_repeat(dec, 300);
  widget_set_name(dec, "dec");
  widget_use_style(dec, "2:spinbox_down");
  widget_set_self_layout_params(dec, "right", "bottom", "20", "50%");

  return spin_box;
}
