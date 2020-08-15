/**
 * File:   widget.c
 * Author: AWTK Develop Team
 * Brief:  basic class of all widget
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
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "tkc/color_parser.h"
#include "tkc/object_default.h"

#include "base/keys.h"
#include "base/enums.h"
#include "base/theme.h"
#include "tkc/time_now.h"
#include "base/idle.h"
#include "base/widget.h"
#include "base/layout.h"
#include "native_window.h"
#include "base/main_loop.h"
#include "base/ui_feedback.h"
#include "base/system_info.h"
#include "base/window_manager.h"
#include "base/widget_vtable.h"
#include "base/style_mutable.h"
#include "base/style_factory.h"
#include "base/widget_animator_manager.h"
#include "base/widget_animator_factory.h"
#include "blend/image_g2d.h"

#define return_if_equal(p, value) \
  if ((p) == value) {             \
    return (value);               \
  }

ret_t widget_focus_up(widget_t* widget);
ret_t widget_focus_down(widget_t* widget);
ret_t widget_focus_left(widget_t* widget);
ret_t widget_focus_right(widget_t* widget);
static ret_t widget_unref_async(widget_t* widget);
static ret_t widget_destroy_sync(widget_t* widget);
static ret_t widget_ensure_style_mutable(widget_t* widget);
static ret_t widget_dispatch_blur_event(widget_t* widget);
static ret_t widget_on_paint_done(widget_t* widget, canvas_t* c);

typedef widget_t* (*widget_find_wanted_focus_widget_t)(widget_t* widget, darray_t* all_focusable);
static ret_t widget_move_focus(widget_t* widget, widget_find_wanted_focus_widget_t find);

ret_t widget_set_need_update_style(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (!widget->need_update_style) {
    widget_invalidate_force(widget, NULL);
  }

  widget->need_update_style = TRUE;

  return RET_OK;
}

ret_t widget_update_style_recursive(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget_update_style(widget);
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_update_style_recursive(iter);
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t widget_set_need_update_style_recursive(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget_set_need_update_style(widget);
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_set_need_update_style_recursive(iter);
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

ret_t widget_update_style(widget_t* widget) {
  widget->need_update_style = FALSE;

  return style_notify_widget_state_changed(widget->astyle, widget);
}

static ret_t widget_real_destroy(widget_t* widget) {
  ENSURE(widget->ref_count == 1);

  if (widget->vt->on_destroy) {
    widget->vt->on_destroy(widget);
  }

  TKMEM_FREE(widget->name);
  TKMEM_FREE(widget->state);
  TKMEM_FREE(widget->style);
  TKMEM_FREE(widget->tr_text);
  TKMEM_FREE(widget->animation);

  memset(widget, 0x00, sizeof(widget_t));
  TKMEM_FREE(widget);

  return RET_OK;
}

static widget_t* widget_real_create(const widget_vtable_t* vt) {
  widget_t* widget = TKMEM_ALLOC(vt->size);
  return_value_if_fail(widget != NULL, NULL);

  memset(widget, 0x00, vt->size);
  widget->vt = vt;

  return widget;
}

static bool_t widget_is_scrollable(widget_t* widget) {
  return widget != NULL && widget->vt != NULL && widget->vt->scrollable;
}

static bool_t widget_with_focus_state(widget_t* widget) {
  value_t v;
  return_value_if_fail(widget != NULL && widget->vt != NULL, FALSE);
  value_set_bool(&v, FALSE);
  widget_get_prop(widget, WIDGET_PROP_WITH_FOCUS_STATE, &v);

  return value_bool(&v);
}

static bool_t widget_is_focusable(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt != NULL, FALSE);

  if (!widget->visible || !widget->sensitive) {
    return FALSE;
  }

  return widget->focusable || widget->vt->focusable;
}

ret_t widget_move(widget_t* widget, xy_t x, xy_t y) {
  event_t e = event_init(EVT_WILL_MOVE, widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->x != x || widget->y != y) {
    widget_dispatch(widget, &e);

    widget_invalidate_force(widget, NULL);
    widget->x = x;
    widget->y = y;
    widget_invalidate_force(widget, NULL);

    e.type = EVT_MOVE;
    widget_dispatch(widget, &e);
  }

  return RET_OK;
}

ret_t widget_resize(widget_t* widget, wh_t w, wh_t h) {
  event_t e = event_init(EVT_WILL_RESIZE, widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->w != w || widget->h != h) {
    widget_dispatch(widget, &e);

    widget_invalidate_force(widget, NULL);
    widget->w = w;
    widget->h = h;
    widget_invalidate_force(widget, NULL);
    widget_set_need_relayout_children(widget);

    e.type = EVT_RESIZE;
    widget_dispatch(widget, &e);
  }

  return RET_OK;
}

ret_t widget_move_resize(widget_t* widget, xy_t x, xy_t y, wh_t w, wh_t h) {
  event_t e = event_init(EVT_WILL_MOVE_RESIZE, widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->x != x || widget->y != y || widget->w != w || widget->h != h) {
    widget_dispatch(widget, &e);

    widget_invalidate_force(widget, NULL);
    widget->x = x;
    widget->y = y;
    widget->w = w;
    widget->h = h;
    widget_invalidate_force(widget, NULL);
    widget_set_need_relayout_children(widget);

    e.type = EVT_MOVE_RESIZE;
    widget_dispatch(widget, &e);
  }

  return RET_OK;
}

ret_t widget_set_value(widget_t* widget, int32_t value) {
  value_t v;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, WIDGET_PROP_VALUE, value_set_int(&v, value));
}

ret_t widget_add_value(widget_t* widget, int32_t delta) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_value(widget, widget_get_value(widget) + delta);
}

ret_t widget_animate_value_to(widget_t* widget, int32_t value, uint32_t duration) {
  if (duration == 0) {
    return widget_set_value(widget, value);
  } else {
    char params[64];
    tk_snprintf(params, sizeof(params) - 1, "value(to=%d, duration=%d)", value, duration);

    widget_destroy_animator(widget, "value");
    return widget_create_animator(widget, params);
  }
}

bool_t widget_is_window_opened(widget_t* widget) {
  widget_t* win = widget_get_window(widget);

  if (win != NULL) {
    int32_t stage = widget_get_prop_int(win, WIDGET_PROP_STAGE, WINDOW_STAGE_NONE);
    return WINDOW_STAGE_OPENED == stage || WINDOW_STAGE_SUSPEND == stage;
  } else {
    return FALSE;
  }
}

ret_t widget_use_style(widget_t* widget, const char* value) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->style = tk_str_copy(widget->style, value);
  if (widget_is_window_opened(widget)) {
    widget_update_style(widget);
  }

  return widget_invalidate(widget, NULL);
}

ret_t widget_set_text(widget_t* widget, const wchar_t* text) {
  value_t v;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, WIDGET_PROP_TEXT, value_set_wstr(&v, text));
}

ret_t widget_set_text_utf8(widget_t* widget, const char* text) {
  value_t v;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, WIDGET_PROP_TEXT, value_set_str(&v, text));
}

image_manager_t* widget_get_image_manager(widget_t* widget) {
  image_manager_t* ret = image_manager();
  return_value_if_fail(widget != NULL && widget->vt != NULL, ret);

  if (tk_str_eq(widget->vt->type, WIDGET_TYPE_WINDOW_MANAGER)) {
    ret = image_manager();
  } else {
    value_t v;
    widget_t* win = widget_get_window(widget);
    if (widget_get_prop(win, WIDGET_PROP_IMAGE_MANAGER, &v) == RET_OK) {
      ret = (image_manager_t*)value_pointer(&v);
    }
  }

  return ret;
}

locale_info_t* widget_get_locale_info(widget_t* widget) {
  locale_info_t* ret = locale_info();
  return_value_if_fail(widget != NULL && widget->vt != NULL, ret);

  if (tk_str_eq(widget->vt->type, WIDGET_TYPE_WINDOW_MANAGER)) {
    ret = locale_info();
  } else {
    value_t v;
    widget_t* win = widget_get_window(widget);
    if (widget_get_prop(win, WIDGET_PROP_LOCALE_INFO, &v) == RET_OK) {
      ret = (locale_info_t*)value_pointer(&v);
    }
  }

  return ret;
}

assets_manager_t* widget_get_assets_manager(widget_t* widget) {
  assets_manager_t* am = assets_manager();
  return_value_if_fail(widget != NULL && widget->vt != NULL, am);

  if (widget->assets_manager != NULL) {
    return widget->assets_manager;
  }

  if (tk_str_eq(widget->vt->type, WIDGET_TYPE_WINDOW_MANAGER)) {
    am = assets_manager();
  } else {
    value_t v;
    widget_t* win = widget_get_window(widget);
    if (widget_get_prop(win, WIDGET_PROP_ASSETS_MANAGER, &v) == RET_OK) {
      am = (assets_manager_t*)value_pointer(&v);
    }
  }
  widget->assets_manager = am;

  return am;
}

font_manager_t* widget_get_font_manager(widget_t* widget) {
  font_manager_t* ret = font_manager();
  return_value_if_fail(widget != NULL && widget->vt != NULL, ret);

  if (tk_str_eq(widget->vt->type, WIDGET_TYPE_WINDOW_MANAGER)) {
    ret = font_manager();
  } else {
    value_t v;
    widget_t* win = widget_get_window(widget);
    if (widget_get_prop(win, WIDGET_PROP_FONT_MANAGER, &v) == RET_OK) {
      ret = (font_manager_t*)value_pointer(&v);
    }
  }

  return ret;
}

static ret_t widget_apply_tr_text_before_paint(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  if (widget->tr_text != NULL) {
    const char* text = locale_info_tr(widget_get_locale_info(widget), widget->tr_text);
    widget_set_prop_str(widget, WIDGET_PROP_TEXT, text);
  }

  return RET_REMOVE;
}

ret_t widget_set_tr_text(widget_t* widget, const char* text) {
  value_t v;
  const char* tr_text = NULL;
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(widget != NULL && text != NULL, RET_OK);

  widget->tr_text = tk_str_copy(widget->tr_text, text);
  if (win != NULL) {
    tr_text = locale_info_tr(widget_get_locale_info(widget), text);
    widget_set_prop(widget, WIDGET_PROP_TEXT, value_set_str(&v, tr_text));
  } else {
    widget_set_prop_str(widget, WIDGET_PROP_TEXT, text);
    widget_on(widget, EVT_BEFORE_PAINT, widget_apply_tr_text_before_paint, widget);
  }

  return RET_OK;
}

ret_t widget_re_translate_text(widget_t* widget) {
  if (widget->vt->on_re_translate != NULL) {
    widget->vt->on_re_translate(widget);
  } else if (widget->tr_text != NULL) {
    value_t v;
    const char* tr_text = locale_info_tr(widget_get_locale_info(widget), widget->tr_text);
    widget_set_prop(widget, WIDGET_PROP_TEXT, value_set_str(&v, tr_text));
    widget_invalidate(widget, NULL);
  }

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_re_translate_text(iter);
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

int32_t widget_get_value(widget_t* widget) {
  value_t v;
  return_value_if_fail(widget != NULL, 0);

  return widget_get_prop(widget, WIDGET_PROP_VALUE, &v) == RET_OK ? value_int(&v) : 0;
}

const wchar_t* widget_get_text(widget_t* widget) {
  value_t v;
  return_value_if_fail(widget != NULL, 0);

  return widget_get_prop(widget, WIDGET_PROP_TEXT, &v) == RET_OK ? value_wstr(&v) : 0;
}

ret_t widget_set_name(widget_t* widget, const char* name) {
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  widget->name = tk_str_copy(widget->name, name);

  return RET_OK;
}

ret_t widget_set_theme(widget_t* widget, const char* name) {
#ifdef WITH_FS_RES
  const asset_info_t* info = NULL;
  event_t e = event_init(EVT_THEME_CHANGED, NULL);
  widget_t* wm = widget_get_window_manager(widget);
  font_manager_t* fm = widget_get_font_manager(widget);
  image_manager_t* imm = widget_get_image_manager(widget);
  assets_manager_t* am = widget_get_assets_manager(widget);
  locale_info_t* locale_info = widget_get_locale_info(widget);

  return_value_if_fail(am != NULL && name != NULL, RET_BAD_PARAMS);

  font_manager_unload_all(fm);
  image_manager_unload_all(imm);
  locale_info_reload(locale_info);
  assets_manager_set_theme(am, name);
  widget_reset_canvas(widget);

  info = assets_manager_ref(am, ASSET_TYPE_STYLE, "default");
  if (info != NULL) {
    assets_manager_unref(assets_manager(), info);
    theme_init(theme(), info->data);
  }

  widget_dispatch(wm, &e);
  widget_invalidate_force(wm, NULL);

  log_debug("theme changed: %s\n", name);
#else
  log_debug("WITH_FS_RES is not defined\n");
#endif

  return RET_OK;
}

ret_t widget_set_pointer_cursor(widget_t* widget, const char* cursor) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->pointer_cursor = tk_str_copy(widget->pointer_cursor, cursor);

  return RET_OK;
}

#ifndef WITHOUT_WIDGET_ANIMATORS
ret_t widget_set_animation(widget_t* widget, const char* animation) {
  return_value_if_fail(widget != NULL && animation != NULL, RET_BAD_PARAMS);

  widget->animation = tk_str_copy(widget->animation, animation);

  return widget_create_animator(widget, animation);
}

ret_t widget_create_animator(widget_t* widget, const char* animation) {
  const char* end = NULL;
  const char* start = animation;
  return_value_if_fail(widget != NULL && animation != NULL, RET_BAD_PARAMS);

  while (start != NULL) {
    char params[256];
    end = strchr(start, ';');

    memset(params, 0x00, sizeof(params));
    if (end != NULL) {
      tk_strncpy(params, start, tk_min(end - start, sizeof(params) - 1));
    } else {
      tk_strncpy(params, start, sizeof(params) - 1);
    }

    return_value_if_fail(widget_animator_create(widget, params) != NULL, RET_BAD_PARAMS);

    if (end == NULL) {
      break;
    } else {
      start = end + 1;
    }
  }

  widget_invalidate(widget, NULL);

  return RET_OK;
}

ret_t widget_start_animator(widget_t* widget, const char* name) {
  return widget_animator_manager_start(widget_animator_manager(), widget, name);
}

ret_t widget_set_animator_time_scale(widget_t* widget, const char* name, float_t time_scale) {
  return widget_animator_manager_set_time_scale(widget_animator_manager(), widget, name,
                                                time_scale);
}

ret_t widget_pause_animator(widget_t* widget, const char* name) {
  return widget_animator_manager_pause(widget_animator_manager(), widget, name);
}

widget_animator_t* widget_find_animator(widget_t* widget, const char* name) {
  return widget_animator_manager_find(widget_animator_manager(), widget, name);
}

ret_t widget_stop_animator(widget_t* widget, const char* name) {
  return widget_animator_manager_stop(widget_animator_manager(), widget, name);
}

ret_t widget_destroy_animator(widget_t* widget, const char* name) {
  return widget_animator_manager_remove_all(widget_animator_manager(), widget, name);
}
#else
ret_t widget_start_animator(widget_t* widget, const char* name) {
  return RET_OK;
}
ret_t widget_create_animator(widget_t* widget, const char* animation) {
  return RET_OK;
}
ret_t widget_set_animation(widget_t* widget, const char* animation) {
  return RET_OK;
}
ret_t widget_set_animator_time_scale(widget_t* widget, const char* name, float_t time_scale) {
  return RET_OK;
}
ret_t widget_pause_animator(widget_t* widget, const char* name) {
  return RET_OK;
}
widget_animator_t* widget_find_animator(widget_t* widget, const char* name) {
  return NULL;
}
ret_t widget_stop_animator(widget_t* widget, const char* name) {
  return RET_OK;
}
ret_t widget_destroy_animator(widget_t* widget, const char* name) {
  return RET_OK;
}
#endif /*WITHOUT_WIDGET_ANIMATORS*/

