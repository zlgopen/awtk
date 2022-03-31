/**
 * File:   waitable_action_darray.c
 * Author: AWTK Develop Team
 * Brief:  waitable_action_darray
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-3-25 vih created
 *
 */

#include "tkc/mem.h"
#include "tkc/event.h"
#include "tkc/waitable_action_darray.h"

static ret_t waitable_action_darray_destroy_qaction(qaction_t* action) {
  event_t event;

  /* 如果有回调，则回调里必须带 qaction 销毁操作或返回 RET_NOT_IMPL（由系统进行销毁） */
  event.type = EVT_DESTROY;
  event.target = action;
  if (qaction_notify(action, &event) == RET_NOT_IMPL) {
    action->exec = NULL;
    qaction_destroy(action);
  }
  return RET_OK;
}

int on_darray_compare(const void* iter, const void* data) {
  qaction_t* qaction = (qaction_t*)iter;
  qaction_exec_t* exec = (qaction_exec_t*)data;
  return ((const char*)qaction->exec - (const char*)exec);
}

waitable_action_darray_t* waitable_action_darray_create(uint16_t capacity) {
  waitable_action_darray_t* q = TKMEM_ZALLOC(waitable_action_darray_t);
  return_value_if_fail(q != NULL, NULL);
  q->darray = darray_create(capacity, NULL, on_darray_compare);
  goto_error_if_fail(q->darray != NULL);

  q->mutex = tk_mutex_create();
  goto_error_if_fail(q->mutex != NULL);

  q->sema_recv = tk_semaphore_create(0, NULL);
  goto_error_if_fail(q->sema_recv != NULL);

  q->sema_send = tk_semaphore_create(capacity, NULL);
  goto_error_if_fail(q->sema_send != NULL);

  return q;
error:
  if (q != NULL) {
    if (q->darray != NULL) {
      darray_destroy(q->darray);
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

ret_t waitable_action_darray_recv(waitable_action_darray_t* q, qaction_t** action,
                                  uint32_t timeout_ms) {
  ret_t ret = RET_TIMEOUT;
  return_value_if_fail(q != NULL && action != NULL, RET_BAD_PARAMS);

  if (tk_semaphore_wait(q->sema_recv, timeout_ms) == RET_OK) {
    if (tk_mutex_lock(q->mutex) == RET_OK) {
      *action = darray_head(q->darray);
      ret = darray_remove_index(q->darray, 0);
      if (ret == RET_OK) {
        ENSURE(tk_semaphore_post(q->sema_send) == RET_OK);
      }
      ENSURE(tk_mutex_unlock(q->mutex) == RET_OK);
    }
  }
  return ret;
}

ret_t waitable_action_darray_send(waitable_action_darray_t* q, qaction_t* action,
                                  uint32_t timeout_ms) {
  ret_t ret = RET_TIMEOUT;
  return_value_if_fail(q != NULL && action != NULL, RET_BAD_PARAMS);

  if (tk_semaphore_wait(q->sema_send, timeout_ms) == RET_OK) {
    if (tk_mutex_lock(q->mutex) == RET_OK) {
      ret = darray_push(q->darray, action);
      if (ret == RET_OK) {
        ENSURE(tk_semaphore_post(q->sema_recv) == RET_OK);
      }
      ENSURE(tk_mutex_unlock(q->mutex) == RET_OK);
    }
  }

  return ret;
}

qaction_t* waitable_action_darray_find_ex(waitable_action_darray_t* q, tk_compare_t cmp,
                                          void* ctx) {
  qaction_t* action = NULL;
  return_value_if_fail(q != NULL && ctx != NULL, NULL);

  if (tk_mutex_lock(q->mutex) == RET_OK) {
    action = (qaction_t*)darray_find_ex(q->darray, cmp, ctx);
    ENSURE(tk_mutex_unlock(q->mutex) == RET_OK);
  }
  return action;
}

qaction_t* waitable_action_darray_find(waitable_action_darray_t* q, qaction_exec_t exec) {
  return waitable_action_darray_find_ex(q, NULL, (void*)exec);
}

ret_t waitable_action_darray_remove_ex(waitable_action_darray_t* q, tk_compare_t cmp, void* ctx) {
  ret_t ret = RET_FAIL;
  qaction_t* action;
  return_value_if_fail(q != NULL && ctx != NULL, RET_BAD_PARAMS);

  if (tk_semaphore_wait(q->sema_recv, 0) == RET_OK) {
    if (tk_mutex_lock(q->mutex) == RET_OK) {
      action = darray_find_ex(q->darray, cmp, ctx);
      ret = darray_remove_ex(q->darray, cmp, ctx);
      if (ret == RET_OK) {
        ENSURE(tk_semaphore_post(q->sema_send) == RET_OK);
        waitable_action_darray_destroy_qaction(action);
      } else {
        ENSURE(tk_semaphore_post(q->sema_recv) == RET_OK);
      }
      ENSURE(tk_mutex_unlock(q->mutex) == RET_OK);
    }
  }
  return ret;
}

ret_t waitable_action_darray_remove(waitable_action_darray_t* q, qaction_exec_t exec) {
  return waitable_action_darray_remove_ex(q, NULL, (void*)exec);
}

ret_t waitable_action_darray_replace(waitable_action_darray_t* q, qaction_t* new_action) {
  return waitable_action_darray_replace_ex(q, new_action->exec, new_action);
}

ret_t waitable_action_darray_replace_ex(waitable_action_darray_t* q, qaction_exec_t exec,
                                        qaction_t* new_action) {
  ret_t ret = RET_FAIL;
  int32_t index;
  return_value_if_fail(q != NULL && exec != NULL && new_action != NULL, RET_BAD_PARAMS);

  if (tk_mutex_lock(q->mutex) == RET_OK) {
    index = darray_find_index(q->darray, exec);
    if (index >= 0) {
      qaction_t* old;
      old = darray_get(q->darray, index);
      ret = darray_replace(q->darray, index, new_action);
      if (ret == RET_OK) {
        waitable_action_darray_destroy_qaction(old);
      }
    }
    ENSURE(tk_mutex_unlock(q->mutex) == RET_OK);
  }
  return ret;
}

ret_t waitable_action_darray_destroy(waitable_action_darray_t* q) {
  return_value_if_fail(q != NULL, RET_BAD_PARAMS);

  if (tk_mutex_lock(q->mutex) == RET_OK) {
    while (q->darray->size) {
      waitable_action_darray_destroy_qaction((qaction_t*)darray_pop(q->darray));
    }
    ENSURE(tk_mutex_unlock(q->mutex) == RET_OK);
  }
  ENSURE(darray_destroy(q->darray) == RET_OK);
  ENSURE(tk_semaphore_destroy(q->sema_recv) == RET_OK);
  ENSURE(tk_semaphore_destroy(q->sema_send) == RET_OK);
  ENSURE(tk_mutex_destroy(q->mutex) == RET_OK);
  TKMEM_FREE(q);

  return RET_OK;
}
