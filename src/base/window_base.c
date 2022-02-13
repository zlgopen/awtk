/**
 * File:   window_base.h
 * Author: AWTK Develop Team
 * Brief:  window_base
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  paint_event_t e;
  widget_dispatch(widget, paint_event_init(&e, EVT_PAINT, widget, c));

  return RET_OK;
}

ret_t window_base_on_paint_begin(widget_t* widget, canvas_t* c) {
  font_manager_t* fm = widget_get_font_manager(widget);
  image_manager_t* imm = widget_get_image_manager(widget);
  assets_manager_t* am = widget_get_assets_manager(widget);

  canvas_set_font_manager(c, fm);
  canvas_set_assets_manager(c, am);
  image_manager_set_assets_manager(imm, am);

  return RET_OK;
}

ret_t window_base_on_paint_end(widget_t* widget, canvas_t* c) {
  return RET_OK;
}

static ret_t window_base_load_theme_obj(widget_t* widget) {
  const char* theme_name = widget->name;
  window_base_t* window_base = WINDOW_BASE(widget);
  assets_manager_t* am = widget_get_assets_manager(widget);

  if (window_base->theme_obj != NULL) {
    return RET_OK;
  }

  if (window_base->theme != NULL && window_base->theme[0] != 0) {
    theme_name = window_base->theme;
  }

  if (theme_name != NULL) {
    window_base->res_theme = assets_manager_ref(am, ASSET_TYPE_STYLE, theme_name);
  }

  if (window_base->res_theme != NULL) {
    window_base->theme_obj = theme_default_create(window_base->res_theme->data);
  }

  if (window_base->theme_obj != NULL) {
    widget_update_style_recursive(widget);
    widget_layout(widget);
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
  window_base_load_theme_obj(widget);

  return RET_OK;
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
  } else if (tk_str_eq(name, WIDGET_PROP_SINGLE_INSTANCE)) {
    value_set_bool(v, window_base->single_instance);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STRONGLY_FOCUS)) {
    value_set_bool(v, window_base->strongly_focus);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DESIGN_W)) {
    value_set_uint32(v, window_base->design_w);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DESIGN_H)) {
    value_set_uint32(v, window_base->design_h);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_SCALE_CHILDREN_X)) {
    value_set_bool(v, window_base->auto_scale_children_x);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_SCALE_CHILDREN_Y)) {
    value_set_bool(v, window_base->auto_scale_children_y);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_SCALE_CHILDREN_W)) {
    value_set_bool(v, window_base->auto_scale_children_w);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_SCALE_CHILDREN_H)) {
    value_set_bool(v, window_base->auto_scale_children_h);
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
    window_base_reload_theme_obj(widget);
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
  } else if (tk_str_eq(name, WIDGET_PROP_SINGLE_INSTANCE)) {
    window_base->single_instance = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STRONGLY_FOCUS)) {
    window_base->strongly_focus = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DESIGN_W)) {
    window_base->design_w = value_uint32(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DESIGN_H)) {
    window_base->design_h = value_uint32(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_SCALE_CHILDREN_X)) {
    window_base->auto_scale_children_x = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_SCALE_CHILDREN_Y)) {
    window_base->auto_scale_children_y = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_SCALE_CHILDREN_W)) {
    window_base->auto_scale_children_w = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_SCALE_CHILDREN_H)) {
    window_base->auto_scale_children_h = value_bool(v);
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

ret_t window_base_invalidate(widget_t* widget, const rect_t* rect) {
  rect_t t = *rect;
  rect_t* r = &t;
  native_window_t* nw = NULL;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->parent != NULL) {
    widget_t* top = window_manager_get_top_main_window(widget->parent);
    if (top != widget) {
      if (widget_index_of(widget) < widget_index_of(top)) {
        if (widget->x >= top->x && widget->y >= top->y &&
            (widget->x + widget->w) <= (top->x + top->w) &&
            (widget->y + widget->h) <= (top->y + top->h)) {
          return RET_OK;
        }
      }
    }
  }

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

ret_t window_base_auto_scale_children(widget_t* widget) {
  window_base_t* win = WINDOW_BASE(widget);
  return_value_if_fail(win->design_w > 0 && win->design_h > 0, RET_BAD_PARAMS);

  return widget_auto_scale_children(widget, win->design_w, win->design_h,
                                    win->auto_scale_children_x, win->auto_scale_children_y,
                                    win->auto_scale_children_w, win->auto_scale_children_h);
}

ret_t window_set_auto_scale_children(widget_t* widget, uint32_t design_w, uint32_t design_h) {
  window_base_t* base = WINDOW_BASE(widget);
  return_value_if_fail(base != NULL && design_w > 0 && design_h > 0, RET_BAD_PARAMS);

  base->design_w = design_w;
  base->design_h = design_h;
  base->auto_scale_children_x = TRUE;
  base->auto_scale_children_y = TRUE;
  base->auto_scale_children_w = TRUE;
  base->auto_scale_children_h = TRUE;

  return window_base_auto_scale_children(widget);
}

ret_t window_base_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  window_base_t* win = WINDOW_BASE(widget);
  return_value_if_fail(widget != NULL && win != NULL, RET_BAD_PARAMS);

  if (e->type == EVT_WINDOW_WILL_OPEN) {
    win->stage = WINDOW_STAGE_CREATED;
    window_base_load_theme_obj(widget);
    widget_update_style_recursive(widget);
    widget_layout(widget);
    if (widget->sensitive) {
      widget_set_focused_internal(widget, TRUE);
    }
  } else if (e->type == EVT_WINDOW_OPEN) {
    win->stage = WINDOW_STAGE_OPENED;
    if (widget->sensitive) {
      widget_set_focused_internal(widget, TRUE);
    }
  } else if (e->type == EVT_WINDOW_LOAD) {
    win->stage = WINDOW_STAGE_LOADED;
    window_base_load_theme_obj(widget);
    widget_layout(widget);

    if (win->design_w && win->design_h) {
      if (win->auto_scale_children_x || win->auto_scale_children_y || win->auto_scale_children_w ||
          win->auto_scale_children_h) {
        window_base_auto_scale_children(widget);
      }
    }
  } else if (e->type == EVT_WINDOW_CLOSE) {
    win->stage = WINDOW_STAGE_CLOSED;
    widget_off_by_ctx(window_manager(), widget);
  } else if (e->type == EVT_THEME_CHANGED) {
    window_base_reload_theme_obj(widget);
  } else if (e->type == EVT_REQUEST_CLOSE_WINDOW) {
    log_debug("EVT_REQUEST_CLOSE_WINDOW\n");
    if (win->closable == WINDOW_CLOSABLE_YES) {
      window_close(widget);
    }

    if (win->closable != WINDOW_CLOSABLE_CONFIRM) {
      ret = RET_STOP;
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

    if (win->pressed) {
      pointer_event_t abort;
      pointer_event_init(&abort, EVT_POINTER_DOWN_ABORT, widget, 0, 0);
      widget_on_pointer_up(widget, &abort);
    }

    if (widget->parent != NULL && widget->parent->grab_widget == widget) {
      win->grab_count_when_to_foreground =
          widget->parent->grab_widget_count - widget->grab_widget_count;
      widget->parent->grab_widget_count = 0;
      widget->parent->grab_widget = NULL;
    } else {
      win->grab_count_when_to_foreground = 0;
    }
  } else if (e->type == EVT_BLUR) {
    widget_t* save_focus_widget = NULL;
    if (win->save_focus_widget) {
      widget_unref(win->save_focus_widget);
      win->save_focus_widget = NULL;
    }
    save_focus_widget = window_base_get_key_target_leaf(widget);
    if (save_focus_widget != widget) {
      win->save_focus_widget = save_focus_widget;
      if (win->save_focus_widget) {
        widget_ref(win->save_focus_widget);
      }
    }
  } else if (e->type == EVT_POINTER_DOWN) {
    win->pressed = TRUE;
  } else if (e->type == EVT_POINTER_UP) {
    win->pressed = FALSE;
  }

  return ret;
}

static ret_t window_on_keydown_before_children(void* ctx, event_t* e) {
  widget_t* win = WIDGET(ctx);
  key_event_t* evt = key_event_cast(e);
  window_base_t* base = WINDOW_BASE(win);
  widget_t* focus = widget_get_focused_widget(win);
  keyboard_type_t keyboard_type = system_info()->keyboard_type;
  bool_t moving_focus_mode = base->moving_focus_mode;

  if (focus != NULL) {
    if (focus->vt->return_key_to_activate) {
      /*对于按钮等通过回车键激活控件，方向键始终用于切换焦点*/
      base->moving_focus_mode = TRUE;
    } else {
      /*其它控件，回车键用于切换模式*/
      if (evt->key == TK_KEY_RETURN) {
        base->moving_focus_mode = !base->moving_focus_mode;
        log_debug("change moving_focus_mode:%d\n", base->moving_focus_mode);

#ifdef WITH_STATE_ACTIVATED
        if (!base->moving_focus_mode) {
          widget_set_state(focus, WIDGET_STATE_ACTIVATED);
        } else {
          widget_set_state(focus, WIDGET_STATE_FOCUSED);
        }
#endif /*WITH_STATE_ACTIVATED*/

        return RET_OK;
      }
    }

    if (moving_focus_mode) {
      if (keyboard_type == KEYBOARD_3KEYS) {
        switch (evt->key) {
          case TK_KEY_LEFT:
          case TK_KEY_UP: {
            widget_focus_prev(focus);
            break;
          }
          case TK_KEY_RIGHT:
          case TK_KEY_DOWN: {
            widget_focus_next(focus);
            return RET_STOP;
          }
          default:
            break;
        }
      } else {
        switch (evt->key) {
          case TK_KEY_LEFT: {
            widget_focus_left(focus);
            return RET_STOP;
          }
          case TK_KEY_RIGHT: {
            widget_focus_right(focus);
            return RET_STOP;
          }
          case TK_KEY_UP: {
            widget_focus_up(focus);
            return RET_STOP;
          }
          case TK_KEY_DOWN: {
            widget_focus_down(focus);
            return RET_STOP;
          }
          default:
            break;
        }
      }
    }
  }

  return RET_OK;
}

