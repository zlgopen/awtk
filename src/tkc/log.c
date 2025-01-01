/**
 * File:   log.c
 * Author: AWTK Develop Team
 * Brief:  log functions
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static tk_log_level_t s_log_level = LOG_LEVEL_DEBUG;

ret_t log_set_log_level(tk_log_level_t log_level) {
  s_log_level = log_level;

  return RET_OK;
}

tk_log_level_t log_get_log_level(void) {
  return s_log_level;
}

int32_t log_dummy(const char* fmt, ...) {
  return 0;
}

#ifndef WITHOUT_FSCRIPT

static void* s_log_hook_ctx = NULL;
static tk_log_hook_t s_log_hook = NULL;

ret_t log_notify(tk_log_level_t level, const char* format, ...) {
  va_list va;

  if (s_log_hook != NULL) {
    va_start(va, format);
    s_log_hook(s_log_hook_ctx, level, format, va);
    va_end(va);
  }

  return RET_OK;
}

ret_t log_set_hook(tk_log_hook_t log, void* ctx) {
  s_log_hook = log;
  s_log_hook_ctx = ctx;

  return RET_OK;
}
#else
ret_t log_notify(tk_log_level_t level, const char* format, ...) {
  return RET_OK;
}
ret_t log_set_hook(tk_log_hook_t log, void* ctx) {
  return RET_OK;
}
#endif /*WITHOUT_FSCRIPT*/
