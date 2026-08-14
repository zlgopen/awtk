/**
 * File:   vgcanvas_cairo.c
 * Author: AWTK Develop Team
 * Brief:  vector graphics canvas base on cairo
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/darray.h"
#include "cairo/cairo.h"
#include "base/vgcanvas.h"
#include "base/font.h"
#include "base/font_manager.h"
#include "base/system_info.h"
#include "base/image_manager.h"
#include "cairo/cairo-private.h"

typedef enum _cairo_source_type_t {
  CAIRO_SOURCE_NONE = 0,
  CAIRO_SOURCE_COLOR,
  CAIRO_SOURCE_IMAGE,
  CAIRO_SOURCE_GRADIENT,
} cairo_source_type_t;

typedef struct _vgcanvas_cairo_t {
  vgcanvas_t base;

  cairo_t* vg;
  cairo_pattern_t* stroke_gradient;
  cairo_pattern_t* fill_gradient;

  cairo_source_type_t stroke_source_type;
  cairo_source_type_t fill_source_type;
  uint8_t text_align_h;
  uint8_t text_align_v;
  font_t* awtk_font;
  font_t* awtk_fallback_font;
  font_size_t awtk_font_size;

  darray_t images;
} vgcanvas_cairo_t;

typedef struct _vgcanvas_cairo_text_run_t {
  glyph_t glyph;
  int32_t pen_x;
} vgcanvas_cairo_text_run_t;

enum { CAIRO_TEXT_ALIGN_LEFT = 0, CAIRO_TEXT_ALIGN_CENTER = 1, CAIRO_TEXT_ALIGN_RIGHT = 2 };

enum {
  CAIRO_TEXT_BASELINE_TOP = 0,
  CAIRO_TEXT_BASELINE_MIDDLE = 1,
  CAIRO_TEXT_BASELINE_BOTTOM = 2
};

static font_size_t vgcanvas_cairo_text_to_font_size(float_t size) {
  int32_t font_size = tk_roundi(size);
  if (font_size <= 0) {
    font_size = TK_DEFAULT_FONT_SIZE;
  }
  return (font_size_t)font_size;
}

static ret_t vgcanvas_cairo_text_ensure_font(vgcanvas_cairo_t* canvas, const char* name_hint) {
  font_manager_t* fm = font_manager();
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);

  if (canvas->awtk_font_size == 0) {
    canvas->awtk_font_size = vgcanvas_cairo_text_to_font_size(canvas->base.font_size);
  }

  if (canvas->awtk_font == NULL) {
    canvas->awtk_font = font_manager_get_font(fm, name_hint, canvas->awtk_font_size);
    canvas->awtk_fallback_font = font_manager_get_font(fm, NULL, canvas->awtk_font_size);
  }

  return_value_if_fail(canvas->awtk_font != NULL || canvas->awtk_fallback_font != NULL, RET_FAIL);

  return RET_OK;
}

static ret_t vgcanvas_cairo_text_get_glyph(vgcanvas_cairo_t* canvas, wchar_t chr, glyph_t* glyph) {
  ret_t ret = RET_FAIL;
  font_t* font = canvas->awtk_font;

  if (font != NULL) {
    ret = font_get_glyph(font, chr, canvas->awtk_font_size, glyph);
    if (ret == RET_OK && glyph->data != NULL) {
      return RET_OK;
    }
  }

  if (canvas->awtk_fallback_font != NULL && canvas->awtk_fallback_font != font) {
    ret = font_get_glyph(canvas->awtk_fallback_font, chr, canvas->awtk_font_size, glyph);
    if (ret == RET_OK && glyph->data != NULL) {
      return RET_OK;
    }
  }

  return RET_FAIL;
}

static uint32_t vgcanvas_cairo_text_get_row_pitch(const glyph_t* glyph) {
  if (glyph->pitch > 0) {
    return glyph->pitch;
  }

  switch (glyph->format) {
    case GLYPH_FMT_ALPHA:
      return glyph->w;
    case GLYPH_FMT_MONO:
      return (glyph->w + 7) >> 3;
    case GLYPH_FMT_ALPHA2:
      return (glyph->w + 3) >> 2;
    case GLYPH_FMT_ALPHA4:
      return (glyph->w + 1) >> 1;
    case GLYPH_FMT_RGBA:
      return glyph->w * 4;
    default:
      return glyph->w;
  }
}

static uint8_t vgcanvas_cairo_text_alpha_from_glyph(const glyph_t* glyph, int32_t x, int32_t y) {
  uint32_t row_pitch = vgcanvas_cairo_text_get_row_pitch(glyph);
  const uint8_t* row = glyph->data + y * row_pitch;

  switch (glyph->format) {
    case GLYPH_FMT_ALPHA:
      return row[x];
    case GLYPH_FMT_MONO: {
      uint8_t v = row[x >> 3];
      uint8_t shift = 7 - (x & 0x7);
      return ((v >> shift) & 0x1) ? 0xFF : 0;
    }
    case GLYPH_FMT_ALPHA2: {
      uint8_t v = row[x >> 2];
      uint8_t shift = (uint8_t)((3 - (x & 0x3)) * 2);
      return (uint8_t)(((v >> shift) & 0x3) * 85);
    }
    case GLYPH_FMT_ALPHA4: {
      uint8_t v = row[x >> 1];
      uint8_t shift = (uint8_t)((1 - (x & 0x1)) * 4);
      return (uint8_t)(((v >> shift) & 0xF) * 17);
    }
    case GLYPH_FMT_RGBA:
      return row[x * 4 + 3];
    default:
      return 0;
  }
}

ret_t vgcanvas_cairo_set_sreen_orientation(cairo_t* canvas) {
#ifdef WITH_FAST_LCD_PORTRAIT
  float angle = 0.0f;
  float anchor_x = 0.0f;
  float anchor_y = 0.0f;
  system_info_t* info = system_info();
  lcd_orientation_t orientation = info->lcd_orientation;
  switch (orientation) {
    case LCD_ORIENTATION_0:
      angle = 0.0f;
      break;
    case LCD_ORIENTATION_90:
      angle = TK_D2R(270);
      break;
    case LCD_ORIENTATION_180:
      angle = TK_D2R(180);
      break;
    case LCD_ORIENTATION_270:
      angle = TK_D2R(90);
      break;
    default:
      break;
  }
  anchor_x = info->lcd_w / 2.0f;
  anchor_y = info->lcd_h / 2.0f;

  if (orientation == LCD_ORIENTATION_90 || orientation == LCD_ORIENTATION_270) {
    cairo_translate(canvas, anchor_x, anchor_y);
    cairo_rotate(canvas, angle);
    cairo_translate(canvas, -anchor_y, -anchor_x);
  } else if (orientation == LCD_ORIENTATION_180) {
    cairo_translate(canvas, anchor_x, anchor_y);
    cairo_rotate(canvas, angle);
    cairo_translate(canvas, -anchor_x, -anchor_y);
  }
#else
  /* only fast lcd portrait need rotate, because of this mode g2d flush mode will bear */
