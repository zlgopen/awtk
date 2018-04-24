#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../include/picasso.h"
#include "drawFunc.h"
#include "timeuse.h"

static ps_gradient * gr;
static ps_matrix* pm;
static ps_matrix* pn;
static ps_path * pa;
static ps_image * pi;
static ps_pattern * pt;
static ps_point p[2];
static ps_font* pf;
const char* text = "Hello World! this is the first words picasso out put!";
const ps_uchar16 wtext[] = {0x77e2,0x91cf,0x56fe,0x5f62,0x5e93};
const char gtext[] = "ASDFG";
static int step = 0;
void draw_test (int id, ps_context* gc)
{
//    int i;
//    double w;
    ps_font* old;
//    ps_size ts;
//    ps_font_info info;
//    ps_glyph g[5];
    ps_glyph tg;
//    ps_point s1;
//    ps_point e1;
    ps_color c1= {1, 0, 0, 0.8f};
    ps_color c2= {0, 0, 1, 0.8f};
    //ps_color x = {0, 0, 0, 1};
    ps_rect r = {150, 150, 400, 250};
    /*
    ps_rect er = {150, 50, 100, 100};
    ps_reset_clip(gc);
    ps_ellipse(gc, &er);
    ps_clip(gc);
    */
        ps_point s = {100, 100};
        ps_point e = {250, 100};

//    ps_set_shadow(gc, 1, 1, 0.15f);
//    ps_set_shadow_color(gc, &x);
    ps_translate(gc, -200, -200);
    ps_rotate(gc, 0.02f);
    ps_translate(gc, 200, 200);

  //  ps_set_text_antialias(gc, False);
        
//        double ds[] = {5, 3.1, 1.2, 5.5};

//        ps_set_line_dash(gc, 0, ds, 4);
        ps_move_to(gc, &s);
        ps_line_to(gc, &e);
    ps_stroke(gc);

  //  ps_set_text_render_type(gc, TEXT_TYPE_MONO);
    ps_set_text_matrix(gc, pn);
    ps_text_out_length(gc, 100, 100, text, strlen(text));


/*    
    if (a) {
    //    ps_font_set_size(pf, 16);
        ps_set_text_color(gc, &c1);
        a = 0;
    } else {
    //    ps_font_set_size(pf, 26);
        ps_set_text_color(gc, &c2);
        a = 1;
    }
*/
    //ps_matrix_rotate(pm, 0.13);
    ps_set_text_matrix(gc, pm);
    //ps_font_set_weight(pf, w);
    ps_set_text_color(gc, &c1);
    ps_set_text_stroke_color(gc, &c2);

    ps_rectangle(gc, &r);
    ps_stroke(gc);
//    ps_wide_text_out_length(gc, 200, 100, wtext, 5);
    old = ps_set_font(gc, pf);

    //ts = ps_text_extent(gc, wtext, 5);
//fprintf(stderr, "w: %.2f - h: %.2f \n", ts.w, ts.h);
///    s1.x = s1.y =  150;
//    e1.y = 150;
//    e1.x = 150 + ts.w;
//    ps_move_to(gc, &s1);
//    ps_line_to(gc, &e1);
//    ps_stroke(gc);


    ps_draw_text(gc, &r, wtext, 5, DRAW_TEXT_STROKE, TEXT_ALIGN_LEFT | TEXT_ALIGN_TOP);

    //ps_set_source_gradient(gc, gr);
    //ps_set_source_image(gc, pi);
    ps_set_source_pattern(gc, pt);
    ps_get_glyph(gc, 0x56fe, &tg);

//    ts = ps_glyph_extent(&tg);
//fprintf(stderr, "w: %.2f - h: %.2f \n", ts.w, ts.h);
    ps_get_path_from_glyph(gc, &tg, pa);
    ps_set_path(gc, pa);
    ps_fill(gc);

//    ps_get_font_info(gc, &info);
//fprintf(stderr, "h: %.2f - a: %.2f - d: %.2f - l: %.2f \n", info.size, info.ascent, info.descent, info.leading);
    ps_set_font(gc, old);
    ps_set_text_render_type(gc, TEXT_TYPE_STROKE);

/*
    for (i=0; i<5; i++)
        ps_get_glyph(gc, gtext[i], &g[i]);

    ps_show_glyphs(gc, 200, 200, g, 5);
*/
}

void init_context (ps_context* gc, ps_canvas* cs, unsigned char* buf)
{
    float version = (float)ps_version() / 10000;
    fprintf(stderr, "picasso version %.2f\n", version);

    pa = ps_path_create();
    pm = ps_matrix_create();
    pn = ps_matrix_create();

    pf = ps_font_create("ËÎÌו", CHARSET_UNICODE,48, FONT_WEIGHT_BOLD, False);

    p[0].x = 150;
    p[0].y = 100;

    p[1].x = 50;
    p[1].y = 80;
    gr = ps_gradient_create_conic(GRADIENT_SPREAD_REFLECT, &p[1], 0);
    //ps_scale(gc, 0.5, 0.5);
    //ps_rotate(gc, 0.21);
    //ps_shear(gc, 0.5, 0.2);
    //ps_matrix_shear(pm, 0.5, 0.2);
   // ps_matrix_scale(pm, 0.5, 0.5);
}

void dini_context (ps_context* gc)
{
    ps_image_ref(pi);
    ps_matrix_unref(pm);
    ps_matrix_unref(pn);
    ps_pattern_unref(pt);
    ps_path_unref(pa);
    ps_gradient_unref(gr);
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
    }
}

