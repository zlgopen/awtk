/**
 * File:   main_loop_console.c
 * Author: AWTK Develop Team
 * Brief:  a console main loop
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-06-30 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "tkc/time_now.h"
#include "main_loop_console.h"

#include "tkc/event_source_idle.h"
#include "tkc/event_source_timer.h"
#include "tkc/event_source_manager_default.h"

static ret_t main_loop_console_queue_event_mutex(main_loop_t* l, const event_queue_req_t* r) {
  ret_t ret = RET_FAIL;
  main_loop_console_t* loop = (main_loop_console_t*)l;

  tk_mutex_lock(loop->mutex);
  ret = event_queue_send(loop->queue, r);
  tk_mutex_unlock(loop->mutex);

  return ret;
}

static ret_t main_loop_console_recv_event_mutex(main_loop_t* l, event_queue_req_t* r) {
  ret_t ret = RET_FAIL;
  main_loop_console_t* loop = (main_loop_console_t*)l;

  tk_mutex_lock(loop->mutex);
  ret = event_queue_recv(loop->queue, r);
  tk_mutex_unlock(loop->mutex);

  return ret;
}

static ret_t main_loop_dispatch_events(main_loop_console_t* loop) {
  event_queue_req_t r;
  int time_in = time_now_ms();
  int time_out = time_in;

  while ((time_out - time_in < 20) && (main_loop_recv_event((main_loop_t*)loop, &r) == RET_OK)) {
    switch (r.event.type) {
      case REQ_ADD_IDLE: {
        uint32_t id = idle_add(r.add_idle.func, r.add_idle.e.target);
        if (id != TK_INVALID_ID && r.add_idle.on_destroy != NULL) {
          idle_set_on_destroy(id, r.add_idle.on_destroy, r.add_idle.on_destroy_ctx);
        }
      } break;
      case REQ_ADD_TIMER: {
        uint32_t id = timer_add(r.add_timer.func, r.add_timer.e.target, r.add_timer.duration);
        if (id != TK_INVALID_ID && r.add_timer.on_destroy != NULL) {
          timer_set_on_destroy(id, r.add_timer.on_destroy, r.add_timer.on_destroy_ctx);
        }
      } break;
      default: {
        break;
      }
    }
    time_out = time_now_ms();
    /*HANDLE OTHER EVENT*/
  }

  return RET_OK;
}

static ret_t main_loop_dispatch_input(main_loop_console_t* loop) {
  if (loop->dispatch_input) {
    loop->dispatch_input(loop);
  }

  return RET_OK;
}

static ret_t main_loop_console_step(main_loop_t* l) {
  main_loop_console_t* loop = (main_loop_console_t*)l;
  const uint32_t duration = 16;
  uint32_t curr_expected_sleep_time = 0xFFFFFFFF;
  uint32_t cost_time = 0;
  uint64_t time_in = time_now_ms();
  uint64_t time_out = time_in;

  main_loop_dispatch_input(loop);
  main_loop_dispatch_events(loop);
  event_source_manager_dispatch(loop->event_source_manager);

  time_out = time_now_ms();
  cost_time = (uint32_t)(time_out - time_in);

  curr_expected_sleep_time = (duration > cost_time) ? (duration - cost_time) : 0;
  main_loop_set_curr_expected_sleep_time(l, curr_expected_sleep_time);

  return RET_OK;
}

static ret_t main_loop_console_run(main_loop_t* l) {
  while (l->running) {
    if (l->quit_num) {
      --l->quit_num;
      --l->running;
      break;
    }
    main_loop_step(l);
    main_loop_sleep(l);
  }

  return RET_OK;
}

static event_source_manager_t* main_loop_console_get_event_source_manager(main_loop_t* l) {
  main_loop_console_t* loop = (main_loop_console_t*)l;

  return loop->event_source_manager;
}

main_loop_console_t* main_loop_console_init(void) {
  event_source_t* idle_source = NULL;
  event_source_t* timer_source = NULL;
  static main_loop_console_t s_main_loop_console;
  main_loop_console_t* loop = &s_main_loop_console;

  memset(loop, 0x00, sizeof(main_loop_console_t));

  loop->queue = event_queue_create(MAIN_LOOP_QUEUE_SIZE);
  return_value_if_fail(loop->queue != NULL, NULL);

  loop->base.run = main_loop_console_run;
  loop->base.step = main_loop_console_step;

  loop->mutex = tk_mutex_create();
  return_value_if_fail(loop->mutex != NULL, NULL);
  loop->base.recv_event = main_loop_console_recv_event_mutex;
  loop->base.queue_event = main_loop_console_queue_event_mutex;

  loop->base.get_event_source_manager = main_loop_console_get_event_source_manager;

  main_loop_set((main_loop_t*)loop);

  loop->event_source_manager = event_source_manager_default_create();

  idle_source = event_source_idle_create(idle_manager());
  timer_source = event_source_timer_create(timer_manager());
  event_source_manager_add(loop->event_source_manager, idle_source);
  event_source_manager_add(loop->event_source_manager, timer_source);
  TK_OBJECT_UNREF(idle_source);
  TK_OBJECT_UNREF(timer_source);

  return loop;
}

ret_t main_loop_console_reset(main_loop_console_t* loop) {
  return_value_if_fail(loop != NULL, RET_BAD_PARAMS);

  event_source_manager_destroy(loop->event_source_manager);
  event_queue_destroy(loop->queue);

  if (loop->mutex != NULL) {
    tk_mutex_destroy(loop->mutex);
  }

  memset(loop, 0x00, sizeof(main_loop_console_t));

  return RET_OK;
}