#endif
  return RET_OK;
}

ret_t vgcanvas_cairo_begin_frame(vgcanvas_t* vgcanvas, const dirty_rects_t* dirty_rects) {
  system_info_t* info = system_info();
  const rect_t* dirty_rect = dirty_rects != NULL ? &(dirty_rects->max) : NULL;
  const rect_t* r = dirty_rect;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;
  cairo_t* vg = canvas->vg;

  cairo_identity_matrix(vg);
  vg->status = CAIRO_STATUS_SUCCESS;

  if (r != NULL) {
    cairo_rectangle(vg, r->x, r->y, r->w, r->h);
  } else if (info->lcd_orientation == LCD_ORIENTATION_90 ||
             info->lcd_orientation == LCD_ORIENTATION_270) {
    cairo_rectangle(vg, 0, 0, info->lcd_h, info->lcd_w);
  } else {
    cairo_rectangle(vg, 0, 0, info->lcd_w, info->lcd_h);
  }

  cairo_save(vg);
  cairo_clip(vg);
  cairo_new_path(vg);
  vgcanvas->global_alpha = 1;
  vgcanvas_cairo_set_sreen_orientation(vg);

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
  cairo_identity_matrix(vg);
  vgcanvas->global_alpha = 1;
  vg->status = CAIRO_STATUS_SUCCESS;

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

  if (x == 0) {
    x = 0.00001f;
  }
  if (y == 0) {
    y = 0.00001f;
  }

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
  if (cairo_matrix_invert(&m) != CAIRO_STATUS_SUCCESS) {
    log_debug("invalid matrix: %f %f %f %f %f %f\n", a, b, c, d, e, f);
    return RET_FAIL;
  }

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

  cairo_matrix_t save_matrix;
  cairo_get_matrix(vg, &save_matrix);

  cairo_translate(vg, x, y);
  cairo_scale(vg, 1, ry / rx);
  cairo_translate(vg, -x, -y);
  cairo_new_path(vg);

  cairo_arc(
      /*vg =*/vg,
      /*xc =*/x,
      /*yc =*/y,
      /*radius =*/rx,
      /*angle1 =*/0,
      /*angle2 =*/2 * M_PI);

  cairo_set_matrix(vg, &save_matrix);

  return RET_OK;
}

static ret_t vgcanvas_cairo_close_path(vgcanvas_t* vgcanvas) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_close_path(vg);

  return RET_OK;
}

static ret_t cairo_set_source_color(cairo_t* vg, color_t c) {
  cairo_set_source_rgba(vg, c.rgba.r / 255.0f, c.rgba.g / 255.0f, c.rgba.b / 255.0f,
                        c.rgba.a / 255.0f);

  return RET_OK;
}

static ret_t vgcanvas_cairo_fill(vgcanvas_t* vgcanvas) {
  color_t c = vgcanvas->fill_color;
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (canvas->fill_source_type == CAIRO_SOURCE_GRADIENT) {
    /*not support global alpha yet*/
    cairo_set_source(vg, canvas->fill_gradient);
  } else {
    c.rgba.a = c.rgba.a * vgcanvas->global_alpha;
    cairo_set_source_color(vg, c);
  }

  cairo_fill_preserve(vg);

  return RET_OK;
}

