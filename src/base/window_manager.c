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
#include "widgets/dialog.h"
#include "base/locale_info.h"
#include "base/system_info.h"
#include "base/image_manager.h"
#include "base/window_manager.h"
#include "base/dialog_highlighter_factory.h"

static ret_t window_manager_invalidate(widget_t* widget, rect_t* r);

static ret_t window_manager_inc_fps(widget_t* widget);
static ret_t window_manager_update_fps(widget_t* widget);
static ret_t window_manager_do_open_window(widget_t* wm, widget_t* window);
static ret_t window_manager_layout_child(widget_t* widget, widget_t* window);
static ret_t window_manager_create_dialog_highlighter(widget_t* widget, widget_t* curr_win);

static bool_t is_window_fullscreen(widget_t* widget) {
  value_t v;
  value_set_bool(&v, FALSE);
  widget_get_prop(widget, WIDGET_PROP_FULLSCREEN, &v);

  return value_bool(&v);
}

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

static bool_t is_window(widget_t* widget) {
  return widget != NULL && widget->vt != NULL && widget->vt->is_window;
}

static bool_t is_window_opened(widget_t* widget) {
  int32_t stage = widget_get_prop_int(widget, WIDGET_PROP_STAGE, WINDOW_STAGE_NONE);

  return stage == WINDOW_STAGE_OPENED;
}

static ret_t wm_on_screen_saver_timer(const timer_info_t* info) {
  window_manager_t* wm = WINDOW_MANAGER(info->ctx);
  event_t e = event_init(EVT_SCREEN_SAVER, wm);
  wm->screen_saver_timer_id = TK_INVALID_ID;

  widget_dispatch(WIDGET(wm), &e);
  log_debug("emit: EVT_SCREEN_SAVER\n");

  return RET_REMOVE;
}

