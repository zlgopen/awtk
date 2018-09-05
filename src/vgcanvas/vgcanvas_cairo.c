/**
 * File:   vgcanvas_cairo.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas base on cairo
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
 * 2018-09-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/utf8.h"
#include "base/image_manager.h"
#include "base/assets_manager.h"

#include "cairo.h"
#include "base/mem.h"
#include "base/vgcanvas.h"

typedef struct _vgcanvas_cairo_t {
  vgcanvas_t base;

  cairo_t* vg;
  cairo_surface_t* surface;
} vgcanvas_cairo_t;

ret_t vgcanvas_cairo_begin_frame(vgcanvas_t* vgcanvas, rect_t* dirty_rect) {
  rect_t* r = dirty_rect;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;
  cairo_t* vg = canvas->vg;

  cairo_new_path(vg);
  cairo_rectangle(vg, r->x, r->y, r->w, r->h);
  cairo_clip(vg);
  cairo_new_path(vg);
  cairo_save(vg);
  vgcanvas->global_alpha = 0xff;

  return RET_OK;
}

ret_t vgcanvas_cairo_end_frame(vgcanvas_t* vgcanvas) {
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;
  cairo_t* vg = canvas->vg;
  cairo_restore(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_reset(vgcanvas_t* vgcanvas) {
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;
  cairo_t* vg = canvas->vg;
  cairo_new_path(vg);
  vgcanvas->global_alpha = 0xff;

  return RET_OK;
}

static ret_t vgcanvas_cairo_flush(vgcanvas_t* vgcanvas) {
  (void)vgcanvas;

  return RET_OK;
}

static ret_t vgcanvas_cairo_clear_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                       float_t h, color_t c) {
  vgcanvas_set_fill_color(vgcanvas, c);
  vgcanvas_rounded_rect(vgcanvas, x, y, w, h, 0);
  vgcanvas_fill(vgcanvas);

  return RET_OK;
}

static ret_t vgcanvas_cairo_begin_path(vgcanvas_t* vgcanvas) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_new_path(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_move_to(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_move_to(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_cairo_line_to(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_line_to(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_cairo_quad_to(vgcanvas_t* vgcanvas, float_t cpx, float_t cpy, float_t x,
                                    float_t y) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_curve_to(vg, cpx, cpy, cpx, cpy, x, y);

  return RET_OK;
}

static ret_t vgcanvas_cairo_bezier_to(vgcanvas_t* vgcanvas, float_t cp1x, float_t cp1y,
                                      float_t cp2x, float_t cp2y, float_t x, float_t y) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_curve_to(vg, cp1x, cp1y, cp2x, cp2y, x, y);

  return RET_OK;
}

static ret_t vgcanvas_cairo_arc_to(vgcanvas_t* vgcanvas, float_t x1, float_t y1, float_t x2,
                                   float_t y2, float_t r) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  (void)vg;
  (void)x1;
  (void)x2;
  (void)y1;
  (void)y2;
  (void)r;

  return RET_NOT_IMPL;
}

static ret_t vgcanvas_cairo_arc(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t r,
                                float_t start, float_t end, bool_t ccw) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  if (ccw) {
    cairo_arc_negative(vg, x, y, r, start, end);
  } else {
    cairo_arc(vg, x, y, r, start, end);
  }

  return RET_OK;
}

static bool_t vgcanvas_cairo_is_point_in_path(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  (void)vg;
  (void)x;
  (void)y;

  /*TODO*/

  return FALSE;
}

static ret_t vgcanvas_cairo_rotate(vgcanvas_t* vgcanvas, float_t rad) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_rotate(vg, rad);

  return RET_OK;
}

