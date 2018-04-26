
#include <stdio.h>
#include <string.h>

#include "../include/picasso.h"
#include "../include/picasso_image.h"
#include "drawFunc.h"
#if defined(WIN32)
#include <direct.h>
#define getcwd _getcwd
#define DIRPREFIX "\\"
#else
#include <unistd.h>
#define DIRPREFIX "/"
#endif

static psx_image * image = NULL;
static char full_path[1024];
static const char * path = NULL;
static int idx = 0;

void draw_test (int id, ps_context* gc)
{
    ps_rect cr;
    ps_rect ir;
    ps_rect sr;
    ps_size ss;

    if (image) {
        ps_canvas * c = ps_context_get_canvas(gc);
        ps_canvas_get_size(c, &ss);
        ir.x = ir.y = 0;
        ir.w = (float)image->width;
        ir.h = (float)image->height;

        sr.x = 0; sr.y = 0;
        sr.w = ss.w; sr.h = ss.h;
        
        cr.w = ss.w > ir.w ? ir.w : ss.w;
        cr.h = ss.h > ir.h ? ir.h : ss.h;

        cr.x = (ss.w - cr.w) / 2;
        cr.y = (ss.h - cr.h) / 2;
        
        ps_save(gc);
        ps_rectangle(gc, &cr);
        ps_set_source_image(gc, IMG_OBJ_AT_INDEX(image, idx));
        ps_fill(gc);
        ps_restore(gc);
    }
}

void init_context (ps_context* gc, ps_canvas* cs, unsigned char* buf)
{
    float version = (float)ps_version() / 10000;
    fprintf(stderr, "picasso version %.2f\n", version);

    psx_image_init();

    if (argc() >= 1) { 
        path = argv()[1];
        if (path) {
            getcwd(full_path, 1024);
            strcat(full_path, DIRPREFIX);
            strcat(full_path, path);

            image = psx_image_load(full_path, NULL);
        }
    }
}

void dini_context (ps_context* gc)
{
    if (image)
        psx_image_destroy(image);
}

void set_image_data(unsigned char* data, ps_color_format fmt, int w, int h, int p)
{
}

void set_pattern_data(unsigned char* data, ps_color_format fmt, int w, int h, int p)
{
}

void timer_action(ps_context* gc)
{
    if (image) {
        fprintf (stderr, "current frame delay %d ms\n", IMG_DURATION_AT_INDEX(image, idx));
        idx++;
        if (idx == image->num_frames)
            idx = 0;
    }
}
