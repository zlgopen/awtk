/**
 * File:   fscript_iostream_file.c
 * Author: AWTK Develop Team
 * Brief:  iostream_file functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-06 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/buffer.h"
#include "tkc/fscript.h"
#include "streams/file/istream_file.h"
#include "streams/file/ostream_file.h"

static ret_t func_istream_file_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* mode = NULL;
  const char* filename = NULL;
  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  mode = args->size > 1 ? value_str(args->args + 1) : "rb";
  return_value_if_fail(filename != NULL && mode != NULL, RET_BAD_PARAMS);

  value_set_object(result, OBJECT(tk_istream_file_create_ex(filename, mode)));
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_ostream_file_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* mode = NULL;
  const char* filename = NULL;
  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  filename = value_str(args->args);
  mode = args->size > 1 ? value_str(args->args + 1) : "wb+";
  return_value_if_fail(filename != NULL && mode != NULL, RET_BAD_PARAMS);

  value_set_object(result, OBJECT(tk_ostream_file_create_ex(filename, mode)));
  result->free_handle = TRUE;

  return RET_OK;
}

ret_t fscript_iostream_file_register(void) {
  ENSURE(fscript_register_func("istream_file_create", func_istream_file_create) == RET_OK);
  ENSURE(fscript_register_func("ostream_file_create", func_ostream_file_create) == RET_OK);
  return RET_OK;
}
