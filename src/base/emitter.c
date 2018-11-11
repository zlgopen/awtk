/**
 * File:   emitter.c
 * Author: AWTK Develop Team
 * Brief:  emitter dispatcher
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
 * 2018-01-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/emitter.h"
#include "base/time_now.h"

emitter_t* emitter_create() {
  emitter_t* emitter = (emitter_t*)TKMEM_ZALLOC(emitter_t);

  return emitter_init(emitter);
}

emitter_t* emitter_init(emitter_t* emitter) {
  return_value_if_fail(emitter, NULL);

  memset(emitter, 0x00, sizeof(emitter_t));
  emitter->enable = TRUE;
  emitter->next_id = TK_INVALID_ID + 1;

  return emitter;
}

static ret_t emitter_item_destroy(emitter_item_t* iter) {
  if (iter->on_destroy) {
    iter->on_destroy(iter);
  }

  memset(iter, 0x00, sizeof(emitter_item_t));
  TKMEM_FREE(iter);

  return RET_OK;
}

static ret_t emitter_remove(emitter_t* emitter, emitter_item_t* prev, emitter_item_t* iter) {
  return_value_if_fail(emitter != NULL && iter != NULL, RET_BAD_PARAMS);

  if (emitter->curr_iter == iter) {
    emitter->remove_curr_iter = TRUE;
    return RET_OK;
  }

  if (iter == emitter->items) {
    emitter->items = iter->next;
  } else {
    prev->next = iter->next;
  }

  emitter_item_destroy(iter);

  return RET_OK;
}

ret_t emitter_dispatch(emitter_t* emitter, event_t* e) {
  ret_t ret = RET_OK;
  return_value_if_fail(emitter != NULL && e != NULL, RET_BAD_PARAMS);

  if (!(e->time)) {
    e->time = time_now_ms();
  }

  if (e->target == NULL) {
    e->target = emitter;
  }

  if (emitter->enable && emitter->items) {
    emitter_item_t* iter = emitter->items;
    emitter_item_t* prev = emitter->items;

    while (iter != NULL) {
      emitter->curr_iter = iter;
      if (iter->type == e->type) {
        ret = iter->handler(iter->ctx, e);
        if (ret == RET_STOP) {
          return ret;
        } else if (ret == RET_REMOVE || emitter->remove_curr_iter) {
          emitter_item_t* next = iter->next;

          emitter->curr_iter = NULL;
          emitter->remove_curr_iter = FALSE;
          emitter_remove(emitter, prev, iter);
          iter = next;

          continue;
        }
      }

      prev = iter;
      iter = iter->next;
    }
  }
  emitter->curr_iter = NULL;
  emitter->remove_curr_iter = FALSE;

  return RET_OK;
}

uint32_t emitter_on(emitter_t* emitter, uint32_t etype, event_func_t handler, void* ctx) {
  emitter_item_t* iter = NULL;
  return_value_if_fail(emitter != NULL && handler != NULL, TK_INVALID_ID);

  iter = TKMEM_ZALLOC(emitter_item_t);
  return_value_if_fail(iter != NULL, TK_INVALID_ID);

  iter->type = etype;
  iter->ctx = ctx;
  iter->handler = handler;
  iter->id = emitter->next_id++;
  iter->next = emitter->items;
  emitter->items = iter;

  return iter->id;
}

emitter_item_t* emitter_find(emitter_t* emitter, uint32_t id) {
  return_value_if_fail(emitter != NULL, NULL);

  if (emitter->items) {
    emitter_item_t* iter = emitter->items;

    while (iter != NULL) {
      if (iter->id == id) {
        return iter;
      }

      iter = iter->next;
    }
  }

  return NULL;
}

uint32_t emitter_size(emitter_t* emitter) {
  uint32_t size = 0;
  return_value_if_fail(emitter != NULL, size);

  if (emitter->items) {
    emitter_item_t* iter = emitter->items;

    while (iter != NULL) {
      size++;
      iter = iter->next;
    }
  }

  return size;
}

ret_t emitter_off(emitter_t* emitter, uint32_t id) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);

  if (emitter->items) {
    emitter_item_t* iter = emitter->items;
    emitter_item_t* prev = emitter->items;

    while (iter != NULL) {
      if (iter->id == id) {
        return emitter_remove(emitter, prev, iter);
      }

      prev = iter;
      iter = iter->next;
    }
  }

  return RET_FAIL;
}

ret_t emitter_off_by_func(emitter_t* emitter, uint32_t etype, event_func_t handler, void* ctx) {
  return_value_if_fail(emitter != NULL && handler != NULL, RET_BAD_PARAMS);

  if (emitter->items) {
    emitter_item_t* iter = emitter->items;
    emitter_item_t* prev = emitter->items;

    while (iter != NULL) {
      if (iter->type == etype && iter->ctx == ctx && iter->handler == handler) {
        return emitter_remove(emitter, prev, iter);
      }

      prev = iter;
      iter = iter->next;
    }
  }

  return RET_FAIL;
}

ret_t emitter_enable(emitter_t* emitter) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);
  emitter->enable = TRUE;

  return RET_OK;
}

ret_t emitter_disable(emitter_t* emitter) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);
  emitter->enable = FALSE;

  return RET_OK;
}

ret_t emitter_deinit(emitter_t* emitter) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);
  if (emitter->items) {
    emitter_item_t* iter = emitter->items;
    emitter_item_t* next = emitter->items;

    while (iter != NULL) {
      next = iter->next;
      emitter_item_destroy(iter);
      iter = next;
    }
    emitter->items = NULL;
  }

  return RET_OK;
}

ret_t emitter_destroy(emitter_t* emitter) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);
  emitter_deinit(emitter);
  TKMEM_FREE(emitter);

  return RET_OK;
}

ret_t emitter_set_on_destroy(emitter_t* emitter, uint32_t id, tk_destroy_t on_destroy,
                             void* on_destroy_ctx) {
  emitter_item_t* item = emitter_find(emitter, id);
  return_value_if_fail(item != NULL, RET_BAD_PARAMS);

  item->on_destroy = on_destroy;
  item->on_destroy_ctx = on_destroy_ctx;

  return RET_OK;
}
