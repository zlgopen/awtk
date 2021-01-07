/**
 * File:   fscript_app_conf.c
 * Author: AWTK Develop Team
 * Brief:  app_conf functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-07 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/fscript.h"
#include "conf_io/app_conf.h"

static ret_t func_app_conf(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_set_object(result, app_conf_get_instance());
  return RET_OK;
}

static ret_t func_app_conf_save(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_set_bool(result, app_conf_save() == RET_OK);
  return RET_OK;
}

ret_t fscript_app_conf_register(void) {
  ENSURE(fscript_register_func("app_conf", func_app_conf) == RET_OK);
  ENSURE(fscript_register_func("app_conf_save", func_app_conf_save) == RET_OK);

  return RET_OK;
}
