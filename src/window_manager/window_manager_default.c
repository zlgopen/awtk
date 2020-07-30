/**
 * File:   window_manager_default.c
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
#include "base/dialog.h"
#include "base/locale_info.h"
#include "base/system_info.h"
#include "base/image_manager.h"
#include "base/dialog_highlighter_factory.h"
#include "window_manager/window_manager_default.h"

static ret_t window_manager_animate_done(widget_t* widget);
static ret_t window_manager_default_inc_fps(widget_t* widget);
static ret_t window_manager_default_update_fps(widget_t* widget);
static ret_t window_manager_default_invalidate(widget_t* widget, rect_t* r);
static ret_t window_manager_default_get_client_r(widget_t* widget, rect_t* r);
static ret_t window_manager_default_do_open_window(widget_t* wm, widget_t* window);
static ret_t window_manager_default_layout_child(widget_t* widget, widget_t* window);
static ret_t window_manager_default_layout_system_bar(widget_t* widget, widget_t* window);
static ret_t window_manager_default_create_dialog_highlighter(widget_t* widget, widget_t* curr_win);
static ret_t window_manager_default_layout_not_system_bar(widget_t* widget, widget_t* window,
                                                          rect_t client_r);

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

static ret_t wm_on_screen_saver_timer(const timer_info_t* info) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(info->ctx);
  event_t e = event_init(EVT_SCREEN_SAVER, wm);
  wm->screen_saver_timer_id = TK_INVALID_ID;

  widget_dispatch(WIDGET(wm), &e);
  log_debug("emit: EVT_SCREEN_SAVER\n");

  return RET_REMOVE;
}

static ret_t window_manager_start_or_reset_screen_saver_timer(window_manager_default_t* wm) {
  if (wm->screen_saver_time > 0) {
    if (wm->screen_saver_timer_id == TK_INVALID_ID) {
      wm->screen_saver_timer_id = timer_add(wm_on_screen_saver_timer, wm, wm->screen_saver_time);
    } else {
      timer_modify(wm->screen_saver_timer_id, wm->screen_saver_time);
    }
  }

  return RET_OK;
}

static widget_t* window_manager_find_prev_window(widget_t* widget) {
  int32_t i = 0;
  int32_t nr = 0;
  return_value_if_fail(widget != NULL, NULL);

  if (widget->children != NULL && widget->children->size > 0) {
    nr = widget->children->size;
    for (i = nr - 2; i >= 0; i--) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (widget_is_normal_window(iter) || widget_is_dialog(iter) || widget_is_popup(iter)) {
        return iter;
      }
    }
  }

  return NULL;
}

static widget_t* window_manager_find_prev_any_window(widget_t* widget) {
  return_value_if_fail(widget != NULL, NULL);

  if (widget->children != NULL && widget->children->size >= 2) {
    WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
    if (i <= widget->children->size - 2 && widget_is_normal_window(iter) && iter->visible) {
      return iter;
    }
    WIDGET_FOR_EACH_CHILD_END();
  }

  return NULL;
}

ret_t window_manager_default_snap_curr_window(widget_t* widget, widget_t* curr_win, bitmap_t* img,
                                              framebuffer_object_t* fbo, bool_t auto_rotate) {
  canvas_t* c = NULL;
#ifdef WITH_NANOVG_GPU
  vgcanvas_t* vg = NULL;
#else
  rect_t r = {0};
#endif /*WITH_NANOVG_GPU*/

  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(img != NULL && fbo != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm != NULL && curr_win != NULL, RET_BAD_PARAMS);

  c = native_window_get_canvas(wm->native_window);
  window_manager_check_and_layout(widget);

#ifdef WITH_NANOVG_GPU
  vg = lcd_get_vgcanvas(c->lcd);
  ENSURE(vgcanvas_create_fbo(vg, vg->w, vg->h, fbo) == RET_OK);
  ENSURE(vgcanvas_bind_fbo(vg, fbo) == RET_OK);
  canvas_set_clip_rect(c, NULL);
  ENSURE(widget_on_paint_background(widget, c) == RET_OK);
  ENSURE(widget_paint(curr_win, c) == RET_OK);
  ENSURE(vgcanvas_unbind_fbo(vg, fbo) == RET_OK);
  fbo_to_img(fbo, img);
