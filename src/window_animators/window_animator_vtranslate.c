/**
 * File:   window_animator_vtranslate.c
 * Author: AWTK Develop Team
 * Brief:  vertical translate window animator
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

#include "window_animators/window_animator_vtranslate.h"

typedef struct _window_animator_vtranslate_t {
  window_animator_t window_animator;
  uint32_t prev_win_y_range;
} window_animator_vtranslate_t;

static ret_t window_animator_vtranslate_update_percent(window_animator_t* wa) {
  float_t percent = 0.2f + 0.8f * wa->easing(wa->time_percent);
  if (wa->open) {
    wa->percent = percent;
  } else {
    wa->percent = 1.0f - percent;
  }

  return RET_OK;
}

static ret_t window_animator_vtranslate_draw_prev(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->prev_win;
  widget_t* curr_win = wa->curr_win;
  float_t percent = wa->percent;
  window_animator_vtranslate_t* wav = (window_animator_vtranslate_t*)wa;
  uint32_t range = wav->prev_win_y_range ? wav->prev_win_y_range : curr_win->h;
  float_t y = tk_roundi(range * percent);
  float_t h = win->h - y;

#ifndef WITHOUT_WINDOW_ANIMATOR_CACHE
  rectf_t src = rectf_init(win->x, y + win->y, win->w, h);
  rectf_t dst = rectf_init(win->x, win->y, win->w, h);
  return lcd_draw_image(c->lcd, &(wa->prev_img), rectf_scale(&src, wa->ratio), &dst);
#else
  canvas_translate(c, 0, -y);
  widget_paint(win, c);
  canvas_untranslate(c, 0, -y);
  return RET_OK;
#endif /*WITHOUT_WINDOW_ANIMATOR_CACHE*/
}

static ret_t window_animator_vtranslate_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->curr_win;
  float_t percent = wa->percent;
  float_t h = tk_roundi(win->h * percent);
  float_t y = win->parent->h - h;

#ifndef WITHOUT_WINDOW_ANIMATOR_CACHE
  rectf_t src = rectf_init(win->x, win->y, win->w, h);
  rectf_t dst = rectf_init(win->x, y, win->w, h);
  return lcd_draw_image(c->lcd, &(wa->curr_img), rectf_scale(&src, wa->ratio), &dst);
#else
  y = win->h * (1 - percent);
  canvas_translate(c, 0, y);
  widget_paint(win, c);
  canvas_untranslate(c, 0, y);

  return RET_OK;
#endif /*WITHOUT_WINDOW_ANIMATOR_CACHE*/
}

static const window_animator_vtable_t s_window_animator_vtranslate_vt = {
    .overlap = FALSE,
    .type = "vtranslate",
    .desc = "vtranslate",
    .size = sizeof(window_animator_vtranslate_t),
    .update_percent = window_animator_vtranslate_update_percent,
    .draw_prev_window = window_animator_vtranslate_draw_prev,
    .draw_curr_window = window_animator_vtranslate_draw_curr};

window_animator_t* window_animator_vtranslate_create(bool_t open, tk_object_t* args) {
  window_animator_t* wa = window_animator_create(open, &s_window_animator_vtranslate_vt);
  window_animator_vtranslate_t* wav = (window_animator_vtranslate_t*)wa;
  return_value_if_fail(wa != NULL, NULL);

  /*主要用于弹出软键盘，此时前面窗口移动的位置由编辑器的位置决定，而不是软键盘的高度*/
  wav->prev_win_y_range = tk_object_get_prop_uint32(args, "prev_win_y_range", 0);

  return wa;
}
