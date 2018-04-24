#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../include/picasso.h"
#include "drawFunc.h"

static ps_matrix* pm;
static ps_path * pa;
static ps_image * pi;
static ps_image * pp;
static ps_pattern * pt;
static ps_canvas* cv;

void draw_test (int id, ps_context* gc)
{
    ps_rect r = {0, 0, 320, 240};
    ps_rect r2 = {100, 100, 200, 200};

    ps_point p = {300.7f, 200.4f};

    ps_rectangle(gc, &r);
    ps_set_source_image(gc, pi);
    ps_fill(gc);

    ps_canvas_bitblt(cv, &r2, cv, &p);
    p.x = 160, p.y=100;
    ps_canvas_bitblt(cv, &r2, cv, &p);
}

void init_context (ps_context* gc, ps_canvas* cs, unsigned char* buf)
{
    float version = (float)ps_version() / 10000;
    fprintf(stderr, "picasso version %.2f\n", version);

    pa = ps_path_create();
    pm = ps_matrix_create();

    cv = cs;
}

void dini_context (ps_context* gc)
{
    ps_image_unref(pi);
    ps_matrix_unref(pm);
    ps_pattern_unref(pt);
    ps_path_unref(pa);
}

void set_image_data(unsigned char* data, ps_color_format fmt, int w, int h, int p)
{
    ps_rect prect = {100,100, 100, 50};
    pi = ps_image_create_with_data(data, fmt, w, h, p);
    pp = ps_image_create_from_image(pi, &prect);
}

void set_pattern_data(unsigned char* data, ps_color_format fmt, int w, int h, int p)
{
    ps_image* pam = ps_image_create_with_data(data, fmt, w, h, p);
    pt = ps_pattern_create_image(pam, WRAP_TYPE_REPEAT, WRAP_TYPE_REPEAT, pm);
}

void timer_action(ps_context* gc)
{
}

