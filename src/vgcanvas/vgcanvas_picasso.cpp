/**
 * File:   vgcanvas_picasso.cpp
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  vector graphics canvas base on picasso
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-04-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "picasso.h"
#include "base/mem.h"
#include "base/utf8.h"
#include "base/matrix.h"
#include "base/vgcanvas.h"
#include "picasso_objects.h"
#include "base/resource_manager.h"

typedef struct _vgcanvas_picasso_t {
  vgcanvas_t base;

  float_t ratio;
  uint32_t text_align_v;
  uint32_t text_align_h;

  ps_path* path;
  ps_context* vg;
  ps_canvas* canvas;
  matrix_t matrix;
} vgcanvas_picasso_t;

static ps_color_format to_ps_color_format(uint16_t format) {
  ps_color_format fmt = COLOR_FORMAT_ABGR;

  if (format == BITMAP_FMT_RGBA) {
    fmt = COLOR_FORMAT_ABGR;
  } else if (format == BITMAP_FMT_RGB565) {
    fmt = COLOR_FORMAT_RGB565;
  } else {
    assert(!"not supported format!");
  }

  return fmt;
}

static ps_point ps_point_init(float_t x, float_t y) {
  ps_point p = {x, y};

  return p;
}

static ps_color ps_color_init(color_t color) {
  ps_color c;
  c.r = color.rgba.r / 255.0;
  c.g = color.rgba.g / 255.0;
  c.b = color.rgba.b / 255.0;
  c.a = color.rgba.a / 255.0;

  return c;
}

static ret_t picasso_on_bitmap_destroy(bitmap_t* img) {
  ps_image* pimg = (ps_image*)img->specific;

  if (pimg != NULL) {
    ps_image_unref(pimg);
  }

  img->specific = NULL;
  img->specific_ctx = NULL;
  img->specific_destroy = NULL;

  return RET_OK;
}

static ps_image* vgcanvas_picasso_ensure_image(vgcanvas_picasso_t* canvas, bitmap_t* img) {
  ps_image* pimg = (ps_image*)img->specific;

  if (pimg == NULL) {
    pimg = ps_image_create_with_data((ps_byte*)img->data, to_ps_color_format(img->format), img->w,
                                     img->h, img->w * 4);
    pimg->buffer.set_transparent(!(img->flags & BITMAP_FLAG_OPAQUE));

    if (pimg != NULL) {
      img->specific = pimg;
      img->specific_ctx = NULL;
      img->specific_destroy = picasso_on_bitmap_destroy;
    }
  }

  return pimg;
}

ret_t vgcanvas_picasso_begin_frame(vgcanvas_t* vgcanvas, rect_t* dirty_rect) {
  ps_color color = {0, 0, 0, 0};
  vgcanvas_picasso_t* picasso = (vgcanvas_picasso_t*)vgcanvas;
  ps_context* vg = picasso->vg;

  picasso->ratio = 1;

  ps_set_source_color(vg, &color);
  ps_set_line_cap(vg, LINE_CAP_BUTT);
  ps_set_composite_operator(vg, COMPOSITE_SRC_OVER);
  ps_set_antialias(vg, TRUE);

  ps_clear(vg);
  ps_identity(vg);

  (void)dirty_rect;

  return RET_OK;
}

ret_t vgcanvas_picasso_end_frame(vgcanvas_t* vgcanvas) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;
  (void)vg;

  return RET_OK;
}

static ret_t vgcanvas_picasso_reset(vgcanvas_t* vgcanvas) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_identity(vg);
  ps_set_line_cap(vg, LINE_CAP_BUTT);
  ps_set_composite_operator(vg, COMPOSITE_SRC_OVER);

  return RET_OK;
}

static ret_t vgcanvas_picasso_flush(vgcanvas_t* vgcanvas) {
  (void)vgcanvas;

  return RET_OK;
}

static ret_t vgcanvas_picasso_clear_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                         float_t h, color_t c) {
  vgcanvas_set_fill_color(vgcanvas, c);
  vgcanvas_begin_path(vgcanvas);
  vgcanvas_rect(vgcanvas, x, y, w, h);
  vgcanvas_fill(vgcanvas);

  return RET_OK;
}

static ret_t vgcanvas_picasso_begin_path(vgcanvas_t* vgcanvas) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_new_path(vg);

  return RET_OK;
}

static ret_t vgcanvas_picasso_move_to(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  ps_point p = ps_point_init(x, y);
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_move_to(vg, &p);

  return RET_OK;
}

static ret_t vgcanvas_picasso_line_to(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  ps_point p = ps_point_init(x, y);
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_line_to(vg, &p);

  return RET_OK;
}

static ret_t vgcanvas_picasso_quad_to(vgcanvas_t* vgcanvas, float_t cpx, float_t cpy, float_t x,
                                      float_t y) {
  ps_point p = ps_point_init(x, y);
  ps_point cp = ps_point_init(cpx, cpy);
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_quad_curve_to(vg, &cp, &p);

  return RET_OK;
}

static ret_t vgcanvas_picasso_bezier_to(vgcanvas_t* vgcanvas, float_t cp1x, float_t cp1y,
                                        float_t cp2x, float_t cp2y, float_t x, float_t y) {
  ps_point p = ps_point_init(x, y);
  ps_point cp1 = ps_point_init(cp1x, cp1y);
  ps_point cp2 = ps_point_init(cp2x, cp2y);
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_bezier_curve_to(vg, &cp1, &cp2, &p);

  return RET_OK;
}

static ret_t vgcanvas_picasso_arc_to(vgcanvas_t* vgcanvas, float_t x1, float_t y1, float_t x2,
                                     float_t y2, float_t r) {
  ps_point p1 = ps_point_init(x1, y1);
  ps_point p2 = ps_point_init(x2, y2);
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  /*TODO:
   * ps_arc_to(vg, r, &p1, &p2);
   */
  (void)p1;
  (void)p2;
  (void)vg;

  return RET_OK;
}

