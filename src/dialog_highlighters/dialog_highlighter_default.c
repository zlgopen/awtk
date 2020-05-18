/**
 * File:   dialog_highlighter.c
 * Author: AWTK Develop Team
 * Brief:  dialog_highlighter
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied highlighterrranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/dialog_highlighter.h"
#include "dialog_highlighters/dialog_highlighter_default.h"
#include "window_manager/window_manager_default.h"

static ret_t dialog_highlighter_default_draw_mask(canvas_t* c, uint8_t alpha) {
  if (alpha > 1) {
    wh_t w = canvas_get_width(c);
    wh_t h = canvas_get_height(c);
    color_t mask = color_init(0, 0, 0, alpha);

    canvas_set_fill_color(c, mask);
    canvas_fill_rect(c, 0, 0, w, h);
  }

  return RET_OK;
}

static ret_t dialog_highlighter_default_prepare(dialog_highlighter_t* h, canvas_t* c) {
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  return_value_if_fail(h != NULL && h->vt != NULL && c != NULL, RET_BAD_PARAMS);

  /*
   * optimization:
   * if dh->start_alpha == dh->end_alpha, we draw mask layer only at the first time.
   */
  if (dh->start_alpha == dh->end_alpha) {
    window_manager_paint_system_bar(window_manager(), c);
    dialog_highlighter_default_draw_mask(c, dh->start_alpha);
  }

  return RET_OK;
}

static ret_t dialog_highlighter_default_draw(dialog_highlighter_t* h, float_t percent) {
  rect_t r;
  rect_t save_r;
  canvas_t* c = h->canvas;
  bitmap_t* img = &(h->img);
  rect_t src = rect_init(0, 0, img->w, img->h);
  rect_t dst = rect_init(0, 0, canvas_get_width(c), canvas_get_height(c));
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;

  if (percent == 1) {
    /*if window is open, enable clip and draw system bar*/
    canvas_draw_image(c, img, &src, &dst);
    window_manager_paint_system_bar(window_manager(), c);
  } else {
    canvas_get_clip_rect(c, &save_r);
    r = rect_intersect(&save_r, &h->clip_rect);
    canvas_set_clip_rect(c, &r);
    canvas_draw_image(c, img, &src, &dst);
    canvas_set_clip_rect(c, &save_r);
  }

  /*
   * only if dh->start_alpha != dh->end_alpha, we draw mask layer very time.
   */
  if (dh->start_alpha != dh->end_alpha) {
    uint8_t a = ((dh->end_alpha - dh->start_alpha) * percent) + dh->start_alpha;
    dialog_highlighter_default_draw_mask(c, a);
  }

  return RET_OK;
}

static bool_t dialog_highlighter_default_is_dynamic(dialog_highlighter_t* h) {
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;

  return (dh->start_alpha != dh->end_alpha);
}

static const dialog_highlighter_vtable_t s_dialog_highlighter_default_vt = {
    .type = "dialog_highlighter_default_t",
    .desc = "dialog_highlighter_default_t",
    .size = sizeof(dialog_highlighter_default_t),
    .prepare = dialog_highlighter_default_prepare,
    .is_dynamic = dialog_highlighter_default_is_dynamic,
    .draw = dialog_highlighter_default_draw};

dialog_highlighter_t* dialog_highlighter_default_create(object_t* args) {
  value_t v;
  dialog_highlighter_t* h = dialog_highlighter_create(&s_dialog_highlighter_default_vt);
  dialog_highlighter_default_t* dh = (dialog_highlighter_default_t*)h;
  return_value_if_fail(h != NULL, NULL);

  dh->end_alpha = 0;
  dh->start_alpha = 0;

  if (object_get_prop(args, DIALOG_HIGHLIGHTER_DEFAULT_ARG_ALPHA, &v) == RET_OK) {
    dh->start_alpha = value_int(&v);
    dh->end_alpha = value_int(&v);
  }

  if (object_get_prop(args, DIALOG_HIGHLIGHTER_DEFAULT_ARG_START_ALPHA, &v) == RET_OK) {
    dh->start_alpha = value_int(&v);
  }

  if (object_get_prop(args, DIALOG_HIGHLIGHTER_DEFAULT_ARG_END_ALPHA, &v) == RET_OK) {
    dh->end_alpha = value_int(&v);
  }

  return h;
}