ret_t widget_set_enable(widget_t* widget, bool_t enable) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->enable != enable) {
    widget->enable = enable;
    widget_set_need_update_style_recursive(widget);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_set_feedback(widget_t* widget, bool_t feedback) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->feedback = feedback;

  return RET_OK;
}

ret_t widget_set_floating(widget_t* widget, bool_t floating) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->floating = floating;

  return RET_OK;
}

ret_t widget_set_focused_internal(widget_t* widget, bool_t focused) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->focused != focused) {
    widget->focused = focused;
    widget_set_need_update_style(widget);

    if (focused) {
      event_t e = event_init(EVT_FOCUS, widget);
      widget_set_as_key_target(widget);

      widget_dispatch(widget, &e);
    } else {
      event_t e = event_init(EVT_BLUR, widget);
      widget_dispatch(widget, &e);
      widget_dispatch_blur_event(widget);
    }

    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t widget_set_focused(widget_t* widget, bool_t focused) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget_set_focused_internal(widget, focused);
  if (focused) {
    widget_ensure_visible_in_viewport(widget);
  }

  return RET_OK;
}

ret_t widget_set_focusable(widget_t* widget, bool_t focusable) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->focusable = focusable;

  return RET_OK;
}

ret_t widget_set_state(widget_t* widget, const char* state) {
  return_value_if_fail(widget != NULL && state != NULL, RET_BAD_PARAMS);

  if (!tk_str_eq(widget->state, state)) {
    widget_invalidate_force(widget, NULL);
    widget->state = tk_str_copy(widget->state, state);
    widget_set_need_update_style(widget);
    widget_invalidate_force(widget, NULL);
  }

  return RET_OK;
}

const char* widget_get_state_for_style(widget_t* widget, bool_t active, bool_t checked) {
  const char* state = WIDGET_STATE_NORMAL;
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL, state);

  state = (const char*)(widget->state);

  while (iter != NULL) {
    if (!iter->enable) {
      if (active) return WIDGET_STATE_DISABLE_OF_ACTIVE;
      if (checked) return WIDGET_STATE_DISABLE_OF_CHECKED;
      return WIDGET_STATE_DISABLE;
    }
    iter = iter->parent;
  }

  if (widget_is_focusable(widget) || widget_with_focus_state(widget)) {
    if (widget->focused) {
      if (tk_str_eq(state, WIDGET_STATE_NORMAL)) {
        state = WIDGET_STATE_FOCUSED;
      }
    } else {
      if (tk_str_eq(state, WIDGET_STATE_FOCUSED)) {
        state = WIDGET_STATE_NORMAL;
      }
    }
  }

  if (active) {
    if (tk_str_eq(state, WIDGET_STATE_NORMAL)) {
      state = WIDGET_STATE_NORMAL_OF_ACTIVE;
    } else if (tk_str_eq(state, WIDGET_STATE_PRESSED)) {
      state = WIDGET_STATE_PRESSED_OF_ACTIVE;
    } else if (tk_str_eq(state, WIDGET_STATE_OVER)) {
      state = WIDGET_STATE_OVER_OF_ACTIVE;
    } else if (widget_is_focusable(widget) && widget->focused) {
      state = WIDGET_STATE_FOCUSED_OF_ACTIVE;
    }
  } else if (checked) {
    if (tk_str_eq(state, WIDGET_STATE_NORMAL)) {
      state = WIDGET_STATE_NORMAL_OF_CHECKED;
    } else if (tk_str_eq(state, WIDGET_STATE_PRESSED)) {
      state = WIDGET_STATE_PRESSED_OF_CHECKED;
    } else if (tk_str_eq(state, WIDGET_STATE_OVER)) {
      state = WIDGET_STATE_OVER_OF_CHECKED;
    } else if (widget_is_focusable(widget) && widget->focused) {
      state = WIDGET_STATE_FOCUSED_OF_CHECKED;
    }
  }

  return state;
}

ret_t widget_set_opacity(widget_t* widget, uint8_t opacity) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->opacity = opacity;
  widget_invalidate(widget, NULL);

  return RET_OK;
}

ret_t widget_set_dirty_rect_tolerance(widget_t* widget, uint16_t dirty_rect_tolerance) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->dirty_rect_tolerance = dirty_rect_tolerance;
  widget_invalidate(widget, NULL);

  return RET_OK;
}

ret_t widget_destroy_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->children != NULL) {
    WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)

    widget_remove_child_prepare(widget, iter);
    widget_unref(iter);

    widget->children->elms[i] = NULL;

    WIDGET_FOR_EACH_CHILD_END();
    widget->children->size = 0;
  }

  return RET_OK;
}

ret_t widget_add_child(widget_t* widget, widget_t* child) {
  event_t e = event_init(EVT_WIDGET_ADD_CHILD, widget);
  return_value_if_fail(widget != NULL && child != NULL && child->parent == NULL, RET_BAD_PARAMS);

  child->parent = widget;

  if (widget->children == NULL) {
    widget->children = darray_create(4, NULL, NULL);
  }

  if (widget->vt->on_add_child) {
    if (widget->vt->on_add_child(widget, child) == RET_OK) {
      return RET_OK;
    }
  }

  if (child->vt->on_attach_parent) {
    child->vt->on_attach_parent(child, widget);
  }

  ENSURE(darray_push(widget->children, child) == RET_OK);

  if (!widget_is_window_manager(widget)) {
    widget_set_need_relayout_children(widget);
  }

  if (!(child->initializing) && widget_get_window(child) != NULL) {
    widget_set_need_update_style_recursive(child);
  }

  widget_dispatch(widget, &e);

  return RET_OK;
}

ret_t widget_remove_child_prepare(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL, RET_BAD_PARAMS);

  if (!widget_is_window_manager(widget)) {
    widget_set_need_relayout_children(widget);
  }

  widget_invalidate_force(child, NULL);
  if (widget->target == child) {
    widget->target = NULL;
  }

  if (widget->grab_widget == child) {
    widget->grab_widget = NULL;
    widget->grab_widget_count = 0;
  }

  if (widget->key_target == child) {
    widget_dispatch_blur_event(widget->key_target);
    widget->key_target = NULL;
  }

  if (widget->vt->on_remove_child) {
    if (widget->vt->on_remove_child(widget, child) == RET_OK) {
      return RET_OK;
    }
  }

  if (child->vt->on_detach_parent) {
    child->vt->on_detach_parent(child, widget);
  }
  child->parent = NULL;

  return RET_OK;
}

ret_t widget_remove_child(widget_t* widget, widget_t* child) {
  ret_t ret = RET_OK;
  event_t e = event_init(EVT_WIDGET_REMOVE_CHILD, widget);
  return_value_if_fail(widget != NULL && child != NULL, RET_BAD_PARAMS);

  widget_remove_child_prepare(widget, child);
  ret = darray_remove(widget->children, child);

  if (ret == RET_OK) {
    widget_dispatch(widget, &e);
  }

  return ret;
}

ret_t widget_insert_child(widget_t* widget, uint32_t index, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget_add_child(widget, child) == RET_OK, RET_FAIL);

  return widget_restack(child, index);
}

ret_t widget_restack(widget_t* widget, uint32_t index) {
  uint32_t i = 0;
  uint32_t nr = 0;
  int32_t old_index = 0;
  widget_t** children = NULL;
  return_value_if_fail(widget != NULL && widget->parent != NULL, RET_BAD_PARAMS);

  old_index = widget_index_of(widget);
  nr = widget_count_children(widget->parent);
  return_value_if_fail(old_index >= 0 && nr > 0, RET_BAD_PARAMS);

  if (index >= nr) {
    index = nr - 1;
  }

  if (index == old_index || nr == 1) {
    return RET_OK;
  }

  children = (widget_t**)(widget->parent->children->elms);
  if (index < old_index) {
    for (i = old_index; i > index; i--) {
      children[i] = children[i - 1];
    }
  } else {
    for (i = old_index; i < index; i++) {
      children[i] = children[i + 1];
    }
  }
  children[index] = widget;

  return RET_OK;
}

static widget_t* widget_lookup_child(widget_t* widget, const char* name) {
  return_value_if_fail(widget != NULL && name != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (iter->name != NULL && tk_str_eq(iter->name, name)) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END()

  return NULL;
}

widget_t* widget_child(widget_t* widget, const char* path) {
  /*TODO*/
  return widget_lookup_child(widget, path);
}

static widget_t* widget_lookup_all(widget_t* widget, const char* name) {
  return_value_if_fail(widget != NULL && name != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (iter->name != NULL && tk_str_eq(iter->name, name)) {
    return iter;
  } else {
    iter = widget_lookup_all(iter, name);
    if (iter != NULL) {
      return iter;
    }
  }
  WIDGET_FOR_EACH_CHILD_END();

  return NULL;
}

widget_t* widget_lookup(widget_t* widget, const char* name, bool_t recursive) {
  if (recursive) {
    return widget_lookup_all(widget, name);
  } else {
    return widget_lookup_child(widget, name);
  }
}

static widget_t* widget_lookup_by_type_child(widget_t* widget, const char* type) {
  return_value_if_fail(widget != NULL && type != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (tk_str_eq(iter->vt->type, type)) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END()

  return NULL;
}

static widget_t* widget_lookup_by_type_all(widget_t* widget, const char* type) {
  return_value_if_fail(widget != NULL && type != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (tk_str_eq(iter->vt->type, type)) {
    return iter;
  } else {
    iter = widget_lookup_by_type_all(iter, type);
    if (iter != NULL) {
      return iter;
    }
  }
  WIDGET_FOR_EACH_CHILD_END();

  return NULL;
}

widget_t* widget_lookup_by_type(widget_t* widget, const char* type, bool_t recursive) {
  if (recursive) {
    return widget_lookup_by_type_all(widget, type);
  } else {
    return widget_lookup_by_type_child(widget, type);
  }
}

static ret_t widget_set_visible_self(widget_t* widget, bool_t visible) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->visible != visible) {
    widget_invalidate_force(widget, NULL);
    widget->visible = visible;
    widget_set_need_update_style(widget);
    widget_invalidate_force(widget, NULL);
    widget_set_need_relayout_children(widget->parent);
  }

  return RET_OK;
}

ret_t widget_set_sensitive(widget_t* widget, bool_t sensitive) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->sensitive = sensitive;

  return RET_OK;
}

static ret_t widget_set_visible_recursive(widget_t* widget, bool_t visible) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->visible = visible;
  widget_set_need_relayout_children(widget->parent);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_set_visible_recursive(iter, visible);
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

ret_t widget_set_visible_only(widget_t* widget, bool_t visible) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->visible = visible;

  return RET_OK;
}

ret_t widget_set_visible(widget_t* widget, bool_t visible, bool_t recursive) {
  if (recursive) {
    widget_set_visible_recursive(widget, visible);
  } else {
    widget_set_visible_self(widget, visible);
  }

  return widget_invalidate(widget, NULL);
}

widget_t* widget_find_target(widget_t* widget, xy_t x, xy_t y) {
  widget_t* ret = NULL;
  return_value_if_fail(widget != NULL, NULL);

  if (widget->vt && widget->vt->find_target) {
    ret = widget->vt->find_target(widget, x, y);
  } else {
    ret = widget_find_target_default(widget, x, y);
  }

  return ret;
}

ret_t widget_on_event_before_children(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  if (widget->vt && widget->vt->on_event_before_children) {
    ret = widget->vt->on_event_before_children(widget, e);
  }

  return ret;
}

static const char* widget_get_pointer_cursor(widget_t* widget) {
  if (widget->pointer_cursor != NULL) {
    return widget->pointer_cursor;
  } else if (widget->vt->pointer_cursor != NULL) {
    return widget->vt->pointer_cursor;
  }

  return WIDGET_CURSOR_DEFAULT;
}

ret_t widget_update_pointer_cursor(widget_t* widget) {
  widget_t* wm = widget_get_window_manager(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  return window_manager_set_cursor(wm, widget_get_pointer_cursor(widget));
}

ret_t widget_dispatch(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  widget_ref(widget);
  if (e->target == NULL) {
    e->target = widget;
  }

  if (widget->vt && widget->vt->on_event) {
    ret = widget->vt->on_event(widget, e);
  } else {
    ret = widget_on_event_default(widget, e);
  }

  if (ret != RET_STOP) {
    if (widget->emitter != NULL) {
      void* saved_target = e->target;

      e->target = widget;
      ret = emitter_dispatch(widget->emitter, e);
      e->target = saved_target;
    }
  }
  widget_unref(widget);

  return ret;
}

static ret_t widget_dispatch_callback(void* ctx, const void* data) {
  widget_t* widget = WIDGET(data);

  return widget_dispatch(widget, (event_t*)ctx);
}

ret_t widget_dispatch_recursive(widget_t* widget, event_t* e) {
  return widget_foreach(widget, widget_dispatch_callback, e);
}

uint32_t widget_on_with_tag(widget_t* widget, uint32_t type, event_func_t on_event, void* ctx,
                            uint32_t tag) {
  return_value_if_fail(widget != NULL && on_event != NULL, RET_BAD_PARAMS);
  if (widget->emitter == NULL) {
    widget->emitter = emitter_create();
  }

  return emitter_on_with_tag(widget->emitter, type, on_event, ctx, tag);
}

uint32_t widget_on(widget_t* widget, uint32_t type, event_func_t on_event, void* ctx) {
  return widget_on_with_tag(widget, type, on_event, ctx, 0);
}

uint32_t widget_child_on(widget_t* widget, const char* name, uint32_t type, event_func_t on_event,
                         void* ctx) {
  return widget_on(widget_lookup(widget, name, TRUE), type, on_event, ctx);
}

ret_t widget_off(widget_t* widget, uint32_t id) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->emitter != NULL, RET_BAD_PARAMS);

  return emitter_off(widget->emitter, id);
}

ret_t widget_off_by_tag(widget_t* widget, uint32_t tag) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->emitter == NULL) {
    return RET_OK;
  }

  return emitter_off_by_tag(widget->emitter, tag);
}

ret_t widget_off_by_ctx(widget_t* widget, void* ctx) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->emitter == NULL) {
    return RET_OK;
  }

  return emitter_off_by_ctx(widget->emitter, ctx);
}

ret_t widget_off_by_func(widget_t* widget, uint32_t type, event_func_t on_event, void* ctx) {
  return_value_if_fail(widget != NULL && on_event != NULL, RET_BAD_PARAMS);

  if (widget->emitter == NULL) {
    return RET_OK;
  }

  return emitter_off_by_func(widget->emitter, type, on_event, ctx);
}

ret_t widget_calc_icon_text_rect(const rect_t* ir, int32_t font_size, float_t text_size,
                                 int32_t icon_at, uint32_t img_w, uint32_t img_h, int32_t spacer,
                                 rect_t* r_text, rect_t* r_icon) {
  return_value_if_fail(ir != NULL && (r_text != NULL || r_icon != NULL), RET_BAD_PARAMS);

  if (r_icon == NULL) {
    *r_text = *ir;

    return RET_OK;
  }

  if (r_text == NULL) {
    *r_icon = *ir;

    return RET_OK;
  }

  return_value_if_fail(spacer < ir->h && spacer < ir->w, RET_BAD_PARAMS);
  switch (icon_at) {
    case ICON_AT_CENTRE: {
      int32_t w = ir->w - spacer - text_size - img_w;
      int32_t icon_h = ir->h - img_h;
      *r_text = rect_init(ir->x + img_w + spacer + w / 2, ir->y, text_size, ir->h);
      *r_icon = rect_init(ir->x + w / 2, ir->y + icon_h / 2, img_w, img_h);
      break;
    }
    case ICON_AT_RIGHT: {
      *r_icon = rect_init(ir->x + ir->w - ir->h, ir->y, ir->h, ir->h);
      *r_text = rect_init(ir->x, ir->y, ir->w - ir->h - spacer, ir->h);
      break;
    }
    case ICON_AT_TOP: {
      int32_t icon_h = ir->h - font_size - spacer;
      *r_icon = rect_init(ir->x, ir->y, ir->w, icon_h);
      *r_text = rect_init(ir->x, icon_h + spacer, ir->w, font_size);
      break;
    }
    case ICON_AT_BOTTOM: {
      int32_t icon_h = ir->h - font_size - spacer;
      *r_icon = rect_init(ir->x, ir->y + ir->h - icon_h, ir->w, icon_h);
      *r_text = rect_init(ir->x, ir->y, ir->w, font_size);
      break;
    }
    case ICON_AT_LEFT:
    default: {
      *r_icon = rect_init(ir->x, ir->y, ir->h, ir->h);
      *r_text = rect_init(ir->x + ir->h + spacer, ir->y, ir->w - ir->h - spacer, ir->h);
      break;
    }
  }

  return RET_OK;
}

