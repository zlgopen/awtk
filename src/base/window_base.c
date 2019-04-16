/**
 * File:   window_base.h
 * Author: AWTK Develop Team
 * Brief:  window_base
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "base/window_base.h"
#include "base/font_manager.h"
#include "base/image_manager.h"
#include "base/assets_manager.h"
#include "base/window_manager.h"

ret_t window_close(widget_t* widget);

ret_t window_base_on_paint_self(widget_t* widget, canvas_t* c) {
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
  assets_manager_t* am = widget_get_assets_manager(widget);

  const char* theme_name = widget->name;

  if (window_base->theme != NULL) {
    theme_name = window_base->theme;
  }

  if (theme_name != NULL) {
    window_base->res_theme = assets_manager_ref(am, ASSET_TYPE_STYLE, theme_name);
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
  } else if (tk_str_eq(name, WIDGET_PROP_LOCALE_INFO)) {
    value_set_pointer(v, (void*)(locale_info()));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FONT_MANAGER)) {
    value_set_pointer(v, (void*)(font_manager()));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ASSETS_MANAGER)) {
    value_set_pointer(v, (void*)(assets_manager()));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STAGE)) {
    value_set_int(v, window_base->stage);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSABLE)) {
    value_set_int(v, window_base->closable);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t window_base_set_prop(widget_t* widget, const char* name, const value_t* v) {
  window_base_t* window_base = WINDOW_BASE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    window_base->open_anim_hint = tk_str_copy(window_base->open_anim_hint, value_str(v));
    window_base->close_anim_hint = tk_str_copy(window_base->close_anim_hint, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPEN_ANIM_HINT)) {
    window_base->open_anim_hint = tk_str_copy(window_base->open_anim_hint, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_ANIM_HINT)) {
    window_base->close_anim_hint = tk_str_copy(window_base->close_anim_hint, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME)) {
    window_base->theme = tk_str_copy(window_base->theme, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSABLE)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = window_closable_type_find(value_str(v));
      if (kv != NULL) {
        window_base->closable = (window_closable_t)(kv->value);
      }
    } else {
      window_base->closable = (window_closable_t)value_int(v);
    }
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t window_base_on_destroy(widget_t* widget) {
  window_base_t* window_base = WINDOW_BASE(widget);
  return_value_if_fail(widget != NULL && window_base != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(window_base->theme);
  TKMEM_FREE(window_base->open_anim_hint);
  TKMEM_FREE(window_base->close_anim_hint);

  if (window_base->theme_obj != NULL) {
    theme_destroy(window_base->theme_obj);
  }

  if (window_base->res_theme != NULL) {
    assets_manager_t* am = widget_get_assets_manager(widget);
    assets_manager_unref(am, window_base->res_theme);
  }

  return RET_OK;
}

ret_t window_base_on_event(widget_t* widget, event_t* e) {
  window_base_t* win = WINDOW_BASE(widget);
  return_value_if_fail(widget != NULL && win != NULL, RET_BAD_PARAMS);

  if (e->type == EVT_WINDOW_WILL_OPEN) {
    win->stage = WINDOW_STAGE_CREATED;
    window_base_load_theme_obj(widget);
    widget_update_style_recursive(widget);
  } else if (e->type == EVT_WINDOW_OPEN) {
    win->stage = WINDOW_STAGE_OPENED;
  } else if (e->type == EVT_WINDOW_CLOSE) {
    win->stage = WINDOW_STAGE_CLOSED;
  } else if (e->type == EVT_REQUEST_CLOSE_WINDOW) {
    log_debug("EVT_REQUEST_CLOSE_WINDOW\n");
    if (win->closable == WINDOW_CLOSABLE_YES) {
      window_close(widget);
    }
  }

  return RET_OK;
}

widget_t* window_base_create(widget_t* parent, const widget_vtable_t* vt, xy_t x, xy_t y, wh_t w,
                             wh_t h) {
  widget_t* widget = widget_create(NULL, vt, x, y, w, h);
  window_base_t* win = WINDOW_BASE(widget);

  return_value_if_fail(win != NULL, NULL);

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

ret_t window_close_force(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_close_window_force(widget->parent, widget);
}

ret_t window_close(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_close_window(widget->parent, widget);
}

TK_DECL_VTABLE(window_base) = {.size = sizeof(window_base_t), .parent = TK_PARENT_VTABLE(widget)};

widget_t* window_base_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, window_base), NULL);

  return widget;
}