static bool_t vgcanvas_cairo_is_rectf_in_clip_rect(vgcanvas_t* vgcanvas, float_t left, float_t top,
                                                   float_t right, float_t bottom) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  if (!cairo_in_clip(vg, left, top) && !cairo_in_clip(vg, right, bottom)) {
    return FALSE;
  }
  return TRUE;
}

const rectf_t* vgcanvas_cairo_get_clip_rect(vgcanvas_t* vgcanvas) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  cairo_rectangle_list_t* list = cairo_copy_clip_rectangle_list(vg);
  if (list->num_rectangles > 0) {
    vgcanvas->clip_rect = rectf_init(list->rectangles[0].x, list->rectangles[0].y,
                                     list->rectangles[0].width, list->rectangles[0].height);
  }
  cairo_rectangle_list_destroy(list);
  return &(vgcanvas->clip_rect);
}

static ret_t vgcanvas_cairo_clip_rect(vgcanvas_t* vgcanvas, float_t x, float_t y, float_t w,
                                      float_t h) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_reset_clip(vg);
  cairo_rectangle(vg, x, y, w, h);
  cairo_clip(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_fill_mode(vgcanvas_t* vgcanvas, vgcanvas_fill_mode_t fill_mode) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  switch (fill_mode) {
    case VGCANVAS_FILL_MODE_NON_ZERO:
      cairo_set_fill_rule(vg, CAIRO_FILL_RULE_WINDING);
      break;
    case VGCANVAS_FILL_MODE_EVEN_ODD:
      cairo_set_fill_rule(vg, CAIRO_FILL_RULE_EVEN_ODD);
      break;
    default:
      cairo_set_fill_rule(vg, CAIRO_FILL_RULE_WINDING);
      break;
  }
  return RET_OK;
}

