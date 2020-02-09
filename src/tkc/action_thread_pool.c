/**
 * File:   action_thread_pool.c
 * Author: AWTK Develop Team
 * Brief:  action_thread_pool
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-02-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/action_thread_pool.h"

action_thread_pool_t* action_thread_pool_create(uint16_t capacity, uint16_t max_free_nr) {
  action_thread_pool_t* thread_pool = NULL;
  uint32_t size = sizeof(action_thread_pool_t) + sizeof(action_thread_t*) * capacity;

  return_value_if_fail(capacity > 0 && max_free_nr > 0, NULL);
  thread_pool = (action_thread_pool_t*)TKMEM_ALLOC(size);
  return_value_if_fail(thread_pool != NULL, NULL);

  memset(thread_pool, 0x00, size);
  thread_pool->capacity = capacity;
  thread_pool->max_free_nr = max_free_nr;
  thread_pool->mutex = tk_mutex_create();

  if (thread_pool->mutex == NULL) {
    TKMEM_FREE(thread_pool);
    thread_pool = NULL;
  }

  return thread_pool;
}

ret_t action_thread_pool_exec(action_thread_pool_t* thread_pool, qaction_t* action) {
  action_thread_t* thread = NULL;
  return_value_if_fail(thread_pool != NULL && action != NULL, RET_BAD_PARAMS);
  thread = action_thread_pool_get(thread_pool);
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  action_thread_set_max_actions_nr(thread, 1);

  return action_thread_exec(thread, action);
}

ret_t action_thread_pool_put(action_thread_pool_t* thread_pool, action_thread_t* thread) {
  uint32_t i = 0;
  uint32_t free_nr = action_thread_pool_get_free_nr(thread_pool);
  return_value_if_fail(thread_pool != NULL && thread != NULL, RET_BAD_PARAMS);
  return_value_if_fail(thread->thread_pool == thread_pool, RET_BAD_PARAMS);
  log_debug("put: %p\n", thread);
  return_value_if_fail(tk_mutex_lock(thread_pool->mutex) == RET_OK, RET_BAD_PARAMS);

  if (free_nr < thread_pool->max_free_nr) {
    thread->running = FALSE;
    thread->max_actions_nr = 0;
    thread->executed_actions_nr = 0;
  } else {
    for (i = 0; i < thread_pool->capacity; i++) {
      if (thread == thread_pool->threads[i]) {
        thread_pool->threads[i] = NULL;
        break;
      }
    }
    action_thread_destroy(thread);
  }

  return tk_mutex_unlock(thread_pool->mutex);
}

uint32_t action_thread_pool_get_free_nr(action_thread_pool_t* thread_pool) {
  uint32_t i = 0;
  uint32_t n = 0;
  action_thread_t* thread = NULL;
  return_value_if_fail(thread_pool != NULL, 0);
  return_value_if_fail(tk_mutex_lock(thread_pool->mutex) == RET_OK, 0);

  for (i = 0; i < thread_pool->capacity; i++) {
    thread = thread_pool->threads[i];
    if (thread != NULL && !(thread->running)) {
      n++;
    }
  }
  tk_mutex_unlock(thread_pool->mutex);

  return n;
}

action_thread_t* action_thread_pool_get(action_thread_pool_t* thread_pool) {
  uint32_t i = 0;
  action_thread_t* thread = NULL;
  return_value_if_fail(thread_pool != NULL, NULL);
  return_value_if_fail(tk_mutex_lock(thread_pool->mutex) == RET_OK, NULL);

  for (i = 0; i < thread_pool->capacity; i++) {
    thread = thread_pool->threads[i];
    if (thread != NULL && !(thread->running)) {
      thread->running = TRUE;
      break;
    }
  }

  if (thread == NULL) {
    for (i = 0; i < thread_pool->capacity; i++) {
      thread = thread_pool->threads[i];
      if (thread == NULL) {
        thread = action_thread_create(thread_pool);
        thread->running = TRUE;
        thread_pool->threads[i] = thread;
        break;
      }
    }
  }
  tk_mutex_unlock(thread_pool->mutex);

  log_debug("get: %p\n", thread);

  return thread;
}

ret_t action_thread_pool_destroy(action_thread_pool_t* thread_pool) {
  uint32_t i = 0;
  action_thread_t* thread = NULL;
  return_value_if_fail(thread_pool != NULL, RET_BAD_PARAMS);

  for (i = 0; i < thread_pool->capacity; i++) {
    thread = thread_pool->threads[i];
    if (thread != NULL) {
      action_thread_destroy(thread);
      thread_pool->threads[i] = NULL;
    }
  }

  tk_mutex_destroy(thread_pool->mutex);
  memset(thread_pool, 0x00, sizeof(action_thread_pool_t));
  TKMEM_FREE(thread_pool);

  return RET_OK;
}
