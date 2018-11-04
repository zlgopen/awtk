/**
 * File:   idle.c
 * Author: AWTK Develop Team
 * Brief:  idle manager
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
 * 2018-04-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/idle.h"
#include "base/array.h"

static idle_manager_t* s_idle_manager;

static ret_t idle_info_destroy(idle_info_t* info) {
  memset(info, 0x00, sizeof(idle_info_t));
  TKMEM_FREE(info);

  return RET_OK;
}

idle_manager_t* idle_manager(void) {
  return s_idle_manager;
}

ret_t idle_manager_set(idle_manager_t* idle_manager) {
  s_idle_manager = idle_manager;

  return RET_OK;
}

idle_manager_t* idle_manager_create(void) {
  idle_manager_t* idle_manager = TKMEM_ZALLOC(idle_manager_t);

  return idle_manager_init(idle_manager);
}

idle_manager_t* idle_manager_init(idle_manager_t* idle_manager) {
  return_value_if_fail(idle_manager != NULL, NULL);

  idle_manager->next_idle_id = TK_INVALID_ID + 1;

  return idle_manager;
}

ret_t idle_manager_remove_all(idle_manager_t* idle_manager) {
  idle_info_t* iter = NULL;
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  iter = idle_manager->first;
  while (iter != NULL) {
    idle_info_t* next = iter->next;

    idle_info_destroy(iter);
    iter = next;
  }
  idle_manager->first = NULL;

  return RET_OK;
}

ret_t idle_manager_deinit(idle_manager_t* idle_manager) {
  return idle_manager_remove_all(idle_manager);
}

ret_t idle_manager_destroy(idle_manager_t* idle_manager) {
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  idle_manager_deinit(idle_manager);
  TKMEM_FREE(idle_manager);

  return RET_OK;
}

uint32_t idle_manager_add(idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx) {
  idle_info_t* iter = NULL;
  idle_info_t* idle = NULL;
  return_value_if_fail(on_idle != NULL, TK_INVALID_ID);
  return_value_if_fail(idle_manager != NULL, TK_INVALID_ID);

  idle = TKMEM_ZALLOC(idle_info_t);
  return_value_if_fail(idle != NULL, TK_INVALID_ID);

  idle->ctx = ctx;
  idle->id = idle_manager->next_idle_id++;
  idle->on_idle = on_idle;

  iter = idle_manager->first;
  if (iter == NULL) {
    idle_manager->first = idle;
  } else {
    while (iter->next != NULL) {
      iter = iter->next;
    }
    iter->next = idle;
  }

  return idle->id;
}

ret_t idle_manager_remove(idle_manager_t* idle_manager, uint32_t idle_id) {
  ret_t ret = RET_NOT_FOUND;
  idle_info_t* iter = NULL;
  idle_info_t* prev = NULL;
  return_value_if_fail(idle_id != TK_INVALID_ID, RET_BAD_PARAMS);
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  prev = idle_manager->first;
  iter = idle_manager->first;

  if (iter->id == idle_id) {
    if (idle_manager->dispatching) {
      iter->pending_destroy = TRUE;
    } else {
      idle_manager->first = iter->next;
      idle_info_destroy(iter);
    }
    ret = RET_OK;
  } else {
    prev = iter;
    iter = iter->next;
    while (iter != NULL) {
      if (iter->id == idle_id) {
        if (idle_manager->dispatching) {
          iter->pending_destroy = TRUE;
        } else {
          prev->next = iter->next;
          idle_info_destroy(iter);
        }
        ret = RET_OK;
        break;
      }
      prev = iter;
      iter = iter->next;
    }
  }

  return ret;
}

static ret_t idle_manager_remove_pending_destroy_idles(idle_manager_t* idle_manager) {
  ret_t ret = RET_NOT_FOUND;
  idle_info_t* iter = NULL;
  idle_info_t* prev = NULL;
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  prev = idle_manager->first;
  iter = idle_manager->first;
  while (iter != NULL) {
    if (iter->pending_destroy) {
      if (iter == idle_manager->first) {
        idle_manager->first = iter->next;
        idle_info_destroy(iter);

        prev = idle_manager->first;
        iter = idle_manager->first;
      } else {
        prev->next = iter->next;
        idle_info_destroy(iter);
        iter = prev->next;
      }
    } else {
      prev = iter;
      iter = prev->next;
    }
  }

  return ret;
}

const idle_info_t* idle_manager_find(idle_manager_t* idle_manager, uint32_t idle_id) {
  idle_info_t* iter = NULL;
  return_value_if_fail(idle_id != TK_INVALID_ID, NULL);
  return_value_if_fail(idle_manager != NULL, NULL);

  iter = idle_manager->first;
  while (iter != NULL) {
    if (iter->id == idle_id) {
      if (iter->pending_destroy) {
        return NULL;
      }
      return iter;
    }

    iter = iter->next;
  }

  return NULL;
}

ret_t idle_manager_dispatch(idle_manager_t* idle_manager) {
  idle_info_t* iter = NULL;
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  iter = idle_manager->first;
  if (iter == NULL) {
    return RET_OK;
  }

  idle_manager->dispatching = TRUE;
  while (iter != NULL) {
    if (iter->on_idle(iter) != RET_REPEAT) {
      iter->pending_destroy = TRUE;
    }
    iter = iter->next;
  }
  idle_manager->dispatching = FALSE;

  idle_manager_remove_pending_destroy_idles(idle_manager);

  return RET_OK;
}

uint32_t idle_manager_count(idle_manager_t* idle_manager) {
  uint32_t count = 0;
  idle_info_t* iter = NULL;
  return_value_if_fail(idle_manager != NULL, 0);

  iter = idle_manager->first;
  while (iter != NULL) {
    if (!iter->pending_destroy) {
      count++;
    }
    iter = iter->next;
  }

  return count;
}

uint32_t idle_add(idle_func_t on_idle, void* ctx) {
  return idle_manager_add(idle_manager(), on_idle, ctx);
}

ret_t idle_remove(uint32_t idle_id) {
  return idle_manager_remove(idle_manager(), idle_id);
}

const idle_info_t* idle_find(uint32_t idle_id) {
  return idle_manager_find(idle_manager(), idle_id);
}

ret_t idle_dispatch(void) {
  return idle_manager_dispatch(idle_manager());
}

uint32_t idle_count(void) {
  return idle_manager_count(idle_manager());
}

#include "base/main_loop.h"

ret_t idle_queue(idle_func_t on_idle, void* ctx) {
  event_queue_req_t r;
  r.add_idle.func = on_idle;
  r.add_idle.e.target = ctx;
  r.add_idle.e.type = REQ_ADD_IDLE;

  return main_loop_queue_event(main_loop(), &r);
}

ret_t idle_set_on_destroy(uint32_t idle_id, tk_destroy_t on_destroy, void* on_destroy_ctx) {
  idle_info_t* item = (idle_info_t*)idle_find(idle_id);
  return_value_if_fail(item != NULL, RET_BAD_PARAMS);

  item->on_destroy = on_destroy;
  item->on_destroy_ctx = on_destroy_ctx;

  return RET_OK;
}
