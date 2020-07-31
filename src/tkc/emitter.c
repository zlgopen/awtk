/**
 * File:   emitter.c
 * Author: AWTK Develop Team
 * Brief:  emitter dispatcher
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
 * 2018-01-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/emitter.h"
#include "tkc/time_now.h"

emitter_t* emitter_create() {
  emitter_t* emitter = (emitter_t*)TKMEM_ZALLOC(emitter_t);

  return emitter_init(emitter);
}

static uint32_t emitter_next_id(emitter_t* emitter) {
  uint32_t id = emitter->next_id++;

  if (id == TK_INVALID_ID) {
    id = emitter->next_id++;
  }

  return id;
}

emitter_t* emitter_init(emitter_t* emitter) {
  return_value_if_fail(emitter, NULL);

  memset(emitter, 0x00, sizeof(emitter_t));
  emitter->enable = TRUE;
  emitter->next_id = TK_INVALID_ID + 1;

  return emitter;
}

#ifdef AWTK_WEB_JS
#include <emscripten.h>
#endif /*AWTK_WEB_JS*/

static ret_t emitter_item_destroy(emitter_item_t* iter) {
  if (iter->on_destroy) {
    iter->on_destroy(iter);
  }

#ifdef AWTK_WEB_JS
  EM_ASM_INT({ return TBrowser.releaseFunction($0); }, iter->handler);
#endif /*AWTK_WEB_JS*/

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

ret_t emitter_remove_item(emitter_t* emitter, emitter_item_t* item) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);

  if (emitter->items) {
    emitter_item_t* iter = emitter->items;
    emitter_item_t* prev = emitter->items;

    while (iter != NULL) {
      emitter_item_t* next = iter->next;

      if (iter == item) {
        return emitter_remove(emitter, prev, iter);
      }

      prev = iter;
      iter = next;
    }
  }

  return RET_NOT_FOUND;
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

    while (iter != NULL) {
      emitter->curr_iter = iter;
      if (iter->type == e->type) {
        ret = iter->handler(iter->ctx, e);
        if (ret == RET_STOP) {
          if (emitter->remove_curr_iter) {
            emitter->curr_iter = NULL;
            emitter->remove_curr_iter = FALSE;
            emitter_remove_item(emitter, iter);
          }
          return ret;
        } else if (ret == RET_REMOVE || emitter->remove_curr_iter) {
          emitter_item_t* next = iter->next;

          emitter->curr_iter = NULL;
          emitter->remove_curr_iter = FALSE;
          emitter_remove_item(emitter, iter);
          iter = next;

          continue;
        }
      }

      iter = iter->next;
    }
  }
  emitter->curr_iter = NULL;
  emitter->remove_curr_iter = FALSE;

  return RET_OK;
}

uint32_t emitter_on_with_tag(emitter_t* emitter, uint32_t etype, event_func_t handler, void* ctx,
                             uint32_t tag) {
  emitter_item_t* iter = NULL;
  return_value_if_fail(emitter != NULL && handler != NULL, TK_INVALID_ID);

  iter = TKMEM_ZALLOC(emitter_item_t);
  return_value_if_fail(iter != NULL, TK_INVALID_ID);

  iter->tag = tag;
  iter->ctx = ctx;
  iter->type = etype;
  iter->handler = handler;
  iter->id = emitter_next_id(emitter);
  iter->next = emitter->items;
  emitter->items = iter;

  return iter->id;
}

bool_t emitter_exist(emitter_t* emitter, uint32_t etype, event_func_t handler, void* ctx) {
  return_value_if_fail(emitter != NULL, FALSE);

  if (emitter->items) {
    emitter_item_t* iter = emitter->items;

    while (iter != NULL) {
      if (iter->handler == handler && iter->type == etype && iter->ctx == ctx) {
        return TRUE;
      }

      iter = iter->next;
    }
  }

  return FALSE;
  ;
}

