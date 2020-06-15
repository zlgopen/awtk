/**
 * File:   window_manager_simple.c
 * Author: AWTK Develop Team
 * Brief:  default window manager
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
 * 2019-08-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/keys.h"
#include "tkc/mem.h"
#include "base/idle.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "base/layout.h"
#include "tkc/time_now.h"
#include "base/dialog.h"
#include "base/locale_info.h"
#include "base/system_info.h"
#include "base/image_manager.h"
#include "window_manager/window_manager_simple.h"

ret_t window_manager_paint_system_bar(widget_t* widget, canvas_t* c);
static ret_t window_manager_simple_invalidate(widget_t* widget, rect_t* r);
static ret_t window_manager_simple_do_open_window(widget_t* wm, widget_t* window);
static ret_t window_manager_simple_layout_child(widget_t* widget, widget_t* window);

static bool_t window_is_fullscreen(widget_t* widget) {
  value_t v;
  value_set_bool(&v, FALSE);
  widget_get_prop(widget, WIDGET_PROP_FULLSCREEN, &v);

  return value_bool(&v);
}

static bool_t window_is_opened(widget_t* widget) {
  int32_t stage = widget_get_prop_int(widget, WIDGET_PROP_STAGE, WINDOW_STAGE_NONE);

  return stage == WINDOW_STAGE_OPENED || stage == WINDOW_STAGE_SUSPEND;
}

static widget_t* window_manager_find_prev_window(widget_t* widget) {
  int32_t i = 0;
  int32_t nr = 0;
  return_value_if_fail(widget != NULL, NULL);

  if (widget->children != NULL && widget->children->size > 0) {
    nr = widget->children->size;
    for (i = nr - 2; i >= 0; i--) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (widget_is_normal_window(iter)) {
        return iter;
      }
    }
  }

  return NULL;
}

static ret_t window_manager_dispatch_window_open(widget_t* curr_win) {
  window_manager_dispatch_window_event(curr_win, EVT_WINDOW_WILL_OPEN);

  return window_manager_dispatch_window_event(curr_win, EVT_WINDOW_OPEN);
}

static ret_t window_manager_idle_dispatch_window_open(const idle_info_t* info) {
  window_manager_dispatch_window_open(WIDGET(info->ctx));

  return RET_REMOVE;
}

static ret_t window_manager_simple_do_open_window(widget_t* widget, widget_t* window) {
  widget_add_idle(window, (idle_func_t)window_manager_idle_dispatch_window_open);

  return RET_OK;
}

static ret_t wm_on_destroy_child(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  (void)e;
  if (!widget->destroying) {
    window_manager_dispatch_top_window_changed(widget);
  }

  return RET_REMOVE;
}

static ret_t window_manager_simple_open_window(widget_t* widget, widget_t* window) {
  ret_t ret = RET_OK;
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  return_value_if_fail(widget != NULL && window != NULL, RET_BAD_PARAMS);

  if (widget_is_system_bar(window)) {
    return_value_if_fail(wm->system_bar == NULL, RET_BAD_PARAMS);
  }

  wm->prev_win = window_manager_get_top_window(widget);

  window_manager_simple_do_open_window(widget, window);

  ret = widget_add_child(widget, window);
  return_value_if_fail(ret == RET_OK, RET_FAIL);
  window_manager_simple_layout_child(widget, window);

  window->dirty = FALSE;
  widget->target = window;

  if (!widget_is_keyboard(window)) {
    widget->key_target = window;
  }
  widget_invalidate(window, NULL);

  if (widget_is_system_bar(window)) {
    wm->system_bar = window;
  }

  widget_set_prop_pointer(window, WIDGET_PROP_NATIVE_WINDOW, wm->native_window);
  widget_on(window, EVT_DESTROY, wm_on_destroy_child, widget);
  widget_update_style(widget);

  return ret;
}

static ret_t window_manager_idle_destroy_window(const idle_info_t* info) {
  widget_t* win = WIDGET(info->ctx);
  widget_destroy(win);

  return RET_OK;
}

static ret_t window_manager_prepare_close_window(widget_t* widget, widget_t* window) {
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  return_value_if_fail(widget != NULL && window != NULL, RET_BAD_PARAMS);

  if (widget->target == window) {
    widget->target = NULL;
  }

  if (widget->key_target == window) {
    widget->key_target = NULL;
  }

  if (widget->grab_widget != NULL) {
    if (widget->grab_widget == window) {
      widget->grab_widget = NULL;
    }
  }

  if (wm->system_bar == window) {
    wm->system_bar = NULL;
  }

  return RET_OK;
}

static ret_t window_manager_simple_close_window(widget_t* widget, widget_t* window) {
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);

  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget_is_window(window), RET_BAD_PARAMS);
  return_value_if_fail(window_is_opened(window), RET_BAD_PARAMS);

  window_manager_prepare_close_window(widget, window);
  window_manager_dispatch_window_event(window, EVT_WINDOW_CLOSE);
  {
    widget_t* prev_win = window_manager_find_prev_window(WIDGET(wm));
    if (prev_win != NULL) {
      if (!widget_is_keyboard(window)) {
        window_manager_dispatch_window_event(prev_win, EVT_WINDOW_TO_FOREGROUND);
      }
    }
    widget_remove_child(widget, window);
    idle_add(window_manager_idle_destroy_window, window);
  }

  return RET_OK;
}

static ret_t window_manager_simple_close_window_force(widget_t* widget, widget_t* window) {
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);

  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget_is_window(window), RET_BAD_PARAMS);

  window_manager_prepare_close_window(widget, window);
  window_manager_dispatch_window_event(window, EVT_WINDOW_CLOSE);
  widget_remove_child(widget, window);
  widget_destroy(window);

  return RET_OK;
}

static widget_t* window_manager_simple_find_target(widget_t* widget, xy_t x, xy_t y) {
  return window_manager_find_target(widget, NULL, x, y);
}

static ret_t window_manager_paint_normal(widget_t* widget, canvas_t* c) {
  uint64_t start_time = time_now_ms();
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);

  if (native_window_begin_frame(wm->native_window, LCD_DRAW_NORMAL) == RET_OK) {
    ENSURE(widget_paint(WIDGET(wm), c) == RET_OK);
    native_window_end_frame(wm->native_window);
  }
  wm->last_paint_cost = time_now_ms() - start_time;

  return RET_OK;
}

static ret_t window_manager_simple_paint(widget_t* widget) {
  ret_t ret = RET_OK;
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  canvas_t* c = native_window_get_canvas(wm->native_window);
  return_value_if_fail(wm != NULL && c != NULL, RET_BAD_PARAMS);

  canvas_set_global_alpha(c, 0xff);

  ret = window_manager_paint_normal(widget, c);

  return ret;
}

static ret_t window_manager_simple_invalidate(widget_t* widget, rect_t* r) {
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);

  if (wm->native_window != NULL) {
    native_window_invalidate(wm->native_window, r);
  }

  return RET_OK;
}

static widget_t* window_manager_simple_get_prev_window(widget_t* widget) {
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);

  return wm->prev_win;
}

static ret_t window_manager_simple_on_paint_children(widget_t* widget, canvas_t* c) {
  int32_t start = 0;
  bool_t has_fullscreen_win = FALSE;
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter->visible && widget_is_normal_window(iter)) {
    start = i;
    break;
  }
  WIDGET_FOR_EACH_CHILD_END()

  /*paint normal windows*/
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (i >= start && iter->visible) {
    if (widget_is_normal_window(iter)) {
      widget_paint(iter, c);

      if (!has_fullscreen_win) {
        has_fullscreen_win = window_is_fullscreen(iter);
      }
      start = i + 1;
      break;
    }
  }
  WIDGET_FOR_EACH_CHILD_END()

  /*paint system_bar*/
  if (!has_fullscreen_win) {
    window_manager_paint_system_bar(widget, c);
  }
  /*paint dialog and other*/
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (i >= start && iter->visible) {
    if (wm->system_bar != iter && !widget_is_normal_window(iter)) {
      widget_paint(iter, c);
    }
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

static ret_t window_manager_simple_on_remove_child(widget_t* widget, widget_t* window) {
  widget_t* top = window_manager_get_top_main_window(widget);

  if (top != NULL) {
    rect_t r;
    r = rect_init(window->x, window->y, window->w, window->h);
    widget_invalidate(top, &r);
  }

  return RET_FAIL;
}

static ret_t window_manager_simple_get_prop(widget_t* widget, const char* name, value_t* v) {
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_CANVAS)) {
    canvas_t* c = native_window_get_canvas(wm->native_window);
    value_set_pointer(v, c);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t window_manager_simple_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return RET_NOT_FOUND;
}

