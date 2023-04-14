/**
 * File:   log.c
 * Author: AWTK Develop Team
 * Brief:  log functions
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */
#include "tkc/types_def.h"
#include "tkc/log.h"
#include "tkc/mem.h"
#include "tkc/utils.h"

static log_level_t s_log_level = LOG_LEVEL_DEBUG;

ret_t log_set_log_level(log_level_t log_level) {
  s_log_level = log_level;

  return RET_OK;
}

log_level_t log_get_log_level(void) {
  return s_log_level;
}

int32_t log_dummy(const char* fmt, ...) {
  return 0;
}

#ifndef WITHOUT_FSCRIPT

#include "tkc/thread.h"
#define TK_LOG_BUFF_SIZE 255
static char* s_log_buff = NULL;
static void* s_debugger_log_ctx = NULL;
static tk_debugger_log_t s_debugger_log = NULL;
static uint64_t s_debugger_thread_id = 0;

ret_t log_notify_debugger(const char* format, ...) {
  va_list va;

  if (s_log_buff != NULL && s_debugger_log != NULL && s_debugger_thread_id == tk_thread_self()) {
    va_start(va, format);
    *s_log_buff = '\0';
    tk_vsnprintf(s_log_buff, TK_LOG_BUFF_SIZE, format, va);
    va_end(va);

    s_debugger_log(s_debugger_log_ctx, s_log_buff);
  }

  return RET_OK;
}

ret_t log_set_debugger_hook(tk_debugger_log_t log, void* ctx) {
  s_debugger_log = log;
  s_debugger_log_ctx = ctx;
  s_debugger_thread_id = tk_thread_self();

  if (log != NULL) {
    if (s_log_buff == NULL) {
      s_log_buff = TKMEM_ALLOC(TK_LOG_BUFF_SIZE + 1);
    }
  } else {
    TKMEM_FREE(s_log_buff);
  }

  return RET_OK;
}
#else
ret_t log_notify_debugger(const char* format, ...) {
  return RET_OK;
}
ret_t log_set_debugger_hook(tk_debugger_log_t log, void* ctx) {
  return RET_OK;
}
#endif /*WITHOUT_FSCRIPT*/
