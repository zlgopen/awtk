/**
 * File:   window_manager.c
 * Author: AWTK Develop Team
 * Brief:  window manager
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

#include "base/keys.h"
#include "tkc/mem.h"
#include "base/idle.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "base/layout.h"
#include "tkc/time_now.h"
#include "base/locale_info.h"
#include "base/system_info.h"
#include "base/image_manager.h"
#include "base/window_manager.h"

static ret_t window_manager_inc_fps(widget_t* widget);
static ret_t window_manager_update_fps(widget_t* widget);
static ret_t window_manager_do_open_window(widget_t* wm, widget_t* window);
static ret_t window_manger_layout_child(widget_t* widget, widget_t* window);

static bool_t is_system_bar(widget_t* widget) {
  return tk_str_eq(widget->vt->type, WIDGET_TYPE_SYSTEM_BAR);
}

static bool_t is_normal_window(widget_t* widget) {
  return tk_str_eq(widget->vt->type, WIDGET_TYPE_NORMAL_WINDOW);
}

static bool_t is_dialog(widget_t* widget) {
  return tk_str_eq(widget->vt->type, WIDGET_TYPE_DIALOG);
}

static bool_t is_popup(widget_t* widget) {
  return tk_str_eq(widget->vt->type, WIDGET_TYPE_POPUP);
}

static ret_t window_manager_dispatch_top_window_changed(widget_t* widget) {
  window_event_t e;

  e.e = event_init(EVT_TOP_WINDOW_CHANGED, widget);
  e.window = window_manager_get_top_main_window(widget);

  widget_dispatch(widget, (event_t*)(&e));

  return RET_OK;
}

static ret_t window_manager_dispatch_window_event(widget_t* window, event_type_t type) {
  window_event_t evt;
  event_t e = event_init(type, window);
  widget_dispatch(window, &e);

  evt.window = window;
  evt.e = event_init(type, window->parent);

  if (type == EVT_WINDOW_OPEN) {
    window_manager_dispatch_top_window_changed(window->parent);
  }

  return widget_dispatch(window->parent, (event_t*)&(evt));
}

static widget_t* window_manager_find_prev_window(widget_t* widget) {
  int32_t i = 0;
  int32_t nr = 0;
  return_value_if_fail(widget != NULL, NULL);

  if (widget->children != NULL && widget->children->size > 0) {
    nr = widget->children->size;
    for (i = nr - 2; i >= 0; i--) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (is_normal_window(iter)) {
        return iter;
      }
    }
  }

  return NULL;
}

static ret_t window_manager_create_animator(window_manager_t* wm, widget_t* curr_win, bool_t open) {
  value_t v;
  const char* anim_hint = NULL;
  widget_t* prev_win = window_manager_find_prev_window(WIDGET(wm));
  const char* key = open ? WIDGET_PROP_OPEN_ANIM_HINT : WIDGET_PROP_CLOSE_ANIM_HINT;

  return_value_if_fail(wm != NULL && prev_win != NULL && curr_win != NULL, RET_BAD_PARAMS);

  if (wm->animator != NULL) {
    return RET_FAIL;
  }

  if (widget_get_prop(curr_win, key, &v) == RET_OK) {
    anim_hint = value_str(&(v));
  } else {
    key = WIDGET_PROP_ANIM_HINT;
    if (widget_get_prop(curr_win, key, &v) == RET_OK) {
      anim_hint = value_str(&(v));
    }
  }

  if (anim_hint && *anim_hint) {
    if (open) {
      wm->animator = window_animator_create_for_open(anim_hint, wm->canvas, prev_win, curr_win);
    } else {
      wm->animator = window_animator_create_for_close(anim_hint, wm->canvas, prev_win, curr_win);
    }

    wm->animating = wm->animator != NULL;
    if (wm->animating) {
      wm->ignore_user_input = TRUE;
      log_debug("ignore_user_input\n");
    }
  }

  return wm->animating ? RET_OK : RET_FAIL;
}

static ret_t on_idle_invalidate(const timer_info_t* info) {
  widget_t* curr_win = WIDGET(info->ctx);
  widget_invalidate_force(curr_win, NULL);

  return RET_REMOVE;
}

static ret_t window_manager_check_if_need_open_animation(const idle_info_t* info) {
  widget_t* curr_win = WIDGET(info->ctx);
  window_manager_t* wm = WINDOW_MANAGER(curr_win->parent);

  window_manager_dispatch_window_event(curr_win, EVT_WINDOW_WILL_OPEN);

  if (window_manager_create_animator(wm, curr_win, TRUE) != RET_OK) {
    window_manager_dispatch_window_event(curr_win, EVT_WINDOW_OPEN);
    widget_add_timer(curr_win, on_idle_invalidate, 100);
  }

  return RET_REMOVE;
}

static ret_t window_manager_dispatch_window_open(widget_t* curr_win) {
  window_manager_dispatch_window_event(curr_win, EVT_WINDOW_WILL_OPEN);

  return window_manager_dispatch_window_event(curr_win, EVT_WINDOW_OPEN);
}

static ret_t window_manager_idle_dispatch_window_open(const idle_info_t* info) {
  window_manager_dispatch_window_open(WIDGET(info->ctx));

  return RET_REMOVE;
}

static ret_t window_manager_check_if_need_close_animation(window_manager_t* wm,
                                                          widget_t* curr_win) {
  return window_manager_create_animator(wm, curr_win, FALSE);
}

static ret_t window_manager_do_open_window(widget_t* widget, widget_t* window) {
  if (widget->children != NULL && widget->children->size > 0) {
    idle_add((idle_func_t)window_manager_check_if_need_open_animation, window);
  } else {
    idle_add((idle_func_t)window_manager_idle_dispatch_window_open, window);
  }

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

ret_t window_manager_open_window(widget_t* widget, widget_t* window) {
  ret_t ret = RET_OK;
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(widget != NULL && window != NULL, RET_BAD_PARAMS);

  if (is_system_bar(window)) {
    return_value_if_fail(wm->system_bar == NULL, RET_BAD_PARAMS);
  }

  if (wm->animator != NULL) {
    wm->pending_open_window = window;
  } else {
    window_manager_do_open_window(widget, window);
  }

  ret = widget_add_child(widget, window);
  return_value_if_fail(ret == RET_OK, RET_FAIL);
  window_manger_layout_child(widget, window);

  window->dirty = FALSE;
  widget->target = window;
  widget_invalidate(window, NULL);

  if (is_system_bar(window)) {
    wm->system_bar = window;
  }

  widget_on(window, EVT_DESTROY, wm_on_destroy_child, widget);
  widget_update_style(widget);

  return ret;
}

static ret_t window_manager_idle_destroy_window(const idle_info_t* info) {
  widget_t* win = WIDGET(info->ctx);
  widget_destroy(win);

#ifdef ENABLE_MEM_LEAK_CHECK
  tk_mem_dump();
#endif /*ENABLE_MEM_LEAK_CHECK*/

  return RET_OK;
}

