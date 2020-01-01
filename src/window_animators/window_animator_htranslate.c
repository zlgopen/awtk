/**
 * File:   window_animator_htranslate.c
 * Author: AWTK Develop Team
 * Brief:  horizontal translate window animator
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
 * 2018-04-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "window_animators/window_animator_htranslate.h"

static ret_t window_animator_htranslate_update_percent(window_animator_t* wa) {
  float_t percent = 0.2f + 0.8f * wa->easing(wa->time_percent);
  if (wa->open) {
    wa->percent = percent;
  } else {
    wa->percent = 1.0f - percent;
  }

  return RET_OK;
}

static ret_t window_animator_htranslate_draw_prev(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->prev_win;
  float_t percent = wa->percent;
  float_t x = tk_roundi(win->w * percent);
  float_t w = win->w - x;

  rect_t src = rect_init(x, win->y, w, win->h);
  rect_t dst = rect_init(0, win->y, w, win->h);

  return lcd_draw_image(c->lcd, &(wa->prev_img), rect_scale(&src, wa->ratio), &dst);
}

static ret_t window_animator_htranslate_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->curr_win;
  float_t percent = wa->percent;
  float_t x = tk_roundi(win->w * (1 - percent));
  float_t w = win->w - x;

  rect_t src = rect_init(0, win->y, w, win->h);
  rect_t dst = rect_init(x, win->y, w, win->h);

  return lcd_draw_image(c->lcd, &(wa->curr_img), rect_scale(&src, wa->ratio), &dst);
}

static const window_animator_vtable_t s_window_animator_htranslate_vt = {
    .overlap = FALSE,
    .type = "htranslate",
    .desc = "htranslate",
    .size = sizeof(window_animator_t),
    .update_percent = window_animator_htranslate_update_percent,
    .draw_prev_window = window_animator_htranslate_draw_prev,
    .draw_curr_window = window_animator_htranslate_draw_curr};

window_animator_t* window_animator_htranslate_create(bool_t open, object_t* args) {
  return window_animator_create(open, &s_window_animator_htranslate_vt);
}
