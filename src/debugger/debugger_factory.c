/**
 * File:   debugger_factory.c
 * Author: AWTK Develop Team
 * Brief:  debugger factory
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
 * 2022-01-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/general_factory.h"
#include "debugger/debugger_factory.h"

static general_factory_t* s_debugger_factory = NULL;

ret_t debugger_factory_init(void) {
  return_value_if_fail(s_debugger_factory == NULL, RET_BAD_PARAMS);
  s_debugger_factory = general_factory_create();

  return s_debugger_factory != NULL ? RET_OK : RET_FAIL;
}

ret_t debugger_factory_reg(const char* lang, debugger_fscript_create_t create) {
  return_value_if_fail(s_debugger_factory != NULL, RET_BAD_PARAMS);
  return_value_if_fail(lang != NULL && create != NULL, RET_BAD_PARAMS);

  return general_factory_register(s_debugger_factory, lang, (tk_create_t)create);
}

debugger_t* debugger_factory_attach_debugger(const char* lang, const char* code_id) {
  debugger_t* debugger = NULL;
  debugger_fscript_create_t create = NULL;
  return_value_if_fail(s_debugger_factory != NULL, NULL);
  return_value_if_fail(lang != NULL && code_id != NULL, NULL);

  create = (debugger_fscript_create_t)general_factory_find(s_debugger_factory, lang);
  return_value_if_fail(create != NULL, NULL);

  debugger = create();
  return_value_if_fail(debugger != NULL, NULL);
  debugger_attach(debugger, lang, code_id);

  return debugger;
}

debugger_t* debugger_factory_launch_debugger(const char* lang, const binary_data_t* code) {
  debugger_t* debugger = NULL;
  debugger_fscript_create_t create = NULL;
  return_value_if_fail(s_debugger_factory != NULL, NULL);
  return_value_if_fail(lang != NULL && code != NULL, NULL);

  create = (debugger_fscript_create_t)general_factory_find(s_debugger_factory, lang);
  return_value_if_fail(create != NULL, NULL);

  debugger = create();
  return_value_if_fail(debugger != NULL, NULL);
  debugger_launch(debugger, lang, code);

  return debugger;
}

ret_t debugger_factory_deinit(void) {
  return_value_if_fail(s_debugger_factory != NULL, RET_BAD_PARAMS);
  general_factory_destroy(s_debugger_factory);
  s_debugger_factory = NULL;

  return RET_OK;
}