static ret_t vgcanvas_picasso_arc(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t r,
                                  float_t start, float_t end, bool_t ccw) {
  ps_point cp = ps_point_init(x, y);
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_arc(vg, &cp, r, start, end, !ccw);

  return RET_OK;
}

static bool_t vgcanvas_picasso_is_point_in_path(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;
  (void)vg;
  (void)x;
  (void)y;

  /*TODO*/

  return FALSE;
}

static ret_t sync_ps_matrix_to(vgcanvas_picasso_t* picasso) {
  ps_context* vg = picasso->vg;
  matrix_t* m = &(picasso->matrix);

  ps_matrix* pm = ps_matrix_create_init(m->a0, m->a1, m->a2, m->a3, m->a4, m->a5);
  ps_set_matrix(vg, pm);
  ps_matrix_unref(pm);

  return RET_OK;
}

static ret_t sync_ps_matrix_from(vgcanvas_picasso_t* picasso) {
  ps_context* vg = picasso->vg;
  matrix_t* m = &(picasso->matrix);
  ps_matrix* pm = ps_matrix_create_init(1, 0, 0, 1, 0, 0);

  ps_get_matrix(vg, pm);

  m->a0 = pm->matrix.sx();
  m->a1 = pm->matrix.shy();
  m->a2 = pm->matrix.shx();
  m->a3 = pm->matrix.sy();
  m->a4 = pm->matrix.tx();
  m->a5 = pm->matrix.ty();

  ps_matrix_unref(pm);

  return RET_OK;
}

static ret_t vgcanvas_picasso_rotate(vgcanvas_t* vgcanvas, float_t rad) {
  vgcanvas_picasso_t* picasso = (vgcanvas_picasso_t*)vgcanvas;

  matrix_rotate(&(picasso->matrix), rad);
  sync_ps_matrix_to(picasso);

  return RET_OK;
}

