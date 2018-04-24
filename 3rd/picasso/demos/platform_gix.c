/* platform - application framework base on picasso 
 * 
 * Copyright (C) 2009 Zhang Ji Peng <onecoolx@gmail.com>
 * Copyright (C) 2011 Easion <easion@hanxuantech.com>
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2.1 of the  *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 */

#include <stdio.h>
#include <gi/gi.h>
#include <gi/property.h>

#include "picasso.h"
#include "interface.h"


static int width;
static int height;
static ps_color_format fmt;    
static unsigned long gfmt;    

static gi_gc_ptr_t  gc;
gi_window_id_t g_window;
gi_image_t *g_bmp;

static ps_context *context;
static ps_canvas *canvas;

static int window_proc(gi_msg_t *msg,void *data);
static int get_virtual_key(int pk);


int  main(void)
{
    gi_msg_t msg;
    int gifd;
    gi_screen_info_t info;

    width = 640;
    height = 480;

    gifd = gi_init();
    if (gifd <= 0)
        exit(1);
    
#if 1
    gi_get_screen_info(&info);
    gfmt =  info.format;
    if (GI_RENDER_FORMAT_BPP(info.format) == 32)
        fmt = COLOR_FORMAT_BGRA;
    else if (GI_RENDER_FORMAT_BPP(info.format) == 24)
        fmt = COLOR_FORMAT_BGR;
    else if (GI_RENDER_FORMAT_BPP(info.format) == 16)
        fmt = COLOR_FORMAT_RGB565;
#else
    gfmt = GI_RENDER_x8r8g8b8;
    fmt = COLOR_FORMAT_BGRA;
#endif

    g_bmp = gi_create_image_depth(width, height, gfmt);
    ps_initialize();

    canvas = ps_canvas_create_with_data(g_bmp->rgba, fmt, width, height,g_bmp->pitch);
    context = ps_context_create(canvas, 0);
    on_init(context, width, height);    

    g_window = gi_create_window(GI_DESKTOP_WINDOW_ID, 10,30,width, height,
        GI_RGB( 240, 240, 242 ),
        GI_FLAGS_NORESIZE);

    if (g_window <= 0){
        printf("gi_create_window window error\n");
        return -1;
    }

    gi_set_events_mask(g_window, GI_MASK_CONFIGURENOTIFY
        | GI_MASK_KEY_DOWN | GI_MASK_KEY_UP
        | GI_MASK_MOUSE_MOVE | GI_MASK_BUTTON_UP
        | GI_MASK_CLIENT_MSG | GI_MASK_EXPOSURE
        | GI_MASK_BUTTON_DOWN 
        );

    gi_set_dispatch_handler(g_window, &window_proc,NULL);    
    gc = gi_create_gc((g_window),NULL);
    
    gi_set_window_caption(g_window,"Picasso Demos");
    gi_show_window(g_window);

    while (gi_get_new_message(&msg)) {
        gi_dispatch_message(&msg);
    }
    return 0;
}


