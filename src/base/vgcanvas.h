/**
 * File:   vgcanvas.h
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

#ifndef LFTK_VG_CANVAS_H
#define LFTK_VG_CANVAS_H

#include "base/bitmap.h"

BEGIN_C_DECLS

struct _vgcanvas_t;
typedef struct _vgcanvas_t vgcanvas_t;

typedef ret_t (*vgcanvas_reset_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_flush_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_clear_rect_t)(vgcanvas_t* vg, double x, double y, double w, double h,
                                       color_t c);

typedef ret_t (*vgcanvas_begin_path_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_move_to_t)(vgcanvas_t* vg, double x, double y);
typedef ret_t (*vgcanvas_line_to_t)(vgcanvas_t* vg, double x, double y);
typedef ret_t (*vgcanvas_quadratic_curve_to_t)(vgcanvas_t* vg, double cpx, double cpy, double x,
                                               double y);
typedef ret_t (*vgcanvas_bezier_curve_to_t)(vgcanvas_t* vg, double cp1x, double cp1y, double cp2x,
                                            double cp2y, double x, double y);
typedef ret_t (*vgcanvas_arc_to_t)(vgcanvas_t* vg, double x1, double y1, double x2, double y2,
                                   double r);
typedef ret_t (*vgcanvas_arc_t)(vgcanvas_t* vg, double x, double y, double r, double start_angle,
                                double end_angle, bool_t ccw);
typedef bool_t (*vgcanvas_is_point_in_path_t)(vgcanvas_t* vg, double x, double y);
typedef ret_t (*vgcanvas_round_rect_t)(vgcanvas_t* vg, double x, double y, double w, double h,
                                       double r);
typedef ret_t (*vgcanvas_ellipse_t)(vgcanvas_t* vg, double x, double y, double rx, double ry);
typedef ret_t (*vgcanvas_close_path_t)(vgcanvas_t* vg);

typedef ret_t (*vgcanvas_rotate_t)(vgcanvas_t* vg, double rad);
typedef ret_t (*vgcanvas_scale_t)(vgcanvas_t* vg, double x, double y);
typedef ret_t (*vgcanvas_translate_t)(vgcanvas_t* vg, double x, double y);
typedef ret_t (*vgcanvas_transform_t)(vgcanvas_t* vg, double a, double b, double c, double d,
                                      double e, double f);
typedef ret_t (*vgcanvas_set_transform_t)(vgcanvas_t* vg, double a, double b, double c, double d,
                                          double e, double f);

typedef ret_t (*vgcanvas_fill_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_clip_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_stroke_t)(vgcanvas_t* vg);

typedef ret_t (*vgcanvas_set_font_t)(vgcanvas_t* vg, const char* font);
typedef ret_t (*vgcanvas_set_text_align_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_set_text_baseline_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_fill_text_t)(vgcanvas_t* vg, const char* text, double x, double y,
                                      double max_width);
typedef ret_t (*vgcanvas_stroke_text_t)(vgcanvas_t* vg, const char* text, double x, double y,
                                        double max_width);
typedef ret_t (*vgcanvas_draw_image_t)(vgcanvas_t* vg, bitmap_t* img, double sx, double sy,
                                       double sw, double sh, double dx, double dy, double dw,
                                       double dh);

typedef ret_t (*vgcanvas_set_antialias_t)(vgcanvas_t* vg, bool_t value);
typedef ret_t (*vgcanvas_set_global_alpha_t)(vgcanvas_t* vg, double alpha);
typedef ret_t (*vgcanvas_set_line_width_t)(vgcanvas_t* vg, double value);
typedef ret_t (*vgcanvas_set_fill_color_t)(vgcanvas_t* vg, color_t c);
typedef ret_t (*vgcanvas_set_stroke_color_t)(vgcanvas_t* vg, color_t c);
typedef ret_t (*vgcanvas_set_line_cap_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_set_line_join_t)(vgcanvas_t* vg, const char* value);
typedef ret_t (*vgcanvas_set_miter_limit_t)(vgcanvas_t* vg, double value);

typedef ret_t (*vgcanvas_save_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_restore_t)(vgcanvas_t* vg);

typedef ret_t (*vgcanvas_destroy_t)(vgcanvas_t* vg);

typedef struct _vgcanvas_vtable_t {
  vgcanvas_reset_t reset;
  vgcanvas_flush_t flush;
  vgcanvas_clear_rect_t clear_rect;

  vgcanvas_begin_path_t begin_path;
  vgcanvas_move_to_t move_to;
  vgcanvas_line_to_t line_to;
  vgcanvas_arc_t arc;
  vgcanvas_arc_to_t arc_to;
  vgcanvas_bezier_curve_to_t bezier_curve_to;
  vgcanvas_quadratic_curve_to_t quadratic_curve_to;
  vgcanvas_is_point_in_path_t is_point_in_path;
  vgcanvas_ellipse_t ellipse;
  vgcanvas_round_rect_t round_rect;
  vgcanvas_close_path_t close_path;

  vgcanvas_scale_t scale;
  vgcanvas_rotate_t rotate;
  vgcanvas_translate_t translate;
  vgcanvas_transform_t transform;
  vgcanvas_set_transform_t set_transform;

  vgcanvas_fill_t fill;
  vgcanvas_clip_t clip;
  vgcanvas_stroke_t stroke;

  vgcanvas_set_font_t set_font;
  vgcanvas_set_text_align_t set_text_align;
  vgcanvas_set_text_baseline_t set_text_baseline;
  vgcanvas_fill_text_t fill_text;
  vgcanvas_stroke_text_t stroke_text;
  vgcanvas_draw_image_t draw_image;

  vgcanvas_set_antialias_t set_antialias;
  vgcanvas_set_global_alpha_t set_global_alpha;
  vgcanvas_set_line_width_t set_line_width;
  vgcanvas_set_fill_color_t set_fill_color;
  vgcanvas_set_stroke_color_t set_stroke_color;
  vgcanvas_set_line_join_t set_line_join;
  vgcanvas_set_line_cap_t set_line_cap;
  vgcanvas_set_miter_limit_t set_miter_limit;

  vgcanvas_save_t save;
  vgcanvas_restore_t restore;

  vgcanvas_destroy_t destroy;
} vgcanvas_vtable_t;

struct _vgcanvas_t {
  uint32_t w;
  uint32_t h;
  bool_t anti_alias;
  double line_width;
  double global_alpha;
  double miter_limit;
  const char* line_cap;
  const char* line_join;
  const char* font;
  const char* text_align;
  const char* text_baseline;
  color_t fill_color;
  color_t stroke_color;

  uint32_t* buff;

  const vgcanvas_vtable_t* vt;
};

/*大部分函数可以参考：https://www.w3schools.com/tags/ref_canvas.asp*/
vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t* buff);

