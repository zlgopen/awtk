/**
 * File:   fscript_istream.c
 * Author: AWTK Develop Team
 * Brief:  istream functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-06 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/buffer.h"
#include "tkc/fscript.h"
#include "tkc/istream.h"

static ret_t func_istream_seek(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t offset = 0;
  tk_istream_t* istream = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);
  offset = value_uint32(args->args + 1);

  value_set_bool(result, tk_istream_seek(istream, offset) == RET_OK);

  return RET_OK;
}

static ret_t func_istream_tell(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_istream_t* istream = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);

  value_set_uint32(result, tk_istream_tell(istream));

  return RET_OK;
}

static ret_t func_istream_is_eos(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_istream_t* istream = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);

  value_set_bool(result, tk_istream_eos(istream));

  return RET_OK;
}

static ret_t func_istream_read_uint8(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint8_t v = 0;
  int32_t ret = 0;
  uint32_t timeout = 0;
  tk_istream_t* istream = NULL;
  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);
  timeout = args->size > 1 ? value_uint32(args->args + 1) : 0;

  ret = tk_istream_read_len(istream, &v, sizeof(v), timeout);
  if (ret == sizeof(v)) {
    value_set_uint8(result, v);
  }

  return ret == sizeof(v) ? RET_OK : RET_FAIL;
}

static ret_t func_istream_read_uint16(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint16_t v = 0;
  int32_t ret = 0;
  uint32_t timeout = 0;
  tk_istream_t* istream = NULL;
  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);
  timeout = args->size > 1 ? value_uint32(args->args + 1) : 0;

  ret = tk_istream_read_len(istream, &v, sizeof(v), timeout);
  if (ret == sizeof(v)) {
    value_set_uint16(result, v);
  }

  return ret == sizeof(v) ? RET_OK : RET_FAIL;
}

static ret_t func_istream_read_uint32(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t v = 0;
  int32_t ret = 0;
  uint32_t timeout = 0;
  tk_istream_t* istream = NULL;
  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);
  timeout = args->size > 1 ? value_uint32(args->args + 1) : 0;

  ret = tk_istream_read_len(istream, &v, sizeof(v), timeout);
  if (ret == sizeof(v)) {
    value_set_uint32(result, v);
  }

  return ret == sizeof(v) ? RET_OK : RET_FAIL;
}

static ret_t func_istream_read_uint64(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint64_t v = 0;
  int32_t ret = 0;
  uint32_t timeout = 0;
  tk_istream_t* istream = NULL;
  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);
  timeout = args->size > 1 ? value_uint32(args->args + 1) : 0;

  ret = tk_istream_read_len(istream, &v, sizeof(v), timeout);
  if (ret == sizeof(v)) {
    value_set_uint64(result, v);
  }

  return ret == sizeof(v) ? RET_OK : RET_FAIL;
}

static ret_t func_istream_read_int8(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  ret_t ret = func_istream_read_uint8(fscript, args, result);
  if (result->type != VALUE_TYPE_INVALID) {
    result->type = VALUE_TYPE_INT8;
  }
  return ret;
}

static ret_t func_istream_read_int16(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  ret_t ret = func_istream_read_uint16(fscript, args, result);
  if (result->type != VALUE_TYPE_INVALID) {
    result->type = VALUE_TYPE_INT16;
  }
  return ret;
}

static ret_t func_istream_read_int32(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  ret_t ret = func_istream_read_uint32(fscript, args, result);
  if (result->type != VALUE_TYPE_INVALID) {
    result->type = VALUE_TYPE_INT32;
  }
  return ret;
}

static ret_t func_istream_read_int64(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  ret_t ret = func_istream_read_uint64(fscript, args, result);
  if (result->type != VALUE_TYPE_INVALID) {
    result->type = VALUE_TYPE_INT64;
  }
  return ret;
}

static ret_t func_istream_read_float(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  float v = 0;
  int32_t ret = 0;
  uint32_t timeout = 0;
  tk_istream_t* istream = NULL;
  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);
  timeout = args->size > 1 ? value_uint32(args->args + 1) : 0;

  ret = tk_istream_read_len(istream, &v, sizeof(v), timeout);
  if (ret == sizeof(v)) {
    value_set_float(result, v);
  }

  return ret == sizeof(v) ? RET_OK : RET_FAIL;
}

static ret_t func_istream_read_double(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  double v = 0;
  int32_t ret = 0;
  uint32_t timeout = 0;
  tk_istream_t* istream = NULL;
  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);
  timeout = args->size > 1 ? value_uint32(args->args + 1) : 0;

  ret = tk_istream_read_len(istream, &v, sizeof(v), timeout);
  if (ret == sizeof(v)) {
    value_set_double(result, v);
  }

  return ret == sizeof(v) ? RET_OK : RET_FAIL;
}

static ret_t func_istream_read_string(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char* v = 0;
  int32_t ret = 0;
  uint32_t size = 0;
  uint32_t timeout = 0;
  tk_istream_t* istream = NULL;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);
  size = args->size > 1 ? value_uint32(args->args + 1) : 0;
  timeout = args->size > 2 ? value_uint32(args->args + 2) : 0;
  FSCRIPT_FUNC_CHECK(size > 0, RET_BAD_PARAMS);
  v = TKMEM_ALLOC(size + 1);
  FSCRIPT_FUNC_CHECK(v != NULL, RET_BAD_PARAMS);

  ret = tk_istream_read_len(istream, v, size, timeout);
  if (ret >= 0) {
    v[ret] = '\0';
    value_set_str(result, v);
    result->free_handle = TRUE;
  } else {
    TKMEM_FREE(v);
  }

  return ret >= 0 ? RET_OK : RET_FAIL;
}

static ret_t func_istream_read_binary(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char* v = 0;
  int32_t ret = 0;
  uint32_t size = 0;
  uint32_t timeout = 0;
  tk_istream_t* istream = NULL;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);
  size = args->size > 1 ? value_uint32(args->args + 1) : 0;
  timeout = args->size > 2 ? value_uint32(args->args + 2) : 0;
  FSCRIPT_FUNC_CHECK(size > 0, RET_BAD_PARAMS);
  v = TKMEM_ALLOC(size + 1);
  FSCRIPT_FUNC_CHECK(v != NULL, RET_BAD_PARAMS);

  ret = tk_istream_read_len(istream, v, size, timeout);
  if (ret >= 0) {
    v[ret] = '\0';
    value_set_binary_data(result, v, ret);
    result->free_handle = TRUE;
  } else {
    TKMEM_FREE(v);
  }

  return ret >= 0 ? RET_OK : RET_FAIL;
}

static ret_t func_istream_read_line(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char* v = 0;
  int32_t ret = 0;
  uint32_t size = 0;
  uint32_t timeout = 0;
  tk_istream_t* istream = NULL;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  istream = TK_ISTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(istream != NULL && istream != NULL, RET_BAD_PARAMS);
  size = args->size > 1 ? value_uint32(args->args + 1) : 0;
  timeout = args->size > 2 ? value_uint32(args->args + 2) : 0;
  FSCRIPT_FUNC_CHECK(size > 0, RET_BAD_PARAMS);
  v = TKMEM_ALLOC(size + 1);
  FSCRIPT_FUNC_CHECK(v != NULL, RET_BAD_PARAMS);

  ret = tk_istream_read_line(istream, v, size, timeout);
  if (ret >= 0) {
    v[ret] = '\0';
    value_set_str(result, v);
    result->free_handle = TRUE;
  } else {
    TKMEM_FREE(v);
  }

  return ret >= 0 ? RET_OK : RET_FAIL;
}

FACTORY_TABLE_BEGIN(s_ext_istream)
FACTORY_TABLE_ENTRY("istream_seek", func_istream_seek)
FACTORY_TABLE_ENTRY("istream_tell", func_istream_tell)
FACTORY_TABLE_ENTRY("istream_read_uint8", func_istream_read_uint8)
FACTORY_TABLE_ENTRY("istream_read_uint16", func_istream_read_uint16)
FACTORY_TABLE_ENTRY("istream_read_uint32", func_istream_read_uint32)
FACTORY_TABLE_ENTRY("istream_read_uint64", func_istream_read_uint64)
FACTORY_TABLE_ENTRY("istream_read_int8", func_istream_read_int8)
FACTORY_TABLE_ENTRY("istream_read_int16", func_istream_read_int16)
FACTORY_TABLE_ENTRY("istream_read_int32", func_istream_read_int32)
FACTORY_TABLE_ENTRY("istream_read_int64", func_istream_read_int64)
FACTORY_TABLE_ENTRY("istream_read_float", func_istream_read_float)
FACTORY_TABLE_ENTRY("istream_read_double", func_istream_read_double)
FACTORY_TABLE_ENTRY("istream_read_string", func_istream_read_string)
FACTORY_TABLE_ENTRY("istream_read_binary", func_istream_read_binary)
FACTORY_TABLE_ENTRY("istream_read_line", func_istream_read_line)
FACTORY_TABLE_ENTRY("istream_is_eos", func_istream_is_eos)
FACTORY_TABLE_END()

ret_t fscript_istream_register(void) {
  return fscript_register_funcs(s_ext_istream);
}