const char* widget_get_bidi(widget_t* widget) {
  value_t v;
  if (widget_get_prop(widget, WIDGET_PROP_BIDI, &v) == RET_OK) {
    return value_str(&v);
  }

  return NULL;
}

ret_t widget_draw_icon_text(widget_t* widget, canvas_t* c, const char* icon, wstr_t* text) {
  rect_t ir;
  wh_t w = 0;
  wh_t h = 0;
  bitmap_t img;
  rect_t r_icon;
  rect_t r_text;
  int32_t margin = 0;
  int32_t spacer = 0;
  int32_t icon_at = 0;
  uint16_t font_size = 0;
  float_t text_size = 0.0f;
  int32_t margin_left = 0;
  int32_t margin_right = 0;
  int32_t margin_top = 0;
  int32_t margin_bottom = 0;
  style_t* style = widget->astyle;
  int32_t align_h = ALIGN_H_LEFT;
  int32_t align_v = ALIGN_V_MIDDLE;
  return_value_if_fail(widget->astyle != NULL, RET_BAD_PARAMS);

  spacer = style_get_int(style, STYLE_ID_SPACER, 2);
  margin = style_get_int(style, STYLE_ID_MARGIN, 0);
  margin_top = style_get_int(style, STYLE_ID_MARGIN_TOP, margin);
  margin_left = style_get_int(style, STYLE_ID_MARGIN_LEFT, margin);
  margin_right = style_get_int(style, STYLE_ID_MARGIN_RIGHT, margin);
  margin_bottom = style_get_int(style, STYLE_ID_MARGIN_BOTTOM, margin);
  icon_at = style_get_int(style, STYLE_ID_ICON_AT, ICON_AT_AUTO);

  w = widget->w - margin_left - margin_right;
  h = widget->h - margin_top - margin_bottom;
  ir = rect_init(margin_left, margin_top, w, h);

  if (text == NULL) {
    text = &(widget->text);
  }

  if (icon == NULL) {
    icon = style_get_str(style, STYLE_ID_ICON, NULL);
  }

  widget_prepare_text_style(widget, c);

  font_size = c->font_size;
  text_size = text->str ? canvas_measure_text(c, text->str, text->size) : 0;
  if (icon_at == ICON_AT_RIGHT || icon_at == ICON_AT_LEFT) {
    align_v = style_get_int(style, STYLE_ID_TEXT_ALIGN_V, ALIGN_V_MIDDLE);
    align_h = style_get_int(style, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_LEFT);
  } else {
    align_v = style_get_int(style, STYLE_ID_TEXT_ALIGN_V, ALIGN_V_MIDDLE);
    align_h = style_get_int(style, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_CENTER);
  }
  canvas_set_text_align(c, (align_h_t)align_h, (align_v_t)align_v);

  if (icon != NULL && widget_load_image(widget, icon, &img) == RET_OK) {
    float_t dpr = system_info()->device_pixel_ratio;

    if (text->size > 0) {
      if ((h > (img.h / dpr + font_size) && icon_at == ICON_AT_AUTO)) {
        icon_at = ICON_AT_TOP;
      }

      widget_calc_icon_text_rect(&ir, font_size, text_size, icon_at, img.w, img.h, spacer, &r_text,
                                 &r_icon);

      canvas_draw_icon_in_rect(c, &img, &r_icon);
      widget_draw_text_in_rect(widget, c, text->str, text->size, &r_text, FALSE);
    } else {
      if (icon_at == ICON_AT_AUTO) {
        widget_calc_icon_text_rect(&ir, font_size, text_size, icon_at, img.w, img.h, spacer, NULL,
                                   &r_icon);
      } else {
        widget_calc_icon_text_rect(&ir, font_size, text_size, icon_at, img.w, img.h, spacer,
                                   &r_text, &r_icon);
      }
      canvas_draw_icon_in_rect(c, &img, &r_icon);
    }
  } else if (text != NULL && text->size > 0) {
    widget_calc_icon_text_rect(&ir, font_size, text_size, icon_at, 0, 0, spacer, &r_text, NULL);
    widget_draw_text_in_rect(widget, c, text->str, text->size, &r_text, FALSE);
  }

  return RET_OK;
}

ret_t widget_fill_rect(widget_t* widget, canvas_t* c, rect_t* r, bool_t bg,
                       image_draw_type_t draw_type) {
  bitmap_t img;
  ret_t ret = RET_OK;
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  uint32_t radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);
  rect_t bg_r = rect_init(widget->x, widget->y, widget->w, widget->h);
  const char* color_key = bg ? STYLE_ID_BG_COLOR : STYLE_ID_FG_COLOR;
  const char* image_key = bg ? STYLE_ID_BG_IMAGE : STYLE_ID_FG_IMAGE;
  const char* draw_type_key = bg ? STYLE_ID_BG_IMAGE_DRAW_TYPE : STYLE_ID_FG_IMAGE_DRAW_TYPE;

  color_t color = style_get_color(style, color_key, trans);
  const char* image_name = style_get_str(style, image_key, NULL);

  if (color.rgba.a && r->w > 0 && r->h > 0) {
    canvas_set_fill_color(c, color);
    if (radius > 3) {
      if (bg) {
        ret = canvas_fill_rounded_rect(c, r, NULL, &color, radius);
      } else {
        ret = canvas_fill_rounded_rect(c, r, &bg_r, &color, radius);
      }
      if (ret == RET_FAIL) {
        canvas_fill_rect(c, r->x, r->y, r->w, r->h);
      }
    } else {
      canvas_fill_rect(c, r->x, r->y, r->w, r->h);
    }
  }

  if (image_name != NULL && *image_name && r->w > 0 && r->h > 0) {
    if (widget_load_image(widget, image_name, &img) == RET_OK) {
      draw_type = (image_draw_type_t)style_get_int(style, draw_type_key, draw_type);
      canvas_draw_image_ex(c, &img, draw_type, r);
    }
  }

  return RET_OK;
}

static inline ret_t widget_stroke_border_rect_for_border_type(canvas_t* c, rect_t* r, color_t bd,
                                                              int32_t border,
                                                              uint32_t border_width) {
  wh_t w = r->w;
  wh_t h = r->h;
  xy_t x = r->x + 0.5;
  xy_t y = r->y + 0.5;
  xy_t y1 = y;
  wh_t h1 = h;
  bool_t draw_top = FALSE;
  bool_t draw_bottom = FALSE;
  canvas_set_fill_color(c, bd);
  if (border & BORDER_TOP) {
    draw_top = TRUE;
    canvas_fill_rect(c, x, y, w, border_width);
  }
  if (border & BORDER_BOTTOM) {
    draw_bottom = TRUE;
    canvas_fill_rect(c, x, y + h - border_width, w, border_width);
  }
  /* 减少重复绘制的部分，可以修复有透明的时候重叠区域显示不正常为问题 */
  if (draw_top) {
    y1 += border_width;
    h1 -= border_width;
  }
  if (draw_bottom) {
    h1 -= border_width;
  }
  if (border & BORDER_LEFT) {
    canvas_fill_rect(c, x, y1, border_width, h1);
  }
  if (border & BORDER_RIGHT) {
    canvas_fill_rect(c, x + w - border_width, y1, border_width, h1);
  }
  return RET_OK;
}

ret_t widget_stroke_border_rect(widget_t* widget, canvas_t* c, rect_t* r) {
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  color_t bd = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);
  uint32_t radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);
  int32_t border = style_get_int(style, STYLE_ID_BORDER, BORDER_ALL);
  uint32_t border_width = style_get_int(style, STYLE_ID_BORDER_WIDTH, 1);

  if (bd.rgba.a) {
    canvas_set_stroke_color(c, bd);
    if (radius > 3) {
      if (border == BORDER_ALL) {
        if (canvas_stroke_rounded_rect(c, r, NULL, &bd, radius, border_width) != RET_OK) {
          widget_stroke_border_rect_for_border_type(c, r, bd, border, border_width);
        }
      } else {
        log_warn("border != BORDER_ALL, stroke border radius > 3 not supported !");
        widget_stroke_border_rect_for_border_type(c, r, bd, border, border_width);
      }
    } else {
      widget_stroke_border_rect_for_border_type(c, r, bd, border, border_width);
    }
  }

  return RET_OK;
}

ret_t widget_draw_background(widget_t* widget, canvas_t* c) {
  rect_t r;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  r = rect_init(0, 0, widget->w, widget->h);

  return widget_fill_rect(widget, c, &r, TRUE, IMAGE_DRAW_CENTER);
}

ret_t widget_fill_bg_rect(widget_t* widget, canvas_t* c, rect_t* r, image_draw_type_t draw_type) {
  return widget_fill_rect(widget, c, r, TRUE, draw_type);
}

ret_t widget_fill_fg_rect(widget_t* widget, canvas_t* c, rect_t* r, image_draw_type_t draw_type) {
  return widget_fill_rect(widget, c, r, FALSE, draw_type);
}

static ret_t widget_draw_border(widget_t* widget, canvas_t* c) {
  rect_t r;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  r = rect_init(0, 0, widget->w, widget->h);
  return widget_stroke_border_rect(widget, c, &r);
}

ret_t widget_paint_helper(widget_t* widget, canvas_t* c, const char* icon, wstr_t* text) {
  if (style_is_valid(widget->astyle)) {
    widget_draw_icon_text(widget, c, icon, text);
  }

  return RET_OK;
}

static ret_t widget_paint_impl(widget_t* widget, canvas_t* c) {
  int32_t ox = widget->x;
  int32_t oy = widget->y;
  uint8_t save_alpha = c->global_alpha;

  if (widget->opacity < TK_OPACITY_ALPHA) {
    canvas_set_global_alpha(c, (widget->opacity * save_alpha) / 0xff);
  }

  if (widget->astyle != NULL) {
    ox += style_get_int(widget->astyle, STYLE_ID_X_OFFSET, 0);
    oy += style_get_int(widget->astyle, STYLE_ID_Y_OFFSET, 0);
  }

  canvas_translate(c, ox, oy);
  widget_on_paint_begin(widget, c);
  widget_on_paint_background(widget, c);
  widget_on_paint_self(widget, c);
  widget_on_paint_children(widget, c);
  widget_on_paint_border(widget, c);
  widget_on_paint_end(widget, c);

  canvas_untranslate(c, ox, oy);
  if (widget->opacity < TK_OPACITY_ALPHA) {
    canvas_set_global_alpha(c, save_alpha);
  }

  widget_on_paint_done(widget, c);

  return RET_OK;
}

ret_t widget_paint(widget_t* widget, canvas_t* c) {
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  if (!widget->visible || widget->opacity <= 0x08 || widget->w <= 0 || widget->h <= 0) {
    widget->dirty = FALSE;
    return RET_OK;
  }

  if (widget->need_relayout_children) {
    widget_layout_children(widget);
  }

  if (widget->need_update_style) {
    widget_update_style(widget);
  }

  canvas_save(c);
  widget_paint_impl(widget, c);
  canvas_restore(c);

  widget->dirty = FALSE;

  return RET_OK;
}

static const widget_cmd_t s_widget_cmds[] = {{"start_animator", widget_start_animator},
                                             {"stop_animator", widget_stop_animator},
                                             {"pause_animator", widget_pause_animator},
                                             {"destroy_animator", widget_destroy_animator}};

static ret_t widget_do_exec(widget_t* widget, const char* cmd, const char* args) {
  uint32_t i = 0;

  for (i = 0; i < ARRAY_SIZE(s_widget_cmds); i++) {
    const widget_cmd_t* iter = s_widget_cmds + i;
    if (tk_str_eq(cmd, iter->name)) {
      return iter->exec(widget, args);
    }
  }

  return RET_NOT_FOUND;
}

static ret_t widget_exec(widget_t* widget, const char* str) {
  if (str != NULL) {
    char cmd[TK_NAME_LEN + 1] = {0};
    const char* args = strchr(str, ':');

    if (args != NULL) {
      return_value_if_fail((args - str) < TK_NAME_LEN, RET_BAD_PARAMS);
      tk_strncpy(cmd, str, args - str);
      args += 1;
    } else {
      return_value_if_fail(strlen(str) < TK_NAME_LEN, RET_BAD_PARAMS);
      tk_strcpy(cmd, str);
    }

    return widget_do_exec(widget, cmd, args);
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t widget_on_grabbed_keys(void* ctx, event_t* e) {
  if (e->type == EVT_KEY_DOWN || e->type == EVT_KEY_UP) {
    ret_t ret = RET_OK;
    widget_t* widget = WIDGET(ctx);
    widget_t* win = widget_get_window(widget);
    widget_t* top_win = window_manager_get_top_window(win->parent);

    if (win == top_win) {
      if (e->type == EVT_KEY_DOWN) {
        ret = widget_on_keydown(widget, (key_event_t*)e);
      } else {
        ret = widget_on_keyup(widget, (key_event_t*)e);
      }
    }

    return ret;
  }

  return RET_OK;
}

static ret_t widget_on_ungrab_keys(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  widget_t* wm = widget_get_window_manager(widget);

  widget_off_by_tag(wm, tk_pointer_to_int(widget));

  return RET_REMOVE;
}

ret_t widget_set_prop(widget_t* widget, const char* name, const value_t* v) {
  ret_t ret = RET_OK;
  prop_change_event_t e;
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_EXEC)) {
    return widget_exec(widget, value_str(v));
  }

  e.value = v;
  e.name = name;
  e.e = event_init(EVT_PROP_WILL_CHANGE, widget);
  widget_dispatch(widget, (event_t*)&e);

  if (tk_str_eq(name, WIDGET_PROP_X)) {
    widget->x = (wh_t)value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_Y)) {
    widget->y = (wh_t)value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_W)) {
    widget->w = (wh_t)value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_H)) {
    widget->h = (wh_t)value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_OPACITY)) {
    widget->opacity = (uint8_t)value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_VISIBLE)) {
    widget->visible = value_bool(v);
  } else if (tk_str_eq(name, WIDGET_PROP_SENSITIVE)) {
    widget->sensitive = value_bool(v);
  } else if (tk_str_eq(name, WIDGET_PROP_FLOATING)) {
    widget->floating = value_bool(v);
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUSABLE)) {
    widget->focusable = value_bool(v);
  } else if (tk_str_eq(name, WIDGET_PROP_WITH_FOCUS_STATE)) {
    widget->with_focus_state = value_bool(v);
  } else if (tk_str_eq(name, WIDGET_PROP_DIRTY_RECT_TOLERANCE)) {
    widget->dirty_rect_tolerance = value_int(v);
  } else if (tk_str_eq(name, WIDGET_PROP_STYLE)) {
    const char* name = value_str(v);
    return widget_use_style(widget, name);
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE)) {
    widget_set_enable(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_FEEDBACK)) {
    widget->feedback = value_bool(v);
  } else if (tk_str_eq(name, WIDGET_PROP_NAME)) {
    widget_set_name(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_TR_TEXT)) {
    widget_set_tr_text(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ANIMATION)) {
    widget_set_animation(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_SELF_LAYOUT)) {
    widget_set_self_layout(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_LAYOUT) || tk_str_eq(name, WIDGET_PROP_CHILDREN_LAYOUT)) {
    widget_set_children_layout(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_POINTER_CURSOR)) {
    widget_set_pointer_cursor(widget, value_str(v));
  } else {
    ret = RET_NOT_FOUND;
  }

  if (widget->vt->set_prop) {
    ret_t ret1 = widget->vt->set_prop(widget, name, v);
    if (ret == RET_NOT_FOUND) {
      ret = ret1;
    }
  }

  if (ret == RET_NOT_FOUND) {
    if (tk_str_eq(name, WIDGET_PROP_FOCUSED) || tk_str_eq(name, WIDGET_PROP_FOCUS)) {
      widget_set_focused(widget, value_bool(v));
      ret = RET_OK;
    } else if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
      wstr_from_value(&(widget->text), v);
      ret = RET_OK;
    } else if (tk_str_start_with(name, "style:")) {
      return widget_set_style(widget, name + 6, v);
    } else {
      if (widget->custom_props == NULL) {
        widget->custom_props = object_default_create();
      }

      if (tk_str_eq(name, WIDGET_PROP_GRAB_KEYS)) {
        widget_t* wm = widget_get_window_manager(widget);

        if (value_bool(v)) {
          widget_on(widget, EVT_DESTROY, widget_on_ungrab_keys, widget);
          widget_on_with_tag(wm, EVT_KEY_DOWN, widget_on_grabbed_keys, widget,
                             tk_pointer_to_int(widget));
          widget_on_with_tag(wm, EVT_KEY_UP, widget_on_grabbed_keys, widget,
                             tk_pointer_to_int(widget));
        }
      }

      ret = object_set_prop(widget->custom_props, name, v);
    }
  }

  if (ret != RET_NOT_FOUND) {
    e.e.type = EVT_PROP_CHANGED;
    widget_dispatch(widget, (event_t*)&e);
    widget_invalidate(widget, NULL);
  }

  return ret;
}

