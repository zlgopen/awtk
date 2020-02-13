/**
 * File:   action_thread.c
 * Author: AWTK Develop Team
 * Brief:  action_thread
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
#include "tkc/action_thread.h"
#include "tkc/waitable_action_queue.h"
#include "tkc/waitable_action_queue.h"

static void* action_thread_entry(void* args) {
  done_event_t done;
  action_thread_t* thread = (action_thread_t*)args;

  thread->quit = FALSE;
  thread->quited = FALSE;
  log_debug("action thread start\n");

  while (!(thread->quit)) {
    qaction_t* action = NULL;
    while (waitable_action_queue_recv(thread->queue, &action, 1000) == RET_OK) {
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

static action_thread_t* action_thread_create_internal(void) {
  action_thread_t* thread = NULL;
  thread = TKMEM_ZALLOC(action_thread_t);
  return_value_if_fail(thread != NULL, NULL);

  thread->thread = tk_thread_create(action_thread_entry, thread);
  goto_error_if_fail(thread->thread != NULL);

  return thread;
error:

  TKMEM_FREE(thread);

  return NULL;
}

action_thread_t* action_thread_create(void) {
  action_thread_t* thread = NULL;
  waitable_action_queue_t* queue = waitable_action_queue_create(10);
  return_value_if_fail(queue != NULL, NULL);

  thread = action_thread_create_internal();
  if (thread != NULL) {
    thread->queue = queue;
    tk_thread_start(thread->thread);
  } else {
    waitable_action_queue_destroy(queue);
  }

  return thread;
}

action_thread_t* action_thread_create_with_queue(waitable_action_queue_t* queue) {
  action_thread_t* thread = NULL;
  return_value_if_fail(queue != NULL, NULL);

  thread = action_thread_create_internal();
  return_value_if_fail(thread != NULL, NULL);

  if (thread != NULL) {
    thread->queue = queue;
    thread->is_shared_queue = TRUE;
    tk_thread_start(thread->thread);
  }

  return thread;
}

ret_t action_thread_exec(action_thread_t* thread, qaction_t* action) {
  return_value_if_fail(thread != NULL && thread->queue != NULL, RET_BAD_PARAMS);
  return_value_if_fail(action != NULL && action->exec != NULL, RET_BAD_PARAMS);

  return waitable_action_queue_send(thread->queue, action, 3000);
}

static ret_t qaction_quit_exec(qaction_t* action) {
  return_value_if_fail(action != NULL, RET_BAD_PARAMS);

  return RET_QUIT;
}

static ret_t action_thread_quit(action_thread_t* thread) {
  qaction_t* a = qaction_create(qaction_quit_exec, NULL, 0);
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  if (thread->quited || !thread->queue) {
    return RET_OK;
  }

  thread->quit = TRUE;

  return waitable_action_queue_send(thread->queue, a, 3000);
}

ret_t action_thread_destroy(action_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  action_thread_quit(thread);
  tk_thread_join(thread->thread);
  tk_thread_destroy(thread->thread);

  if (!thread->is_shared_queue) {
    waitable_action_queue_destroy(thread->queue);
  }
  TKMEM_FREE(thread);

  return RET_OK;
}

ret_t action_thread_set_on_idle(action_thread_t* thread, action_thread_on_idle_t on_idle,
                                void* ctx) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->on_idle = on_idle;
  thread->on_idle_ctx = ctx;

  return RET_OK;
}

ret_t action_thread_set_on_quit(action_thread_t* thread, action_thread_on_quit_t on_quit,
                                void* ctx) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->on_quit = on_quit;
  thread->on_quit_ctx = ctx;

  return RET_OK;
}
