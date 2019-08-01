/**
 * File:   main_loop_simple.c
 * Author: AWTK Develop Team
 * Brief:  a simple main loop
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/time_now.h"
#include "main_loop/main_loop_simple.h"

static ret_t main_loop_simple_queue_event(main_loop_t* l, const event_queue_req_t* r) {
  ret_t ret = RET_FAIL;
  main_loop_simple_t* loop = (main_loop_simple_t*)l;

  tk_mutex_lock(loop->mutex);
  ret = event_queue_send(loop->queue, r);
  tk_mutex_unlock(loop->mutex);

  return ret;
}

static ret_t main_loop_simple_recv_event(main_loop_simple_t* loop, event_queue_req_t* r) {
  ret_t ret = RET_FAIL;

  tk_mutex_lock(loop->mutex);
  ret = event_queue_recv(loop->queue, r);
  tk_mutex_unlock(loop->mutex);

  return ret;
}

ret_t main_loop_post_pointer_event(main_loop_t* l, bool_t pressed, xy_t x, xy_t y) {
  event_queue_req_t r;
  pointer_event_t event;
  main_loop_simple_t* loop = (main_loop_simple_t*)l;

  return_value_if_fail(loop != NULL, RET_BAD_PARAMS);

  event.x = x;
  event.y = y;
  event.button = 0;
  event.e.time = time_now_ms();

  if (pressed) {
    loop->last_x = x;
    loop->last_y = y;

    if (loop->pressed) {
      event.e.type = EVT_POINTER_MOVE;
    } else {
      event.e.type = EVT_POINTER_DOWN;
    }
    loop->pressed = TRUE;
    event.pressed = loop->pressed;

    r.pointer_event = event;

    return main_loop_queue_event(l, &r);
  } else {
    if (loop->pressed) {
      event.e.type = EVT_POINTER_UP;
      event.pressed = loop->pressed;
      event.x = loop->last_x;
      event.y = loop->last_y;
      r.pointer_event = event;

      loop->pressed = FALSE;

      return main_loop_queue_event(l, &r);
    }
  }

  return RET_OK;
}

ret_t main_loop_post_key_event(main_loop_t* l, bool_t pressed, uint8_t key) {
  event_queue_req_t r;
  key_event_t event;
  main_loop_simple_t* loop = (main_loop_simple_t*)l;
  return_value_if_fail(loop != NULL, RET_BAD_PARAMS);

  loop->last_key = key;
  event.key = key;

  if (pressed) {
    loop->key_pressed = TRUE;
    event.e.type = EVT_KEY_DOWN;

    r.key_event = event;
    return main_loop_queue_event(l, &r);
  } else {
    if (loop->key_pressed) {
      loop->key_pressed = FALSE;
      event.e.type = EVT_KEY_UP;

      r.key_event = event;
      return main_loop_queue_event(l, &r);
    }
  }

  return RET_OK;
}

static ret_t main_loop_dispatch_events(main_loop_simple_t* loop) {
  event_queue_req_t r;
  widget_t* widget = loop->base.wm;

  while (main_loop_simple_recv_event(loop, &r) == RET_OK) {
    switch (r.event.type) {
      case EVT_POINTER_DOWN:
      case EVT_POINTER_MOVE:
      case EVT_POINTER_UP:
        window_manager_dispatch_input_event(widget, (event_t*)&(r.pointer_event));
        break;
      case EVT_KEY_DOWN:
      case EVT_KEY_UP:
        window_manager_dispatch_input_event(widget, (event_t*)&(r.key_event));
        break;
      case REQ_ADD_IDLE:
        idle_add(r.add_idle.func, r.add_idle.e.target);
        break;
      case REQ_ADD_TIMER:
        timer_add(r.add_timer.func, r.add_timer.e.target, r.add_timer.duration);
        break;
      default:
        break;
    }
    /*HANDLE OTHER EVENT*/
  }

  return RET_OK;
}

static ret_t main_loop_dispatch_input(main_loop_simple_t* loop) {
  if (loop->dispatch_input) {
    loop->dispatch_input(loop);
  }

  return RET_OK;
}

static ret_t main_loop_simple_step(main_loop_t* l) {
  main_loop_simple_t* loop = (main_loop_simple_t*)l;

  timer_dispatch();
  main_loop_dispatch_input(loop);
  main_loop_dispatch_events(loop);
  idle_dispatch();

  window_manager_paint(loop->base.wm);

  return RET_OK;
}

static ret_t main_loop_simple_run(main_loop_t* l) {
  main_loop_simple_t* loop = (main_loop_simple_t*)l;

  loop->pressed = FALSE;
  while (l->running) {
    main_loop_step(l);
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
  loop->base.wm = window_manager();
  return_value_if_fail(loop->base.wm != NULL, NULL);

  loop->queue = event_queue_create(20);
  return_value_if_fail(loop->queue != NULL, NULL);

  loop->mutex = tk_mutex_create();
  return_value_if_fail(loop->mutex != NULL, NULL);

  loop->base.run = main_loop_simple_run;
  loop->base.step = main_loop_simple_step;
  loop->base.queue_event = main_loop_simple_queue_event;

  window_manager_post_init(loop->base.wm, w, h);
  main_loop_set((main_loop_t*)loop);

  return loop;
}

ret_t main_loop_simple_reset(main_loop_simple_t* loop) {
  return_value_if_fail(loop != NULL, RET_BAD_PARAMS);
  event_queue_destroy(loop->queue);
  tk_mutex_destroy(loop->mutex);

  memset(loop, 0x00, sizeof(main_loop_simple_t));

  return RET_OK;
}
