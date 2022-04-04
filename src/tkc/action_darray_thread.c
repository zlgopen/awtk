/**
 * File:   action_darray_thread.c
 * Author: AWTK Develop Team
 * Brief:  action_darray_thread
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
#include "tkc/waitable_action_darray.h"
#include "tkc/action_darray_thread.h"

static void* action_darray_thread_entry(void* args) {
  done_event_t done;
  action_darray_thread_t* thread = (action_darray_thread_t*)args;

  thread->quit = FALSE;
  thread->quited = FALSE;
  log_debug("action thread start\n");

  while (!(thread->quit)) {
    qaction_t* action = NULL;
    while (waitable_action_darray_recv(thread->darray, &action, 1000) == RET_OK) {
      ret_t ret = qaction_exec(action);

      if (ret == RET_QUIT) {
        thread->quit = TRUE;
      }

      if (qaction_notify(action, done_event_init(&done, ret)) == RET_NOT_IMPL) {
        qaction_destroy(action);
      }

      thread->executed_actions_nr++;
    }

    if (thread->on_idle != NULL) {
      if (thread->on_idle(thread->on_idle_ctx, thread) == RET_QUIT) {
        thread->quit = TRUE;
        break;
      }
    }
  }

  thread->quited = TRUE;
  if (thread->on_quit != NULL) {
    thread->on_quit(thread->on_quit_ctx, thread);
  }
  log_debug("action thread done\n");

  return NULL;
}

static action_darray_thread_t* action_darray_thread_create_internal(void) {
  action_darray_thread_t* thread = NULL;
  thread = TKMEM_ZALLOC(action_darray_thread_t);
  return_value_if_fail(thread != NULL, NULL);

  thread->thread = tk_thread_create(action_darray_thread_entry, thread);
  goto_error_if_fail(thread->thread != NULL);

  return thread;
error:

  TKMEM_FREE(thread);

  return NULL;
}

action_darray_thread_t* action_darray_thread_create(void) {
  return action_darray_thread_create_ex(NULL, 0, TK_THREAD_PRIORITY_NORMAL);
}

action_darray_thread_t* action_darray_thread_create_ex(const char* name, uint32_t stack_size,
                                                       tk_thread_priority_t priority) {
  action_darray_thread_t* thread = NULL;
  waitable_action_darray_t* darray = waitable_action_darray_create(10);
  return_value_if_fail(darray != NULL, NULL);

  thread = action_darray_thread_create_internal();
  if (thread != NULL) {
    thread->darray = darray;
    if (name != NULL) {
      tk_thread_set_name(thread->thread, name);
    }
    if (priority != TK_THREAD_PRIORITY_NORMAL) {
      tk_thread_set_priority(thread->thread, priority);
    }
    if (stack_size != 0) {
      tk_thread_set_stack_size(thread->thread, stack_size);
    }
    tk_thread_start(thread->thread);
  } else {
    waitable_action_darray_destroy(darray);
  }

  return thread;
}

action_darray_thread_t* action_darray_thread_create_with_darray(waitable_action_darray_t* darray) {
  return action_darray_thread_create_with_darray_ex(darray, NULL, 0, TK_THREAD_PRIORITY_NORMAL);
}

action_darray_thread_t* action_darray_thread_create_with_darray_ex(waitable_action_darray_t* darray,
                                                                   const char* name,
                                                                   uint32_t stack_size,
                                                                   tk_thread_priority_t priority) {
  action_darray_thread_t* thread = NULL;
  return_value_if_fail(darray != NULL, NULL);

  thread = action_darray_thread_create_internal();
  return_value_if_fail(thread != NULL, NULL);

  if (thread != NULL) {
    thread->darray = darray;
    thread->is_shared_darray = TRUE;

    if (name != NULL) {
      tk_thread_set_name(thread->thread, name);
    }
    if (priority != TK_THREAD_PRIORITY_NORMAL) {
      tk_thread_set_priority(thread->thread, priority);
    }
    if (stack_size != 0) {
      tk_thread_set_stack_size(thread->thread, stack_size);
    }
    tk_thread_start(thread->thread);
  }

  return thread;
}

ret_t action_darray_thread_set_strategy(action_darray_thread_t* thread,
                                        action_darray_thread_strategy_t strategy) {
  thread->strategy = strategy;
  return RET_OK;
}

ret_t action_darray_thread_exec(action_darray_thread_t* thread, qaction_t* action) {
  return_value_if_fail(thread != NULL && thread->darray != NULL, RET_BAD_PARAMS);
  return_value_if_fail(action != NULL && action->exec != NULL, RET_BAD_PARAMS);

  if (thread->strategy == ACTION_DARRAY_THREAD_STRATEGY_FLUSH) {
    waitable_action_darray_remove(thread->darray, action->exec);
    return waitable_action_darray_send(thread->darray, action, 3000);
  } else if (thread->strategy == ACTION_DARRAY_THREAD_STRATEGY_REPLACE) {
    if (waitable_action_darray_replace(thread->darray, action) != RET_OK) {
      return waitable_action_darray_send(thread->darray, action, 3000);
    }
    return RET_OK;
  } else {
    return waitable_action_darray_send(thread->darray, action, 3000);
  }
}

static ret_t qaction_quit_exec(qaction_t* action) {
  return_value_if_fail(action != NULL, RET_BAD_PARAMS);

  return RET_QUIT;
}

static ret_t action_darray_thread_quit(action_darray_thread_t* thread) {
  qaction_t* a = qaction_create(qaction_quit_exec, NULL, 0);
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  if (thread->quited || !thread->darray) {
    return RET_OK;
  }

  thread->quit = TRUE;

  return waitable_action_darray_send(thread->darray, a, 3000);
}

ret_t action_darray_thread_destroy(action_darray_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  action_darray_thread_quit(thread);
  tk_thread_join(thread->thread);
  tk_thread_destroy(thread->thread);

  if (!thread->is_shared_darray) {
    waitable_action_darray_destroy(thread->darray);
  }
  TKMEM_FREE(thread);

  return RET_OK;
}

ret_t action_darray_thread_set_on_idle(action_darray_thread_t* thread,
                                       action_darray_thread_on_idle_t on_idle, void* ctx) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->on_idle = on_idle;
  thread->on_idle_ctx = ctx;

  return RET_OK;
}

ret_t action_darray_thread_set_on_quit(action_darray_thread_t* thread,
                                       action_darray_thread_on_quit_t on_quit, void* ctx) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->on_quit = on_quit;
  thread->on_quit_ctx = ctx;

  return RET_OK;
}
