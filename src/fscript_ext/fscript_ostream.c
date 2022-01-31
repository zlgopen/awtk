/**
 * File:   fscript_ostream.c
 * Author: AWTK Develop Team
 * Brief:  ostream functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  char str[64];
  int32_t ret = 0;
  uint32_t size = -1;
  value_t* v = NULL;
  uint32_t slen = 0;
  const uint8_t* data = NULL;
  tk_ostream_t* ostream = NULL;
  value_set_uint32(result, 0);
  FSCRIPT_FUNC_CHECK(args->size == 2 || args->size == 3, RET_BAD_PARAMS);
  ostream = TK_OSTREAM(value_object(args->args));
  FSCRIPT_FUNC_CHECK(ostream != NULL, RET_BAD_PARAMS);

  v = args->args + 1;
  if (args->size > 2) {
    size = value_uint32(args->args + 2);
  }
  if (v->type == VALUE_TYPE_STRING) {
    data = (const uint8_t*)value_str(v);
    if (data != NULL) {
      slen = strlen((char*)data);
    }
  } else {
    value_str_ex(v, str, sizeof(str) - 1);
    data = (const uint8_t*)str;
    slen = strlen(str);
  }
  size = tk_min(slen, size);
  return_value_if_fail(data != NULL && size > 0, RET_BAD_PARAMS);

  ret = my_ostream_write(ostream, data, size);
  value_set_uint32(result, ret);

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
  if (args->size > 2) {
    size = value_uint32(args->args + 2);
  }
  if (v->type == VALUE_TYPE_BINARY) {
    binary_data_t* bin = value_binary_data(v);
    if (bin != NULL) {
      data = bin->data;
      size = tk_min(bin->size, size);
    }
  } else {
    data = value_pointer(v);
  }
  return_value_if_fail(data != NULL && size > 0, RET_BAD_PARAMS);

  ret = my_ostream_write(ostream, data, size);
  value_set_uint32(result, ret);

  return ret > 0 ? RET_OK : RET_FAIL;
}

FACTORY_TABLE_BEGIN(s_ext_ostream)
FACTORY_TABLE_ENTRY("ostream_seek", func_ostream_seek)
FACTORY_TABLE_ENTRY("ostream_tell", func_ostream_tell)
FACTORY_TABLE_ENTRY("ostream_flush", func_ostream_flush)
FACTORY_TABLE_ENTRY("ostream_write_uint8", func_ostream_write_uint8)
FACTORY_TABLE_ENTRY("ostream_write_uint16", func_ostream_write_uint16)
FACTORY_TABLE_ENTRY("ostream_write_uint32", func_ostream_write_uint32)
FACTORY_TABLE_ENTRY("ostream_write_uint64", func_ostream_write_uint64)
FACTORY_TABLE_ENTRY("ostream_write_int8", func_ostream_write_uint8)
FACTORY_TABLE_ENTRY("ostream_write_int16", func_ostream_write_uint16)
FACTORY_TABLE_ENTRY("ostream_write_int32", func_ostream_write_uint32)
FACTORY_TABLE_ENTRY("ostream_write_int64", func_ostream_write_uint64)
FACTORY_TABLE_ENTRY("ostream_write_float", func_ostream_write_float)
FACTORY_TABLE_ENTRY("ostream_write_double", func_ostream_write_double)
FACTORY_TABLE_ENTRY("ostream_write_string", func_ostream_write_string)
FACTORY_TABLE_ENTRY("ostream_write_binary", func_ostream_write_binary)
FACTORY_TABLE_END()

ret_t fscript_ostream_register(void) {
  return fscript_register_funcs(s_ext_ostream);
}
