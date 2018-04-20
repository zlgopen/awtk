/**
 * File:   window_animator_direct
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  window_animator_direct
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

#include "base/mem.h"
#include "base/window_animator.h"

static ret_t window_animator_open_destroy(window_animator_t* wa) {
  vgcanvas_t* vg = lcd_get_vgcanvas(wa->canvas->lcd);
  vgcanvas_destroy_fbo(vg, &(wa->prev_fbo));
  vgcanvas_destroy_fbo(vg, &(wa->curr_fbo));

  memset(wa, 0x00, sizeof(window_animator_t));
  MEM_FREE(wa);

  return RET_OK;
}

static ret_t window_animator_close_destroy(window_animator_t* wa) {
  vgcanvas_t* vg = lcd_get_vgcanvas(wa->canvas->lcd);
  widget_destroy(wa->curr_win);
  vgcanvas_destroy_fbo(vg, &(wa->prev_fbo));
  vgcanvas_destroy_fbo(vg, &(wa->curr_fbo));

  memset(wa, 0x00, sizeof(window_animator_t));
  MEM_FREE(wa);

  return RET_OK;
}

static ret_t fbo_to_img(framebuffer_object_t* fbo, bitmap_t* img) {
  return_value_if_fail(fbo != NULL && img != NULL, RET_BAD_PARAMS);

  img->id = fbo->id;
  img->w = fbo->w * fbo->ratio;
  img->h = fbo->h * fbo->ratio;

  img->flags = BITMAP_FLAG_TEXTURE;

  return RET_OK;
}

static ret_t window_animator_prepare(window_animator_t* wa, canvas_t* c, widget_t* prev_win,
                                     widget_t* curr_win, bool_t open) {
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);

  wa->canvas = c;
  wa->open = open;
  wa->duration = 400;
  wa->prev_win = prev_win;
  wa->curr_win = curr_win;

  ENSURE(vgcanvas_create_fbo(vg, &(wa->prev_fbo)) == RET_OK);
  ENSURE(vgcanvas_bind_fbo(vg, &(wa->prev_fbo)) == RET_OK);
  vgcanvas_scale(vg, 1, 1);
  ENSURE(widget_paint(prev_win, c) == RET_OK);
  ENSURE(vgcanvas_unbind_fbo(vg, &(wa->prev_fbo)) == RET_OK);

  ENSURE(vgcanvas_create_fbo(vg, &(wa->curr_fbo)) == RET_OK);
  ENSURE(vgcanvas_bind_fbo(vg, &(wa->curr_fbo)) == RET_OK);
  vgcanvas_scale(vg, 1, 1);
  ENSURE(widget_paint(curr_win, c) == RET_OK);
  ENSURE(vgcanvas_unbind_fbo(vg, &(wa->curr_fbo)) == RET_OK);

  fbo_to_img(&(wa->prev_fbo), &(wa->prev_img));
  fbo_to_img(&(wa->curr_fbo), &(wa->curr_img));
  wa->ratio = wa->curr_fbo.ratio;

  return RET_OK;
}

static ret_t window_animator_open_scale_update_percent(window_animator_t* wa) {
  if (wa->open) {
    wa->percent = 0.9 + 0.1 * wa->easing(wa->time_percent);
  } else {
    wa->percent = 1.0 - 0.1 * wa->easing(wa->time_percent);
  }

  return RET_OK;
}

static ret_t window_animator_open_scale_draw_prev(window_animator_t* wa) {
  widget_t* prev_win = wa->prev_win;
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  xy_t x = prev_win->x * ratio;
  xy_t y = prev_win->y * ratio;
  wh_t w = prev_win->w * ratio;
  wh_t h = prev_win->h * ratio;
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);

  vgcanvas_draw_image(vg, &(wa->prev_img), x, y, w, h, prev_win->x, prev_win->y, prev_win->w,
                      prev_win->h);

  return RET_OK;
}

static ret_t window_animator_open_scale_draw_curr(window_animator_t* wa) {
  widget_t* curr_win = wa->curr_win;
  canvas_t* c = wa->canvas;
  float_t ratio = wa->ratio;
  xy_t x = curr_win->x;
  xy_t y = curr_win->y;
  wh_t w = curr_win->w;
  wh_t h = curr_win->h;
  float_t scale = wa->percent;
  float_t alpha = wa->open ? wa->time_percent : 1 - wa->time_percent;
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);

  vgcanvas_save(vg);

  vgcanvas_translate(vg, x + (w >> 1), y + (h >> 1));

  vgcanvas_scale(vg, scale, scale);
  vgcanvas_translate(vg, -(w >> 1), -(h >> 1));

  vgcanvas_set_global_alpha(vg, alpha);
  vgcanvas_draw_image(vg, &(wa->curr_img), x * ratio, y * ratio, w * ratio, h * ratio, 0, 0,
                      curr_win->w, curr_win->h);

  vgcanvas_restore(vg);

  return RET_OK;
}

static window_animator_t* window_animator_create_scale(bool_t open) {
  window_animator_t* wa = MEM_ZALLOC(window_animator_t);
  return_value_if_fail(wa != NULL, NULL);

  if (open) {
    wa->easing = easing_get(EASING_QUADRATIC_IN);
    wa->destroy = window_animator_open_destroy;
  } else {
    wa->easing = easing_get(EASING_QUADRATIC_IN);
    wa->destroy = window_animator_close_destroy;
  }

  wa->update_percent = window_animator_open_scale_update_percent;
  wa->draw_prev_window = window_animator_open_scale_draw_prev;
  wa->draw_curr_window = window_animator_open_scale_draw_curr;

  return wa;
}

window_animator_t* window_animator_create_for_open(window_animator_type_t type, canvas_t* c,
                                                   widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create_scale(TRUE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win, TRUE);

  return wa;
}

window_animator_t* window_animator_create_for_close(window_animator_type_t type, canvas_t* c,
                                                    widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create_scale(FALSE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win, FALSE);

  return wa;
}
