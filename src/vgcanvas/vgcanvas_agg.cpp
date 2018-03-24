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

#include "twombly.hpp"
#include "base/mem.h"
#include "base/vgcanvas.h"

typedef struct _vgcanvas_agg_t {
  vgcanvas_t base;
  tw::Drawing<tw::rgba32>* canvas;
} vgcanvas_agg_t;

static bool_t vgcanvas_agg_get_antialias(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;

  return agg->canvas->antialias();
}

static ret_t vgcanvas_agg_set_antialias(vgcanvas_t* vg, bool_t value) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;

  agg->canvas->antialias(value);

  return RET_OK;
}

static double vgcanvas_agg_get_line_width(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;

  return agg->canvas->line_width();
}

ret_t vgcanvas_agg_set_line_width(vgcanvas_t* vg, double value) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;

  agg->canvas->line_width(value);

  return RET_OK;
}

ret_t vgcanvas_agg_move_to(vgcanvas_t* vg, double x, double y) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->move_to(x, y);

  return RET_OK;
}

ret_t vgcanvas_agg_line_to(vgcanvas_t* vg, double x, double y) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->line_to(x, y);

  return RET_OK;
}

ret_t vgcanvas_agg_reset(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->reset();

  return RET_OK;
}

ret_t vgcanvas_agg_clear(vgcanvas_t* vg, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->clear(r, g, b, a);

  return RET_OK;
}

ret_t vgcanvas_agg_rotate(vgcanvas_t* vg, double rad) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->rotate(rad);

  return RET_OK;
}

ret_t vgcanvas_agg_scale(vgcanvas_t* vg, double x, double y) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->scale(x, y);

  return RET_OK;
}

ret_t vgcanvas_agg_translate(vgcanvas_t* vg, double x, double y) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->translate(x, y);

  return RET_OK;
}

ret_t vgcanvas_agg_set_color(vgcanvas_t* vg, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->set_color(r, g, b, a);

  return RET_OK;
}

ret_t vgcanvas_agg_fill(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  return_value_if_fail(agg->canvas != NULL, RET_BAD_PARAMS);

  agg->canvas->fill();

  return RET_OK;
}

ret_t vgcanvas_agg_stroke(vgcanvas_t* vg) {
  vgcanvas_agg_t* agg = (vgcanvas_agg_t*)vg;
  agg->canvas->stroke();

  return RET_OK;
}

ret_t vgcanvas_agg_destroy(vgcanvas_t* vg) { return RET_OK; }

static const vgcanvas_vtable_t vt = {.get_antialias = vgcanvas_agg_get_antialias,
                                     .set_antialias = vgcanvas_agg_set_antialias,
                                     .get_line_width = vgcanvas_agg_get_line_width,
                                     .set_line_width = vgcanvas_agg_set_line_width,
                                     .move_to = vgcanvas_agg_move_to,
                                     .line_to = vgcanvas_agg_line_to,
                                     .reset = vgcanvas_agg_reset,
                                     .clear = vgcanvas_agg_clear,
                                     .rotate = vgcanvas_agg_rotate,
                                     .scale = vgcanvas_agg_scale,
                                     .translate = vgcanvas_agg_translate,
                                     .set_color = vgcanvas_agg_set_color,
                                     .fill = vgcanvas_agg_fill,
                                     .stroke = vgcanvas_agg_stroke,
                                     .destroy = vgcanvas_agg_destroy};

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