static int window_proc(gi_msg_t *msg,void *data)
{
  unsigned int key;
  mouse_event_type type;

  switch (msg->type)
  {
    case GI_MSG_CLIENT_MSG:
        if(msg->body.client.client_type==GA_WM_PROTOCOLS
                &&msg->params[0] == GA_WM_DELETE_WINDOW){
        on_term(context);
        ps_context_unref(context);
        ps_canvas_unref(canvas);
        ps_shutdown();
        gi_destroy_image(g_bmp);
        gi_destroy_window(g_window);
        gi_quit_dispatch();
    }
    break;

    case GI_MSG_CONFIGURENOTIFY:
      {
        ps_canvas* old_canvas = 0;

        if (msg->params[0] != GI_STRUCT_CHANGE_RESIZE )
          return 0;

        width = msg->body.rect.w;
        height = msg->body.rect.h;

        if (g_bmp)    {
            gi_destroy_image(g_bmp);
        }        

        g_bmp = gi_create_image_depth(width, height, gfmt);
        canvas = ps_canvas_create_with_data(g_bmp->rgba, 
            COLOR_FORMAT_BGRA, width, height, g_bmp->pitch);
        old_canvas = ps_context_set_canvas(context, canvas);
        ps_canvas_unref(old_canvas);
        on_size(width, height);
      }
    break;

    case GI_MSG_WINDOW_DESTROY:
        break;

    case GI_MSG_KEY_DOWN:
        on_key_event(KEY_EVENT_DOWN, get_virtual_key(msg->params[3]));
        break;

    case GI_MSG_KEY_UP:
        on_key_event(KEY_EVENT_UP, get_virtual_key(msg->params[3]));
        break;

    case GI_MSG_MOUSE_MOVE:
        on_mouse_event(MOUSE_MOVE, 0, msg->body.rect.x, msg->body.rect.y);
        break;

    case GI_MSG_BUTTON_DOWN:
        switch(msg->params[2]){
        case GI_BUTTON_L:
            type = LEFT_BUTTON_DOWN;
            key = EVT_LBUTTON;
            break;
        case GI_BUTTON_M:
            type = MIDDLE_BUTTON_DOWN;
            key = EVT_MBUTTON;
            break;
        case GI_BUTTON_R:
            type = RIGHT_BUTTON_DOWN;
            key = EVT_RBUTTON;
            break;
        }
        on_mouse_event(type, key, msg->body.rect.x, msg->body.rect.y);
        break;

    case GI_MSG_BUTTON_UP:
        switch(msg->params[3]){
        case GI_BUTTON_L:
            type = LEFT_BUTTON_UP;
            key = EVT_LBUTTON;
            break;
        case GI_BUTTON_M:
            type = MIDDLE_BUTTON_UP;
            key = EVT_MBUTTON;
            break;
        case GI_BUTTON_R:
            type = RIGHT_BUTTON_UP;
            key = EVT_RBUTTON;
            break;
        }
        on_mouse_event(type, key, msg->body.rect.x, msg->body.rect.y);
        break;

    case GI_MSG_EXPOSURE:
        on_draw(context);
        gi_draw_image(gc,g_bmp,0,0);
        break;

    default:
        return gi_default_handler(msg,data);    
  }
  return TRUE;
}

static int my_timer_handler(void* arg)
{
    on_timer();
    return TRUE; //continue.
}

unsigned set_timer(unsigned mc)
{
    unsigned sid = 0;

    sid = (unsigned)gi_dispatch_timer_new(mc, &my_timer_handler,(void*)NULL);
    return sid;
}

void clear_timer(unsigned id)
{
    gi_dispatch_timer_remove((void*)id);
}

void refresh(const ps_rect* r)
{
    if (r) {
        gi_clear_area(g_window,(int)r->x, (int)r->y, 
            (int)(r->w), (int)(r->h),TRUE);
    } else {
        on_draw(context);
        gi_draw_image(gc,g_bmp,0,0);
    }
}

picture* load_picture(const char* name)
{
    picture* pic;
    ps_image * img;
    gi_image_t* nb = NULL;
    unsigned size;
    char* pname;
    ps_color_format fmt;    

    if (!name || !strlen(name))
        return NULL;

    pname = (char*)malloc(strlen(name)+5);
    sprintf(pname, "%s.png", name);

    gi_image_create_from_png(pname, &nb); 
    if (!nb){
        fprintf(stderr, "open file %s failed\n", pname);
        return NULL;
    }
    if (GI_RENDER_FORMAT_BPP(nb->format) == 32)
        fmt = COLOR_FORMAT_BGRA;
    else if (GI_RENDER_FORMAT_BPP(nb->format) == 24)
        fmt = COLOR_FORMAT_BGR;
    else if (GI_RENDER_FORMAT_BPP(nb->format) == 16)
        fmt = COLOR_FORMAT_RGB565;
    
    free(pname);
    img = ps_image_create_with_data(nb->rgba, 
        fmt, nb->w, nb->h, nb->pitch);

    pic = (picture*)malloc(sizeof(picture));
    pic->width = nb->w;
    pic->height = nb->h;
    pic->native = nb;
    pic->image = img;
    return pic;
}

void free_picture(picture* p)
{
    if (!p)
        return;

    ps_image_unref(p->image);
    gi_destroy_image(((gi_image_t*)p->native));
    free(p);
}



