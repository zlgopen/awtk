﻿/**
 * File:   spin_box.h
 * Author: AWTK Develop Team
 * Brief:  spin_box
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/widget_vtable.h"

#define SPIN_DEFAULT_REPEAT 300

const char* const s_spin_box_properties[] = {TK_EDIT_PROPS, WIDGET_PROP_REPEAT,
                                             WIDGET_PROP_EASY_TOUCH_MODE,
                                             WIDGET_PROP_BUTTON_POSITION, NULL};

widget_t* spin_box_create_self(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

static ret_t spin_box_create_children(widget_t* widget) {
  widget_t* inc = NULL;
  widget_t* dec = NULL;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  inc = button_create(widget, 0, 0, 0, 0);
  inc->auto_created = TRUE;
  button_set_repeat(inc, SPIN_DEFAULT_REPEAT);
  widget_set_name(inc, STR_EDIT_INC_NAME);

  dec = button_create(widget, 0, 0, 0, 0);
  dec->auto_created = TRUE;
  button_set_repeat(dec, SPIN_DEFAULT_REPEAT);
  widget_set_name(dec, STR_EDIT_DEC_NAME);

  widget_set_need_relayout_children(widget);

  return RET_OK;
}

static ret_t spin_box_on_copy(widget_t* widget, widget_t* other) {
  spin_box_t* spin_box = SPIN_BOX(widget);
  spin_box_t* spin_box_other = SPIN_BOX(other);
  return_value_if_fail(spin_box != NULL && spin_box_other != NULL, RET_BAD_PARAMS);

  spin_box->easy_touch_mode = spin_box_other->easy_touch_mode;
  spin_box->button_position =
      tk_str_copy(spin_box->button_position, spin_box_other->button_position);

  edit_on_copy(widget, other);
  return spin_box_create_children(widget);
}

ret_t spin_set_repeat(widget_t* widget, int32_t repeat) {
  widget_t* inc = NULL;
  widget_t* dec = NULL;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  inc = widget_lookup(widget, STR_EDIT_INC_NAME, FALSE);
  dec = widget_lookup(widget, STR_EDIT_DEC_NAME, FALSE);

  if (inc != NULL) {
    button_set_repeat(inc, repeat);
  }

  if (dec != NULL) {
    button_set_repeat(dec, repeat);
  }

  return RET_OK;
}

static ret_t spin_box_set_prop(widget_t* widget, const char* name, const value_t* v) {
  if (tk_str_eq(name, WIDGET_PROP_EASY_TOUCH_MODE)) {
    spin_box_set_easy_touch_mode(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BUTTON_POSITION)) {
    return spin_box_set_button_position(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_REPEAT)) {
    return spin_set_repeat(widget, value_int32(v));
  }

  return edit_set_prop(widget, name, v);
}

static ret_t spin_box_get_prop(widget_t* widget, const char* name, value_t* v) {
  if (tk_str_eq(name, WIDGET_PROP_EASY_TOUCH_MODE)) {
    value_set_bool(v, SPIN_BOX(widget)->easy_touch_mode);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BUTTON_POSITION)) {
    value_set_str(v, SPIN_BOX(widget)->button_position);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_REPEAT)) {
    widget_t* inc = widget_lookup(widget, STR_EDIT_INC_NAME, FALSE);
    if (inc != NULL) {
      return widget_get_prop(inc, WIDGET_PROP_REPEAT, v);
    } else {
      value_set_int32(v, SPIN_DEFAULT_REPEAT);
      return RET_OK;
    }
  }

  return edit_get_prop(widget, name, v);
}

ret_t spin_box_on_destroy(widget_t* widget) {
  spin_box_t* spin_box = SPIN_BOX(widget);
  return_value_if_fail(widget != NULL && spin_box != NULL, RET_BAD_PARAMS);

  edit_on_destroy(widget);
  TKMEM_FREE(spin_box->button_position);

  return RET_OK;
}

static ret_t spin_box_on_layout_children(widget_t* widget) {
  edit_t* edit = EDIT(widget);
  spin_box_t* spin_box = SPIN_BOX(widget);
  widget_t* inc = widget_child(widget, STR_EDIT_INC_NAME);
  widget_t* dec = widget_child(widget, STR_EDIT_DEC_NAME);
  return_value_if_fail(inc != NULL && dec != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(spin_box->button_position, SPIN_BOX_BUTTON_POSITION_DEFAULT) ||
      (tk_str_eq(spin_box->button_position, SPIN_BOX_BUTTON_POSITION_NONE) &&
       !(spin_box->easy_touch_mode))) {
    int32_t size = widget->h / 2;
    int32_t x = widget->w - size;
    edit->right_margin = size + 1;
    widget_move_resize(inc, x, 0, size, size);
    widget_move_resize(dec, x, size, size, size);
    widget_use_style(inc, "spinbox_up");
    widget_use_style(dec, "spinbox_down");
  } else {
    int32_t font_size = style_get_int(widget->astyle, STYLE_ID_FONT_SIZE, 18);
    if (tk_str_eq(spin_box->button_position, SPIN_BOX_BUTTON_POSITION_TOP_BOTTOM) ||
        (tk_str_eq(spin_box->button_position, SPIN_BOX_BUTTON_POSITION_NONE) &&
         widget->h > font_size * 3)) {
      int32_t h = widget->h / 3;
      edit->top_margin = h;
      edit->bottom_margin = h;
      widget_move_resize(inc, 0, 0, widget->w, h);
      widget_move_resize(dec, 0, widget->h - h, widget->w, h);
      widget_use_style(inc, "spinbox_top");
      widget_use_style(dec, "spinbox_bottom");
    } else {
      int32_t size = widget->h;
      edit->left_margin = size;
      edit->right_margin = size;
      widget_move_resize(dec, 0, 0, size, size);
      widget_move_resize(inc, widget->w - size, 0, size, size);
      widget_use_style(dec, "spinbox_left");
      widget_use_style(inc, "spinbox_right");
    }
  }
  return RET_OK;
}

static ret_t spin_box_init(widget_t* widget) {
  ret_t ret = RET_OK;
  edit_t* edit = NULL;
  return_value_if_fail(SPIN_BOX(widget) != NULL, RET_BAD_PARAMS);

  ret = widget_vtable_init_by_parent(widget, WIDGET_VTABLE_GET_VTABLE(spin_box));
  return_value_if_fail(ret == RET_OK || ret == RET_NOT_IMPL, ret);

  edit = EDIT(widget);
  return_value_if_fail(edit != NULL, RET_FAIL);

  edit->step = 1;
  edit->input_type = INPUT_INT;

  return RET_OK;
}

TK_DECL_VTABLE(spin_box) = {.size = sizeof(spin_box_t),
                            .type = WIDGET_TYPE_SPIN_BOX,
                            .inputable = TRUE,
                            .focusable = TRUE,
                            .pointer_cursor = WIDGET_CURSOR_EDIT,
                            .clone_properties = s_spin_box_properties,
                            .persistent_properties = s_spin_box_properties,
                            .get_parent_vt = TK_GET_PARENT_VTABLE(edit),
                            .create = spin_box_create_self,
                            .init = spin_box_init,
                            .on_paint_self = edit_on_paint_self,
                            .set_prop = spin_box_set_prop,
                            .get_prop = spin_box_get_prop,
                            .on_layout_children = spin_box_on_layout_children,
                            .on_destroy = spin_box_on_destroy,
                            .on_copy = spin_box_on_copy,
                            .on_event = edit_on_event,
                            .on_add_child = edit_on_add_child};

widget_t* spin_box_create_self(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* spin_box = widget_create(parent, TK_REF_VTABLE(spin_box), x, y, w, h);
  return_value_if_fail(spin_box != NULL, NULL);

  spin_box_init(spin_box);

  return spin_box;
}

widget_t* spin_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* spin_box = spin_box_create_self(parent, x, y, w, h);
  return_value_if_fail(spin_box != NULL, NULL);

  spin_box_set_button_position(spin_box, SPIN_BOX_BUTTON_POSITION_NONE);
  spin_box_create_children(spin_box);

  return spin_box;
}

ret_t spin_box_set_easy_touch_mode(widget_t* widget, bool_t easy_touch_mode) {
  spin_box_t* spin_box = SPIN_BOX(widget);
  return_value_if_fail(spin_box != NULL, RET_BAD_PARAMS);

  spin_box->easy_touch_mode = easy_touch_mode;

  return widget_invalidate_force(widget, NULL);
}

ret_t spin_box_set_button_position(widget_t* widget, const char* button_position) {
  spin_box_t* spin_box = SPIN_BOX(widget);
  return_value_if_fail(spin_box != NULL, RET_BAD_PARAMS);

  spin_box->button_position = tk_str_copy(spin_box->button_position, button_position);

  return widget_invalidate_force(widget, NULL);
}

widget_t* spin_box_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, spin_box), NULL);

  return widget;
}
