#include <jni.h>
#include <errno.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "picasso.h"
#include "drawFunc.h"
#include "timeuse.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

/**
 * Inline image data
 */
#include "pat565.h"
#include "selt2565.h"


/**
 * Shared state for our app.
 */
struct engine {
    struct android_app* app;

    int32_t width;
    int32_t height;
    int32_t bpp;
    ps_color_format fmt;    

    ps_context *context;
    ps_canvas *canvas;
};

/**
 * Initialize an context for the current display.
 */
static int engine_init_display(struct engine* engine)
{
    int w, h, f, s, b;
    ps_color_format fmt;    

    ps_initialize();

    w = ANativeWindow_getWidth(engine->app->window);
    h = ANativeWindow_getHeight(engine->app->window);
    f = ANativeWindow_getFormat(engine->app->window);

    
    if (f == WINDOW_FORMAT_RGBA_8888 || f == WINDOW_FORMAT_RGBX_8888) {
        b = 4;
        fmt = COLOR_FORMAT_ARGB;
    } else {
        b = 2;
        fmt = COLOR_FORMAT_RGB565;
    }

    ANativeWindow_Buffer buffer;
    ANativeWindow_lock(engine->app->window, &buffer, 0);
    s = buffer.stride * b;
    // create canvas with fake buffer bits !
    engine->canvas = ps_canvas_create_with_data((uint8_t*)buffer.bits, fmt, w, h, s);
    engine->context = ps_context_create(engine->canvas, 0);
    init_context(engine->context, engine->canvas, (unsigned char*)buffer.bits);    
    set_image_data(selt2565.bits, COLOR_FORMAT_RGB565, selt2565.width, selt2565.height, selt2565.pitch);
    set_pattern_data(pat565.bits, COLOR_FORMAT_RGB565, pat565.width, pat565.height, pat565.pitch);
   // ps_scale(engine->context, 4, 4); // zoom 

    engine->width = w;
    engine->height = h;
    engine->bpp = b;
    engine->fmt = fmt;
    ANativeWindow_unlockAndPost(engine->app->window);

    return 0;

}

/**
 * Just the current frame in the display.
 */
static void engine_draw_frame(struct engine* engine)
{
    if (engine->app->window != NULL) {
        suseconds_t t1, t2;

        ANativeWindow_Buffer buffer;
        ANativeWindow_lock(engine->app->window, &buffer, 0);
        engine->canvas = ps_canvas_replace_data(engine->canvas, 
                (uint8_t*)buffer.bits, engine->fmt, engine->width, engine->height, buffer.stride * engine->bpp); 

        memset((uint8_t*)buffer.bits, 0xFF, engine->height * buffer.stride * engine->bpp);
        t1 = get_time();
        draw_test(0, engine->context);
        t2 = get_time();

        LOGI("draw a frame use %.4f ms", ((double)t2 - (double)t1)/1000);
        ANativeWindow_unlockAndPost(engine->app->window);
    }
}

/**
 * Tear down the context currently associated with the display.
 */
static void engine_term_display(struct engine* engine)
{
    dini_context(engine->context);
    ps_context_unref(engine->context);
    ps_canvas_unref(engine->canvas);
    ps_shutdown();
}

/**
 * Process the next input event.
 */
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        // to some thing.


        return 1;

    }

    return 0;
}

/**
 * Process the next main command.
 */
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    struct engine* engine = (struct engine*)app->userData;

    switch (cmd) {
        case APP_CMD_INIT_WINDOW:

            // The window is being shown, get it ready.

            if (engine->app->window != NULL) {

                engine_init_display(engine);

                engine_draw_frame(engine);

            }

            break;

        case APP_CMD_TERM_WINDOW:

            // The window is being hidden or closed, clean it up.

            engine_term_display(engine);

            break;

    }

}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state)
{
    int times = 0;
    struct engine engine;

    // Make sure glue isn't stripped.

    app_dummy();


    memset(&engine, 0, sizeof(engine));

    state->userData = &engine;

    state->onAppCmd = engine_handle_cmd;

    state->onInputEvent = engine_handle_input;

    engine.app = state;

    // loop waiting for stuff to do.


    while (1) {

        // Read all pending events.

        int ident;

        int events;
        
        struct android_poll_source* source;


        while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) {
            // Process this event.

            if (source != NULL) {

                source->process(state, source);

            }


            // Check if we are exiting.

            if (state->destroyRequested != 0) {

                engine_term_display(&engine);

                return;

            }

        }

        times++;
        if (times == 10) {
            times = 0;
            timer_action(engine.context);
        }
        engine_draw_frame(&engine);

    }

}

