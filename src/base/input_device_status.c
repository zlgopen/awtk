/**
 * File:   input_device_status.h
 * Author: AWTK Develop Team
 * Brief:  input device status
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
 * 2018-11-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/keys.h"
#include "base/system_info.h"
#include "base/input_device_status.h"

input_device_status_t* input_device_status_init(input_device_status_t* ids) {
  return_value_if_fail(ids != NULL, NULL);
  memset(ids, 0x00, sizeof(input_device_status_t));

  return ids;
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

static ret_t input_device_status_shift_key(input_device_status_t* ids, key_event_t* e) {
  char c = (char)e->key;

  if (ids->shift) {
    uint32_t i = 0;
    for (i = 0; i < ARRAY_SIZE(key_shift); i++) {
      if (key_shift[i].key == c) {
        e->key = key_shift[i].shift_key;
        return RET_OK;
      }
    }
  }

  if (ids->shift && ids->capslock) {
    if (c >= 'A' && c <= 'Z') {
      e->key = tolower(c);
    }

    return RET_OK;
  }

  if (ids->shift || ids->capslock) {
    if (c >= 'a' && c <= 'z') {
      e->key = toupper(c);
    }
  }

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
  return_value_if_fail(ids != NULL && e != NULL, RET_BAD_PARAMS);

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

      input_device_status_update_key_status(ids, evt->key, TRUE);
      input_device_status_init_key_event(ids, evt);
      input_device_status_shift_key(ids, evt);
      widget_on_keydown(widget, evt);
      break;
    }
    case EVT_KEY_UP: {
      key_event_t* evt = (key_event_t*)e;

      input_device_status_init_key_event(ids, evt);
      input_device_status_shift_key(ids, evt);
      widget_on_keyup(widget, evt);

      input_device_status_update_key_status(ids, evt->key, FALSE);
      break;
    }
    case EVT_WHEEL: {
      wheel_event_t* evt = (wheel_event_t*)e;

      input_device_status_init_wheel_event(ids, evt);
      widget_on_wheel(widget, evt);
      break;
    }
    default:
      break;
  }

  return RET_OK;
}