ret_t widget_get_prop(widget_t* widget, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_X)) {
    value_set_int32(v, widget->x);
  } else if (tk_str_eq(name, WIDGET_PROP_Y)) {
    value_set_int32(v, widget->y);
  } else if (tk_str_eq(name, WIDGET_PROP_W)) {
    value_set_int32(v, widget->w);
  } else if (tk_str_eq(name, WIDGET_PROP_H)) {
    value_set_int32(v, widget->h);
  } else if (tk_str_eq(name, WIDGET_PROP_OPACITY)) {
    value_set_int32(v, widget->opacity);
  } else if (tk_str_eq(name, WIDGET_PROP_VISIBLE)) {
    value_set_bool(v, widget->visible);
  } else if (tk_str_eq(name, WIDGET_PROP_SENSITIVE)) {
    value_set_bool(v, widget->sensitive);
  } else if (tk_str_eq(name, WIDGET_PROP_FLOATING)) {
    value_set_bool(v, widget->floating);
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUSABLE)) {
    value_set_bool(v, widget_is_focusable(widget));
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUSED)) {
    value_set_bool(v, widget->focused);
  } else if (tk_str_eq(name, WIDGET_PROP_WITH_FOCUS_STATE)) {
    value_set_bool(v, widget->with_focus_state);
  } else if (tk_str_eq(name, WIDGET_PROP_DIRTY_RECT_TOLERANCE)) {
    value_set_int(v, widget->dirty_rect_tolerance);
  } else if (tk_str_eq(name, WIDGET_PROP_STYLE)) {
    value_set_str(v, widget->style);
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE)) {
    value_set_bool(v, widget->enable);
  } else if (tk_str_eq(name, WIDGET_PROP_FEEDBACK)) {
    value_set_bool(v, widget->feedback);
  } else if (tk_str_eq(name, WIDGET_PROP_NAME)) {
    value_set_str(v, widget->name);
  } else if (tk_str_eq(name, WIDGET_PROP_ANIMATION)) {
    value_set_str(v, widget->animation);
  } else if (tk_str_eq(name, WIDGET_PROP_POINTER_CURSOR)) {
    value_set_str(v, widget->pointer_cursor);
  } else if (tk_str_eq(name, WIDGET_PROP_SELF_LAYOUT)) {
    if (widget->self_layout != NULL) {
      value_set_str(v, self_layouter_to_string(widget->self_layout));
    } else {
      ret = RET_NOT_FOUND;
    }
  } else if (tk_str_eq(name, WIDGET_PROP_CHILDREN_LAYOUT)) {
    if (widget->children_layout != NULL) {
      value_set_str(v, children_layouter_to_string(widget->children_layout));
    } else {
      ret = RET_NOT_FOUND;
    }
  } else {
    if (widget->vt->get_prop) {
      ret = widget->vt->get_prop(widget, name, v);
    } else {
      ret = RET_NOT_FOUND;
    }
  }

  /*default*/
  if (ret == RET_NOT_FOUND) {
    if (tk_str_eq(name, WIDGET_PROP_LAYOUT_W)) {
      value_set_int32(v, widget->w);
      ret = RET_OK;
    } else if (tk_str_eq(name, WIDGET_PROP_LAYOUT_H)) {
      value_set_int32(v, widget->h);
      ret = RET_OK;
    } else if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
      wchar_t* text = widget->text.str;
      if (text != NULL) {
        text[widget->text.size] = 0;
      }
      value_set_wstr(v, text);
      ret = RET_OK;
    } else if (tk_str_eq(name, WIDGET_PROP_STATE_FOR_STYLE)) {
      value_set_str(v, widget_get_state_for_style(widget, FALSE, FALSE));
      ret = RET_OK;
    }
  }

  if (ret == RET_NOT_FOUND) {
    if (widget->custom_props != NULL) {
      ret = object_get_prop(widget->custom_props, name, v);
    }
  }

  if (ret == RET_NOT_FOUND) {
    if (tk_str_eq(name, WIDGET_PROP_TYPE)) {
      value_set_str(v, widget->vt->type);
      ret = RET_OK;
    }
  }

  return ret;
}

ret_t widget_set_prop_str(widget_t* widget, const char* name, const char* str) {
  value_t v;
  value_set_str(&v, str);

  return widget_set_prop(widget, name, &v);
}

const char* widget_get_prop_str(widget_t* widget, const char* name, const char* defval) {
  value_t v;
  return_value_if_fail(widget_get_prop(widget, name, &v) == RET_OK, defval);

  return value_str(&v);
}

ret_t widget_set_prop_pointer(widget_t* widget, const char* name, void* pointer) {
  value_t v;
  value_set_pointer(&v, pointer);

  return widget_set_prop(widget, name, &v);
}

void* widget_get_prop_pointer(widget_t* widget, const char* name) {
  value_t v;
  if (widget_get_prop(widget, name, &v) == RET_OK) {
    return value_pointer(&v);
  } else {
    return NULL;
  }
}

ret_t widget_set_prop_int(widget_t* widget, const char* name, int32_t num) {
  value_t v;
  value_set_int(&v, num);

  return widget_set_prop(widget, name, &v);
}

int32_t widget_get_prop_int(widget_t* widget, const char* name, int32_t defval) {
  value_t v;
  if (widget_get_prop(widget, name, &v) == RET_OK) {
    return value_int(&v);
  } else {
    return defval;
  }
}

ret_t widget_set_prop_bool(widget_t* widget, const char* name, bool_t num) {
  value_t v;
  value_set_bool(&v, num);

  return widget_set_prop(widget, name, &v);
}

bool_t widget_get_prop_bool(widget_t* widget, const char* name, bool_t defval) {
  value_t v;
  return_value_if_fail(widget_get_prop(widget, name, &v) == RET_OK, defval);

  return value_bool(&v);
}

ret_t widget_on_paint_background(widget_t* widget, canvas_t* c) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_background) {
    ret = widget->vt->on_paint_background(widget, c);
  } else {
    if (style_is_valid(widget->astyle)) {
      widget_draw_background(widget, c);
    }
  }

  return ret;
}

ret_t widget_on_paint_self(widget_t* widget, canvas_t* c) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_self) {
    ret = widget->vt->on_paint_self(widget, c);
  } else {
    paint_event_t e;
    widget_dispatch(widget, paint_event_init(&e, EVT_PAINT, widget, c));
  }

  return ret;
}

ret_t widget_on_paint_children(widget_t* widget, canvas_t* c) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_children) {
    ret = widget->vt->on_paint_children(widget, c);
  } else {
    ret = widget_on_paint_children_default(widget, c);
  }

  return ret;
}

ret_t widget_on_paint_border(widget_t* widget, canvas_t* c) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_border) {
    ret = widget->vt->on_paint_border(widget, c);
  } else {
    if (style_is_valid(widget->astyle)) {
      ret = widget_draw_border(widget, c);
    }
  }

  return ret;
}

ret_t widget_on_paint_begin(widget_t* widget, canvas_t* c) {
  paint_event_t e;
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_begin) {
    ret = widget->vt->on_paint_begin(widget, c);
  }

  widget_dispatch(widget, paint_event_init(&e, EVT_BEFORE_PAINT, widget, c));

  return ret;
}

static ret_t widget_on_paint_done(widget_t* widget, canvas_t* c) {
  paint_event_t e;
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget_dispatch(widget, paint_event_init(&e, EVT_PAINT_DONE, widget, c));

  return ret;
}

ret_t widget_on_paint_end(widget_t* widget, canvas_t* c) {
  paint_event_t e;
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->vt->on_paint_end) {
    ret = widget->vt->on_paint_end(widget, c);
  }

  widget_dispatch(widget, paint_event_init(&e, EVT_AFTER_PAINT, widget, c));

  return ret;
}

ret_t widget_dispatch_to_target(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  e->target = widget;
  widget_dispatch(widget, e);

  if (widget->target) {
    ret = widget_dispatch_to_target(widget->target, e);
  }

  return ret;
}

static ret_t widget_map_key(widget_t* widget, key_event_t* e) {
  value_t v;
  const key_type_value_t* kv = NULL;

  if (widget->custom_props != NULL) {
    kv = keys_type_find_by_value(e->key);
    if (kv != NULL) {
      const char* to = NULL;
      char from[TK_NAME_LEN + 1] = {0};
      char fixed_name[TK_NAME_LEN + 1];

      tk_snprintf(from, sizeof(from), "map_key:%s", kv->name);
      if (object_get_prop(widget->custom_props, from, &v) == RET_OK) {
        to = value_str(&v);
      } else if (strlen(kv->name) > 1) {
        tk_strcpy(fixed_name, kv->name);
        tk_str_tolower(fixed_name);
        tk_snprintf(from, sizeof(from), "map_key:%s", fixed_name);
        if (object_get_prop(widget->custom_props, from, &v) == RET_OK) {
          to = value_str(&v);
        }
      }

      if (to != NULL) {
        kv = keys_type_find(to);
        if (kv != NULL) {
          e->key = kv->value;
          log_debug("map key %s to %s\n", from, to);
        }
      }
    }
  }

  return RET_OK;
}

ret_t widget_dispatch_to_key_target(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  e->target = widget;
  widget_dispatch(widget, e);

  if (widget->key_target) {
    ret = widget_dispatch_to_target(widget->key_target, e);
  }

  return ret;
}

static ret_t widget_on_keydown_before_children(widget_t* widget, key_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  if (widget->emitter != NULL) {
    key_event_t before = *e;
    before.e.type = EVT_KEY_DOWN_BEFORE_CHILDREN;
    return_if_equal(emitter_dispatch(widget->emitter, (event_t*)&(before)), RET_STOP);
  }

  return widget_on_event_before_children(widget, (event_t*)e);
}

static ret_t widget_on_keydown_children(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;

  if (widget->key_target != NULL) {
    ret = widget_on_keydown(widget->key_target, e);
  }

  return ret;
}

static ret_t widget_on_keydown_after_children(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;

  return_if_equal(ret = widget_dispatch(widget, (event_t*)e), RET_STOP);
  if (widget->vt->on_keydown) {
    ret = widget->vt->on_keydown(widget, e);
  }

  return ret;
}

bool_t widget_is_activate_key(widget_t* widget, key_event_t* e) {
  return_value_if_fail(widget != NULL && widget->vt != NULL && e != NULL, FALSE);

  return (widget->vt->space_key_to_activate && e->key == TK_KEY_SPACE) ||
         (widget->vt->return_key_to_activate && key_code_is_enter(e->key));
}

static bool_t shortcut_fast_match(const char* shortcut, key_event_t* e) {
  uint32_t key = e->key;
  const char* kname = strrchr(shortcut, '+');
  bool_t cmd = strstr(shortcut, "cmd") != NULL;
  bool_t ctrl = strstr(shortcut, "ctrl") != NULL;
  bool_t shift = strstr(shortcut, "shift") != NULL;
  const key_type_value_t* kv = keys_type_find_by_value(key);

  if (kv != NULL) {
    if (kname == NULL) {
      kname = shortcut;
    } else {
      kname++;
    }

    if (tk_str_ieq(kname, kv->name) && cmd == e->cmd && ctrl == e->ctrl && shift == e->shift) {
      return TRUE;
    }
  }

  return FALSE;
}

static bool_t widget_match_key(widget_t* widget, const char* prop, key_event_t* e) {
  const char* shortcut = NULL;
  widget_t* win = widget_get_window(widget);

  if (widget_is_window_manager(widget)) {
    return FALSE;
  }

  return_value_if_fail(win != NULL, FALSE);
  shortcut = widget_get_prop_str(win, prop, NULL);

  if (shortcut != NULL) {
    return shortcut_fast_match(shortcut, e);
  }

  return FALSE;
}

static bool_t widget_is_move_focus_prev_key(widget_t* widget, key_event_t* e) {
  return widget_match_key(widget, WIDGET_PROP_MOVE_FOCUS_PREV_KEY, e);
}

static bool_t widget_is_move_focus_next_key(widget_t* widget, key_event_t* e) {
  return widget_match_key(widget, WIDGET_PROP_MOVE_FOCUS_NEXT_KEY, e);
}

static bool_t widget_is_move_focus_up_key(widget_t* widget, key_event_t* e) {
  return widget_match_key(widget, WIDGET_PROP_MOVE_FOCUS_UP_KEY, e);
}

static bool_t widget_is_move_focus_down_key(widget_t* widget, key_event_t* e) {
  return widget_match_key(widget, WIDGET_PROP_MOVE_FOCUS_DOWN_KEY, e);
}

static bool_t widget_is_move_focus_left_key(widget_t* widget, key_event_t* e) {
  return widget_match_key(widget, WIDGET_PROP_MOVE_FOCUS_LEFT_KEY, e);
}

static bool_t widget_is_move_focus_right_key(widget_t* widget, key_event_t* e) {
  return widget_match_key(widget, WIDGET_PROP_MOVE_FOCUS_RIGHT_KEY, e);
}

