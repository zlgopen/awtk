/**
 * File:   vgcanvas.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas interface.
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/system_info.h"
#include "tkc/color_parser.h"
#include "tkc/mem.h"
#include "tkc/utils.h"

ret_t vgcanvas_set_assets_manager(vgcanvas_t* vg, assets_manager_t* assets_manager) {
  return_value_if_fail(vg != NULL, RET_BAD_PARAMS);

  vg->assets_manager = assets_manager;

  return RET_OK;
}

ret_t vgcanvas_reset(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->reset != NULL, RET_BAD_PARAMS);

  return vg->vt->reset(vg);
}

ret_t vgcanvas_flush(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->flush != NULL, RET_BAD_PARAMS);

  return vg->vt->flush(vg);
}

ret_t vgcanvas_clear_rect(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h, color_t c) {
  return_value_if_fail(vg != NULL && vg->vt->clear_rect != NULL, RET_BAD_PARAMS);

  fix_xywh(x, y, w, h);
  return vg->vt->clear_rect(vg, x, y, w, h, c);
}

ret_t vgcanvas_move_to(vgcanvas_t* vg, float_t x, float_t y) {
  return_value_if_fail(vg != NULL && vg->vt->move_to != NULL, RET_BAD_PARAMS);

  return vg->vt->move_to(vg, x, y);
}

ret_t vgcanvas_line_to(vgcanvas_t* vg, float_t x, float_t y) {
  return_value_if_fail(vg != NULL && vg->vt->line_to != NULL, RET_BAD_PARAMS);

  return vg->vt->line_to(vg, x, y);
}

ret_t vgcanvas_rotate(vgcanvas_t* vg, float_t rad) {
  return_value_if_fail(vg != NULL && vg->vt->rotate != NULL, RET_BAD_PARAMS);

  return vg->vt->rotate(vg, rad);
}

ret_t vgcanvas_scale(vgcanvas_t* vg, float_t x, float_t y) {
  return_value_if_fail(vg != NULL && vg->vt->scale != NULL, RET_BAD_PARAMS);

  return vg->vt->scale(vg, x, y);
}

ret_t vgcanvas_translate(vgcanvas_t* vg, float_t x, float_t y) {
  return_value_if_fail(vg != NULL && vg->vt->translate != NULL, RET_BAD_PARAMS);

  return vg->vt->translate(vg, x, y);
}

ret_t vgcanvas_clip_rect(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h) {
  return_value_if_fail(vg != NULL && vg->vt->clip_rect != NULL, RET_BAD_PARAMS);

  fix_xywh(x, y, w, h);
  vg->clip_rect.x = x;
  vg->clip_rect.y = y;
  vg->clip_rect.w = w;
  vg->clip_rect.h = h;

  return vg->vt->clip_rect(vg, x, y, w, h);
}

ret_t vgcanvas_intersect_clip_rect(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h) {
  ret_t ret = RET_OK;
  return_value_if_fail(vg != NULL && vg->vt->intersect_clip_rect != NULL, RET_BAD_PARAMS);

  fix_xywh(x, y, w, h);

  ret = vg->vt->intersect_clip_rect(vg, &x, &y, &w, &h);

  vg->clip_rect.x = x;
  vg->clip_rect.y = y;
  vg->clip_rect.w = w;
  vg->clip_rect.h = h;

  return ret;
}

ret_t vgcanvas_fill(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->fill != NULL, RET_BAD_PARAMS);

  return vg->vt->fill(vg);
}

ret_t vgcanvas_stroke(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->stroke != NULL, RET_BAD_PARAMS);

  return vg->vt->stroke(vg);
}

ret_t vgcanvas_paint(vgcanvas_t* vg, bool_t stroke, bitmap_t* img) {
  return_value_if_fail(vg != NULL && vg->vt->paint != NULL && img != NULL, RET_BAD_PARAMS);

  return vg->vt->paint(vg, stroke, img);
}

ret_t vgcanvas_destroy(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->destroy != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(vg->font);
  TKMEM_FREE(vg->text_baseline);
  TKMEM_FREE(vg->text_align);

  return vg->vt->destroy(vg);
}

ret_t vgcanvas_begin_path(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->begin_path != NULL, RET_BAD_PARAMS);

  return vg->vt->begin_path(vg);
}

ret_t vgcanvas_rounded_rect(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h, float_t r) {
  return_value_if_fail(vg != NULL && vg->vt->rounded_rect != NULL, RET_BAD_PARAMS);

  fix_xywh(x, y, w, h);
  vgcanvas_begin_path(vg);
  return vg->vt->rounded_rect(vg, x, y, w, h, r);
}

ret_t vgcanvas_rect(vgcanvas_t* vg, float_t x, float_t y, float_t w, float_t h) {
  return_value_if_fail(vg != NULL, RET_BAD_PARAMS);

  fix_xywh(x, y, w, h);
  return vgcanvas_rounded_rect(vg, x, y, w, h, 0);
}

ret_t vgcanvas_ellipse(vgcanvas_t* vg, float_t x, float_t y, float_t rx, float_t ry) {
  return_value_if_fail(vg != NULL && vg->vt->ellipse != NULL, RET_BAD_PARAMS);

  vgcanvas_begin_path(vg);
  return vg->vt->ellipse(vg, x, y, rx, ry);
}

ret_t vgcanvas_close_path(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->close_path != NULL, RET_BAD_PARAMS);

  return vg->vt->close_path(vg);
}

ret_t vgcanvas_path_winding(vgcanvas_t* vg, bool_t dir) {
  return_value_if_fail(vg != NULL && vg->vt->path_winding != NULL, RET_BAD_PARAMS);

  return vg->vt->path_winding(vg, dir);
}

ret_t vgcanvas_transform(vgcanvas_t* vg, float_t a, float_t b, float_t c, float_t d, float_t e,
                         float_t f) {
  return_value_if_fail(vg != NULL && vg->vt->transform != NULL, RET_BAD_PARAMS);

  return vg->vt->transform(vg, a, b, c, d, e, f);
}

ret_t vgcanvas_set_transform(vgcanvas_t* vg, float_t a, float_t b, float_t c, float_t d, float_t e,
                             float_t f) {
  return_value_if_fail(vg != NULL && vg->vt->set_transform != NULL, RET_BAD_PARAMS);

  return vg->vt->set_transform(vg, a, b, c, d, e, f);
}

ret_t vgcanvas_quad_to(vgcanvas_t* vg, float_t cpx, float_t cpy, float_t x, float_t y) {
  return_value_if_fail(vg != NULL && vg->vt->quad_to != NULL, RET_BAD_PARAMS);

  return vg->vt->quad_to(vg, cpx, cpy, x, y);
}

ret_t vgcanvas_bezier_to(vgcanvas_t* vg, float_t cp1x, float_t cp1y, float_t cp2x, float_t cp2y,
                         float_t x, float_t y) {
  return_value_if_fail(vg != NULL && vg->vt->bezier_to != NULL, RET_BAD_PARAMS);

  return vg->vt->bezier_to(vg, cp1x, cp1y, cp2x, cp2y, x, y);
}

ret_t vgcanvas_arc_to(vgcanvas_t* vg, float_t x1, float_t y1, float_t x2, float_t y2, float_t r) {
  return_value_if_fail(vg != NULL && vg->vt->arc_to != NULL, RET_BAD_PARAMS);

  return vg->vt->arc_to(vg, x1, y1, x2, y2, r);
}

ret_t vgcanvas_arc(vgcanvas_t* vg, float_t x, float_t y, float_t r, float_t start, float_t end,
                   bool_t ccw) {
  return_value_if_fail(vg != NULL && vg->vt->arc != NULL, RET_BAD_PARAMS);

  return vg->vt->arc(vg, x, y, r, start, end, ccw);
}

bool_t vgcanvas_is_point_in_path(vgcanvas_t* vg, float_t x, float_t y) {
  return_value_if_fail(vg != NULL && vg->vt->is_point_in_path != NULL, FALSE);

  return vg->vt->is_point_in_path(vg, x, y);
}

ret_t vgcanvas_set_font(vgcanvas_t* vg, const char* font) {
  return_value_if_fail(vg != NULL && vg->vt->set_font != NULL, RET_BAD_PARAMS);

  font = system_info_fix_font_name(font);
  vg->font = tk_str_copy(vg->font, font);

  return vg->vt->set_font(vg, vg->font);
}

ret_t vgcanvas_set_font_size(vgcanvas_t* vg, float_t size) {
  return_value_if_fail(vg != NULL && vg->vt->set_font_size != NULL, RET_BAD_PARAMS);

  size = system_info()->font_scale * size;
  vg->font_size = size;

  return vg->vt->set_font_size(vg, size);
}

ret_t vgcanvas_set_text_align(vgcanvas_t* vg, const char* text_align) {
  return_value_if_fail(vg != NULL && vg->vt->set_text_align != NULL && text_align != NULL,
                       RET_BAD_PARAMS);

  vg->text_align = tk_str_copy(vg->text_align, text_align);

  return vg->vt->set_text_align(vg, text_align);
}

ret_t vgcanvas_set_text_baseline(vgcanvas_t* vg, const char* text_baseline) {
  return_value_if_fail(vg != NULL && vg->vt->set_text_baseline != NULL && text_baseline != NULL,
                       RET_BAD_PARAMS);

  vg->text_baseline = tk_str_copy(vg->text_baseline, text_baseline);

  return vg->vt->set_text_baseline(vg, text_baseline);
}

ret_t vgcanvas_fill_text(vgcanvas_t* vg, const char* text, float_t x, float_t y,
                         float_t max_width) {
  return_value_if_fail(vg != NULL && vg->vt->fill_text != NULL && text != NULL, RET_BAD_PARAMS);

  return vg->vt->fill_text(vg, text, x, y, max_width);
}

float_t vgcanvas_measure_text(vgcanvas_t* vg, const char* text) {
  return_value_if_fail(vg != NULL && vg->vt->measure_text != NULL && text != NULL, RET_BAD_PARAMS);

  return vg->vt->measure_text(vg, text);
}

ret_t vgcanvas_draw_image(vgcanvas_t* vg, bitmap_t* img, float_t sx, float_t sy, float_t sw,
                          float_t sh, float_t dx, float_t dy, float_t dw, float_t dh) {
  return_value_if_fail(vg != NULL && vg->vt->draw_image != NULL && img != NULL, RET_BAD_PARAMS);

  return vg->vt->draw_image(vg, img, sx, sy, sw, sh, dx, dy, dw, dh);
}

ret_t vgcanvas_set_antialias(vgcanvas_t* vg, bool_t value) {
  return_value_if_fail(vg != NULL && vg->vt->set_antialias != NULL, RET_BAD_PARAMS);

  vg->anti_alias = value;

  return vg->vt->set_antialias(vg, value);
}

ret_t vgcanvas_set_line_width(vgcanvas_t* vg, float_t value) {
  return_value_if_fail(vg != NULL && vg->vt->set_line_width != NULL, RET_BAD_PARAMS);
  vg->line_width = value;

  return vg->vt->set_line_width(vg, value);
}

ret_t vgcanvas_set_global_alpha(vgcanvas_t* vg, float_t value) {
  return_value_if_fail(vg != NULL && vg->vt->set_global_alpha != NULL, RET_BAD_PARAMS);

  vg->global_alpha = value;

  return vg->vt->set_global_alpha(vg, value);
}

ret_t vgcanvas_set_fill_color(vgcanvas_t* vg, color_t value) {
  return_value_if_fail(vg != NULL && vg->vt->set_fill_color != NULL, RET_BAD_PARAMS);

  vg->fill_color = value;

  return vg->vt->set_fill_color(vg, value);
}

ret_t vgcanvas_set_fill_color_str(vgcanvas_t* vg, const char* color) {
  color_t c = color_parse(color);

  return vgcanvas_set_fill_color(vg, c);
}

ret_t vgcanvas_set_fill_linear_gradient(vgcanvas_t* vg, float_t sx, float_t sy, float_t ex,
                                        float_t ey, color_t icolor, color_t ocolor) {
  return_value_if_fail(vg != NULL && vg->vt->set_fill_linear_gradient != NULL, RET_BAD_PARAMS);

  return vg->vt->set_fill_linear_gradient(vg, sx, sy, ex, ey, icolor, ocolor);
}

ret_t vgcanvas_set_fill_radial_gradient(vgcanvas_t* vg, float_t cx, float_t cy, float_t inr,
                                        float_t outr, color_t icolor, color_t ocolor) {
  return_value_if_fail(vg != NULL && vg->vt->set_fill_radial_gradient != NULL, RET_BAD_PARAMS);

  return vg->vt->set_fill_radial_gradient(vg, cx, cy, inr, outr, icolor, ocolor);
}

ret_t vgcanvas_set_stroke_color(vgcanvas_t* vg, color_t value) {
  return_value_if_fail(vg != NULL && vg->vt->set_stroke_color != NULL, RET_BAD_PARAMS);

  vg->stroke_color = value;

  return vg->vt->set_stroke_color(vg, value);
}

ret_t vgcanvas_set_stroke_color_str(vgcanvas_t* vg, const char* color) {
  color_t c = color_parse(color);

  return vgcanvas_set_stroke_color(vg, c);
}

ret_t vgcanvas_set_stroke_linear_gradient(vgcanvas_t* vg, float_t sx, float_t sy, float_t ex,
                                          float_t ey, color_t icolor, color_t ocolor) {
  return_value_if_fail(vg != NULL && vg->vt->set_stroke_linear_gradient != NULL, RET_BAD_PARAMS);

  return vg->vt->set_stroke_linear_gradient(vg, sx, sy, ex, ey, icolor, ocolor);
}

ret_t vgcanvas_set_stroke_radial_gradient(vgcanvas_t* vg, float_t cx, float_t cy, float_t inr,
                                          float_t outr, color_t icolor, color_t ocolor) {
  return_value_if_fail(vg != NULL && vg->vt->set_stroke_radial_gradient != NULL, RET_BAD_PARAMS);

  return vg->vt->set_stroke_radial_gradient(vg, cx, cy, inr, outr, icolor, ocolor);
}

ret_t vgcanvas_set_line_cap(vgcanvas_t* vg, const char* value) {
  return_value_if_fail(vg != NULL && vg->vt->set_line_cap != NULL && value != NULL, RET_BAD_PARAMS);

  vg->line_cap = value;

  return vg->vt->set_line_cap(vg, value);
}

ret_t vgcanvas_set_line_join(vgcanvas_t* vg, const char* value) {
  return_value_if_fail(vg != NULL && vg->vt->set_line_join != NULL && value != NULL,
                       RET_BAD_PARAMS);

  vg->line_join = value;

  return vg->vt->set_line_join(vg, value);
}

ret_t vgcanvas_set_miter_limit(vgcanvas_t* vg, float_t value) {
  return_value_if_fail(vg != NULL && vg->vt->set_miter_limit != NULL, RET_BAD_PARAMS);

  vg->miter_limit = value;

  return vg->vt->set_miter_limit(vg, value);
}

ret_t vgcanvas_begin_frame(vgcanvas_t* vg, rect_t* dirty_rect) {
  return_value_if_fail(vg != NULL && vg->vt->begin_frame != NULL, RET_BAD_PARAMS);

  return vg->vt->begin_frame(vg, dirty_rect);
}

ret_t vgcanvas_save(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->save != NULL, RET_BAD_PARAMS);

  return vg->vt->save(vg);
}

ret_t vgcanvas_restore(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->restore != NULL, RET_BAD_PARAMS);

  return vg->vt->restore(vg);
}

ret_t vgcanvas_end_frame(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt->end_frame != NULL, RET_BAD_PARAMS);

  return vg->vt->end_frame(vg);
}

ret_t vgcanvas_create_fbo(vgcanvas_t* vg, uint32_t w, uint32_t h, framebuffer_object_t* fbo) {
  return_value_if_fail(vg != NULL && vg->vt->create_fbo != NULL && fbo != NULL, RET_BAD_PARAMS);

  return vg->vt->create_fbo(vg, w, h, fbo);
}

ret_t vgcanvas_destroy_fbo(vgcanvas_t* vg, framebuffer_object_t* fbo) {
  return_value_if_fail(vg != NULL && vg->vt->destroy_fbo != NULL && fbo != NULL, RET_BAD_PARAMS);

  return vg->vt->destroy_fbo(vg, fbo);
}

ret_t vgcanvas_bind_fbo(vgcanvas_t* vg, framebuffer_object_t* fbo) {
  return_value_if_fail(vg != NULL && vg->vt->bind_fbo != NULL && fbo != NULL, RET_BAD_PARAMS);

  return vg->vt->bind_fbo(vg, fbo);
}

ret_t vgcanvas_unbind_fbo(vgcanvas_t* vg, framebuffer_object_t* fbo) {
  return_value_if_fail(vg != NULL && vg->vt->unbind_fbo != NULL && fbo != NULL, RET_BAD_PARAMS);

  return vg->vt->unbind_fbo(vg, fbo);
}

ret_t vgcanvas_draw_icon(vgcanvas_t* vg, bitmap_t* img, float_t sx, float_t sy, float_t sw,
                         float_t sh, float_t dx, float_t dy, float_t dw, float_t dh) {
  float_t x = 0;
  float_t y = 0;
  float_t w = 0;
  float_t h = 0;
  return_value_if_fail(vg != NULL && img != NULL, RET_BAD_PARAMS);

  w = sw / vg->ratio;
  h = sh / vg->ratio;
  x = (dw - w) * 0.5f + dx;
  y = (dh - h) * 0.5f + dy;

  return vgcanvas_draw_image(vg, img, sx, sy, sw, sh, x, y, w, h);
}

ret_t vgcanvas_reinit(vgcanvas_t* vg, uint32_t w, uint32_t h, uint32_t stride,
                      bitmap_format_t format, void* data) {
  return_value_if_fail(vg != NULL && data != NULL, RET_BAD_PARAMS);

  if (vg->w == w && vg->h == h && vg->buff == (uint32_t*)data) {
    return RET_OK;
  }

  if (vg->vt->reinit != NULL) {
    return vg->vt->reinit(vg, w, h, stride, format, data);
  }

  return RET_NOT_IMPL;
}

vgcanvas_t* vgcanvas_cast(vgcanvas_t* vg) {
  return vg;
}

ret_t fbo_to_img(framebuffer_object_t* fbo, bitmap_t* img) {
  return_value_if_fail(fbo != NULL && img != NULL, RET_BAD_PARAMS);

  memset(img, 0x00, sizeof(bitmap_t));
  img->specific = tk_pointer_from_int(fbo->id);
  img->specific_ctx = NULL;
  img->specific_destroy = NULL;
  img->w = fbo->w * fbo->ratio;
  img->h = fbo->h * fbo->ratio;

  img->flags = BITMAP_FLAG_TEXTURE;

  return RET_OK;
}

ret_t vgcanvas_fbo_to_bitmap(vgcanvas_t* vg, framebuffer_object_t* fbo, bitmap_t* img, rect_t* r) {
  return_value_if_fail(vg != NULL && fbo != NULL && img != NULL, RET_BAD_PARAMS);
  if (vg->vt != NULL && vg->vt->fbo_to_bitmap != NULL) {
    return vg->vt->fbo_to_bitmap(vg, fbo, img, r);
  }

  return RET_NOT_IMPL;
}

wh_t vgcanvas_get_width(vgcanvas_t* vgcanvas) {
  return_value_if_fail(vgcanvas != NULL && vgcanvas->vt != NULL, 0);

  if (vgcanvas->vt->get_width != NULL) {
    return vgcanvas->vt->get_width(vgcanvas);
  } else {
    return vgcanvas->w;
  }
}

wh_t vgcanvas_get_height(vgcanvas_t* vgcanvas) {
  return_value_if_fail(vgcanvas != NULL && vgcanvas->vt != NULL, 0);

  if (vgcanvas->vt->get_height != NULL) {
    return vgcanvas->vt->get_height(vgcanvas);
  } else {
    return vgcanvas->h;
  }
}

ret_t vgcanvas_get_text_metrics(vgcanvas_t* vg, float_t* ascent, float_t* descent,
                                float_t* line_hight) {
  return_value_if_fail(vg != NULL && vg->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(vg->vt->get_text_metrics != NULL, RET_BAD_PARAMS);

  return vg->vt->get_text_metrics(vg, ascent, descent, line_hight);
}

ret_t vgcanvas_clear_cache(vgcanvas_t* vg) {
  return_value_if_fail(vg != NULL && vg->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(vg->vt->clear_cache != NULL, RET_BAD_PARAMS);

   return vg->vt->clear_cache(vg);
}
