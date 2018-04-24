#include "stdio.h"

#include "../include/picasso.h"
#include "drawFunc.h"

static ps_matrix* pm;
static ps_path * pa;
static ps_image * pi;
static ps_pattern * pt;
static int ct = 0;
static int lc = 1;
static int step = 0;
void draw_test (int id, ps_context* gc)
{
    ps_rect br = {2.2f , 4.4f, 322.6f, 261.1f};
    ps_rect cr = {2.2f , 4.4f, 222.6f, 261.1f};
    ps_rect dr = {2.2f , 4.4f, 122.6f, 161.1f};
    ps_rect er = {2.2f , 4.4f, 22.6f, 61.1f};

    switch (ct) {
        case 0:
            {
                if (lc > 0) {
                    ps_save(gc);
                    ps_rectangle(gc, &cr);
                    ps_clip(gc);
                } else {
                    ps_restore(gc);
                }
            }
            break;
        case 1:
            {
                if (lc > 0) {
                    ps_save(gc);
                    ps_rectangle(gc, &dr);
                    ps_clip(gc);
                } else {
                    ps_restore(gc);
                }
            }
            break;
        case 2:
            {
                if (lc > 0) {
                    ps_save(gc);
                    ps_rectangle(gc, &er);
                    ps_clip(gc);
                    ps_set_antialias(gc, False);
                } else {
                    ps_restore(gc);
                }
            }
            break;
    }

    ps_rectangle(gc, &br);
    ps_set_source_image(gc, pi);
    //ps_set_source_pattern(gc, pt);
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
        ct += lc;
        step = 0;
    }

    if (ct > 2) {
        lc = -1;
       }
    if (ct < 0) {
        lc = 1;
    }
}

