/**
 * File:   window_animator.c
 * Author: AWTK Develop Team
 * Brief:  window_animator
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
 * 2018-04-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/window_animator.h"
#include "base/window_manager.h"
#include "base/dialog_highlighter_factory.h"

static bool_t window_animator_is_overlap(window_animator_t* wa);
static ret_t window_animator_paint_system_bar(window_animator_t* wa);
static ret_t window_animator_update_percent(window_animator_t* wa);
static ret_t window_animator_draw_prev_window(window_animator_t* wa);
static ret_t window_animator_draw_curr_window(window_animator_t* wa);
static ret_t window_animator_begin_frame_normal(window_animator_t* wa);
static ret_t window_animator_begin_frame_overlap(window_animator_t* wa);

static ret_t window_animator_prepare_highligter(window_animator_t* wa, canvas_t* c) {
  value_t v;
  widget_t* curr_win = wa->curr_win;
  widget_t* wm = wa->curr_win->parent;

  if (wa->open && window_animator_is_overlap(wa)) {
    if (widget_get_prop(curr_win, WIDGET_PROP_HIGHLIGHT, &v) == RET_OK) {
      dialog_highlighter_factory_t* f = dialog_highlighter_factory();
      wa->dialog_highlighter = dialog_highlighter_factory_create_highlighter(f, value_str(&v));
    }

    window_manager_set_dialog_highlighter(wm, wa->dialog_highlighter);
  } else {
    wa->dialog_highlighter = WINDOW_MANAGER(wm)->dialog_highlighter;
  }

  if (wa->dialog_highlighter != NULL) {
    dialog_highlighter_prepare(wa->dialog_highlighter, c);
  }

  return RET_OK;
}

static ret_t window_animator_open_destroy(window_animator_t* wa) {
#ifdef WITH_NANOVG_GPU
  vgcanvas_t* vg = lcd_get_vgcanvas(wa->canvas->lcd);
  if (wa->dialog_highlighter == NULL) {
    vgcanvas_destroy_fbo(vg, &(wa->prev_fbo));
  }
  vgcanvas_destroy_fbo(vg, &(wa->curr_fbo));
#else
  if (wa->dialog_highlighter == NULL) {
    bitmap_destroy(&(wa->prev_img));
  }
  bitmap_destroy(&(wa->curr_img));
#endif /*WITH_NANOVG_GPU*/

  memset(wa, 0x00, sizeof(window_animator_t));
  TKMEM_FREE(wa);

  return RET_OK;
}

static ret_t window_animator_close_destroy(window_animator_t* wa) {
  widget_t* wm = wa->curr_win->parent;

  widget_destroy(wa->curr_win);

  if (wa->dialog_highlighter != NULL) {
    dialog_highlighter_destroy(wa->dialog_highlighter);
    window_manager_set_dialog_highlighter(wm, NULL);
  }

  return window_animator_open_destroy(wa);
}

static bool_t window_animator_is_overlap(window_animator_t* wa) {
  return_value_if_fail(wa != NULL && wa->vt != NULL, FALSE);

  return wa->vt->overlap;
}

static ret_t window_animator_begin_frame(window_animator_t* wa) {
  return_value_if_fail(wa != NULL, RET_OK);

  if (window_animator_is_overlap(wa)) {
    return window_animator_begin_frame_overlap(wa);
  } else {
    return window_animator_begin_frame_normal(wa);
  }
}

static ret_t window_animator_end_frame(window_animator_t* wa) {
  canvas_end_frame(wa->canvas);

  return RET_OK;
}

ret_t window_animator_update(window_animator_t* wa, uint32_t time_ms) {
  return_value_if_fail(wa != NULL, RET_FAIL);

  if (wa->start_time == 0) {
    wa->start_time = time_ms;
  }

  wa->time_percent = (time_ms - wa->start_time) / (float_t)(wa->duration);
  if (wa->time_percent >= 1) {
    wa->time_percent = 1;
  }

  ENSURE(window_animator_update_percent(wa) == RET_OK);
  ENSURE(window_animator_begin_frame(wa) == RET_OK);
  ENSURE(window_animator_draw_prev_window(wa) == RET_OK);
  ENSURE(window_animator_draw_curr_window(wa) == RET_OK);
  ENSURE(window_animator_end_frame(wa) == RET_OK);

  return wa->time_percent >= 1 ? RET_DONE : RET_OK;
}

