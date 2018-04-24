/* platform - application framework base on picasso 
 * 
 * Copyright (C) 2009 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "picasso.h"
#include "interface.h"

static int width;
static int height;

GtkWidget *window;
GtkWidget *drawarea;

GdkPixbuf * pixbuf;

static ps_context *context;
static ps_canvas *canvas;
static ps_color_format fmt;    

static int get_virtual_key(int pk);

static void size_change(GtkWidget *widget, GtkAllocation *allocation)
{
    ps_canvas* old_canvas = 0;
    width = allocation->width;
    height = allocation->height;
    g_object_unref(pixbuf);

    if (width < 1)
        width = 1;
    if (height < 1)
        height = 1;

    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    gchar* buf = (gchar*)gdk_pixbuf_get_pixels(pixbuf);
    gint stride = gdk_pixbuf_get_rowstride(pixbuf);
    canvas = ps_canvas_create_with_data((ps_byte*)buf, fmt, width, height, stride);
    old_canvas = ps_context_set_canvas(context, canvas);
    ps_canvas_unref(old_canvas);
    on_size(width, height);
}

static gboolean expose(GtkWidget *widget, GdkEventExpose *event)
{
    on_draw(context);
    gdk_draw_pixbuf(widget->window, widget->style->white_gc, pixbuf, 0, 0, 0, 0, width, height, 0,0,0);
    return FALSE;
}

static void destroy(GtkWidget *widget, gpointer data)
{
    on_term(context);
    ps_context_unref(context);
    ps_canvas_unref(canvas);
    ps_shutdown();
    g_object_unref(pixbuf);
    gtk_main_quit();
}

static gboolean time_func(gpointer data)
{
    on_timer();
    return TRUE;
}

static void init_pixbuf()
{
    GError * e = 0;
    fmt = COLOR_FORMAT_RGB;
    ps_initialize();
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, width, height);
    gchar* buf = (gchar*)gdk_pixbuf_get_pixels(pixbuf);
    gint stride = gdk_pixbuf_get_rowstride(pixbuf);
    canvas = ps_canvas_create_with_data((ps_byte*)buf, fmt, width, height, stride);
    context = ps_context_create(canvas, 0);
    on_init(context, width, height);
}

picture* load_picture(const char* name)
{
    GdkPixbuf * nb;
    picture* pic;
    ps_image * img;
    gchar* pname;
    GError * e = 0;

    if (!name || !strlen(name))
        return NULL;

    pname = (char*)malloc(strlen(name)+5);
    sprintf(pname, "%s.png", name);
    nb = gdk_pixbuf_new_from_file(pname, &e);
    free(pname);
    gchar* buf = (gchar*)gdk_pixbuf_get_pixels(nb);
    gint w = gdk_pixbuf_get_width(nb);
    gint h = gdk_pixbuf_get_height(nb);
    gint r = gdk_pixbuf_get_rowstride(nb);
    img = ps_image_create_with_data((ps_byte*)buf, fmt, w, h, r);

    pic = (picture*)malloc(sizeof(picture));
    pic->width = w;
    pic->height = h;
    pic->native = nb;
    pic->image = img;

    return pic;
}

void free_picture(picture* p)
{
    if (!p)
        return;

    ps_image_unref(p->image);
    g_object_unref((GdkPixbuf*)p->native);
    free(p);
}

unsigned set_timer(unsigned mc)
{
    return g_timeout_add(mc, time_func, GTK_WIDGET(drawarea));
}

void clear_timer(unsigned id)
{
    g_source_remove(id);
}

void refresh(const ps_rect* r)
{
    if (r) {
        gtk_widget_queue_draw_area(GTK_WIDGET(drawarea), r->x, r->y, r->w, r->h);
    } else {
        gtk_widget_queue_draw(GTK_WIDGET(drawarea));
    }
}

static gboolean key_press(GtkWidget *widget, GdkEventKey *event)
{
    on_key_event(KEY_EVENT_DOWN, get_virtual_key(event->keyval));
    return FALSE;
}

static gboolean key_release(GtkWidget *widget, GdkEventKey *event)
{
    on_key_event(KEY_EVENT_UP, get_virtual_key(event->keyval));
    return FALSE;
}

static gboolean mouse_button_press(GtkWidget *widget, GdkEventButton *event)
{
    mouse_event_type type;
    unsigned int key = 0;
    switch(event->button){
        case 1:
            type = LEFT_BUTTON_DOWN;
            key = EVT_LBUTTON;
            break;
        case 2:
            type = MIDDLE_BUTTON_DOWN;
            key = EVT_MBUTTON;
            break;
        case 3:
            type = RIGHT_BUTTON_DOWN;
            key = EVT_RBUTTON;
            break;
    }
    if (event->state & 1)
        key |= EVT_SHIFT;
    if (event->state & 4)
        key |= EVT_CONTROL;
    on_mouse_event(type, key, (int)event->x, (int)event->y);
    return FALSE;
}

static gboolean mouse_button_release(GtkWidget *widget, GdkEventButton *event)
{
    mouse_event_type type;
    unsigned int key = 0;
    switch(event->button){
        case 1:
            type = LEFT_BUTTON_UP;
            key = EVT_LBUTTON;
            break;
        case 2:
            type = MIDDLE_BUTTON_UP;
            key = EVT_MBUTTON;
            break;
        case 3:
            type = RIGHT_BUTTON_UP;
            key = EVT_RBUTTON;
            break;
    }
    if (event->state & 1)
        key |= EVT_SHIFT;
    if (event->state & 4)
        key |= EVT_CONTROL;
    on_mouse_event(type, key, (int)event->x, (int)event->y);
    return FALSE;
}

static gboolean mouse_motion_notify(GtkWidget *widget, GdkEventMotion *event)
{
    unsigned int key = 0;
    if (event->state & 1)
        key |= EVT_SHIFT;
    if (event->state & 4)
        key |= EVT_CONTROL;
    if (event->state & 256)
        key |= EVT_LBUTTON;
    if (event->state & 512)
        key |= EVT_MBUTTON;
    if (event->state & 1024)
        key |= EVT_RBUTTON;
    on_mouse_event(MOUSE_MOVE, key, (int)event->x, (int)event->y);
    return FALSE;
}

int main(int argc, char* argv[])
{
    width = 640;
    height = 480;

    gtk_init(&argc, &argv);

    init_pixbuf();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_title(GTK_WINDOW(window), "Picasso Demos");

    drawarea = gtk_drawing_area_new();

    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK (destroy), NULL);

    g_signal_connect (G_OBJECT(drawarea), "expose_event", G_CALLBACK (expose), NULL);

    g_signal_connect (G_OBJECT(window), "size_allocate", G_CALLBACK (size_change), NULL);

    g_signal_connect (G_OBJECT(window), "key_press_event", G_CALLBACK (key_press), NULL);

    g_signal_connect (G_OBJECT(window), "key_release_event", G_CALLBACK (key_release), NULL);

    gtk_widget_add_events (GTK_WIDGET(window), GDK_BUTTON_PRESS_MASK);
    g_signal_connect (G_OBJECT(window), "button_press_event", G_CALLBACK (mouse_button_press), NULL);

    gtk_widget_add_events (GTK_WIDGET(window), GDK_BUTTON_RELEASE_MASK);
    g_signal_connect (G_OBJECT(window), "button_release_event", G_CALLBACK (mouse_button_release), NULL);

    gtk_widget_add_events (GTK_WIDGET(window), GDK_POINTER_MOTION_MASK);
    g_signal_connect (G_OBJECT(window), "motion_notify_event", G_CALLBACK (mouse_motion_notify), NULL);

    gtk_container_add (GTK_CONTAINER (window), drawarea);

    gtk_widget_show(drawarea);
    gtk_widget_show(window);

    gtk_main();

    return 0;
}


typedef struct {
    int vk;
    int pk;
}KeyEntities;

static KeyEntities key_map[] = {
    {KEY_BACK, GDK_BackSpace},
    {KEY_TAB, GDK_Tab},
    {KEY_CLEAR, GDK_Clear},
    {KEY_RETURN, GDK_Return},
    {KEY_SHIFT, GDK_Shift_L},
    {KEY_CONTROL, GDK_Control_L},
    {KEY_MENU, GDK_Menu},
    {KEY_PAUSE, GDK_Pause},
    {KEY_CAPITAL, GDK_Caps_Lock},
    {KEY_ESCAPE, GDK_Escape},
    {KEY_SPACE, GDK_space},
    {KEY_PRIOR, GDK_Prior},
    {KEY_NEXT, GDK_Next},
    {KEY_END, GDK_End},
    {KEY_HOME, GDK_Home},
    {KEY_LEFT, GDK_Left},
    {KEY_UP, GDK_Up},
    {KEY_RIGHT, GDK_Right},
    {KEY_DOWN, GDK_Down},
    {KEY_SELECT, GDK_Select},
    {KEY_PRINT, GDK_Print},
    {KEY_EXEC, GDK_Execute},
    {KEY_INSERT, GDK_Insert},
    {KEY_DELETE, GDK_Delete},
    {KEY_HELP, GDK_Help},
    {KEY_0, GDK_0},
    {KEY_1, GDK_1},
    {KEY_2, GDK_2},
    {KEY_3, GDK_3},
    {KEY_4, GDK_4},
    {KEY_5, GDK_5},
    {KEY_6, GDK_6},
    {KEY_7, GDK_7},
    {KEY_8, GDK_8},
    {KEY_9, GDK_9},
    {KEY_A, GDK_a},
    {KEY_B, GDK_b},
    {KEY_C, GDK_c},
    {KEY_D, GDK_d},
    {KEY_E, GDK_e},
    {KEY_F, GDK_f},
    {KEY_G, GDK_g},
    {KEY_H, GDK_h},
    {KEY_I, GDK_i},
    {KEY_J, GDK_j},
    {KEY_K, GDK_k},
    {KEY_L, GDK_l},
    {KEY_M, GDK_m},
    {KEY_N, GDK_n},
    {KEY_O, GDK_o},
    {KEY_P, GDK_p},
    {KEY_Q, GDK_q},
    {KEY_R, GDK_r},
    {KEY_S, GDK_s},
    {KEY_T, GDK_t},
    {KEY_U, GDK_u},
    {KEY_V, GDK_v},
    {KEY_W, GDK_w},
    {KEY_X, GDK_x},
    {KEY_Y, GDK_y},
    {KEY_Z, GDK_z},
    {KEY_LWIN, GDK_Meta_L},
    {KEY_RWIN, GDK_Meta_R},
    {KEY_OEM_PLUS, GDK_equal},
    {KEY_OEM_MINUS, GDK_minus},
    {KEY_DECIMAL, GDK_decimalpoint},
    {KEY_DIVIDE, GDK_division},
    {KEY_F1, GDK_F1},
    {KEY_F2, GDK_F2},
    {KEY_F3, GDK_F3},
    {KEY_F4, GDK_F4},
    {KEY_F5, GDK_F5},
    {KEY_F6, GDK_F6},
    {KEY_F7, GDK_F7},
    {KEY_F8, GDK_F8},
    {KEY_F9, GDK_F9},
    {KEY_F10, GDK_F10},
    {KEY_F11, GDK_F11},
    {KEY_F12, GDK_F12},
    {KEY_F13, GDK_F13},
    {KEY_F14, GDK_F14},
    {KEY_F15, GDK_F15},
    {KEY_F16, GDK_F16},
    {KEY_F17, GDK_F17},
    {KEY_F18, GDK_F18},
    {KEY_F19, GDK_F19},
    {KEY_F20, GDK_F20},
    {KEY_F21, GDK_F21},
    {KEY_F22, GDK_F22},
    {KEY_F23, GDK_F23},
    {KEY_F24, GDK_F24},
    {KEY_LSHIFT, GDK_Shift_L},
    {KEY_RSHIFT, GDK_Shift_R},
    {KEY_LCONTROL, GDK_Control_L},
    {KEY_RCONTROL, GDK_Control_R},
};

static int get_virtual_key(int pk)
{
    int i;
    for(i = 0; i < (sizeof(key_map)/sizeof(KeyEntities)); i++)
        if (key_map[i].pk == pk)
            return key_map[i].vk;
    return KEY_UNKNOWN; 
}

