/* lake - application interface
 * 
 * Copyright (C) 2009 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

/*
 * This idea is come from AmanithVG.
 */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "picasso.h"
#include "interface.h"

static int width;
static int height;
static unsigned tid;
static double zoomFactor = 1;
static double power = 0.5;
static long times = 0;

static picture* pic = 0;
static int img_width;
static int img_height;

static void draw_wave(ps_context* gc, int h)
{
    int n, offset;
    ps_rect cr;
    ps_rect r = {0, img_height, img_width, img_height};
    int swing = h>>3;

    times++;
    for (n=0; n<h; n++) {
        offset=(int)((swing*(n+20)*sin((double)swing*(h-n)/(n+1)+times)/h)*power);

        if((n + offset) < 0) {
            offset = -n;
        } else if((n + offset) >= h) {
            offset = h - n - 1;
        }

        cr.x = r.x;
        cr.y = r.y+n;
        cr.w = pic->width; 
        cr.h = 1;

        ps_clip_device_rect(gc, &cr);

        cr.x = r.x;
        cr.y = r.y+n - h+1+(n<<1)+offset;
        cr.w = pic->width; 
        cr.h = h;

        ps_rectangle(gc, &cr);
        ps_set_source_image(gc, pic->image);
        ps_fill(gc);
        ps_reset_clip(gc);
    }
}

void on_init(ps_context* gc, int w, int h)
{
    width = w;
    height = h;
    zoomFactor = 1.0;

    pic = load_picture("lake");
    img_width = pic->width;
    img_height = pic->height;
    ps_set_filter(gc, FILTER_NEAREST);
    tid = set_timer(100);
}

void on_draw(ps_context* gc)
{
    ps_rect r = {0, 0, img_width, img_height};
    ps_color color = {1, 1, 1, 1};

    if (!pic)
        return;

    ps_set_source_color(gc, &color);
    ps_clear(gc);

    ps_set_source_image(gc, pic->image);
    ps_rectangle(gc, &r);
    ps_fill(gc);

    if (img_height+1 < (int)pic->height)
        img_height++;

    draw_wave(gc, img_height);
}

void on_term(ps_context* gc)
{
    clear_timer(tid);
    free_picture(pic);
}

void on_key_event(key_event_type kvt, int vk)
{
}

void on_mouse_event(mouse_event_type evt, unsigned key, int x, int y)
{
}

void on_timer()
{
    refresh(NULL);
}

void on_size(int w, int h)
{
    width = w;
    height = h;
}
