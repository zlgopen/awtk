/**
 * File:   events.c
 * Author: AWTK Develop Team
 * Brief:  events structs
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  return_value_if_fail(event->type == EVT_WHEEL || event->type == EVT_WHEEL_BEFORE_CHILDREN, NULL);
  return_value_if_fail(event->size == sizeof(wheel_event_t), NULL);

  return (wheel_event_t*)event;
}

orientation_event_t* orientation_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(
      event->type == EVT_ORIENTATION_CHANGED || event->type == EVT_ORIENTATION_WILL_CHANGED, NULL);
  return_value_if_fail(event->size == sizeof(orientation_event_t), NULL);

  return (orientation_event_t*)event;
}

pointer_event_t* pointer_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_POINTER_DOWN && event->type <= EVT_CLICK, NULL);
  return_value_if_fail(event->size == sizeof(pointer_event_t), NULL);

  return (pointer_event_t*)event;
}

value_change_event_t* value_change_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_VALUE_WILL_CHANGE && event->type <= EVT_VALUE_CHANGING,
                       NULL);
  return_value_if_fail(event->size == sizeof(value_change_event_t), NULL);

  return (value_change_event_t*)event;
}

offset_change_event_t* offset_change_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->size == sizeof(offset_change_event_t), NULL);

  return (offset_change_event_t*)event;
}

key_event_t* key_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type >= EVT_KEY_DOWN && event->type <= EVT_KEY_UP_BEFORE_CHILDREN,
                       NULL);
  return_value_if_fail(event->size == sizeof(key_event_t), NULL);

  return (key_event_t*)event;
}

paint_event_t* paint_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(
      event->type == EVT_PAINT || event->type == EVT_BEFORE_PAINT || event->type == EVT_AFTER_PAINT,
      NULL);
  return_value_if_fail(event->size == sizeof(paint_event_t), NULL);

  return (paint_event_t*)event;
}

window_event_t* window_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_WINDOW_WILL_OPEN || event->type == EVT_WINDOW_OPEN ||
                           event->type == EVT_WINDOW_CLOSE,
                       NULL);
  return_value_if_fail(event->size == sizeof(window_event_t), NULL);

  return (window_event_t*)event;
}

ret_t pointer_event_rotate(pointer_event_t* evt, system_info_t* info) {
  xy_t x = evt->x;
  xy_t y = evt->y;
  return_value_if_fail(evt != NULL && info != NULL, RET_BAD_PARAMS);

  switch (info->lcd_orientation) {
    case LCD_ORIENTATION_90: {
#ifdef WITH_GPU
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
#ifdef WITH_GPU
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
  event->e.size = sizeof(*event);
  event->dy = dy;

  return (event_t*)event;
}

event_t* orientation_event_init(orientation_event_t* event, uint32_t type, void* target,
                                lcd_orientation_t old_orientation,
                                lcd_orientation_t new_orientation) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(orientation_event_t));

  event->e = event_init(type, target);
  event->e.size = sizeof(*event);
  event->orientation = new_orientation;
  event->old_orientation = old_orientation;

  return (event_t*)event;
}

event_t* pointer_event_init(pointer_event_t* event, uint32_t type, void* target, int32_t x,
                            int32_t y) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(*event));

  event->e = event_init(type, target);
  event->e.size = sizeof(*event);
  event->x = x;
  event->y = y;

  return (event_t*)event;
}

event_t* value_change_event_init(value_change_event_t* event, uint32_t type, void* target) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(*event));
  event->e = event_init(type, target);
  event->e.size = sizeof(*event);

  return (event_t*)event;
}

event_t* offset_change_event_init(offset_change_event_t* event, uint32_t type, void* target,
                                  float_t old_offset, float_t new_offset) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(*event));
  event->e = event_init(type, target);
  event->e.size = sizeof(*event);

  event->old_offset = old_offset;
  event->new_offset = new_offset;

  return (event_t*)event;
}

event_t* key_event_init(key_event_t* event, uint32_t type, void* target, int32_t key) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(key_event_t));

  event->e = event_init(type, target);
  event->e.size = sizeof(*event);
  event->key = key;

  return (event_t*)event;
}

event_t* paint_event_init(paint_event_t* event, uint32_t type, void* target, canvas_t* c) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(paint_event_t));

  event->e = event_init(type, target);
  event->e.size = sizeof(*event);
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

multi_gesture_event_t* multi_gesture_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_MULTI_GESTURE, NULL);
  return_value_if_fail(event->size == sizeof(multi_gesture_event_t), NULL);

  return (multi_gesture_event_t*)event;
}

event_t* multi_gesture_event_init(multi_gesture_event_t* event, void* target, int32_t x, int32_t y,
                                  float rotation, float distance) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(multi_gesture_event_t));

  event->e = event_init(EVT_MULTI_GESTURE, target);
  event->e.size = sizeof(*event);

  event->x = x;
  event->y = y;
  event->rotation = rotation;
  event->distance = distance;

  return (event_t*)event;
}

event_t* assets_event_init(assets_event_t* event, assets_manager_t* am, uint32_t type,
                           asset_type_t asset_type, asset_info_t* asset_info) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(assets_event_t));

  event->e = event_init(type, am);
  event->e.size = sizeof(*event);

  event->type = asset_type;
  event->asset_info = asset_info;
  return (event_t*)event;
}

theme_change_event_t* theme_change_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_THEME_CHANGED || event->type == EVT_THEME_WILL_CHANGE,
                       NULL);
  return_value_if_fail(event->size == sizeof(theme_change_event_t), NULL);

  return (theme_change_event_t*)event;
}

event_t* theme_change_event_init(theme_change_event_t* event, uint32_t type, const char* name) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(*event));

  event->e = event_init(type, NULL);
  event->e.size = sizeof(*event);
  event->name = name;

  return (event_t*)(event);
}

int32_t event_from_name(const char* name) {
  return_value_if_fail(name != NULL, EVT_NONE);

  switch (*name) {
    case 'f': {
      if (tk_str_eq(name, "focus")) {
        return EVT_FOCUS;
      }
    }
    case 'b': {
      if (tk_str_eq(name, "blur")) {
        return EVT_BLUR;
      }
    }
    case 'g': {
      if (tk_str_eq(name, "global_key_up")) {
        return EVT_KEY_UP;
      } else if (tk_str_eq(name, "global_key_down")) {
        return EVT_KEY_DOWN;
      } else if (tk_str_eq(name, "global_key_long_press")) {
        return EVT_KEY_LONG_PRESS;
      }
      break;
    }
    case 'k': {
      if (tk_str_eq(name, "key_up")) {
        return EVT_KEY_UP;
      } else if (tk_str_eq(name, "key_down")) {
        return EVT_KEY_DOWN;
      } else if (tk_str_eq(name, "key_long_press")) {
        return EVT_KEY_LONG_PRESS;
      } else if (tk_str_eq(name, "key_down_before_children")) {
        return EVT_KEY_DOWN_BEFORE_CHILDREN;
      } else if (tk_str_eq(name, "key_up_before_children")) {
        return EVT_KEY_UP_BEFORE_CHILDREN;
      }
      break;
    }
    case 'p': {
      if (tk_str_eq(name, "pointer_up")) {
        return EVT_POINTER_UP;
      } else if (tk_str_eq(name, "pointer_down")) {
        return EVT_POINTER_DOWN;
      } else if (tk_str_eq(name, "pointer_move")) {
        return EVT_POINTER_MOVE;
      }
      break;
    }
    case 'c': {
      if (tk_str_eq(name, "click")) {
        return EVT_CLICK;
      }
      break;
    }
    case 't': {
      if (tk_str_eq(name, "timer")) {
        return EVT_TIMER;
      }
      break;
    }
    case 'w': {
      if (tk_str_eq(name, "window_close")) {
        return EVT_WINDOW_CLOSE;
      } else if (tk_str_eq(name, "window_open")) {
        return EVT_WINDOW_OPEN;
      } else if (tk_str_eq(name, "window_will_open")) {
        return EVT_WINDOW_WILL_OPEN;
      }
      break;
    }
    case 'v': {
      if (tk_str_eq(name, "value_changed")) {
        return EVT_VALUE_CHANGED;
      } else if (tk_str_eq(name, STR_VALUE_CHANGED_BY_UI)) {
        return EVT_VALUE_CHANGED;
      }
      break;
    }
    case 'i': {
      if (tk_str_eq(name, "im_action")) {
        return EVT_IM_ACTION;
      }
      break;
    }
    default:
      break;
  }
  return EVT_NONE;
}
