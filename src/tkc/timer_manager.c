/**
 * File:   timer.c
 * Author: AWTK Develop Team
 * Brief:  timer manager
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/timer_manager.h"

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
  timer_manager->last_dispatch_time = get_time();
  timer_manager->get_time = get_time;
  slist_init(&(timer_manager->timers), (tk_destroy_t)object_unref, timer_info_compare);

  return timer_manager;
}

ret_t timer_manager_deinit(timer_manager_t* timer_manager) {
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  slist_deinit(&(timer_manager->timers));

  return RET_OK;
}

ret_t timer_manager_destroy(timer_manager_t* timer_manager) {
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  timer_manager_deinit(timer_manager);
  TKMEM_FREE(timer_manager);

  return RET_OK;
}

ret_t timer_manager_append(timer_manager_t* timer_manager, timer_info_t* timer) {
  return_value_if_fail(timer_manager != NULL && timer != NULL, RET_BAD_PARAMS);

  return slist_append(&(timer_manager->timers), timer);
}

uint32_t timer_manager_add(timer_manager_t* timer_manager, timer_func_t on_timer, void* ctx,
                           uint32_t duration) {
  timer_info_t* timer = NULL;
  return_value_if_fail(on_timer != NULL, TK_INVALID_ID);
  return_value_if_fail(timer_manager != NULL, TK_INVALID_ID);

  timer = timer_info_create(timer_manager, on_timer, ctx, duration);
  return_value_if_fail(timer != NULL, TK_INVALID_ID);

  return timer->id;
}

ret_t timer_manager_remove(timer_manager_t* timer_manager, uint32_t timer_id) {
  timer_info_t timer;
  return_value_if_fail(timer_id != TK_INVALID_ID, RET_BAD_PARAMS);
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  return slist_remove(&(timer_manager->timers), timer_info_init_dummy(&timer, timer_id));
}

ret_t timer_manager_reset(timer_manager_t* timer_manager, uint32_t timer_id) {
  timer_info_t* info = (timer_info_t*)timer_manager_find(timer_manager, timer_id);
  return_value_if_fail(info != NULL, RET_NOT_FOUND);
  info->start = timer_manager->get_time();

  return RET_OK;
}

const timer_info_t* timer_manager_find(timer_manager_t* timer_manager, uint32_t timer_id) {
  timer_info_t timer;
  return_value_if_fail(timer_id != TK_INVALID_ID, NULL);
  return_value_if_fail(timer_manager != NULL, NULL);

  return slist_find(&(timer_manager->timers), timer_info_init_dummy(&timer, timer_id));
}

static ret_t timer_manager_dispatch_one(timer_manager_t* timer_manager, uint64_t now,
                                        int32_t delta_time) {
  slist_node_t* iter = timer_manager->timers.first;

  while (iter != NULL && !timer_info_is_available(TIMER_INFO(iter->data), now)) {
    iter = iter->next;
  }

  if (iter != NULL) {
    timer_info_t* timer = (timer_info_t*)object_ref((object_t*)(iter->data));
    return_value_if_fail(timer != NULL, RET_BAD_PARAMS);

    timer->now = now;
    if ((timer->start + timer->duration) <= now) {
      if (timer_info_on_timer(timer, now) != RET_REPEAT) {
        timer_manager_remove(timer_manager, timer->id);
      } else {
        timer->start = now;
      }
    }

    object_unref((object_t*)timer);

    return RET_OK;
  }

  return RET_DONE;
}

ret_t timer_manager_dispatch(timer_manager_t* timer_manager) {
  uint64_t now = 0;
  int32_t delta_time = 0;
  return_value_if_fail(timer_manager != NULL, RET_BAD_PARAMS);

  now = timer_manager->get_time();
  delta_time = now - timer_manager->last_dispatch_time;

  if (timer_manager->timers.first == NULL) {
    timer_manager->last_dispatch_time = now;
    return RET_OK;
  }

  timer_manager->last_dispatch_time = now;
  while (timer_manager_dispatch_one(timer_manager, now, delta_time) == RET_OK) {
    if (timer_manager->last_dispatch_time != now) {
      log_debug("abort dispatch because sub main loop\n");
    }
  }

  return RET_OK;
}

uint32_t timer_manager_count(timer_manager_t* timer_manager) {
  return_value_if_fail(timer_manager != NULL, 0);

  return slist_size(&(timer_manager->timers));
}

uint64_t timer_manager_next_time(timer_manager_t* timer_manager) {
  slist_node_t* iter = NULL;
  uint64_t t = timer_manager->get_time() + 0xffff;
  return_value_if_fail(timer_manager != NULL, t);

  iter = timer_manager->timers.first;
  while (iter != NULL) {
    timer_info_t* timer = TIMER_INFO(iter->data);
    if ((timer->start + timer->duration) < t) {
      t = timer->start + timer->duration;
    }
    iter = iter->next;
  }

  return t;
}