static ret_t window_manager_simple_on_destroy(widget_t* widget) {
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  object_unref(OBJECT(wm->native_window));

  return RET_OK;
}

static ret_t window_manager_simple_on_layout_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  window_manager_simple_layout_child(widget, iter);
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t window_manager_simple_resize(widget_t* widget, wh_t w, wh_t h);
static ret_t window_manager_simple_post_init(widget_t* widget, wh_t w, wh_t h);
static ret_t window_manager_simple_dispatch_input_event(widget_t* widget, event_t* e);

static ret_t window_manager_simple_get_pointer(widget_t* widget, xy_t* x, xy_t* y,
                                               bool_t* pressed) {
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (x != NULL) {
    *x = wm->input_device_status.last_x;
  }
  if (y != NULL) {
    *y = wm->input_device_status.last_y;
  }
  if (pressed != NULL) {
    *pressed = wm->input_device_status.pressed;
  }

  return RET_OK;
}

static ret_t window_manager_simple_on_event(widget_t* widget, event_t* e) {
  if (e->type == EVT_ORIENTATION_WILL_CHANGED) {
    orientation_event_t* evt = orientation_event_cast(e);
    lcd_orientation_t orientation = evt->orientation;
    window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
    lcd_t* lcd = native_window_get_canvas(wm->native_window)->lcd;

    wh_t w = wm->lcd_w;
    wh_t h = wm->lcd_h;
    if (orientation == LCD_ORIENTATION_90 || orientation == LCD_ORIENTATION_270) {
      w = wm->lcd_h;
      h = wm->lcd_w;
    }

    lcd_resize(lcd, w, h, 0);
    window_manager_simple_resize(widget, w, h);
    e->type = EVT_ORIENTATION_CHANGED;

    widget_dispatch(widget, e);
  }

  return RET_OK;
}

