/**
 * File:   waitable_action_queue.c
 * Author: AWTK Develop Team
 * Brief:  waitable_action_queue
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
 * 2020-02-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/waitable_action_queue.h"

waitable_action_queue_t* waitable_action_queue_create(uint16_t capacity) {
  waitable_action_queue_t* q = TKMEM_ZALLOC(waitable_action_queue_t);
  return_value_if_fail(q != NULL, NULL);
  q->queue = action_queue_create(capacity);
  goto_error_if_fail(q->queue != NULL);

  q->mutex = tk_mutex_create();
  goto_error_if_fail(q->mutex != NULL);

  q->sema_recv = tk_semaphore_create(0, NULL);
  goto_error_if_fail(q->sema_recv != NULL);

  q->sema_send = tk_semaphore_create(capacity, NULL);
  goto_error_if_fail(q->sema_send != NULL);

  return q;
error:
  if (q != NULL) {
    if (q->queue != NULL) {
      action_queue_destroy(q->queue);
    }

    if (q->mutex != NULL) {
      tk_mutex_destroy(q->mutex);
    }

    if (q->sema_recv != NULL) {
      tk_semaphore_destroy(q->sema_recv);
    }

    if (q->sema_send != NULL) {
      tk_semaphore_destroy(q->sema_send);
    }

    TKMEM_FREE(q);
  }

  return NULL;
}

ret_t waitable_action_queue_recv(waitable_action_queue_t* q, qaction_t** action,
                                 uint32_t timeout_ms) {
  ret_t ret = RET_FAIL;
  return_value_if_fail(q != NULL && action != NULL, RET_BAD_PARAMS);

  if (tk_semaphore_wait(q->sema_recv, timeout_ms) == RET_OK) {
    if (tk_mutex_lock(q->mutex) == RET_OK) {
      ret = action_queue_recv(q->queue, action);
      if (ret == RET_OK) {
        ENSURE(tk_semaphore_post(q->sema_send) == RET_OK);
      }
      ENSURE(tk_mutex_unlock(q->mutex) == RET_OK);
    }
  }

  return ret;
}

ret_t waitable_action_queue_send(waitable_action_queue_t* q, qaction_t* action,
                                 uint32_t timeout_ms) {
  ret_t ret = RET_FAIL;
  return_value_if_fail(q != NULL && action != NULL, RET_BAD_PARAMS);

  if (tk_semaphore_wait(q->sema_send, timeout_ms) == RET_OK) {
    if (tk_mutex_lock(q->mutex) == RET_OK) {
      ret = action_queue_send(q->queue, action);
      assert(ret == RET_OK);
      ENSURE(tk_semaphore_post(q->sema_recv) == RET_OK);
      ENSURE(tk_mutex_unlock(q->mutex) == RET_OK);
    }
  }

  return ret;
}

ret_t waitable_action_queue_destroy(waitable_action_queue_t* q) {
  return_value_if_fail(q != NULL, RET_BAD_PARAMS);

  ENSURE(action_queue_destroy(q->queue) == RET_OK);
  ENSURE(tk_mutex_destroy(q->mutex) == RET_OK);
  ENSURE(tk_semaphore_destroy(q->sema_recv) == RET_OK);
  ENSURE(tk_semaphore_destroy(q->sema_send) == RET_OK);
  TKMEM_FREE(q);

  return RET_OK;
}
