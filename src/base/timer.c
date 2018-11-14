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

  timer_manager->next_timer_id = TK_INVALID_ID + 1;
  timer_manager->get_time = get_time;

  return timer_manager;
}

ret_t timer_manager_deinit(timer_manager_t* timer_manager) {
  timer_info_t* iter = NULL;
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  iter = timer_manager->first;
  while (iter != NULL) {
    timer_info_t* next = iter->next;

    timer_info_destroy(iter);
    iter = next;
  }
  timer_manager->first = NULL;

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
  timer_info_t* iter = NULL;
  timer_info_t* timer = NULL;
  return_value_if_fail(on_timer != NULL, TK_INVALID_ID);
  return_value_if_fail(timer_manager != NULL, TK_INVALID_ID);

  timer = TKMEM_ZALLOC(timer_info_t);
  return_value_if_fail(timer != NULL, TK_INVALID_ID);

  timer->ctx = ctx;
  timer->on_timer = on_timer;
  timer->duration_ms = duration_ms;
  timer->start = timer_manager->get_time();
  timer->id = timer_manager->next_timer_id++;
  timer->timer_manager = timer_manager;

  iter = timer_manager->first;
  if (iter == NULL) {
    timer_manager->first = timer;
  } else {
    while (iter->next != NULL) {
      iter = iter->next;
    }
    iter->next = timer;
  }

  return timer->id;
}

ret_t timer_manager_remove(timer_manager_t* timer_manager, uint32_t timer_id) {
  ret_t ret = RET_NOT_FOUND;
  timer_info_t* iter = NULL;
  timer_info_t* prev = NULL;
  return_value_if_fail(timer_id != TK_INVALID_ID, RET_BAD_PARAMS);
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  prev = timer_manager->first;
  iter = timer_manager->first;

  if (iter->id == timer_id) {
    if (timer_manager->dispatching) {
      iter->pending_destroy = TRUE;
    } else {
      timer_manager->first = iter->next;
      timer_info_destroy(iter);
    }
    ret = RET_OK;
  } else {
    prev = iter;
    iter = iter->next;
    while (iter != NULL) {
      if (iter->id == timer_id) {
        if (timer_manager->dispatching) {
          iter->pending_destroy = TRUE;
        } else {
          prev->next = iter->next;
          timer_info_destroy(iter);
        }
        ret = RET_OK;
        break;
      }
      prev = iter;
      iter = iter->next;
    }
  }

  return ret;
}

static ret_t timer_manager_remove_pending_destroy_timers(timer_manager_t* timer_manager) {
  ret_t ret = RET_NOT_FOUND;
  timer_info_t* iter = NULL;
  timer_info_t* prev = NULL;
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  prev = timer_manager->first;
  iter = timer_manager->first;
  while (iter != NULL) {
    if (iter->pending_destroy) {
      if (iter == timer_manager->first) {
        timer_manager->first = iter->next;
        timer_info_destroy(iter);

        prev = timer_manager->first;
        iter = timer_manager->first;
      } else {
        prev->next = iter->next;
        timer_info_destroy(iter);
        iter = prev->next;
      }
    } else {
      prev = iter;
      iter = prev->next;
    }
  }

  return ret;
}

const timer_info_t* timer_manager_find(timer_manager_t* timer_manager, uint32_t timer_id) {
  timer_info_t* iter = NULL;
  return_value_if_fail(timer_id != TK_INVALID_ID, NULL);
  return_value_if_fail(timer_manager != NULL, NULL);

  iter = timer_manager->first;
  while (iter != NULL) {
    if (iter->id == timer_id) {
      if (iter->pending_destroy) {
        return NULL;
      }
      return iter;
    }

    iter = iter->next;
  }

  return NULL;
}

ret_t timer_manager_dispatch(timer_manager_t* timer_manager) {
  uint32_t now = 0;
  timer_info_t* iter = NULL;
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  iter = timer_manager->first;
  if (iter == NULL) {
    return RET_OK;
  }

  now = timer_manager->get_time();
  timer_manager->dispatching = TRUE;
  while (iter != NULL) {
    iter->now = now;
    if ((iter->start + iter->duration_ms) <= now) {
      if (iter->on_timer(iter) != RET_REPEAT) {
        iter->pending_destroy = TRUE;
      } else {
        iter->start = now;
      }
    }
    iter = iter->next;
  }
  timer_manager->dispatching = FALSE;
  timer_manager_remove_pending_destroy_timers(timer_manager);

  return RET_OK;
}

uint32_t timer_manager_count(timer_manager_t* timer_manager) {
  uint32_t count = 0;
  timer_info_t* iter = NULL;
  return_value_if_fail(timer_manager != NULL, 0);

  iter = timer_manager->first;
  while (iter != NULL) {
    if (!iter->pending_destroy) {
      count++;
    }
    iter = iter->next;
  }

  return count;
}

uint32_t timer_manager_next_time(timer_manager_t* timer_manager) {
  timer_info_t* iter = NULL;
  uint32_t t = timer_manager->get_time() + 0xffff;
  return_value_if_fail(timer_manager != NULL, t);

  iter = timer_manager->first;
  while (iter != NULL) {
    if ((iter->start + iter->duration_ms) < t) {
      t = iter->start + iter->duration_ms;
    }
    iter = iter->next;
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
  return timer_manager_count(timer_manager());
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