static ret_t vgcanvas_cairo_scale(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_scale(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_cairo_translate(vgcanvas_t* vgcanvas, float_t x, float_t y) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_translate(vg, x, y);

  return RET_OK;
}

static ret_t vgcanvas_cairo_transform(vgcanvas_t* vgcanvas, float_t a, float_t b, float_t c,
                                      float_t d, float_t e, float_t f) {
  cairo_matrix_t m;
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_matrix_init(&m, a, b, c, d, e, f);
  cairo_transform(vg, &m);

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_transform(vgcanvas_t* vgcanvas, float_t a, float_t b, float_t c,
                                          float_t d, float_t e, float_t f) {
  cairo_matrix_t m;
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_matrix_init(&m, a, b, c, d, e, f);
  cairo_set_matrix(vg, &m);

  return RET_OK;
}

static ret_t vgcanvas_cairo_rounded_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                         float_t h, float_t r) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  if (w == 0) {
    cairo_move_to(vg, x, y);
    cairo_line_to(vg, x, y + h);
  } else if (h == 0) {
    cairo_move_to(vg, x, y);
    cairo_line_to(vg, x + w, y);
  } else if (r <= 0) {
    cairo_rectangle(vg, x, y, w, h);
  } else {
    cairo_move_to(vg, x + r, y);
    cairo_line_to(vg, x + w - r, y);
    cairo_curve_to(vg, x + w, y, x + w, y, x + w, y + r);
    cairo_line_to(vg, x + w, y + h - r);
    cairo_curve_to(vg, x + w, y + h, x + w, y + h, x + w - r, y + h);
    cairo_line_to(vg, x + r, y + h);
    cairo_curve_to(vg, x, y + h, x, y + h, x, y + h - r);
    cairo_line_to(vg, x, y + r);
    cairo_curve_to(vg, x, y, x, y, x + r, y);
  }

  return RET_OK;
}

static ret_t vgcanvas_cairo_ellipse(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t rx,
                                    float_t ry) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  (void)vg;
  (void)x;
  (void)y;
  (void)rx;
  (void)ry;

  /*TODO*/
  return RET_OK;
}

