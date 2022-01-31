/**
 * File:   input_device_status.h
 * Author: AWTK Develop Team
 * Brief:  input device status
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-11-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/time_now.h"
#include "base/keys.h"
#include "base/system_info.h"
#include "base/window_manager.h"
#include "base/input_device_status.h"

static ret_t input_device_status_init_key_event(input_device_status_t* ids, key_event_t* evt);

input_device_status_t* input_device_status_init(input_device_status_t* ids) {
  return_value_if_fail(ids != NULL, NULL);
  memset(ids, 0x00, sizeof(input_device_status_t));

  return ids;
}

static key_pressed_info_t* input_device_status_find_press_info(input_device_status_t* ids,
                                                               uint32_t key) {
  uint32_t i = 0;
  for (i = 0; i < MAX_PRESSED_KEYS_NR; i++) {
    key_pressed_info_t* iter = ids->pressed_info + i;
    if (iter->key == key) {
      return iter;
    }
  }

  return NULL;
}

static bool_t input_device_status_has_pressed_key(input_device_status_t* ids) {
  uint32_t i = 0;
  for (i = 0; i < MAX_PRESSED_KEYS_NR; i++) {
    key_pressed_info_t* iter = ids->pressed_info + i;
    if (iter->key) {
      return TRUE;
    }
  }

  return FALSE;
}

ret_t input_device_status_abort_all_pressed_keys(input_device_status_t* ids) {
  uint32_t i = 0;
  return_value_if_fail(ids != NULL, RET_BAD_PARAMS);

  for (i = 0; i < MAX_PRESSED_KEYS_NR; i++) {
    key_pressed_info_t* iter = ids->pressed_info + i;
    if (iter->key) {
      iter->should_abort = TRUE;
    }
  }

  return RET_OK;
}

static ret_t input_device_status_dispatch_long_press(input_device_status_t* ids) {
  uint32_t i = 0;
  key_event_t evt;
  uint64_t now = time_now_ms();
  widget_t* widget = ids->widget;

  for (i = 0; i < MAX_PRESSED_KEYS_NR; i++) {
    key_pressed_info_t* iter = ids->pressed_info + i;
    if (iter->key && !iter->emitted && !iter->should_abort) {
      uint64_t t = now - iter->time;
      if (t >= TK_KEY_LONG_PRESS_TIME) {
        window_manager_t* wm = WINDOW_MANAGER(window_manager());
        event_t* e = key_event_init(&evt, EVT_KEY_LONG_PRESS, wm->global_emitter, iter->key);

        input_device_status_init_key_event(ids, &evt);
        if (emitter_dispatch(wm->global_emitter, e) != RET_STOP) {
          e->target = widget;
          widget_on_keydown(widget, &evt);
        }

        log_debug("long press:%d\n", iter->key);
        iter->emitted = TRUE;
      }
    }
  }

  return RET_OK;
}

static ret_t long_press_check_on_timer(const timer_info_t* info) {
  input_device_status_t* ids = (input_device_status_t*)(info->ctx);

  input_device_status_dispatch_long_press(ids);

  if (input_device_status_has_pressed_key(ids)) {
    return RET_REPEAT;
  } else {
    ids->long_press_check_timer = TK_INVALID_ID;
    return RET_REMOVE;
  }
}

static ret_t input_device_status_update_key_press_info(input_device_status_t* ids, uint32_t key,
                                                       bool_t down) {
  key_pressed_info_t* info = input_device_status_find_press_info(ids, key);

  if (down) {
    if (info == NULL) {
      info = input_device_status_find_press_info(ids, 0);
      return_value_if_fail(info != NULL, RET_BAD_PARAMS);
      info->key = key;
      info->time = time_now_ms();
    }

    if (ids->long_press_check_timer == TK_INVALID_ID) {
      ids->long_press_check_timer =
          timer_add(long_press_check_on_timer, ids, TK_KEY_LONG_PRESS_TIME);
    }
  } else {
    return_value_if_fail(info != NULL, RET_BAD_PARAMS);
    memset(info, 0x00, sizeof(key_pressed_info_t));
  }

  return RET_OK;
}

static ret_t input_device_status_update_key_status(input_device_status_t* ids, uint32_t key,
                                                   bool_t down) {
  if (key == TK_KEY_LSHIFT) {
    ids->shift = down;
    ids->lshift = down;
  } else if (key == TK_KEY_RSHIFT) {
    ids->shift = down;
    ids->rshift = down;
  } else if (key == TK_KEY_LALT) {
    ids->alt = down;
    ids->lalt = down;
  } else if (key == TK_KEY_RALT) {
    ids->alt = down;
    ids->ralt = down;
  } else if (key == TK_KEY_LCTRL) {
    ids->ctrl = down;
    ids->lctrl = down;
  } else if (key == TK_KEY_RCTRL) {
    ids->ctrl = down;
    ids->rctrl = down;
  } else if (key == TK_KEY_MENU) {
    ids->menu = down;
  } else if (key == TK_KEY_LCOMMAND || key == TK_KEY_RCOMMAND) {
    ids->cmd = down;
  }

  if (!down) {
    if (key == TK_KEY_CAPSLOCK) {
      ids->capslock = !(ids->capslock);
    }
  }
  input_device_status_update_key_press_info(ids, key, down);

  return RET_OK;
}

typedef struct _key_shift_t {
  char key;
  char shift_key;
} key_shift_t;

static const key_shift_t key_shift[] = {
    {'`', '~'}, {'1', '!'}, {'2', '@'}, {'3', '#'},  {'4', '$'}, {'5', '%'}, {'6', '^'},
    {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'},  {'-', '_'}, {'=', '+'}, {'[', '{'},
    {']', '}'}, {',', '<'}, {'.', '>'}, {'\\', '|'}, {'/', '?'}, {';', ':'}, {'\'', '\"'}};

static uint32_t input_device_status_get_shift_key_code(input_device_status_t* ids,
                                                       uint32_t old_key_code) {
  char c = (char)old_key_code;

  if (ids->shift) {
    uint32_t i = 0;
    for (i = 0; i < ARRAY_SIZE(key_shift); i++) {
      if (key_shift[i].key == c) {
        return key_shift[i].shift_key;
      }
    }
  }

  if (ids->shift && ids->capslock) {
    if (c >= 'A' && c <= 'Z') {
      return tolower(c);
    }
  }

  if (ids->shift || ids->capslock) {
    if (c >= 'a' && c <= 'z') {
      return toupper(c);
    }
  }
  return old_key_code;
}

static ret_t input_device_status_shift_key(input_device_status_t* ids, key_event_t* e) {
  e->key = input_device_status_get_shift_key_code(ids, e->key);

  return RET_OK;
}

static ret_t input_device_status_init_pointer_event(input_device_status_t* ids,
                                                    pointer_event_t* evt) {
  return_value_if_fail(ids != NULL && evt != NULL, RET_BAD_PARAMS);
  evt->alt = ids->alt;
  evt->ctrl = ids->ctrl;
  evt->shift = ids->shift;
  evt->cmd = ids->cmd;
  evt->menu = ids->menu;

  return RET_OK;
}

static ret_t input_device_status_init_wheel_event(input_device_status_t* ids, wheel_event_t* evt) {
  return_value_if_fail(ids != NULL && evt != NULL, RET_BAD_PARAMS);
  evt->alt = ids->alt;
  evt->ctrl = ids->ctrl;
  evt->shift = ids->shift;

  return RET_OK;
}

static ret_t input_device_status_init_key_event(input_device_status_t* ids, key_event_t* evt) {
  return_value_if_fail(ids != NULL && evt != NULL, RET_BAD_PARAMS);
  evt->alt = ids->alt;
  evt->ctrl = ids->ctrl;
  evt->shift = ids->shift;
  evt->lalt = ids->lalt;
  evt->lctrl = ids->lctrl;
  evt->lshift = ids->lshift;
  evt->ralt = ids->ralt;
  evt->rctrl = ids->rctrl;
  evt->rshift = ids->rshift;
  evt->capslock = ids->capslock;
  evt->cmd = ids->cmd;
  evt->menu = ids->menu;

  return RET_OK;
}

ret_t input_device_status_on_input_event(input_device_status_t* ids, widget_t* widget, event_t* e) {
  window_manager_t* wm = WINDOW_MANAGER(widget_get_window_manager(widget));
  return_value_if_fail(ids != NULL && e != NULL, RET_BAD_PARAMS);

  ids->widget = widget;
  switch (e->type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t* evt = (pointer_event_t*)e;
      pointer_event_rotate(evt, system_info());

      ids->pressed = TRUE;
      ids->last_x = evt->x;
      ids->last_y = evt->y;
      input_device_status_init_pointer_event(ids, evt);

      widget_on_pointer_down(widget, evt);

      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      pointer_event_rotate(evt, system_info());

      if (evt->x != ids->last_x || evt->y != ids->last_y) {
        ids->last_x = evt->x;
        ids->last_y = evt->y;

        input_device_status_init_pointer_event(ids, evt);
        widget_on_pointer_move(widget, evt);
      }
      break;
    }
    case EVT_POINTER_UP: {
      pointer_event_t* evt = (pointer_event_t*)e;
      pointer_event_rotate(evt, system_info());

      input_device_status_init_pointer_event(ids, evt);
      widget_on_pointer_up(widget, evt);

      ids->last_x = evt->x;
      ids->last_y = evt->y;
      ids->pressed = FALSE;
      break;
    }
    case EVT_CONTEXT_MENU: {
      pointer_event_t* evt = (pointer_event_t*)e;
      pointer_event_rotate(evt, system_info());

      input_device_status_init_pointer_event(ids, evt);
      widget_on_context_menu(widget, evt);

      break;
    }
    case EVT_KEY_DOWN: {
      key_event_t* evt = (key_event_t*)e;
      uint32_t key = input_device_status_get_shift_key_code(ids, evt->key);
      key_pressed_info_t* info = input_device_status_find_press_info(ids, key);

      input_device_status_update_key_status(ids, key, TRUE);
      input_device_status_init_key_event(ids, evt);
      input_device_status_shift_key(ids, evt);

      if (info == NULL || !info->should_abort) {
        if (wm->widget_grab_key != NULL) {
          widget_on_keydown(wm->widget_grab_key, evt);
        } else {
          widget_on_keydown(widget, evt);
        }
      }
      break;
    }
    case EVT_KEY_UP: {
      key_event_t* evt = (key_event_t*)e;
      key_pressed_info_t* info = input_device_status_find_press_info(ids, evt->key);

      input_device_status_init_key_event(ids, evt);
      input_device_status_shift_key(ids, evt);

      if (info == NULL || !info->should_abort) {
        if (wm->widget_grab_key != NULL) {
          widget_on_keyup(wm->widget_grab_key, evt);
        } else {
          widget_on_keyup(widget, evt);
        }
      }

      input_device_status_update_key_status(ids, evt->key, FALSE);
      break;
    }
    case EVT_WHEEL: {
      wheel_event_t* evt = (wheel_event_t*)e;

      input_device_status_init_wheel_event(ids, evt);
      widget_on_wheel(widget, evt);
      break;
    }
    case EVT_MULTI_GESTURE: {
      multi_gesture_event_t* evt = (multi_gesture_event_t*)e;
      widget_on_multi_gesture(widget, evt);
      break;
    }
    default:
      break;
  }

  return RET_OK;
}
