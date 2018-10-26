/**
 * File:   window_base.h
 * Author: AWTK Develop Team
 * Brief:  window_base
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
#include "base/window_base.h"
#include "base/window_manager.h"

ret_t window_base_on_paint_self(widget_t* widget, canvas_t* c) {
  if (widget->style_data.data != NULL) {
    return widget_paint_helper(widget, c, NULL, NULL);
  }

  return RET_OK;
}

ret_t window_base_get_prop(widget_t* widget, const char* name, value_t* v) {
  window_base_t* window_base = WINDOW_BASE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    value_set_str(v, window_base->open_anim_hint);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPEN_ANIM_HINT)) {
    value_set_str(v, window_base->open_anim_hint);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_ANIM_HINT)) {
    value_set_str(v, window_base->close_anim_hint);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME)) {
    value_set_str(v, window_base->theme);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCRIPT)) {
    value_set_str(v, window_base->script);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t window_base_set_prop(widget_t* widget, const char* name, const value_t* v) {
  window_base_t* window_base = WINDOW_BASE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    TKMEM_FREE(window_base->open_anim_hint);
    window_base->open_anim_hint = tk_strdup(value_str(v));
    TKMEM_FREE(window_base->close_anim_hint);
    window_base->close_anim_hint = tk_strdup(value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPEN_ANIM_HINT)) {
    TKMEM_FREE(window_base->open_anim_hint);
    window_base->open_anim_hint = tk_strdup(value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_ANIM_HINT)) {
    TKMEM_FREE(window_base->close_anim_hint);
    window_base->close_anim_hint = tk_strdup(value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME)) {
    TKMEM_FREE(window_base->theme);
    window_base->theme = tk_strdup(value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCRIPT)) {
    TKMEM_FREE(window_base->script);
    window_base->script = tk_strdup(value_str(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t window_base_destroy(widget_t* widget) {
  window_base_t* window_base = WINDOW_BASE(widget);

  TKMEM_FREE(window_base->script);
  TKMEM_FREE(window_base->theme);
  TKMEM_FREE(window_base->open_anim_hint);
  TKMEM_FREE(window_base->close_anim_hint);

  return RET_OK;
}

ret_t window_base_on_event(widget_t* widget, event_t* e) {
  window_base_t* win = WINDOW_BASE(widget);
  if (e->type == EVT_WINDOW_OPEN) {
    win->stage = WINDOW_STAGE_OPENED;
  } else if (e->type == EVT_WINDOW_CLOSE) {
    win->stage = WINDOW_STAGE_CLOSED;
  }

  return RET_OK;
}

widget_t* window_base_init(widget_t* widget, widget_t* parent, const widget_vtable_t* vt, xy_t x,
                           xy_t y, wh_t w, wh_t h) {
  window_base_t* win = WINDOW_BASE(widget);

  return_value_if_fail(win != NULL, NULL);

  widget_init(widget, NULL, vt, x, y, w, h);
  if (parent == NULL) {
    parent = window_manager();
  }

  return_value_if_fail(window_manager_open_window(parent, widget) == RET_OK, NULL);
  widget_update_style(widget);

  win->stage = WINDOW_STAGE_CREATED;
#ifdef ENABLE_MEM_LEAK_CHECK
  tk_mem_dump();
#endif /*ENABLE_MEM_LEAK_CHECK*/

  return widget;
}

ret_t window_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_close_window(widget->parent, widget);
}