static ret_t vgcanvas_cairo_close_path(vgcanvas_t* vgcanvas) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_close_path(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_fill(vgcanvas_t* vgcanvas) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  color_t c = vgcanvas->fill_color;

  cairo_set_source_rgba(vg, c.rgba.r, c.rgba.g, c.rgba.b, c.rgba.a);
  cairo_fill(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_clip_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                      float_t h) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_reset_clip(vg);
  cairo_rectangle(vg, x, y, w, h);
  cairo_clip(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_stroke(vgcanvas_t* vgcanvas) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  color_t c = vgcanvas->stroke_color;

  cairo_set_source_rgba(vg, c.rgba.r, c.rgba.g, c.rgba.b, c.rgba.a);
  cairo_stroke(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_font_size(vgcanvas_t* vgcanvas, float_t size) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  cairo_set_font_size(vg, size);

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_font(vgcanvas_t* vgcanvas, const char* name) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (name == NULL) {
    name = TK_DEFAULT_FONT;
  }

  cairo_select_font_face(vg, name, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_text_align(vgcanvas_t* vgcanvas, const char* text_align) {
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_text_baseline(vgcanvas_t* vgcanvas, const char* text_baseline) {
  (void)text_baseline;
  (void)vgcanvas;
  /*TODO*/

  return RET_OK;
}

static ret_t vgcanvas_cairo_fill_text(vgcanvas_t* vgcanvas, const char* text, float_t x, float_t y,
                                      float_t max_width) {
  (void)vgcanvas;
  (void)text;
  (void)x;
  (void)y;
  (void)max_width;

  /*TODO*/
  return RET_OK;
}

static float_t vgcanvas_cairo_measure_text(vgcanvas_t* vgcanvas, const char* text) {
  (void)vgcanvas;
  (void)text;
  /*TODO*/

  return 0;
}

static cairo_surface_t* create_surface(uint32_t w, uint32_t h, bitmap_format_t format,
                                       void* fbuff) {
  int32_t bpp = 0;
  cairo_format_t cairo_format = CAIRO_FORMAT_ARGB32;
  switch (format) {
    case BITMAP_FMT_RGBA: {
      bpp = 4;
      /*FIXME*/
      cairo_format = CAIRO_FORMAT_ARGB32;
      break;
    }
    case BITMAP_FMT_BGRA: {
      bpp = 4;
      cairo_format = CAIRO_FORMAT_ARGB32;
      break;
    }
    case BITMAP_FMT_RGB565: {
      bpp = 2;
      cairo_format = CAIRO_FORMAT_RGB16_565;
      break;
    }
    default: { return NULL; }
  }

  return cairo_image_surface_create_for_data(fbuff, cairo_format, w, h, w * bpp);
}

static ret_t cairo_on_bitmap_destroy(bitmap_t* img) {
  cairo_surface_t* surface = (cairo_surface_t*)img->specific;

  if (surface != NULL) {
    cairo_surface_destroy(surface);
  }

  img->specific = NULL;
  img->specific_ctx = NULL;
  img->specific_destroy = NULL;

  return RET_OK;
}

static cairo_surface_t* vgcanvas_cairo_ensure_image(bitmap_t* img) {
  cairo_surface_t* surface = (cairo_surface_t*)img->specific;

  if (surface == NULL) {
    surface = create_surface(img->w, img->h, img->format, (void*)(img->data));

    if (surface != NULL) {
      img->specific = surface;
      img->specific_ctx = NULL;
      img->specific_destroy = cairo_on_bitmap_destroy;
    }
  }

  return surface;
}

static ret_t vgcanvas_cairo_draw_image(vgcanvas_t* vgcanvas, bitmap_t* img, float_t sx, float_t sy,
                                       float_t sw, float_t sh, float_t dx, float_t dy, float_t dw,
                                       float_t dh) {
  int iw = img->w;
  int ih = img->h;
  float fx = (float)dw / sw;
  float fy = (float)dh / sh;
  float_t global_alpha = 1;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  cairo_surface_t* surface = vgcanvas_cairo_ensure_image(img);

  cairo_save(vg);

  if (dw != sw || dh != sh) {
    cairo_scale(vg, fx, fy);
    dx /= fx;
    dy /= fy;
    dw /= fx;
    dh /= fy;
  }

  cairo_rectangle(vg, dx, dy, dw, dh);
  cairo_clip(vg);

  cairo_set_source_surface(vg, surface, dx - sx, dy - sy);
  cairo_pattern_set_filter(cairo_get_source(vg), CAIRO_FILTER_BEST);
  cairo_pattern_set_extend(cairo_get_source(vg), CAIRO_EXTEND_REFLECT);
  cairo_paint_with_alpha(vg, global_alpha);

  cairo_restore(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_antialias(vgcanvas_t* vgcanvas, bool_t value) {
  (void)vgcanvas;
  (void)value;
  /*always*/

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_line_width(vgcanvas_t* vgcanvas, float_t value) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_set_line_width(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_global_alpha(vgcanvas_t* vgcanvas, float_t value) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_paint_with_alpha(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_fill_color(vgcanvas_t* vgcanvas, color_t c) {
  (void)vgcanvas;
  (void)c;
  return RET_OK;
}

static ret_t vgcanvas_cairo_set_stroke_color(vgcanvas_t* vgcanvas, color_t c) {
  (void)vgcanvas;
  (void)c;
  return RET_OK;
}

static ret_t vgcanvas_cairo_set_line_cap(vgcanvas_t* vgcanvas, const char* value) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  if (*value == 'r') {
    cairo_set_line_cap(vg, CAIRO_LINE_CAP_ROUND);
  } else if (*value == 's') {
    cairo_set_line_cap(vg, CAIRO_LINE_CAP_SQUARE);
  } else {
    cairo_set_line_cap(vg, CAIRO_LINE_CAP_BUTT);
  }

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_line_join(vgcanvas_t* vgcanvas, const char* value) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  if (*value == 'r') {
    cairo_set_line_join(vg, CAIRO_LINE_JOIN_ROUND);
  } else if (*value == 'b') {
    cairo_set_line_join(vg, CAIRO_LINE_JOIN_BEVEL);
  } else {
    cairo_set_line_join(vg, CAIRO_LINE_JOIN_MITER);
  }

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_miter_limit(vgcanvas_t* vgcanvas, float_t value) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_set_miter_limit(vg, value);

  return RET_OK;
}

static ret_t vgcanvas_cairo_save(vgcanvas_t* vgcanvas) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_save(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_restore(vgcanvas_t* vgcanvas) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_restore(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_create_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  (void)vgcanvas;
  (void)fbo;
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_cairo_destroy_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  (void)vgcanvas;
  (void)fbo;
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_cairo_bind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  (void)vgcanvas;
  (void)fbo;
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_cairo_unbind_fbo(vgcanvas_t* vgcanvas, framebuffer_object_t* fbo) {
  (void)vgcanvas;
  (void)fbo;
  return RET_NOT_IMPL;
}

static ret_t vgcanvas_cairo_destroy(vgcanvas_t* vgcanvas) {
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (canvas->vg != NULL) {
    cairo_destroy(canvas->vg);
  }

  if (canvas->surface != NULL) {
    cairo_surface_destroy(canvas->surface);
  }

  return RET_OK;
}

static const vgcanvas_vtable_t vt = {vgcanvas_cairo_begin_frame,
                                     vgcanvas_cairo_reset,
                                     vgcanvas_cairo_flush,
                                     vgcanvas_cairo_clear_rect,
                                     vgcanvas_cairo_begin_path,
                                     vgcanvas_cairo_move_to,
                                     vgcanvas_cairo_line_to,
                                     vgcanvas_cairo_arc,
                                     vgcanvas_cairo_arc_to,
                                     vgcanvas_cairo_bezier_to,
                                     vgcanvas_cairo_quad_to,
                                     vgcanvas_cairo_is_point_in_path,
                                     vgcanvas_cairo_ellipse,
                                     vgcanvas_cairo_rounded_rect,
                                     vgcanvas_cairo_close_path,
                                     vgcanvas_cairo_scale,
                                     vgcanvas_cairo_rotate,
                                     vgcanvas_cairo_translate,
                                     vgcanvas_cairo_transform,
                                     vgcanvas_cairo_set_transform,
                                     vgcanvas_cairo_fill,
                                     vgcanvas_cairo_clip_rect,
                                     vgcanvas_cairo_stroke,
                                     vgcanvas_cairo_set_font,
                                     vgcanvas_cairo_set_font_size,
                                     vgcanvas_cairo_set_text_align,
                                     vgcanvas_cairo_set_text_baseline,
                                     vgcanvas_cairo_fill_text,
                                     vgcanvas_cairo_measure_text,
                                     vgcanvas_cairo_draw_image,
                                     vgcanvas_cairo_set_antialias,
                                     vgcanvas_cairo_set_global_alpha,
                                     vgcanvas_cairo_set_line_width,
                                     vgcanvas_cairo_set_fill_color,
                                     vgcanvas_cairo_set_stroke_color,
                                     vgcanvas_cairo_set_line_join,
                                     vgcanvas_cairo_set_line_cap,
                                     vgcanvas_cairo_set_miter_limit,
                                     vgcanvas_cairo_save,
                                     vgcanvas_cairo_restore,
                                     vgcanvas_cairo_end_frame,
                                     vgcanvas_cairo_create_fbo,
                                     vgcanvas_cairo_destroy_fbo,
                                     vgcanvas_cairo_bind_fbo,
                                     vgcanvas_cairo_unbind_fbo,
                                     vgcanvas_cairo_destroy};

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, bitmap_format_t format, void* fbuff) {
  int32_t bpp = 1;
  cairo_t* cr = NULL;
  cairo_surface_t* surface = NULL;
  vgcanvas_cairo_t* cairo = (vgcanvas_cairo_t*)TKMEM_ZALLOC(vgcanvas_cairo_t);
  return_value_if_fail(cairo != NULL, NULL);

  cairo->base.w = w;
  cairo->base.h = h;
  cairo->base.vt = &vt;
  cairo->base.ratio = 1;

  surface = create_surface(w, h, format, fbuff);
  return_value_if_fail(surface != NULL, NULL);

  cairo->vg = cairo_create(surface);
  return_value_if_fail(cairo->vg, NULL);

  return &(cairo->base);
}
