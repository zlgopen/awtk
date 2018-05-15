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

static uint32_t s_timer_id = 1;
static array_t* s_timer_manager = NULL;
static timer_get_time_t s_get_time = NULL;

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

uint32_t timer_add(timer_func_t on_timer, void* ctx, uint32_t duration_ms) {
  timer_info_t* timer = NULL;
  return_value_if_fail(on_timer != NULL, 0);
  return_value_if_fail(s_get_time != NULL && ensure_timer_manager() == RET_OK, 0);

  timer = TKMEM_ZALLOC(timer_info_t);
  return_value_if_fail(timer != NULL, 0);

  timer->ctx = ctx;
  timer->id = s_timer_id++;
  timer->start = s_get_time();
  timer->on_timer = on_timer;
  timer->duration_ms = duration_ms;

  return array_push(s_timer_manager, timer) == RET_OK ? timer->id : 0;
}

static int compare_timer(const void* a, const void* b) {
  timer_info_t* t1 = (timer_info_t*)a;
  timer_info_t* t2 = (timer_info_t*)b;

  if (t1->id == t2->id) {
    return 0;
  }

  return -1;
}

ret_t timer_remove(uint32_t timer_id) {
  timer_info_t timer;
  timer_info_t* ret = NULL;
  return_value_if_fail(timer_id > 0, RET_BAD_PARAMS);
  return_value_if_fail(s_get_time != NULL && ensure_timer_manager() == RET_OK, RET_BAD_PARAMS);

  timer.id = timer_id;
  ret = (timer_info_t*)array_find(s_timer_manager, compare_timer, &timer);
  return_value_if_fail(ret != NULL, RET_NOT_FOUND);
  if (array_remove(s_timer_manager, compare_timer, &timer, NULL) == RET_OK) {
    memset(ret, 0x00, sizeof(timer_info_t));
    /*will be removed on next check*/
  }

  return RET_OK;
}

const timer_info_t* timer_find(uint32_t timer_id) {
  timer_info_t timer;
  return_value_if_fail(timer_id > 0, NULL);
  return_value_if_fail(s_get_time != NULL && ensure_timer_manager() == RET_OK, NULL);

  timer.id = timer_id;

  return (const timer_info_t*)array_find(s_timer_manager, compare_timer, &timer);
}

ret_t timer_dispatch() {
  uint32_t i = 0;
  uint32_t k = 0;
  uint32_t nr = 0;
  uint32_t now = 0;
  timer_info_t** timers = NULL;
  return_value_if_fail(s_get_time != NULL && ensure_timer_manager() == RET_OK, RET_BAD_PARAMS);

  if (s_timer_manager->size == 0) {
    return RET_OK;
  }

  now = s_get_time();
  timers = (timer_info_t**)s_timer_manager->elms;
  for (i = 0, nr = s_timer_manager->size; i < nr; i++) {
    uint32_t end = 0;
    timer_info_t* iter = timers[i];
    if (iter->on_timer == NULL) {
      /*it is removed*/
      continue;
    }

    end = iter->start + iter->duration_ms;
    if (end <= now) {
      iter->now = now;
      iter->repeat = RET_REPEAT == iter->on_timer(iter);
      if (iter->repeat) {
        iter->start = now;
      }
    } else {
      iter->repeat = TRUE;
    }
  }

  for (k = 0, i = 0, nr = s_timer_manager->size; i < nr; i++) {
    timer_info_t* iter = timers[i];
    if (iter->repeat) {
      timers[k++] = timers[i];
    } else {
      TKMEM_FREE(timers[i]);
    }
  }
  s_timer_manager->size = k;

  return RET_OK;
}

uint32_t timer_count() {
  uint32_t i = 0;
  uint32_t nr = 0;
  uint32_t count = 0;

  if (s_timer_manager && s_timer_manager->size > 0) {
    timer_info_t** timers = (timer_info_t**)s_timer_manager->elms;
    for (i = 0, nr = s_timer_manager->size; i < nr; i++) {
      timer_info_t* iter = timers[i];
      if (iter->on_timer != NULL) {
        count++;
      }
    }
  }

  return count;
}