static ret_t vgcanvas_picasso_scale(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  vgcanvas_picasso_t* picasso = (vgcanvas_picasso_t*)vgcanvas;

  matrix_scale(&(picasso->matrix), x, y);
  sync_ps_matrix_to(picasso);

  return RET_OK;
}

static ret_t vgcanvas_picasso_translate(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  vgcanvas_picasso_t* picasso = (vgcanvas_picasso_t*)vgcanvas;

  matrix_translate(&(picasso->matrix), x, y);
  sync_ps_matrix_to(picasso);

  return RET_OK;
}

static ret_t vgcanvas_picasso_transform(vgcanvas_t* vgcanvas, float_t a, float_t b, float_t c,
                                        float_t d, float_t e, float_t f) {
  matrix_t mb;
  vgcanvas_picasso_t* picasso = (vgcanvas_picasso_t*)vgcanvas;

  matrix_set(&mb, a, b, c, d, e, f);
  matrix_multiply(&(picasso->matrix), &mb);
  sync_ps_matrix_to(picasso);

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_transform(vgcanvas_t* vgcanvas, float_t a, float_t b, float_t c,
                                            float_t d, float_t e, float_t f) {
  vgcanvas_picasso_t* picasso = (vgcanvas_picasso_t*)vgcanvas;

  matrix_set(&(picasso->matrix), a, b, c, d, e, f);
  sync_ps_matrix_to(picasso);

  return RET_OK;
}

static ret_t vgcanvas_picasso_rounded_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                           float_t h, float_t r) {
  ps_rect rect;
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  ps_rounded_rect(vg, &rect, r, r, r, r, r, r, r, r);

  return RET_OK;
}

static ret_t vgcanvas_picasso_ellipse(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t rx,
                                      float_t ry) {
  ps_rect rect;
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  rect.x = x - rx;
  rect.y = y - ry;
  rect.w = 2 * rx;
  rect.h = 2 * ry;

  ps_ellipse(vg, &rect);

  return RET_OK;
}

static ret_t vgcanvas_picasso_close_path(vgcanvas_t* vgcanvas) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_close_path(vg);

  return RET_OK;
}

static ret_t vgcanvas_picasso_fill(vgcanvas_t* vgcanvas) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_fill(vg);

  return RET_OK;
}

static ret_t vgcanvas_picasso_clip_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                        float_t h) {
  ps_rect rect;
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;

  ps_clip_rect(vg, &rect);

  return RET_OK;
}