ret_t window_manager_prepare_close_window(widget_t* widget, widget_t* window) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
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

ret_t window_manager_close_window(widget_t* widget, widget_t* window) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(widget != NULL && window != NULL, RET_BAD_PARAMS);

  window_manager_prepare_close_window(widget, window);

  if (wm->animator) {
    wm->pending_close_window = window;
    return RET_OK;
  }

  window_manager_dispatch_window_event(window, EVT_WINDOW_CLOSE);
  if (window_manager_check_if_need_close_animation(wm, window) != RET_OK) {
    widget_remove_child(widget, window);
    idle_add(window_manager_idle_destroy_window, window);
  }

  return RET_OK;
}

ret_t window_manager_close_window_force(widget_t* widget, widget_t* window) {
  return_value_if_fail(widget != NULL && window != NULL, RET_BAD_PARAMS);

  window_manager_prepare_close_window(widget, window);
  window_manager_dispatch_window_event(window, EVT_WINDOW_CLOSE);
  widget_remove_child(widget, window);
  widget_destroy(window);

  return RET_OK;
}

widget_t* window_manager_find_target(widget_t* widget, xy_t x, xy_t y) {
  point_t p = {x, y};
  return_value_if_fail(widget != NULL, NULL);

  if (widget->grab_widget != NULL) {
    return widget->grab_widget;
  }

  widget_to_local(widget, &p);
  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  xy_t r = iter->x + iter->w;
  xy_t b = iter->y + iter->h;

  if (iter->visible && iter->sensitive && iter->enable && p.x >= iter->x && p.y >= iter->y &&
      p.x <= r && p.y <= b) {
    return iter;
  }

  if (is_dialog(iter) || is_popup(iter)) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END()

  return NULL;
}

