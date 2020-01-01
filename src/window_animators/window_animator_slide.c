/**
 * File:   window_animator_slide_up.h
 * Author: AWTK Develop Team
 * Brief:  slide window animator
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
 * 2019-03-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "window_animators/window_animator_slide.h"

static ret_t window_animator_slide_draw_mask(window_animator_t* wa, uint8_t alpha) {
  if (alpha > 1) {
    canvas_t* c = wa->canvas;
    widget_t* win = wa->prev_win;
    color_t mask = color_init(0, 0, 0, alpha);

    canvas_set_fill_color(c, mask);
    canvas_fill_rect(c, win->x, win->y, win->w, win->h);
  }

  return RET_OK;
}

static ret_t on_prev_window_paint_done(void* ctx, event_t* e) {
  window_animator_t* wa = (window_animator_t*)ctx;
  window_animator_slide_t* was = (window_animator_slide_t*)wa;

  window_animator_slide_draw_mask(wa, was->start_alpha);

  return RET_REMOVE;
}

ret_t window_animator_slide_init(window_animator_t* wa) {
  window_animator_slide_t* was = (window_animator_slide_t*)wa;

  if (was->start_alpha == was->end_alpha) {
    widget_on(wa->prev_win, EVT_PAINT_DONE, on_prev_window_paint_done, wa);
  }

  return RET_OK;
}

ret_t window_animator_slide_draw_prev(window_animator_t* wa) {
  float_t percent = wa->percent;
  window_animator_slide_t* was = (window_animator_slide_t*)wa;

  window_animator_overlap_default_draw_prev(wa);

  /*
   * only if was->start_alpha != was->end_alpha, we draw mask layer very time.
   */
  if (was->start_alpha != was->end_alpha) {
    uint8_t a = ((was->end_alpha - was->start_alpha) * percent) + was->start_alpha;
    window_animator_slide_draw_mask(wa, a);
  }

  return RET_OK;
}

window_animator_t* window_animator_slide_create(bool_t open, object_t* args,
                                                const window_animator_vtable_t* vt) {
  value_t v;
  window_animator_t* wa = window_animator_create(open, vt);
  window_animator_slide_t* was = (window_animator_slide_t*)wa;
  return_value_if_fail(was != NULL, NULL);

  was->end_alpha = 0;
  was->start_alpha = 0;

  if (object_get_prop(args, WINDOW_ANIMATOR_SLIDE_ARG_ALPHA, &v) == RET_OK) {
    was->start_alpha = value_int(&v);
    was->end_alpha = value_int(&v);
  }

  if (object_get_prop(args, WINDOW_ANIMATOR_SLIDE_ARG_START_ALPHA, &v) == RET_OK) {
    was->start_alpha = value_int(&v);
  }

  if (object_get_prop(args, WINDOW_ANIMATOR_SLIDE_ARG_END_ALPHA, &v) == RET_OK) {
    was->end_alpha = value_int(&v);
  }

  return wa;
}
