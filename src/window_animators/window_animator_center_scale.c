/**
 * File:   window_animator_center_scale.c
 * Author: AWTK Develop Team
 * Brief:  center_scale window animator
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

#include "window_animators/window_animator_center_scale.h"

static ret_t window_animator_center_scale_update_percent(window_animator_t* wa) {
  if (wa->open) {
    wa->percent = 0.9f + 0.1f * wa->easing(wa->time_percent);
    if (wa->easing(wa->time_percent) == 0) {
      wa->percent = 0;
    }
  } else {
    wa->percent = 1.0f - 0.1f * wa->easing(wa->time_percent);
  }

  return RET_OK;
}

static ret_t window_animator_center_scale_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->curr_win;
  float_t scale = wa->percent;

  rect_t src = rect_init(win->x, win->y, win->w, win->h);
  rect_t dst = rect_init(0, 0, win->w * scale, win->h * scale);

  if (wa->time_percent < 5) {
    float_t alpha = wa->open ? wa->time_percent : 1 - wa->time_percent;
    lcd_set_global_alpha(c->lcd, alpha * 0xff);
  } else {
    lcd_set_global_alpha(c->lcd, 0xff);
  }

  dst.x = win->x + ((win->w - dst.w) >> 1);
  dst.y = win->y + ((win->h - dst.h) >> 1);

  return lcd_draw_image(c->lcd, &(wa->curr_img), rect_scale(&src, wa->ratio), &dst);
}

static const window_animator_vtable_t s_window_animator_center_scale_vt = {
    .overlap = TRUE,
    .type = "center_scale",
    .desc = "center_scale",
    .size = sizeof(window_animator_t),
    .update_percent = window_animator_center_scale_update_percent,
    .draw_prev_window = window_animator_overlap_default_draw_prev,
    .draw_curr_window = window_animator_center_scale_draw_curr};

window_animator_t* window_animator_center_scale_create(bool_t open, object_t* args) {
  return window_animator_create(open, &s_window_animator_center_scale_vt);
}
