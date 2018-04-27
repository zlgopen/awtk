/**
 * file:   main_loop_stm32_raw.c
 * author: li xianjing <xianjimli@hotmail.com>
 * brief:  stm32_raw implemented main_loop interface
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

#include "button.h"
#include "delay.h"
#include "flash.h"
#include "gui.h"
#include "lcd_driver.h"
#include "led.h"
#include "rtc.h"
#include "stdlib.h"
#include "sys.h"
#include "tim.h"
#include "touch.h"
#include "usart.h"

#include "base/idle.h"
#include "base/timer.h"
#include "lcd/lcd_reg.h"
#include "base/event_queue.h"
#include "base/font_manager.h"
#include "base/window_manager.h"
#include "main_loop/main_loop_stm32_raw.h"

typedef struct _main_loop_stm32_raw_t {
  main_loop_t base;

  int w;
  int h;
  widget_t* wm;
  canvas_t canvas;

  xy_t touch_x;
  xy_t touch_y;
  bool_t pressed;
  event_queue_t* queue;
} main_loop_stm32_raw_t;

static main_loop_stm32_raw_t loop;

static void dispatch_touch_events(bool_t pressed, xy_t x, xy_t y) {
  event_all_t any;
  pointer_event_t event;

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

    any.e.pointer_event = event;
    event_queue_send(loop.queue, &any);
  } else {
    if (loop.pressed) {
      loop.pressed = FALSE;
      event.e.type = EVT_POINTER_UP;
      event.pressed = loop.pressed;

      any.e.pointer_event = event;
      event_queue_send(loop.queue, &any);
    }
  }
}

void TIM3_IRQHandler(void) {
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    if (TOUCH_Scan() == 0) {
      dispatch_touch_events(TRUE, TouchData.lcdx, TouchData.lcdy);
    } else {
      dispatch_touch_events(FALSE, TouchData.lcdx, TouchData.lcdy);
    }
  }
}

static ret_t main_loop_stm32_raw_dispatch(main_loop_stm32_raw_t* loop) {
  event_all_t event;
  widget_t* widget = loop->wm;
  uint8_t key = keyscan(0);
  /*TODO dispatch key*/
  while (event_queue_recv(loop->queue, &event) == RET_OK) {
    switch (event.e.event.type) {
      case EVT_POINTER_DOWN:
        window_manager_dispatch_input_event(widget, (event_t*)&(event.e.pointer_event));
        break;
      case EVT_POINTER_MOVE:
        window_manager_dispatch_input_event(widget, (event_t*)&(event.e.pointer_event));
        break;
      case EVT_POINTER_UP:
        window_manager_dispatch_input_event(widget, (event_t*)&(event.e.pointer_event));
        break;
      default:
        break;
    }
  }

  return RET_OK;
}

static ret_t main_loop_stm32_raw_paint(main_loop_stm32_raw_t* loop) {
  canvas_t* c = &(loop->canvas);

  // return canvas_test_paint(c, loop->pressed, loop->touch_x, loop->touch_y);
  return window_manager_paint(loop->wm, c);
}

static ret_t main_loop_stm32_raw_run(main_loop_t* l) {
  main_loop_stm32_raw_t* loop = (main_loop_stm32_raw_t*)l;

  while (l->running) {
    timer_check();
    main_loop_stm32_raw_dispatch(loop);
    idle_dispatch();

    main_loop_stm32_raw_paint(loop);

    delay_ms(100);
  }

  return RET_OK;
}

static ret_t main_loop_stm32_raw_quit(main_loop_t* l) { return RET_OK; }

static ret_t main_loop_stm32_raw_destroy(main_loop_t* l) {
  main_loop_stm32_raw_t* loop = (main_loop_stm32_raw_t*)l;
  event_queue_destroy(loop->queue);

  return RET_OK;
}

main_loop_t* main_loop_init(int w, int h) {
  lcd_t* lcd = NULL;
  event_queue_t* queue = NULL;
  widget_t* wm = window_manager();
  main_loop_t* base = &(loop.base);
  queue = event_queue_create(20);
  return_value_if_fail(queue != NULL, NULL);

  memset(&loop, 0x00, sizeof(loop));

  base->run = main_loop_stm32_raw_run;
  base->quit = main_loop_stm32_raw_quit;
  base->destroy = main_loop_stm32_raw_destroy;

  loop.wm = wm;
  loop.queue = queue;
  window_manager_resize(wm, w, h);

  lcd = lcd_reg_create(w, h);
  canvas_init(&(loop.canvas), lcd, font_manager());
  main_loop_set_default(base);

  return base;
}
