#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../include/picasso.h"
#include "drawFunc.h"

static ps_matrix* pm;
static ps_path * pa;
static ps_image * pi;
static ps_pattern * pt;
static ps_mask * m;

static ps_byte* mask_buf;

void init_mask(ps_byte* buffer, int w, int h)
{
    int i;
    ps_byte c = 255;
    for (i=0; i<h; i++) {
        if (c > 0) {
            memset(buffer+i*w, c, w);
            c--;
        }
    }
}

void draw_test (int id, ps_context* gc)
{
    ps_rect r = {0.3f, 2.7f, 320.45f, 240.19f};


    ps_ellipse(gc, &r);

    ps_set_source_image(gc, pi);
    ps_fill(gc);

}

void init_context (ps_context* gc, ps_canvas* cs, unsigned char* buf)
{
    float version = (float)ps_version() / 10000;
    fprintf(stderr, "picasso version %.2f\n", version);

    pa = ps_path_create();
    pm = ps_matrix_create();
    mask_buf = (ps_byte*)calloc(1, 320*240);
    init_mask(mask_buf, 320, 240);
    m = ps_mask_create_with_data(mask_buf, 320, 240);
    ps_canvas_set_mask(cs, m);
}

void dini_context (ps_context* gc)
{
    ps_mask_unref(m);
    free(mask_buf);
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
}