typedef struct {
    int vk;
    unsigned pk;
}KeyEntities;

static KeyEntities key_map[] = {
    {KEY_BACK, G_KEY_BACKSPACE},
    {KEY_TAB, G_KEY_TAB},
    {KEY_RETURN, G_KEY_ENTER},
    {KEY_SHIFT, G_KEY_LSHIFT},
    {KEY_CONTROL, G_KEY_LCTRL},
    {KEY_MENU, G_KEY_MENU},
    {KEY_PAUSE, G_KEY_PAUSE},
    {KEY_CAPITAL, G_KEY_CAPSLOCK},
    {KEY_ESCAPE, G_KEY_ESCAPE},
    {KEY_SPACE, G_KEY_SPACE},
    {KEY_END, G_KEY_END},
    {KEY_HOME, G_KEY_HOME},
    {KEY_LEFT, G_KEY_LEFT},
    {KEY_UP, G_KEY_UP},
    {KEY_RIGHT, G_KEY_RIGHT},
    {KEY_DOWN, G_KEY_DOWN},
    {KEY_INSERT, G_KEY_INSERT},
    {KEY_DELETE, G_KEY_DELETE},
    {KEY_0, G_KEY_0},
    {KEY_1, G_KEY_1},
    {KEY_2, G_KEY_2},
    {KEY_3, G_KEY_3},
    {KEY_4, G_KEY_4},
    {KEY_5, G_KEY_5},
    {KEY_6, G_KEY_6},
    {KEY_7, G_KEY_7},
    {KEY_8, G_KEY_8},
    {KEY_9, G_KEY_9},
    {KEY_A, G_KEY_a},
    {KEY_B, G_KEY_b},
    {KEY_C, G_KEY_c},
    {KEY_D, G_KEY_d},
    {KEY_E, G_KEY_e},
    {KEY_F, G_KEY_f},
    {KEY_G, G_KEY_g},
    {KEY_H, G_KEY_h},
    {KEY_I, G_KEY_i},
    {KEY_J, G_KEY_j},
    {KEY_K, G_KEY_k},
    {KEY_L, G_KEY_l},
    {KEY_M, G_KEY_m},
    {KEY_N, G_KEY_n},
    {KEY_O, G_KEY_o},
    {KEY_P, G_KEY_p},
    {KEY_Q, G_KEY_q},
    {KEY_R, G_KEY_r},
    {KEY_S, G_KEY_s},
    {KEY_T, G_KEY_t},
    {KEY_U, G_KEY_u},
    {KEY_V, G_KEY_v},
    {KEY_W, G_KEY_w},
    {KEY_X, G_KEY_x},
    {KEY_Y, G_KEY_y},
    {KEY_Z, G_KEY_z},
    //{KEY_LWIN, G_KEY_LWIN},
    //{KEY_RWIN, G_KEY_RIGHTWIN},
    {KEY_OEM_PLUS, G_KEY_EQUALS},
    {KEY_OEM_MINUS, G_KEY_MINUS},
    {KEY_F1, G_KEY_F1},
    {KEY_F2, G_KEY_F2},
    {KEY_F3, G_KEY_F3},
    {KEY_F4, G_KEY_F4},
    {KEY_F5, G_KEY_F5},
    {KEY_F6, G_KEY_F6},
    {KEY_F7, G_KEY_F7},
    {KEY_F8, G_KEY_F8},
    {KEY_F9, G_KEY_F9},
    {KEY_F10, G_KEY_F10},
    {KEY_F11, G_KEY_F11},
    {KEY_F12, G_KEY_F12},
    {KEY_LSHIFT, G_KEY_LSHIFT},
    {KEY_RSHIFT, G_KEY_RSHIFT},
    {KEY_LCONTROL, G_KEY_LCTRL},
    {KEY_RCONTROL, G_KEY_RCTRL},
};

static int get_virtual_key(int pk)
{
    int i;
    for(i = 0; i < (sizeof(key_map)/sizeof(KeyEntities)); i++)
        if (key_map[i].pk == pk)
            return key_map[i].vk;
    return KEY_UNKNOWN; 
}


