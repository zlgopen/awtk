/**
 * File:   window_animator_center_scale.c
 * Author: AWTK Develop Team
 * Brief:  center_scale window animator
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifdef WITH_GPU
#define START_PERCENT 0.5f
#else
#define START_PERCENT 0.9f
#endif

static ret_t window_animator_center_scale_update_percent(window_animator_t* wa) {
  if (wa->open) {
    wa->percent = START_PERCENT + (1 - START_PERCENT) * wa->easing(wa->time_percent);
    if (wa->easing(wa->time_percent) == 0) {
      wa->percent = 0;
    }
  } else {
    wa->percent = 1.0f - (1 - START_PERCENT) * wa->easing(wa->time_percent);
  }

  return RET_OK;
}

static ret_t window_animator_center_scale_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->curr_win;
  float_t scale = wa->percent;

#ifndef WITHOUT_WINDOW_ANIMATOR_CACHE
  ret_t ret = RET_OK;
  rectf_t src = rectf_init(win->x, win->y, win->w, win->h);
  rectf_t dst = rectf_init(0.0f, 0.0f, win->w * scale, win->h * scale);
  if (wa->time_percent < 5) {
    float_t alpha = wa->open ? wa->time_percent : 1 - wa->time_percent;
    lcd_set_global_alpha(c->lcd, alpha * 0xff);
  } else {
    lcd_set_global_alpha(c->lcd, 0xff);
  }

  dst.x = win->x + ((win->w - dst.w) / 2.0f);
  dst.y = win->y + ((win->h - dst.h) / 2.0f);
  ret = lcd_draw_image(c->lcd, &(wa->curr_img), rectf_scale(&src, wa->ratio), &dst);

  lcd_set_global_alpha(c->lcd, 0xff);
  return ret;
#else
#ifdef WITH_GPU
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  float x = win->x + (win->w / 2);
  float y = win->y + (win->h / 2);
  float alpha = 1 - (1 - scale) / (1 - START_PERCENT);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, x, y);
  vgcanvas_scale(vg, scale, scale);
  vgcanvas_translate(vg, -x, -y);
  vgcanvas_set_global_alpha(vg, alpha);
  widget_paint(win, c);
  vgcanvas_restore(vg);
  vgcanvas_set_global_alpha(vg, 0xff);
  return RET_OK;
#else
  assert(!"not supported");
  return RET_FAIL;
#endif /*WITH_GPU*/

#endif /*WITHOUT_WINDOW_ANIMATOR_CACHE*/
}

static const window_animator_vtable_t s_window_animator_center_scale_vt = {
    .overlap = TRUE,
    .type = "center_scale",
    .desc = "center_scale",
    .size = sizeof(window_animator_t),
    .update_percent = window_animator_center_scale_update_percent,
    .draw_prev_window = window_animator_overlap_default_draw_prev,
    .draw_curr_window = window_animator_center_scale_draw_curr};

window_animator_t* window_animator_center_scale_create(bool_t open, tk_object_t* args) {
  return window_animator_create(open, &s_window_animator_center_scale_vt);
}
