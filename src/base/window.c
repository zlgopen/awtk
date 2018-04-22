/**
 * File:   window.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  window
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
#include "base/window.h"
#include "base/prop_names.h"
#include "base/window_manager.h"

static ret_t window_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t window_get_prop(widget_t* widget, const char* name, value_t* v) {
  window_t* window = WINDOW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (str_equal(name, WIDGET_PROP_ANIM_HINT)) {
    value_set_int(v, window->anim_hint);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t window_set_prop(widget_t* widget, const char* name, const value_t* v) {
  window_t* window = WINDOW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (str_equal(name, WIDGET_PROP_ANIM_HINT)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = window_animator_type_find(value_str(v));
      window->anim_hint = kv != NULL ? kv->value : WINDOW_ANIMATOR_NONE;
    } else {
      window->anim_hint = value_int(v);
    }
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const widget_vtable_t s_window_vtable = {.on_paint_self = window_on_paint_self,
                                                .set_prop = window_set_prop,
                                                .get_prop = window_get_prop};

widget_t* window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  window_t* win = MEM_ZALLOC(window_t);
  return_value_if_fail(win != NULL, NULL);

  widget = WIDGETP(win);
  widget_init(widget, NULL, WIDGET_NORMAL_WINDOW);
  widget->vt = &s_window_vtable;

  if (parent == NULL) {
    parent = default_wm();
  }

  widget_move_resize(widget, x, y, w, h);
  return_value_if_fail(window_manager_add_child(parent, widget) == RET_OK, NULL);

  return widget;
}

ret_t window_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_remove_child(widget->parent, widget);
}