static rect_t window_manager_calc_dirty_rect(window_manager_t* wm) {
  rect_t r = wm->dirty_rect;
  widget_t* widget = WIDGET(wm);
  rect_t* ldr = &(wm->last_dirty_rect);

  rect_merge(&r, ldr);

  return rect_fix(&r, widget->w, widget->h);
}

static ret_t window_manager_paint_cursor(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  window_manager_t* wm = WINDOW_MANAGER(widget);

  if (wm->cursor != NULL) {
    return_value_if_fail(image_manager_get_bitmap(image_manager(), wm->cursor, &bitmap) == RET_OK,
                         RET_BAD_PARAMS);
    canvas_draw_icon(c, &bitmap, wm->r_cursor.x, wm->r_cursor.y);
  }

  return RET_OK;
}

static ret_t window_manager_update_cursor(widget_t* widget, int32_t x, int32_t y) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  uint32_t w = wm->r_cursor.w;
  uint32_t h = wm->r_cursor.h;

  if (wm->cursor != NULL && w > 0 && h > 0) {
    uint32_t hw = w >> 1;
    uint32_t hh = h >> 1;
    int32_t oldx = wm->r_cursor.x;
    int32_t oldy = wm->r_cursor.y;
    rect_t r = rect_init(oldx - hw, oldy - hh, w, h);

    widget->dirty = FALSE;
    widget_invalidate(widget, &r);

    wm->r_cursor.x = x;
    wm->r_cursor.y = y;

    r = rect_init(x - hw, y - hh, w, h);

    widget->dirty = FALSE;
    widget_invalidate(widget, &r);
  }

  return RET_OK;
}

static ret_t window_manager_paint_normal(widget_t* widget, canvas_t* c) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  rect_t* dr = &(wm->dirty_rect);

  window_manager_inc_fps(widget);

  if (wm->show_fps) {
    rect_t fps_rect = rect_init(0, 0, 60, 30);
    widget_invalidate(widget, &fps_rect);
  }

  if (dr->w && dr->h) {
    uint32_t start_time = time_now_ms();
    rect_t r = window_manager_calc_dirty_rect(wm);

    if (r.w > 0 && r.h > 0) {
      ENSURE(canvas_begin_frame(c, &r, LCD_DRAW_NORMAL) == RET_OK);
      ENSURE(widget_paint(WIDGET(wm), c) == RET_OK);
      window_manager_paint_cursor(widget, c);
      ENSURE(canvas_end_frame(c) == RET_OK);
      wm->last_paint_cost = time_now_ms() - start_time;
      wm->last_dirty_rect = wm->dirty_rect;
      /*
        log_debug("%s x=%d y=%d w=%d h=%d cost=%d\n", __FUNCTION__, (int)(r.x), (int)(r.y),
                (int)(r.w), (int)(r.h), (int)wm->last_paint_cost);
      */
    }
  }

  wm->dirty_rect = rect_init(widget->w, widget->h, 0, 0);

  return RET_OK;
}

