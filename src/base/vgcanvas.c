/**
 * File:   vgcanvas.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  vector graphics canvas interface.
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

#include "base/vgcanvas.h"

bool_t vgcanvas_get_antialias(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->get_antialias != NULL, FALSE);

  return vg->vt->get_antialias(vg);
}

ret_t vgcanvas_set_antialias(vgcanvas_t* vg, bool_t value) {
  return_value_if_fail(vg != NULL && vg->vt->set_antialias != NULL, RET_BAD_PARAMS);

  return vg->vt->set_antialias(vg, value);
}

double vgcanvas_get_line_width(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->get_line_width != NULL, 0);

  return vg->vt->get_line_width(vg);
}

ret_t vgcanvas_set_line_width(vgcanvas_t* vg, double value) {
  return_value_if_fail(vg != NULL && vg->vt->set_line_width != NULL, RET_BAD_PARAMS);

  return vg->vt->set_line_width(vg, value);
}

ret_t vgcanvas_move_to(vgcanvas_t* vg, double x, double y) {
  return_value_if_fail(vg != NULL && vg->vt->move_to != NULL, RET_BAD_PARAMS);

  return vg->vt->move_to(vg, x, y);
}

ret_t vgcanvas_line_to(vgcanvas_t* vg, double x, double y) {
  return_value_if_fail(vg != NULL && vg->vt->line_to != NULL, RET_BAD_PARAMS);

  return vg->vt->line_to(vg, x, y);
}

ret_t vgcanvas_reset(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->reset != NULL, RET_BAD_PARAMS);

  return vg->vt->reset(vg);
}

ret_t vgcanvas_clear(vgcanvas_t* vg, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  return_value_if_fail(vg != NULL && vg->vt->clear != NULL, RET_BAD_PARAMS);

  return vg->vt->clear(vg, r, g, b, a);
}

ret_t vgcanvas_rotate(vgcanvas_t* vg, double rad) {
  return_value_if_fail(vg != NULL && vg->vt->rotate != NULL, RET_BAD_PARAMS);

  return vg->vt->rotate(vg, rad);
}

ret_t vgcanvas_scale(vgcanvas_t* vg, double x, double y) {
  return_value_if_fail(vg != NULL && vg->vt->scale != NULL, RET_BAD_PARAMS);

  return vg->vt->scale(vg, x, y);
}

ret_t vgcanvas_translate(vgcanvas_t* vg, double x, double y) {
  return_value_if_fail(vg != NULL && vg->vt->translate != NULL, RET_BAD_PARAMS);

  return vg->vt->translate(vg, x, y);
}

ret_t vgcanvas_set_color(vgcanvas_t* vg, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  return_value_if_fail(vg != NULL && vg->vt->set_color != NULL, RET_BAD_PARAMS);

  return vg->vt->set_color(vg, r, g, b, a);
}

ret_t vgcanvas_fill(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->fill != NULL, RET_BAD_PARAMS);

  return vg->vt->fill(vg);
}

ret_t vgcanvas_stroke(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->stroke != NULL, RET_BAD_PARAMS);

  return vg->vt->stroke(vg);
}

ret_t vgcanvas_destroy(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->destroy != NULL, RET_BAD_PARAMS);

  return vg->vt->destroy(vg);
}
