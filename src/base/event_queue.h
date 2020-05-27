/**
 * File:   event_queue.h
 * Author: AWTK Develop Team
 * Brief:  event_queue
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
 * 2018-03-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_EVENT_QUEUE_H
#define TK_EVENT_QUEUE_H

#include "base/idle.h"
#include "base/timer.h"
#include "base/events.h"

BEGIN_C_DECLS

enum { REQ_ADD_IDLE = EVT_REQ_START, REQ_ADD_TIMER };

typedef struct _add_idle_t {
  event_t e;
  idle_func_t func;
} add_idle_t;

typedef struct _add_timer_t {
  event_t e;
  timer_func_t func;
  uint32_t duration;
} add_timer_t;

typedef union _event_queue_req_t {
  event_t event;
  key_event_t key_event;
  wheel_event_t wheel_event;
  pointer_event_t pointer_event;
  add_idle_t add_idle;
  add_timer_t add_timer;
} event_queue_req_t;

typedef struct _event_queue_t {
  uint16_t r;
  uint16_t w;
  uint16_t full;
  uint16_t capacity;
  event_queue_req_t events[1];
} event_queue_t;

event_queue_t* event_queue_create(uint16_t capacity);
ret_t event_queue_recv(event_queue_t* q, event_queue_req_t* r);
ret_t event_queue_send(event_queue_t* q, const event_queue_req_t* r);
ret_t event_queue_replace_last(event_queue_t* q, const event_queue_req_t* r);
ret_t event_queue_destroy(event_queue_t* q);

END_C_DECLS

#endif /*TK_EVENT_QUEUE_H*/
