#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#include "../include/picasso.h"
#include "drawFunc.h"

static ps_matrix* pm;
static ps_path * pa;
static ps_gradient * gr;
static int step = 0;
static ps_point p[2];
//static float gamma = 1.0f;
void draw_test (int id, ps_context* gc)
{
    ps_rect cr = {2.3f , 3.7f, 272.4f, 261.3f};

//    ps_color sol = {0, 0, 0, 1};
//    ps_set_shadow(gc, 0, 0, 0.375);
//    ps_set_shadow_color(gc, &sol);

    ps_rectangle(gc, &cr);

    ps_set_source_gradient(gc, gr);

    ps_fill(gc);

    ps_move_to(gc, &p[0]);
    ps_line_to(gc, &p[1]);

    ps_stroke(gc);
}

void init_context (ps_context* gc, ps_canvas* cs, unsigned char* buf)
{
    float version = (float)ps_version() / 10000;
    fprintf(stderr, "picasso version %.2f\n", version);
    srand ((unsigned)time(NULL));

    p[0].x = 150;
    p[0].y = 100;

    p[1].x = 50;
    p[1].y = 80;

    pa = ps_path_create();
    pm = ps_matrix_create();

//    gr = ps_gradient_create_conic(GRADIENT_SPREAD_PAD, &p[1], 0);
//    gr = ps_gradient_create_conic(GRADIENT_SPREAD_REFLECT, &p[1], -1.45);
//    gr = ps_gradient_create_conic(GRADIENT_SPREAD_REPEAT, &p[1], -1.45);
//    gr = ps_gradient_create_radial(GRADIENT_SPREAD_PAD, &p[1], 10, &p[1], 100);
    gr = ps_gradient_create_radial(GRADIENT_SPREAD_REFLECT, &p[1], 0, &p[1], 200);
//    gr = ps_gradient_create_radial(GRADIENT_SPREAD_REPEAT, &p[0], 10, &p[1], 100);
//    gr = ps_gradient_create_linear(GRADIENT_SPREAD_PAD, &p[0], &p[1]);
//    gr = ps_gradient_create_linear(GRADIENT_SPREAD_REFLECT, &p[0], &p[1]);
//    gr = ps_gradient_create_linear(GRADIENT_SPREAD_REPEAT, &p[0], &p[1]);

    ps_translate(gc, 33.3f, 45.7f);
#if 0
    ps_scale(gc, 0.5, 0.5);
    ps_rotate(gc, 0.35);
#endif
}

void dini_context (ps_context* gc)
{
    ps_matrix_unref(pm);
    ps_gradient_unref(gr);
    ps_path_unref(pa);
}

void set_image_data(unsigned char* data, ps_color_format fmt, int w, int h, int p)
{
    int i;
    ps_color pc;
    for (i=0; i < 10; i++){
        pc.r = (float)rand()/RAND_MAX;
        pc.g = (float)rand()/RAND_MAX;
        pc.b = (float)rand()/RAND_MAX;
        pc.a = 1;

        ps_gradient_add_color_stop(gr, (float)i*rand(), &pc);
    }
}

void set_pattern_data(unsigned char* data, ps_color_format fmt, int w, int h, int p)
{
}

void timer_action(ps_context* gc)
{
    int i;
    ps_color pc;
    step ++;
    if (step > 50) {
        ps_gradient_clear_color_stops(gr);
        for (i=0; i < 10; i++){
            pc.r = (float)rand()/RAND_MAX;
            pc.g = (float)rand()/RAND_MAX;
            pc.b = (float)rand()/RAND_MAX;
            pc.a = 1;

            ps_gradient_add_color_stop(gr, (float)i*rand(), &pc);
        }
        step = 0;
    }
}

