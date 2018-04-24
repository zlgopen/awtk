/* flowers - application interface
 * 
 * Copyright (C) 2009 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

/*
 * Conversion from the original AmanithVG's example, by Mazatech 
 * Srl - http://www.amanithvg.com 
 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "picasso.h"
#include "interface.h"

#define MAX_FLOWERS 350
#define MAX_PAINTS 100

#define PI 3.14159265358979323846f

static int width;
static int height;
static unsigned tid;
static ps_path * path = 0;
static float zoomFactor = 1;
static int check = 0;

typedef struct {
    int used;
    float x, y;
    float dx, dy;
    float rot;
    float rotf;
    float scl;
    float cr, cg, cb, ca;
}particle;

static particle flowers[MAX_FLOWERS];

void on_init(ps_context* gc, int w, int h)
{
    int i;
    ps_point ps;
    ps_point pt[3];
    width = w;
    height = h;
    path = ps_path_create();
    zoomFactor = 1.0f;
    srand((unsigned)time(NULL));

    /*init flowers*/
    for (i = 0; i < MAX_FLOWERS; i++) {
        flowers[i].used = 0;
        flowers[i].x = 0;
        flowers[i].y = 0;
        flowers[i].dx = 0;
        flowers[i].dx = 0;
        flowers[i].rot = 0;
        flowers[i].rotf = 0;
        flowers[i].scl = 0;
        flowers[i].cr = 0;
        flowers[i].cg = 0;
        flowers[i].cb = 0;
        flowers[i].ca = 0;
    }

    /*init path*/
    ps.x = -20; ps.y = 20;
    ps_path_move_to(path, &ps);
    pt[0].x = -200; pt[0].y = 170; 
    pt[1].x = -200; pt[1].y = -170; 
    pt[2].x = -20; pt[2].y = -20; 
    ps_path_bezier_to(path, &pt[0], &pt[1], &pt[2]);
    pt[0].x = -170; pt[0].y = -200; 
    pt[1].x = 170; pt[1].y = -200; 
    pt[2].x = 20; pt[2].y = -20; 
    ps_path_bezier_to(path, &pt[0], &pt[1], &pt[2]);
    pt[0].x = 200; pt[0].y = -170; 
    pt[1].x = 200; pt[1].y = 170; 
    pt[2].x = 20; pt[2].y = 20; 
    ps_path_bezier_to(path, &pt[0], &pt[1], &pt[2]);
    pt[0].x = 170; pt[0].y = 200; 
    pt[1].x = -170; pt[1].y = 200; 
    pt[2].x = -20; pt[2].y = 20; 
    ps_path_bezier_to(path, &pt[0], &pt[1], &pt[2]);
    ps_path_sub_close(path);

    tid = set_timer(50);
}

void on_draw(ps_context* gc)
{
    int i;
    ps_color fcolor;
    ps_color color = {0, 0, 0, 1};
    ps_color tcolor = {1, 1, 1, 1};
    ps_set_source_color(gc, &color);
    ps_clear(gc);

    ps_set_composite_operator(gc, COMPOSITE_PLUS);

    for (i = 0; i < MAX_FLOWERS; i++) {
        if (!flowers[i].used)
            continue;
        flowers[i].rot += flowers[i].rotf;
        flowers[i].x += flowers[i].dx;
        flowers[i].y -= flowers[i].dy;

        ps_identity(gc);
        ps_translate(gc, (float)width/2, (float)height/2);
        ps_scale(gc, zoomFactor, zoomFactor);
        ps_translate(gc, -(float)width/2, -(float)height/2);
        ps_rotate(gc, flowers[i].rot);
        ps_scale(gc, flowers[i].scl, flowers[i].scl);
        ps_translate(gc, flowers[i].x, flowers[i].y);
        fcolor.r = flowers[i].cr; fcolor.g = flowers[i].cg;
        fcolor.b = flowers[i].cb; fcolor.a = flowers[i].ca;
        ps_set_source_color(gc, &fcolor);
        ps_set_path(gc, path);
        ps_fill(gc);
    }

    ps_identity(gc);
    ps_set_text_color(gc, &tcolor);
    ps_text_out_length(gc, 0, 0, "Left mouse button click.", 24);
}

void on_term(ps_context* gc)
{
    clear_timer(tid);
    ps_path_unref(path);
}

void on_key_event(key_event_type kvt, int vk)
{
}

void on_mouse_event(mouse_event_type evt, unsigned key, int x, int y)
{
    int i;
    if ((evt == MOUSE_MOVE && (key&EVT_LBUTTON))
        || evt == LEFT_BUTTON_DOWN) {
        for (i = 0; i < MAX_FLOWERS; i++) {
            if (!flowers[i].used) {
                flowers[i].x = (float)x;
                flowers[i].y = (float)y;
                flowers[i].dx = 1.5f*((float)rand()/RAND_MAX)-1.6f;
                flowers[i].dy = 1.5f*((float)rand()/RAND_MAX)-1.6f;
                flowers[i].scl = 0.3f * ((float)rand()/RAND_MAX);
                flowers[i].rot = PI*((float)rand()/RAND_MAX);
                flowers[i].rotf = 0.2f*((float)rand()/RAND_MAX)-0.1f;
                flowers[i].cr = (float)rand()/RAND_MAX+0.1f;
                flowers[i].cg = (float)rand()/RAND_MAX+0.1f;
                flowers[i].cb = (float)rand()/RAND_MAX+0.1f;
                flowers[i].ca = (float)rand()/RAND_MAX+0.1f;
                flowers[i].used = 1;
                break;
            }
        }
    }
}

void on_timer()
{
    int i;
    check++;
    if (check > 200) {
        for (i = 0; i < MAX_FLOWERS; i++) {
            if (flowers[i].x < -200 * flowers[i].scl)
                flowers[i].used = 0;
        }
    }
    refresh(NULL);
}

void on_size(int w, int h)
{
    width = w;
    height = h;
}
