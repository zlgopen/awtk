/**
 * File:   events.c
 * Author: AWTK Develop Team
 * Brief:  events structs
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
 * 2018-03-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/events.h"
#include "tkc/time_now.h"

wheel_event_t* wheel_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_WHEEL, NULL);

  return (wheel_event_t*)event;
}

orientation_event_t* orientation_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(
      event->type == EVT_ORIENTATION_CHANGED || event->type == EVT_ORIENTATION_WILL_CHANGED, NULL);

  return (orientation_event_t*)event;
}

pointer_event_t* pointer_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_POINTER_DOWN && event->type <= EVT_CLICK, NULL);

  return (pointer_event_t*)event;
}

key_event_t* key_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_KEY_DOWN && event->type <= EVT_KEY_UP_BEFORE_CHILDREN,
                       NULL);

  return (key_event_t*)event;
}

paint_event_t* paint_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(
      event->type == EVT_PAINT || event->type == EVT_BEFORE_PAINT || event->type == EVT_AFTER_PAINT,
      NULL);

  return (paint_event_t*)event;
}

window_event_t* window_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_WINDOW_WILL_OPEN || event->type == EVT_WINDOW_OPEN ||
                           event->type == EVT_WINDOW_CLOSE,
                       NULL);

  return (window_event_t*)event;
}

ret_t pointer_event_rotate(pointer_event_t* evt, system_info_t* info) {
  xy_t x = evt->x;
  xy_t y = evt->y;
  return_value_if_fail(evt != NULL && info != NULL, RET_BAD_PARAMS);

  switch (info->lcd_orientation) {
    case LCD_ORIENTATION_90: {
#if WITH_NANOVG_GPU
      evt->y = info->lcd_w - x - 1;
      evt->x = y;
#else
      evt->y = x;
      evt->x = info->lcd_h - y - 1;
#endif
      break;
    }
    case LCD_ORIENTATION_180: {
      evt->y = info->lcd_h - y - 1;
      evt->x = info->lcd_w - x - 1;
      break;
    }
    case LCD_ORIENTATION_270: {
#if WITH_NANOVG_GPU
      evt->y = x;
      evt->x = info->lcd_h - y - 1;
#else
      evt->y = info->lcd_w - x - 1;
      evt->x = y;
#endif
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

event_t* wheel_event_init(wheel_event_t* event, uint32_t type, void* target, int32_t dy) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(wheel_event_t));

  event->e = event_init(type, target);
  event->dy = dy;

  return (event_t*)event;
}

event_t* orientation_event_init(orientation_event_t* event, uint32_t type, void* target,
                                lcd_orientation_t orientation) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(orientation_event_t));

  event->e = event_init(type, target);
  event->orientation = orientation;

  return (event_t*)event;
}

event_t* pointer_event_init(pointer_event_t* event, uint32_t type, void* target, int32_t x,
                            int32_t y) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(pointer_event_t));

  event->e = event_init(type, target);
  event->x = x;
  event->y = y;

  return (event_t*)event;
}

event_t* key_event_init(key_event_t* event, uint32_t type, void* target, int32_t key) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(key_event_t));

  event->e = event_init(type, target);
  event->key = key;

  return (event_t*)event;
}

event_t* paint_event_init(paint_event_t* event, uint32_t type, void* target, canvas_t* c) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(paint_event_t));

  event->e = event_init(type, target);
  event->c = c;

  return (event_t*)event;
}

event_t* window_event_init(window_event_t* event, uint32_t type, void* target, widget_t* window) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(window_event_t));

  event->e = event_init(type, target);
  event->window = window;

  return (event_t*)event;
}
