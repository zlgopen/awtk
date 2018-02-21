/**
 * File:   events.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  events structs
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_EVENTS_H
#define LFTK_EVENTS_H

#include "types_def.h"

BEGIN_C_DECLS

typedef enum _event_type_t {
  EVT_NONE,
  EVT_POINTER_DOWN,
  EVT_POINTER_MOVE,
  EVT_POINTER_UP,
  EVT_POINTER_ENTER,
  EVT_POINTER_LEAVE,
  EVT_CLICK,
  EVT_KEY_DOWN,
  EVT_KEY_UP,
  EVT_MOVE,
  EVT_RESIZE,
  EVT_DESTROY,
  EVT_MOVE_RESIZE,
  EVT_PROP_CHANGED
}event_type_t;

typedef struct _event_t {
  uint16_t type;
  void* target;
}event_t;

typedef struct _pointer_event_t {
  event_t e;
  xy_t x;
  xy_t y;
  uint8_t button;
  uint8_t pressed:1;
}pointer_event_t;

typedef struct _key_event_t {
  event_t  e;
  uint8_t key;
  uint8_t alt:1;
  uint8_t ctrl:1;
  uint8_t shift:1;
}key_event_t;

typedef ret_t (*event_handler)(void* ctx, event_t* e);

END_C_DECLS

#endif/**LFTK_EVENTS_H*/

