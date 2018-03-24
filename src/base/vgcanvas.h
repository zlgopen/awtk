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

#include "base/types_def.h"

BEGIN_C_DECLS

struct _vgcanvas_t;
typedef struct _vgcanvas_t vgcanvas_t;

typedef bool_t (*vgcanvas_get_antialias_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_set_antialias_t)(vgcanvas_t* vg, bool_t value);

typedef double (*vgcanvas_get_line_width_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_set_line_width_t)(vgcanvas_t* vg, double value);

typedef ret_t (*vgcanvas_move_to_t)(vgcanvas_t* vg, double x, double y);
typedef ret_t (*vgcanvas_line_to_t)(vgcanvas_t* vg, double x, double y);
typedef ret_t (*vgcanvas_reset_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_clear_t)(vgcanvas_t* vg, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

typedef ret_t (*vgcanvas_rotate_t)(vgcanvas_t* vg, double rad);
typedef ret_t (*vgcanvas_scale_t)(vgcanvas_t* vg, double x, double y);
typedef ret_t (*vgcanvas_translate_t)(vgcanvas_t* vg, double x, double y);

typedef ret_t (*vgcanvas_set_color_t)(vgcanvas_t* vg, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
typedef ret_t (*vgcanvas_fill_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_stroke_t)(vgcanvas_t* vg);
typedef ret_t (*vgcanvas_destroy_t)(vgcanvas_t* vg);

typedef struct _vgcanvas_vtable_t {
  vgcanvas_get_antialias_t get_antialias;
  vgcanvas_set_antialias_t set_antialias;
  vgcanvas_get_line_width_t get_line_width;
  vgcanvas_set_line_width_t set_line_width;
  vgcanvas_set_color_t set_color;

  vgcanvas_reset_t reset;
  vgcanvas_clear_t clear;

  vgcanvas_move_to_t move_to;
  vgcanvas_line_to_t line_to;

  vgcanvas_scale_t scale;
  vgcanvas_rotate_t rotate;
  vgcanvas_translate_t translate;

  vgcanvas_fill_t fill;
  vgcanvas_stroke_t stroke;

  vgcanvas_destroy_t destroy;
} vgcanvas_vtable_t;

struct _vgcanvas_t {
  uint32_t w;
  uint32_t h;
  uint32_t* buff;

  const vgcanvas_vtable_t* vt;
};

vgcanvas_t* vgcanvas_create(uint32_t w, uint32_t h, uint32_t* buff);

bool_t vgcanvas_get_antialias(vgcanvas_t* vg);
ret_t vgcanvas_set_antialias(vgcanvas_t* vg, bool_t value);

double vgcanvas_get_line_width(vgcanvas_t* vg);
ret_t vgcanvas_set_line_width(vgcanvas_t* vg, double value);

ret_t vgcanvas_move_to(vgcanvas_t* vg, double x, double y);
ret_t vgcanvas_line_to(vgcanvas_t* vg, double x, double y);
ret_t vgcanvas_reset(vgcanvas_t* vg);
ret_t vgcanvas_clear(vgcanvas_t* vg, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

ret_t vgcanvas_rotate(vgcanvas_t* vg, double rad);
ret_t vgcanvas_scale(vgcanvas_t* vg, double x, double y);
ret_t vgcanvas_translate(vgcanvas_t* vg, double x, double y);

ret_t vgcanvas_set_color(vgcanvas_t* vg, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
ret_t vgcanvas_fill(vgcanvas_t* vg);
ret_t vgcanvas_stroke(vgcanvas_t* vg);
ret_t vgcanvas_destroy(vgcanvas_t* vg);

/*
void draw_close_polygon(drawing*);
void draw_set_miter_limit(drawing*, double);
double draw_get_miter_limit(drawing*);
void draw_set_line_join(drawing*, line_join_style);
line_join_style draw_get_line_join(drawing*);
void draw_set_line_cap(drawing*, line_cap_style);
line_cap_style draw_get_line_cap(drawing*);
void draw_set_active_path(drawing*, unsigned int);
unsigned int draw_get_active_path(drawing*);
unsigned int draw_new_path(drawing*);
void draw_skew(drawing*, double, double);
void draw_clear_transforms(drawing*);
void draw_end_polygon(drawing*);
void draw_invert_polygon(drawing*);
void draw_remove_all(drawing*);
void draw_ellipse(drawing*, double, double, double, double);
void draw_rect(drawing*, double, double, double, double, double);
void draw_clip(drawing*, double, double, double, double);
void draw_reset_clip(drawing*);
double draw_last_x(drawing*);
double draw_last_y(drawing*);
void draw_rel_to_abs(drawing*, double *, double *);
void draw_move_rel(drawing*, double, double);
void draw_line_rel(drawing*, double, double);
void draw_hline_to(drawing*, double a);
void draw_hline_rel(drawing*, double a);
void draw_vline_to(drawing*, double a);
void draw_vline_rel(drawing*, double a);
void draw_curve_to2(drawing*, double, double);
void draw_curve_rel2(drawing*, double, double);
void draw_curve_to4(drawing*, double, double, double, double);
void draw_curve_rel4(drawing*, double, double, double, double);
void draw_curve_to6(drawing*, double, double, double, double, double, double);
void draw_curve_rel6(drawing*, double, double, double, double, double, double);
void draw_arc_to(drawing*, double, double, double, double, double);
void draw_arc_rel(drawing*, double, double, double, double, double);
double draw_text_simple(drawing*, double, double, const char *, int, double, const char *);
double draw_text(drawing*, double, double, const char *, const char *, double, double);
void draw_fill_color(drawing*, float*);
void draw_stroke_color(drawing*, float*);
void draw_dash(drawing*, double, double);
void draw_dash_color(drawing*, float*, double, double);
void draw_paint(drawing*);
void draw_auto_close(drawing*, bool);
bool draw_in_path(drawing*, double, double);
bool draw_is_drawn(drawing* , double x, double y);
unsigned int draw_get_vertex(drawing*, unsigned int, double *, double *);
unsigned int draw_next_vertex(drawing*, double *, double *);
unsigned int draw_get_command(drawing*, unsigned int);
unsigned int draw_last_vertex(drawing*, double *, double *);
unsigned int draw_prev_vertex(drawing*, double *, double *);
void draw_modify_vertex(drawing*, unsigned int, double, double, unsigned int);
unsigned int draw_total_vertices(drawing*);

void draw_alpha_mask_init(drawing* a);
void draw_alpha_mask_free(drawing* a);
void draw_alpha_mask_fill(drawing* a, uint8_t v);
void draw_alpha_mask_set(drawing* a, int32_t x, int32_t y, uint8_t val);
uint8_t draw_alpha_mask_get(drawing* a, int32_t x, int32_t y);
uint8_t *draw_alpha_mask_ptr(drawing* a);
uint8_t *draw_alpha_mask_ptr_offs(drawing* a, int32_t x, int32_t y);

void draw_join(drawing*, drawing*);
void draw_concat(drawing*, drawing*);

typedef struct transform_matrix {
    void *handle;
    int free_handle;
} transform_matrix;

typedef struct gradient {
    void *handle;
} gradient;

void draw_transform_matrix_scale(transform_matrix* mtx, double a, double b);
void draw_transform_matrix_translate(transform_matrix* mtx, double a, double b);
void draw_transform_matrix_rotate(transform_matrix* mtx, double a);
void draw_transform_matrix_reset(transform_matrix* mtx);
transform_matrix* draw_get_transform_matrix(drawing* d);
transform_matrix* draw_transform_matrix_create();
void draw_transform_matrix_free(transform_matrix* mtx);
double draw_transform_matrix_determinant(transform_matrix* mtx);
void draw_transform_matrix_inverse_transform(transform_matrix* mtx, double *x, double *y);
void draw_transform_matrix_transform(transform_matrix* mtx, double *x, double *y);
void draw_transform_matrix_to_double(transform_matrix* mtx, double *d);
void draw_transform_matrix_from_double(transform_matrix* mtx, double *d);

gradient* draw_gradient_create();
void draw_gradient_free(gradient *grad);
void draw_gradient_add_stop(gradient* grad, float *color);
transform_matrix draw_gradient_get_matrix(gradient* grad);
void draw_gradient_set_matrix(gradient* grad, transform_matrix* mtx);
void draw_fill_gradient(drawing*, gradient* grad, int s, int x, gradient_type grad_type);
void draw_stroke_gradient(drawing*, gradient* grad, int s, int x, gradient_type grad_type);
gradient* draw_gradient_create16();
void draw_gradient_add_stop16(gradient* grad, float *color);
void draw_fill_gradient16(drawing*, gradient* grad, int s, int x, gradient_type grad_type);
void draw_stroke_gradient16(drawing*, gradient* grad, int s, int x, gradient_type grad_type);
*/

END_C_DECLS

#endif /*LFTK_VG_CANVAS_H*/