static ret_t window_manager_paint_animation(widget_t* widget, canvas_t* c) {
  uint32_t start_time = time_now_ms();
  window_manager_t* wm = WINDOW_MANAGER(widget);

  ret_t ret = window_animator_update(wm->animator, start_time);
  wm->last_paint_cost = time_now_ms() - start_time;
  window_manager_inc_fps(widget);

  if (ret == RET_DONE) {
    if (wm->animator->open) {
      window_manager_dispatch_window_event(wm->animator->curr_win, EVT_WINDOW_OPEN);
    }

    window_animator_destroy(wm->animator);
    wm->animator = NULL;
    wm->animating = FALSE;
    wm->ignore_user_input = FALSE;

    if (wm->pending_close_window != NULL) {
      widget_t* window = wm->pending_close_window;
      wm->pending_close_window = NULL;
      window_manager_close_window(widget, window);
    } else if (wm->pending_open_window != NULL) {
      widget_t* window = wm->pending_open_window;
      wm->pending_open_window = NULL;
      window_manager_do_open_window(widget, window);
    }

    if (wm->system_bar != NULL) {
      widget_invalidate_force(wm->system_bar, NULL);
    }
  }

  return RET_OK;
}

static ret_t window_manager_inc_fps(widget_t* widget) {
  window_manager_t* wm = WINDOW_MANAGER(widget);

  wm->fps_count++;

  return RET_OK;
}

static ret_t window_manager_update_fps(widget_t* widget) {
  uint32_t elapse = 0;
  uint32_t now = time_now_ms();
  window_manager_t* wm = WINDOW_MANAGER(widget);

  elapse = now - wm->fps_time;
  if (elapse >= 200) {
    wm->fps = wm->fps_count * 1000 / elapse;

    wm->fps_time = now;
    wm->fps_count = 0;
  }

  canvas_set_fps(wm->canvas, wm->show_fps, wm->fps);

  return RET_OK;
}

ret_t window_manager_paint(widget_t* widget, canvas_t* c) {
  ret_t ret = RET_OK;
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && c != NULL, RET_BAD_PARAMS);

  wm->canvas = c;
  canvas_set_global_alpha(c, 0xff);
  window_manager_update_fps(widget);

  if (wm->animator != NULL) {
    ret = window_manager_paint_animation(widget, c);
  } else {
    ret = window_manager_paint_normal(widget, c);
  }

  return ret;
}

static widget_t* s_window_manager = NULL;

widget_t* window_manager(void) {
  return s_window_manager;
}

ret_t window_manager_set(widget_t* widget) {
  s_window_manager = widget;

  return RET_OK;
}

widget_t* window_manager_create(void) {
  window_manager_t* wm = TKMEM_ZALLOC(window_manager_t);
  return_value_if_fail(wm != NULL, NULL);

  return window_manager_init(wm);
}

static ret_t window_manager_invalidate(widget_t* widget, rect_t* r) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  rect_t* dr = &(wm->dirty_rect);

  rect_merge(dr, r);

  return RET_OK;
}

