/**
 * File:   fscript_iostream.c
 * Author: AWTK Develop Team
 * Brief:  iostream functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-04 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/buffer.h"
#include "tkc/fscript.h"
#include "tkc/iostream.h"

static ret_t func_iostream_get_istream(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_iostream_t* iostream = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  iostream = TK_IOSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(iostream != NULL, RET_BAD_PARAMS);

  value_set_object(result, TK_OBJECT(tk_iostream_get_istream(iostream)));

  return RET_OK;
}

static ret_t func_iostream_get_ostream(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_iostream_t* iostream = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  iostream = TK_IOSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(iostream != NULL, RET_BAD_PARAMS);

  value_set_object(result, TK_OBJECT(tk_iostream_get_ostream(iostream)));

  return RET_OK;
}

FACTORY_TABLE_BEGIN(s_ext_iostream)
FACTORY_TABLE_ENTRY("iostream_get_istream", func_iostream_get_istream)
FACTORY_TABLE_ENTRY("iostream_get_ostream", func_iostream_get_ostream)
FACTORY_TABLE_END()

ret_t fscript_iostream_register(void) {
  return fscript_register_funcs(s_ext_iostream);
}
