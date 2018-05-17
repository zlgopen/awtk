/**
 * File:   main_loop_simple.c
 * Author: AWTK Develop Team
 * Brief:  a simple main loop
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-05-17 li xianjing <xianjimli@hotmail.com> created
 *
 */

#include "main_loop/main_loop_simple.h"

ret_t main_loop_post_pointer_event(main_loop_t* l, bool_t pressed, xy_t x, xy_t y) {
  event_all_t any;
  pointer_event_t event;
  main_loop_simple_t* loop = (main_loop_simple_t*)l;

  return_value_if_fail(loop != NULL, RET_BAD_PARAMS);

  loop->last_x = x;
  loop->last_y = y;

  event.x = x;
  event.y = y;
  event.button = 0;

  if (pressed) {
    if (loop->pressed) {
      event.e.type = EVT_POINTER_MOVE;
    } else {
      event.e.type = EVT_POINTER_DOWN;
    }
    loop->pressed = TRUE;
    event.pressed = loop->pressed;

    any.e.pointer_event = event;
    event_queue_send(loop->queue, &any);
  } else {
    if (loop->pressed) {
      loop->pressed = FALSE;
      event.e.type = EVT_POINTER_UP;
      event.pressed = loop->pressed;

      any.e.pointer_event = event;
      event_queue_send(loop->queue, &any);
    }
  }

  return RET_OK;
}

ret_t main_loop_post_key_event(main_loop_t* l, bool_t pressed, uint8_t key) {
  event_all_t any;
  key_event_t event;
  main_loop_simple_t* loop = (main_loop_simple_t*)l;
  return_value_if_fail(loop != NULL, RET_BAD_PARAMS);

  loop->last_key = key;
  event.key = key;

  if (pressed) {
    loop->key_pressed = TRUE;
    event.e.type = EVT_KEY_DOWN;

    any.e.key_event = event;
    event_queue_send(loop->queue, &any);
  } else {
    if (loop->key_pressed) {
      loop->key_pressed = FALSE;
      event.e.type = EVT_KEY_UP;

      any.e.key_event = event;
      event_queue_send(loop->queue, &any);
    }
  }

  return RET_OK;
}


static ret_t main_loop_dispatch_events(main_loop_simple_t* loop) {
  event_all_t event;
  widget_t* widget = loop->wm;

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
    /*HANDLE OTHER EVENT*/
  }

  return RET_OK;
}

static ret_t main_loop_dispatch_input(main_loop_simple_t* loop) {
  if(loop->dispatch_input) {
    loop->dispatch_input(loop);
  }

  return RET_OK;
}

static ret_t main_loop_simple_run(main_loop_t* l) {
  main_loop_simple_t* loop = (main_loop_simple_t*)l;

  while (l->running) {
    timer_dispatch();
    main_loop_dispatch_input(loop);
    main_loop_dispatch_events(loop);
    idle_dispatch();

    window_manager_paint(loop->wm, &(loop->canvas));
    main_loop_sleep(l);
  }

  return RET_OK;
}

main_loop_simple_t* main_loop_simple_init(int w, int h) {
  static main_loop_simple_t s_main_loop_simple;
  main_loop_simple_t* loop = &s_main_loop_simple;
  
  memset(loop, 0x00, sizeof(main_loop_simple_t));

  loop->w = w;
  loop->h = h;
  loop->wm = window_manager();
  return_value_if_fail(loop->wm != NULL, NULL);

  loop->queue = event_queue_create(20);
  return_value_if_fail(loop->queue != NULL, NULL);

  loop->base.run = main_loop_simple_run;
  window_manager_resize(loop->wm, w, h);
  main_loop_set((main_loop_t*)loop);

  return loop;
}

ret_t main_loop_simple_reset(main_loop_simple_t* loop) {
  return_value_if_fail(loop != NULL, RET_BAD_PARAMS);
  event_queue_destroy(loop->queue);
  memset(loop, 0x00, sizeof(main_loop_simple_t));

  return RET_OK;
}