static ret_t vgcanvas_picasso_stroke(vgcanvas_t* vgcanvas) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_stroke(vg);

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_font_size(vgcanvas_t* vgcanvas, float_t size) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;
  (void)vg;

  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_picasso_set_font(vgcanvas_t* vgcanvas, const char* name) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  if (name == NULL) {
    name = STR_DEFAULT_FONT;
  }

  (void)vg;

  /*TODO*/

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_text_align(vgcanvas_t* vgcanvas, const char* text_align) {
  vgcanvas_picasso_t* picasso = (vgcanvas_picasso_t*)vgcanvas;

  if (text_align[0] == 'r') {
    picasso->text_align_h = TEXT_ALIGN_RIGHT;
  } else if (text_align[0] == 'c') {
    picasso->text_align_h = TEXT_ALIGN_CENTER;
  } else {
    picasso->text_align_h = TEXT_ALIGN_LEFT;
  }

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_text_baseline(vgcanvas_t* vgcanvas, const char* text_baseline) {
  vgcanvas_picasso_t* picasso = (vgcanvas_picasso_t*)vgcanvas;

  if (text_baseline[0] == 'b') {
    picasso->text_align_v = TEXT_ALIGN_BOTTOM;
  } else if (text_baseline[0] == 'm') {
    picasso->text_align_v = TEXT_ALIGN_CENTER;
  } else {
    picasso->text_align_v = TEXT_ALIGN_TOP;
  }

  return RET_OK;
}

static ret_t vgcanvas_picasso_fill_text(vgcanvas_t* vgcanvas, const char* text, float_t x,
                                        float_t y, float_t max_width) {
  ps_color color = ps_color_init(vgcanvas->fill_color);
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_set_text_color(vg, &color);
  ps_text_out_length(vg, x, y, text, strlen(text));

  (void)max_width;

  return RET_OK;
}

static float_t vgcanvas_picasso_measure_text(vgcanvas_t* vgcanvas, const char* text) {
  ps_size size = {0, 0};
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_get_text_extent(vg, text, strlen(text), &size);

  return size.w;
}

static ret_t vgcanvas_picasso_draw_image(vgcanvas_t* vgcanvas, bitmap_t* img, float_t sx,
                                         float_t sy, float_t sw, float_t sh, float_t dx, float_t dy,
                                         float_t dw, float_t dh) {
  float_t x = 0;
  float_t y = 0;
  float_t w = 0;
  float_t h = 0;
  float_t scale_x = dw / sw;
  float_t scale_y = dh / sh;
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;
  ps_image* pimg = vgcanvas_picasso_ensure_image((vgcanvas_picasso_t*)vgcanvas, img);

  vgcanvas_save(vgcanvas);

  ps_set_source_image(vg, pimg);
  ps_set_filter(vg, FILTER_NEAREST);
  vgcanvas_begin_path(vgcanvas);

  x = dx - scale_x * sx;
  y = dy - scale_y * sy;
  w = img->w * scale_x;
  h = img->h * scale_y;

  vgcanvas_clip_rect(vgcanvas, dx, dy, dw, dh);
  vgcanvas_rect(vgcanvas, x, y, w, h);
  vgcanvas_fill(vgcanvas);
  vgcanvas_restore(vgcanvas);

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_antialias(vgcanvas_t* vgcanvas, bool_t value) {
  (void)vgcanvas;
  (void)value;
  /*always*/

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_line_width(vgcanvas_t* vgcanvas, float_t value) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_set_line_width(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_global_alpha(vgcanvas_t* vgcanvas, float_t value) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_set_alpha(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_fill_color(vgcanvas_t* vgcanvas, color_t c) {
  ps_color color = ps_color_init(c);
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_set_source_color(vg, &color);

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_stroke_color(vgcanvas_t* vgcanvas, color_t c) {
  ps_color color = ps_color_init(c);
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_set_stroke_color(vg, &color);

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_line_cap(vgcanvas_t* vgcanvas, const char* value) {
  ps_line_cap line_cap = LINE_CAP_BUTT;
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  if (*value == 'r') {
    line_cap = LINE_CAP_ROUND;
  } else if (*value == 's') {
    line_cap = LINE_CAP_SQUARE;
  }

  ps_set_line_cap(vg, line_cap);

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_line_join(vgcanvas_t* vgcanvas, const char* value) {
  ps_line_join line_join = LINE_JOIN_MITER;
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  if (*value == 'r') {
    line_join = LINE_JOIN_ROUND;
  } else if (*value == 'b') {
    line_join = LINE_JOIN_BEVEL;
  }

  ps_set_line_join(vg, line_join);

  return RET_OK;
}

static ret_t vgcanvas_picasso_set_miter_limit(vgcanvas_t* vgcanvas, float_t value) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_set_miter_limit(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_picasso_save(vgcanvas_t* vgcanvas) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  sync_ps_matrix_to((vgcanvas_picasso_t*)vgcanvas);
  ps_save(vg);

  return RET_OK;
}

static ret_t vgcanvas_picasso_restore(vgcanvas_t* vgcanvas) {
  ps_context* vg = ((vgcanvas_picasso_t*)vgcanvas)->vg;

  ps_restore(vg);
  sync_ps_matrix_from((vgcanvas_picasso_t*)vgcanvas);

  return RET_OK;
}

static ret_t vgcanvas_picasso_create_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_picasso_destroy_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  /*TODO*/

  return RET_OK;
}

static ret_t vgcanvas_picasso_bind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  /*TODO*/

  return RET_OK;
}

static ret_t vgcanvas_picasso_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  /*TODO*/

  return RET_OK;
}

static ret_t vgcanvas_picasso_destroy(vgcanvas_t* vgcanvas) {
  vgcanvas_picasso_t* picasso = (vgcanvas_picasso_t*)vgcanvas;

  ps_context_unref(picasso->vg);
  ps_canvas_unref(picasso->canvas);
  ps_shutdown();

  return RET_OK;
}

static const vgcanvas_vtable_t vt = {vgcanvas_picasso_begin_frame,
                                     vgcanvas_picasso_reset,
                                     vgcanvas_picasso_flush,
                                     vgcanvas_picasso_clear_rect,
                                     vgcanvas_picasso_begin_path,
                                     vgcanvas_picasso_move_to,
                                     vgcanvas_picasso_line_to,
                                     vgcanvas_picasso_arc,
                                     vgcanvas_picasso_arc_to,
                                     vgcanvas_picasso_bezier_to,
                                     vgcanvas_picasso_quad_to,
                                     vgcanvas_picasso_is_point_in_path,
                                     vgcanvas_picasso_ellipse,
                                     vgcanvas_picasso_rounded_rect,
                                     vgcanvas_picasso_close_path,
                                     vgcanvas_picasso_scale,
                                     vgcanvas_picasso_rotate,
                                     vgcanvas_picasso_translate,
                                     vgcanvas_picasso_transform,
                                     vgcanvas_picasso_set_transform,
                                     vgcanvas_picasso_fill,
                                     vgcanvas_picasso_clip_rect,
                                     vgcanvas_picasso_stroke,
                                     vgcanvas_picasso_set_font,
                                     vgcanvas_picasso_set_font_size,
                                     vgcanvas_picasso_set_text_align,
                                     vgcanvas_picasso_set_text_baseline,
                                     vgcanvas_picasso_fill_text,
                                     vgcanvas_picasso_measure_text,
                                     vgcanvas_picasso_draw_image,
                                     vgcanvas_picasso_set_antialias,
                                     vgcanvas_picasso_set_global_alpha,
                                     vgcanvas_picasso_set_line_width,
                                     vgcanvas_picasso_set_fill_color,
                                     vgcanvas_picasso_set_stroke_color,
                                     vgcanvas_picasso_set_line_join,
                                     vgcanvas_picasso_set_line_cap,
                                     vgcanvas_picasso_set_miter_limit,
                                     vgcanvas_picasso_save,
                                     vgcanvas_picasso_restore,
                                     vgcanvas_picasso_end_frame,
                                     vgcanvas_picasso_create_fbo,
                                     vgcanvas_picasso_destroy_fbo,
                                     vgcanvas_picasso_bind_fbo,
                                     vgcanvas_picasso_unbind_fbo,
                                     vgcanvas_picasso_destroy};

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, bitmap_format_t format, void* buff) {
  int pitch = 0;
  ps_color_format fmt = to_ps_color_format(format);
  vgcanvas_picasso_t* picasso = (vgcanvas_picasso_t*)TKMEM_ZALLOC(vgcanvas_picasso_t);
  return_value_if_fail(picasso != NULL && buff != NULL, NULL);
  return_value_if_fail(format == BITMAP_FMT_RGBA || format == BITMAP_FMT_RGB565, NULL);

  picasso->base.w = w;
  picasso->base.h = h;
  picasso->base.ratio = 1;
  picasso->base.vt = &vt;

  if (format == BITMAP_FMT_RGBA) {
    pitch = w * 4;
  } else {
    pitch = w * 2;
  }

  ps_initialize();
  matrix_init(&(picasso->matrix));
  picasso->canvas = ps_canvas_create_with_data((ps_byte*)buff, fmt, w, h, pitch);
  picasso->vg = ps_context_create(picasso->canvas, 0);
  assert(picasso->canvas != NULL);

  return &(picasso->base);
}
