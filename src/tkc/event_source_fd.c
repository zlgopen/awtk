/**
 * File:   event_source_fd.h
 * Author: AWTK Develop Team
 * Brief:  event source fd
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

#include "tkc/event_source_fd.h"

static const object_vtable_t s_event_source_fd_vtable = {
    .type = "event_source_fd", .desc = "event_source_fd", .size = sizeof(event_source_fd_t)};

static int32_t event_source_fd_get_fd(event_source_t* source) {
  event_source_fd_t* event_source_fd = EVENT_SOURCE_FD(source);

  return event_source_fd->fd;
}

static ret_t event_source_fd_check(event_source_t* source) {
  return RET_OK;
}

static ret_t event_source_fd_dispatch(event_source_t* source) {
  event_source_fd_t* event_source_fd = EVENT_SOURCE_FD(source);

  return event_source_fd->on_event(source);
}

uint32_t event_source_fd_get_wakeup_time(event_source_t* source) {
  return 0xffff;
}

event_source_t* event_source_fd_create(int fd, event_source_on_event_t on_event, void* ctx) {
  object_t* obj = NULL;
  event_source_t* event_source = NULL;
  event_source_fd_t* event_source_fd = NULL;
  return_value_if_fail(fd >= 0 && on_event != NULL, NULL);

  obj = object_create(&s_event_source_fd_vtable);
  event_source = EVENT_SOURCE(obj);
  event_source_fd = EVENT_SOURCE_FD(obj);
  return_value_if_fail(event_source_fd != NULL, NULL);

  event_source_fd->fd = fd;
  event_source_fd->ctx = ctx;
  event_source_fd->on_event = on_event;

  event_source->check = event_source_fd_check;
  event_source->get_fd = event_source_fd_get_fd;
  event_source->dispatch = event_source_fd_dispatch;
  event_source->get_wakeup_time = event_source_fd_get_wakeup_time;

  return event_source;
}
