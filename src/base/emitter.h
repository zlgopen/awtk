/**
 * File:   emitter.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  emitter dispatcher
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

#ifndef LFTK_EMITTER_H
#define LFTK_EMITTER_H

#include "base/events.h"

BEGIN_C_DECLS

typedef struct _emitter_item_t {
  uint32_t id;
  uint16_t type;
  void* ctx;
  event_func_t handler;
} emitter_item_t;

typedef struct _emitter_t {
  uint8_t size;
  uint8_t capacity;
  uint8_t stop;
  uint8_t enable;
  emitter_item_t* items;
  uint32_t curr_id;
} emitter_t;

emitter_t* emitter_create(void);
emitter_t* emitter_init(emitter_t* emitter);
ret_t emitter_dispatch(emitter_t* emitter, event_t* e);

uint32_t emitter_on(emitter_t* emitter, uint16_t etype, event_func_t handler, void* ctx);
ret_t emitter_off(emitter_t* emitter, uint32_t id);
emitter_item_t* emitter_find(emitter_t* emitter, uint32_t id);
ret_t emitter_off_by_func(emitter_t* emitter, uint16_t etype, event_func_t handler, void* ctx);

ret_t emitter_stop(emitter_t* emitter);
ret_t emitter_enable(emitter_t* emitter);
ret_t emitter_disable(emitter_t* emitter);

ret_t emitter_deinit(emitter_t* emitter);
ret_t emitter_destroy(emitter_t* emitter);

END_C_DECLS

#endif /*LFTK_EMITTER_H*/