ret_t window_animator_destroy(window_animator_t* wa) {
  return_value_if_fail(wa != NULL, RET_FAIL);

  if (wa->open) {
    return window_animator_open_destroy(wa);
  } else {
    return window_animator_close_destroy(wa);
  }
}

/******************helper******************/

static ret_t window_animator_paint_system_bar(window_animator_t* wa) {
  window_manager_t* wm = WINDOW_MANAGER(wa->curr_win->parent);

  if (!(wa->canvas->lcd->support_dirty_rect) && wm->system_bar) {
    widget_paint(wm->system_bar, wa->canvas);
  }

  return RET_OK;
}

static ret_t window_animator_begin_frame_normal(window_animator_t* wa) {
#ifdef WITH_NANOVG_GPU
#else
  rect_t r;
  widget_t* wm = wa->curr_win->parent;
  r = rect_init(wm->x, wm->y, wm->w, wm->h);
  ENSURE(canvas_begin_frame(wa->canvas, &r, LCD_DRAW_ANIMATION) == RET_OK);
#endif

  return window_animator_paint_system_bar(wa);
}

static ret_t window_animator_begin_frame_overlap(window_animator_t* wa) {
#ifdef WITH_NANOVG_GPU
  (void)wa;
#else
  rect_t r;
  widget_t* w = NULL;

  if (wa->percent > 0) {
    w = wa->curr_win;
  } else {
    w = wa->curr_win->parent;
  }

  r = rect_init(w->x, w->y, w->w, w->h);
  ENSURE(canvas_begin_frame(wa->canvas, &r, LCD_DRAW_ANIMATION_OVERLAP) == RET_OK);
#endif

  return window_animator_paint_system_bar(wa);
}

#ifdef WITH_NANOVG_GPU
static ret_t fbo_to_img(framebuffer_object_t* fbo, bitmap_t* img) {
  return_value_if_fail(fbo != NULL && img != NULL, RET_BAD_PARAMS);

  memset(img, 0x00, sizeof(bitmap_t));
  img->specific = (char*)NULL + fbo->id;
  img->specific_ctx = NULL;
  img->specific_destroy = NULL;
  img->w = fbo->w * fbo->ratio;
  img->h = fbo->h * fbo->ratio;

  img->flags = BITMAP_FLAG_TEXTURE;

  return RET_OK;
}

