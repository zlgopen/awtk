﻿/**
 * File:   window_animator_htranslate.c
 * Author: AWTK Develop Team
 * Brief:  horizontal translate window animator
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/window_manager.h"
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
  bool_t start = FALSE;
  canvas_t* c = wa->canvas;
  widget_t* win = wa->prev_win;
  widget_t* wm = window_manager();
  float_t percent = wa->percent;
  float_t x = tk_roundi(win->w * percent);
  float_t w = win->w - x;

#ifndef WITHOUT_WINDOW_ANIMATOR_CACHE
  rectf_t src = rectf_init(x, win->y, w, win->h);
  rectf_t dst = rectf_init(0.0f, win->y, w, win->h);
  ret_t ret = lcd_draw_image(c->lcd, &(wa->prev_img), rectf_scale(&src, wa->ratio), &dst);
  if (ret == RET_OK) {
    /* 非普通窗口应该需要重绘，因为可能会叠在 system_bar 上面，同时非普通窗口可能是半透明的 */
    canvas_translate(c, -x, 0);
    WIDGET_FOR_EACH_CHILD_BEGIN(wm, iter, i)
    if (iter == wa->curr_win) {
      break;
    }
    if (iter == wa->prev_win) {
      start = TRUE;
      continue;
    }
    if (start) {
      widget_paint(iter, c);
    }
    WIDGET_FOR_EACH_CHILD_END()
    canvas_untranslate(c, -x, 0);
  }
  return ret;
#else
  canvas_translate(c, -x, 0);
  WIDGET_FOR_EACH_CHILD_BEGIN(wm, iter, i)
  if (iter == wa->curr_win) {
    break;
  }
  if (iter == wa->prev_win) {
    start = TRUE;
  }
  if (start) {
    widget_paint(iter, c);
  }
  WIDGET_FOR_EACH_CHILD_END()
  canvas_untranslate(c, -x, 0);
  return RET_OK;
#endif /*WITHOUT_WINDOW_ANIMATOR_CACHE*/
}

static ret_t window_animator_htranslate_draw_curr(window_animator_t* wa) {
  canvas_t* c = wa->canvas;
  widget_t* win = wa->curr_win;
  float_t percent = wa->percent;
  float_t x = tk_roundi(win->w * (1 - percent));
  float_t w = win->w - x;

#ifndef WITHOUT_WINDOW_ANIMATOR_CACHE
  rectf_t src = rectf_init(0.0f, win->y, w, win->h);
  rectf_t dst = rectf_init(x, win->y, w, win->h);
  return lcd_draw_image(c->lcd, &(wa->curr_img), rectf_scale(&src, wa->ratio), &dst);
#else
  canvas_translate(c, x, 0);
  widget_paint(win, c);
  canvas_untranslate(c, x, 0);

  return RET_OK;
#endif /*WITHOUT_WINDOW_ANIMATOR_CACHE*/
}

static const window_animator_vtable_t s_window_animator_htranslate_vt = {
    .overlap = FALSE,
    .type = "htranslate",
    .desc = "htranslate",
    .size = sizeof(window_animator_t),
    .update_percent = window_animator_htranslate_update_percent,
    .draw_prev_window = window_animator_htranslate_draw_prev,
    .draw_curr_window = window_animator_htranslate_draw_curr};

window_animator_t* window_animator_htranslate_create(bool_t open, tk_object_t* args) {
  return window_animator_create(open, &s_window_animator_htranslate_vt);
}
