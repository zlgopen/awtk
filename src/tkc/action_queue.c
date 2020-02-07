/**
 * File:   action_queue.c
 * Author: AWTK Develop Team
 * Brief:  action_queue
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
 * 2020-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/action_queue.h"

ret_t qaction_exec(qaction_t* action) {
  return_value_if_fail(action != NULL && action->vt != NULL, RET_BAD_PARAMS);

  if (action->vt->exec != NULL) {
    return action->vt->exec(action);
  }

  return RET_NOT_IMPL;
}

ret_t qaction_destroy(qaction_t* action) {
  return_value_if_fail(action != NULL && action->vt != NULL, RET_BAD_PARAMS);

  if (action->vt->destroy != NULL) {
    return action->vt->destroy(action);
  }

  return RET_NOT_IMPL;
}

action_queue_t* action_queue_create(uint16_t capacity) {
  uint16_t size = 0;
  action_queue_t* q = NULL;
  return_value_if_fail(capacity > 1, NULL);

  size = sizeof(action_queue_t) + (capacity - 1) * sizeof(qaction_t);
  q = (action_queue_t*)TKMEM_ALLOC(size);
  return_value_if_fail(q != NULL, NULL);

  memset(q, 0x00, size);
  q->capacity = capacity;

  return q;
}

ret_t action_queue_recv(action_queue_t* q, qaction_t* action) {
  return_value_if_fail(q != NULL && action != NULL, RET_BAD_PARAMS);
  if (q->r != q->w || q->full) {
    memcpy(action, q->actions + q->r, sizeof(*action));
    if ((q->r + 1) < q->capacity) {
      q->r++;
    } else {
      q->r = 0;
    }
    q->full = FALSE;

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t action_queue_send(action_queue_t* q, const qaction_t* action) {
  return_value_if_fail(q != NULL && action != NULL, RET_BAD_PARAMS);
  if (q->r != q->w || !q->full) {
    memcpy(q->actions + q->w, action, sizeof(*action));
    if ((q->w + 1) < q->capacity) {
      q->w++;
    } else {
      q->w = 0;
    }
    if (q->r == q->w) {
      q->full = TRUE;
    }

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t action_queue_destroy(action_queue_t* q) {
  return_value_if_fail(q != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(q);

  return RET_OK;
}