ret_t window_animator_prepare(window_animator_t* wa, canvas_t* c, widget_t* prev_win,
                              widget_t* curr_win) {
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  widget_t* wm = prev_win->parent;
  wa->canvas = c;
  wa->prev_win = prev_win;
  wa->curr_win = curr_win;
  wa->duration = wa->duration ? wa->duration : 500;

  ENSURE(vgcanvas_create_fbo(vg, &(wa->prev_fbo)) == RET_OK);
  ENSURE(vgcanvas_bind_fbo(vg, &(wa->prev_fbo)) == RET_OK);
  vgcanvas_scale(vg, 1, 1);
  ENSURE(widget_on_paint_background(wm, c) == RET_OK);
  ENSURE(widget_paint(prev_win, c) == RET_OK);
  window_animator_prepare_highligter(wa, c);
  ENSURE(vgcanvas_unbind_fbo(vg, &(wa->prev_fbo)) == RET_OK);

  ENSURE(vgcanvas_create_fbo(vg, &(wa->curr_fbo)) == RET_OK);
  ENSURE(vgcanvas_bind_fbo(vg, &(wa->curr_fbo)) == RET_OK);
  vgcanvas_scale(vg, 1, 1);
  ENSURE(widget_on_paint_background(wm, c) == RET_OK);
  ENSURE(widget_paint(curr_win, c) == RET_OK);
  ENSURE(vgcanvas_unbind_fbo(vg, &(wa->curr_fbo)) == RET_OK);

  fbo_to_img(&(wa->prev_fbo), &(wa->prev_img));
  fbo_to_img(&(wa->curr_fbo), &(wa->curr_img));
  wa->ratio = wa->curr_fbo.ratio;

  if (wa->dialog_highlighter != NULL) {
    dialog_highlighter_set_bg(wa->dialog_highlighter, &(wa->prev_img), &(wa->prev_fbo));
  }

  return RET_OK;
}
#elif defined(WITH_NANOVG_SOFT)
ret_t window_animator_prepare(window_animator_t* wa, canvas_t* c, widget_t* prev_win,
                              widget_t* curr_win) {
  rect_t r;
  lcd_t* lcd = c->lcd;
  bool_t auto_rotate = FALSE;
  widget_t* wm = prev_win->parent;

  wa->ratio = 1;
  wa->canvas = c;
  wa->prev_win = prev_win;
  wa->curr_win = curr_win;
  r = rect_init(0, 0, wm->w, wm->h);
  wa->duration = wa->duration ? wa->duration : 500;

  if (!(window_animator_is_overlap(wa))) {
    auto_rotate = TRUE;
  }

  r = rect_init(prev_win->x, prev_win->y, prev_win->w, prev_win->h);
  ENSURE(canvas_begin_frame(c, &r, LCD_DRAW_OFFLINE) == RET_OK);
  canvas_set_clip_rect(c, &r);
  ENSURE(widget_on_paint_background(wm, c) == RET_OK);
  ENSURE(widget_paint(prev_win, c) == RET_OK);
  window_animator_prepare_highligter(wa, c);
  ENSURE(lcd_take_snapshot(lcd, &(wa->prev_img), auto_rotate) == RET_OK);
  ENSURE(canvas_end_frame(c) == RET_OK);

  r = rect_init(curr_win->x, curr_win->y, curr_win->w, curr_win->h);
  ENSURE(canvas_begin_frame(c, &r, LCD_DRAW_OFFLINE) == RET_OK);
  canvas_set_clip_rect(c, &r);
  ENSURE(widget_on_paint_background(wm, c) == RET_OK);
  ENSURE(widget_paint(curr_win, c) == RET_OK);
  ENSURE(lcd_take_snapshot(lcd, &(wa->curr_img), auto_rotate) == RET_OK);
  ENSURE(canvas_end_frame(c) == RET_OK);

  wa->prev_img.flags = BITMAP_FLAG_OPAQUE;
  wa->curr_img.flags = BITMAP_FLAG_OPAQUE;

  if (wa->dialog_highlighter != NULL) {
    dialog_highlighter_set_bg(wa->dialog_highlighter, &(wa->prev_img), NULL);
  }

  return RET_OK;
}
#else
ret_t window_animator_prepare(window_animator_t* wa, canvas_t* c, widget_t* prev_win,
                              widget_t* curr_win) {
  return RET_OK;
}
#endif /*WITH_NANOVG_GPU|WITH_NANOVG_SOFT*/

window_animator_t* window_animator_create(bool_t open, const window_animator_vtable_t* vt) {
  window_animator_t* wa = NULL;
  return_value_if_fail(vt != NULL && vt->size >= sizeof(window_animator_t), NULL);

  wa = (window_animator_t*)TKMEM_ALLOC(vt->size);
  return_value_if_fail(wa != NULL, NULL);

  memset(wa, 0, vt->size);

  wa->vt = vt;
  wa->open = open;
  wa->easing = easing_get(EASING_CUBIC_OUT);

  return wa;
}

static ret_t window_animator_update_percent_default(window_animator_t* wa) {
  if (wa->open) {
    wa->percent = wa->easing(wa->time_percent);
  } else {
    wa->percent = 1.0f - wa->easing(wa->time_percent);
  }

  return RET_OK;
}

static ret_t window_animator_update_percent(window_animator_t* wa) {
  return_value_if_fail(wa != NULL && wa->vt != NULL, RET_BAD_PARAMS);

  if (wa->vt->update_percent != NULL) {
    return wa->vt->update_percent(wa);
  } else {
    return window_animator_update_percent_default(wa);
  }
}

static ret_t window_animator_draw_prev_window(window_animator_t* wa) {
  return_value_if_fail(wa != NULL && wa->vt != NULL && wa->vt->draw_prev_window, RET_BAD_PARAMS);

  if (wa->dialog_highlighter != NULL) {
    return dialog_highlighter_draw(wa->dialog_highlighter, wa->percent);
  } else {
    return wa->vt->draw_prev_window(wa);
  }
}

static ret_t window_animator_draw_curr_window(window_animator_t* wa) {
  return_value_if_fail(wa != NULL && wa->vt != NULL && wa->vt->draw_curr_window, RET_BAD_PARAMS);

  return wa->vt->draw_curr_window(wa);
}
