/**
 * File:   qaction.c
 * Author: AWTK Develop Team
 * Brief:  qaction
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
#include "tkc/qaction.h"

qaction_t* qaction_create(qaction_exec_t exec, void* args, uint32_t args_size) {
  qaction_t* action = NULL;
  uint32_t size = sizeof(qaction_t) + args_size;
  return_value_if_fail(exec != NULL, NULL);

  action = TKMEM_ALLOC(size);
  return_value_if_fail(action != NULL, NULL);

  memset(action, 0x00, size);
  action->exec = exec;
  if (args != NULL) {
    memcpy(action->args, args, args_size);
  }

  return action;
}

ret_t qaction_set_on_event(qaction_t* action, qaction_on_event_t on_event) {
  return_value_if_fail(action != NULL, RET_BAD_PARAMS);

  action->on_event = on_event;

  return RET_OK;
}

ret_t qaction_notify(qaction_t* action, event_t* event) {
  return_value_if_fail(action != NULL && event != NULL, RET_BAD_PARAMS);

  if (action->on_event != NULL) {
    return action->on_event(action, event);
  }

  return RET_NOT_IMPL;
}

ret_t qaction_exec(qaction_t* action) {
  return_value_if_fail(action != NULL && action->exec != NULL, RET_BAD_PARAMS);

  return action->exec(action);
}

ret_t qaction_destroy(qaction_t* q) {
  return_value_if_fail(q != NULL, RET_BAD_PARAMS);
  memset(q, 0x00, sizeof(qaction_t));

  TKMEM_FREE(q);

  return RET_OK;
}
