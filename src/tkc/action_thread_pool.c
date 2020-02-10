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

action_thread_pool_t* action_thread_pool_create(uint16_t capacity, uint16_t min_idle_nr) {
  action_thread_pool_t* thread_pool = NULL;
  uint32_t size = sizeof(action_thread_pool_t) + sizeof(action_thread_t*) * capacity;

  return_value_if_fail(capacity > 0 && min_idle_nr > 0, NULL);
  thread_pool = (action_thread_pool_t*)TKMEM_ALLOC(size);
  return_value_if_fail(thread_pool != NULL, NULL);

  memset(thread_pool, 0x00, size);
  thread_pool->capacity = capacity;
  thread_pool->min_idle_nr = min_idle_nr;

  thread_pool->mutex = tk_mutex_create();
  goto_error_if_fail(thread_pool->mutex != NULL);

  thread_pool->queue = waitable_action_queue_create(capacity * 5);
  goto_error_if_fail(thread_pool->queue != NULL);

  return thread_pool;
error:
  if (thread_pool->mutex != NULL) {
    tk_mutex_destroy(thread_pool->mutex);
  }

  if(thread_pool->queue != NULL) {
    waitable_action_queue_destroy(thread_pool->queue);
  }

  TKMEM_FREE(thread_pool);

  return NULL;
}

static uint32_t action_thread_pool_get_thread_nr(action_thread_pool_t* thread_pool) {
  uint32_t i = 0;
  uint32_t n = 0;
  action_thread_t* thread = NULL;
  return_value_if_fail(thread_pool != NULL, 0);
  return_value_if_fail(tk_mutex_lock(thread_pool->mutex) == RET_OK, 0);

  for (i = 0; i < thread_pool->capacity; i++) {
    thread = thread_pool->threads[i];
    if (thread != NULL) {
      n++;
    }
  }
  tk_mutex_unlock(thread_pool->mutex);

  return n;
}

static ret_t action_thread_pool_on_thread_idle(action_thread_pool_t* thread_pool,
    action_thread_t* thread) {
  uint32_t thread_nr = action_thread_pool_get_thread_nr(thread_pool);
  return_value_if_fail(thread_pool != NULL && thread != NULL, RET_BAD_PARAMS);
  
  return (thread_nr > thread_pool->min_idle_nr) ? RET_QUIT : RET_OK;
}

static ret_t action_thread_pool_create_thread(action_thread_pool_t* thread_pool) {
  uint32_t i = 0;
  return_value_if_fail(thread_pool != NULL, RET_BAD_PARAMS);
  return_value_if_fail(tk_mutex_lock(thread_pool->mutex) == RET_OK, RET_BAD_PARAMS);
  
  for (i = 0; i < thread_pool->capacity; i++) {
    if(thread_pool->threads[i] == NULL) {
      thread_pool->threads[i] = action_thread_create_with_queue(thread_pool->queue);
      action_thread_set_on_quit(thread_pool->threads[i],
          (action_thread_on_idle_t)action_thread_pool_on_thread_idle, thread_pool);
    }
  }

  return tk_mutex_unlock(thread_pool->mutex);
}

static ret_t action_thread_pool_ensure_threads(action_thread_pool_t* thread_pool) {
  uint32_t thread_nr = action_thread_pool_get_thread_nr(thread_pool);
  return_value_if_fail(thread_pool != NULL, RET_BAD_PARAMS);

  if(thread_nr < thread_pool->min_idle_nr) {
    return action_thread_pool_create_thread(thread_pool);
  }

  return RET_OK;
}

ret_t action_thread_pool_exec(action_thread_pool_t* thread_pool, qaction_t* action) {
  return_value_if_fail(thread_pool != NULL && action != NULL, RET_BAD_PARAMS);
  return_value_if_fail(action_thread_pool_ensure_threads(thread_pool) == RET_OK, RET_FAIL);

  if(thread_pool->queue->queue->full) {
    action_thread_pool_create_thread(thread_pool);
  }

  return waitable_action_queue_send(thread_pool->queue, action, 1000);
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
  waitable_action_queue_destroy(thread_pool->queue);
  memset(thread_pool, 0x00, sizeof(action_thread_pool_t));
  TKMEM_FREE(thread_pool);

  return RET_OK;
}
