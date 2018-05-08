/**
 * File:   idle.c
 * Author: Li XianJing <xianjimli@hotmail.com>
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

static uint32_t s_idle_id = 1;
static array_t* s_idle_manager = NULL;

static ret_t ensure_idle_manager() {
  if (s_idle_manager == NULL) {
    s_idle_manager = array_create(5);
  }

  return_value_if_fail(s_idle_manager != NULL, RET_FAIL);

  return RET_OK;
}

uint32_t idle_add(idle_func_t on_idle, void* ctx) {
  idle_info_t* idle = NULL;
  return_value_if_fail(on_idle != NULL, 0);
  return_value_if_fail(ensure_idle_manager() == RET_OK, 0);

  idle = TKMEM_ZALLOC(idle_info_t);
  return_value_if_fail(idle != NULL, 0);

  idle->ctx = ctx;
  idle->id = s_idle_id++;
  idle->on_idle = on_idle;

  return array_push(s_idle_manager, idle) == RET_OK ? idle->id : 0;
}

static int compare_idle(const void* a, const void* b) {
  idle_info_t* t1 = (idle_info_t*)a;
  idle_info_t* t2 = (idle_info_t*)b;

  if (t1->id == t2->id) {
    return 0;
  }

  return -1;
}

ret_t idle_remove(uint32_t idle_id) {
  idle_info_t idle;
  idle_info_t* ret = NULL;
  return_value_if_fail(idle_id > 0, RET_BAD_PARAMS);
  return_value_if_fail(ensure_idle_manager() == RET_OK, RET_BAD_PARAMS);

  idle.id = idle_id;
  ret = (idle_info_t*)array_find(s_idle_manager, compare_idle, &idle);
  return_value_if_fail(ret != NULL, RET_NOT_FOUND);

  if (array_remove(s_idle_manager, compare_idle, &idle) == RET_OK) {
    memset(ret, 0x00, sizeof(idle_info_t));
    /*remove it when dispatch*/
  }

  return RET_OK;
}

const idle_info_t* idle_find(uint32_t idle_id) {
  idle_info_t idle;
  return_value_if_fail(idle_id > 0, NULL);
  return_value_if_fail(ensure_idle_manager() == RET_OK, NULL);

  idle.id = idle_id;

  return (const idle_info_t*)array_find(s_idle_manager, compare_idle, &idle);
}

ret_t idle_dispatch(void) {
  uint32_t i = 0;
  uint32_t nr = 0;
  idle_info_t** idles = NULL;
  return_value_if_fail(ensure_idle_manager() == RET_OK, RET_BAD_PARAMS);

  if (s_idle_manager->size == 0) {
    return RET_OK;
  }

  idles = (idle_info_t**)s_idle_manager->elms;
  for (i = 0, nr = s_idle_manager->size; i < nr; i++) {
    idle_info_t* iter = idles[i];
    if (iter->on_idle) {
      iter->on_idle(iter);
    } else {
      /*it is removed*/
    }
    TKMEM_FREE(iter);
  }

  s_idle_manager->size = 0;

  return RET_OK;
}

uint32_t idle_count() {
  uint32_t i = 0;
  uint32_t nr = 0;
  uint32_t count = 0;

  if (s_idle_manager && s_idle_manager->size > 0) {
    idle_info_t** idles = (idle_info_t**)s_idle_manager->elms;
    for (i = 0, nr = s_idle_manager->size; i < nr; i++) {
      idle_info_t* iter = idles[i];
      if (iter->on_idle != NULL) {
        count++;
      }
    }
  }

  return count;
}
