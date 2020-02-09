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

qaction_t* qaction_init(qaction_t* action, qaction_exec_t exec, void* args, uint32_t args_size) {
  return_value_if_fail(action != NULL & exec != NULL, NULL);
  return_value_if_fail(args_size <= sizeof(action->args), NULL);

  memset(action, 0x00, sizeof(qaction_t));

  action->exec = exec;

  if (args != NULL) {
    memcpy(action->args, args, args_size);
  }

  return action;
}

ret_t qaction_set_on_event(qaction_t* action, qaction_on_event_t on_event, void* on_event_ctx) {
  return_value_if_fail(action != NULL, RET_BAD_PARAMS);

  action->on_event = on_event;
  action->on_event_ctx = on_event_ctx;

  return RET_OK;
}

ret_t qaction_notify(qaction_t* action, event_t* event) {
  return_value_if_fail(action != NULL && event != NULL, RET_BAD_PARAMS);

  if (action->on_event != NULL) {
    action->on_event(action->on_event_ctx, event);
  }

  return RET_OK;
}

ret_t qaction_exec(qaction_t* action) {
  return_value_if_fail(action != NULL && action->exec != NULL, RET_BAD_PARAMS);

  return action->exec(action);
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
