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
#include "base/window_manager.h"

static ret_t window_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t window_get_prop(widget_t* widget, const char* name, value_t* v) {
  window_t* window = WINDOW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    value_set_str(v, window->anim_hint);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME)) {
    value_set_str(v, window->theme);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t window_set_prop(widget_t* widget, const char* name, const value_t* v) {
  window_t* window = WINDOW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    TKMEM_FREE(window->anim_hint);
    window->anim_hint = tk_strdup(value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME)) {
    TKMEM_FREE(window->theme);
    window->theme = tk_strdup(value_str(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t window_destroy(widget_t* widget) {
  window_t* window = WINDOW(widget);

  TKMEM_FREE(window->theme);
  TKMEM_FREE(window->anim_hint);

  return RET_OK;
}

static const widget_vtable_t s_window_vtable = {.type = WIDGET_TYPE_NORMAL_WINDOW,
                                                .on_paint_self = window_on_paint_self,
                                                .set_prop = window_set_prop,
                                                .get_prop = window_get_prop,
                                                .destroy = window_destroy};

widget_t* window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  window_t* win = TKMEM_ZALLOC(window_t);
  widget_t* widget = WIDGET(win);
  return_value_if_fail(win != NULL, NULL);

  widget_init(widget, NULL, &s_window_vtable, x, y, w, h);
  if (parent == NULL) {
    parent = window_manager();
  }

  return_value_if_fail(window_manager_open_window(parent, widget) == RET_OK, NULL);
  widget_update_style(widget);

#ifdef ENABLE_MEM_LEAK_CHECK
  tk_mem_dump();
#endif /*ENABLE_MEM_LEAK_CHECK*/

  return widget;
}

ret_t window_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_close_window(widget->parent, widget);
}

widget_t* window_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_window_vtable, NULL);

  return widget;
}
