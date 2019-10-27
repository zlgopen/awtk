/**
 * File:   thread.c
 * Author: AWTK Develop Team
 * Brief:  thread on cmsis_os
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
 * 2019-10-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/thread.h"
#include "cmsis_os.h"

struct _tk_thread_t {
  void* args;
  tk_thread_entry_t entry;
  osThreadId id;
  bool_t running;
  
  const char* name;
  uint32_t stack_size;
  uint32_t priority;
};

ret_t tk_thread_set_name(tk_thread_t* thread, const char* name) {
  return_value_if_fail(thread != NULL && name != NULL, RET_BAD_PARAMS);

  thread->name = name;

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

  if (thread->id) {
    osThreadSetPriority(thread->id, priority);
  }

  return RET_OK;
}

void* tk_thread_get_args(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, NULL);

  return thread->args;
}

static void cmsis_os_thread_entry(void* arg) {
  tk_thread_t* thread = (tk_thread_t*)arg;
  thread->running = TRUE;
  thread->entry(thread->args);
  thread->running = FALSE;
}

tk_thread_t* tk_thread_create(tk_thread_entry_t entry, void* args) {
  osThreadDef_t def;
  tk_thread_t* thread = NULL;
  return_value_if_fail(entry != NULL, NULL);

  thread = (tk_thread_t*)TKMEM_ZALLOC(tk_thread_t);
  return_value_if_fail(thread != NULL, NULL);

  thread->args = args;
  thread->entry = entry;
  thread->def.stack_size = 1024;
  thread->def.entry = cmsis_os_thread_entry;

  return thread;
}

ret_t tk_thread_start(tk_thread_t* thread) {
  k_err_t err = K_ERR_NONE;
  return_value_if_fail(thread != NULL && !(thread->id), RET_BAD_PARAMS);

  thread->id = osThreadCreate(&(thread->def), thread->args);

  return thread->id != NULL ? RET_OK : RET_FAIL;
}

ret_t tk_thread_join(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);
  /*FIXME: impl*/
  return RET_OK;
}

ret_t tk_thread_destroy(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL && thread->id, RET_BAD_PARAMS);

  osThreadTerminate(thread->id);

  memset(thread, 0x00, sizeof(tk_thread_t));
  TKMEM_FREE(thread);

  return RET_OK;
}
