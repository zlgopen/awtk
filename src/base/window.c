/**
 * File:   window.h
 * Author: AWTK Develop Team
 * Brief:  window
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
#include "base/window.h"
#include "base/prop_names.h"
#include "base/window_manager.h"

static ret_t window_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t window_get_prop(widget_t* widget, const char* name, value_t* v) {
  window_t* window = WINDOW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (str_fast_equal(name, WIDGET_PROP_ANIM_HINT)) {
    value_set_str(v, window->anim_hint.str);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t window_set_prop(widget_t* widget, const char* name, const value_t* v) {
  window_t* window = WINDOW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (str_fast_equal(name, WIDGET_PROP_ANIM_HINT)) {
    str_from_value(&(window->anim_hint), v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t window_destroy(widget_t* widget) {
  window_t* win = WINDOW(widget);

  str_reset(&(win->anim_hint));

  return RET_OK;
}

static const widget_vtable_t s_window_vtable = {.on_paint_self = window_on_paint_self,
                                                .set_prop = window_set_prop,
                                                .get_prop = window_get_prop,
                                                .destroy = window_destroy};

widget_t* window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  window_t* win = TKMEM_ZALLOC(window_t);
  return_value_if_fail(win != NULL, NULL);

  widget = WIDGET(win);
  widget_init(widget, NULL, WIDGET_NORMAL_WINDOW);
  widget->vt = &s_window_vtable;

  if (parent == NULL) {
    parent = window_manager();
  }

  str_init(&(win->anim_hint), 0);
  widget_move_resize(widget, x, y, w, h);
  return_value_if_fail(window_manager_add_child(parent, widget) == RET_OK, NULL);

  return widget;
}

ret_t window_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_remove_child(widget->parent, widget);
}
