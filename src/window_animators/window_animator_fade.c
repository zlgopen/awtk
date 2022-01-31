/**
 * File:   window_animator_fade.c
 * Author: AWTK Develop Team
 * Brief:  fade window animator
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "window_animators/window_animator_fade.h"

static ret_t window_animator_fade_draw_curr(window_animator_t* wa) {
  ret_t ret = RET_OK;
  canvas_t* c = wa->canvas;
  widget_t* win = wa->curr_win;

  uint8_t global_alpha = wa->percent * 0xff;
#ifndef WITHOUT_WINDOW_ANIMATOR_CACHE
  rectf_t dst = rectf_init(win->x, win->y, win->w, win->h);
  rectf_t src = rectf_init(win->x, win->y, win->w, win->h);
  lcd_set_global_alpha(c->lcd, global_alpha);

  ret = lcd_draw_image(c->lcd, &(wa->curr_img), rectf_scale(&src, wa->ratio), &dst);
#else
  lcd_set_global_alpha(c->lcd, global_alpha);

  ret = widget_paint(win, c);
#endif /*WITHOUT_WINDOW_ANIMATOR_CACHE*/
  lcd_set_global_alpha(c->lcd, 0xff);
  return ret;
}

static const window_animator_vtable_t s_window_animator_fade_vt = {
    .overlap = TRUE,
    .type = "fade",
    .desc = "fade",
    .size = sizeof(window_animator_t),
    .draw_prev_window = window_animator_overlap_default_draw_prev,
    .draw_curr_window = window_animator_fade_draw_curr};

window_animator_t* window_animator_fade_create(bool_t open, tk_object_t* args) {
  window_animator_t* wa = window_animator_create(open, &s_window_animator_fade_vt);
  return_value_if_fail(wa != NULL, NULL);

  wa->easing = easing_get(EASING_QUADRATIC_IN);

  return wa;
}
