/**
 * File:   thread.c
 * Author: AWTK Develop Team
 * Brief:  thread on tos
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-10-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tos.h"

struct _tk_thread_t {
  void* args;
  k_task_t task;
  bool_t running;
  tk_thread_entry_t entry;
  char name[TK_NAME_LEN + 1];
  uint32_t stack_size;
  uint32_t priority;
};

ret_t tk_thread_set_name(tk_thread_t* thread, const char* name) {
  return_value_if_fail(thread != NULL && name != NULL, RET_BAD_PARAMS);

  tk_strncpy(thread->name, name, TK_NAME_LEN);

  return RET_OK;
}

ret_t tk_thread_set_stack_size(tk_thread_t* thread, uint32_t stack_size) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->stack_size = stack_size;

  return RET_OK;
}

ret_t tk_thread_set_priority(tk_thread_t* thread, uint32_t priority) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  thread->priority = priority;

  return RET_OK;
}

void* tk_thread_get_args(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, NULL);

  return thread->args;
}

tk_thread_t* tk_thread_create(tk_thread_entry_t entry, void* args) {
  tk_thread_t* thread = NULL;
  return_value_if_fail(entry != NULL, NULL);

  thread = (tk_thread_t*)TKMEM_ZALLOC(tk_thread_t);
  return_value_if_fail(thread != NULL, NULL);

  thread->args = args;
  thread->entry = entry;
  thread->stack_size = 1024;

  return thread;
}

static void entry(void* arg) {
  tk_thread_t* thread = (tk_thread_t*)arg;
  thread->entry(thread->args);
  thread->running = FALSE;
}

ret_t tk_thread_start(tk_thread_t* thread) {
  void* stack = NULL;
  uint32_t priority = 0;
  uint32_t stack_size = 0;
  k_err_t err = K_ERR_NONE;
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  priority = thread->priority;
  stack_size = tk_max(thread->stack_size, 1024);
  stack = TKMEM_ALLOC(stack_size);
  return_value_if_fail(stack != NULL, RET_OOM);

  err = tos_task_create(&(thread->task), thread->name, entry, thread, priority, stack,
                        thread->stack_size, 20);
  thread->running = err == K_ERR_NONE;

  return thread->running ? RET_OK : RET_FAIL;
}

ret_t tk_thread_join(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);
  if (thread->running) {
  }

  return RET_OK;
}

ret_t tk_thread_destroy(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);
  memset(thread, 0x00, sizeof(tk_thread_t));
  TKMEM_FREE(thread);

  return RET_OK;
}
