/**
 * File:   thread.c
 * Author: AWTK Develop Team
 * Brief:  thread on rtthread
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-11-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/thread.h"
#include "tkc/mutex.h"

struct _tk_thread_t {
  void* args;
  tk_thread_entry_t entry;
};

ret_t tk_thread_set_name(tk_thread_t* thread, const char* name) {
  return RET_NOT_IMPL;
}

ret_t tk_thread_set_stack_size(tk_thread_t* thread, uint32_t stack_size) {
  return RET_NOT_IMPL;
}

int32_t tk_thread_get_priority_from_platform(tk_thread_priority_t priority) {
  return 0;
}

ret_t tk_thread_set_priority(tk_thread_t* thread, tk_thread_priority_t priority) {
  return RET_NOT_IMPL;
}

void* tk_thread_get_args(tk_thread_t* thread) {
  return thread->args;
}

tk_thread_t* tk_thread_create(tk_thread_entry_t entry, void* args) {
  return NULL;
}

ret_t tk_thread_start(tk_thread_t* thread) {
  return RET_NOT_IMPL;
}

ret_t tk_thread_join(tk_thread_t* thread) {
  return RET_NOT_IMPL;
}

ret_t tk_thread_destroy(tk_thread_t* thread) {
  return RET_NOT_IMPL;
}
