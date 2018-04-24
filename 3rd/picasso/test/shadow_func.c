#include "stdio.h"

#include "../include/picasso.h"
#include "drawFunc.h"

static ps_matrix* pm;
static ps_path * pa;
static ps_image * pi;
static ps_pattern * pt;
static ps_canvas * cx;
void draw_test (int id, ps_context* gc)
{
    ps_rect cr = {2.3f, 4.7f, 187.7f, 161.5f};
    ps_rect cr2 = {20, 20, 120, 80};
    ps_color col = {1, 0, 0, 1};
    ps_color sol = {0, 0, 0, 1};
    ps_color sol1 = {1, 0, 0, 1};
    ps_color sol2 = {0, 0, 0, 1};

    ps_identity(gc);
    ps_ellipse(gc, &cr2);
    ps_clip(gc);

    ps_set_shadow(gc, 2, 2, 0.375f);
    ps_set_shadow_color(gc, &sol);
    ps_set_source_color(gc, &col);
    ps_rectangle(gc, &cr2);
    ps_fill(gc);
    ps_reset_clip(gc);
    ps_reset_shadow(gc);

    ps_identity(gc);

    ps_translate(gc, 150.3f, 205.7f);
    ps_rotate(gc, -0.42f);
    ps_set_shadow(gc, 10, 10, 0.375f);
    ps_set_shadow_color(gc, &sol1);
    ps_set_source_pattern(gc, pt);
    ps_rectangle(gc, &cr);
    ps_fill(gc);
    ps_reset_shadow(gc);

    ps_identity(gc);

    ps_translate(gc, 220.3f, 65.7f);
    ps_rotate(gc, 0.36f);
    ps_scale(gc, 0.5f, 0.5f);
    ps_set_shadow(gc, 0, 0, 0.375f);
    ps_set_shadow_color(gc, &sol2);
    ps_set_source_image(gc, pi);
    ps_rectangle(gc, &cr);
    ps_fill(gc);
    ps_reset_shadow(gc);
}

void init_context (ps_context* gc, ps_canvas* cs, unsigned char* buf)
{
    float version = (float)ps_version() / 10000;
    fprintf(stderr, "picasso version %.2f\n", version);

    pa = ps_path_create();
    pm = ps_matrix_create();
}

void dini_context (ps_context* gc)
{
    ps_image_unref(pi);
    ps_matrix_unref(pm);
    ps_pattern_unref(pt);
    ps_path_unref(pa);
    ps_canvas_unref(cx);
}

void set_image_data(unsigned char* data, ps_color_format fmt, int w, int h, int p)
{
    ps_context* gt;
    ps_color xol = {0.23f, 0.45f, 0.56f, 1};
    pi = ps_image_create_with_data(data, fmt, w, h, p);
    cx = ps_canvas_create(fmt, 200, 200);
    gt = ps_context_create(cx, 0);
    ps_set_source_color(gt, &xol);
    ps_clear(gt);
    ps_context_unref(gt);
}

void set_pattern_data(unsigned char* data, ps_color_format fmt, int w, int h, int p)
{
    ps_image* pam = ps_image_create_with_data(data, fmt, w, h, p);
    pt = ps_pattern_create_image(pam, WRAP_TYPE_REPEAT, WRAP_TYPE_REPEAT, pm);
}

void timer_action(ps_context* gc)
{
}

