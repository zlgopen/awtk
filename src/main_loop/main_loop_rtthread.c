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
  rt_err_t result;
  struct rtgui_event *event;

  result = rtgui_recv(event, sizeof(union rtgui_event_generic), 100);
  if(result == RT_EOK) {
    /*TODO*/
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

main_loop_t* main_loop_rtthread_init(int w, int h) {
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
  window_manager_resize(wm, driver->width, driver->height);

  lcd = lcd_rtthread_init(driver);
  canvas_init(&(loop.canvas), lcd, default_fm());

  (void)w;
  (void)h;

  return base;
}

main_loop_t* default_main_loop() { return &loop.base; }

