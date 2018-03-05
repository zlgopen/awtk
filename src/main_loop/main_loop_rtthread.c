/**
 * file:   main_loop_rtthread.c
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  rtthread implemented main_loop interface
 *
 * copyright (c) 2018 - 2018  li xianjing <xianjimli@hotmail.com>
 *
 * this program is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * license file for more details.
 *
 */

/**
 * history:
 * ================================================================
 * 2018-02-17 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "base/timer.h"
#include "rtgui/event.h"
#include "lcd/lcd_rtthread.h"
#include <rtgui/widgets/window.h>
#include <rtgui/event.h>
#include <rtgui/rtgui_app.h>
#include <rtgui/rtgui_system.h>
#include "base/font_manager.h"
#include "base/window_manager.h"
#include "main_loop/main_loop_rtthread.h"

typedef struct _main_loop_rtthread_t {
  main_loop_t base;

  int w;
  int h;
  widget_t* wm;
  canvas_t canvas;

  xy_t touch_x;
  xy_t touch_y;
  bool_t pressed;
  struct rtgui_app* app;
  struct rtgui_win* main_win;
} main_loop_rtthread_t;

static main_loop_rtthread_t loop;

static void dispatch_touch_events(bool_t pressed, xy_t x, xy_t y) {
  pointer_event_t event;
  widget_t* widget = loop.wm;

  loop.touch_x = x;
  loop.touch_y = y;

  event.button = 0;
  event.x = loop.touch_x;
  event.y = loop.touch_y;

  if (pressed) {
    if (loop.pressed) {
      event.e.type = EVT_POINTER_MOVE;
    } else {
      event.e.type = EVT_POINTER_DOWN;
    }
    loop.pressed = TRUE;
    event.pressed = loop.pressed;

    widget_on_pointer_down(widget, &event);
  } else {
    if (loop.pressed) {
      loop.pressed = FALSE;
      event.e.type = EVT_POINTER_UP;
      event.pressed = loop.pressed;
      widget_on_pointer_up(widget, &event);
    }
  }
}

static ret_t main_loop_rtthread_dispatch(main_loop_rtthread_t* loop) {
  rt_err_t result = 0;
  widget_t* widget = loop->wm;
  struct rtgui_event* rt_event = (struct rtgui_event*)(loop->app->event_buffer);

  result = rtgui_recv(rt_event, sizeof(union rtgui_event_generic), 100);
  if (result == RT_EOK) {
    uint32_t type = rt_event->type;
    switch (type) {
      case RTGUI_EVENT_MOUSE_BUTTON: {
        pointer_event_t event;
        struct rtgui_event_mouse* e = (struct rtgui_event_mouse*)rt_event;
        event.x = e->x;
        event.y = e->y;
        event.button = e->button;
        if (e->button & RTGUI_MOUSE_BUTTON_DOWN) {
          printf("type=%d down\n", type);
          loop->pressed = 1;
          event.e.type = EVT_POINTER_DOWN;
          event.pressed = loop->pressed;

          widget_on_pointer_down(widget, &event);
        } else {
          printf("type=%d up\n", type);
          event.e.type = EVT_POINTER_UP;
          event.pressed = loop->pressed;

          widget_on_pointer_up(widget, &event);
          loop->pressed = 0;
        }
        break;
      }
      case RTGUI_EVENT_MOUSE_MOTION: {
        struct rtgui_event_mouse* e = (struct rtgui_event_mouse*)rt_event;
        break;
      }
      case RTGUI_EVENT_KBD: {
        struct rtgui_event_kbd* e = (struct rtgui_event_kbd*)rt_event;
        break;
      }
      case RTGUI_EVENT_PAINT: {
        struct rtgui_event_paint* e = (struct rtgui_event_paint*)rt_event;
        break;
      }
      default:
        break;
    }
  }

  return RET_OK;
}

static ret_t main_loop_rtthread_paint(main_loop_rtthread_t* loop) {
  canvas_t* c = &(loop->canvas);

  return window_manager_paint(loop->wm, c);
}

static ret_t main_loop_rtthread_run(main_loop_t* l) {
  main_loop_rtthread_t* loop = (main_loop_rtthread_t*)l;

  while (l->running) {
    timer_check();
    main_loop_rtthread_dispatch(loop);
    main_loop_rtthread_paint(loop);
  }

  return RET_OK;
}

static ret_t main_loop_rtthread_quit(main_loop_t* l) { return RET_OK; }

static ret_t main_loop_rtthread_destroy(main_loop_t* l) {
  (void)l;
  return RET_OK;
}

rt_bool_t on_event(struct rtgui_object* object, rtgui_event_t* event) {
  uint32_t type = event->type;

  printf("type=%d\n", type);

  return FALSE;
}

main_loop_t* main_loop_init(int w, int h) {
  lcd_t* lcd = NULL;
  widget_t* wm = default_wm();
  main_loop_t* base = &(loop.base);
  struct rtgui_graphic_driver* driver = rtgui_graphic_driver_get_default();
  return_value_if_fail(driver != NULL, NULL);

  memset(&loop, 0x00, sizeof(loop));

  base->run = main_loop_rtthread_run;
  base->quit = main_loop_rtthread_quit;
  base->destroy = main_loop_rtthread_destroy;

  loop.wm = wm;
  loop.app = rtgui_app_create("gui_demo");
  window_manager_resize(wm, driver->width, driver->height);

  lcd = lcd_rtthread_init(driver);
  canvas_init(&(loop.canvas), lcd, default_fm());

  // DebugBreak();
  loop.main_win = rtgui_mainwin_create(RT_NULL, "UiWindow", RTGUI_WIN_STYLE_NO_TITLE);
  rtgui_object_set_event_handler(RTGUI_OBJECT(&loop), on_event);
  rtgui_win_show(loop.main_win, RT_FALSE);
  main_loop_set_default(base);

  (void)w;
  (void)h;

  return base;
}

