/**
 * File:   window_animator_common.c
 * Author: AWTK Develop Team
 * Brief:  window animator common used functions.
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
 * 2019-03-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "window_animators/window_animator_common.h"

ret_t window_animator_to_bottom_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->curr_win;
  float_t percent = wa->percent;

  int32_t h = tk_roundi(win->h * percent);
  rect_t src = rect_init(win->x, win->y + win->h - h, win->w, h);
  rect_t dst = rect_init(win->x, tk_roundi(win->y * percent), win->w, h);

  return lcd_draw_image(c->lcd, &(wa->curr_img), rect_scale(&src, wa->ratio), &dst);
}

ret_t window_animator_to_top_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->curr_win;
  float_t percent = wa->percent;
  int32_t h = tk_roundi(win->h * percent);
  int32_t y = win->parent->h - tk_roundi((win->parent->h - win->y) * percent);

  rect_t src = rect_init(win->x, win->y, win->w, h);
  rect_t dst = rect_init(win->x, y, win->w, h);

  return lcd_draw_image(c->lcd, &(wa->curr_img), rect_scale(&src, wa->ratio), &dst);
}

ret_t window_animator_to_left_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->curr_win;
  float_t percent = wa->percent;
  int32_t w = tk_roundi(win->w * percent);
  int32_t x = win->parent->w - tk_roundi((win->parent->w - win->x) * percent);

  rect_t src = rect_init(win->x, win->y, w, win->h);
  rect_t dst = rect_init(x, win->y, w, win->h);

  return lcd_draw_image(c->lcd, &(wa->curr_img), rect_scale(&src, wa->ratio), &dst);
}

ret_t window_animator_to_right_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->curr_win;
  float_t percent = wa->percent;

  int32_t w = tk_roundi(win->w * percent);
  rect_t src = rect_init(win->x + win->w - w, win->y, w, win->h);
  rect_t dst = rect_init(tk_roundi(win->x * percent), win->y, w, win->h);

  return lcd_draw_image(c->lcd, &(wa->curr_img), rect_scale(&src, wa->ratio), &dst);
}
