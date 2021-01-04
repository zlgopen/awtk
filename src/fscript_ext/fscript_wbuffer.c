/**
 * File:   fscript_wbuffer.c
 * Author: AWTK Develop Team
 * Brief:  wbuffer functions for fscript
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
#include "tkc/object_wbuffer.h"

static ret_t func_wbuffer_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 0, RET_BAD_PARAMS);
  value_set_object(result, object_wbuffer_create_extendable());

  return RET_OK;
}

static ret_t func_wbuffer_attach(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t size = 0;
  uint8_t* data = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  data = (uint8_t*)value_pointer(args->args);
  size = value_uint32(args->args + 1);
  FSCRIPT_FUNC_CHECK(data != NULL, RET_BAD_PARAMS);

  value_set_object(result, object_wbuffer_create(data, size));

  return RET_OK;
}

static ret_t func_wbuffer_write(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    value_t* iter = args->args + i;
    switch (iter->type) {
      case VALUE_TYPE_INT8:
      case VALUE_TYPE_UINT8: {
        wbuffer_write_uint8(obj->wbuffer, value_uint8(iter));
        break;
      }
      case VALUE_TYPE_INT16:
      case VALUE_TYPE_UINT16: {
        wbuffer_write_uint16(obj->wbuffer, value_uint16(iter));
        break;
      }
      case VALUE_TYPE_INT32:
      case VALUE_TYPE_UINT32: {
        wbuffer_write_uint32(obj->wbuffer, value_uint32(iter));
        break;
      }
      case VALUE_TYPE_INT64:
      case VALUE_TYPE_UINT64: {
        wbuffer_write_uint64(obj->wbuffer, value_uint64(iter));
        break;
      }
      case VALUE_TYPE_FLOAT:
      case VALUE_TYPE_FLOAT32: {
        wbuffer_write_float(obj->wbuffer, value_float32(iter));
        break;
      }
      case VALUE_TYPE_DOUBLE: {
        wbuffer_write_double(obj->wbuffer, value_double(iter));
        break;
      }
      case VALUE_TYPE_STRING: {
        wbuffer_write_string(obj->wbuffer, value_str(iter));
        break;
      }
      case VALUE_TYPE_BINARY: {
        binary_data_t* data = value_binary_data(iter);
        FSCRIPT_FUNC_CHECK(data != NULL, RET_BAD_PARAMS);
        wbuffer_write_binary(obj->wbuffer, data->data, data->size);
        break;
      }
      default: {
        FSCRIPT_FUNC_CHECK(FALSE, RET_BAD_PARAMS);
      }
    }
  }

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

  obj->wbuffer->cursor = 0;
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
  ret_t ret = RET_OK;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  ret = wbuffer_write_uint8(obj->wbuffer, value_uint8(args->args + 1));
  value_set_bool(result, ret == RET_OK);

  return ret;
}

static ret_t func_wbuffer_write_uint16(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  ret_t ret = RET_OK;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  ret = wbuffer_write_uint16(obj->wbuffer, value_uint16(args->args + 1));
  value_set_bool(result, ret == RET_OK);

  return ret;
}

static ret_t func_wbuffer_write_uint32(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  ret_t ret = RET_OK;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  ret = wbuffer_write_uint32(obj->wbuffer, value_uint32(args->args + 1));
  value_set_bool(result, ret == RET_OK);

  return ret;
}

static ret_t func_wbuffer_write_uint64(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  ret_t ret = RET_OK;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  ret = wbuffer_write_uint64(obj->wbuffer, value_uint64(args->args + 1));
  value_set_bool(result, ret == RET_OK);

  return ret;
}

static ret_t func_wbuffer_write_float(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  ret_t ret = RET_OK;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  ret = wbuffer_write_float(obj->wbuffer, value_float32(args->args + 1));
  value_set_bool(result, ret == RET_OK);

  return ret;
}

static ret_t func_wbuffer_write_double(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  ret_t ret = RET_OK;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  ret = wbuffer_write_double(obj->wbuffer, value_double(args->args + 1));
  value_set_bool(result, ret == RET_OK);

  return ret;
}

static ret_t func_wbuffer_write_string(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  ret_t ret = RET_OK;
  object_wbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = OBJECT_WBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->wbuffer != NULL, RET_BAD_PARAMS);

  ret = wbuffer_write_string(obj->wbuffer, value_str(args->args + 1));
  value_set_bool(result, ret == RET_OK);

  return ret;
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
 
  v = args->args+1;
  if(v->type == VALUE_TYPE_BINARY) {
    binary_data_t* bin = value_binary_data(v);
    FSCRIPT_FUNC_CHECK(bin != NULL, RET_BAD_PARAMS);
    data = bin->data;
    size = bin->size;
  } else {
    data = value_pointer(args->args+1);
    size = value_uint32(args->args+2);
    FSCRIPT_FUNC_CHECK(data != NULL && size > 0, RET_BAD_PARAMS);
  }
  ret = wbuffer_write_binary(obj->wbuffer, data, size);
  value_set_bool(result, ret == RET_OK);

  return ret;
}

ret_t fscript_wbuffer_register(void) {
  ENSURE(fscript_register_func("wbuffer_create", func_wbuffer_create) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_attach", func_wbuffer_attach) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_write", func_wbuffer_write) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_skip", func_wbuffer_skip) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_rewind", func_wbuffer_rewind) == RET_OK);

  ENSURE(fscript_register_func("wbuffer_write_uint8", func_wbuffer_write_uint8) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_write_uint16", func_wbuffer_write_uint16) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_write_uint32", func_wbuffer_write_uint32) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_write_uint64", func_wbuffer_write_uint64) == RET_OK);

  ENSURE(fscript_register_func("wbuffer_write_float", func_wbuffer_write_float) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_write_double", func_wbuffer_write_double) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_write_string", func_wbuffer_write_string) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_write_binary", func_wbuffer_write_binary) == RET_OK);

  ENSURE(fscript_register_func("wbuffer_get_data", func_wbuffer_get_data) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_get_cursor", func_wbuffer_get_cursor) == RET_OK);
  ENSURE(fscript_register_func("wbuffer_get_capacity", func_wbuffer_get_capacity) == RET_OK);

  return RET_OK;
}
