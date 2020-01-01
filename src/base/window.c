/**
 * File:   window.h
 * Author: AWTK Develop Team
 * Brief:  window
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "base/window.h"
#include "base/window_manager.h"

static const char* s_window_properties[] = {WIDGET_PROP_ANIM_HINT,
                                            WIDGET_PROP_OPEN_ANIM_HINT,
                                            WIDGET_PROP_FULLSCREEN,
                                            WIDGET_PROP_CLOSE_ANIM_HINT,
                                            WIDGET_PROP_MOVE_FOCUS_PREV_KEY,
                                            WIDGET_PROP_MOVE_FOCUS_NEXT_KEY,
                                            WIDGET_PROP_THEME,
                                            NULL};

static ret_t window_set_prop(widget_t* widget, const char* name, const value_t* v) {
  if (tk_str_eq(name, WIDGET_PROP_FULLSCREEN)) {
    window_set_fullscreen(widget, value_bool(v));

    return RET_OK;
  }

  return window_base_set_prop(widget, name, v);
}

static ret_t window_get_prop(widget_t* widget, const char* name, value_t* v) {
  window_t* window = WINDOW(widget);
  return_value_if_fail(window != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_FULLSCREEN)) {
    value_set_bool(v, window->fullscreen);

    return RET_OK;
  }

  return window_base_get_prop(widget, name, v);
}

ret_t window_set_fullscreen(widget_t* widget, bool_t fullscreen) {
  window_t* window = WINDOW(widget);
  return_value_if_fail(window != NULL, RET_BAD_PARAMS);

  if (window->fullscreen != fullscreen) {
    window->fullscreen = fullscreen;
    widget_layout_children(widget->parent);
    widget_invalidate_force(widget, NULL);
  }

  return RET_OK;
}

TK_DECL_VTABLE(window) = {.type = WIDGET_TYPE_NORMAL_WINDOW,
                          .size = sizeof(window_t),
                          .is_window = TRUE,
                          .parent = TK_PARENT_VTABLE(window_base),
                          .create = window_create,
                          .clone_properties = s_window_properties,
                          .persistent_properties = s_window_properties,
                          .on_event = window_base_on_event,
                          .on_paint_self = window_base_on_paint_self,
                          .on_paint_begin = window_base_on_paint_begin,
                          .on_paint_end = window_base_on_paint_end,
                          .invalidate = window_base_invalidate,
                          .set_prop = window_set_prop,
                          .get_prop = window_get_prop,
                          .on_destroy = window_base_on_destroy};

widget_t* window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return window_base_create(parent, TK_REF_VTABLE(window), x, y, w, h);
}

widget_t* window_create_default(void) {
  return window_create(NULL, 0, 0, 0, 0);
}

widget_t* window_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, window), NULL);

  return widget;
}
