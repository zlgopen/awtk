/**
 * File:   event_queue.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  event_queue
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
 * 2018-03-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_EVENT_QUEUE_H
#define LFTK_EVENT_QUEUE_H

#include "base/events.h"

BEGIN_C_DECLS

typedef struct _event_all_t {
  union {
    event_t event;
    key_event_t key_event;
    pointer_event_t pointer_event;
  }e;
}event_all_t;

typedef struct _event_queue_t {
  uint16_t r;
  uint16_t w;
  uint16_t full;
  uint16_t capacity;
  event_all_t events[1];
}event_queue_t;

event_queue_t* event_queue_create(uint16_t capacity);
ret_t event_queue_recv(event_queue_t* q, event_all_t* e);
ret_t event_queue_send(event_queue_t* q, const event_all_t* e);
ret_t event_queue_replace_last(event_queue_t* q, const event_all_t* e);
ret_t event_queue_destroy(event_queue_t* q);

END_C_DECLS

#endif/*LFTK_EVENT_QUEUE_H*/

