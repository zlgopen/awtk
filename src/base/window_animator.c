/**
 * File:   window_animator.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  window_animator
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
  rect_t r;
  widget_t* wm = NULL;
  canvas_t* c = NULL;
  return_value_if_fail(wa != NULL, RET_FAIL);

  c = wa->canvas;
  wm = wa->curr_win->parent;
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

  rect_init(r, wm->x, wm->y, wm->w, wm->h);
  ENSURE(canvas_begin_frame(c, &r, LCD_DRAW_ANIMATION) == RET_OK);

  if (wa->draw_prev_window != NULL) {
    wa->draw_prev_window(wa);
  }

  if (wa->draw_curr_window != NULL) {
    wa->draw_curr_window(wa);
  }

  // log_debug("percent=%f time_percent=%f time=%u\n", wa->percent, wa->time_percent, time_ms);
  ENSURE(canvas_end_frame(c) == RET_OK);

  return wa->time_percent >= 1 ? RET_DONE : RET_OK;
}

ret_t window_animator_destroy(window_animator_t* wa) {
  return_value_if_fail(wa != NULL && wa->destroy != NULL, RET_FAIL);

  return wa->destroy(wa);
}