static ret_t window_manager_simple_layout_child(widget_t* widget, widget_t* window) {
  xy_t x = window->x;
  xy_t y = window->y;
  wh_t w = window->w;
  wh_t h = window->h;
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  rect_t client_r = rect_init(0, 0, widget->w, widget->h);

  if (wm->system_bar != NULL) {
    widget_t* bar = wm->system_bar;
    client_r = rect_init(0, bar->h, widget->w, widget->h - bar->h);
  }

  if (widget_is_normal_window(window)) {
    if (window_is_fullscreen(window)) {
      x = 0;
      y = 0;
      w = widget->w;
      h = widget->h;
    } else {
      x = client_r.x;
      y = client_r.y;
      w = client_r.w;
      h = client_r.h;
    }
  } else if (widget_is_system_bar(window)) {
    x = 0;
    y = 0;
    w = widget->w;
  } else if (widget_is_dialog(window)) {
    x = (widget->w - window->w) >> 1;
    y = (widget->h - window->h) >> 1;
  } else {
    x = window->x;
    y = window->y;
    w = window->w;
    h = window->h;
  }

  widget_move_resize(window, x, y, w, h);
  widget_layout(window);

  return RET_OK;
}

static ret_t window_manager_simple_resize(widget_t* widget, wh_t w, wh_t h) {
  rect_t r = rect_init(0, 0, w, h);
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  widget_move_resize(widget, 0, 0, w, h);

  native_window_resize(wm->native_window, w, h, TRUE);
  native_window_invalidate(wm->native_window, &r);
  native_window_update_last_dirty_rect(wm->native_window);

  return widget_layout_children(widget);
}