ret_t window_enable_35keys_mode(widget_t* win) {
  widget_on(win, EVT_KEY_DOWN_BEFORE_CHILDREN, window_on_keydown_before_children, win);
  return RET_OK;
}

widget_t* window_base_create(widget_t* parent, const widget_vtable_t* vt, xy_t x, xy_t y, wh_t w,
                             wh_t h) {
  widget_t* widget = widget_create(NULL, vt, x, y, w, h);
  window_base_t* win = WINDOW_BASE(widget);
  keyboard_type_t keyboard_type = system_info()->keyboard_type;

  return_value_if_fail(win != NULL, NULL);

  if (parent == NULL) {
    parent = window_manager();
  }

  return_value_if_fail(window_manager_open_window(parent, widget) == RET_OK, NULL);
  win->stage = WINDOW_STAGE_NONE;
  win->move_focus_next_key = tk_strdup(TK_KEY_MOVE_FOCUS_NEXT);
  win->move_focus_prev_key = tk_strdup(TK_KEY_MOVE_FOCUS_PREV);
  if (keyboard_type == KEYBOARD_3KEYS || keyboard_type == KEYBOARD_5KEYS) {
    window_enable_35keys_mode(widget);
  }

  return widget;
}

ret_t window_base_set_need_relayout(widget_t* widget, bool_t need_relayout) {
  window_base_t* win = WINDOW_BASE(widget);
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  win->need_relayout = need_relayout;

  return RET_OK;
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

static const char* s_window_base_properties[] = {WIDGET_PROP_ANIM_HINT,
                                                 WIDGET_PROP_OPEN_ANIM_HINT,
                                                 WIDGET_PROP_DISABLE_ANIM,
                                                 WIDGET_PROP_NATIVE_WINDOW,
                                                 WIDGET_PROP_CLOSE_ANIM_HINT,
                                                 WIDGET_PROP_THEME,
                                                 WIDGET_PROP_MOVE_FOCUS_PREV_KEY,
                                                 WIDGET_PROP_MOVE_FOCUS_NEXT_KEY,
                                                 WIDGET_PROP_MOVE_FOCUS_UP_KEY,
                                                 WIDGET_PROP_MOVE_FOCUS_DOWN_KEY,
                                                 WIDGET_PROP_MOVE_FOCUS_LEFT_KEY,
                                                 WIDGET_PROP_MOVE_FOCUS_RIGHT_KEY,
                                                 WIDGET_PROP_SINGLE_INSTANCE,
                                                 WIDGET_PROP_STRONGLY_FOCUS,
                                                 WIDGET_PROP_DESIGN_W,
                                                 WIDGET_PROP_DESIGN_H,
                                                 WIDGET_PROP_AUTO_SCALE_CHILDREN_X,
                                                 WIDGET_PROP_AUTO_SCALE_CHILDREN_Y,
                                                 WIDGET_PROP_AUTO_SCALE_CHILDREN_W,
                                                 WIDGET_PROP_AUTO_SCALE_CHILDREN_H,
                                                 WIDGET_PROP_CLOSABLE,
                                                 NULL};

TK_DECL_VTABLE(window_base) = {
    .size = sizeof(window_base_t),
    .parent = TK_PARENT_VTABLE(widget),
    .clone_properties = s_window_base_properties,
    .persistent_properties = s_window_base_properties,
};

widget_t* window_base_cast(widget_t* widget) {
  if (widget != NULL && widget->vt != NULL && widget->vt->is_window) {
    return widget;
  }

  return NULL;
}