static ret_t widget_on_keydown_general(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;
  if (!widget_is_window_manager(widget)) {
    if (widget_is_activate_key(widget, e)) {
      ret = RET_STOP;
      widget_set_state(widget, WIDGET_STATE_PRESSED);
    } else if (widget_is_move_focus_next_key(widget, e)) {
      if (widget_is_focusable(widget)) {
        ret = RET_STOP;
        widget_focus_next(widget);
      } else if (widget_is_window(widget) && !widget_has_focused_widget_in_window(widget)) {
        ret = RET_STOP;
        widget_focus_first(widget);
      }
    } else if (widget_is_move_focus_prev_key(widget, e)) {
      if (widget_is_focusable(widget)) {
        ret = RET_STOP;
        widget_focus_prev(widget);
      } else if (widget_is_window(widget) && !widget_has_focused_widget_in_window(widget)) {
        ret = RET_STOP;
        widget_focus_first(widget);
      }
    } else if (widget_is_move_focus_up_key(widget, e)) {
      if (widget_is_focusable(widget)) {
        ret = RET_STOP;
        widget_focus_up(widget);
      }
    } else if (widget_is_move_focus_down_key(widget, e)) {
      if (widget_is_focusable(widget)) {
        ret = RET_STOP;
        widget_focus_down(widget);
      }
    } else if (widget_is_move_focus_left_key(widget, e)) {
      if (widget_is_focusable(widget)) {
        ret = RET_STOP;
        widget_focus_left(widget);
      }
    } else if (widget_is_move_focus_right_key(widget, e)) {
      if (widget_is_focusable(widget)) {
        ret = RET_STOP;
        widget_focus_right(widget);
      }
    }
  }

  return ret;
}

static ret_t widget_on_keydown_impl(widget_t* widget, key_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  return_if_equal(widget_on_keydown_before_children(widget, e), RET_STOP);
  return_if_equal(widget_on_keydown_children(widget, e), RET_STOP);
  return_if_equal(widget_on_keydown_after_children(widget, e), RET_STOP);

  return RET_OK;
}

ret_t widget_on_keydown(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;
  uint32_t key = e->key;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  widget_ref(widget);
  widget_map_key(widget, e);
  ret = widget_on_keydown_impl(widget, e);
  if (widget->feedback) {
    ui_feedback_request(widget, (event_t*)e);
  }

  e->key = key;
  if (ret != RET_STOP) {
    ret = widget_on_keydown_general(widget, e);
  }
  widget_unref(widget);

  return ret;
}

static ret_t widget_on_keyup_before_children(widget_t* widget, key_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  if (widget->emitter != NULL) {
    key_event_t before = *e;
    before.e.type = EVT_KEY_UP_BEFORE_CHILDREN;
    return_if_equal(emitter_dispatch(widget->emitter, (event_t*)&(before)), RET_STOP);
  }

  return widget_on_event_before_children(widget, (event_t*)e);
}

static ret_t widget_on_keyup_children(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;

  if (widget->key_target != NULL) {
    ret = widget_on_keyup(widget->key_target, e);
  }

  return ret;
}

static ret_t widget_on_keyup_after_children(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;

  return_if_equal(ret = widget_dispatch(widget, (event_t*)e), RET_STOP);
  if (widget->vt->on_keyup) {
    ret = widget->vt->on_keyup(widget, e);
  }

  return ret;
}

static ret_t widget_on_keyup_impl(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  return_if_equal(widget_on_keyup_before_children(widget, e), RET_STOP);
  return_if_equal(widget_on_keyup_children(widget, e), RET_STOP);
  return_if_equal(widget_on_keyup_after_children(widget, e), RET_STOP);

  if (widget_is_activate_key(widget, e)) {
    pointer_event_t click;
    if (widget_is_focusable(widget)) {
      widget_set_state(widget, WIDGET_STATE_FOCUSED);
    } else {
      widget_set_state(widget, WIDGET_STATE_NORMAL);
    }
    widget_dispatch(widget, pointer_event_init(&click, EVT_CLICK, widget, 0, 0));

    ret = RET_STOP;
  } else if (widget_is_move_focus_next_key(widget, e)) {
    if (widget_is_focusable(widget)) {
      ret = RET_STOP;
    } else if (widget_is_window(widget) && !widget_has_focused_widget_in_window(widget)) {
      ret = RET_STOP;
    }
  } else if (widget_is_move_focus_prev_key(widget, e)) {
    if (widget_is_focusable(widget)) {
      ret = RET_STOP;
    } else if (widget_is_window(widget) && !widget_has_focused_widget_in_window(widget)) {
      ret = RET_STOP;
    }
  } else if (widget_is_move_focus_up_key(widget, e)) {
    if (widget_is_focusable(widget)) {
      ret = RET_STOP;
    }
  } else if (widget_is_move_focus_down_key(widget, e)) {
    if (widget_is_focusable(widget)) {
      ret = RET_STOP;
    }
  } else if (widget_is_move_focus_left_key(widget, e)) {
    if (widget_is_focusable(widget)) {
      ret = RET_STOP;
    }
  } else if (widget_is_move_focus_right_key(widget, e)) {
    if (widget_is_focusable(widget)) {
      ret = RET_STOP;
    }
  }

  return ret;
}

ret_t widget_on_keyup(widget_t* widget, key_event_t* e) {
  ret_t ret = RET_OK;
  uint32_t key = e->key;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  widget_ref(widget);
  widget_map_key(widget, e);
  ret = widget_on_keyup_impl(widget, e);
  if (widget->feedback) {
    ui_feedback_request(widget, (event_t*)e);
  }
  widget_unref(widget);

  e->key = key;

  return ret;
}

static ret_t widget_on_wheel_before_children(widget_t* widget, wheel_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  if (widget->emitter != NULL) {
    wheel_event_t before = *e;
    before.e.type = EVT_WHEEL_BEFORE_CHILDREN;
    return_if_equal(emitter_dispatch(widget->emitter, (event_t*)&(before)), RET_STOP);
  }

  return widget_on_event_before_children(widget, (event_t*)e);
}

static ret_t widget_on_wheel_children(widget_t* widget, wheel_event_t* e) {
  ret_t ret = RET_OK;

  if (widget->key_target != NULL) {
    ret = widget_on_wheel(widget->key_target, e);
  }

  return ret;
}

static ret_t widget_on_wheel_after_children(widget_t* widget, wheel_event_t* e) {
  ret_t ret = RET_OK;

  return_if_equal(ret = widget_dispatch(widget, (event_t*)e), RET_STOP);
  if (widget->vt->on_wheel) {
    ret = widget->vt->on_wheel(widget, e);
  }

  return ret;
}

static ret_t widget_on_wheel_impl(widget_t* widget, wheel_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  return_if_equal(widget_on_wheel_before_children(widget, e), RET_STOP);
  return_if_equal(widget_on_wheel_children(widget, e), RET_STOP);
  return_if_equal(widget_on_wheel_after_children(widget, e), RET_STOP);

  return RET_OK;
}

ret_t widget_on_wheel(widget_t* widget, wheel_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  widget_ref(widget);
  ret = widget_on_wheel_impl(widget, e);
  widget_unref(widget);

  return ret;
}

static ret_t widget_dispatch_leave_event(widget_t* widget, pointer_event_t* e) {
  widget_t* target = widget;

  while (target != NULL) {
    widget_t* curr = target;
    pointer_event_t leave = *e;
    leave.e.type = EVT_POINTER_LEAVE;

    widget_dispatch(target, (event_t*)(&leave));
    target = curr->target;
    curr->target = NULL;
  }

  return RET_OK;
}

static ret_t widget_dispatch_blur_event(widget_t* widget) {
  widget_t* target = widget;
  widget_t* temp;

  while (target != NULL) {
    widget_ref(target);
    if (target->focused) {
      target->focused = FALSE;
      event_t e = event_init(EVT_BLUR, target);
      widget_dispatch(target, &e);
      widget_set_need_update_style(target);
    }

    if (target->parent && target->parent->key_target == target) {
      target->parent->key_target = NULL;
    }

    temp = target->key_target;
    widget_unref(target);
    target = temp;
  }

  return RET_OK;
}

ret_t widget_dispatch_event_to_target_recursive(widget_t* widget, event_t* e) {
  widget_t* target = NULL;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  target = widget->grab_widget ? widget->grab_widget : widget->target;
  while (target != NULL) {
    widget_dispatch(target, e);
    target = target->target != NULL ? target->target : target->key_target;
  }

  return RET_OK;
}

static ret_t widget_on_pointer_down_before_children(widget_t* widget, pointer_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  if (widget->emitter != NULL) {
    pointer_event_t before = *e;
    before.e.type = EVT_POINTER_DOWN_BEFORE_CHILDREN;
    return_if_equal(emitter_dispatch(widget->emitter, (event_t*)&(before)), RET_STOP);
  }

  return widget_on_event_before_children(widget, (event_t*)e);
}

static ret_t widget_on_pointer_down_children(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  widget_t* target = widget_find_target(widget, e->x, e->y);

  if (target != NULL && target->enable) {
    if (!(widget_is_keyboard(target))) {
      if (!target->focused) {
        widget_set_focused_internal(target, TRUE);
      } else {
        widget->key_target = target;
      }
    }
  } else if (widget->key_target) {
    widget_set_focused_internal(widget->key_target, FALSE);
  }
  return_if_equal(ret, RET_STOP);

  if (widget->target != target) {
    if (widget->target != NULL) {
      widget_dispatch_leave_event(widget->target, e);
    }
    widget->target = target;
  }

  if (widget->target != NULL) {
    ret = widget_on_pointer_down(widget->target, e);
  }

  return ret;
}

static ret_t widget_on_pointer_down_after_children(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;

  return_if_equal(ret = widget_dispatch(widget, (event_t*)e), RET_STOP);
  if (widget->vt->on_pointer_down) {
    return_if_equal(ret = widget->vt->on_pointer_down(widget, e), RET_STOP);
  }

  return ret;
}

static ret_t widget_on_pointer_down_impl(widget_t* widget, pointer_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  widget->grab_widget = NULL;
  widget->grab_widget_count = 0;
  return_if_equal(widget_on_pointer_down_before_children(widget, e), RET_STOP);
  return_if_equal(widget_on_pointer_down_children(widget, e), RET_STOP);
  return_if_equal(widget_on_pointer_down_after_children(widget, e), RET_STOP);

  return RET_OK;
}

ret_t widget_on_pointer_down(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  widget_ref(widget);
  ret = widget_on_pointer_down_impl(widget, e);
  if (widget->feedback) {
    ui_feedback_request(widget, (event_t*)e);
  }
  widget_unref(widget);

  return ret;
}

static ret_t widget_on_pointer_move_before_children(widget_t* widget, pointer_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  if (widget->emitter != NULL) {
    pointer_event_t before = *e;
    before.e.type = EVT_POINTER_MOVE_BEFORE_CHILDREN;
    return_if_equal(emitter_dispatch(widget->emitter, (event_t*)&(before)), RET_STOP);
  }

  return widget_on_event_before_children(widget, (event_t*)e);
}

static ret_t widget_on_pointer_move_children(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  widget_t* target = widget_find_target(widget, e->x, e->y);

  if (target != widget->target) {
    if (widget->target != NULL) {
      widget_dispatch_leave_event(widget->target, e);
    }

    if (target != NULL) {
      pointer_event_t enter = *e;
      enter.e.type = EVT_POINTER_ENTER;
      ret = widget_dispatch(target, (event_t*)(&enter));
      widget_update_pointer_cursor(target);
    } else {
      widget_update_pointer_cursor(widget);
    }

    widget->target = target;
  }
  return_if_equal(ret, RET_STOP);

  if (widget->target != NULL) {
    ret = widget_on_pointer_move(widget->target, e);
  }

  return ret;
}

static ret_t widget_on_pointer_move_after_children(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;

  return_if_equal(ret = widget_dispatch(widget, (event_t*)e), RET_STOP);
  if (widget->vt->on_pointer_move) {
    return_if_equal(ret = widget->vt->on_pointer_move(widget, e), RET_STOP);
  }

  return ret;
}

static ret_t widget_on_pointer_move_impl(widget_t* widget, pointer_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  return_if_equal(widget_on_pointer_move_before_children(widget, e), RET_STOP);
  if (widget_on_pointer_move_children(widget, e) == RET_STOP) {
    if (e->pressed) {
      pointer_event_t abort;
      pointer_event_init(&abort, EVT_POINTER_DOWN_ABORT, widget, e->x, e->y);
      return_if_equal(widget_on_pointer_move_after_children(widget, &abort), RET_STOP);
    }

    return RET_STOP;
  } else {
    return widget_on_pointer_move_after_children(widget, e);
  }
}

ret_t widget_on_pointer_move(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  widget_ref(widget);
  ret = widget_on_pointer_move_impl(widget, e);
  widget_unref(widget);

  return ret;
}

static ret_t widget_on_pointer_up_before_children(widget_t* widget, pointer_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  if (widget->emitter != NULL) {
    pointer_event_t before = *e;
    before.e.type = EVT_POINTER_UP_BEFORE_CHILDREN;
    return_if_equal(emitter_dispatch(widget->emitter, (event_t*)&(before)), RET_STOP);
  }

  return widget_on_event_before_children(widget, (event_t*)e);
}

static ret_t widget_on_pointer_up_children(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;

  widget_t* target = widget_find_target(widget, e->x, e->y);
  if (target != NULL) {
    ret = widget_on_pointer_up(target, e);
  }

  return ret;
}

static ret_t widget_on_pointer_up_after_children(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;

  return_if_equal(ret = widget_dispatch(widget, (event_t*)e), RET_STOP);
  if (widget->vt->on_pointer_up) {
    return_if_equal(ret = widget->vt->on_pointer_up(widget, e), RET_STOP);
  }

  return ret;
}

static ret_t widget_on_pointer_up_impl(widget_t* widget, pointer_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  return_if_equal(widget_on_pointer_up_before_children(widget, e), RET_STOP);
  if (widget_on_pointer_up_children(widget, e) == RET_STOP) {
    if (e->pressed) {
      pointer_event_t abort;
      pointer_event_init(&abort, EVT_POINTER_DOWN_ABORT, widget, e->x, e->y);
      return_if_equal(widget_on_pointer_up_after_children(widget, &abort), RET_STOP);
    }

    return RET_STOP;
  } else {
    return widget_on_pointer_up_after_children(widget, e);
  }
}

ret_t widget_on_pointer_up(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  widget_ref(widget);
  ret = widget_on_pointer_up_impl(widget, e);
  if (widget->feedback) {
    ui_feedback_request(widget, (event_t*)e);
  }
  widget_unref(widget);

  return ret;
}

static ret_t widget_on_context_menu_children(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;

  widget_t* target = widget_find_target(widget, e->x, e->y);
  if (target != NULL) {
    ret = widget_on_context_menu(target, e);
  }

  return ret;
}

static ret_t widget_on_context_menu_after_children(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;

  return_if_equal(ret = widget_dispatch(widget, (event_t*)e), RET_STOP);
  if (widget->vt->on_context_menu) {
    return_if_equal(ret = widget->vt->on_context_menu(widget, e), RET_STOP);
  }

  return ret;
}

static ret_t widget_on_context_menu_impl(widget_t* widget, pointer_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (widget_on_context_menu_children(widget, e) == RET_STOP) {
    return RET_STOP;
  } else {
    return widget_on_context_menu_after_children(widget, e);
  }
}

ret_t widget_on_context_menu(widget_t* widget, pointer_event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  widget_ref(widget);
  ret = widget_on_context_menu_impl(widget, e);
  widget_unref(widget);

  return ret;
}

ret_t widget_grab(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL && widget->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->grab_widget == NULL || widget->grab_widget == child, RET_BAD_PARAMS);

  if (widget->grab_widget == NULL) {
    widget->grab_widget = child;
    widget->grab_widget_count = 1;
  } else {
    widget->grab_widget_count++;
  }

  if (widget->parent) {
    widget_grab(widget->parent, widget);
  }

  return RET_OK;
}

ret_t widget_ungrab(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && widget->vt != NULL, RET_BAD_PARAMS);

  if (widget->grab_widget == child) {
    if (widget->grab_widget->grab_widget_count < widget->grab_widget_count) {
      widget->grab_widget_count--;
      if (widget->grab_widget_count <= 0) {
        widget->grab_widget = NULL;
        widget->grab_widget_count = 0;
      }

      if (widget->parent) {
        widget_ungrab(widget->parent, widget);
      }
    }
  }

  return RET_OK;
}

