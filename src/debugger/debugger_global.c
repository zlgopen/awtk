/**
 * File:   debugger_global.c
 * Author: AWTK Develop Team
 * Brief:  debugger global
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-01-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "debugger/debugger_fscript.h"
#include "debugger/debugger_factory.h"
#include "debugger/debugger_global.h"
#include "debugger/debugger_server.h"

static ret_t debugger_fscript_on_connect(fscript_t* fscript) {
  debugger_t* debugger = NULL;
  return_value_if_fail(fscript != NULL, RET_BAD_PARAMS);
  if (fscript->code_id == NULL) {
    return RET_OK;
  }

  debugger = debugger_server_find_debugger(fscript->code_id);
  if (debugger != NULL) {
    if (debugger_lock(debugger) == RET_OK) {
      debugger_fscript_set_fscript(debugger, fscript);
      debugger_unlock(debugger);
    }
  }

  return RET_FAIL;
}

static ret_t debugger_fscript_on_disconnect(fscript_t* fscript) {
  debugger_t* debugger = NULL;
  return_value_if_fail(fscript != NULL, RET_BAD_PARAMS);
  if (fscript->code_id == NULL) {
    return RET_OK;
  }

  debugger = debugger_server_find_debugger(fscript->code_id);
  if (debugger != NULL) {
    if (debugger_lock(debugger) == RET_OK) {
      debugger_fscript_set_fscript(debugger, NULL);
      debugger_unlock(debugger);
    }
  }

  return RET_FAIL;
}

static ret_t debugger_fscript_on_fscript_init(fscript_t* fscript, const char* code) {
  return RET_OK;
}

static ret_t debugger_fscript_on_fscript_before_exec(fscript_t* fscript) {
  return debugger_fscript_on_connect(fscript);
}

static ret_t debugger_fscript_on_fscript_after_exec(fscript_t* fscript) {
  return debugger_fscript_on_disconnect(fscript);
}

static ret_t debugger_fscript_on_fscript_deinit(fscript_t* fscript) {
  return RET_OK;
}

static const fscript_hooks_t s_fscript_hooks = {
    .on_init = debugger_fscript_on_fscript_init,
    .on_deinit = debugger_fscript_on_fscript_deinit,
    .set_var = debugger_fscript_set_var,
    .exec_func = debugger_fscript_exec_func,
    .before_exec = debugger_fscript_on_fscript_before_exec,
    .after_exec = debugger_fscript_on_fscript_after_exec,
};

ret_t debugger_global_init(void) {
  debugger_factory_init();
  debugger_factory_reg(DEBUGGER_LANG_FSCRIPT, debugger_fscript_create);

  fscript_set_hooks(&s_fscript_hooks);

  return RET_OK;
}

ret_t debugger_global_deinit(void) {
  fscript_set_hooks(NULL);
  debugger_factory_deinit();
  return RET_OK;
}