#else
  r = rect_init(curr_win->x, curr_win->y, curr_win->w, curr_win->h);
  ENSURE(canvas_begin_frame(c, &r, LCD_DRAW_OFFLINE) == RET_OK);
  canvas_set_clip_rect(c, &r);
  ENSURE(widget_on_paint_background(widget, c) == RET_OK);
  ENSURE(widget_paint(curr_win, c) == RET_OK);
  ENSURE(lcd_take_snapshot(c->lcd, img, auto_rotate) == RET_OK);
  ENSURE(canvas_end_frame(c) == RET_OK);
#endif

  return RET_OK;
}

ret_t window_manager_default_snap_prev_window(widget_t* widget, widget_t* prev_win, bitmap_t* img,
                                              framebuffer_object_t* fbo, bool_t auto_rotate) {
  rect_t r = {0};
  canvas_t* c = NULL;
#ifdef WITH_NANOVG_GPU
  vgcanvas_t* vg = NULL;
#endif /*WITH_NANOVG_GPU*/

  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  dialog_highlighter_t* dialog_highlighter = NULL;
  int32_t end = -1, start = -1;

  return_value_if_fail(img != NULL && fbo != NULL, RET_BAD_PARAMS);
  return_value_if_fail(wm != NULL && prev_win != NULL, RET_BAD_PARAMS);

  c = native_window_get_canvas(wm->native_window);
  dialog_highlighter = wm->dialog_highlighter;
  window_manager_check_and_layout(widget);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter == prev_win) {
    start = i;
    end = widget->children->size - 2;
    break;
  }
  WIDGET_FOR_EACH_CHILD_END()

  r = rect_init(prev_win->x, prev_win->y, prev_win->w, prev_win->h);

#ifdef WITH_NANOVG_GPU
  vg = lcd_get_vgcanvas(c->lcd);
  ENSURE(vgcanvas_create_fbo(vg, vg->w, vg->h, fbo) == RET_OK);
  ENSURE(vgcanvas_bind_fbo(vg, fbo) == RET_OK);
  canvas_set_clip_rect(c, &r);
  ENSURE(widget_on_paint_background(widget, c) == RET_OK);
  window_manager_paint_system_bar(widget, c);
  {
    widget_t** children = (widget_t**)(widget->children->elms);
    for (; start <= end; ++start) {
      widget_t* iter = children[start];
      if (widget_is_system_bar(iter) || !iter->visible) continue;
      /* 过滤 curr_win 的对象 */
      if (iter != wm->curr_win) {
        ENSURE(widget_paint(iter, c) == RET_OK);
      }
    }
  }

  if (dialog_highlighter != NULL) {
    dialog_highlighter_prepare(dialog_highlighter, c);
  }
  ENSURE(vgcanvas_unbind_fbo(vg, fbo) == RET_OK);
  fbo_to_img(fbo, img);
#else
  ENSURE(canvas_begin_frame(c, &r, LCD_DRAW_OFFLINE) == RET_OK);
  canvas_set_clip_rect(c, &r);
  ENSURE(widget_on_paint_background(widget, c) == RET_OK);
  window_manager_paint_system_bar(widget, c);
  {
    widget_t** children = (widget_t**)(widget->children->elms);
    for (; start <= end; ++start) {
      widget_t* iter = children[start];
      if (widget_is_system_bar(iter) || !iter->visible) continue;
      /* 过滤 curr_win 的对象 */
      if (iter != wm->curr_win) {
        ENSURE(widget_paint(iter, c) == RET_OK);
      }
    }
  }

  if (dialog_highlighter != NULL) {
    dialog_highlighter_prepare(dialog_highlighter, c);
  }
  ENSURE(lcd_take_snapshot(c->lcd, img, auto_rotate) == RET_OK);
  ENSURE(canvas_end_frame(c) == RET_OK);
#endif /*WITH_NANOVG_GPU*/

  if (dialog_highlighter != NULL) {
    dialog_highlighter_set_bg(dialog_highlighter, img, fbo);
    dialog_highlighter_set_bg_clip_rect(dialog_highlighter, &r);
  }
  wm->curr_win = NULL;
  return RET_OK;
}

static ret_t window_manager_on_highlighter_destroy(void* ctx, event_t* e) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(ctx);

  if (wm->dialog_highlighter == e->target) {
    wm->dialog_highlighter = NULL;
  }

  return RET_OK;
}

static dialog_highlighter_t* window_manager_default_get_dialog_highlighter(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  return wm->dialog_highlighter;
}

