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

#include "base/timer.h"
#include "main_loop/main_loop_stm32_raw.h"
#include "base/font_manager.h"
#include "base/window_manager.h"
#include "button.h"
#include "delay.h"
#include "flash.h"
#include "gui.h"
#include "lcd/lcd_stm32_raw.h"
#include "lcd_driver.h"
#include "led.h"
#include "rtc.h"
#include "stdlib.h"
#include "sys.h"
#include "tim.h"
#include "touch.h"
#include "usart.h"

typedef struct _main_loop_stm32_raw_t {
  main_loop_t base;

  int w;
  int h;
  widget_t* wm;
  canvas_t canvas;

  xy_t touch_x;
  xy_t touch_y;
  bool_t pressed;
} main_loop_stm32_raw_t;

static main_loop_stm32_raw_t loop;

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
  uint8_t key = keyscan(0);

  /*TODO:
  main_loop_stm32_raw_dispatch_key_event(loop, &event);
  */

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
    main_loop_stm32_raw_paint(loop);
    delay_ms(100);
  }

  return RET_OK;
}

static ret_t main_loop_stm32_raw_quit(main_loop_t* l) { return RET_OK; }

static ret_t main_loop_stm32_raw_destroy(main_loop_t* l) {
  (void)l;
  return RET_OK;
}

main_loop_t* main_loop_stm32_raw_init(int w, int h) {
  lcd_t* lcd = NULL;
  widget_t* wm = default_wm();
  main_loop_t* base = &(loop.base);

  memset(&loop, 0x00, sizeof(loop));

  base->run = main_loop_stm32_raw_run;
  base->quit = main_loop_stm32_raw_quit;
  base->destroy = main_loop_stm32_raw_destroy;

  loop.wm = wm;
  window_manager_resize(wm, w, h);

  lcd = lcd_stm32_raw_create(w, h, TFT_WriteData, TFT_SetWindow);
  canvas_init(&(loop.canvas), lcd, default_fm());

  return base;
}

main_loop_t* default_main_loop() { return &loop.base; }
