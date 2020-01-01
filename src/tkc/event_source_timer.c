/**
 * File:   event_source_timer.h
 * Author: AWTK Develop Team
 * Brief:  event source timer
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/event_source_timer.h"

static const object_vtable_t s_event_source_timer_vtable = {.type = "event_source_timer",
                                                            .desc = "event_source_timer",
                                                            .size = sizeof(event_source_timer_t)};

static int32_t event_source_timer_get_fd(event_source_t* source) {
  return -1;
}

static ret_t event_source_timer_check(event_source_t* source) {
  return RET_OK;
}

static ret_t event_source_timer_dispatch(event_source_t* source) {
  event_source_timer_t* event_source_timer = EVENT_SOURCE_TIMER(source);

  return timer_manager_dispatch(event_source_timer->timer_manager);
}

uint32_t event_source_timer_get_wakeup_time(event_source_t* source) {
  event_source_timer_t* event_source_timer = EVENT_SOURCE_TIMER(source);
  uint64_t next_time = timer_manager_next_time(event_source_timer->timer_manager);
  int64_t ret = next_time - event_source_timer->timer_manager->get_time();

  return ret > 0 ? ret : 0;
}

event_source_t* event_source_timer_create(timer_manager_t* timer_manager) {
  object_t* obj = NULL;
  event_source_t* event_source = NULL;
  event_source_timer_t* event_source_timer = NULL;
  return_value_if_fail(timer_manager != NULL, NULL);

  obj = object_create(&s_event_source_timer_vtable);
  event_source = EVENT_SOURCE(obj);
  event_source_timer = EVENT_SOURCE_TIMER(obj);
  return_value_if_fail(event_source_timer != NULL, NULL);

  event_source_timer->timer_manager = timer_manager;
  event_source->check = event_source_timer_check;
  event_source->get_fd = event_source_timer_get_fd;
  event_source->dispatch = event_source_timer_dispatch;
  event_source->get_wakeup_time = event_source_timer_get_wakeup_time;

  return event_source;
}
