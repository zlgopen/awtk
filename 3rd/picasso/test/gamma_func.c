#include "stdio.h"

#include "../include/picasso.h"
#include "drawFunc.h"

static ps_rect gr = {200, 100, 320, 240};
static ps_rect gt = {100, 100, 200, 200};
static ps_matrix* pm;
static ps_path * pa;
static ps_image * pi;
static ps_pattern * pt;
static float gamma = 1.0f;
static int step = 0;
static ps_bool anti = False;

struct dash_data 
{
    float s;
    float d[4];
} dashs[] = {
    {1, {1, 1, 1, 1}},
    {0.5, {1.5, 3, 1.5, 3}},
    {2, {4, 1, 4, 1}},
    {1, {0.5, 1.5, 1, 2}},
};

void draw_test (int id, ps_context* gc)
{
    ps_color col = {0, 0, 1, 1};
    ps_color sol = {1, 0, 0, 1};
    ps_rect cr = {2.7f , 3.4f, 272.4f, 261.3f};
    ps_point s = {50.1f, 50.3f};
    ps_point e = {200.7f, 50.3f};

    ps_point p[3] = {{100,100},{200, 300},{ 300, 40}};
    ps_point cp = {128, 128};
    ps_point sc = {10, 228};
    ps_rect br = {50, 50, 120, 100};

    ps_set_line_width(gc, 1);
    ps_set_stroke_color(gc, &sol);

//    ps_set_line_dash(gc, dashs[0].s, dashs[0].d, 4);
    ps_set_source_color(gc, &col);
    ps_arc (gc, &cp, 10 , 0, 6.28f, True);
    ps_stroke(gc);

//    ps_set_line_dash(gc, dashs[1].s, dashs[1].d, 4);
    ps_tangent_arc(gc, &br, 0, 1.254f);
    ps_stroke(gc);
    ps_move_to(gc, &sc);
    ps_bezier_curve_to(gc, &p[0], &p[1], &p[2]);

    ps_arc (gc, &cp, 100 , 0.785f, 3.140f, False);
    
    ps_bezier_curve_to(gc, &p[0], &p[1], &p[2]);
    
//    ps_set_line_dash(gc, dashs[2].s, dashs[2].d, 4);
    ps_quad_curve_to(gc, &p[1], &p[2]);
    ps_stroke(gc);

//    ps_set_line_dash(gc, dashs[3].s, dashs[3].d, 4);
    ps_rectangle(gc, &cr);
    ps_rounded_rect(gc, &gt, 45.2f, 35.2f, 25.2f, 25.2f, 35.2f, 55.2f, 65.2f, 85.2f);
    ps_ellipse(gc, &gr);
    ps_stroke(gc);

//    ps_reset_line_dash(gc);
    ps_move_to(gc, &s);
    ps_line_to(gc, &e);
    ps_stroke(gc);
}

void init_context (ps_context* gc, ps_canvas* cs, unsigned char* buf)
{
    float version = (float)ps_version() / 10000;
    fprintf(stderr, "picasso version %.2f\n", version);

    pa = ps_path_create();
    pm = ps_matrix_create();

    ps_translate(gc, 203, 35);
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
    pi = ps_image_create_with_data(data, fmt, w, h, p);
}

void set_pattern_data(unsigned char* data, ps_color_format fmt, int w, int h, int p)
{
    ps_image* pam = ps_image_create_with_data(data, fmt, w, h, p);
    pt = ps_pattern_create_image(pam, WRAP_TYPE_REPEAT, WRAP_TYPE_REPEAT, pm);
}

void timer_action(ps_context* gc)
{
    step ++;
    if (step > 5) {
        step = 0;
        ps_set_gamma(gc, gamma);
        gamma++;
    }
    if (gamma > 5) {
        gamma = 0.05f;
        ps_set_antialias(gc, anti);
        if (!anti) anti = True;
        else anti = False;
    }
}

