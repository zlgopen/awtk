
#include <stdio.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#include "picasso.h"
#include "drawFunc.h"
#include "timeuse.h"


GdkPixbuf * pixbuf;
GdkPixbuf * pixa;
GdkPixbuf * pixb;


static ps_context *context;
static ps_canvas *canvas;


gboolean expose (GtkWidget *widget, GdkEventExpose *event)
{
    suseconds_t t1, t2;
    gdk_pixbuf_fill(pixbuf, 0xFFFFFFFF);
    t1 = get_time();
    draw_test(0, context);
    t2 = get_time();
    fprintf(stderr, "draw frame use %.4f ms --- %.4f fps\n", (t2-t1)/1000.0, 1000.0/((t2-t1)/1000.0));
    gdk_draw_pixbuf(widget->window, widget->style->white_gc, pixbuf, 0, 0, 0, 0, 640, 480, 0,0,0);
    return FALSE;
}

void destroy(GtkWidget *widget, gpointer data)
{
    dini_context(context);
    ps_context_unref(context);
    ps_canvas_unref(canvas);
    ps_shutdown();
    g_object_unref(pixbuf);
    gtk_main_quit();
}

gboolean time_func(gpointer data)
{
    timer_action(context);
    GtkWidget * p = GTK_WIDGET(data);
    gtk_widget_queue_draw(p);
    return 1;
}

static void init_pixbuf()
{
    GError * e = 0;
    pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, FALSE, 8, 640, 480);
    pixa = gdk_pixbuf_new_from_file("selt2.png", &e); 
    pixb = gdk_pixbuf_new_from_file("pat.png", &e); 

    guchar* buf = gdk_pixbuf_get_pixels(pixbuf);
    canvas = ps_canvas_create_with_data((ps_byte*)buf, COLOR_FORMAT_RGB, 640, 480, 640*3);
    context = ps_context_create(canvas, 0);
    init_context(context, canvas, buf);    

    guchar* bufa = gdk_pixbuf_get_pixels(pixa);
    guchar* bufb = gdk_pixbuf_get_pixels(pixb);

    set_image_data(bufa, COLOR_FORMAT_RGB, gdk_pixbuf_get_width(pixa),
                                 gdk_pixbuf_get_height(pixa), gdk_pixbuf_get_rowstride(pixa));
    set_pattern_data(bufb, COLOR_FORMAT_RGB, gdk_pixbuf_get_width(pixb), 
                                 gdk_pixbuf_get_height(pixb), gdk_pixbuf_get_rowstride(pixb));
}

static int __argc = 0;
static const char** __argv = NULL;

int argc(void)
{
    return __argc;
}

const char** argv(void)
{
    return __argv;
}

int main(int argc, char* argv[])
{
    GtkWidget *window;
    GtkWidget *drawarea;

    __argc = argc;
    __argv = argv;

    gtk_init(&argc, &argv);

    ps_initialize();

    init_pixbuf();

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    drawarea = gtk_drawing_area_new();

    gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);

    g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK (destroy), NULL);

    g_signal_connect (G_OBJECT(drawarea), "expose_event", G_CALLBACK (expose), NULL);

    g_timeout_add(100, time_func, GTK_WIDGET(drawarea));

    gtk_container_add (GTK_CONTAINER (window), drawarea);

    gtk_widget_show(drawarea);
    gtk_widget_show(window);

    gtk_main();

    return 0;
}

