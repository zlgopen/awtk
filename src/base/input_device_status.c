/**
 * File:   input_device_status.h
 * Author: AWTK Develop Team
 * Brief:  input device status
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static key_long_press_info_t* key_long_press_info_create(uint32_t key, uint32_t time) {
  key_long_press_info_t* info = TKMEM_ZALLOC(key_long_press_info_t);
  return_value_if_fail(info != NULL, NULL);

  info->key = key;
  info->time = time;

  return info;
}

static void key_long_press_info_destroy(key_long_press_info_t* info) {
  TKMEM_FREE(info);
}

key_long_press_info_t* input_device_status_find_key_long_press_info(input_device_status_t* ids,
                                                                    uint32_t key) {
  uint32_t i = 0;
  darray_t* arr = NULL;
  return_value_if_fail(ids != NULL, NULL);

  arr = &(ids->key_long_press_time);
  for (i = 0; i < arr->size; i++) {
    key_long_press_info_t* iter = (key_long_press_info_t*)darray_get(arr, i);
    if (iter->key == key) {
      return iter;
    }
  }

  return NULL;
}

static ret_t input_device_status_init_key_event(input_device_status_t* ids, key_event_t* evt);

input_device_status_t* input_device_status_init(input_device_status_t* ids) {
  return_value_if_fail(ids != NULL, NULL);
  memset(ids, 0x00, sizeof(input_device_status_t));
  darray_init(&(ids->key_long_press_time), 2, (tk_destroy_t)key_long_press_info_destroy, NULL);

  return ids;
}

ret_t input_device_status_set_key_long_press_time(input_device_status_t* ids, uint32_t key,
                                                  uint32_t time) {
  key_long_press_info_t* info = input_device_status_find_key_long_press_info(ids, key);
  return_value_if_fail(ids != NULL, RET_BAD_PARAMS);

  if (info == NULL) {
    info = key_long_press_info_create(key, time);
    return_value_if_fail(info != NULL, RET_OOM);
    darray_push(&(ids->key_long_press_time), info);
  } else {
    info->time = time;
  }

  return RET_OK;
}

ret_t input_device_status_deinit(input_device_status_t* ids) {
  return_value_if_fail(ids != NULL, RET_BAD_PARAMS);

  if (ids->long_press_check_timer != TK_INVALID_ID) {
    timer_remove(ids->long_press_check_timer);
    ids->long_press_check_timer = TK_INVALID_ID;
  }
  darray_deinit(&(ids->key_long_press_time));

  return RET_OK;
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
      key_long_press_info_t* info = input_device_status_find_key_long_press_info(ids, iter->key);
      uint32_t long_press_time = info ? info->time : TK_KEY_LONG_PRESS_TIME;

      if (t >= long_press_time) {
        window_manager_t* wm = WINDOW_MANAGER(window_manager());
        event_t* e = key_event_init(&evt, EVT_KEY_LONG_PRESS, wm->global_emitter, iter->key);

        input_device_status_init_key_event(ids, &evt);
        if (emitter_dispatch(wm->global_emitter, e) != RET_STOP) {
          e->target = widget;
          widget_on_keydown(widget, &evt);
        }

        log_debug("long press:%d long_press_time=%d\n", iter->key, long_press_time);
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
                                                   key_event_t* e, bool_t down) {
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

  ids->capslock = e->capslock;
  ids->numlock = e->numlock;

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

static uint32_t input_device_status_get_shift_key_code(input_device_status_t* ids, uint32_t key) {
  if (ids->shift) {
    uint32_t i = 0;
    for (i = 0; i < ARRAY_SIZE(key_shift); i++) {
      if (key_shift[i].key == key) {
        return key_shift[i].shift_key;
      }
    }
  }

  if (ids->shift && ids->capslock) {
    if (key >= 'a' && key <= 'z') {
      return key;
    } else if (key >= 'A' && key <= 'Z') {
      return tolower(key);
    }
  }

  if (ids->shift || ids->capslock) {
    if (key >= 'a' && key <= 'z') {
      return toupper(key);
    } else if (key >= 'A' && key <= 'Z') {
      return key;
    }
  }

  switch (key) {
    case TK_KEY_KP_DIVIDE:
      return TK_KEY_SLASH;
    case TK_KEY_KP_MULTIPLY:
      return TK_KEY_ASTERISK;
    case TK_KEY_KP_MINUS:
      return TK_KEY_MINUS;
    case TK_KEY_KP_PLUS:
      return TK_KEY_PLUS;
    case TK_KEY_KP_ENTER:
      return TK_KEY_RETURN;
    case TK_KEY_KP_1:
      return ids->numlock ? TK_KEY_1 : TK_KEY_END;
    case TK_KEY_KP_2:
      return ids->numlock ? TK_KEY_2 : TK_KEY_DOWN;
    case TK_KEY_KP_3:
      return ids->numlock ? TK_KEY_3 : TK_KEY_PAGEDOWN;
    case TK_KEY_KP_4:
      return ids->numlock ? TK_KEY_4 : TK_KEY_LEFT;
    case TK_KEY_KP_5:
      return ids->numlock ? TK_KEY_5 : TK_KEY_UNKNOWN;
    case TK_KEY_KP_6:
      return ids->numlock ? TK_KEY_6 : TK_KEY_RIGHT;
    case TK_KEY_KP_7:
      return ids->numlock ? TK_KEY_7 : TK_KEY_HOME;
    case TK_KEY_KP_8:
      return ids->numlock ? TK_KEY_8 : TK_KEY_UP;
    case TK_KEY_KP_9:
      return ids->numlock ? TK_KEY_9 : TK_KEY_PAGEUP;
    case TK_KEY_KP_0:
      return ids->numlock ? TK_KEY_0 : TK_KEY_INSERT;
    case TK_KEY_KP_PERIOD:
      return ids->numlock ? TK_KEY_PERIOD : TK_KEY_DELETE;
    default:
      break;
  }

  return key;
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
  evt->x = ids->last_x;
  evt->y = ids->last_y;
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
  evt->numlock = ids->numlock;
  evt->cmd = ids->cmd;
  evt->menu = ids->menu;

  return RET_OK;
}

static ret_t input_device_status_dispatch_input_event(input_device_status_t* ids, widget_t* widget,
                                                      event_t* e, bool_t dispatch) {
  window_manager_t* wm = WINDOW_MANAGER(widget_get_window_manager(widget));
  return_value_if_fail(ids != NULL && e != NULL, RET_BAD_PARAMS);

  ids->widget = widget;
  switch (e->type) {
    case EVT_POINTER_DOWN: {
      if (dispatch) {
        pointer_event_t* evt = (pointer_event_t*)e;
        pointer_event_rotate(evt, system_info());

        ids->pressed = TRUE;
        ids->last_x = evt->x;
        ids->last_y = evt->y;
        ids->last_pointer_down_time = e->time;
        input_device_status_init_pointer_event(ids, evt);

        widget_on_pointer_down(widget, evt);
      }
      break;
    }
    case EVT_POINTER_MOVE: {
      if (dispatch) {
        pointer_event_t* evt = (pointer_event_t*)e;
        pointer_event_rotate(evt, system_info());

        if (evt->x != ids->last_x || evt->y != ids->last_y || ids->pointer_has_left_window) {
          ids->last_x = evt->x;
          ids->last_y = evt->y;

          input_device_status_init_pointer_event(ids, evt);
          widget_on_pointer_move(widget, evt);
        }
      }
      break;
    }
    case EVT_POINTER_UP: {
      if (dispatch || ids->pressed) {
        uint32_t delta_time = e->time - ids->last_pointer_up_time;
        int32_t delta_x, delta_y;
        pointer_event_t* evt = (pointer_event_t*)e;
        pointer_event_rotate(evt, system_info());
        delta_x = evt->x - ids->last_pointer_up_x;
        delta_y = evt->y - ids->last_pointer_up_y;

        ids->in_pointer_up = TRUE;
        input_device_status_init_pointer_event(ids, evt);
        widget_on_pointer_up(widget, evt);
        ids->in_pointer_up = FALSE;

        if (delta_time < TK_DOUBLE_CLICK_TIME && tk_abs(delta_x) < TK_DOUBLE_CLICK_XY &&
            tk_abs(delta_y) < TK_DOUBLE_CLICK_XY) {
          event_t* d_evt;
          pointer_event_t double_click;
          d_evt = pointer_event_init(&double_click, EVT_DOUBLE_CLICK, widget, evt->x, evt->y);
          widget_dispatch_event_to_target_recursive(widget, d_evt);
          log_debug("double clicked\n");
        }

        ids->last_x = evt->x;
        ids->last_y = evt->y;
        ids->pressed = FALSE;
        ids->last_pointer_up_time = e->time;
        ids->last_pointer_up_x = evt->x;
        ids->last_pointer_up_y = evt->y;
      }
      break;
    }
    case EVT_CONTEXT_MENU: {
      if (dispatch) {
        pointer_event_t* evt = (pointer_event_t*)e;
        pointer_event_rotate(evt, system_info());

        input_device_status_init_pointer_event(ids, evt);
        widget_on_context_menu(widget, evt);
      }
      break;
    }
    case EVT_KEY_DOWN: {
      if (dispatch) {
        key_event_t* evt = (key_event_t*)e;
        key_pressed_info_t* info = NULL;
        input_device_status_update_key_status(ids, evt->key, evt, TRUE);
        input_device_status_shift_key(ids, evt);
        if (evt->key == TK_KEY_UNKNOWN) {
          break;
        }

        input_device_status_update_key_press_info(ids, evt->key, TRUE);

        info = input_device_status_find_press_info(ids, evt->key);

        input_device_status_init_key_event(ids, evt);
        if (info == NULL || !info->should_abort) {
          if (wm->widget_grab_key != NULL) {
            widget_on_keydown(wm->widget_grab_key, evt);
          } else {
            widget_on_keydown(widget, evt);
          }
        }
      }
      break;
    }
    case EVT_KEY_UP: {
      key_event_t* evt = (key_event_t*)e;
      key_pressed_info_t* info = NULL;
      input_device_status_shift_key(ids, evt);
      input_device_status_update_key_status(ids, evt->key, evt, FALSE);
      if (evt->key == TK_KEY_UNKNOWN) {
        break;
      }

      info = input_device_status_find_press_info(ids, evt->key);

      if (dispatch || info != NULL) {
        input_device_status_init_key_event(ids, evt);
        if (info == NULL || !info->should_abort) {
          if (wm->widget_grab_key != NULL) {
            widget_on_keyup(wm->widget_grab_key, evt);
          } else {
            widget_on_keyup(widget, evt);
          }
        }

        input_device_status_update_key_press_info(ids, evt->key, FALSE);
      }
      break;
    }
    case EVT_WHEEL: {
      if (dispatch) {
        wheel_event_t* evt = (wheel_event_t*)e;

        input_device_status_init_wheel_event(ids, evt);
        widget_on_wheel(widget, evt);
      }
      break;
    }
    case EVT_MULTI_GESTURE: {
      if (dispatch) {
        multi_gesture_event_t* evt = (multi_gesture_event_t*)e;
        widget_on_multi_gesture(widget, evt);
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

ret_t input_device_status_on_input_event(input_device_status_t* ids, widget_t* widget, event_t* e) {
  return input_device_status_dispatch_input_event(ids, widget, e, TRUE);
}

ret_t input_device_status_on_ignore_input_event(input_device_status_t* ids, widget_t* widget,
                                                event_t* e) {
  return input_device_status_dispatch_input_event(ids, widget, e, FALSE);
}

ret_t input_device_status_on_pointer_enter(input_device_status_t* ids, widget_t* widget, xy_t x,
                                           xy_t y) {
  pointer_event_t e;
  ret_t ret = RET_OK;
  return_value_if_fail(ids != NULL && widget != NULL, RET_BAD_PARAMS);

  pointer_event_init(&e, EVT_POINTER_MOVE, widget, x, y);
  ret = input_device_status_on_input_event(ids, widget, (event_t*)&e);
  ids->pointer_has_left_window = FALSE;

  return ret;
}

ret_t input_device_status_on_pointer_leave(input_device_status_t* ids, widget_t* widget) {
  pointer_event_t e;
  return_value_if_fail(ids != NULL && widget != NULL, RET_BAD_PARAMS);

  ids->pointer_has_left_window = TRUE;
  pointer_event_init(&e, EVT_POINTER_LEAVE, widget, 0, 0);

  return widget_dispatch_leave_event(widget, &e);
}

ret_t input_device_status_abort_keys_and_mouse(input_device_status_t* ids) {
  return_value_if_fail(ids != NULL, RET_BAD_PARAMS);

  ids->cmd = FALSE;
  ids->alt = FALSE;
  ids->menu = FALSE;
  ids->lalt = FALSE;
  ids->ralt = FALSE;
  ids->ctrl = FALSE;
  ids->lctrl = FALSE;
  ids->rctrl = FALSE;
  ids->shift = FALSE;
  ids->lshift = FALSE;
  ids->rshift = FALSE;

  return RET_OK;
}