static ret_t window_manager_simple_post_init(widget_t* widget, wh_t w, wh_t h) {
  native_window_info_t info;
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->lcd_w = w;
  wm->lcd_h = h;
  wm->native_window = native_window_create(widget);

  if (native_window_get_info(wm->native_window, &info) == RET_OK) {
    w = info.w;
    h = info.h;
  }

  window_manager_simple_resize(widget, w, h);

  return RET_OK;
}

static ret_t window_manager_simple_dispatch_input_event(widget_t* widget, event_t* e) {
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  return_value_if_fail(wm != NULL && e != NULL, RET_BAD_PARAMS);

  native_window_preprocess_event(wm->native_window, e);
  input_device_status_on_input_event(&(wm->input_device_status), widget, e);

  return RET_OK;
}

ret_t window_manager_paint_system_bar(widget_t* widget, canvas_t* c) {
  window_manager_simple_t* wm = WINDOW_MANAGER_SIMPLE(widget);
  return_value_if_fail(wm != NULL && c != NULL, RET_BAD_PARAMS);

  if (wm->system_bar != NULL && wm->system_bar->visible) {
    widget_paint(wm->system_bar, c);
  }

  return RET_OK;
}

static ret_t window_manager_native_native_window_resized(widget_t* widget, void* handle) {
  uint32_t w = 0;
  uint32_t h = 0;
  system_info_t* info = system_info();
  native_window_t* nw = WINDOW_MANAGER_SIMPLE(widget)->native_window;

  if (info->lcd_orientation == LCD_ORIENTATION_90 || info->lcd_orientation == LCD_ORIENTATION_270) {
    w = info->lcd_h;
    h = info->lcd_w;
  } else {
    w = info->lcd_w;
    h = info->lcd_h;
  }

  window_manager_simple_resize(widget, w, h);
  native_window_on_resized(nw, w, h);

  return RET_OK;
}

static ret_t window_manager_native_dispatch_native_window_event(widget_t* widget, event_t* e,
                                                                void* handle) {
  if (e->type == EVT_NATIVE_WINDOW_RESIZED) {
    window_manager_native_native_window_resized(widget, handle);
  }

  return RET_OK;
}

static window_manager_vtable_t s_window_manager_self_vtable = {
    .paint = window_manager_simple_paint,
    .post_init = window_manager_simple_post_init,
    .open_window = window_manager_simple_open_window,
    .get_pointer = window_manager_simple_get_pointer,
    .close_window = window_manager_simple_close_window,
    .get_prev_window = window_manager_simple_get_prev_window,
    .close_window_force = window_manager_simple_close_window_force,
    .dispatch_input_event = window_manager_simple_dispatch_input_event,
    .dispatch_native_window_event = window_manager_native_dispatch_native_window_event,
    .resize = window_manager_simple_resize,
};

static const widget_vtable_t s_window_manager_vtable = {
    .size = sizeof(window_manager_t),
    .is_window_manager = TRUE,
    .type = WIDGET_TYPE_WINDOW_MANAGER,
    .set_prop = window_manager_simple_set_prop,
    .get_prop = window_manager_simple_get_prop,
    .on_event = window_manager_simple_on_event,
    .invalidate = window_manager_simple_invalidate,
    .on_layout_children = window_manager_simple_on_layout_children,
    .on_paint_children = window_manager_simple_on_paint_children,
    .on_remove_child = window_manager_simple_on_remove_child,
    .find_target = window_manager_simple_find_target,
    .on_destroy = window_manager_simple_on_destroy};

widget_t* window_manager_create(void) {
  window_manager_simple_t* wm = TKMEM_ZALLOC(window_manager_simple_t);
  return_value_if_fail(wm != NULL, NULL);

  return window_manager_init(WINDOW_MANAGER(wm), &s_window_manager_vtable,
                             &s_window_manager_self_vtable);
}