ret_t widget_foreach(widget_t* widget, tk_visit_t visit, void* ctx) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && visit != NULL, RET_BAD_PARAMS);

  ret = visit(ctx, widget);
  if (ret != RET_OK) {
    return ret;
  }

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  ret = widget_foreach(iter, visit, ctx);
  if (ret == RET_STOP || ret == RET_DONE) {
    return ret;
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

widget_t* widget_get_window(widget_t* widget) {
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL, NULL);

  while (iter) {
    if (widget_is_window(iter)) {
      return iter;
    }
    iter = iter->parent;
  }

  return NULL;
}

static widget_t* widget_get_window_or_keyboard(widget_t* widget) {
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL, NULL);

  while (iter) {
    if (widget_is_window(iter) || widget_is_keyboard(iter)) {
      return iter;
    }
    iter = iter->parent;
  }

  return NULL;
}

widget_t* widget_get_window_manager(widget_t* widget) {
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL, NULL);

  while (iter) {
    if (widget_is_window_manager(iter)) {
      return iter;
    }
    iter = iter->parent;
  }

  return window_manager();
}

static ret_t widget_remove_timer_on_destroy(void* ctx, event_t* e) {
  uint32_t id = (char*)ctx - (char*)NULL;
  timer_remove(id);

  return RET_REMOVE;
}

uint32_t widget_add_timer(widget_t* widget, timer_func_t on_timer, uint32_t duration_ms) {
  uint32_t id = 0;
  return_value_if_fail(widget != NULL && on_timer != NULL, TK_INVALID_ID);

  id = timer_add(on_timer, widget, duration_ms);
  widget_on(widget, EVT_DESTROY, widget_remove_timer_on_destroy, tk_pointer_from_int(id));

  return id;
}

ret_t widget_remove_timer(widget_t* widget, uint32_t timer_id) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget_off_by_func(widget, EVT_DESTROY, widget_remove_timer_on_destroy,
                         tk_pointer_from_int(timer_id)) == RET_OK) {
    return timer_remove(timer_id);
  }

  return RET_BAD_PARAMS;
}

static ret_t widget_remove_idle_on_destroy(void* ctx, event_t* e) {
  uint32_t id = (char*)ctx - (char*)NULL;
  idle_remove(id);

  return RET_REMOVE;
}

uint32_t widget_add_idle(widget_t* widget, idle_func_t on_idle) {
  uint32_t id = 0;
  return_value_if_fail(widget != NULL && on_idle != NULL, TK_INVALID_ID);

  id = idle_add(on_idle, widget);
  widget_on(widget, EVT_DESTROY, widget_remove_idle_on_destroy, tk_pointer_from_int(id));

  return id;
}

static ret_t widget_destroy_sync(widget_t* widget) {
  event_t e = event_init(EVT_DESTROY, widget);
  return_value_if_fail(widget != NULL && widget->vt != NULL, RET_BAD_PARAMS);

#ifndef WITHOUT_WIDGET_ANIMATORS
  widget_destroy_animator(widget, NULL);
#endif /*WITHOUT_WIDGET_ANIMATORS*/

  widget->destroying = TRUE;
  if (widget->emitter != NULL) {
    widget_dispatch(widget, &e);
    emitter_destroy(widget->emitter);
    widget->emitter = NULL;
  }

  if (widget->children != NULL) {
    widget_destroy_children(widget);
    darray_destroy(widget->children);
    widget->children = NULL;
  }

  if (widget->children_layout != NULL) {
    children_layouter_destroy(widget->children_layout);
    widget->children_layout = NULL;
  }

  if (widget->self_layout != NULL) {
    self_layouter_destroy(widget->self_layout);
    widget->self_layout = NULL;
  }

  if (widget->custom_props != NULL) {
    object_unref(widget->custom_props);
    widget->custom_props = NULL;
  }

  wstr_reset(&(widget->text));
  style_destroy(widget->astyle);
  widget->astyle = NULL;

  widget->destroying = FALSE;

  return widget_real_destroy(widget);
}

widget_t* widget_create(widget_t* parent, const widget_vtable_t* vt, xy_t x, xy_t y, wh_t w,
                        wh_t h) {
  return_value_if_fail(vt != NULL, NULL);

  return widget_init(widget_real_create(vt), parent, vt, x, y, w, h);
}

ret_t widget_destroy(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->ref_count > 0 && widget->vt != NULL,
                       RET_BAD_PARAMS);

  if (widget->parent != NULL) {
    widget_remove_child(widget->parent, widget);
  }

  return widget_unref_async(widget);
}

static ret_t widget_destroy_on_idle(const idle_info_t* info) {
  widget_destroy(WIDGET(info->ctx));

  return RET_REMOVE;
}

ret_t widget_destroy_async(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->ref_count > 0 && widget->vt != NULL,
                       RET_BAD_PARAMS);

  return_value_if_fail(idle_add(widget_destroy_on_idle, widget) != TK_INVALID_ID, RET_FAIL);

  return RET_OK;
}

static ret_t widget_set_parent_not_dirty(widget_t* widget) {
  widget_t* iter = widget->parent;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    iter->dirty = FALSE;
    if (iter->vt->is_window) {
      break;
    }
    iter = iter->parent;
  }

  return RET_OK;
}

ret_t widget_invalidate(widget_t* widget, rect_t* r) {
  rect_t rself;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->dirty) {
    return RET_OK;
  }

  if (r == NULL) {
    rself = rect_init(0, 0, widget->w, widget->h);
    r = &rself;
  }

  widget->dirty = TRUE;
  widget_set_parent_not_dirty(widget);

  if (widget->vt && widget->vt->invalidate) {
    return widget->vt->invalidate(widget, r);
  } else {
    return widget_invalidate_default(widget, r);
  }
}

ret_t widget_invalidate_force(widget_t* widget, rect_t* r) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget->dirty = FALSE;
  return widget_invalidate(widget, NULL);
}

widget_t* widget_init(widget_t* widget, widget_t* parent, const widget_vtable_t* vt, xy_t x, xy_t y,
                      wh_t w, wh_t h) {
  return_value_if_fail(widget != NULL && vt != NULL, NULL);

  widget->x = x;
  widget->y = y;
  widget->w = w;
  widget->h = h;
  widget->vt = vt;
  widget->dirty = TRUE;
  widget->ref_count = 1;
  widget->opacity = 0xff;
  widget->enable = TRUE;
  widget->visible = TRUE;
  widget->feedback = FALSE;
  widget->sensitive = TRUE;
  widget->emitter = NULL;
  widget->children = NULL;
  widget->initializing = TRUE;
  widget->state = tk_strdup(WIDGET_STATE_NORMAL);
  widget->target = NULL;
  widget->key_target = NULL;
  widget->grab_widget = NULL;
  widget->grab_widget_count = 0;
  widget->focused = FALSE;
  widget->focusable = FALSE;
  widget->with_focus_state = FALSE;
  widget->dirty_rect_tolerance = 4;
  widget->need_relayout_children = TRUE;
  widget->need_update_style = TRUE;

  if (parent) {
    widget_add_child(parent, widget);
  }

  wstr_init(&(widget->text), 0);
  if (!widget->vt) {
    widget->vt = widget_vtable_default();
  }

  if (widget->astyle == NULL) {
    widget->astyle = style_factory_create_style(style_factory(), widget);
    ENSURE(widget->astyle != NULL);
  }

  if (parent != NULL && widget_is_window_opened(widget)) {
    widget_set_need_update_style(widget);
  }

  widget_invalidate_force(widget, NULL);

  widget->initializing = FALSE;

  return widget;
}

ret_t widget_get_prop_default_value(widget_t* widget, const char* name, value_t* v) {
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_X)) {
    value_set_int32(v, 0);
  } else if (tk_str_eq(name, WIDGET_PROP_Y)) {
    value_set_int32(v, 0);
  } else if (tk_str_eq(name, WIDGET_PROP_W)) {
    value_set_int32(v, 0);
  } else if (tk_str_eq(name, WIDGET_PROP_H)) {
    value_set_int32(v, 0);
  } else if (tk_str_eq(name, WIDGET_PROP_OPACITY)) {
    value_set_int32(v, 0xff);
  } else if (tk_str_eq(name, WIDGET_PROP_VISIBLE)) {
    value_set_bool(v, TRUE);
  } else if (tk_str_eq(name, WIDGET_PROP_SENSITIVE)) {
    value_set_bool(v, TRUE);
  } else if (tk_str_eq(name, WIDGET_PROP_FLOATING)) {
    value_set_bool(v, FALSE);
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUSABLE)) {
    value_set_bool(v, FALSE);
  } else if (tk_str_eq(name, WIDGET_PROP_WITH_FOCUS_STATE)) {
    value_set_bool(v, FALSE);
  } else if (tk_str_eq(name, WIDGET_PROP_DIRTY_RECT_TOLERANCE)) {
    value_set_int(v, 4);
  } else if (tk_str_eq(name, WIDGET_PROP_STYLE)) {
    value_set_str(v, NULL);
  } else if (tk_str_eq(name, WIDGET_PROP_ENABLE)) {
    value_set_bool(v, TRUE);
  } else if (tk_str_eq(name, WIDGET_PROP_NAME)) {
    value_set_str(v, NULL);
  } else if (tk_str_eq(name, WIDGET_PROP_TEXT)) {
    value_set_wstr(v, NULL);
  } else if (tk_str_eq(name, WIDGET_PROP_ANIMATION)) {
    value_set_str(v, NULL);
  } else if (tk_str_eq(name, WIDGET_PROP_SELF_LAYOUT)) {
    value_set_str(v, NULL);
  } else if (tk_str_eq(name, WIDGET_PROP_CHILDREN_LAYOUT)) {
    value_set_str(v, NULL);
  } else if (tk_str_eq(name, WIDGET_PROP_FOCUSED)) {
    value_set_bool(v, FALSE);
  } else if (tk_str_eq(name, WIDGET_PROP_FEEDBACK)) {
    value_set_bool(v, FALSE);
  } else {
    if (widget->vt->get_prop_default_value) {
      ret = widget->vt->get_prop_default_value(widget, name, v);
    } else {
      ret = RET_NOT_FOUND;
    }
  }

  return ret;
}

ret_t widget_to_screen(widget_t* widget, point_t* p) {
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL && p != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    if (widget_is_scrollable(iter)) {
      p->x -= widget_get_prop_int(iter, WIDGET_PROP_XOFFSET, 0);
      p->y -= widget_get_prop_int(iter, WIDGET_PROP_YOFFSET, 0);
    }

    p->x += iter->x;
    p->y += iter->y;

    iter = iter->parent;
  }

  return RET_OK;
}

ret_t widget_to_local(widget_t* widget, point_t* p) {
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL && p != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    if (widget_is_scrollable(iter)) {
      p->x += widget_get_prop_int(iter, WIDGET_PROP_XOFFSET, 0);
      p->y += widget_get_prop_int(iter, WIDGET_PROP_YOFFSET, 0);
    }

    p->x -= iter->x;
    p->y -= iter->y;

    iter = iter->parent;
  }

  return RET_OK;
}

ret_t widget_to_global(widget_t* widget, point_t* p) {
  widget_t* iter = widget;
  return_value_if_fail(widget != NULL && p != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    p->x += iter->x;
    p->y += iter->y;

    iter = iter->parent;
  }

  return RET_OK;
}

int32_t widget_count_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, 0);

  return widget->children != NULL ? widget->children->size : 0;
}

widget_t* widget_get_child(widget_t* widget, int32_t index) {
  return_value_if_fail(widget != NULL && widget->children != NULL, NULL);
  return_value_if_fail(index < widget->children->size, NULL);

  return WIDGET(widget->children->elms[index]);
}

