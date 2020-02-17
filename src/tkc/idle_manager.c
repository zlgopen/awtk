/**
 * File:   idle.c
 * Author: AWTK Develop Team
 * Brief:  idle manager
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/time_now.h"
#include "tkc/idle_manager.h"

static idle_manager_t* s_idle_manager;

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
  slist_init(&(idle_manager->idles), (tk_destroy_t)object_unref, idle_info_compare);

  return idle_manager;
}

ret_t idle_manager_deinit(idle_manager_t* idle_manager) {
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  return slist_deinit(&(idle_manager->idles));
}

ret_t idle_manager_remove_all(idle_manager_t* idle_manager) {
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  return slist_remove_all(&(idle_manager->idles));
}

ret_t idle_manager_destroy(idle_manager_t* idle_manager) {
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  idle_manager_deinit(idle_manager);
  TKMEM_FREE(idle_manager);

  return RET_OK;
}

ret_t idle_manager_append(idle_manager_t* idle_manager, idle_info_t* idle) {
  return_value_if_fail(idle_manager != NULL && idle != NULL, RET_BAD_PARAMS);

  return slist_append(&(idle_manager->idles), idle);
}

uint32_t idle_manager_add(idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx) {
  idle_info_t* idle = NULL;
  return_value_if_fail(on_idle != NULL, TK_INVALID_ID);
  return_value_if_fail(idle_manager != NULL, TK_INVALID_ID);

  idle = idle_info_create(idle_manager, on_idle, ctx);
  return_value_if_fail(idle != NULL, TK_INVALID_ID);

  return idle->id;
}

ret_t idle_manager_remove(idle_manager_t* idle_manager, uint32_t idle_id) {
  idle_info_t idle;
  return_value_if_fail(idle_id != TK_INVALID_ID, RET_BAD_PARAMS);
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  return slist_remove(&(idle_manager->idles), idle_info_init_dummy(&idle, idle_id));
}

const idle_info_t* idle_manager_find(idle_manager_t* idle_manager, uint32_t idle_id) {
  idle_info_t idle;
  return_value_if_fail(idle_id != TK_INVALID_ID, NULL);
  return_value_if_fail(idle_manager != NULL, NULL);

  return slist_find(&(idle_manager->idles), idle_info_init_dummy(&idle, idle_id));
}

static ret_t idle_manager_dispatch_one(idle_manager_t* idle_manager, uint32_t dispatch_id) {
  slist_node_t* iter = idle_manager->idles.first;

  while (iter != NULL && !idle_info_is_available(IDLE_INFO(iter->data), dispatch_id)) {
    iter = iter->next;
  }

  if (iter != NULL) {
    idle_info_t* idle = (idle_info_t*)object_ref((object_t*)(iter->data));
    return_value_if_fail(idle != NULL, RET_BAD_PARAMS);

    if (idle_info_on_idle(idle, dispatch_id) != RET_REPEAT) {
      idle_manager_remove(idle_manager, idle->id);
    }

    object_unref((object_t*)idle);

    return RET_OK;
  }

  return RET_DONE;
}

ret_t idle_manager_dispatch(idle_manager_t* idle_manager) {
  uint32_t dispatch_times = 0;
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  idle_manager->dispatch_times++;
  if (idle_manager->idles.first == NULL) {
    return RET_OK;
  }

  dispatch_times = idle_manager->dispatch_times;
  while (idle_manager_dispatch_one(idle_manager, dispatch_times) == RET_OK) {
    if (idle_manager->dispatch_times != dispatch_times) {
      log_debug("abort dispatch because sub main loop\n");
    }
  }

  return RET_OK;
}

uint32_t idle_manager_count(idle_manager_t* idle_manager) {
  return_value_if_fail(idle_manager != NULL, 0);

  return slist_size(&(idle_manager->idles));
}

bool_t idle_manager_exist(idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx) {
  slist_node_t* iter = idle_manager->idles.first;

  while (iter != NULL) {
    idle_info_t* idle = (idle_info_t*)(iter->data);

    if (idle->on_idle == on_idle && idle->ctx == ctx) {
      return TRUE;
    }

    iter = iter->next;
  }

  return FALSE;
}