static ret_t window_manager_default_create_dialog_highlighter(widget_t* widget,
                                                              widget_t* curr_win) {
  value_t v;
  ret_t ret = RET_FAIL;
  dialog_highlighter_t* dialog_highlighter = NULL;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  dialog_highlighter = wm->dialog_highlighter;
  if (dialog_highlighter != NULL && dialog_highlighter->dialog != curr_win) {
    widget_t* dialog = dialog_highlighter->dialog;
    if (dialog != NULL) {
      widget_off_by_func(dialog, EVT_DESTROY, dialog_highlighter_on_dialog_destroy,
                         dialog_highlighter);
    }
    dialog_highlighter_destroy(dialog_highlighter);
    wm->dialog_highlighter = dialog_highlighter = NULL;
  }

  if (dialog_highlighter == NULL && (widget_is_dialog(curr_win) || widget_is_popup(curr_win)) &&
      widget_get_prop(curr_win, WIDGET_PROP_HIGHLIGHT, &v) == RET_OK) {
    const char* args = value_str(&v);
    if (args != NULL) {
      dialog_highlighter_factory_t* f = dialog_highlighter_factory();
      dialog_highlighter = dialog_highlighter_factory_create_highlighter(f, args, curr_win);

      if (dialog_highlighter != NULL) {
        wm->dialog_highlighter = dialog_highlighter;

        emitter_on(EMITTER(dialog_highlighter), EVT_DESTROY, window_manager_on_highlighter_destroy,
                   widget);
        ret = RET_OK;
      }
    }
  }

  return ret;
}

