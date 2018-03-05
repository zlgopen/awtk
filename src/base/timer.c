/**
 * File:   timer.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  timer manager
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#include "base/timer.h"
#include "base/array.h"
#include "base/mem.h"

static timer_get_time_t s_get_time = NULL;
static array_t* s_timer_manager = NULL;

ret_t timer_init(timer_get_time_t get_time) {
  return_value_if_fail(get_time != NULL, RET_BAD_PARAMS);

  s_get_time = get_time;

  return RET_OK;
}

static ret_t ensure_timer_manager() {
  if (s_timer_manager == NULL) {
    s_timer_manager = array_create(5);
  }
  return_value_if_fail(s_timer_manager != NULL, RET_FAIL);

  return RET_OK;
}

ret_t timer_add(timer_func_t on_timer, void* user_data, uint32_t duration_ms) {
  f_timer_t* timer = NULL;

  return_value_if_fail(s_get_time != NULL && on_timer != NULL && ensure_timer_manager() == RET_OK,
                       RET_BAD_PARAMS);

  timer = MEM_ZALLOC(f_timer_t);
  return_value_if_fail(timer != NULL, RET_BAD_PARAMS);

  timer->start = s_get_time();
  timer->user_data = user_data;
  timer->on_timer = on_timer;
  timer->duration_ms = duration_ms;

  return array_push(s_timer_manager, timer) ? RET_OK : RET_FAIL;
}

static int compare_timer(const void* a, const void* b) {
  f_timer_t* t1 = (f_timer_t*)a;
  f_timer_t* t2 = (f_timer_t*)b;

  if (t1->on_timer == t2->on_timer && t1->user_data == t2->user_data &&
      t1->duration_ms == t2->duration_ms) {
    return 0;
  }

  return -1;
}

ret_t timer_remove(timer_func_t on_timer, void* user_data, uint32_t duration_ms) {
  f_timer_t timer;
  return_value_if_fail(on_timer != NULL && ensure_timer_manager() == RET_OK, RET_BAD_PARAMS);

  timer.on_timer = on_timer;
  timer.user_data = user_data;
  timer.duration_ms = duration_ms;

  return array_remove(s_timer_manager, compare_timer, &timer) ? RET_OK : RET_FAIL;
}

ret_t timer_check() {
  uint32_t i = 0;
  uint32_t k = 0;
  uint32_t nr = 0;
  uint32_t now = 0;
  f_timer_t** timers = NULL;
  return_value_if_fail(s_get_time != NULL && ensure_timer_manager() == RET_OK, RET_BAD_PARAMS);

  if (s_timer_manager->size == 0) {
    return RET_OK;
  }

  now = s_get_time();
  timers = (f_timer_t**)s_timer_manager->elms;
  for (i = 0, nr = s_timer_manager->size; i < nr; i++) {
    f_timer_t* iter = timers[i];
    if ((iter->start + iter->duration_ms) <= now) {
      iter->repeat = RET_REPEAT == iter->on_timer(iter);
      if (iter->repeat) {
        iter->start = now;
      }
    } else {
      iter->repeat = TRUE;
    }
  }

  for (k = 0, i = 0, nr = s_timer_manager->size; i < nr; i++) {
    f_timer_t* iter = timers[i];
    if (iter->repeat) {
      timers[k++] = timers[i];
    }
  }
  s_timer_manager->size = k;

  return RET_OK;
}

uint32_t timer_count() { return ensure_timer_manager() == RET_OK ? s_timer_manager->size : 0; }
