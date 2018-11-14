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
#include "base/font_manager.h"
#include "base/image_manager.h"
#include "base/assets_manager.h"
#include "base/window_manager.h"

ret_t window_base_on_paint_self(widget_t* widget, canvas_t* c) {
  if (style_is_valid(widget->astyle)) {
    return widget_paint_helper(widget, c, NULL, NULL);
  }

  return RET_OK;
}

ret_t window_base_on_paint_begin(widget_t* widget, canvas_t* c) {
  (void)widget;
  (void)c;
  return RET_OK;
}

ret_t window_base_on_paint_end(widget_t* widget, canvas_t* c) {
  (void)widget;
  (void)c;
  return RET_OK;
}

static ret_t window_base_load_theme_obj(widget_t* widget) {
  window_base_t* window_base = WINDOW_BASE(widget);

  const char* theme_name = widget->name;

  if (window_base->theme != NULL) {
    theme_name = window_base->theme;
  }

  if (theme_name != NULL) {
    window_base->res_theme = assets_manager_ref(assets_manager(), ASSET_TYPE_STYLE, theme_name);
  }

  if (window_base->res_theme != NULL) {
    window_base->theme_obj = theme_create(window_base->res_theme->data);
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
  } else if (tk_str_eq(name, WIDGET_PROP_THEME_OBJ)) {
    value_set_pointer(v, (void*)(window_base->theme_obj));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DEFAULT_THEME_OBJ)) {
    value_set_pointer(v, (void*)(theme()));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE_MANAGER)) {
    value_set_pointer(v, (void*)(image_manager()));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FONT_MANAGER)) {
    value_set_pointer(v, (void*)(font_manager()));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ASSETS_MANAGER)) {
    value_set_pointer(v, (void*)(assets_manager()));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCRIPT)) {
    value_set_str(v, window_base->script);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STAGE)) {
    value_set_int(v, window_base->stage);
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

  if (window_base->theme_obj != NULL) {
    theme_destroy(window_base->theme_obj);
  }

  if (window_base->res_theme != NULL) {
    assets_manager_unref(assets_manager(), window_base->res_theme);
  }

  return RET_OK;
}

ret_t window_base_on_event(widget_t* widget, event_t* e) {
  window_base_t* win = WINDOW_BASE(widget);

  if (e->type == EVT_WINDOW_WILL_OPEN) {
    win->stage = WINDOW_STAGE_CREATED;
    window_base_load_theme_obj(widget);
    widget_update_style_recursive(widget);
  } else if (e->type == EVT_WINDOW_OPEN) {
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

  widget->is_window = TRUE;
  widget_init(widget, NULL, vt, x, y, w, h);
  if (parent == NULL) {
    parent = window_manager();
  }

  return_value_if_fail(window_manager_open_window(parent, widget) == RET_OK, NULL);

  win->stage = WINDOW_STAGE_NONE;
#ifdef ENABLE_MEM_LEAK_CHECK
  tk_mem_dump();
#endif /*ENABLE_MEM_LEAK_CHECK*/

  return widget;
}

ret_t window_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_close_window(widget->parent, widget);
}