static ret_t window_manager_start_or_reset_screen_saver_timer(window_manager_t* wm) {
  if (wm->screen_saver_time > 0) {
    if (wm->screen_saver_timer_id == TK_INVALID_ID) {
      wm->screen_saver_timer_id = timer_add(wm_on_screen_saver_timer, wm, wm->screen_saver_time);
    } else {
      timer_modify(wm->screen_saver_timer_id, wm->screen_saver_time);
    }
  }

  return RET_OK;
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

  if (type == EVT_WINDOW_TO_FOREGROUND) {
    window->parent->key_target = window;
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

ret_t window_manager_snap_curr_window(widget_t* widget, widget_t* curr_win, bitmap_t* img,
                                      framebuffer_object_t* fbo, bool_t auto_rotate) {
  canvas_t* c = NULL;
#ifdef WITH_NANOVG_GPU
  vgcanvas_t* vg = NULL;
#else
  rect_t r = {0};
#endif /*WITH_NANOVG_GPU*/

  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(img != NULL && fbo != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm != NULL && curr_win != NULL, RET_BAD_PARAMS);

  c = wm->canvas;

#ifdef WITH_NANOVG_GPU
  vg = lcd_get_vgcanvas(c->lcd);
  ENSURE(vgcanvas_create_fbo(vg, fbo) == RET_OK);
  ENSURE(vgcanvas_bind_fbo(vg, fbo) == RET_OK);
  ENSURE(canvas_begin_frame(c, NULL, LCD_DRAW_OFFLINE) == RET_OK);
  ENSURE(widget_on_paint_background(widget, c) == RET_OK);
  ENSURE(widget_paint(curr_win, c) == RET_OK);
  ENSURE(canvas_end_frame(c) == RET_OK);
  ENSURE(vgcanvas_unbind_fbo(vg, fbo) == RET_OK);
  fbo_to_img(fbo, img);
#else
  r = rect_init(curr_win->x, curr_win->y, curr_win->w, curr_win->h);
  ENSURE(canvas_begin_frame(c, &r, LCD_DRAW_OFFLINE) == RET_OK);
  canvas_set_clip_rect(c, &r);
  ENSURE(widget_on_paint_background(widget, c) == RET_OK);
  ENSURE(widget_paint(curr_win, c) == RET_OK);
  ENSURE(canvas_end_frame(c) == RET_OK);
  ENSURE(lcd_take_snapshot(c->lcd, img, auto_rotate) == RET_OK);
#endif

  return RET_OK;
}

ret_t window_manager_snap_prev_window(widget_t* widget, widget_t* prev_win, bitmap_t* img,
                                      framebuffer_object_t* fbo, bool_t auto_rotate) {
  canvas_t* c = NULL;
#ifdef WITH_NANOVG_GPU
  vgcanvas_t* vg = NULL;
#else
  rect_t r = {0};
#endif /*WITH_NANOVG_GPU*/

  window_manager_t* wm = WINDOW_MANAGER(widget);
  dialog_highlighter_t* dialog_highlighter = NULL;

  return_value_if_fail(img != NULL && fbo != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm != NULL && prev_win != NULL, RET_BAD_PARAMS);

  c = wm->canvas;
  dialog_highlighter = wm->dialog_highlighter;

#ifdef WITH_NANOVG_GPU
  vg = lcd_get_vgcanvas(c->lcd);
  ENSURE(vgcanvas_create_fbo(vg, fbo) == RET_OK);
  ENSURE(vgcanvas_bind_fbo(vg, fbo) == RET_OK);
  ENSURE(canvas_begin_frame(c, NULL, LCD_DRAW_OFFLINE) == RET_OK);
  ENSURE(widget_on_paint_background(widget, c) == RET_OK);
  if (wm->system_bar) {
    widget_paint(wm->system_bar, c);
  }

  window_manager_paint_system_bar(widget, c);
  ENSURE(widget_paint(prev_win, c) == RET_OK);

  if (dialog_highlighter != NULL) {
    dialog_highlighter_prepare(dialog_highlighter, c);
  }
  ENSURE(canvas_end_frame(c) == RET_OK);
  ENSURE(vgcanvas_unbind_fbo(vg, fbo) == RET_OK);
  fbo_to_img(fbo, img);
#else
  r = rect_init(prev_win->x, prev_win->y, prev_win->w, prev_win->h);
  ENSURE(canvas_begin_frame(c, &r, LCD_DRAW_OFFLINE) == RET_OK);
  canvas_set_clip_rect(c, &r);
  ENSURE(widget_on_paint_background(widget, c) == RET_OK);
  window_manager_paint_system_bar(widget, c);
  ENSURE(widget_paint(prev_win, c) == RET_OK);
  if (dialog_highlighter != NULL) {
    dialog_highlighter_prepare(dialog_highlighter, c);
  }
  ENSURE(canvas_end_frame(c) == RET_OK);
  ENSURE(lcd_take_snapshot(c->lcd, img, auto_rotate) == RET_OK);
#endif /*WITH_NANOVG_GPU*/

  if (dialog_highlighter != NULL) {
    dialog_highlighter_set_bg(dialog_highlighter, img, fbo);
  }

  return RET_OK;
}

static ret_t window_manager_create_dialog_highlighter(widget_t* widget, widget_t* curr_win) {
  value_t v;
  ret_t ret = RET_FAIL;
  dialog_highlighter_t* dialog_highlighter = NULL;

  if (is_dialog(curr_win) && widget_get_prop(curr_win, WIDGET_PROP_HIGHLIGHT, &v) == RET_OK) {
    const char* args = value_str(&v);
    if (args != NULL) {
      dialog_highlighter_factory_t* f = dialog_highlighter_factory();
      dialog_highlighter = dialog_highlighter_factory_create_highlighter(f, args, curr_win);

      if (dialog_highlighter != NULL) {
        window_manager_set_dialog_highlighter(widget, dialog_highlighter);
        ret = RET_OK;
      }
    }
  }

  return ret;
}

static ret_t window_manager_prepare_dialog_highlighter(widget_t* widget, widget_t* prev_win,
                                                       widget_t* curr_win) {
  if (window_manager_create_dialog_highlighter(widget, curr_win) == RET_OK) {
    bitmap_t img = {0};
    framebuffer_object_t fbo = {0};
    window_manager_snap_prev_window(widget, prev_win, &img, &fbo, TRUE);

    return RET_OK;
  }

  return RET_FAIL;
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
    window_manager_create_dialog_highlighter(WIDGET(wm), curr_win);
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
  } else {
    widget_invalidate_force(prev_win, NULL);
    window_manager_prepare_dialog_highlighter(WIDGET(wm), prev_win, curr_win);
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
    widget_t* prev_win = window_manager_find_prev_window(WIDGET(wm));
    if (prev_win != NULL) {
      window_manager_dispatch_window_event(prev_win, EVT_WINDOW_TO_BACKGROUND);
    }
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
    widget_add_idle(window, (idle_func_t)window_manager_check_if_need_open_animation);
  } else {
    widget_add_idle(window, (idle_func_t)window_manager_idle_dispatch_window_open);
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

  wm->prev_win = window_manager_get_top_window(widget);

  if (wm->animator != NULL) {
    wm->pending_open_window = window;
  } else {
    window_manager_do_open_window(widget, window);
  }

  ret = widget_add_child(widget, window);
  return_value_if_fail(ret == RET_OK, RET_FAIL);
  window_manager_layout_child(widget, window);

  window->dirty = FALSE;
  widget->target = window;

  if (!widget_is_keyboard(window)) {
    widget->key_target = window;
  }
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

  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);
  return_value_if_fail(is_window(window), RET_BAD_PARAMS);
  return_value_if_fail(is_window_opened(window), RET_BAD_PARAMS);
  return_value_if_fail(wm->pending_close_window != window, RET_BAD_PARAMS);

  window_manager_prepare_close_window(widget, window);

  if (wm->animator) {
    wm->pending_close_window = window;
    return RET_OK;
  }

  window_manager_dispatch_window_event(window, EVT_WINDOW_CLOSE);
  if (window_manager_check_if_need_close_animation(wm, window) != RET_OK) {
    widget_t* prev_win = window_manager_find_prev_window(WIDGET(wm));
    if (prev_win != NULL) {
      window_manager_dispatch_window_event(prev_win, EVT_WINDOW_TO_FOREGROUND);
    }
    widget_remove_child(widget, window);
    idle_add(window_manager_idle_destroy_window, window);
  }

  return RET_OK;
}

ret_t window_manager_close_window_force(widget_t* widget, widget_t* window) {
  window_manager_t* wm = WINDOW_MANAGER(widget);

  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);
  return_value_if_fail(is_window(window), RET_BAD_PARAMS);
  return_value_if_fail(wm->pending_close_window != window, RET_BAD_PARAMS);

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

    window_manager_invalidate(widget, &r);

    wm->r_cursor.x = x;
    wm->r_cursor.y = y;

    r = rect_init(x - hw, y - hh, w, h);

    window_manager_invalidate(widget, &r);
  }

  return RET_OK;
}

