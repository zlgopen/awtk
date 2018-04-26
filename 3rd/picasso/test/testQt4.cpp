
#include <stdio.h>
#include <stdlib.h>

#include <QApplication>
#include <QPainter>
#include <QWidget>
#include <QMainWindow>
#include <QImage>
#include <QTimer>

#include "picasso.h"
#include "drawFunc.h"
#include "timeuse.h"

QImage * buffer;
QImage * img1;
QImage * img2;

static ps_context *context;
static ps_canvas *canvas;

class PWindow : public QMainWindow, public QTimer
{
public:
    PWindow(QWidget* parent = 0)
        : QMainWindow(parent, 0)
        , QTimer(parent)
    {
        buffer = new QImage(640, 480, QImage::Format_RGB32);
        img1 = new QImage(QString("selt2.png"));
        img2 = new QImage(QString("pat.png"));

        canvas = ps_canvas_create_with_data(buffer->bits(), COLOR_FORMAT_BGRA,
                                                    640, 480, buffer->bytesPerLine());
        context = ps_context_create(canvas, 0);
        init_context(context, canvas, buffer->bits());

        set_image_data(img1->bits(), COLOR_FORMAT_BGRA, img1->width(),
                                                img1->height(), img1->bytesPerLine());
        set_pattern_data(img2->bits(), COLOR_FORMAT_BGRA, img2->width(),
                                                img2->height(), img2->bytesPerLine());
        start(100);
    }

    virtual ~PWindow()
    {
        dini_context(context);
        ps_context_unref(context);
        ps_canvas_unref(canvas);
        ps_shutdown();
        delete img2;
        delete img1;
        delete buffer;
    }
protected:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent * timer);
};

inline void PWindow::timerEvent(QTimerEvent * timer)
{
    timer_action(context);
    update();
}

inline void PWindow::paintEvent(QPaintEvent *event)
{
    suseconds_t t1, t2;
    QPainter painter(this);
    buffer->fill(0xFFFFFFFF);
    t1 = get_time();
    draw_test(0, context);
    t2 = get_time();
    fprintf(stderr, "draw frame use %.4f ms --- %.4f fps\n", (t2-t1)/1000.0, 1000.0/((t2-t1)/1000.0));
    QRect rect(0, 0, 640, 480);
    painter.drawImage(rect, *buffer);
}

static int __argc = 0;
static const char** __argv = NULL;

extern "C" int argc(void)
{
    return __argc;
}

extern "C" const char** argv(void)
{
    return __argv;
}

int main(int argc, char* argv[])
{
    __argc = argc;
    __argv = (const char**)argv;

    ps_initialize();

    QApplication app(argc, argv);
    PWindow *window = new PWindow;
    window->resize(640, 480);
    window->show();
    return app.exec();
}

