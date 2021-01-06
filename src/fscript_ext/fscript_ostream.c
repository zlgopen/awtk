/**
 * File:   fscript_ostream.c
 * Author: AWTK Develop Team
 * Brief:  ostream functions for fscript
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
#include "tkc/ostream.h"

static int32_t my_ostream_write(tk_ostream_t* ostream, const void* data, uint32_t size) {
  return tk_ostream_write_len(ostream, data, size, 1000);
}

static ret_t func_ostream_seek(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t offset = 0;
  tk_ostream_t* ostream = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);
  offset = value_uint32(args->args + 1);

  value_set_bool(result, tk_ostream_seek(ostream, offset) == RET_OK);

  return RET_OK;
}

static ret_t func_ostream_tell(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_ostream_t* ostream = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);

  value_set_uint32(result, tk_ostream_tell(ostream));

  return RET_OK;
}

static ret_t func_ostream_flush(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_ostream_t* ostream = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);

  value_set_bool(result, tk_ostream_flush(ostream) == RET_OK);

  return RET_OK;
}

static ret_t func_ostream_write_uint8(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  int32_t ret = 0;
  tk_ostream_t* ostream = NULL;
  value_set_uint32(result, 0);
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    uint8_t v = value_uint8(iter);
    ret += my_ostream_write(ostream, &v, sizeof(v));
  }
  value_set_int32(result, ret);

  return ret > 0 ? RET_OK : RET_FAIL;
}

static ret_t func_ostream_write_uint16(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  int32_t ret = 0;
  tk_ostream_t* ostream = NULL;
  value_set_uint32(result, 0);
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    uint16_t v = value_uint16(iter);
    ret += my_ostream_write(ostream, &v, sizeof(v));
  }
  value_set_int32(result, ret);

  return ret > 0 ? RET_OK : RET_FAIL;
}

static ret_t func_ostream_write_uint32(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  int32_t ret = 0;
  tk_ostream_t* ostream = NULL;
  value_set_uint32(result, 0);
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    uint32_t v = value_uint32(iter);
    ret += my_ostream_write(ostream, &v, sizeof(v));
  }
  value_set_int32(result, ret);

  return ret > 0 ? RET_OK : RET_FAIL;
}

static ret_t func_ostream_write_uint64(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  int32_t ret = 0;
  tk_ostream_t* ostream = NULL;
  value_set_uint32(result, 0);
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    uint64_t v = value_uint64(iter);
    ret += my_ostream_write(ostream, &v, sizeof(v));
  }
  value_set_int32(result, ret);

  return ret > 0 ? RET_OK : RET_FAIL;
}

static ret_t func_ostream_write_float(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  int32_t ret = 0;
  tk_ostream_t* ostream = NULL;
  value_set_uint32(result, 0);
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    float v = value_float(iter);
    ret += my_ostream_write(ostream, &v, sizeof(v));
  }
  value_set_int32(result, ret);

  return ret > 0 ? RET_OK : RET_FAIL;
}

static ret_t func_ostream_write_double(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  int32_t ret = 0;
  tk_ostream_t* ostream = NULL;
  value_set_uint32(result, 0);
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    double v = value_double(iter);
    ret += my_ostream_write(ostream, &v, sizeof(v));
  }
  value_set_int32(result, ret);

  return ret > 0 ? RET_OK : RET_FAIL;
}

static ret_t func_ostream_write_string(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  int32_t ret = 0;
  tk_ostream_t* ostream = NULL;
  value_set_uint32(result, 0);
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    const char* v = value_str(iter);
    if (v != NULL) {
      ret += my_ostream_write(ostream, v, strlen(v));
    }
  }

  value_set_int32(result, ret);

  return ret > 0 ? RET_OK : RET_FAIL;
}

static ret_t func_ostream_write_binary(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int32_t ret = 0;
  uint32_t size = 0;
  value_t* v = NULL;
  const uint8_t* data = NULL;
  tk_ostream_t* ostream = NULL;
  value_set_uint32(result, 0);
  FSCRIPT_FUNC_CHECK(args->size == 2 || args->size == 3, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);

  v = args->args + 1;
  if (v->type == VALUE_TYPE_BINARY) {
    binary_data_t* bin = value_binary_data(v);
    if (bin != NULL) {
      data = bin->data;
      size = bin->size;
    }
  } else {
    data = value_pointer(v);
    size = value_uint32(args->args + 2);
  }
  return_value_if_fail(data != NULL && size > 0, RET_BAD_PARAMS);

  ret = my_ostream_write(ostream, data, size);
  value_set_uint32(result, ret);

  return ret > 0 ? RET_OK : RET_FAIL;
}

ret_t fscript_ostream_register(void) {
  ENSURE(fscript_register_func("ostream_seek", func_ostream_seek) == RET_OK);
  ENSURE(fscript_register_func("ostream_tell", func_ostream_tell) == RET_OK);
  ENSURE(fscript_register_func("ostream_flush", func_ostream_flush) == RET_OK);

  ENSURE(fscript_register_func("ostream_write_uint8", func_ostream_write_uint8) == RET_OK);
  ENSURE(fscript_register_func("ostream_write_uint16", func_ostream_write_uint16) == RET_OK);
  ENSURE(fscript_register_func("ostream_write_uint32", func_ostream_write_uint32) == RET_OK);
  ENSURE(fscript_register_func("ostream_write_uint64", func_ostream_write_uint64) == RET_OK);
  ENSURE(fscript_register_func("ostream_write_int8", func_ostream_write_uint8) == RET_OK);
  ENSURE(fscript_register_func("ostream_write_int16", func_ostream_write_uint16) == RET_OK);
  ENSURE(fscript_register_func("ostream_write_int32", func_ostream_write_uint32) == RET_OK);
  ENSURE(fscript_register_func("ostream_write_int64", func_ostream_write_uint64) == RET_OK);

  ENSURE(fscript_register_func("ostream_write_float", func_ostream_write_float) == RET_OK);
  ENSURE(fscript_register_func("ostream_write_double", func_ostream_write_double) == RET_OK);
  ENSURE(fscript_register_func("ostream_write_string", func_ostream_write_string) == RET_OK);
  ENSURE(fscript_register_func("ostream_write_binary", func_ostream_write_binary) == RET_OK);

  return RET_OK;
}