ret_t vgcanvas_begin_path(vgcanvas_t* vg);
ret_t vgcanvas_move_to(vgcanvas_t* vg, double x, double y);
ret_t vgcanvas_line_to(vgcanvas_t* vg, double x, double y);
ret_t vgcanvas_quadratic_curve_to(vgcanvas_t* vg, double cpx, double cpy, double x, double y);
ret_t vgcanvas_bezier_curve_to(vgcanvas_t* vg, double cp1x, double cp1y, double cp2x, double cp2y,
                               double x, double y);
ret_t vgcanvas_arc_to(vgcanvas_t* vg, double x1, double y1, double x2, double y2, double r);
ret_t vgcanvas_arc(vgcanvas_t* vg, double x, double y, double r, double start_angle,
                   double end_angle, bool_t ccw);
bool_t vgcanvas_is_point_in_path(vgcanvas_t* vg, double x, double y);
ret_t vgcanvas_rect(vgcanvas_t* vg, double x, double y, double w, double h);
ret_t vgcanvas_round_rect(vgcanvas_t* vg, double x, double y, double w, double h, double r);
ret_t vgcanvas_ellipse(vgcanvas_t* vg, double x, double y, double rx, double ry);
ret_t vgcanvas_close_path(vgcanvas_t* vg);

ret_t vgcanvas_reset(vgcanvas_t* vg);
ret_t vgcanvas_flush(vgcanvas_t* vg);
ret_t vgcanvas_clear_rect(vgcanvas_t* vg, double x, double y, double w, double h, color_t c);

ret_t vgcanvas_rotate(vgcanvas_t* vg, double rad);
ret_t vgcanvas_scale(vgcanvas_t* vg, double x, double y);
ret_t vgcanvas_translate(vgcanvas_t* vg, double x, double y);
ret_t vgcanvas_transform(vgcanvas_t* vg, double a, double b, double c, double d, double e,
                         double f);
ret_t vgcanvas_set_transform(vgcanvas_t* vg, double a, double b, double c, double d, double e,
                             double f);

ret_t vgcanvas_fill(vgcanvas_t* vg);
ret_t vgcanvas_clip(vgcanvas_t* vg);
ret_t vgcanvas_stroke(vgcanvas_t* vg);

ret_t vgcanvas_set_font(vgcanvas_t* vg, const char* font);
/*left|center|right*/
ret_t vgcanvas_set_text_align(vgcanvas_t* vg, const char* value);
/*top|middle|bottom*/
ret_t vgcanvas_set_text_baseline(vgcanvas_t* vg, const char* value);
ret_t vgcanvas_fill_text(vgcanvas_t* vg, const char* text, double x, double y, double max_width);
ret_t vgcanvas_stroke_text(vgcanvas_t* vg, const char* text, double x, double y, double max_width);
ret_t vgcanvas_draw_image(vgcanvas_t* vg, bitmap_t* img, double sx, double sy, double sw, double sh,
                          double dx, double dy, double dw, double dh);
ret_t vgcanvas_set_antialias(vgcanvas_t* vg, bool_t value);
ret_t vgcanvas_set_global_alpha(vgcanvas_t* vg, double alpha);
ret_t vgcanvas_set_line_width(vgcanvas_t* vg, double value);
ret_t vgcanvas_set_fill_color(vgcanvas_t* vg, color_t c);
ret_t vgcanvas_set_stroke_color(vgcanvas_t* vg, color_t c);
/*butt|round|square*/
ret_t vgcanvas_set_line_cap(vgcanvas_t* vg, const char* value);
/*bevel|round|miter*/
ret_t vgcanvas_set_line_join(vgcanvas_t* vg, const char* value);
ret_t vgcanvas_set_miter_limit(vgcanvas_t* vg, double value);

ret_t vgcanvas_save(vgcanvas_t* vg);
ret_t vgcanvas_restore(vgcanvas_t* vg);

ret_t vgcanvas_destroy(vgcanvas_t* vg);

END_C_DECLS

#endif /*LFTK_VG_CANVAS_H*/
