/**
 * File:   keyboard.h
 * Author: AWTK Develop Team
 * Brief:  keyboard
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/enums.h"
#include "base/keyboard.h"
#include "base/prop_names.h"
#include "base/window_manager.h"

static ret_t keyboard_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static const widget_vtable_t s_keyboard_vtable = {.on_paint_self = keyboard_on_paint_self};

widget_t* keyboard_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  keyboard_t* win = TKMEM_ZALLOC(keyboard_t);
  return_value_if_fail(win != NULL, NULL);

  widget = WIDGET(win);
  widget_init(widget, NULL, WIDGET_KEYBOARD);
  widget->vt = &s_keyboard_vtable;

  if (parent == NULL) {
    parent = window_manager();
  }

  widget_move_resize(widget, x, y, w, h);
  return_value_if_fail(window_manager_add_child(parent, widget) == RET_OK, NULL);

  return widget;
}

ret_t keyboard_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_remove_child(widget->parent, widget);
}
