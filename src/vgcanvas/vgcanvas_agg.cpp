/**
 * File:   vgcanvas.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  vector graphics canvas base on agg
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
 * 2018-03-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/vgcanvas.h"
#include "twombly.hpp"

typedef struct _vgcanvas_agg_t {
  vgcanvas_t base;
  tw::Drawing<tw::rgba32>* canvas;
} vgcanvas_agg_t;

static ret_t vgcanvas_agg_reset(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->reset();

  return RET_OK;
}

static ret_t vgcanvas_agg_flush(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  /*TODO*/

  return RET_OK;
}

static ret_t vgcanvas_agg_clear_rect(vgcanvas_t* vg, double x, double y, double w, double h,
                                     color_t c) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  /*TODO*/

  return RET_OK;
}

static ret_t vgcanvas_agg_begin_path(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  agg->canvas->new_path();

  return RET_OK;
}

static ret_t vgcanvas_agg_move_to(vgcanvas_t* vg, double x, double y) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->move_to(x, y);

  return RET_OK;
}

static ret_t vgcanvas_agg_line_to(vgcanvas_t* vg, double x, double y) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->line_to(x, y);

  return RET_OK;
}

static ret_t vgcanvas_agg_quadratic_curve_to(vgcanvas_t* vg, double cpx, double cpy, double x,
                                             double y) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/

  return RET_OK;
}

static ret_t vgcanvas_agg_bezier_curve_to(vgcanvas_t* vg, double cp1x, double cp1y, double cp2x,
                                          double cp2y, double x, double y) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/

  return RET_OK;
}

static ret_t vgcanvas_agg_arc_to(vgcanvas_t* vg, double x1, double y1, double x2, double y2,
                                 double r) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_arc(vgcanvas_t* vg, double x, double y, double r, double start,
                              double end, bool_t ccw) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/

  return RET_OK;
}

static bool_t vgcanvas_agg_is_point_in_path(vgcanvas_t* vg, double x, double y) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/

  return FALSE;
}

static ret_t vgcanvas_agg_rotate(vgcanvas_t* vg, double rad) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->rotate(rad);

  return RET_OK;
}

static ret_t vgcanvas_agg_scale(vgcanvas_t* vg, double x, double y) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->scale(x, y);

  return RET_OK;
}

static ret_t vgcanvas_agg_translate(vgcanvas_t* vg, double x, double y) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->translate(x, y);

  return RET_OK;
}

static ret_t vgcanvas_agg_transform(vgcanvas_t* vg, double a, double b, double c, double d,
                                    double e, double f) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/

  return RET_OK;
}

static ret_t vgcanvas_agg_set_transform(vgcanvas_t* vg, double a, double b, double c, double d,
                                        double e, double f) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/

  return RET_OK;
}

static ret_t vgcanvas_agg_round_rect(vgcanvas_t* vg, double x, double y, double w, double h,
                                     double r) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  agg->canvas->rect(x, y, x + w, y + h, r);

  return RET_OK;
}

static ret_t vgcanvas_agg_ellipse(vgcanvas_t* vg, double x, double y, double rx, double ry) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  agg->canvas->ellipse(x, y, rx, ry);

  return RET_OK;
}

static ret_t vgcanvas_agg_close_path(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  agg->canvas->end_polygon();

  return RET_OK;
}

static ret_t vgcanvas_agg_fill(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->fill();

  return RET_OK;
}

static ret_t vgcanvas_agg_clip(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_stroke(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  agg->canvas->stroke();

  return RET_OK;
}

static ret_t vgcanvas_agg_set_font(vgcanvas_t* vg, const char* font) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_set_text_align(vgcanvas_t* vg, const char* text_align) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_set_text_baseline(vgcanvas_t* vg, const char* text_baseline) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_fill_text(vgcanvas_t* vg, const char* text, double x, double y,
                                    double max_width) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_stroke_text(vgcanvas_t* vg, const char* text, double x, double y,
                                      double max_width) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_draw_image(vgcanvas_t* vg, bitmap_t* img, double sx, double sy, double sw,
                                     double sh, double dx, double dy, double dw, double dh) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_set_antialias(vgcanvas_t* vg, bool_t value) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->antialias(value);

  return RET_OK;
}

static ret_t vgcanvas_agg_set_line_width(vgcanvas_t* vg, double value) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->line_width(value);

  return RET_OK;
}

static ret_t vgcanvas_agg_set_global_alpha(vgcanvas_t* vg, double value) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_set_fill_color(vgcanvas_t* vg, color_t c) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->set_fill_color(c.rgba.r, c.rgba.g, c.rgba.b, c.rgba.a);

  return RET_OK;
}

static ret_t vgcanvas_agg_set_stroke_color(vgcanvas_t* vg, color_t c) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->set_stroke_color(c.rgba.r, c.rgba.g, c.rgba.b, c.rgba.a);

  return RET_OK;
}

static ret_t vgcanvas_agg_set_line_cap(vgcanvas_t* vg, const char* value) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_set_line_join(vgcanvas_t* vg, const char* value) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_set_miter_limit(vgcanvas_t* vg, double value) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_save(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_restore(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_agg_destroy(vgcanvas_t* vg) { return RET_OK; }

static const vgcanvas_vtable_t vt = {vgcanvas_agg_reset,
                                     vgcanvas_agg_flush,
                                     vgcanvas_agg_clear_rect,
                                     vgcanvas_agg_begin_path,
                                     vgcanvas_agg_move_to,
                                     vgcanvas_agg_line_to,
                                     vgcanvas_agg_arc,
                                     vgcanvas_agg_arc_to,
                                     vgcanvas_agg_bezier_curve_to,
                                     vgcanvas_agg_quadratic_curve_to,
                                     vgcanvas_agg_is_point_in_path,
                                     vgcanvas_agg_ellipse,
                                     vgcanvas_agg_round_rect,
                                     vgcanvas_agg_close_path,

                                     vgcanvas_agg_scale,
                                     vgcanvas_agg_rotate,
                                     vgcanvas_agg_translate,
                                     vgcanvas_agg_transform,
                                     vgcanvas_agg_set_transform,

                                     vgcanvas_agg_fill,
                                     vgcanvas_agg_clip,
                                     vgcanvas_agg_stroke,
                                     vgcanvas_agg_set_font,
                                     vgcanvas_agg_set_text_align,
                                     vgcanvas_agg_set_text_baseline,
                                     vgcanvas_agg_fill_text,
                                     vgcanvas_agg_stroke_text,
                                     vgcanvas_agg_draw_image,

                                     vgcanvas_agg_set_antialias,
                                     vgcanvas_agg_set_global_alpha,
                                     vgcanvas_agg_set_line_width,
                                     vgcanvas_agg_set_fill_color,
                                     vgcanvas_agg_set_stroke_color,
                                     vgcanvas_agg_set_line_join,
                                     vgcanvas_agg_set_line_cap,
                                     vgcanvas_agg_set_miter_limit,

                                     vgcanvas_agg_save,
                                     vgcanvas_agg_restore,

                                     vgcanvas_agg_destroy};

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t* buff) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)MEM_ZALLOC(vgcanvas_agg_t);
  return_value_if_fail(agg != NULL, NULL);

  agg->canvas = new tw::Drawing<tw::rgba32>(w, h, 4, (uint8_t*)buff);
  agg->base.vt = &vt;
  agg->base.w = w;
  agg->base.h = h;
  agg->base.buff = buff;

  return &(agg->base);
}