int32_t widget_index_of(widget_t* widget) {
  widget_t* parent = NULL;
  return_value_if_fail(widget != NULL && widget->parent != NULL, -1);

  parent = widget->parent;
  WIDGET_FOR_EACH_CHILD_BEGIN(parent, iter, i)
  if (iter == widget) {
    return i;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return -1;
}

ret_t widget_prepare_text_style(widget_t* widget, canvas_t* c) {
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  color_t tc = style_get_color(style, STYLE_ID_TEXT_COLOR, trans);
  const char* font_name = style_get_str(style, STYLE_ID_FONT_NAME, NULL);
  uint16_t font_size = style_get_int(style, STYLE_ID_FONT_SIZE, TK_DEFAULT_FONT_SIZE);
  align_h_t align_h = (align_h_t)style_get_int(style, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_CENTER);
  align_v_t align_v = (align_v_t)style_get_int(style, STYLE_ID_TEXT_ALIGN_V, ALIGN_V_MIDDLE);

  canvas_set_text_color(c, tc);
  canvas_set_font(c, font_name, font_size);
  canvas_set_text_align(c, align_h, align_v);

  return RET_OK;
}

static ret_t widget_copy_style(widget_t* clone, widget_t* widget) {
  if (style_is_mutable(widget->astyle) && style_mutable_cast(widget->astyle) != NULL) {
    if (!style_is_mutable(clone->astyle)) {
      widget_ensure_style_mutable(clone);
    }

    if (style_mutable_cast(clone->astyle) != NULL) {
      style_mutable_copy(clone->astyle, widget->astyle);
    }
  }

  return RET_OK;
}

static const char* const s_widget_persistent_props[] = {WIDGET_PROP_NAME,
                                                        WIDGET_PROP_STYLE,
                                                        WIDGET_PROP_TR_TEXT,
                                                        WIDGET_PROP_TEXT,
                                                        WIDGET_PROP_ANIMATION,
                                                        WIDGET_PROP_ENABLE,
                                                        WIDGET_PROP_VISIBLE,
                                                        WIDGET_PROP_FLOATING,
                                                        WIDGET_PROP_CHILDREN_LAYOUT,
                                                        WIDGET_PROP_SELF_LAYOUT,
                                                        WIDGET_PROP_OPACITY,
                                                        WIDGET_PROP_FOCUSED,
                                                        WIDGET_PROP_FEEDBACK,
                                                        WIDGET_PROP_FOCUSABLE,
                                                        WIDGET_PROP_SENSITIVE,
                                                        WIDGET_PROP_WITH_FOCUS_STATE,
                                                        NULL};

const char* const* widget_get_persistent_props(void) {
  return s_widget_persistent_props;
}

static ret_t widget_copy_base_props(widget_t* widget, widget_t* other) {
  widget->state = tk_str_copy(widget->state, other->state);
  widget->name = tk_str_copy(widget->name, other->name);
  widget->style = tk_str_copy(widget->style, other->style);

  if (other->text.str != NULL) {
    widget_set_text(widget, other->text.str);
  }
  widget->tr_text = tk_str_copy(widget->tr_text, other->tr_text);

  widget->enable = other->enable;
  widget->visible = other->visible;
  widget->floating = other->floating;
  widget->opacity = other->opacity;
  widget->feedback = other->feedback;
  widget->focusable = other->focusable;
  widget->sensitive = other->sensitive;
  widget->auto_created = other->auto_created;
  widget->with_focus_state = other->with_focus_state;
  widget->dirty_rect_tolerance = other->dirty_rect_tolerance;

  if (other->animation != NULL && *(other->animation)) {
    widget_set_animation(widget, other->animation);
  }

  if (other->self_layout != NULL) {
    widget->self_layout = self_layouter_clone(other->self_layout);
  }

  if (other->children_layout != NULL) {
    widget->children_layout = children_layouter_clone(other->children_layout);
  }

  return RET_OK;
}

static ret_t widget_copy(widget_t* widget, widget_t* other) {
  return_value_if_fail(widget != NULL && other != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget->vt == other->vt, RET_BAD_PARAMS);

  widget_copy_style(widget, other);
  widget_copy_base_props(widget, other);

  if (other->custom_props) {
    widget->custom_props = object_default_clone(OBJECT_DEFAULT(other->custom_props));
  }

  if (widget->vt->on_copy != NULL) {
    widget->vt->on_copy(widget, other);
  } else {
    widget_on_copy_default(widget, other);
  }

  widget_set_need_update_style(widget);

  return RET_OK;
}

widget_t* widget_clone(widget_t* widget, widget_t* parent) {
  widget_t* clone = NULL;
  return_value_if_fail(widget != NULL && widget->vt != NULL && widget->vt->create != NULL, NULL);

  clone = widget->vt->create(parent, widget->x, widget->y, widget->w, widget->h);
  return_value_if_fail(clone != NULL, NULL);

  widget_copy(clone, widget);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_clone(iter, clone);
  WIDGET_FOR_EACH_CHILD_END();

  return clone;
}

#define PROP_EQ(prop) (widget->prop == other->prop)
bool_t widget_equal(widget_t* widget, widget_t* other) {
  bool_t ret = FALSE;
  const char* const* properties = NULL;
  return_value_if_fail(widget != NULL && other != NULL, FALSE);

  ret = PROP_EQ(opacity) && PROP_EQ(enable) && PROP_EQ(visible) && PROP_EQ(vt) && PROP_EQ(x) &&
        PROP_EQ(y) && PROP_EQ(w) && PROP_EQ(h) && PROP_EQ(floating);
  if (widget->name != NULL || other->name != NULL) {
    ret = ret && (tk_str_eq(widget->name, other->name) || PROP_EQ(name));
  }

  if (widget->style != NULL || other->style != NULL) {
    ret = ret && tk_str_eq(widget->style, other->style);
  }

  if (!ret) {
    return ret;
  }

  ret = ret && wstr_equal(&(widget->text), &(other->text));

  if (widget->tr_text != NULL || other->tr_text != NULL) {
    ret = ret && (tk_str_eq(widget->tr_text, other->tr_text) || PROP_EQ(tr_text));
  }

  if (!ret) {
    return ret;
  }

  properties = widget->vt->clone_properties;
  if (properties != NULL) {
    value_t v1;
    value_t v2;
    uint32_t i = 0;
    for (i = 0; properties[i] != NULL; i++) {
      const char* prop = properties[i];
      if (widget_get_prop(widget, prop, &v1) != RET_OK) {
        continue;
      }

      if (widget_get_prop(other, prop, &v2) != RET_OK) {
        return FALSE;
      }

      if (!value_equal(&v1, &v2)) {
        log_debug("prop %s not equal\n", prop);
        return FALSE;
      }
    }
  }

  if (widget->children_layout != other->children_layout) {
    const char* str1 = children_layouter_to_string(widget->children_layout);
    const char* str2 = children_layouter_to_string(other->children_layout);
    if (!tk_str_eq(str1, str2)) {
      return FALSE;
    }
  }

  if (widget->self_layout != other->self_layout) {
    const char* str1 = self_layouter_to_string(widget->self_layout);
    const char* str2 = self_layouter_to_string(other->self_layout);
    if (!tk_str_eq(str1, str2)) {
      return FALSE;
    }
  }

  if (!ret) {
    return ret;
  }

  if (widget->children == other->children) {
    return TRUE;
  }

  if (widget->children == NULL || other->children == NULL) {
    return FALSE;
  }

  if (widget->children->size != other->children->size) {
    return FALSE;
  }

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_t* iter_other = WIDGET(other->children->elms[i]);
  if (!widget_equal(iter, iter_other)) {
    return FALSE;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return TRUE;
}

float_t widget_measure_text(widget_t* widget, const wchar_t* text) {
  canvas_t* c = widget_get_canvas(widget);
  return_value_if_fail(widget != NULL && text != NULL && c != NULL, 0);

  widget_prepare_text_style(widget, c);

  return canvas_measure_text(c, (wchar_t*)text, wcslen(text));
}

ret_t widget_load_image(widget_t* widget, const char* name, bitmap_t* bitmap) {
  image_manager_t* imm = widget_get_image_manager(widget);

  return_value_if_fail(imm != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget != NULL && name != NULL && bitmap != NULL, RET_BAD_PARAMS);

  return image_manager_get_bitmap(imm, name, bitmap);
}

ret_t widget_unload_image(widget_t* widget, bitmap_t* bitmap) {
  image_manager_t* imm = widget_get_image_manager(widget);

  return_value_if_fail(imm != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget != NULL && bitmap != NULL, RET_BAD_PARAMS);

  return image_manager_unload_bitmap(imm, bitmap);
}

const asset_info_t* widget_load_asset(widget_t* widget, asset_type_t type, const char* name) {
  assets_manager_t* am = widget_get_assets_manager(widget);
  return_value_if_fail(widget != NULL && name != NULL && am != NULL, NULL);

  return assets_manager_ref(am, type, name);
}

ret_t widget_unload_asset(widget_t* widget, const asset_info_t* asset) {
  assets_manager_t* am = widget_get_assets_manager(widget);
  return_value_if_fail(widget != NULL && asset != NULL && am != NULL, RET_BAD_PARAMS);

  return assets_manager_unref(am, asset);
}

bool_t widget_is_point_in(widget_t* widget, xy_t x, xy_t y, bool_t is_local) {
  point_t p = {x, y};
  return_value_if_fail(widget != NULL, FALSE);

  if (!is_local) {
    widget_to_local(widget, &p);
  }

  return (p.x >= 0 && p.y >= 0 && p.x < widget->w && p.y < widget->h);
}

const char* widget_get_type(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt != NULL, NULL);

  return widget_get_prop_str(widget, WIDGET_PROP_TYPE, NULL);
}

widget_t* widget_cast(widget_t* widget) {
  return widget;
}

bool_t widget_is_instance_of(widget_t* widget, const widget_vtable_t* vt) {
  const widget_vtable_t* iter = NULL;
  return_value_if_fail(widget != NULL && vt != NULL, FALSE);

  iter = widget->vt;
  while (iter != NULL) {
    if (iter == vt) {
      return TRUE;
    }

    iter = iter->parent;
  }
#ifdef WITH_WIDGET_TYPE_CHECK
  if (vt == widget_vtable_default()) {
    return TRUE;
  } else {
    return FALSE;
  }
#else
  return TRUE;
#endif /*WITH_WIDGET_TYPE_CHECK*/
}

static ret_t widget_ensure_visible_in_scroll_view(widget_t* widget, widget_t* parent) {
  int32_t ox = 0;
  int32_t oy = 0;
  int32_t old_ox = 0;
  int32_t old_oy = 0;
  return_value_if_fail(widget != NULL && parent != NULL, RET_BAD_PARAMS);

  ox = widget_get_prop_int(parent, WIDGET_PROP_XOFFSET, 0);
  oy = widget_get_prop_int(parent, WIDGET_PROP_YOFFSET, 0);
  old_ox = ox;
  old_oy = oy;

  if (oy > widget->y) {
    oy = widget->y;
  }

  if (ox > widget->x) {
    ox = widget->x;
  }

  if ((widget->y + widget->h) > (oy + parent->h)) {
    oy = widget->y + widget->h - parent->h;
  }

  if ((widget->x + widget->w) > (ox + parent->w)) {
    ox = widget->x + widget->w - parent->w;
  }

  if (ox != old_ox) {
    widget_set_prop_int(parent, WIDGET_PROP_XOFFSET, ox);
  }

  if (oy != old_oy) {
    widget_set_prop_int(parent, WIDGET_PROP_YOFFSET, oy);
  }

  return RET_OK;
}

ret_t widget_ensure_visible_in_viewport(widget_t* widget) {
  widget_t* parent = NULL;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  parent = widget->parent;
  while (parent != NULL) {
    if (widget_is_scrollable(parent)) {
      widget_ensure_visible_in_scroll_view(widget, parent);
    }

    widget = parent;
    parent = widget->parent;
  }

  return RET_OK;
}

ret_t widget_set_as_key_target(widget_t* widget) {
  if (widget_is_keyboard(widget)) {
    return RET_OK;
  }

  if (widget != NULL) {
    widget_t* parent = widget->parent;

    if (parent != NULL) {
      if (!(parent->focused)) {
        parent->focused = TRUE;
        event_t e = event_init(EVT_FOCUS, NULL);
        widget_dispatch(parent, &e);
      }

      if (parent->key_target != NULL && parent->key_target != widget) {
        widget_set_focused_internal(widget->parent->key_target, FALSE);
      }

      parent->key_target = widget;
      widget_set_as_key_target(parent);
    }
    widget_set_need_update_style(widget);
  }

  return RET_OK;
}

bool_t widget_is_keyboard(widget_t* widget) {
  value_t v;
  return_value_if_fail(widget != NULL && widget->vt != NULL, FALSE);

  if (widget->vt->is_keyboard) {
    return TRUE;
  }

  if (widget_get_prop(widget, WIDGET_PROP_IS_KEYBOARD, &v) == RET_OK) {
    return value_bool(&v);
  }

  return FALSE;
}

static ret_t widget_on_visit_focusable(void* ctx, const void* data) {
  widget_t* widget = WIDGET(data);
  darray_t* all_focusable = (darray_t*)ctx;

  if (!(widget->sensitive) || !(widget->visible) || !(widget->enable)) {
    return RET_SKIP;
  }

  if (widget->vt->only_active_child_visible) {
    widget_t* child = widget_find_target(widget, 1, 1);
    widget_foreach(child, widget_on_visit_focusable, all_focusable);

    return RET_SKIP;
  }

  if (widget_is_focusable(widget)) {
    darray_push(all_focusable, widget);
  }

  return RET_OK;
}

static ret_t widget_get_all_focusable_widgets_in_window(widget_t* widget, darray_t* all_focusable) {
  widget_t* win = widget_get_window_or_keyboard(widget);
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  widget_foreach(win, widget_on_visit_focusable, all_focusable);

  return RET_OK;
}

static widget_t* widget_get_first_focusable_widget_in_window(widget_t* widget) {
  widget_t* first = NULL;
  darray_t all_focusable;
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(win != NULL, first);

  darray_init(&all_focusable, 10, NULL, NULL);
  widget_foreach(win, widget_on_visit_focusable, &all_focusable);
  if (all_focusable.size > 0) {
    first = WIDGET(all_focusable.elms[0]);
  }
  darray_deinit(&all_focusable);

  return first;
}

bool_t widget_has_focused_widget_in_window(widget_t* widget) {
  widget_t* iter = NULL;
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(win != NULL, FALSE);

  iter = win->key_target;
  while (iter != NULL) {
    if (widget_is_focusable(iter) && iter->focused) {
      return TRUE;
    }

    iter = iter->key_target;
  }

  return FALSE;
}

ret_t widget_focus_first(widget_t* widget) {
  widget_t* first = widget_get_first_focusable_widget_in_window(widget);

  if (first != NULL) {
    widget_set_prop_bool(first, WIDGET_PROP_FOCUSED, TRUE);
  }

  return RET_OK;
}

static widget_t* widget_find_prev_focus_widget(widget_t* widget, darray_t* all_focusable) {
  uint32_t i = 0;
  for (i = 0; i < all_focusable->size; i++) {
    widget_t* iter = WIDGET(all_focusable->elms[i]);

    if (iter == widget) {
      uint32_t focus = (i == 0) ? (all_focusable->size - 1) : (i - 1);

      return WIDGET(all_focusable->elms[focus]);
    }
  }

  return NULL;
}

static widget_t* widget_find_next_focus_widget(widget_t* widget, darray_t* all_focusable) {
  uint32_t i = 0;
  for (i = 0; i < all_focusable->size; i++) {
    widget_t* iter = WIDGET(all_focusable->elms[i]);

    if (iter == widget) {
      uint32_t focus = ((i + 1) == all_focusable->size) ? (0) : (i + 1);

      return WIDGET(all_focusable->elms[focus]);
    }
  }

  return NULL;
}

static bool_t is_same_row(const rect_t* r1, const rect_t* r2) {
  int32_t cy = r2->y + r2->h / 2;

  return (cy >= r1->y && cy < (r1->y + r1->h));
}

static bool_t is_same_col(const rect_t* r1, const rect_t* r2) {
  int32_t cx = r2->x + r2->w / 2;

  return (cx >= r1->x && cx < (r1->x + r1->w));
}

static uint32_t distance2(const rect_t* r1, const rect_t* r2) {
  uint32_t dx = (r1->x + r1->w / 2) - (r2->x + r2->w / 2);
  uint32_t dy = (r1->y + r1->h / 2) - (r2->y + r2->h / 2);

  return dx * dx + dy * dy;
}

static bool_t match_up(const rect_t* widget, const rect_t* last_matched, const rect_t* iter) {
  if ((iter->y + iter->h / 2) > widget->y) {
    return FALSE;
  }

  if (last_matched == NULL) {
    return TRUE;
  }

  if (is_same_col(widget, iter)) {
    if (!is_same_col(widget, last_matched)) {
      return TRUE;
    } else {
      return (iter->y + iter->h) > (last_matched->y + last_matched->h);
    }
  } else if (is_same_col(widget, last_matched)) {
    return FALSE;
  } else {
    return distance2(widget, iter) < distance2(widget, last_matched);
  }
}

static bool_t match_down(const rect_t* widget, const rect_t* last_matched, const rect_t* iter) {
  if ((iter->y + iter->h / 2) < (widget->y + widget->h)) {
    return FALSE;
  }

  if (last_matched == NULL) {
    return TRUE;
  }

  if (is_same_col(widget, iter)) {
    if (!is_same_col(widget, last_matched)) {
      return TRUE;
    } else {
      return iter->y < last_matched->y;
    }
  } else if (is_same_col(widget, last_matched)) {
    return FALSE;
  } else {
    return distance2(widget, iter) < distance2(widget, last_matched);
  }
}

static bool_t match_left(const rect_t* widget, const rect_t* last_matched, const rect_t* iter) {
  if ((iter->x + iter->w / 2) > widget->x) {
    return FALSE;
  }

  if (last_matched == NULL) {
    return TRUE;
  }

  if (is_same_row(widget, iter)) {
    if (!is_same_row(widget, last_matched)) {
      return TRUE;
    } else {
      return (iter->x + iter->w) > (last_matched->x + last_matched->w);
    }
  } else if (is_same_row(widget, last_matched)) {
    return FALSE;
  } else {
    return distance2(widget, iter) < distance2(widget, last_matched);
  }
}

static bool_t match_right(const rect_t* widget, const rect_t* last_matched, const rect_t* iter) {
  if ((iter->x + iter->w / 2) < (widget->x + widget->w)) {
    return FALSE;
  }

  if (last_matched == NULL) {
    return TRUE;
  }

  if (is_same_row(widget, iter)) {
    if (!is_same_row(widget, last_matched)) {
      return TRUE;
    } else {
      return iter->x < last_matched->x;
    }
  } else if (is_same_row(widget, last_matched)) {
    return FALSE;
  } else {
    return distance2(widget, iter) < distance2(widget, last_matched);
  }
}

typedef bool_t (*match_focus_widget_t)(const rect_t* widget, const rect_t* last_matched,
                                       const rect_t* iter);

static widget_t* widget_find_matched_focus_widget(widget_t* widget, darray_t* all_focusable,
                                                  match_focus_widget_t match) {
  uint32_t i = 0;
  point_t p = {0, 0};
  widget_t* matched = NULL;
  rect_t riter = {0, 0, 0, 0};
  rect_t rwidget = {0, 0, 0, 0};
  rect_t rmatched = {0, 0, 0, 0};

  widget_to_global(widget, &p);
  rwidget = rect_init(p.x, p.y, widget->w, widget->h);

  for (i = 0; i < all_focusable->size; i++) {
    widget_t* iter = WIDGET(all_focusable->elms[i]);
    if (iter == widget) {
      continue;
    }

    p.x = 0;
    p.y = 0;
    widget_to_global(iter, &p);

    riter = rect_init(p.x, p.y, iter->w, iter->h);
    if (match(&rwidget, (matched != NULL ? &rmatched : NULL), &riter)) {
      matched = iter;
      rmatched = riter;
    }
  }

  return matched;
}

static widget_t* widget_find_up_focus_widget(widget_t* widget, darray_t* all_focusable) {
  return widget_find_matched_focus_widget(widget, all_focusable, match_up);
}

static widget_t* widget_find_down_focus_widget(widget_t* widget, darray_t* all_focusable) {
  return widget_find_matched_focus_widget(widget, all_focusable, match_down);
}

static widget_t* widget_find_left_focus_widget(widget_t* widget, darray_t* all_focusable) {
  return widget_find_matched_focus_widget(widget, all_focusable, match_left);
}

static widget_t* widget_find_right_focus_widget(widget_t* widget, darray_t* all_focusable) {
  return widget_find_matched_focus_widget(widget, all_focusable, match_right);
}

ret_t widget_move_focus(widget_t* widget, widget_find_wanted_focus_widget_t find) {
  ret_t ret = RET_FAIL;
  darray_t all_focusable;

  if (widget == NULL || !widget->focused) {
    return RET_FAIL;
  }

  return_value_if_fail(find != NULL, RET_FAIL);
  return_value_if_fail(darray_init(&all_focusable, 10, NULL, NULL) != NULL, RET_OOM);

  widget_get_all_focusable_widgets_in_window(widget, &all_focusable);
  if (all_focusable.size > 1) {
    widget_t* focus = find(widget, &all_focusable);

    if (focus != NULL && focus != widget) {
      widget_set_prop_bool(widget, WIDGET_PROP_FOCUSED, FALSE);
      widget_set_prop_bool(focus, WIDGET_PROP_FOCUSED, TRUE);
      ret = RET_OK;
    }
  }
  darray_deinit(&all_focusable);

  return ret;
}

ret_t widget_focus_prev(widget_t* widget) {
  return widget_move_focus(widget, widget_find_prev_focus_widget);
}

ret_t widget_focus_next(widget_t* widget) {
  return widget_move_focus(widget, widget_find_next_focus_widget);
}

ret_t widget_focus_up(widget_t* widget) {
  return widget_move_focus(widget, widget_find_up_focus_widget);
}

ret_t widget_focus_down(widget_t* widget) {
  return widget_move_focus(widget, widget_find_down_focus_widget);
}

ret_t widget_focus_left(widget_t* widget) {
  if (widget_move_focus(widget, widget_find_left_focus_widget) == RET_OK) {
    return RET_OK;
  } else {
    return widget_focus_up(widget);
  }
}

ret_t widget_focus_right(widget_t* widget) {
  if (widget_move_focus(widget, widget_find_right_focus_widget) == RET_OK) {
    return RET_OK;
  } else {
    return widget_focus_down(widget);
  }
}

bool_t widget_is_window(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt != NULL, FALSE);

  return widget->vt->is_window;
}

