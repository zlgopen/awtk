/**
 * File:   thread.c
 * Author: AWTK Develop Team
 * Brief:  thread on cmsis_os
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/mutex.h"
#include "cmsis_os.h"

struct _tk_thread_t {
  void* args;
  tk_thread_entry_t entry;

  bool_t running;
  const char* name;
  uint32_t stack_size;
  uint32_t priority;
  void* stackbase;
  tk_mutex_t* mutex;

  osThreadId id;
  osThreadDef_t def;
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
    osThreadSetPriority(thread->id, (osPriority)priority);
  }

  return RET_OK;
}

void* tk_thread_get_args(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, NULL);

  return thread->args;
}

static void cmsis_os_thread_entry(const void* arg) {
  tk_thread_t* thread = (tk_thread_t*)arg;

  tk_mutex_lock(thread->mutex);
  thread->running = TRUE;
  thread->entry(thread->args);
  thread->running = FALSE;
  tk_mutex_unlock(thread->mutex);
}

tk_thread_t* tk_thread_create(tk_thread_entry_t entry, void* args) {
  tk_thread_t* thread = NULL;
  return_value_if_fail(entry != NULL, NULL);

  thread = (tk_thread_t*)TKMEM_ZALLOC(tk_thread_t);
  return_value_if_fail(thread != NULL, NULL);

  thread->args = args;
  thread->entry = entry;
  thread->mutex = tk_mutex_create();

  return thread;
}

ret_t tk_thread_start(tk_thread_t* thread) {
  osThreadDef_t* def = NULL;
  return_value_if_fail(thread != NULL && !(thread->id), RET_BAD_PARAMS);

  def = &(thread->def);
  memset(def, 0x00, sizeof(*def));

#ifdef _TOS_CONFIG_H_
  def->timeslice = 20;
  def->task = TKMEM_ZALLOC(k_task_t);
  def->name = (char*)(thread->name);
  def->stacksize = thread->stack_size;
  def->tpriority = (osPriority)(thread->priority);
  def->pthread = cmsis_os_thread_entry;
  def->stackbase = TKMEM_ALLOC(def->stacksize + 4);
  return_value_if_fail(def->stackbase != NULL, RET_OOM);
  thread->stackbase = def->stackbase;
#endif /*_TOS_CONFIG_H_*/

  thread->id = osThreadCreate(def, thread);

  return thread->id != NULL ? RET_OK : RET_FAIL;
}

ret_t tk_thread_join(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL, RET_BAD_PARAMS);

  return_value_if_fail(tk_mutex_lock(thread->mutex) == RET_OK, RET_FAIL);
  tk_mutex_unlock(thread->mutex);

  return RET_OK;
}

ret_t tk_thread_destroy(tk_thread_t* thread) {
  return_value_if_fail(thread != NULL && thread->id, RET_BAD_PARAMS);

  if (thread->stackbase != NULL) {
    TKMEM_FREE(thread->stackbase);
  }

  if (thread->mutex != NULL) {
    tk_mutex_destroy(thread->mutex);
  }

  osThreadTerminate(thread->id);
#ifdef _TOS_CONFIG_H_
  TKMEM_FREE(thread->def.task);
#endif /*_TOS_CONFIG_H_*/

  memset(thread, 0x00, sizeof(tk_thread_t));
  TKMEM_FREE(thread);

  return RET_OK;
}
