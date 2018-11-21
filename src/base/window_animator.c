/**
 * File:   window_animator.c
 * Author: AWTK Develop Team
 * Brief:  window_animator
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

ret_t window_animator_update(window_animator_t* wa, uint32_t time_ms) {
  canvas_t* c = NULL;
  return_value_if_fail(wa != NULL, RET_FAIL);

  c = wa->canvas;
  if (wa->start_time == 0) {
    wa->start_time = time_ms;
  }

  wa->time_percent = (time_ms - wa->start_time) / (float_t)(wa->duration);
  if (wa->time_percent >= 1) {
    wa->time_percent = 1;
  }

  if (wa->update_percent != NULL) {
    wa->update_percent(wa);
  } else {
    wa->percent = wa->time_percent;
  }

  wa->begin_frame(wa);

  if (wa->draw_prev_window != NULL) {
    wa->draw_prev_window(wa);
  }

  if (wa->draw_curr_window != NULL) {
    wa->draw_curr_window(wa);
  }

  if (wa->end_frame) {
    wa->end_frame(wa);
  } else {
    ENSURE(canvas_end_frame(c) == RET_OK);
  }

  return wa->time_percent >= 1 ? RET_DONE : RET_OK;
}

ret_t window_animator_destroy(window_animator_t* wa) {
  return_value_if_fail(wa != NULL && wa->destroy != NULL, RET_FAIL);

  return wa->destroy(wa);
}

ret_t window_animator_begin_frame(window_animator_t* wa) {
#ifdef WITH_NANOVG_GPU
  (void)wa;
#else
  rect_t r;
  widget_t* wm = wa->curr_win->parent;
  r = rect_init(wm->x, wm->y, wm->w, wm->h);
  ENSURE(canvas_begin_frame(wa->canvas, &r, LCD_DRAW_ANIMATION) == RET_OK);
#endif

  return RET_OK;
}

ret_t window_animator_begin_frame_overlap(window_animator_t* wa) {
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

  return RET_OK;
}
