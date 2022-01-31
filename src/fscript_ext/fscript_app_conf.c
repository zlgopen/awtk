/**
 * File:   fscript_app_conf.c
 * Author: AWTK Develop Team
 * Brief:  app_conf functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

FACTORY_TABLE_BEGIN(s_ext_app_conf)
FACTORY_TABLE_ENTRY("app_conf", func_app_conf)
FACTORY_TABLE_ENTRY("app_conf_save", func_app_conf_save)
FACTORY_TABLE_END()

ret_t fscript_app_conf_register(void) {
  return fscript_register_funcs(s_ext_app_conf);
}
