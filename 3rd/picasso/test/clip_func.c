#include "stdio.h"

#include "../include/picasso.h"
#include "drawFunc.h"

static ps_matrix* pm;
static ps_path * pa;
static ps_image * pi;
static ps_pattern * pt;
static int ct = 0;
static int step = 0;
void draw_test (int id, ps_context* gc)
{
    ps_rect cr = {2.2f , 4.4f, 322.6f, 261.1f};

    ps_reset_clip(gc);

    switch (ct)
    {
    case 1:
        {
            ps_rect er = {10, 10, 120, 100};
            ps_ellipse(gc, &er);
            ps_clip(gc);
        }
        break;
    case 2:
        {
            ps_rect r1 = {10, 10, 80, 80};
            ps_rect r2 = {70, 10, 80, 80};
            ps_rect r3 = {40, 70, 80, 80};

            ps_clip_rect(gc, &r1);
            ps_clip_rect(gc, &r2);
            ps_clip_rect(gc, &r3);
        }
        break;
    case 3:
        {
            ps_rect ra[] = {{10, 10, 50, 50},{70, 10, 50, 50},{40, 70, 50, 50}};

            ps_clip_rects(gc, ra, 3);
        }
        break;
    case 4:
        {
            ps_rect ar = {30, 30, 80,100};
            ps_rect br = {10, 10, 120, 140};
            ps_path * ph = ps_path_create();
            ps_path_add_rect(ph, &ar);
            ps_path_add_rounded_rect(ph, &br, 10, 10, 10, 10, 15, 15, 15, 15);
            ps_path_sub_close(ph);

            ps_clip_path(gc, ph, FILL_RULE_EVEN_ODD);

            ps_path_unref(ph);
        }
        break;
    }

    ps_rectangle(gc, &cr);
    //ps_set_source_image(gc, pi);
    ps_set_source_pattern(gc, pt);
    ps_fill(gc);
}

void init_context (ps_context* gc, ps_canvas* cs, unsigned char* buf)
{
    float version = (float)ps_version() / 10000;
    fprintf(stderr, "picasso version %.2f\n", version);

    pa = ps_path_create();
    pm = ps_matrix_create();

    ps_translate(gc, 203, 35);
    ps_rotate(gc, 0.32f);
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
        ct++;
        step = 0;
    }

    if (ct > 4) ct = 0;
}