static ret_t vgcanvas_cairo_clip_path(vgcanvas_t* vgcanvas) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;

  cairo_clip(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_stroke(vgcanvas_t* vgcanvas) {
  color_t c = vgcanvas->stroke_color;
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (canvas->stroke_source_type == CAIRO_SOURCE_GRADIENT) {
    /*not support global alpha yet*/
    cairo_set_source(vg, canvas->stroke_gradient);
  } else {
    c.rgba.a = c.rgba.a * vgcanvas->global_alpha;
    cairo_set_source_color(vg, c);
  }

  cairo_stroke_preserve(vg);

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_font_size(vgcanvas_t* vgcanvas, float_t size) {
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;
  canvas->awtk_font_size = vgcanvas_cairo_text_to_font_size(size);

  if (canvas->awtk_font != NULL || canvas->awtk_fallback_font != NULL) {
    font_manager_t* fm = font_manager();
    const char* name = vgcanvas->font;
    if (fm != NULL) {
      canvas->awtk_font = font_manager_get_font(fm, name, canvas->awtk_font_size);
      canvas->awtk_fallback_font = font_manager_get_font(fm, NULL, canvas->awtk_font_size);
    }
  }

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_font(vgcanvas_t* vgcanvas, const char* name) {
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;
  font_manager_t* fm = font_manager();
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(fm != NULL, RET_BAD_PARAMS);

  if (canvas->awtk_font_size == 0) {
    canvas->awtk_font_size = vgcanvas_cairo_text_to_font_size(vgcanvas->font_size);
  }

  canvas->awtk_font = font_manager_get_font(fm, name, canvas->awtk_font_size);
  canvas->awtk_fallback_font = font_manager_get_font(fm, NULL, canvas->awtk_font_size);

  return_value_if_fail(canvas->awtk_font != NULL || canvas->awtk_fallback_font != NULL, RET_FAIL);

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_text_align(vgcanvas_t* vgcanvas, const char* text_align) {
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (text_align[0] == 'r') {
    canvas->text_align_h = CAIRO_TEXT_ALIGN_RIGHT;
  } else if (text_align[0] == 'c') {
    canvas->text_align_h = CAIRO_TEXT_ALIGN_CENTER;
  } else {
    canvas->text_align_h = CAIRO_TEXT_ALIGN_LEFT;
  }

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_text_baseline(vgcanvas_t* vgcanvas, const char* text_baseline) {
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (text_baseline[0] == 'b') {
    canvas->text_align_v = CAIRO_TEXT_BASELINE_BOTTOM;
  } else if (text_baseline[0] == 'm') {
    canvas->text_align_v = CAIRO_TEXT_BASELINE_MIDDLE;
  } else {
    canvas->text_align_v = CAIRO_TEXT_BASELINE_TOP;
  }

  return RET_OK;
}

static ret_t vgcanvas_cairo_fill_text(vgcanvas_t* vgcanvas, const char* text, float_t x, float_t y,
                                      float_t max_width) {
  int32_t pen_x = 0;
  int32_t min_x = 0;
  int32_t min_y = 0;
  int32_t max_x = 0;
  int32_t max_y = 0;
  int32_t base_x = 0;
  int32_t base_y = 0;
  uint32_t text_len = 0;
  uint32_t i = 0;
  uint32_t run_nr = 0;
  uint8_t* mask_data = NULL;
  wchar_t* ws = NULL;
  vgcanvas_cairo_text_run_t* runs = NULL;
  color_t c = vgcanvas->fill_color;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;
  cairo_t* vg = canvas->vg;
  cairo_surface_t* mask_surface = NULL;
  int32_t mask_w = 0;
  int32_t mask_h = 0;
  int32_t mask_stride = 0;
  ret_t result = RET_OK;
  (void)max_width;
  ret_t ret = vgcanvas_cairo_text_ensure_font(canvas, vgcanvas->font);
  return_value_if_fail(ret == RET_OK && text != NULL, RET_BAD_PARAMS);
  if (*text == '\0') {
    return RET_OK;
  }

  text_len = (uint32_t)strlen(text);
  ws = (wchar_t*)TKMEM_ZALLOCN(wchar_t, text_len + 1);
  runs = (vgcanvas_cairo_text_run_t*)TKMEM_ZALLOCN(vgcanvas_cairo_text_run_t, text_len + 1);
  if (ws == NULL || runs == NULL) {
    result = RET_OOM;
    goto done;
  }

  tk_utf8_to_utf16(text, ws, text_len + 1);
  for (i = 0; ws[i] != 0; i++) {
    glyph_t glyph;
    memset(&glyph, 0x00, sizeof(glyph));
    if (vgcanvas_cairo_text_get_glyph(canvas, ws[i], &glyph) != RET_OK) {
      continue;
    }

    runs[run_nr].glyph = glyph;
    runs[run_nr].pen_x = pen_x;
    if (run_nr == 0) {
      min_x = pen_x + glyph.x;
      min_y = glyph.y;
      max_x = pen_x + glyph.x + glyph.w;
      max_y = glyph.y + glyph.h;
    } else {
      min_x = tk_min(min_x, pen_x + glyph.x);
      min_y = tk_min(min_y, (int32_t)glyph.y);
      max_x = tk_max(max_x, pen_x + glyph.x + glyph.w);
      max_y = tk_max(max_y, (int32_t)(glyph.y + glyph.h));
    }
    pen_x += glyph.advance;
    run_nr++;
  }

  if (run_nr == 0 || max_x <= min_x || max_y <= min_y) {
    TKMEM_FREE(ws);
    TKMEM_FREE(runs);
    return RET_OK;
  }

  base_x = tk_roundi(x);
  base_y = tk_roundi(y);
  if (canvas->text_align_h == CAIRO_TEXT_ALIGN_CENTER) {
    base_x -= (min_x + max_x) / 2;
  } else if (canvas->text_align_h == CAIRO_TEXT_ALIGN_RIGHT) {
    base_x -= max_x;
  } else {
    base_x -= min_x;
  }

  if (canvas->text_align_v == CAIRO_TEXT_BASELINE_MIDDLE) {
    base_y -= (min_y + max_y) / 2;
  } else if (canvas->text_align_v == CAIRO_TEXT_BASELINE_BOTTOM) {
    base_y -= max_y;
  } else {
    base_y -= min_y;
  }

  mask_w = max_x - min_x;
  mask_h = max_y - min_y;
  mask_stride = cairo_format_stride_for_width(CAIRO_FORMAT_A8, mask_w);
  if (mask_stride <= 0) {
    result = RET_FAIL;
    goto done;
  }
  mask_data = (uint8_t*)TKMEM_ZALLOCN(uint8_t, (uint32_t)mask_stride*(uint32_t)mask_h);
  if (mask_data == NULL) {
    result = RET_OOM;
    goto done;
  }

  for (i = 0; i < run_nr; i++) {
    const glyph_t* glyph = &(runs[i].glyph);
    int32_t glyph_x = runs[i].pen_x + glyph->x - min_x;
    int32_t glyph_y = glyph->y - min_y;
    int32_t gx = 0;
    int32_t gy = 0;

    if (glyph->data == NULL || glyph->w == 0 || glyph->h == 0) {
      continue;
    }

    for (gy = 0; gy < glyph->h; gy++) {
      int32_t dst_y = glyph_y + gy;
      if (dst_y < 0 || dst_y >= mask_h) {
        continue;
      }
      for (gx = 0; gx < glyph->w; gx++) {
        int32_t dst_x = glyph_x + gx;
        uint8_t value = 0;
        if (dst_x < 0 || dst_x >= mask_w) {
          continue;
        }
        value = vgcanvas_cairo_text_alpha_from_glyph(glyph, gx, gy);
        if (value > 0) {
          uint8_t* pixel = mask_data + dst_y * mask_stride + dst_x;
          if (value > *pixel) {
            *pixel = value;
          }
        }
      }
    }
  }

  mask_surface =
      cairo_image_surface_create_for_data(mask_data, CAIRO_FORMAT_A8, mask_w, mask_h, mask_stride);
  if (mask_surface == NULL || cairo_surface_status(mask_surface) != CAIRO_STATUS_SUCCESS) {
    result = RET_FAIL;
    goto done;
  }

  cairo_save(vg);
  if (canvas->fill_source_type == CAIRO_SOURCE_GRADIENT) {
    cairo_set_source(vg, canvas->fill_gradient);
  } else {
    c.rgba.a = c.rgba.a * vgcanvas->global_alpha;
    cairo_set_source_color(vg, c);
  }
  cairo_mask_surface(vg, mask_surface, base_x + min_x, base_y + min_y);
  cairo_restore(vg);

done:
  if (mask_surface != NULL) {
    cairo_surface_destroy(mask_surface);
  }
  TKMEM_FREE(mask_data);
  TKMEM_FREE(ws);
  TKMEM_FREE(runs);

  return result;
}

static float_t vgcanvas_cairo_measure_text(vgcanvas_t* vgcanvas, const char* text) {
  uint32_t i = 0;
  int32_t width = 0;
  uint32_t text_len = 0;
  wchar_t* ws = NULL;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;
  ret_t ret = vgcanvas_cairo_text_ensure_font(canvas, vgcanvas->font);
  return_value_if_fail(ret == RET_OK && text != NULL, 0);

  return_value_if_fail(text != NULL, 0);
  if (*text == '\0') {
    return 0;
  }

  text_len = (uint32_t)strlen(text);
  ws = (wchar_t*)TKMEM_ZALLOCN(wchar_t, text_len + 1);
  return_value_if_fail(ws != NULL, 0);

  tk_utf8_to_utf16(text, ws, text_len + 1);
  for (i = 0; ws[i] != 0; i++) {
    glyph_t glyph;
    memset(&glyph, 0x00, sizeof(glyph));
    if (vgcanvas_cairo_text_get_glyph(canvas, ws[i], &glyph) == RET_OK) {
      width += glyph.advance;
    }
  }
  TKMEM_FREE(ws);

  return (float_t)width;
}

static ret_t vgcanvas_cairo_get_text_metrics(vgcanvas_t* vgcanvas, float_t* ascent,
                                             float_t* descent, float_t* line_hight) {
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;
  font_vmetrics_t metrics;
  font_t* metric_font = NULL;
  ret_t ret = vgcanvas_cairo_text_ensure_font(canvas, vgcanvas->font);
  return_value_if_fail(ret == RET_OK, RET_FAIL);

  metric_font = canvas->awtk_font != NULL ? canvas->awtk_font : canvas->awtk_fallback_font;
  return_value_if_fail(metric_font != NULL, RET_FAIL);

  metrics = font_get_vmetrics(metric_font, canvas->awtk_font_size);
  if (ascent != NULL) {
    *ascent = (float_t)metrics.ascent;
  }
  if (descent != NULL) {
    *descent = (float_t)metrics.descent;
  }
  if (line_hight != NULL) {
    *line_hight = (float_t)(metrics.ascent - metrics.descent + metrics.line_gap);
  }

  return RET_OK;
}

static cairo_surface_t* create_surface(uint32_t w, uint32_t h, bitmap_format_t format,
                                       void* fbuff) {
  int32_t bpp = 0;
  cairo_format_t cairo_format = CAIRO_FORMAT_ARGB32;
  switch (format) {
    case BITMAP_FMT_RGBA8888: {
      bpp = 4;
      /*FIXME*/
      cairo_format = CAIRO_FORMAT_ARGB32;
      break;
    }
    case BITMAP_FMT_BGRA8888: {
      bpp = 4;
      cairo_format = CAIRO_FORMAT_ARGB32;
      break;
    }
    case BITMAP_FMT_BGR565: {
      bpp = 2;
      cairo_format = CAIRO_FORMAT_RGB16_565;
      break;
    }
    default: {
      return NULL;
    }
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

static cairo_surface_t* vgcanvas_cairo_ensure_image(vgcanvas_cairo_t* vg, bitmap_t* img) {
  cairo_surface_t* surface = NULL;
  darray_t* images = &(vg->images);
  bitmap_t* cairo_img = (bitmap_t*)darray_find(images, img);

  if (cairo_img == NULL) {
    cairo_img = bitmap_clone(img);
    return_value_if_fail(cairo_img != NULL, NULL);
    darray_push(images, cairo_img);
    bitmap_premulti_alpha(cairo_img);
  }

  surface = (cairo_surface_t*)(cairo_img->specific);
  if (surface == NULL) {
    uint8_t* cairo_img_data = bitmap_lock_buffer_for_read(cairo_img);
    surface = create_surface(cairo_img->w, cairo_img->h, cairo_img->format, cairo_img_data);
    bitmap_unlock_buffer(cairo_img);

    if (surface != NULL) {
      cairo_img->specific = surface;
      cairo_img->specific_ctx = NULL;
      cairo_img->specific_destroy = cairo_on_bitmap_destroy;
    }
  }

  return surface;
}

static ret_t vgcanvas_cairo_draw_image(vgcanvas_t* vgcanvas, bitmap_t* img, float_t sx, float_t sy,
                                       float_t sw, float_t sh, float_t dx, float_t dy, float_t dw,
                                       float_t dh) {
  float fx = (float)dw / sw;
  float fy = (float)dh / sh;
  float_t global_alpha = vgcanvas->global_alpha;
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  cairo_surface_t* surface = vgcanvas_cairo_ensure_image((vgcanvas_cairo_t*)vgcanvas, img);

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
  return RET_OK;
}

static ret_t vgcanvas_cairo_set_fill_color(vgcanvas_t* vgcanvas, color_t c) {
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  (void)c;
  canvas->fill_source_type = CAIRO_SOURCE_COLOR;

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_stroke_color(vgcanvas_t* vgcanvas, color_t c) {
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  (void)c;
  canvas->stroke_source_type = CAIRO_SOURCE_COLOR;

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

static ret_t vgcanvas_cairo_create_fbo(vgcanvas_t* vgcanvas, uint32_t w, uint32_t h,
                                       bool_t custom_draw_model, framebuffer_object_t* fbo) {
  (void)vgcanvas;
  (void)fbo;
  (void)w;
  (void)h;
  (void)custom_draw_model;
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

  if (canvas->stroke_gradient != NULL) {
    cairo_pattern_destroy(canvas->stroke_gradient);
  }

  if (canvas->fill_gradient != NULL) {
    cairo_pattern_destroy(canvas->fill_gradient);
  }

  if (canvas->vg != NULL) {
    cairo_destroy(canvas->vg);
    cairo_debug_reset_static_data();
  }

  darray_deinit(&(canvas->images));
  TKMEM_FREE(canvas);

  return RET_OK;
}

static ret_t cairo_pattern_add_color_stop_color(cairo_pattern_t* pat, float_t offset, color_t c) {
  return_value_if_fail(pat != NULL, RET_BAD_PARAMS);
  cairo_pattern_add_color_stop_rgba(pat, offset, c.rgba.r / 255.0f, c.rgba.g / 255.0f,
                                    c.rgba.b / 255.0f, c.rgba.a / 255.0f);

  return RET_OK;
}

static cairo_pattern_t* vgcanvas_cairo_create_pattern_from_gradient(const vg_gradient_t* gradient) {
  uint32_t i = 0;
  cairo_pattern_t* pattern = NULL;

  if (gradient->gradient.type == GRADIENT_LINEAR) {
    const vg_gradient_linear_info_t* info = &(gradient->info.linear);
    pattern = cairo_pattern_create_linear(info->sx, info->sy, info->ex, info->ey);
  } else if (gradient->gradient.type == GRADIENT_RADIAL) {
    const vg_gradient_radial_info_t* info = &(gradient->info.radial);
    pattern =
        cairo_pattern_create_radial(info->x0, info->y0, info->r0, info->x1, info->y1, info->r1);
  }
  return_value_if_fail(pattern != NULL, NULL);

  for (i = 0; i < gradient->gradient.nr; i++) {
    const gradient_stop_t* iter = vg_gradient_get_stop(gradient, i);
    cairo_pattern_add_color_stop_color(pattern, iter->offset, iter->color);
  }

  return pattern;
}

static ret_t vgcanvas_cairo_set_stroke_gradient(vgcanvas_t* vgcanvas,
                                                const vg_gradient_t* gradient) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (canvas->stroke_gradient != NULL) {
    cairo_pattern_destroy(canvas->stroke_gradient);
    canvas->stroke_gradient = NULL;
  }

  canvas->stroke_gradient = vgcanvas_cairo_create_pattern_from_gradient(gradient);
  return_value_if_fail(canvas->stroke_gradient != NULL, RET_FAIL);

  cairo_set_source(vg, canvas->stroke_gradient);
  canvas->stroke_source_type = CAIRO_SOURCE_GRADIENT;

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_fill_gradient(vgcanvas_t* vgcanvas, const vg_gradient_t* gradient) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (canvas->fill_gradient != NULL) {
    cairo_pattern_destroy(canvas->fill_gradient);
    canvas->fill_gradient = NULL;
  }

  canvas->fill_gradient = vgcanvas_cairo_create_pattern_from_gradient(gradient);
  return_value_if_fail(canvas->fill_gradient != NULL, RET_FAIL);

  cairo_set_source(vg, canvas->fill_gradient);
  canvas->fill_source_type = CAIRO_SOURCE_GRADIENT;

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_stroke_linear_gradient(vgcanvas_t* vgcanvas, float_t sx, float_t sy,
                                                       float_t ex, float_t ey, color_t icolor,
                                                       color_t ocolor) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (canvas->stroke_gradient != NULL) {
    cairo_pattern_destroy(canvas->stroke_gradient);
    canvas->stroke_gradient = NULL;
  }

  canvas->stroke_gradient = cairo_pattern_create_linear(sx, sy, ex, ey);
  cairo_pattern_add_color_stop_color(canvas->stroke_gradient, 0, icolor);
  cairo_pattern_add_color_stop_color(canvas->stroke_gradient, 1, ocolor);

  cairo_set_source(vg, canvas->stroke_gradient);
  canvas->stroke_source_type = CAIRO_SOURCE_GRADIENT;

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_stroke_radial_gradient(vgcanvas_t* vgcanvas, float_t cx, float_t cy,
                                                       float_t inr, float_t outr, color_t icolor,
                                                       color_t ocolor) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (canvas->stroke_gradient != NULL) {
    cairo_pattern_destroy(canvas->stroke_gradient);
    canvas->stroke_gradient = NULL;
  }

  canvas->stroke_gradient = cairo_pattern_create_radial(cx, cy, inr, cx, cy, outr);
  cairo_pattern_add_color_stop_color(canvas->stroke_gradient, 0, icolor);
  cairo_pattern_add_color_stop_color(canvas->stroke_gradient, 1, ocolor);

  cairo_set_source(vg, canvas->stroke_gradient);
  canvas->stroke_source_type = CAIRO_SOURCE_GRADIENT;

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_fill_radial_gradient(vgcanvas_t* vgcanvas, float_t cx, float_t cy,
                                                     float_t inr, float_t outr, color_t icolor,
                                                     color_t ocolor) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (canvas->fill_gradient != NULL) {
    cairo_pattern_destroy(canvas->fill_gradient);
    canvas->fill_gradient = NULL;
  }

  canvas->fill_gradient = cairo_pattern_create_radial(cx, cy, inr, cx, cy, outr);
  cairo_pattern_add_color_stop_color(canvas->fill_gradient, 0, icolor);
  cairo_pattern_add_color_stop_color(canvas->fill_gradient, 1, ocolor);

  cairo_set_source(vg, canvas->fill_gradient);
  canvas->fill_source_type = CAIRO_SOURCE_GRADIENT;

  return RET_OK;
}

static ret_t vgcanvas_cairo_set_fill_linear_gradient(vgcanvas_t* vgcanvas, float_t sx, float_t sy,
                                                     float_t ex, float_t ey, color_t icolor,
                                                     color_t ocolor) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  if (canvas->fill_gradient != NULL) {
    cairo_pattern_destroy(canvas->fill_gradient);
    canvas->fill_gradient = NULL;
  }

  canvas->fill_gradient = cairo_pattern_create_linear(sx, sy, ex, ey);
  cairo_pattern_add_color_stop_color(canvas->fill_gradient, 0, icolor);
  cairo_pattern_add_color_stop_color(canvas->fill_gradient, 1, ocolor);

  cairo_set_source(vg, canvas->fill_gradient);
  canvas->fill_source_type = CAIRO_SOURCE_GRADIENT;

  return RET_OK;
}

static ret_t vgcanvas_cairo_reinit(vgcanvas_t* vgcanvas, uint32_t w, uint32_t h, uint32_t stride,
                                   bitmap_format_t format, void* data) {
  cairo_surface_t* surface = NULL;
  vgcanvas_cairo_t* canvas = (vgcanvas_cairo_t*)vgcanvas;

  vgcanvas->w = w;
  vgcanvas->h = h;
  vgcanvas->ratio = 1;
  vgcanvas->stride = stride;
  vgcanvas->format = format;
  vgcanvas->buff = (uint32_t*)data;

  cairo_destroy(canvas->vg);
  surface = create_surface(w, h, format, vgcanvas->buff);
  if (surface != NULL) {
    canvas->vg = cairo_create(surface);
  }
  cairo_surface_destroy(surface);
  return_value_if_fail(canvas->vg, RET_OOM);
  log_debug("resize to w=%u h=%u format=%d\n", w, h, format);

  return RET_OK;
}

static ret_t vgcanvas_cairo_paint(vgcanvas_t* vgcanvas, bool_t stroke, bitmap_t* img) {
  cairo_t* vg = ((vgcanvas_cairo_t*)vgcanvas)->vg;
  cairo_surface_t* surface = vgcanvas_cairo_ensure_image((vgcanvas_cairo_t*)vgcanvas, img);

  cairo_set_source_surface(vg, surface, 0, 0);
  cairo_pattern_set_filter(cairo_get_source(vg), CAIRO_FILTER_BEST);

  if (stroke) {
    cairo_stroke_preserve(vg);
  } else {
    cairo_fill_preserve(vg);
  }

  return RET_OK;
}

static const vgcanvas_vtable_t vt = {
    .reinit = vgcanvas_cairo_reinit,
    .begin_frame = vgcanvas_cairo_begin_frame,
    .reset = vgcanvas_cairo_reset,
    .flush = vgcanvas_cairo_flush,
    .clear_rect = vgcanvas_cairo_clear_rect,
    .begin_path = vgcanvas_cairo_begin_path,
    .move_to = vgcanvas_cairo_move_to,
    .line_to = vgcanvas_cairo_line_to,
    .arc = vgcanvas_cairo_arc,
    .arc_to = vgcanvas_cairo_arc_to,
    .bezier_to = vgcanvas_cairo_bezier_to,
    .quad_to = vgcanvas_cairo_quad_to,
    .is_point_in_path = vgcanvas_cairo_is_point_in_path,
    .ellipse = vgcanvas_cairo_ellipse,
    .rounded_rect = vgcanvas_cairo_rounded_rect,
    .close_path = vgcanvas_cairo_close_path,
    .scale = vgcanvas_cairo_scale,
    .rotate = vgcanvas_cairo_rotate,
    .translate = vgcanvas_cairo_translate,
    .transform = vgcanvas_cairo_transform,
    .set_transform = vgcanvas_cairo_set_transform,
    .clip_path = vgcanvas_cairo_clip_path,
    .clip_rect = vgcanvas_cairo_clip_rect,
    .fill = vgcanvas_cairo_fill,
    .stroke = vgcanvas_cairo_stroke,
    .paint = vgcanvas_cairo_paint,
    .set_font = vgcanvas_cairo_set_font,
    .set_font_size = vgcanvas_cairo_set_font_size,
    .set_text_align = vgcanvas_cairo_set_text_align,
    .set_text_baseline = vgcanvas_cairo_set_text_baseline,
    .fill_text = vgcanvas_cairo_fill_text,
    .measure_text = vgcanvas_cairo_measure_text,
    .get_text_metrics = vgcanvas_cairo_get_text_metrics,
    .draw_image = vgcanvas_cairo_draw_image,
    .set_antialias = vgcanvas_cairo_set_antialias,
    .set_global_alpha = vgcanvas_cairo_set_global_alpha,
    .set_line_width = vgcanvas_cairo_set_line_width,
    .set_fill_color = vgcanvas_cairo_set_fill_color,
    .set_fill_linear_gradient = vgcanvas_cairo_set_fill_linear_gradient,
    .set_fill_radial_gradient = vgcanvas_cairo_set_fill_radial_gradient,
    .set_stroke_color = vgcanvas_cairo_set_stroke_color,
    .set_stroke_linear_gradient = vgcanvas_cairo_set_stroke_linear_gradient,
    .set_stroke_radial_gradient = vgcanvas_cairo_set_stroke_radial_gradient,
    .set_stroke_gradient = vgcanvas_cairo_set_stroke_gradient,
    .set_fill_gradient = vgcanvas_cairo_set_fill_gradient,
    .is_rectf_in_clip_rect = vgcanvas_cairo_is_rectf_in_clip_rect,
    .get_clip_rect = vgcanvas_cairo_get_clip_rect,
    .set_fill_mode = vgcanvas_cairo_set_fill_mode,

    .set_line_join = vgcanvas_cairo_set_line_join,
    .set_line_cap = vgcanvas_cairo_set_line_cap,
    .set_miter_limit = vgcanvas_cairo_set_miter_limit,
    .save = vgcanvas_cairo_save,
    .restore = vgcanvas_cairo_restore,
    .end_frame = vgcanvas_cairo_end_frame,
    .create_fbo = vgcanvas_cairo_create_fbo,
    .bind_fbo = vgcanvas_cairo_bind_fbo,
    .destroy_fbo = vgcanvas_cairo_destroy_fbo,
    .unbind_fbo = vgcanvas_cairo_unbind_fbo,
    .destroy = vgcanvas_cairo_destroy};

static int cairo_bitmap_cmp(const bitmap_t* a, bitmap_t* b) {
  return_value_if_fail(a != NULL && a->name != NULL && b != NULL && b->name != NULL, -1);

  if (tk_str_eq(a->name, b->name)) {
    return 0;
  } else {
    return 1;
  }
}

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t stride, bitmap_format_t format,
                            void* data) {
  cairo_surface_t* surface = NULL;
  vgcanvas_cairo_t* cairo = (vgcanvas_cairo_t*)TKMEM_ZALLOC(vgcanvas_cairo_t);
  return_value_if_fail(cairo != NULL, NULL);

  cairo->base.w = w;
  cairo->base.h = h;
  cairo->base.vt = &vt;
  cairo->base.ratio = 1;
  cairo->base.stride = stride;
  cairo->base.format = format;
  cairo->base.buff = (uint32_t*)data;

  surface = create_surface(w, h, format, data);
  return_value_if_fail(surface != NULL, NULL);

  cairo->vg = cairo_create(surface);
  return_value_if_fail(cairo->vg, NULL);
  cairo->text_align_h = CAIRO_TEXT_ALIGN_LEFT;
  cairo->text_align_v = CAIRO_TEXT_BASELINE_TOP;
  darray_init(&(cairo->images), 10, (tk_destroy_t)bitmap_destroy, (tk_compare_t)cairo_bitmap_cmp);
  vgcanvas_set_global_alpha((vgcanvas_t*)cairo, 1);
  cairo_surface_destroy(surface);

  log_debug("vgcanvas_cairo created\n");
  return &(cairo->base);
}
