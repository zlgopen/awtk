#include "stdio.h"

#include "../include/picasso.h"
#include "timeuse.h"
#include "drawFunc.h"

static ps_path * pa;
static ps_path * pb;
static ps_path * pr;
static ps_image * pi;
static ps_pattern * pt;
static ps_matrix* pm;
static int ct = 0;
static int step = 0;
void draw_test (int id, ps_context* gc)
{
    ps_color c1 = {0, 0, 1, 1};
    ps_color c2 = {1, 0, 0, 1};
    ps_color c3 = {0, 1, 0, 0.5};

    ps_set_source_color(gc, &c1);
    ps_set_path(gc, pa);
    ps_fill(gc);

    ps_set_source_color(gc, &c2);
    ps_set_path(gc, pb);
    ps_fill(gc);

    ps_path_clipping(pr, (ps_path_operation)ct, pa, pb);
    ps_set_source_color(gc, &c3);
    ps_set_path(gc, pr);
    ps_fill(gc);
}

void init_context (ps_context* gc, ps_canvas* cs, unsigned char* buf)
{
    ps_rect rc = {20, 20, 100, 80};
    ps_rect rc2 = {80, 20, 100, 80};

    ps_point pp = {21 ,25};

    float version = (float)ps_version() / 10000;
    fprintf(stderr, "picasso version %.2f\n", version);

    pa = ps_path_create();
    pb = ps_path_create();
    pr = ps_path_create();
    
    ps_path_add_rounded_rect(pa, &rc, 5, 5, 5, 5, 5, 5, 5, 5);

    ps_path_add_ellipse(pb, &rc2);

    if (ps_path_contains(pa, &pp, FILL_RULE_WINDING))
        fprintf (stderr, "point in path!\n");
    else
        fprintf (stderr, "point not in path!\n");

    pm = ps_matrix_create();
    ps_translate(gc, 20, 35);
}

void dini_context (ps_context* gc)
{
    ps_image_unref(pi);
    ps_matrix_unref(pm);
    ps_pattern_unref(pt);
    ps_path_unref(pa);
    ps_path_unref(pb);
    ps_path_unref(pr);
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
        ct++;
        step = 0;
    }

    if (ct > 3) ct = 0;
}

