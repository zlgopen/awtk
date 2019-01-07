/**
 * File:   spin_box.h
 * Author: AWTK Develop Team
 * Brief:  spin_box
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
 * 2018-06-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "widgets/button.h"
#include "base/layout.h"
#include "widgets/spin_box.h"

extern const char* s_edit_properties[];

widget_t* spin_box_create_self(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

static const widget_vtable_t s_spin_box_vtable = {.size = sizeof(edit_t),
                                                  .type = WIDGET_TYPE_SPIN_BOX,
                                                  .clone_properties = s_edit_properties,
                                                  .persistent_properties = s_edit_properties,
                                                  .create = spin_box_create_self,
                                                  .on_paint_self = edit_on_paint_self,
                                                  .set_prop = edit_set_prop,
                                                  .get_prop = edit_get_prop,
                                                  .on_event = edit_on_event};

widget_t* spin_box_create_self(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* spin_box = edit_create_ex(parent, x, y, w, h, &s_spin_box_vtable);
  edit_t* edit = EDIT(spin_box);
  return_value_if_fail(spin_box != NULL, NULL);

  edit->right_margin = 21;

  return spin_box;
}

widget_t* spin_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* inc = NULL;
  widget_t* dec = NULL;
  widget_t* spin_box = spin_box_create_self(parent, x, y, w, h);
  return_value_if_fail(spin_box != NULL, NULL);

  inc = button_create(spin_box, 0, 0, 0, 0);
  inc->auto_created = TRUE;
  button_set_repeat(inc, 300);
  widget_set_name(inc, "inc");
  widget_use_style(inc, "spinbox_up");
  widget_set_self_layout_params(inc, "right", "0", "20", "50%");

  dec = button_create(spin_box, 0, 0, 0, 0);
  dec->auto_created = TRUE;
  button_set_repeat(dec, 300);
  widget_set_name(dec, "dec");
  widget_use_style(dec, "spinbox_down");
  widget_set_self_layout_params(dec, "right", "bottom", "20", "50%");

  return spin_box;
}

widget_t* spin_box_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_spin_box_vtable, NULL);

  return widget;
}
