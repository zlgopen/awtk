/**
 * File:   window_animator_nanovg
 * Author: AWTK Develop Team
 * Brief:  nanovg implemented window animator
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
 * 2018-04-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/window_animator.h"

static ret_t window_animator_open_destroy(window_animator_t* wa) {
  vgcanvas_t* vg = lcd_get_vgcanvas(wa->canvas->lcd);
  vgcanvas_destroy_fbo(vg, &(wa->prev_fbo));
  vgcanvas_destroy_fbo(vg, &(wa->curr_fbo));

  memset(wa, 0x00, sizeof(window_animator_t));
  TKMEM_FREE(wa);

  return RET_OK;
}

static ret_t window_animator_close_destroy(window_animator_t* wa) {
  widget_destroy(wa->curr_win);

  return window_animator_open_destroy(wa);
}

static ret_t fbo_to_img(framebuffer_object_t* fbo, bitmap_t* img) {
  return_value_if_fail(fbo != NULL && img != NULL, RET_BAD_PARAMS);

  memset(img, 0x00, sizeof(bitmap_t));
  img->specific = (char*)NULL + fbo->id;
  img->specific_ctx = NULL;
  img->specific_destroy = NULL;
  img->w = fbo->w * fbo->ratio;
  img->h = fbo->h * fbo->ratio;

  img->flags = BITMAP_FLAG_TEXTURE;

  return RET_OK;
}

static ret_t window_animator_prepare(window_animator_t* wa, canvas_t* c, widget_t* prev_win,
                                     widget_t* curr_win, bool_t open) {
  vgcanvas_t* vg = lcd_get_vgcanvas(c->lcd);
  widget_t* wm = prev_win->parent;

  wa->canvas = c;
  wa->open = open;
  wa->prev_win = prev_win;
  wa->curr_win = curr_win;
  wa->duration = wa->duration ? wa->duration : 500;

  ENSURE(vgcanvas_create_fbo(vg, &(wa->prev_fbo)) == RET_OK);
  ENSURE(vgcanvas_bind_fbo(vg, &(wa->prev_fbo)) == RET_OK);
  vgcanvas_scale(vg, 1, 1);
  ENSURE(widget_on_paint_background(wm, c) == RET_OK);
  ENSURE(widget_paint(prev_win, c) == RET_OK);
  ENSURE(vgcanvas_unbind_fbo(vg, &(wa->prev_fbo)) == RET_OK);

  ENSURE(vgcanvas_create_fbo(vg, &(wa->curr_fbo)) == RET_OK);
  ENSURE(vgcanvas_bind_fbo(vg, &(wa->curr_fbo)) == RET_OK);
  vgcanvas_scale(vg, 1, 1);
  ENSURE(widget_on_paint_background(wm, c) == RET_OK);
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

#include "common.inc"
