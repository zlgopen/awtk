/**
 * File:   window_animator_bottom_to_top.c
 * Author: AWTK Develop Team
 * Brief:  bottom_to_top window animator
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
 * 2018-04-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "window_animators/window_animator_bottom_to_top.h"

static ret_t window_animator_bottom_to_top_draw_prev(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->prev_win;

  rect_t src = rect_init(win->x, win->y, win->w, win->h);
  rect_t dst = rect_init(win->x, win->y, win->w, win->h);

  return lcd_draw_image(c->lcd, &(wa->prev_img), rect_scale(&src, wa->ratio), &dst);
}

static ret_t window_animator_bottom_to_top_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->curr_win;
  float_t percent = wa->percent;
  int32_t h = win->h * percent;
  int32_t y = win->parent->h - h;

  rect_t src = rect_init(win->x, win->y, win->w, h);
  rect_t dst = rect_init(win->x, y, win->w, h);

  return lcd_draw_image(c->lcd, &(wa->curr_img), rect_scale(&src, wa->ratio), &dst);
}

static const window_animator_vtable_t s_window_animator_bottom_to_top_vt = {
    .overlap = TRUE,
    .type = "bottom_to_top",
    .desc = "bottom_to_top",
    .size = sizeof(window_animator_t),
    .draw_prev_window = window_animator_bottom_to_top_draw_prev,
    .draw_curr_window = window_animator_bottom_to_top_draw_curr};

window_animator_t* window_animator_bottom_to_top_create(bool_t open, object_t* args) {
  return window_animator_create(open, &s_window_animator_bottom_to_top_vt);
}
