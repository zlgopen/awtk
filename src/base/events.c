/**
 * File:   events.c
 * Author: AWTK Develop Team
 * Brief:  events structs
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
 * 2018-03-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/time.h"
#include "base/events.h"

pointer_event_t* pointer_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_POINTER_DOWN && event->type <= EVT_CLICK, NULL);

  return (pointer_event_t*)event;
}

key_event_t* key_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_KEY_DOWN && event->type <= EVT_KEY_UP, NULL);

  return (key_event_t*)event;
}

paint_event_t* paint_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(
      event->type == EVT_PAINT || event->type == EVT_BEFORE_PAINT || event->type == EVT_AFTER_PAINT,
      NULL);

  return (paint_event_t*)event;
}

event_t event_init(event_type_t type, void* target) {
  event_t e;

  e.type = type;
  e.target = target;
  e.time = time_now_ms();

  return e;
}