static ret_t window_manager_prepare_dialog_highlighter(widget_t* widget, widget_t* prev_win,
                                                       widget_t* curr_win) {
  if (window_manager_default_create_dialog_highlighter(widget, curr_win) == RET_OK) {
    bitmap_t img = {0};
    framebuffer_object_t fbo = {0};
    window_manager_default_snap_prev_window(widget, prev_win, &img, &fbo, TRUE);

    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t window_manager_create_highlighter(widget_t* widget, widget_t* curr_win) {
  widget_t* prev_win = window_manager_find_prev_any_window(widget);
  window_manager_prepare_dialog_highlighter(widget, prev_win, curr_win);
  return RET_OK;
}

static ret_t window_manager_create_animator(window_manager_default_t* wm, widget_t* curr_win,
                                            bool_t open) {
  value_t v;
  const char* anim_hint = NULL;
  widget_t* prev_win = window_manager_find_prev_any_window(WIDGET(wm));
  const char* key = open ? WIDGET_PROP_OPEN_ANIM_HINT : WIDGET_PROP_CLOSE_ANIM_HINT;

  if (prev_win == curr_win || prev_win == NULL) {
    return RET_FAIL;
  }

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
    canvas_t* c = native_window_get_canvas(wm->native_window);
    window_manager_default_create_dialog_highlighter(WIDGET(wm), curr_win);
    wm->curr_win = curr_win;
    if (open) {
      wm->animator = window_animator_create_for_open(anim_hint, c, prev_win, curr_win);
    } else {
      wm->animator = window_animator_create_for_close(anim_hint, c, prev_win, curr_win);
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
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(curr_win->parent);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  window_manager_dispatch_window_event(curr_win, EVT_WINDOW_WILL_OPEN);
  wm->ready_animator = FALSE;
  if (window_manager_create_animator(wm, curr_win, TRUE) != RET_OK) {
    widget_t* prev_win = window_manager_find_prev_window(WIDGET(wm));
    if (prev_win != NULL) {
      if (!widget_is_keyboard(curr_win)) {
        window_manager_dispatch_window_event(prev_win, EVT_WINDOW_TO_BACKGROUND);
      }
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
  widget_t* curr_win = WIDGET(info->ctx);
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(curr_win->parent);
  if (wm != NULL) {
    wm->ready_animator = FALSE;
    window_manager_dispatch_window_open(curr_win);
  }

  return RET_REMOVE;
}

static ret_t window_manager_check_if_need_close_animation(window_manager_default_t* wm,
                                                          widget_t* curr_win) {
  return window_manager_create_animator(wm, curr_win, FALSE);
}

static ret_t window_manager_default_do_open_window(widget_t* widget, widget_t* window) {
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

static ret_t window_manager_default_open_window(widget_t* widget, widget_t* window) {
  ret_t ret = RET_OK;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(widget != NULL && window != NULL, RET_BAD_PARAMS);

  wm->prev_win = window_manager_get_top_window(widget);

  if (wm->animator != NULL) {
    wm->pending_open_window = window;
  } else {
    wm->ready_animator = TRUE;
    window_manager_default_do_open_window(widget, window);
  }

  ret = widget_add_child(widget, window);
  return_value_if_fail(ret == RET_OK, RET_FAIL);
  window_manager_default_layout_child(widget, window);

  window->dirty = FALSE;
  widget->target = window;

  widget_invalidate(window, NULL);

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

  return RET_OK;
}

static ret_t window_manager_default_close_window(widget_t* widget, widget_t* window) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget_is_window(window), RET_BAD_PARAMS);
  return_value_if_fail(window_is_opened(window), RET_BAD_PARAMS);
  return_value_if_fail(wm->pending_close_window != window, RET_BAD_PARAMS);

  window_manager_prepare_close_window(widget, window);

  if (wm->animator) {
    wm->pending_close_window = window;
    return RET_OK;
  }

  window_manager_dispatch_window_event(window, EVT_WINDOW_CLOSE);
  if (window_manager_check_if_need_close_animation(wm, window) != RET_OK) {
    widget_t* prev_win = NULL;

    widget_remove_child(widget, window);
    idle_add(window_manager_idle_destroy_window, window);

    prev_win = window_manager_get_top_window(widget);
    if (prev_win != NULL) {
      if (!widget_is_keyboard(window)) {
        window_manager_dispatch_window_event(prev_win, EVT_WINDOW_TO_FOREGROUND);
        window_manager_create_highlighter(widget, prev_win);
      }
    }
  }

  return RET_OK;
}

static ret_t window_manager_default_close_window_force(widget_t* widget, widget_t* window) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget_is_window(window), RET_BAD_PARAMS);
  return_value_if_fail(wm->pending_close_window != window, RET_BAD_PARAMS);

  if (wm->animator != NULL) {
    if (wm->animator->prev_win == window || wm->animator->curr_win == window) {
      return window_manager_animate_done(widget);
    }
  }

  window_manager_prepare_close_window(widget, window);
  window_manager_dispatch_window_event(window, EVT_WINDOW_CLOSE);
  widget_remove_child(widget, window);
  widget_destroy(window);

  return RET_OK;
}

static widget_t* window_manager_default_find_target(widget_t* widget, xy_t x, xy_t y) {
  return window_manager_find_target(widget, NULL, x, y);
}

static ret_t window_manager_paint_cursor(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  if (wm->r_cursor.w > 0 && wm->r_cursor.h > 0) {
    return_value_if_fail(image_manager_get_bitmap(image_manager(), wm->cursor, &bitmap) == RET_OK,
                         RET_BAD_PARAMS);
    canvas_draw_icon(c, &bitmap, wm->r_cursor.x, wm->r_cursor.y);
  }

  return RET_OK;
}

static ret_t window_manager_update_cursor(widget_t* widget, int32_t x, int32_t y) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  uint32_t w = wm->r_cursor.w;
  uint32_t h = wm->r_cursor.h;

  if (w > 0 && h > 0) {
    uint32_t hw = w >> 1;
    uint32_t hh = h >> 1;
    int32_t oldx = wm->r_cursor.x;
    int32_t oldy = wm->r_cursor.y;
    rect_t r = rect_init(oldx - hw, oldy - hh, w, h);

    window_manager_default_invalidate(widget, &r);

    wm->r_cursor.x = x;
    wm->r_cursor.y = y;

    r = rect_init(x - hw, y - hh, w, h);

    window_manager_default_invalidate(widget, &r);
  }

  return RET_OK;
}

static ret_t window_manager_paint_normal(widget_t* widget, canvas_t* c) {
#ifdef FRAGMENT_FRAME_BUFFER_SIZE
  uint32_t i = 0;
  uint32_t y = 0;
  uint32_t h = 0;
  uint32_t tmp_h = 0;
  uint32_t number = 0;
#endif

  uint64_t start_time = time_now_ms();
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  window_manager_default_inc_fps(widget);

  if (WINDOW_MANAGER(wm)->show_fps) {
    rect_t fps_rect = rect_init(0, 0, 60, 30);
    window_manager_default_invalidate(widget, &fps_rect);
  }
#ifdef FRAGMENT_FRAME_BUFFER_SIZE
  if (wm->native_window->dirty_rect.w > 0 && wm->native_window->dirty_rect.h > 0) {
    rect_t r = native_window_calc_dirty_rect(wm->native_window);
    if (r.w > 0 && r.h > 0) {
      assert(r.w <= FRAGMENT_FRAME_BUFFER_SIZE);
      y = r.y;
      h = r.h;
      tmp_h = FRAGMENT_FRAME_BUFFER_SIZE / r.w;
      number = r.h / tmp_h;

      for (i = 0; i <= number; i++) {
        r.y = y + i * tmp_h;
        if (i == number) {
          tmp_h = h % tmp_h;
        }
        r.h = tmp_h;
        if (r.h == 0) {
          break;
        }

        canvas_t* c = native_window_get_canvas(wm->native_window);
        canvas_begin_frame(c, &r, LCD_DRAW_NORMAL);
        wm->native_window->dirty = TRUE;
        ENSURE(widget_paint(WIDGET(wm), c) == RET_OK);
        window_manager_paint_cursor(widget, c);
        canvas_end_frame(c);
      }

      native_window_update_last_dirty_rect(wm->native_window);
    }
    native_window_clear_dirty_rect(wm->native_window);
  }
#else
  if (native_window_begin_frame(wm->native_window, LCD_DRAW_NORMAL) == RET_OK) {
    ENSURE(widget_paint(WIDGET(wm), c) == RET_OK);
    window_manager_paint_cursor(widget, c);
    native_window_end_frame(wm->native_window);
  }
#endif
  wm->last_paint_cost = time_now_ms() - start_time;

  return RET_OK;
}

#ifdef WITH_WINDOW_ANIMATORS
static ret_t window_manager_invalidate_system_bar(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (widget_is_system_bar(iter)) {
    widget_invalidate_force(iter, NULL);
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

static ret_t window_manager_animate_done_set_window_foreground(widget_t* widget, widget_t* prev_win,
                                                               widget_t* curr_win) {
  bool_t is_set = FALSE;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (prev_win == iter) {
    is_set = TRUE;
  } else if (curr_win == iter) {
    is_set = FALSE;
  }

  if (is_set) {
    window_manager_dispatch_window_event(iter, EVT_WINDOW_TO_FOREGROUND);
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

static ret_t window_manager_animate_done(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  bool_t curr_win_is_keyboard = widget_is_keyboard(wm->animator->curr_win);

  if (wm->animator != NULL) {
    bool_t is_open = wm->animator->open;
    widget_t* prev_win = wm->animator->prev_win;
    widget_t* curr_win = wm->animator->curr_win;
    window_animator_destroy(wm->animator);

    wm->animator = NULL;
    wm->animating = FALSE;
    wm->ignore_user_input = FALSE;

    if (is_open) {
      if (!curr_win_is_keyboard) {
        window_manager_dispatch_window_event(prev_win, EVT_WINDOW_TO_BACKGROUND);
      }
      window_manager_dispatch_window_event(curr_win, EVT_WINDOW_OPEN);
    } else {
      if (!curr_win_is_keyboard) {
        window_manager_animate_done_set_window_foreground(widget, prev_win, curr_win);
        window_manager_create_highlighter(widget, prev_win);
      }
    }

    if (wm->pending_close_window != NULL) {
      widget_t* window = wm->pending_close_window;
      wm->pending_close_window = NULL;
      if (wm->pending_open_window != NULL) {
        window_manager_close_window_force(widget, window);
      } else {
        window_manager_close_window(widget, window);
      }
    }

    if (wm->pending_open_window != NULL) {
      widget_t* window = wm->pending_open_window;
      wm->pending_open_window = NULL;
      window_manager_default_do_open_window(widget, window);
    }

    window_manager_invalidate_system_bar(widget);
  }

  return RET_OK;
}

static ret_t window_manager_paint_animation(widget_t* widget, canvas_t* c) {
  paint_event_t e;
  uint64_t start_time = time_now_ms();
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  ENSURE(window_animator_begin_frame(wm->animator) == RET_OK);

  widget_dispatch(widget, paint_event_init(&e, EVT_BEFORE_PAINT, widget, c));

  ret_t ret = window_animator_update(wm->animator, start_time);

  widget_dispatch(widget, paint_event_init(&e, EVT_AFTER_PAINT, widget, c));

  ENSURE(window_animator_end_frame(wm->animator) == RET_OK);

  wm->last_paint_cost = time_now_ms() - start_time;
  window_manager_default_inc_fps(widget);

  if (ret == RET_DONE) {
    window_manager_animate_done(widget);
  }

  return RET_OK;
}
#else
static ret_t window_manager_animate_done(widget_t* widget) {
  return RET_OK;
}
#endif /*WITH_WINDOW_ANIMATORS*/

static ret_t window_manager_default_inc_fps(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  wm->fps_count++;

  return RET_OK;
}

static ret_t window_manager_default_update_fps(widget_t* widget) {
  canvas_t* c = NULL;
  uint32_t elapse = 0;
  uint64_t now = time_now_ms();
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  elapse = now - wm->fps_time;
  if (elapse >= 200) {
    wm->fps = wm->fps_count * 1000 / elapse;

    wm->fps_time = now;
    wm->fps_count = 0;
  }

  c = native_window_get_canvas(wm->native_window);
  canvas_set_fps(c, WINDOW_MANAGER(wm)->show_fps, wm->fps);

  return RET_OK;
}

static ret_t window_manager_default_paint(widget_t* widget) {
  ret_t ret = RET_OK;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  canvas_t* c = native_window_get_canvas(wm->native_window);
  return_value_if_fail(wm != NULL && c != NULL, RET_BAD_PARAMS);

  canvas_set_global_alpha(c, 0xff);
  window_manager_default_update_fps(widget);

#ifdef WITH_WINDOW_ANIMATORS
  if (wm->animator != NULL) {
    ret = window_manager_paint_animation(widget, c);
  } else if (!wm->ready_animator) {
    ret = window_manager_paint_normal(widget, c);
  }
#else
  ret = window_manager_paint_normal(widget, c);
#endif /*WITH_WINDOW_ANIMATORS*/
  return ret;
}

static ret_t window_manager_default_invalidate(widget_t* widget, rect_t* r) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  if (wm->native_window != NULL) {
    native_window_invalidate(wm->native_window, r);
  }

  return RET_OK;
}

static widget_t* window_manager_default_get_prev_window(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

  return wm->prev_win;
}

static ret_t window_manager_default_on_paint_children(widget_t* widget, canvas_t* c) {
  int32_t start = 0;
  bool_t has_fullscreen_win = FALSE;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (wm->dialog_highlighter != NULL) {
    if (wm->dialog_highlighter->dialog == iter) {
      start = i;
      break;
    }
  } else if (iter->visible && widget_is_normal_window(iter)) {
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
  }
  /*paint dialog and other*/
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (i >= start && iter->visible) {
    if (!widget_is_system_bar(iter) && !widget_is_normal_window(iter)) {
      widget_paint(iter, c);
    }
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

static ret_t window_manager_default_on_remove_child(widget_t* widget, widget_t* window) {
  widget_t* top = window_manager_get_top_main_window(widget);

  if (top != NULL) {
    rect_t r;
    r = rect_init(window->x, window->y, window->w, window->h);
    widget_invalidate(top, &r);
  }

  if (widget->destroying) {
    widget_off_by_ctx(window, widget);
  }

  return RET_FAIL;
}

static ret_t window_manager_default_get_prop(widget_t* widget, const char* name, value_t* v) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_POINTER_CURSOR)) {
    value_set_str(v, wm->cursor);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CANVAS)) {
    canvas_t* c = native_window_get_canvas(wm->native_window);
    value_set_pointer(v, c);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t window_manager_default_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_POINTER_CURSOR)) {
    return window_manager_set_cursor(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t window_manager_default_on_destroy(widget_t* widget) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);

#ifdef WITH_WINDOW_ANIMATORS
  if (wm->animator != NULL) {
    wm->animator->prev_win = NULL;
    wm->animator->curr_win = NULL;
    window_animator_destroy(wm->animator);
    wm->animator = NULL;
  }
#endif /*WITH_WINDOW_ANIMATORS*/

  object_unref(OBJECT(wm->native_window));

  return RET_OK;
}

static ret_t window_manager_default_on_layout_children(widget_t* widget) {
  rect_t client_r = rect_init(0, 0, widget->w, widget->h);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (widget_is_system_bar(iter)) {
    window_manager_default_layout_system_bar(widget, iter);
  }
  WIDGET_FOR_EACH_CHILD_END();

  window_manager_default_get_client_r(widget, &client_r);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (!widget_is_system_bar(iter)) {
    window_manager_default_layout_not_system_bar(widget, iter, client_r);
  }
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t window_manager_default_resize(widget_t* widget, wh_t w, wh_t h);
static ret_t window_manager_default_post_init(widget_t* widget, wh_t w, wh_t h);
static ret_t window_manager_default_set_cursor(widget_t* widget, const char* cursor);
static ret_t window_manager_default_set_show_fps(widget_t* widget, bool_t show_fps);
static ret_t window_manager_default_dispatch_input_event(widget_t* widget, event_t* e);
static ret_t window_manager_default_set_screen_saver_time(widget_t* widget,
                                                          uint32_t screen_saver_time);

static ret_t window_manager_default_get_pointer(widget_t* widget, xy_t* x, xy_t* y,
                                                bool_t* pressed) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
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

static ret_t window_manager_default_is_animating(widget_t* widget, bool_t* playing) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  *playing = wm->animating;
  return RET_OK;
}

ret_t window_manager_default_on_event(widget_t* widget, event_t* e) {
  if (e->type == EVT_ORIENTATION_WILL_CHANGED) {
    orientation_event_t* evt = orientation_event_cast(e);
    lcd_orientation_t orientation = evt->orientation;
    window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
    lcd_t* lcd = native_window_get_canvas(wm->native_window)->lcd;

    wh_t w = wm->lcd_w;
    wh_t h = wm->lcd_h;
    if (orientation == LCD_ORIENTATION_90 || orientation == LCD_ORIENTATION_270) {
      w = wm->lcd_h;
      h = wm->lcd_w;
    }

    lcd_resize(lcd, w, h, 0);
    window_manager_default_resize(widget, w, h);
    e->type = EVT_ORIENTATION_CHANGED;

    widget_dispatch(widget, e);
  } else if (e->type == EVT_THEME_CHANGED) {
    window_manager_on_theme_changed(widget);
  }

  return RET_OK;
}

static bool_t widget_is_system_bar_top(widget_t* window) {
  return tk_str_eq(window->vt->type, WIDGET_TYPE_SYSTEM_BAR);
}

static bool_t widget_is_system_bar_bottom(widget_t* window) {
  return tk_str_eq(window->vt->type, WIDGET_TYPE_SYSTEM_BAR_BOTTOM);
}

static ret_t window_manager_default_get_client_r(widget_t* widget, rect_t* r) {
  *r = rect_init(0, 0, widget->w, widget->h);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (widget_is_system_bar_top(iter)) {
    r->y = iter->h;
    r->h -= iter->h;
  }

  if (widget_is_system_bar_bottom(iter)) {
    r->h -= iter->h;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t window_manager_default_layout_system_bar(widget_t* widget, widget_t* window) {
  window->x = 0;
  window->w = widget->w;

  if (widget_is_system_bar_bottom(window)) {
    window->y = widget->h - window->h;
  }

  widget_layout(window);

  return RET_OK;
}

static ret_t window_manager_default_layout_not_system_bar(widget_t* widget, widget_t* window,
                                                          rect_t client_r) {
  xy_t x = window->x;
  xy_t y = window->y;
  wh_t w = window->w;
  wh_t h = window->h;

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

static ret_t window_manager_default_layout_child(widget_t* widget, widget_t* window) {
  if (widget_is_system_bar(window)) {
    return window_manager_default_layout_system_bar(widget, window);
  } else {
    rect_t client_r = rect_init(0, 0, widget->w, widget->h);
    window_manager_default_get_client_r(widget, &client_r);
    return window_manager_default_layout_not_system_bar(widget, window, client_r);
  }
}

static ret_t window_manager_default_resize(widget_t* widget, wh_t w, wh_t h) {
  rect_t r = rect_init(0, 0, w, h);
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  widget_move_resize(widget, 0, 0, w, h);

  native_window_resize(wm->native_window, w, h, TRUE);
  native_window_invalidate(wm->native_window, &r);
  native_window_update_last_dirty_rect(wm->native_window);

  return widget_layout_children(widget);
}

static ret_t window_manager_default_post_init(widget_t* widget, wh_t w, wh_t h) {
  native_window_info_t info;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->lcd_w = w;
  wm->lcd_h = h;
  wm->native_window = native_window_create(widget);

  if (native_window_get_info(wm->native_window, &info) == RET_OK) {
    w = info.w;
    h = info.h;
  }

  window_manager_default_resize(widget, w, h);

  return RET_OK;
}

static ret_t window_manager_default_dispatch_input_event(widget_t* widget, event_t* e) {
  input_device_status_t* ids = NULL;
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL && e != NULL, RET_BAD_PARAMS);

  window_manager_start_or_reset_screen_saver_timer(wm);

  native_window_preprocess_event(wm->native_window, e);
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

static ret_t window_manager_default_set_show_fps(widget_t* widget, bool_t show_fps) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->show_fps = show_fps;

  return RET_OK;
}

static ret_t window_manager_default_set_screen_saver_time(widget_t* widget,
                                                          uint32_t screen_saver_time) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->screen_saver_time = screen_saver_time;
  window_manager_start_or_reset_screen_saver_timer(wm);

  return RET_OK;
}

static ret_t window_manager_default_set_cursor(widget_t* widget, const char* cursor) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

#if defined(ENABLE_CURSOR)
  if (tk_str_eq(cursor, wm->cursor)) {
    return RET_OK;
  }

  tk_strncpy(wm->cursor, cursor, TK_NAME_LEN);
  if (cursor != NULL && *cursor) {
    bitmap_t bitmap;
    bitmap_t* img = NULL;

    if (image_manager_get_bitmap(image_manager(), cursor, &bitmap) == RET_OK) {
      img = &bitmap;
    }

    if (native_window_set_cursor(wm->native_window, cursor, img) != RET_OK) {
      wm->r_cursor.w = bitmap.w;
      wm->r_cursor.h = bitmap.h;
    } else {
      wm->r_cursor.w = 0;
      wm->r_cursor.h = 0;
    }
  }
#endif /*ENABLE_CURSOR*/

  return RET_OK;
}

ret_t window_manager_paint_system_bar(widget_t* widget, canvas_t* c) {
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  return_value_if_fail(wm != NULL && c != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (widget_is_system_bar(iter)) {
    widget_paint(iter, c);
  }
  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

static ret_t window_manager_default_native_window_resized(widget_t* widget, void* handle) {
  uint32_t w = 0;
  uint32_t h = 0;
  system_info_t* info = system_info();
  window_manager_default_t* wm = WINDOW_MANAGER_DEFAULT(widget);
  native_window_t* nw = WINDOW_MANAGER_DEFAULT(widget)->native_window;

  if (info->lcd_orientation == LCD_ORIENTATION_90 || info->lcd_orientation == LCD_ORIENTATION_270) {
    w = info->lcd_h;
    h = info->lcd_w;
  } else {
    w = info->lcd_w;
    h = info->lcd_h;
  }

  window_manager_default_resize(widget, w, h);
  native_window_on_resized(nw, w, h);

  if (wm->dialog_highlighter != NULL) {
    bitmap_t img;
    framebuffer_object_t fbo;
    widget_t* prev = window_manager_get_prev_window(widget);

    memset(&img, 0x00, sizeof(img));
    memset(&fbo, 0x00, sizeof(fbo));
    if (prev != NULL) {
      dialog_highlighter_clear_image(wm->dialog_highlighter);
      window_manager_default_snap_prev_window(widget, prev, &img, &fbo, FALSE);
    }
  }

  return RET_OK;
}

static ret_t window_manager_default_dispatch_native_window_event(widget_t* widget, event_t* e,
                                                                 void* handle) {
  if (e->type == EVT_NATIVE_WINDOW_RESIZED) {
    window_manager_default_native_window_resized(widget, handle);
  }

  return RET_OK;
}

static window_manager_vtable_t s_window_manager_self_vtable = {
    .paint = window_manager_default_paint,
    .post_init = window_manager_default_post_init,
    .set_cursor = window_manager_default_set_cursor,
    .open_window = window_manager_default_open_window,
    .get_pointer = window_manager_default_get_pointer,
    .is_animating = window_manager_default_is_animating,
    .close_window = window_manager_default_close_window,
    .set_show_fps = window_manager_default_set_show_fps,
    .get_prev_window = window_manager_default_get_prev_window,
    .close_window_force = window_manager_default_close_window_force,
    .dispatch_input_event = window_manager_default_dispatch_input_event,
    .dispatch_native_window_event = window_manager_default_dispatch_native_window_event,
    .snap_curr_window = window_manager_default_snap_curr_window,
    .snap_prev_window = window_manager_default_snap_prev_window,
    .get_dialog_highlighter = window_manager_default_get_dialog_highlighter,
    .set_screen_saver_time = window_manager_default_set_screen_saver_time,
    .resize = window_manager_default_resize};

static const widget_vtable_t s_window_manager_vtable = {
    .size = sizeof(window_manager_t),
    .is_window_manager = TRUE,
    .type = WIDGET_TYPE_WINDOW_MANAGER,
    .set_prop = window_manager_default_set_prop,
    .get_prop = window_manager_default_get_prop,
    .on_event = window_manager_default_on_event,
    .invalidate = window_manager_default_invalidate,
    .on_layout_children = window_manager_default_on_layout_children,
    .on_paint_children = window_manager_default_on_paint_children,
    .on_remove_child = window_manager_default_on_remove_child,
    .find_target = window_manager_default_find_target,
    .on_destroy = window_manager_default_on_destroy};

widget_t* window_manager_create(void) {
  window_manager_default_t* wm = TKMEM_ZALLOC(window_manager_default_t);
  return_value_if_fail(wm != NULL, NULL);
  wm->ready_animator = FALSE;
  return window_manager_init(WINDOW_MANAGER(wm), &s_window_manager_vtable,
                             &s_window_manager_self_vtable);
}
