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

#include "fade.inc"
#include "htranslate.inc"
#include "vtranslate.inc"
#include "center_scale.inc"
#include "bottom_to_top.inc"
#include "top_to_bottom.inc"

static window_animator_t* window_animator_create(window_animator_type_t type, bool_t open) {
  window_animator_t* wa = NULL;

  if(type == WINDOW_ANIMATOR_CENTER_SCALE) {
    wa = window_animator_create_scale(open);
  } else if(type == WINDOW_ANIMATOR_HTRANSLATE) {
    wa = window_animator_create_htranslate(open);
  } else if(type == WINDOW_ANIMATOR_VTRANSLATE) {
    wa = window_animator_create_vtranslate(open);
  } else if(type == WINDOW_ANIMATOR_BOTTOM_TO_TOP) {
    wa = window_animator_create_bottom_to_top(open);
  } else if(type == WINDOW_ANIMATOR_TOP_TO_BOTTOM) {
    wa = window_animator_create_top_to_bottom(open);
  } else if(type == WINDOW_ANIMATOR_FADE) {
    wa = window_animator_create_fade(open);
  }

  return wa;
}

window_animator_t* window_animator_create_for_open(window_animator_type_t type, canvas_t* c,
                                                   widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create(type, TRUE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win, TRUE);

  return wa;
}

window_animator_t* window_animator_create_for_close(window_animator_type_t type, canvas_t* c,
                                                    widget_t* prev_win, widget_t* curr_win) {
  window_animator_t* wa = window_animator_create(type, FALSE);
  return_value_if_fail(wa != NULL, NULL);

  window_animator_prepare(wa, c, prev_win, curr_win, FALSE);

  return wa;
}