static ret_t window_manager_paint_normal(widget_t* widget, canvas_t* c) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  rect_t* dr = &(wm->dirty_rect);

  window_manager_inc_fps(widget);

  if (wm->show_fps) {
    rect_t fps_rect = rect_init(0, 0, 60, 30);
    window_manager_invalidate(widget, &fps_rect);
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
  paint_event_t e;
  uint32_t start_time = time_now_ms();
  window_manager_t* wm = WINDOW_MANAGER(widget);

  ENSURE(window_animator_begin_frame(wm->animator) == RET_OK);

  widget_dispatch(widget, paint_event_init(&e, EVT_BEFORE_PAINT, widget, c));

  ret_t ret = window_animator_update(wm->animator, start_time);

  widget_dispatch(widget, paint_event_init(&e, EVT_AFTER_PAINT, widget, c));

  ENSURE(window_animator_end_frame(wm->animator) == RET_OK);

  wm->last_paint_cost = time_now_ms() - start_time;
  window_manager_inc_fps(widget);

  if (ret == RET_DONE) {
    bool_t is_open = wm->animator->open;
    widget_t* prev_win = wm->animator->prev_win;
    widget_t* curr_win = wm->animator->curr_win;
    window_animator_destroy(wm->animator);

    wm->animator = NULL;
    wm->animating = FALSE;
    wm->ignore_user_input = FALSE;

    if (is_open) {
      window_manager_dispatch_window_event(prev_win, EVT_WINDOW_TO_BACKGROUND);
      window_manager_dispatch_window_event(curr_win, EVT_WINDOW_OPEN);
    } else {
      window_manager_dispatch_window_event(prev_win, EVT_WINDOW_TO_FOREGROUND);
    }

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

widget_t* window_manager_get_top_main_window(widget_t* widget) {
  return_value_if_fail(widget != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (is_normal_window(iter) && iter->visible) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return NULL;
}

widget_t* window_manager_get_top_window(widget_t* widget) {
  return_value_if_fail(widget != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter->visible) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return NULL;
}

ret_t window_manager_on_paint_children(widget_t* widget, canvas_t* c) {
  int32_t start = 0;
  bool_t has_fullscreen_win = FALSE;
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter->visible && is_normal_window(iter)) {
    start = i;
    break;
  }
  WIDGET_FOR_EACH_CHILD_END()

  if (wm->dialog_highlighter != NULL) {
    dialog_highlighter_draw(wm->dialog_highlighter, 1);
  } else {
    /*paint normal windows*/
    WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
    if (i >= start && iter->visible) {
      if (is_normal_window(iter)) {
        widget_paint(iter, c);

        if (!has_fullscreen_win) {
          has_fullscreen_win = is_window_fullscreen(iter);
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
  }
  /*paint dialog and other*/
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (i >= start && iter->visible) {
    if (wm->system_bar != iter && !is_normal_window(iter)) {
      widget_paint(iter, c);
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
    .size = sizeof(window_manager_t),
    .is_window_manager = TRUE,
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
  widget_invalidate(widget, NULL);

  return RET_OK;
}

widget_t* window_manager_init(window_manager_t* wm) {
  widget_t* w = &(wm->widget);
  return_value_if_fail(wm != NULL, NULL);

  widget_init(w, NULL, &s_window_manager_vtable, 0, 0, 0, 0);

  locale_info_on(locale_info(), EVT_LOCALE_CHANGED, wm_on_locale_changed, wm);

  return w;
}

static ret_t window_manager_layout_child(widget_t* widget, widget_t* window) {
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
    if (is_window_fullscreen(window)) {
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
  } else if (is_system_bar(window)) {
    x = 0;
    y = 0;
    w = widget->w;
  } else if (is_dialog(window)) {
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

ret_t window_manager_layout_children(widget_t* widget) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  window_manager_layout_child(widget, iter);
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

  return window_manager_layout_children(widget);
}

ret_t window_manager_dispatch_input_event(widget_t* widget, event_t* e) {
  input_device_status_t* ids = NULL;
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && e != NULL, RET_BAD_PARAMS);

  window_manager_start_or_reset_screen_saver_timer(wm);

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

ret_t window_manager_set_screen_saver_time(widget_t* widget, uint32_t screen_saver_time) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->screen_saver_time = screen_saver_time;
  window_manager_start_or_reset_screen_saver_timer(wm);

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

ret_t window_manager_back(widget_t* widget) {
  event_t e;
  widget_t* top_window = window_manager_get_top_window(widget);
  return_value_if_fail(top_window != NULL, RET_NOT_FOUND);

  if (is_normal_window(top_window)) {
    e = event_init(EVT_REQUEST_CLOSE_WINDOW, top_window);
    return widget_dispatch(top_window, &e);
  } else {
    log_warn("not support call window_manager_back on non-normal window\n");
    return RET_FAIL;
  }
}

static ret_t window_manager_back_to_home_sync(widget_t* widget) {
  uint32_t k = 0;
  darray_t wins;
  widget_t* top = NULL;
  widget_t* home = NULL;
  int32_t children_nr = widget_count_children(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (children_nr < 2) {
    return RET_OK;
  }

  darray_init(&wins, 10, NULL, NULL);
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (home == NULL) {
    if (is_normal_window(iter)) {
      home = iter;
    }
  } else if ((i + 1) < children_nr) {
    if (!is_system_bar(iter)) {
      darray_push(&wins, iter);
    }
  }
  WIDGET_FOR_EACH_CHILD_END()

  for (k = 0; k < wins.size; k++) {
    widget_t* iter = WIDGET(wins.elms[k]);
    assert(!is_dialog(iter));
    window_manager_close_window_force(widget, iter);
  }
  darray_deinit(&wins);

  children_nr = widget_count_children(widget);
  top = widget_get_child(widget, children_nr - 1);
  return_value_if_fail(top != home, RET_OK);

  return window_manager_close_window(widget, top);
}

static ret_t window_manager_back_to_home_async(const idle_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);

  window_manager_back_to_home_sync(widget);

  return RET_REMOVE;
}

static ret_t window_manager_back_to_home_on_dialog_destroy(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);

  window_manager_back_to_home_sync(widget);

  return RET_REMOVE;
}

ret_t window_manager_back_to_home(widget_t* widget) {
  widget_t* top = NULL;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  top = window_manager_get_top_window(widget);
  return_value_if_fail(top != NULL, RET_BAD_PARAMS);

  if (!is_dialog(top) || !dialog_is_modal(top)) {
    idle_add(window_manager_back_to_home_async, widget);

    return RET_OK;
  } else {
    if (dialog_is_quited(top)) {
      widget_on(top, EVT_DESTROY, window_manager_back_to_home_on_dialog_destroy, widget);
    } else {
      log_warn("not support call window_manager_back_to_home on dialog\n");
    }

    return RET_FAIL;
  }
}

ret_t window_manager_set_dialog_highlighter(widget_t* widget, dialog_highlighter_t* highlighter) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->dialog_highlighter = highlighter;

  return RET_OK;
}

ret_t window_manager_paint_system_bar(widget_t* widget, canvas_t* c) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && c != NULL, RET_BAD_PARAMS);

  if (wm->system_bar != NULL && wm->system_bar->visible) {
    widget_paint(wm->system_bar, c);
  }

  return RET_OK;
}
