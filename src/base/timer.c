/**
 * File:   timer.c
 * Author: AWTK Develop Team
 * Brief:  timer manager
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/timer.h"
#include "base/array.h"

static ret_t timer_info_destroy(timer_info_t* info) {
  memset(info, 0x00, sizeof(timer_info_t));
  TKMEM_FREE(info);

  return RET_OK;
}

static timer_manager_t* s_timer_manager;
#define ACTIVE_TIMERS(timer_manager) (((timer_manager)->timers) + (timer_manager)->active)
#define OFFLINE_TIMERS(timer_manager) \
  (((timer_manager)->timers) + ((timer_manager)->active ? 0 : 1))

timer_manager_t* timer_manager(void) {
  return s_timer_manager;
}

ret_t timer_manager_set(timer_manager_t* timer_manager) {
  s_timer_manager = timer_manager;

  return RET_OK;
}

timer_manager_t* timer_manager_create(timer_get_time_t get_time) {
  timer_manager_t* timer_manager = NULL;
  return_value_if_fail(get_time != NULL, NULL);

  timer_manager = TKMEM_ZALLOC(timer_manager_t);
  return_value_if_fail(timer_manager != NULL, NULL);

  return timer_manager_init(timer_manager, get_time);
}

timer_manager_t* timer_manager_init(timer_manager_t* timer_manager, timer_get_time_t get_time) {
  return_value_if_fail(timer_manager != NULL, NULL);

  timer_manager->next_timer_id = 1;
  timer_manager->get_time = get_time;
  array_init(timer_manager->timers + 0, 0);
  array_init(timer_manager->timers + 1, 0);

  return timer_manager;
}

ret_t timer_manager_deinit(timer_manager_t* timer_manager) {
  uint32_t i = 0;
  uint32_t nr = 0;
  timer_info_t** timers = NULL;
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  nr = ACTIVE_TIMERS(timer_manager)->size;
  timers = (timer_info_t**)ACTIVE_TIMERS(timer_manager)->elms;

  for (i = 0; i < nr; i++) {
    timer_info_t* iter = timers[i];
    timer_info_destroy(iter);
  }
  array_deinit(timer_manager->timers + 0);
  array_deinit(timer_manager->timers + 1);

  return RET_OK;
}

ret_t timer_manager_destroy(timer_manager_t* timer_manager) {
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  timer_manager_deinit(timer_manager);
  TKMEM_FREE(timer_manager);

  return RET_OK;
}

uint32_t timer_manager_add(timer_manager_t* timer_manager, timer_func_t on_timer, void* ctx,
                           uint32_t duration_ms) {
  timer_info_t* timer = NULL;
  return_value_if_fail(on_timer != NULL, 0);
  return_value_if_fail(timer_manager != NULL, 0);

  timer = TKMEM_ZALLOC(timer_info_t);
  return_value_if_fail(timer != NULL, 0);

  timer->ctx = ctx;
  timer->on_timer = on_timer;
  timer->duration_ms = duration_ms;
  timer->start = timer_manager->get_time();
  timer->id = timer_manager->next_timer_id++;
  timer->timer_manager = timer_manager;

  return array_push(ACTIVE_TIMERS(timer_manager), timer) == RET_OK ? timer->id : 0;
}

static int timer_info_compare_id(const void* a, const void* b) {
  timer_info_t* t1 = (timer_info_t*)a;
  timer_info_t* t2 = (timer_info_t*)b;

  if (t1->id == t2->id) {
    return 0;
  }

  return -1;
}

ret_t timer_manager_remove(timer_manager_t* timer_manager, uint32_t timer_id) {
  timer_info_t timer;
  ret_t ret = RET_FAIL;
  return_value_if_fail(timer_id > 0, RET_BAD_PARAMS);
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  timer.id = timer_id;

  ret = array_remove(ACTIVE_TIMERS(timer_manager), timer_info_compare_id, &timer,
                     (tk_destroy_t)timer_info_destroy);
  if (ret != RET_OK) {
    ret = array_remove(OFFLINE_TIMERS(timer_manager), timer_info_compare_id, &timer,
                       (tk_destroy_t)timer_info_destroy);
  }

  return ret;
}

const timer_info_t* timer_manager_find(timer_manager_t* timer_manager, uint32_t timer_id) {
  timer_info_t timer;
  const timer_info_t* ret = NULL;
  return_value_if_fail(timer_id > 0, NULL);
  return_value_if_fail(timer_manager != NULL, NULL);

  timer.id = timer_id;

  ret =
      (const timer_info_t*)array_find(ACTIVE_TIMERS(timer_manager), timer_info_compare_id, &timer);
  if (ret == NULL) {
    ret = (const timer_info_t*)array_find(OFFLINE_TIMERS(timer_manager), timer_info_compare_id,
                                          &timer);
  }

  return ret;
}

ret_t timer_manager_dispatch(timer_manager_t* timer_manager) {
  uint32_t now = 0;
  array_t* active = NULL;
  array_t* offline = NULL;
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  now = timer_manager->get_time();
  active = ACTIVE_TIMERS(timer_manager);
  offline = OFFLINE_TIMERS(timer_manager);

  while (active->size > 0) {
    timer_info_t* iter = (timer_info_t*)array_pop(active);
    if ((iter->start + iter->duration_ms) <= now) {
      iter->now = now;
      if (iter->on_timer(iter) != RET_REPEAT) {
        timer_info_destroy(iter);
      } else {
        iter->start = now;
        array_push(offline, iter);
      }
    } else {
      array_push(offline, iter);
    }
  }

  timer_manager->active = timer_manager->active ? 0 : 1;

  return RET_OK;
}

uint32_t timer_manager_count(timer_manager_t* timer_manager) {
  return_value_if_fail(timer_manager != NULL, 0);

  return ACTIVE_TIMERS(timer_manager)->size + OFFLINE_TIMERS(timer_manager)->size;
}

uint32_t timer_manager_next_time(timer_manager_t* timer_manager) {
  uint32_t i = 0;
  array_t* active = NULL;
  uint32_t t = timer_manager->get_time() + 0xffff;
  return_value_if_fail(timer_manager != NULL, t);
  active = ACTIVE_TIMERS(timer_manager);

  for (i = 0; i < active->size; i++) {
    timer_info_t* iter = (timer_info_t*)(active->elms[i]);
    if ((iter->start + iter->duration_ms) < t) {
      t = iter->start + iter->duration_ms;
    }
  }

  return t;
}

ret_t timer_init(timer_get_time_t get_time) {
  return_value_if_fail(get_time != NULL, RET_BAD_PARAMS);
  return_value_if_fail(s_timer_manager == NULL, RET_FAIL);

  return timer_manager_set(timer_manager_create(get_time));
}

uint32_t timer_add(timer_func_t on_timer, void* ctx, uint32_t duration_ms) {
  return timer_manager_add(timer_manager(), on_timer, ctx, duration_ms);
}

ret_t timer_remove(uint32_t timer_id) {
  return timer_manager_remove(timer_manager(), timer_id);
}

const timer_info_t* timer_find(uint32_t timer_id) {
  return timer_manager_find(timer_manager(), timer_id);
}

ret_t timer_dispatch(void) {
  return timer_manager_dispatch(timer_manager());
}

uint32_t timer_count(void) {
  return ACTIVE_TIMERS(timer_manager())->size;
}

uint32_t timer_next_time(void) {
  return timer_manager_next_time(timer_manager());
}

uint32_t timer_now(void) {
  return timer_manager()->get_time();
}

#include "base/main_loop.h"

ret_t timer_queue(timer_func_t on_timer, void* ctx, uint32_t duration) {
  event_queue_req_t r;
  r.add_timer.func = on_timer;
  r.add_timer.duration = duration;
  r.add_timer.e.target = ctx;
  r.add_timer.e.type = REQ_ADD_TIMER;

  return main_loop_queue_event(main_loop(), &r);
}

ret_t timer_set_on_destroy(uint32_t timer_id, tk_destroy_t on_destroy, void* on_destroy_ctx) {
  timer_info_t* item = (timer_info_t*)timer_find(timer_id);
  return_value_if_fail(item != NULL, RET_BAD_PARAMS);

  item->on_destroy = on_destroy;
  item->on_destroy_ctx = on_destroy_ctx;

  return RET_OK;
}
