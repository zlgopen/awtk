/**
 * File:   fscript_wbuffer.c
 * Author: AWTK Develop Team
 * Brief:  wbuffer functions for fscript
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
#include "tkc/object_wbuffer.h"
#include "fscript_utils.h"

static ret_t func_wbuffer_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 0, RET_BAD_PARAMS);
  value_set_object(result, object_wbuffer_create_extendable());
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_wbuffer_attach(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t size = 0;
  const uint8_t* data = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  FSCRIPT_FUNC_CHECK(fargs_get_data_and_size(args, &data, &size) == RET_OK, RET_BAD_PARAMS);

  value_set_object(result, object_wbuffer_create((uint8_t*)data, size));
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_wbuffer_skip(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  value_set_bool(result, wbuffer_skip(obj->wbuffer, value_int32(args->args + 1)) == RET_OK);

  return RET_OK;
}

static ret_t func_wbuffer_rewind(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(obj->wbuffer);
  value_set_bool(result, TRUE);

  return RET_OK;
}

static ret_t func_wbuffer_get_data(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  value_set_pointer(result, obj->wbuffer->data);

  return RET_OK;
}

static ret_t func_wbuffer_get_cursor(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  value_set_uint32(result, obj->wbuffer->cursor);

  return RET_OK;
}

static ret_t func_wbuffer_get_capacity(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  value_set_uint32(result, obj->wbuffer->capacity);

  return RET_OK;
}

static ret_t func_wbuffer_write_uint8(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    if (wbuffer_write_uint8(obj->wbuffer, value_uint8(iter)) != RET_OK) {
      break;
    }
  }
  value_set_int32(result, (i - 1) * sizeof(uint8_t));

  return RET_OK;
}

static ret_t func_wbuffer_write_uint16(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    if (wbuffer_write_uint16(obj->wbuffer, value_uint16(iter)) != RET_OK) {
      break;
    }
  }
  value_set_int32(result, (i - 1) * sizeof(uint16_t));

  return RET_OK;
}

static ret_t func_wbuffer_write_uint32(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    if (wbuffer_write_uint32(obj->wbuffer, value_uint32(iter)) != RET_OK) {
      break;
    }
  }
  value_set_int32(result, (i - 1) * sizeof(uint32_t));

  return RET_OK;
}

static ret_t func_wbuffer_write_uint64(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    if (wbuffer_write_uint64(obj->wbuffer, value_uint64(iter)) != RET_OK) {
      break;
    }
  }
  value_set_int32(result, (i - 1) * sizeof(uint64_t));

  return RET_OK;
}

static ret_t func_wbuffer_write_float(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    if (wbuffer_write_float(obj->wbuffer, value_float32(iter)) != RET_OK) {
      break;
    }
  }
  value_set_int32(result, (i - 1) * sizeof(float));

  return RET_OK;
}

static ret_t func_wbuffer_write_double(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    if (wbuffer_write_double(obj->wbuffer, value_double(iter)) != RET_OK) {
      break;
    }
  }
  value_set_int32(result, (i - 1) * sizeof(double));

  return RET_OK;
}

static ret_t func_wbuffer_write_string(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  uint32_t size = 0;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    const char* str = value_str(iter);
    if (str != NULL) {
      if (wbuffer_write_string(obj->wbuffer, str) != RET_OK) {
        break;
      }
      size += strlen(str) + 1;
    }
  }
  value_set_int32(result, size);

  return RET_OK;
}

static ret_t func_wbuffer_write_binary(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  void* data = NULL;
  uint32_t size = 0;
  ret_t ret = RET_OK;
  value_t* v = NULL;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 2, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  v = args->args + 1;
  if (v->type == VALUE_TYPE_BINARY) {
    binary_data_t* bin = value_binary_data(v);
    FSCRIPT_FUNC_CHECK(bin != NULL, RET_BAD_PARAMS);
    data = bin->data;
    size = bin->size;
  } else {
    data = value_pointer(args->args + 1);
    size = value_uint32(args->args + 2);
    FSCRIPT_FUNC_CHECK(data != NULL && size > 0, RET_BAD_PARAMS);
  }
  ret = wbuffer_write_binary(obj->wbuffer, data, size);
  if (ret == RET_OK) {
    value_set_uint32(result, size);
  } else {
    value_set_uint32(result, 0);
  }

  return ret;
}

FACTORY_TABLE_BEGIN(s_ext_wbuffer)
FACTORY_TABLE_ENTRY("wbuffer_create", func_wbuffer_create)
FACTORY_TABLE_ENTRY("wbuffer_attach", func_wbuffer_attach)
FACTORY_TABLE_ENTRY("wbuffer_skip", func_wbuffer_skip)
FACTORY_TABLE_ENTRY("wbuffer_rewind", func_wbuffer_rewind)

FACTORY_TABLE_ENTRY("wbuffer_write_uint8", func_wbuffer_write_uint8)
FACTORY_TABLE_ENTRY("wbuffer_write_uint16", func_wbuffer_write_uint16)
FACTORY_TABLE_ENTRY("wbuffer_write_uint32", func_wbuffer_write_uint32)
FACTORY_TABLE_ENTRY("wbuffer_write_uint64", func_wbuffer_write_uint64)
FACTORY_TABLE_ENTRY("wbuffer_write_int8", func_wbuffer_write_uint8)
FACTORY_TABLE_ENTRY("wbuffer_write_int16", func_wbuffer_write_uint16)
FACTORY_TABLE_ENTRY("wbuffer_write_int32", func_wbuffer_write_uint32)
FACTORY_TABLE_ENTRY("wbuffer_write_int64", func_wbuffer_write_uint64)

FACTORY_TABLE_ENTRY("wbuffer_write_float", func_wbuffer_write_float)
FACTORY_TABLE_ENTRY("wbuffer_write_double", func_wbuffer_write_double)
FACTORY_TABLE_ENTRY("wbuffer_write_string", func_wbuffer_write_string)
FACTORY_TABLE_ENTRY("wbuffer_write_binary", func_wbuffer_write_binary)

FACTORY_TABLE_ENTRY("wbuffer_get_data", func_wbuffer_get_data)
FACTORY_TABLE_ENTRY("wbuffer_get_cursor", func_wbuffer_get_cursor)
FACTORY_TABLE_ENTRY("wbuffer_get_capacity", func_wbuffer_get_capacity)
FACTORY_TABLE_END()

ret_t fscript_wbuffer_register(void) {
  return fscript_register_funcs(s_ext_wbuffer);
}
