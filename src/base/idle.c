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
#define ACTIVE_IDLES(idle_manager) (((idle_manager)->idles) + (idle_manager)->active)
#define OFFLINE_IDLES(idle_manager) (((idle_manager)->idles) + ((idle_manager)->active ? 0 : 1))

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

  idle_manager->next_idle_id = 1;
  array_init(idle_manager->idles + 0, 0);
  array_init(idle_manager->idles + 1, 0);

  return idle_manager;
}

ret_t idle_manager_remove_all(idle_manager_t* idle_manager) {
  uint32_t i = 0;
  uint32_t nr = 0;
  idle_info_t** idles = NULL;
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  nr = ACTIVE_IDLES(idle_manager)->size;
  idles = (idle_info_t**)ACTIVE_IDLES(idle_manager)->elms;

  for (i = 0; i < nr; i++) {
    idle_info_t* iter = idles[i];
    idle_info_destroy(iter);
  }
  ACTIVE_IDLES(idle_manager)->size = 0;

  return RET_OK;
}

ret_t idle_manager_deinit(idle_manager_t* idle_manager) {
  if (idle_manager_remove_all(idle_manager) == RET_OK) {
    array_deinit(idle_manager->idles + 0);
    array_deinit(idle_manager->idles + 1);
  }

  return RET_OK;
}

ret_t idle_manager_destroy(idle_manager_t* idle_manager) {
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  idle_manager_deinit(idle_manager);
  TKMEM_FREE(idle_manager);

  return RET_OK;
}

uint32_t idle_manager_add(idle_manager_t* idle_manager, idle_func_t on_idle, void* ctx) {
  idle_info_t* idle = NULL;
  return_value_if_fail(on_idle != NULL, 0);
  return_value_if_fail(idle_manager != NULL, 0);

  idle = TKMEM_ZALLOC(idle_info_t);
  return_value_if_fail(idle != NULL, 0);

  idle->ctx = ctx;
  idle->id = idle_manager->next_idle_id++;
  idle->on_idle = on_idle;

  return array_push(ACTIVE_IDLES(idle_manager), idle) == RET_OK ? idle->id : 0;
}

static int idle_info_compare_id(const void* a, const void* b) {
  idle_info_t* t1 = (idle_info_t*)a;
  idle_info_t* t2 = (idle_info_t*)b;

  if (t1->id == t2->id) {
    return 0;
  }

  return -1;
}

ret_t idle_manager_remove(idle_manager_t* idle_manager, uint32_t idle_id) {
  idle_info_t idle;
  ret_t ret = RET_OK;
  return_value_if_fail(idle_id > 0, RET_BAD_PARAMS);
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  idle.id = idle_id;
  ret = array_remove(ACTIVE_IDLES(idle_manager), idle_info_compare_id, &idle,
                     (tk_destroy_t)idle_info_destroy);
  if (ret != RET_OK) {
    ret = array_remove(OFFLINE_IDLES(idle_manager), idle_info_compare_id, &idle,
                       (tk_destroy_t)idle_info_destroy);
  }

  return ret;
}

const idle_info_t* idle_manager_find(idle_manager_t* idle_manager, uint32_t idle_id) {
  idle_info_t idle;
  const idle_info_t* ret = NULL;
  return_value_if_fail(idle_id > 0, NULL);
  return_value_if_fail(idle_manager != NULL, NULL);

  idle.id = idle_id;

  ret = (const idle_info_t*)array_find(ACTIVE_IDLES(idle_manager), idle_info_compare_id, &idle);
  if (ret == NULL) {
    ret = (const idle_info_t*)array_find(OFFLINE_IDLES(idle_manager), idle_info_compare_id, &idle);
  }

  return ret;
}

ret_t idle_manager_dispatch(idle_manager_t* idle_manager) {
  array_t* active = NULL;
  array_t* offline = NULL;
  return_value_if_fail(idle_manager != NULL, RET_BAD_PARAMS);

  active = ACTIVE_IDLES(idle_manager);
  offline = OFFLINE_IDLES(idle_manager);

  while (active->size > 0) {
    idle_info_t* iter = (idle_info_t*)array_pop(active);
    if (iter->on_idle(iter) != RET_REPEAT) {
      idle_info_destroy(iter);
    } else {
      array_push(offline, iter);
    }
  }

  idle_manager->active = idle_manager->active ? 0 : 1;

  return RET_OK;
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
  return ACTIVE_IDLES(idle_manager())->size + OFFLINE_IDLES(idle_manager())->size;
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
