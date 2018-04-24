/* platform - application framework base on picasso 
 * 
 * Copyright (C) 2009 Zhang Ji Peng
 * Contact: onecoolx@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QApplication>
#include <QPainter>
#include <QSize>
#include <QResizeEvent>
#include <QWidget>
#include <QMainWindow>
#include <QImage>
#include <QTimer>

#include "picasso.h"
#include "interface.h"

static int g_width;
static int g_height;

static ps_context *context;
static ps_canvas *canvas;
static ps_color_format fmt;    

static int get_virtual_key(int pk);

class PTimer : public QTimer
{
public:
    PTimer(QWidget* parent = 0)
        : QTimer(parent)
    {
    }

protected:
    void timerEvent(QTimerEvent * timer);
};

class PWindow : public QMainWindow
{
public:
    PWindow(QWidget* parent = 0)
        : QMainWindow(parent, 0)
    {
        fmt = COLOR_FORMAT_BGRA;
        image = QImage(g_width, g_height, QImage::Format_RGB32);
        canvas = ps_canvas_create_with_data(image.bits(), fmt, g_width, g_height, image.bytesPerLine());
        context = ps_context_create(canvas, 0);
        on_init(context, g_width, g_height);
    }

    virtual ~PWindow()
    {
        on_term(context);
        ps_context_unref(context);
        ps_canvas_unref(canvas);
        ps_shutdown();
    }

protected:
    void resizeEvent(QResizeEvent * event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent * event);
    void paintEvent(QPaintEvent *event);
private:
    QImage image;
};

inline void PWindow::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    ps_canvas* old_canvas = 0;
    g_width = event->size().width();
    g_height = event->size().height();
    if (g_width < 1)
        g_width = 1;
    if (g_height < 1)
        g_height = 1;

    image = QImage(g_width, g_height, QImage::Format_RGB32);
    canvas = ps_canvas_create_with_data(image.bits(), fmt, g_width, g_height, image.bytesPerLine());
    old_canvas = ps_context_set_canvas(context, canvas);
    ps_canvas_unref(old_canvas);
    on_size(g_width, g_height);
}

inline void PWindow::keyPressEvent(QKeyEvent * event)
{
    on_key_event(KEY_EVENT_DOWN, get_virtual_key(event->key()));
}

inline void PWindow::mousePressEvent(QMouseEvent *event)
{
    mouse_event_type type;
    unsigned int key;
    switch(event->button()){
        case Qt::LeftButton:
            type = LEFT_BUTTON_DOWN;
            key = EVT_LBUTTON;
            break;
        case Qt::MidButton:
            type = MIDDLE_BUTTON_DOWN;
            key = EVT_MBUTTON;
            break;
        case Qt::RightButton:
            type = RIGHT_BUTTON_DOWN;
            key = EVT_RBUTTON;
            break;
    }
    if (event->buttons() & Qt::SHIFT)
        key |= EVT_SHIFT;
    if (event->buttons() & Qt::CTRL)
        key |= EVT_CONTROL;
    on_mouse_event(type, key, event->x(), event->y());
}

inline void PWindow::mouseMoveEvent(QMouseEvent *event)
{
    unsigned int key;
    if (event->buttons() & Qt::SHIFT)
        key |= EVT_SHIFT;
    if (event->buttons() & Qt::CTRL)
        key |= EVT_CONTROL;
    if (event->buttons() & Qt::LeftButton)
        key |= EVT_LBUTTON;
    if (event->buttons() & Qt::MidButton)
        key |= EVT_MBUTTON;
    if (event->buttons() & Qt::RightButton)
        key |= EVT_RBUTTON;
    on_mouse_event(MOUSE_MOVE, key, event->x(), event->y());
}

inline void PTimer::timerEvent(QTimerEvent * timer)
{
    on_timer();
}

inline void PWindow::paintEvent(QPaintEvent *event)
{
    on_draw(context);
    QPainter painter(this);
    QRect rect(0, 0, g_width, g_height);
    painter.drawImage(rect, image);
}

static PWindow *window = 0;

int main(int argc, char* argv[])
{
    g_width = 640;
    g_height = 480;
    ps_initialize();
    QApplication app(argc, argv);
    window = new PWindow;
    window->setWindowTitle("Picasso Demos");
    window->resize(g_width, g_height);
    window->show();
    return app.exec();
}

extern "C" void refresh(const ps_rect* r)
{
    if (r) {
        window->update(r->x, r->y, r->w, r->h);
    } else {
        window->update();
    }
}

extern "C" unsigned set_timer(unsigned mc)
{
    PTimer * timer = new PTimer;
    timer->start(mc);
    return reinterpret_cast<unsigned>(timer);
}

extern "C" void clear_timer(unsigned id)
{
    PTimer * timer = reinterpret_cast<PTimer*>(id);
    timer->stop();
    delete timer;
}

extern "C" picture* load_picture(const char* name)
{
    QImage * nb;
    picture* pic;
    ps_image * img;
    char* pname;

    if (!name || !strlen(name))
        return NULL;
    pname = (char*)malloc(strlen(name)+5);
    sprintf(pname, "%s.png", name);

    nb = new QImage;
    nb->load(QString(pname));
    free(pname);
    img = ps_image_create_with_data(nb->bits(), fmt, nb->width(), nb->height(), nb->bytesPerLine());

    pic = (picture*)malloc(sizeof(picture));
    pic->width = nb->width();
    pic->height = nb->height();
    pic->native = static_cast<void*>(nb);
    pic->image = img;

    return pic;
}

extern "C" void free_picture(picture* p)
{
    if (!p)
        return;

    ps_image_unref(p->image);
    delete static_cast<QImage*>(p->native);
    free(p);
}


typedef struct {
    int vk;
    int pk;
}KeyEntities;

static KeyEntities key_map[] = {
    {KEY_BACK, Qt::Key_Backspace},
    {KEY_TAB, Qt::Key_Tab},
    {KEY_CLEAR, Qt::Key_Clear},
    {KEY_RETURN, Qt::Key_Return},
    {KEY_SHIFT, Qt::Key_Shift},
    {KEY_CONTROL, Qt::Key_Control},
    {KEY_MENU, Qt::Key_Menu},
    {KEY_PAUSE, Qt::Key_Pause},
    {KEY_CAPITAL, Qt::Key_CapsLock},
    {KEY_ESCAPE, Qt::Key_Escape},
    {KEY_SPACE, Qt::Key_Space},
    {KEY_PRIOR, Qt::Key_PageUp},
    {KEY_NEXT, Qt::Key_PageDown},
    {KEY_END, Qt::Key_End},
    {KEY_HOME, Qt::Key_Home},
    {KEY_LEFT, Qt::Key_Left},
    {KEY_UP, Qt::Key_Up},
    {KEY_RIGHT, Qt::Key_Right},
    {KEY_DOWN, Qt::Key_Down},
    {KEY_SELECT, Qt::Key_Select},
    {KEY_PRINT, Qt::Key_Print},
    {KEY_EXEC, Qt::Key_Execute},
    {KEY_INSERT, Qt::Key_Insert},
    {KEY_DELETE, Qt::Key_Delete},
    {KEY_HELP, Qt::Key_Help},
    {KEY_0, Qt::Key_0},
    {KEY_1, Qt::Key_1},
    {KEY_2, Qt::Key_2},
    {KEY_3, Qt::Key_3},
    {KEY_4, Qt::Key_4},
    {KEY_5, Qt::Key_5},
    {KEY_6, Qt::Key_6},
    {KEY_7, Qt::Key_7},
    {KEY_8, Qt::Key_8},
    {KEY_9, Qt::Key_9},
    {KEY_A, Qt::Key_A},
    {KEY_B, Qt::Key_B},
    {KEY_C, Qt::Key_C},
    {KEY_D, Qt::Key_D},
    {KEY_E, Qt::Key_E},
    {KEY_F, Qt::Key_F},
    {KEY_G, Qt::Key_G},
    {KEY_H, Qt::Key_H},
    {KEY_I, Qt::Key_I},
    {KEY_J, Qt::Key_J},
    {KEY_K, Qt::Key_K},
    {KEY_L, Qt::Key_L},
    {KEY_M, Qt::Key_M},
    {KEY_N, Qt::Key_N},
    {KEY_O, Qt::Key_O},
    {KEY_P, Qt::Key_P},
    {KEY_Q, Qt::Key_Q},
    {KEY_R, Qt::Key_R},
    {KEY_S, Qt::Key_S},
    {KEY_T, Qt::Key_T},
    {KEY_U, Qt::Key_U},
    {KEY_V, Qt::Key_V},
    {KEY_W, Qt::Key_W},
    {KEY_X, Qt::Key_X},
    {KEY_Y, Qt::Key_Y},
    {KEY_Z, Qt::Key_Z},
    {KEY_LWIN, Qt::Key_Meta},
    {KEY_RWIN, Qt::Key_Meta},
    {KEY_OEM_PLUS, Qt::Key_Equal},
    {KEY_OEM_MINUS, Qt::Key_Minus},
    {KEY_DECIMAL, Qt::Key_Greater},
    {KEY_DIVIDE, Qt::Key_Question},
    {KEY_F1, Qt::Key_F1},
    {KEY_F2, Qt::Key_F2},
    {KEY_F3, Qt::Key_F3},
    {KEY_F4, Qt::Key_F4},
    {KEY_F5, Qt::Key_F5},
    {KEY_F6, Qt::Key_F6},
    {KEY_F7, Qt::Key_F7},
    {KEY_F8, Qt::Key_F8},
    {KEY_F9, Qt::Key_F9},
    {KEY_F10, Qt::Key_F10},
    {KEY_F11, Qt::Key_F11},
    {KEY_F12, Qt::Key_F12},
    {KEY_F13, Qt::Key_F13},
    {KEY_F14, Qt::Key_F14},
    {KEY_F15, Qt::Key_F15},
    {KEY_F16, Qt::Key_F16},
    {KEY_F17, Qt::Key_F17},
    {KEY_F18, Qt::Key_F18},
    {KEY_F19, Qt::Key_F19},
    {KEY_F20, Qt::Key_F20},
    {KEY_F21, Qt::Key_F21},
    {KEY_F22, Qt::Key_F22},
    {KEY_F23, Qt::Key_F23},
    {KEY_F24, Qt::Key_F24},
};

static int get_virtual_key(int pk)
{
    int i;
    for(i = 0; i < (sizeof(key_map)/sizeof(KeyEntities)); i++)
        if (key_map[i].pk == pk)
            return key_map[i].vk;
    return KEY_UNKNOWN; 
}