int32_t window_manager_find_top_window_index(widget_t* widget) {
  return_value_if_fail(widget != NULL, -1);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  const char* type = widget_get_type(iter);
  if (tk_str_eq(type, WIDGET_TYPE_NORMAL_WINDOW)) {
    return i;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return -1;
}

widget_t* window_manager_get_top_main_window(widget_t* widget) {
  int32_t index = window_manager_find_top_window_index(widget);
  return_value_if_fail(index >= 0, NULL);

  return widget_get_child(widget, index);
}

ret_t window_manager_on_paint_children(widget_t* widget, canvas_t* c) {
  int32_t start = 0;
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  const char* type = widget_get_type(iter);
  if (iter->visible && tk_str_eq(type, WIDGET_TYPE_NORMAL_WINDOW)) {
    start = i;
    break;
  }
  WIDGET_FOR_EACH_CHILD_END()

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (i >= start && iter->visible) {
    widget_paint(iter, c);
    if (i == start) {
      if (wm->system_bar != NULL) {
        widget_paint(wm->system_bar, c);
      }
    }
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

static ret_t wm_on_remove_child(widget_t* widget, widget_t* window) {
  widget_t* top = window_manager_get_top_main_window(widget);

  if (top != NULL) {
    rect_t r;
    r = rect_init(window->x, window->y, window->w, window->h);
    widget_invalidate(top, &r);
  }

  return RET_FAIL;
}

static ret_t window_manager_get_prop(widget_t* widget, const char* name, value_t* v) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_CURSOR)) {
    value_set_str(v, wm->cursor);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t window_manager_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_CURSOR)) {
    return window_manager_set_cursor(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t window_manager_on_destroy(widget_t* widget) {
  window_manager_t* wm = WINDOW_MANAGER(widget);

  TKMEM_FREE(wm->cursor);

  return RET_OK;
}

static const widget_vtable_t s_window_manager_vtable = {
    .type = WIDGET_TYPE_WINDOW_MANAGER,
    .set_prop = window_manager_set_prop,
    .get_prop = window_manager_get_prop,
    .invalidate = window_manager_invalidate,
    .on_paint_children = window_manager_on_paint_children,
    .on_remove_child = wm_on_remove_child,
    .find_target = window_manager_find_target,
    .on_destroy = window_manager_on_destroy};

static ret_t wm_on_locale_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  widget_re_translate_text(iter);
  widget_dispatch(iter, e);
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

widget_t* window_manager_init(window_manager_t* wm) {
  widget_t* w = &(wm->widget);
  return_value_if_fail(wm != NULL, NULL);

  widget_init(w, NULL, &s_window_manager_vtable, 0, 0, 0, 0);

  locale_info_on(locale_info(), EVT_LOCALE_CHANGED, wm_on_locale_changed, wm);

  return w;
}

static ret_t window_manger_layout_child(widget_t* widget, widget_t* window) {
  xy_t x = window->x;
  xy_t y = window->y;
  wh_t w = window->w;
  wh_t h = window->h;
  window_manager_t* wm = WINDOW_MANAGER(widget);
  rect_t client_r = rect_init(0, 0, widget->w, widget->h);

  if (wm->system_bar != NULL) {
    widget_t* bar = wm->system_bar;
    client_r = rect_init(0, bar->h, widget->w, widget->h - bar->h);
  }

  if (is_normal_window(window)) {
    x = client_r.x;
    y = client_r.y;
    w = client_r.w;
    h = client_r.h;
  } else if (is_system_bar(window)) {
    x = 0;
    y = 0;
    w = widget->w;
  } else if (is_dialog(window)) {
    x = (widget->w - window->w) >> 1;
    y = (widget->h - window->h) >> 1;
  }

  widget_move_resize(window, x, y, w, h);
  widget_layout(window);

  return RET_OK;
}

static ret_t window_manger_layout_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  window_manger_layout_child(widget, iter);
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

ret_t window_manager_resize(widget_t* widget, wh_t w, wh_t h) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->dirty_rect.x = 0;
  wm->dirty_rect.y = 0;
  wm->dirty_rect.w = w;
  wm->dirty_rect.h = h;
  wm->last_dirty_rect = wm->dirty_rect;
  widget_move_resize(widget, 0, 0, w, h);

  return window_manger_layout_children(widget);
}

ret_t window_manager_dispatch_input_event(widget_t* widget, event_t* e) {
  input_device_status_t* ids = NULL;
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && e != NULL, RET_BAD_PARAMS);

  ids = &(wm->input_device_status);
  if (wm->ignore_user_input) {
    if (ids->pressed && e->type == EVT_POINTER_UP) {
      log_debug("animating ignore input, but it is last pointer_up\n");
    } else {
      log_debug("animating ignore input\n");
      return RET_OK;
    }
  }

  input_device_status_on_input_event(ids, widget, e);
  window_manager_update_cursor(widget, ids->last_x, ids->last_y);

  return RET_OK;
}

ret_t window_manager_set_show_fps(widget_t* widget, bool_t show_fps) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->show_fps = show_fps;

  return RET_OK;
}

widget_t* window_manager_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_window_manager_vtable, NULL);

  return widget;
}

ret_t window_manager_set_cursor(widget_t* widget, const char* cursor) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(wm->cursor);
  if (cursor != NULL) {
    bitmap_t bitmap;
    wm->cursor = tk_strdup(cursor);

    return_value_if_fail(image_manager_get_bitmap(image_manager(), cursor, &bitmap) == RET_OK,
                         RET_BAD_PARAMS);
    wm->r_cursor.w = bitmap.w;
    wm->r_cursor.h = bitmap.h;
  }

  return RET_OK;
}