uint32_t emitter_on(emitter_t* emitter, uint32_t etype, event_func_t handler, void* ctx) {
  return emitter_on_with_tag(emitter, etype, handler, ctx, 0);
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

static ret_t emitter_off_ex(emitter_t* emitter, tk_compare_t cmp, void* ctx) {
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);

  if (emitter->items) {
    emitter_item_t* iter = emitter->items;
    emitter_item_t* prev = emitter->items;

    while (iter != NULL) {
      if ((emitter->curr_iter != iter || !emitter->remove_curr_iter) && cmp(iter, ctx) == 0) {
        return emitter_remove(emitter, prev, iter);
      }

      prev = iter;
      iter = iter->next;
    }
  }

  return RET_FAIL;
}

static int emitter_item_compare_by_tag(const void* a, const void* b) {
  const emitter_item_t* item = (const emitter_item_t*)a;
  uint32_t tag = *(const uint32_t*)b;

  return item->tag - tag;
}

static int emitter_item_compare_by_ctx(const void* a, const void* b) {
  const emitter_item_t* item = (const emitter_item_t*)a;

  return item->ctx == b ? 0 : 1;
}

static int emitter_item_compare_by_id(const void* a, const void* b) {
  const emitter_item_t* item = (const emitter_item_t*)a;
  uint32_t id = *(const uint32_t*)b;

  return item->id - id;
}

static int emitter_item_compare_by_func(const void* a, const void* b) {
  const emitter_item_t* item = (const emitter_item_t*)a;
  const emitter_item_t* p = (const emitter_item_t*)b;

  if (item->type == p->type && item->ctx == p->ctx && item->handler == p->handler) {
    return 0;
  }

  return -1;
}

ret_t emitter_off(emitter_t* emitter, uint32_t id) {
  return emitter_off_ex(emitter, emitter_item_compare_by_id, &id);
}

ret_t emitter_off_by_func(emitter_t* emitter, uint32_t etype, event_func_t handler, void* ctx) {
  ret_t ret;
  emitter_item_t item;

  memset(&item, 0x00, sizeof(item));

  item.ctx = ctx;
  item.type = etype;
  item.handler = handler;

  ret = emitter_off_ex(emitter, emitter_item_compare_by_func, &item);
  if (ret == RET_OK) {
    while (TRUE) {
      if (emitter_off_ex(emitter, emitter_item_compare_by_func, &item) != RET_OK) {
        break;
      }
    }
  }

  return ret;
}

ret_t emitter_off_by_tag(emitter_t* emitter, uint32_t tag) {
  ret_t ret = emitter_off_ex(emitter, emitter_item_compare_by_tag, &tag);

  if (ret == RET_OK) {
    while (TRUE) {
      if (emitter_off_ex(emitter, emitter_item_compare_by_tag, &tag) != RET_OK) {
        break;
      }
    }
  }

  return ret;
}

ret_t emitter_off_by_ctx(emitter_t* emitter, void* ctx) {
  ret_t ret = emitter_off_ex(emitter, emitter_item_compare_by_ctx, ctx);

  if (ret == RET_OK) {
    while (TRUE) {
      if (emitter_off_ex(emitter, emitter_item_compare_by_ctx, ctx) != RET_OK) {
        break;
      }
    }
  }

  return ret;
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

emitter_t* emitter_cast(emitter_t* emitter) {
  return_value_if_fail(emitter != NULL, NULL);

  return emitter;
}

emitter_item_t* emitter_get_item(emitter_t* emitter, uint32_t index) {
  uint32_t i = 0;
  emitter_item_t* iter = NULL;
  return_value_if_fail(emitter != NULL && index < emitter_size(emitter), NULL);

  iter = emitter->items;
  for (i = 0; i < index && iter != NULL; i++) {
    iter = iter->next;
  }

  return iter;
}

ret_t emitter_dispatch_simple_event(emitter_t* emitter, uint32_t type) {
  event_t e = event_init(type, emitter);
  return_value_if_fail(emitter != NULL, RET_BAD_PARAMS);

  return emitter_dispatch(emitter, &e);
}

ret_t emitter_forward(void* ctx, event_t* e) {
  return emitter_dispatch(EMITTER(ctx), e);
}
