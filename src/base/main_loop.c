/**
 * File:   main_loop.c
 * Author: AWTK Develop Team
 * Brief:  main_loop interface
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/time.h"
#include "base/platform.h"
#include "base/main_loop.h"
#include "base/window_manager.h"

ret_t main_loop_run(main_loop_t* l) {
  return_value_if_fail(l != NULL && l->run != NULL, RET_BAD_PARAMS);
  l->running = TRUE;

  return l->run(l);
}

ret_t main_loop_quit(main_loop_t* l) {
  return_value_if_fail(l != NULL, RET_BAD_PARAMS);
  l->running = FALSE;

  if(l->quit != NULL) {
    l->quit(l);
  }

  return RET_OK;
}

ret_t main_loop_wakeup(main_loop_t* l) {
  return_value_if_fail(l != NULL, RET_BAD_PARAMS);

  if (l->wakeup != NULL) {
    l->wakeup(l);
  }

  return RET_OK;
}

ret_t main_loop_destroy(main_loop_t* l) {
  return_value_if_fail(l != NULL && l->destroy != NULL, RET_BAD_PARAMS);
  l->running = FALSE;

  return l->destroy(l);
}

static main_loop_t* s_default_main_loop = NULL;

main_loop_t* main_loop(void) { return s_default_main_loop; }

ret_t main_loop_set(main_loop_t* loop) {
  return_value_if_fail(loop != NULL, RET_BAD_PARAMS);
  s_default_main_loop = loop;

  return RET_OK;
}

ret_t main_loop_queue_event(main_loop_t* l, const event_queue_req_t* e) {
  return_value_if_fail(l != NULL && l->queue_event != NULL && e != NULL, RET_BAD_PARAMS);

  return l->queue_event(l, e);
}

#include "base/idle.h"
#include "base/timer.h"
#include "base/window_manager.h"

ret_t main_loop_sleep(main_loop_t* l) {
  window_manager_t* wm = WINDOW_MANAGER(window_manager());

  if (!wm->animating) {
    uint32_t sleep_time = 0;
    int32_t next_timer = timer_next_time() - time_now_ms();

    if (next_timer < 0) {
      next_timer = 0;
    }

    sleep_time = next_timer > 30 ? 30 : next_timer;
    sleep_ms(sleep_time);
  }

  return RET_OK;
}
