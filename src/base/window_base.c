/**
 * File:   window_base.h
 * Author: AWTK Develop Team
 * Brief:  window_base
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
#include "base/keys.h"
#include "base/enums.h"
#include "base/window_base.h"
#include "base/native_window.h"
#include "base/font_manager.h"
#include "base/image_manager.h"
#include "base/assets_manager.h"
#include "base/window_manager.h"

ret_t window_close(widget_t* widget);

ret_t window_base_on_paint_self(widget_t* widget, canvas_t* c) {
  return RET_OK;
}

ret_t window_base_on_paint_begin(widget_t* widget, canvas_t* c) {
  assets_manager_t* am = widget_get_assets_manager(widget);
  font_manager_t* fm = widget_get_font_manager(widget);

  canvas_set_font_manager(c, fm);
  canvas_set_assets_manager(c, am);

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

  if (window_base->theme != NULL && window_base->theme[0] != 0) {
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

static ret_t window_base_unload_theme_obj(widget_t* widget) {
  window_base_t* window_base = WINDOW_BASE(widget);
  assets_manager_t* am = widget_get_assets_manager(widget);

  if (window_base->res_theme != NULL) {
    assets_manager_unref(am, window_base->res_theme);
    window_base->res_theme = NULL;
  }

  if (window_base->theme_obj != NULL) {
    theme_destroy(window_base->theme_obj);
    window_base->theme_obj = NULL;
  }

  return RET_OK;
}

static ret_t window_base_reload_theme_obj(widget_t* widget) {
  window_base_unload_theme_obj(widget);

  log_debug("window_base_reload_theme_obj\n");
  return window_base_load_theme_obj(widget);
}

ret_t window_base_get_prop(widget_t* widget, const char* name, value_t* v) {
  window_base_t* window_base = WINDOW_BASE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    if (window_base->disable_anim) {
      value_set_str(v, NULL);
    } else {
      value_set_str(v, window_base->open_anim_hint);
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_OPEN_ANIM_HINT)) {
    if (window_base->disable_anim) {
      value_set_str(v, NULL);
    } else {
      value_set_str(v, window_base->open_anim_hint);
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_ANIM_HINT)) {
    if (window_base->disable_anim) {
      value_set_str(v, NULL);
    } else {
      value_set_str(v, window_base->close_anim_hint);
    }
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DISABLE_ANIM)) {
    value_set_bool(v, window_base->disable_anim);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_NATIVE_WINDOW)) {
    value_set_pointer(v, window_base->native_window);
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
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_PREV_KEY)) {
    value_set_str(v, window_base->move_focus_prev_key);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_NEXT_KEY)) {
    value_set_str(v, window_base->move_focus_next_key);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_UP_KEY)) {
    value_set_str(v, window_base->move_focus_up_key);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_DOWN_KEY)) {
    value_set_str(v, window_base->move_focus_down_key);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_LEFT_KEY)) {
    value_set_str(v, window_base->move_focus_left_key);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_RIGHT_KEY)) {
    value_set_str(v, window_base->move_focus_right_key);
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
  } else if (tk_str_eq(name, WIDGET_PROP_DISABLE_ANIM)) {
    window_base->disable_anim = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_NATIVE_WINDOW)) {
    window_base->native_window = (native_window_t*)value_pointer(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_ANIM_HINT)) {
    window_base->close_anim_hint = tk_str_copy(window_base->close_anim_hint, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME)) {
    window_base->theme = tk_str_copy(window_base->theme, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_PREV_KEY)) {
    window_base->move_focus_prev_key = tk_str_copy(window_base->move_focus_prev_key, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_NEXT_KEY)) {
    window_base->move_focus_next_key = tk_str_copy(window_base->move_focus_next_key, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_UP_KEY)) {
    window_base->move_focus_up_key = tk_str_copy(window_base->move_focus_up_key, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_DOWN_KEY)) {
    window_base->move_focus_down_key = tk_str_copy(window_base->move_focus_down_key, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_LEFT_KEY)) {
    window_base->move_focus_left_key = tk_str_copy(window_base->move_focus_left_key, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MOVE_FOCUS_RIGHT_KEY)) {
    window_base->move_focus_right_key =
        tk_str_copy(window_base->move_focus_right_key, value_str(v));
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

  if (window_base->save_focus_widget) {
    widget_unref(window_base->save_focus_widget);
    window_base->save_focus_widget = NULL;
  }

  TKMEM_FREE(window_base->theme);
  TKMEM_FREE(window_base->open_anim_hint);
  TKMEM_FREE(window_base->close_anim_hint);
  TKMEM_FREE(window_base->move_focus_prev_key);
  TKMEM_FREE(window_base->move_focus_next_key);
  TKMEM_FREE(window_base->move_focus_up_key);
  TKMEM_FREE(window_base->move_focus_down_key);
  TKMEM_FREE(window_base->move_focus_left_key);
  TKMEM_FREE(window_base->move_focus_right_key);

  window_base_unload_theme_obj(widget);

  return RET_OK;
}

ret_t window_base_invalidate(widget_t* widget, rect_t* r) {
  native_window_t* nw = NULL;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  nw = (native_window_t*)widget_get_prop_pointer(widget, WIDGET_PROP_NATIVE_WINDOW);
  if (nw != NULL) {
    if (nw->shared) {
      r->x += widget->x;
      r->y += widget->y;
    }
    native_window_invalidate(nw, r);
  }

  return RET_OK;
}

static widget_t* window_base_get_key_target_leaf(widget_t* widget) {
  widget_t* iter = widget;

  while (iter->key_target != NULL) {
    iter = iter->key_target;
  }

  return iter;
}

ret_t window_base_on_event(widget_t* widget, event_t* e) {
  window_base_t* win = WINDOW_BASE(widget);
  return_value_if_fail(widget != NULL && win != NULL, RET_BAD_PARAMS);

  if (e->type == EVT_WINDOW_WILL_OPEN) {
    win->stage = WINDOW_STAGE_CREATED;
    window_base_load_theme_obj(widget);
    widget_layout_children(widget);
    widget_update_style_recursive(widget);
  } else if (e->type == EVT_WINDOW_OPEN) {
    win->stage = WINDOW_STAGE_OPENED;
    if (widget->sensitive) {
      widget_set_focused_internal(widget, TRUE);
    }
  } else if (e->type == EVT_WINDOW_LOAD) {
  } else if (e->type == EVT_WINDOW_CLOSE) {
    win->stage = WINDOW_STAGE_CLOSED;
  } else if (e->type == EVT_THEME_CHANGED) {
    window_base_reload_theme_obj(widget);
  } else if (e->type == EVT_REQUEST_CLOSE_WINDOW) {
    log_debug("EVT_REQUEST_CLOSE_WINDOW\n");
    if (win->closable == WINDOW_CLOSABLE_YES) {
      window_close(widget);
    }
  } else if (e->type == EVT_WINDOW_TO_FOREGROUND) {
    win->stage = WINDOW_STAGE_OPENED;
    widget->parent->grab_widget_count =
        widget->grab_widget_count + win->grab_count_when_to_foreground;
    if (widget->parent->grab_widget_count) {
      widget->parent->grab_widget = widget;
    }
    if (win->save_focus_widget) {
      widget_set_focused_internal(win->save_focus_widget, TRUE);
      widget_unref(win->save_focus_widget);
      win->save_focus_widget = NULL;
    } else if (widget_is_window_manager(widget->parent)) {
      if (widget->sensitive) {
        widget_set_focused_internal(widget, TRUE);
      }
    }
  } else if (e->type == EVT_WINDOW_TO_BACKGROUND) {
    win->stage = WINDOW_STAGE_SUSPEND;
    if (widget->parent != NULL && widget->parent->grab_widget == widget) {
      win->grab_count_when_to_foreground =
          widget->parent->grab_widget_count - widget->grab_widget_count;
      widget->parent->grab_widget_count = 0;
      widget->parent->grab_widget = NULL;
    } else {
      win->grab_count_when_to_foreground = 0;
    }
    if (win->save_focus_widget) {
      widget_unref(win->save_focus_widget);
      win->save_focus_widget = NULL;
    }
    widget_t* save_focus_widget = window_base_get_key_target_leaf(widget);
    if (save_focus_widget != widget) {
      win->save_focus_widget = save_focus_widget;
      if (win->save_focus_widget) {
        widget_ref(win->save_focus_widget);
      }
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
  win->move_focus_next_key = tk_strdup(TK_KEY_MOVE_FOCUS_NEXT);
  win->move_focus_prev_key = tk_strdup(TK_KEY_MOVE_FOCUS_PREV);

  return widget;
}

ret_t window_close_force(widget_t* widget) {
  widget = widget_get_window(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_close_window_force(widget->parent, widget);
}

ret_t window_close(widget_t* widget) {
  widget = widget_get_window(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return window_manager_close_window(widget->parent, widget);
}

TK_DECL_VTABLE(window_base) = {.size = sizeof(window_base_t), .parent = TK_PARENT_VTABLE(widget)};

widget_t* window_base_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, window_base), NULL);

  return widget;
}
