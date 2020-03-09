/**
 * File:   async.c
 * Author: AWTK Develop Team
 * Brief:  async call
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ===============================================================
 * 2019-05-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/event.h"

#include "tkc/async.h"
#include "tkc/action_thread_pool.h"

static action_thread_pool_t* s_async_thread_pool = NULL;

typedef struct _async_call_info_t {
  void* ctx;
  async_exec_t exec;
  async_on_result_t on_result;
} async_call_info_t;

static ret_t async_call_info_init(async_call_info_t* info, async_exec_t exec,
                                  async_on_result_t on_result, void* ctx) {
  return_value_if_fail(info != NULL && exec != NULL, RET_BAD_PARAMS);

  info->ctx = ctx;
  info->exec = exec;
  info->on_result = on_result;

  return RET_OK;
}

static ret_t qaction_async_exec(qaction_t* action) {
  async_call_info_t* info = (async_call_info_t*)action->args;

  return info->exec(info->ctx);
}

static ret_t qaction_async_on_event(qaction_t* action, event_t* event) {
  async_call_info_t* info = (async_call_info_t*)action->args;

  if (event->type == EVT_DONE) {
    done_event_t* e = (done_event_t*)event;
    info->on_result(info->ctx, e->result);
    qaction_destroy(action);
  }

  return RET_OK;
}

ret_t async_call(async_exec_t exec, async_on_result_t on_result, void* ctx) {
  qaction_t* a = NULL;
  async_call_info_t info;
  return_value_if_fail(async_call_info_init(&info, exec, on_result, ctx) == RET_OK, RET_FAIL);

  a = qaction_create(qaction_async_exec, &info, sizeof(info));
  ENSURE(qaction_set_on_event(a, qaction_async_on_event) == RET_OK);

  return action_thread_pool_exec(s_async_thread_pool, a);
}

ret_t async_call_init(void) {
  return_value_if_fail(s_async_thread_pool == NULL, RET_BAD_PARAMS);
  s_async_thread_pool = action_thread_pool_create(5, 1);
  return_value_if_fail(s_async_thread_pool != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t async_call_deinit(void) {
  return_value_if_fail(s_async_thread_pool != NULL, RET_BAD_PARAMS);

  action_thread_pool_destroy(s_async_thread_pool);
  s_async_thread_pool = NULL;

  return RET_OK;
}
