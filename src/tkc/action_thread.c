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
#include "tkc/action_thread_pool.h"
#include "tkc/waitable_action_queue.h"

static void* action_thread_entry(void* args) {
  qaction_t action;
  done_event_t done;
  action_thread_t* thread = (action_thread_t*)args;
  memset(&action, 0x00, sizeof(action));

  thread->quit = FALSE;
  thread->quited = FALSE;
  log_debug("action thread start\n");

  while (!(thread->quit)) {
    memset(&action, 0x00, sizeof(action));
    while (waitable_action_queue_recv(thread->queue, &action, 1000) == RET_OK) {
      ret_t ret = qaction_exec(&action);

      if (ret == RET_QUIT) {
        thread->quit = TRUE;
      }
      qaction_notify(&action, done_event_init(&done, ret));

      if (thread->max_actions_nr > 0) {
        thread->executed_actions_nr++;
        if (thread->executed_actions_nr >= thread->max_actions_nr) {
          action_thread_pool_put(thread->thread_pool, thread);
        }
      }
    }
  }
  thread->quited = TRUE;
  log_debug("action thread done\n");

  return NULL;
}

action_thread_t* action_thread_create(action_thread_pool_t* thread_pool) {
  action_thread_t* thread = NULL;
  return_value_if_fail(thread_pool != NULL, NULL);
  thread = TKMEM_ZALLOC(action_thread_t);
  return_value_if_fail(thread != NULL, NULL);

  thread->thread_pool = thread_pool;
  thread->thread = tk_thread_create(action_thread_entry, thread);
  goto_error_if_fail(thread->thread != NULL);

  thread->queue = waitable_action_queue_create(10);
  goto_error_if_fail(thread->queue != NULL);
  tk_thread_start(thread->thread);

  return thread;
error:

  if (thread->thread != NULL) {
    tk_thread_destroy(thread->thread);
  }

  if (thread->queue != NULL) {
    waitable_action_queue_destroy(thread->queue);
  }

  TKMEM_FREE(thread);

  return NULL;
}

ret_t action_thread_exec(action_thread_t* thread, qaction_t* action) {
  return_value_if_fail(thread != NULL && thread->queue != NULL, RET_BAD_PARAMS);
  return_value_if_fail(action != NULL && action->exec != NULL, RET_BAD_PARAMS);

  return waitable_action_queue_send(thread->queue, action, 1000);
}

ret_t action_thread_set_max_actions_nr(action_thread_t* thread, uint32_t max_actions_nr) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->max_actions_nr = max_actions_nr;

  return RET_OK;
}

static ret_t qaction_quit_exec(qaction_t* action) {
  return_value_if_fail(action != NULL, RET_BAD_PARAMS);

  return RET_QUIT;
}

static ret_t action_thread_quit(action_thread_t* thread) {
  qaction_t action;
  qaction_t* a = qaction_init(&action, qaction_quit_exec, NULL, 0);
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  if (thread->quited || !thread->queue) {
    return RET_OK;
  }

  thread->quit = TRUE;

  return waitable_action_queue_send(thread->queue, a, 10000);
}

ret_t action_thread_destroy(action_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  action_thread_quit(thread);
  tk_thread_join(thread->thread);
  tk_thread_destroy(thread->thread);
  waitable_action_queue_destroy(thread->queue);

  return RET_OK;
}
