#include "stdio.h"
#include "math.h"

#include "../include/picasso.h"
#include "drawFunc.h"

static ps_matrix* pm;
static ps_path * pa;
static ps_image * pi;
static ps_image * pam;
static ps_pattern * pt1;
static ps_pattern * pt2;
static ps_pattern * pt3;
static ps_pattern * pt4;

void draw_test (int id, ps_context* gc)
{
    ps_rect r1 = {10.1f, 10, 200.3f, 200.6f};
    ps_rect r2 = {230.2f, 10, 200.3f, 200.6f};
    ps_rect r3 = {10.1f, 230, 200.3f, 200.6f};
    ps_rect r4 = {230.2f, 230, 200.3f, 200.6f};

    ps_rectangle(gc, &r1);
    ps_set_source_pattern(gc, pt1);
    ps_fill(gc);

    ps_rectangle(gc, &r2);
    ps_set_source_pattern(gc, pt2);
    ps_fill(gc);

    ps_rectangle(gc, &r3);
    ps_set_source_pattern(gc, pt3);
    ps_fill(gc);

    ps_rectangle(gc, &r4);
    ps_set_source_pattern(gc, pt4);
    ps_fill(gc);
}

void init_context (ps_context* gc, ps_canvas* cs, unsigned char* buf)
{
    float version = (float)ps_version() / 10000;
    fprintf(stderr, "picasso version %.2f\n", version);

    pa = ps_path_create();
    pm = ps_matrix_create();

    ps_translate(gc, 86.3f, 5.1f);
}

void dini_context (ps_context* gc)
{
    ps_image_unref(pi);
    ps_matrix_unref(pm);
    ps_pattern_unref(pt1);
    ps_pattern_unref(pt2);
    ps_pattern_unref(pt3);
    ps_pattern_unref(pt4);
    ps_path_unref(pa);
}

void set_image_data(unsigned char* data, ps_color_format fmt, int w, int h, int p)
{
    pi = ps_image_create_with_data(data, fmt, w, h, p);
}

void set_pattern_data(unsigned char* data, ps_color_format fmt, int w, int h, int p)
{
    pam = ps_image_create_with_data(data, fmt, w, h, p);
    pt1 = ps_pattern_create_image(pam, WRAP_TYPE_REPEAT, WRAP_TYPE_REPEAT, pm);
    pt2 = ps_pattern_create_image(pam, WRAP_TYPE_REPEAT, WRAP_TYPE_REFLECT, pm);
    pt3 = ps_pattern_create_image(pam, WRAP_TYPE_REFLECT, WRAP_TYPE_REPEAT, pm);
    pt4 = ps_pattern_create_image(pam, WRAP_TYPE_REFLECT, WRAP_TYPE_REFLECT, pm);
}

void timer_action(ps_context* gc)
{
}