bool_t widget_is_designing_window(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt != NULL, FALSE);

  return widget->vt->is_designing_window;
}

bool_t widget_is_window_manager(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt != NULL, FALSE);

  return widget->vt->is_window_manager;
}

ret_t widget_set_need_relayout_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (!widget->destroying && widget->children != NULL && widget->children->size > 0) {
    widget->need_relayout_children = TRUE;
  }

  return RET_OK;
}

static ret_t widget_ensure_style_mutable(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->astyle == NULL) {
    widget->astyle = style_mutable_create(widget, NULL);
    return_value_if_fail(widget->astyle != NULL, RET_OOM);
  }

  if (!(widget->astyle->vt->is_mutable)) {
    widget->astyle = style_mutable_create(widget, widget->astyle);
    return_value_if_fail(widget->astyle != NULL, RET_OOM);
  }

  return RET_OK;
}

ret_t widget_set_style(widget_t* widget, const char* state_and_name, const value_t* value) {
  char str[256];
  uint32_t len = 0;
  char* name = NULL;
  char* state = NULL;
  return_value_if_fail(widget != NULL && state_and_name != NULL && value != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget_ensure_style_mutable(widget) == RET_OK, RET_BAD_PARAMS);

  len = strlen(state_and_name);
  return_value_if_fail(len < sizeof(str), RET_BAD_PARAMS);

  memcpy(str, state_and_name, len);
  str[len] = '\0';

  name = strchr(str, ':');
  if (name != NULL) {
    *name++ = '\0';
    state = str;
  } else {
    name = str;
    state = WIDGET_STATE_NORMAL;
  }

  widget_invalidate(widget, NULL);

  return style_set(widget->astyle, state, name, value);
}

ret_t widget_set_style_int(widget_t* widget, const char* state_and_name, int32_t value) {
  value_t v;
  return_value_if_fail(widget != NULL && state_and_name != NULL, RET_BAD_PARAMS);

  value_set_int(&v, value);
  return widget_set_style(widget, state_and_name, &v);
}

ret_t widget_set_style_color(widget_t* widget, const char* state_and_name, uint32_t value) {
  value_t v;
  return_value_if_fail(widget != NULL && state_and_name != NULL, RET_BAD_PARAMS);

  value_set_int(&v, value);
  return widget_set_style(widget, state_and_name, &v);
}

ret_t widget_set_style_str(widget_t* widget, const char* state_and_name, const char* value) {
  value_t v;
  return_value_if_fail(widget != NULL && state_and_name != NULL && value != NULL, RET_BAD_PARAMS);

  value_set_str(&v, value);
  return widget_set_style(widget, state_and_name, &v);
}

canvas_t* widget_get_canvas(widget_t* widget) {
  canvas_t* c = NULL;
  widget_t* wm = window_manager();
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(widget != NULL, NULL);

  if (win == NULL) {
    win = window_manager_get_top_window(wm);
  }

  if (win != NULL) {
    c = (canvas_t*)widget_get_prop_pointer(win, WIDGET_PROP_CANVAS);
  }

  if (c == NULL) {
    c = (canvas_t*)widget_get_prop_pointer(wm, WIDGET_PROP_CANVAS);
  }

  return c;
}

bool_t widget_is_system_bar(widget_t* widget) {
  return tk_str_eq(widget->vt->type, WIDGET_TYPE_SYSTEM_BAR) ||
         tk_str_eq(widget->vt->type, WIDGET_TYPE_SYSTEM_BAR_BOTTOM);
}

bool_t widget_is_normal_window(widget_t* widget) {
  return tk_str_eq(widget->vt->type, WIDGET_TYPE_NORMAL_WINDOW);
}

bool_t widget_is_dialog(widget_t* widget) {
  return tk_str_eq(widget->vt->type, WIDGET_TYPE_DIALOG);
}

bool_t widget_is_popup(widget_t* widget) {
  return tk_str_eq(widget->vt->type, WIDGET_TYPE_POPUP);
}

bool_t widget_is_opened_popup(widget_t* widget) {
  int32_t stage = widget_get_prop_int(widget, WIDGET_PROP_STAGE, WINDOW_STAGE_NONE);
  return tk_str_eq(widget->vt->type, WIDGET_TYPE_POPUP) && stage == WINDOW_STAGE_OPENED;
}

ret_t widget_reset_canvas(widget_t* widget) {
#ifndef AWTK_WEB
  rect_t rect;
  canvas_t* c = widget_get_canvas(widget);
  return_value_if_fail(c != NULL, RET_BAD_PARAMS);
  rect = rect_init(0, 0, c->lcd->w, c->lcd->h);
  canvas_set_clip_rect(c, &rect);

  c->font = NULL;
  return vgcanvas_reset(canvas_get_vgcanvas(c));
#else
  (void)widget;
  return RET_OK;
#endif /*AWTK_WEB*/
}

widget_t* widget_ref(widget_t* widget) {
  ENSURE(widget != NULL && widget->ref_count > 0 && widget->vt != NULL);
  return_value_if_fail(widget != NULL && widget->ref_count > 0 && widget->vt != NULL, NULL);

  widget->ref_count++;

  return widget;
}

static ret_t widget_unref_in_idle(const idle_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);
  ENSURE(widget != NULL && widget->ref_count > 0 && widget->vt != NULL);

  if (widget->ref_count > 1) {
    return RET_REPEAT;
  }

  widget_destroy_sync(widget);
  return RET_REMOVE;
}

ret_t widget_unref(widget_t* widget) {
  ENSURE(widget != NULL && widget->ref_count > 0 && widget->vt != NULL);
  return_value_if_fail(widget != NULL && widget->ref_count > 0 && widget->vt != NULL,
                       RET_BAD_PARAMS);

  if (widget->ref_count > 1) {
    widget->ref_count--;
  } else if (widget->ref_count == 1) {
    if (!idle_exist(widget_unref_in_idle, widget)) {
      widget_destroy_sync(widget);
    }
  }

  return RET_OK;
}

static ret_t widget_unref_async(widget_t* widget) {
  if (!idle_exist(widget_unref_in_idle, widget)) {
    idle_add(widget_unref_in_idle, widget);
  }

  return RET_OK;
}

ret_t widget_close_window(widget_t* widget) {
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  return window_manager_close_window(win->parent, win);
}

#if defined(FRAGMENT_FRAME_BUFFER_SIZE)
bitmap_t* widget_take_snapshot_rect(widget_t* widget, rect_t* r) {
  log_warn("not supported yet\n");
  return NULL;
}

#elif defined(WITH_NANOVG_GPU)

bitmap_t* widget_take_snapshot_rect(widget_t* widget, rect_t* r) {
  bitmap_t* img;
  uint32_t w = 0;
  uint32_t h = 0;
  canvas_t* c = NULL;
  vgcanvas_t* vg = NULL;
  framebuffer_object_t fbo;

  native_window_t* native_window =
      (native_window_t*)widget_get_prop_pointer(window_manager(), WIDGET_PROP_NATIVE_WINDOW);
  return_value_if_fail(native_window != NULL, NULL);

  c = native_window_get_canvas(native_window);
  vg = lcd_get_vgcanvas(c->lcd);
  return_value_if_fail(c != NULL && vg != NULL, NULL);

  vgcanvas_create_fbo(vg, vg->w, vg->h, &fbo);
  vgcanvas_bind_fbo(vg, &fbo);
  canvas_set_clip_rect(c, r);
  widget_paint(widget, c);
  vgcanvas_unbind_fbo(vg, &fbo);

  if (r != NULL) {
    w = r->w;
    h = r->h;
  } else {
    w = fbo.w;
    h = fbo.h;
  }

  img = bitmap_create_ex(w * fbo.ratio, h * fbo.ratio, 0, BITMAP_FMT_RGBA8888);
  vgcanvas_fbo_to_bitmap(vg, &fbo, img, r);

  vgcanvas_destroy_fbo(vg, &fbo);

  return img;
}

#else
#include "lcd/lcd_mem_rgba8888.h"
bitmap_t* widget_take_snapshot_rect(widget_t* widget, rect_t* r) {
  wh_t w = 0;
  wh_t h = 0;
  canvas_t canvas;
  lcd_t* lcd = NULL;
  canvas_t* c = NULL;
  uint8_t* buff = NULL;
  bitmap_t* bitmap = NULL;
  bitmap_t* bitmap_clip = NULL;
  native_window_t* native_window = NULL;
  return_value_if_fail(widget != NULL && widget->vt != NULL, NULL);

  native_window =
      (native_window_t*)widget_get_prop_pointer(window_manager(), WIDGET_PROP_NATIVE_WINDOW);
  return_value_if_fail(native_window != NULL, NULL);

  c = native_window_get_canvas(native_window);
  return_value_if_fail(c != NULL, NULL);

  w = widget->w;
  h = widget->h;

  bitmap = bitmap_create_ex(w, h, w * 4, BITMAP_FMT_RGBA8888);
  return_value_if_fail(bitmap != NULL, NULL);

  buff = bitmap_lock_buffer_for_write(bitmap);
  lcd = lcd_mem_rgba8888_create_single_fb(w, h, buff);
  if (lcd != NULL) {
    ((lcd_mem_t*)lcd)->vgcanvas = canvas_get_vgcanvas(c);
    canvas_init(&canvas, lcd, font_manager());
    canvas_begin_frame(&canvas, r, LCD_DRAW_OFFLINE);
    widget_paint(widget, &canvas);
    canvas_end_frame(&canvas);
    canvas_reset(&canvas);
    ((lcd_mem_t*)lcd)->vgcanvas = NULL;
    lcd_destroy(lcd);
  }

  bitmap_unlock_buffer(bitmap);

  if (r != NULL) {
    rect_t widget_rect = rect_init(0, 0, widget->w, widget->h);
    rect_t clip_r = rect_intersect(&widget_rect, r);
    bitmap_clip = bitmap_create_ex(r->w, r->h, r->w * 4, BITMAP_FMT_RGBA8888);

    if (image_copy(bitmap_clip, bitmap, &clip_r, 0, 0) == RET_OK) {
      bitmap_destroy(bitmap);
      return bitmap_clip;
    } else {
      return NULL;
    }
  }

  return bitmap;
}
#endif /*WITH_NANOVG_GPU*/

bitmap_t* widget_take_snapshot(widget_t* widget) {
  return widget_take_snapshot_rect(widget, NULL);
}

ret_t widget_dispatch_simple_event(widget_t* widget, uint32_t type) {
  event_t e = event_init(type, widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_dispatch(widget, &e);
}

native_window_t* widget_get_native_window(widget_t* widget) {
  widget_t* win = widget_get_window(widget);
  return_value_if_fail(win != NULL, NULL);

  return (native_window_t*)widget_get_prop_pointer(win, WIDGET_PROP_NATIVE_WINDOW);
}

ret_t widget_set_child_text_utf8(widget_t* widget, const char* name, const char* text) {
  widget_t* child = widget_lookup(widget, name, TRUE);
  return_value_if_fail(child != NULL, RET_BAD_PARAMS);

  return widget_set_text_utf8(child, text);
}

ret_t widget_set_child_text_with_double(widget_t* widget, const char* name, const char* format,
                                        double value) {
  char text[64];
  widget_t* child = widget_lookup(widget, name, TRUE);
  return_value_if_fail(child != NULL && format != NULL, RET_BAD_PARAMS);

  memset(text, 0x00, sizeof(text));
  tk_snprintf(text, sizeof(text) - 1, format, value);

  return widget_set_text_utf8(child, text);
}

ret_t widget_begin_wait_pointer_cursor(widget_t* widget, bool_t ignore_user_input) {
  widget_t* wm = widget_get_window_manager(widget);

  return window_manager_begin_wait_pointer_cursor(wm, ignore_user_input);
}

ret_t widget_end_wait_pointer_cursor(widget_t* widget) {
  widget_t* wm = widget_get_window_manager(widget);

  return window_manager_end_wait_pointer_cursor(wm);
}

ret_t widget_draw_text_in_rect(widget_t* widget, canvas_t* c, const wchar_t* str, uint32_t size,
                               rect_t* r, bool_t ellipses) {
  const char* bidi_type = widget_get_bidi(widget);
  return_value_if_fail(widget != NULL && c != NULL && str != NULL && r != NULL, RET_BAD_PARAMS);

  return canvas_draw_text_bidi_in_rect(c, str, size, r, bidi_type, ellipses);
}

bool_t widget_is_parent_of(widget_t* widget, widget_t* child) {
  widget_t* iter = NULL;
  return_value_if_fail(widget != NULL && child != NULL, FALSE);

  iter = child->parent;
  while (iter != NULL) {
    if (iter == widget) {
      return TRUE;
    }
    iter = iter->parent;
  }

  return FALSE;
}

bool_t widget_is_direct_parent_of(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL, FALSE);

  return child->parent == widget;
}
