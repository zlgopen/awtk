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

#include "base/events.h"
#include "base/time_now.h"

wheel_event_t* wheel_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_WHEEL, NULL);

  return (wheel_event_t*)event;
}

prop_change_event_t* prop_change_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_PROP_CHANGED || event->type == EVT_PROP_WILL_CHANGE,
                       NULL);

  return (prop_change_event_t*)event;
}

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

event_t event_init(uint32_t type, void* target) {
  event_t e;

  e.type = type;
  e.target = target;
  e.time = time_now_ms();

  return e;
}

ret_t pointer_event_rotate(pointer_event_t* evt, system_info_t* info) {
  xy_t x = evt->x;
  xy_t y = evt->y;
  return_value_if_fail(evt != NULL && info != NULL, RET_BAD_PARAMS);

  switch (info->lcd_orientation) {
    case LCD_ORIENTATION_90: {
      evt->y = x;
      evt->x = info->lcd_h - y - 1;
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

event_t* event_cast(event_t* event) {
  return event;
}
