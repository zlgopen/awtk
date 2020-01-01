/**
 * File:   event_source_idle.h
 * Author: AWTK Develop Team
 * Brief:  event source idle
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

#include "tkc/event_source_idle.h"

static const object_vtable_t s_event_source_idle_vtable = {
    .type = "event_source_idle", .desc = "event_source_idle", .size = sizeof(event_source_idle_t)};

static int32_t event_source_idle_get_fd(event_source_t* source) {
  return -1;
}

static ret_t event_source_idle_check(event_source_t* source) {
  return RET_OK;
}

static ret_t event_source_idle_dispatch(event_source_t* source) {
  event_source_idle_t* event_source_idle = EVENT_SOURCE_IDLE(source);

  return idle_manager_dispatch(event_source_idle->idle_manager);
}

uint32_t event_source_idle_get_wakeup_time(event_source_t* source) {
  event_source_idle_t* event_source_idle = EVENT_SOURCE_IDLE(source);
  uint32_t nr = idle_manager_count(event_source_idle->idle_manager);

  return nr > 0 ? 0 : 0xffff;
}

event_source_t* event_source_idle_create(idle_manager_t* idle_manager) {
  object_t* obj = NULL;
  event_source_t* event_source = NULL;
  event_source_idle_t* event_source_idle = NULL;
  return_value_if_fail(idle_manager != NULL, NULL);

  obj = object_create(&s_event_source_idle_vtable);
  event_source = EVENT_SOURCE(obj);
  event_source_idle = EVENT_SOURCE_IDLE(obj);
  return_value_if_fail(event_source_idle != NULL, NULL);

  event_source_idle->idle_manager = idle_manager;
  event_source->check = event_source_idle_check;
  event_source->get_fd = event_source_idle_get_fd;
  event_source->dispatch = event_source_idle_dispatch;
  event_source->get_wakeup_time = event_source_idle_get_wakeup_time;

  return event_source;
}
