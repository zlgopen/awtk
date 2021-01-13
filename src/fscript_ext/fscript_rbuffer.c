/**
 * File:   fscript_rbuffer.c
 * Author: AWTK Develop Team
 * Brief:  rbuffer functions for fscript
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

#include "tkc/mem.h"
#include "tkc/buffer.h"
#include "tkc/fscript.h"
#include "tkc/object_rbuffer.h"
#include "fscript_utils.h"

static ret_t func_rbuffer_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t size = 0;
  const uint8_t* data = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  FSCRIPT_FUNC_CHECK(fargs_get_data_and_size(args, &data, &size) == RET_OK, RET_BAD_PARAMS);

  value_set_object(result, object_rbuffer_create(data, size));
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_rbuffer_skip(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  value_set_bool(result, rbuffer_skip(obj->rbuffer, value_int32(args->args + 1)) == RET_OK);

  return RET_OK;
}

static ret_t func_rbuffer_rewind(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  rbuffer_rewind(obj->rbuffer);
  value_set_bool(result, TRUE);

  return RET_OK;
}

static ret_t func_rbuffer_get_data(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  value_set_pointer(result, (void*)(obj->rbuffer->data));

  return RET_OK;
}

static ret_t func_rbuffer_get_cursor(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  value_set_uint32(result, obj->rbuffer->cursor);

  return RET_OK;
}

static ret_t func_rbuffer_get_capacity(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  value_set_uint32(result, obj->rbuffer->capacity);

  return RET_OK;
}

static ret_t func_rbuffer_read_uint8(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint8_t v = 0;
  ret_t ret = RET_OK;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  ret = rbuffer_read_uint8(obj->rbuffer, &v);
  value_set_uint8(result, v);

  return ret;
}

static ret_t func_rbuffer_read_uint16(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint16_t v = 0;
  ret_t ret = RET_OK;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  ret = rbuffer_read_uint16(obj->rbuffer, &v);
  value_set_uint16(result, v);

  return ret;
}

static ret_t func_rbuffer_read_uint32(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t v = 0;
  ret_t ret = RET_OK;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  ret = rbuffer_read_uint32(obj->rbuffer, &v);
  value_set_uint32(result, v);

  return ret;
}

static ret_t func_rbuffer_read_uint64(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint64_t v = 0;
  ret_t ret = RET_OK;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  ret = rbuffer_read_uint64(obj->rbuffer, &v);
  value_set_uint64(result, v);

  return ret;
}

static ret_t func_rbuffer_read_int8(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint8_t v = 0;
  ret_t ret = RET_OK;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  ret = rbuffer_read_uint8(obj->rbuffer, &v);
  value_set_int8(result, v);

  return ret;
}

static ret_t func_rbuffer_read_int16(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint16_t v = 0;
  ret_t ret = RET_OK;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  ret = rbuffer_read_uint16(obj->rbuffer, &v);
  value_set_int16(result, v);

  return ret;
}

static ret_t func_rbuffer_read_int32(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t v = 0;
  ret_t ret = RET_OK;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  ret = rbuffer_read_uint32(obj->rbuffer, &v);
  value_set_int32(result, v);

  return ret;
}

static ret_t func_rbuffer_read_int64(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint64_t v = 0;
  ret_t ret = RET_OK;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  ret = rbuffer_read_uint64(obj->rbuffer, &v);
  value_set_int64(result, v);

  return ret;
}

static ret_t func_rbuffer_read_float(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  float v = 0;
  ret_t ret = RET_OK;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  ret = rbuffer_read_float(obj->rbuffer, &v);
  value_set_float(result, v);

  return ret;
}

static ret_t func_rbuffer_read_double(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  double v = 0;
  ret_t ret = RET_OK;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  ret = rbuffer_read_double(obj->rbuffer, &v);
  value_set_double(result, v);

  return ret;
}

static ret_t func_rbuffer_read_string(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  const char* v = 0;
  ret_t ret = RET_OK;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);

  ret = rbuffer_read_string(obj->rbuffer, &v);
  value_set_str(result, v);

  return ret;
}

static ret_t func_rbuffer_read_binary(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t size = 0;
  ret_t ret = RET_OK;
  const uint8_t* data = NULL;
  object_rbuffer_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = OBJECT_RBUFFER(value_object(args->args));
  FSCRIPT_FUNC_CHECK(obj != NULL && obj->rbuffer != NULL, RET_BAD_PARAMS);
  size = value_uint32(args->args + 1);
  return_value_if_fail(size > 0, RET_BAD_PARAMS);

  data = obj->rbuffer->data + obj->rbuffer->cursor;
  ret = rbuffer_skip(obj->rbuffer, size);
  if (ret == RET_OK) {
    value_set_binary_data(result, (void*)data, size);
  }

  return ret;
}

ret_t fscript_rbuffer_register(void) {
  ENSURE(fscript_register_func("rbuffer_create", func_rbuffer_create) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_skip", func_rbuffer_skip) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_rewind", func_rbuffer_rewind) == RET_OK);

  ENSURE(fscript_register_func("rbuffer_read_uint8", func_rbuffer_read_uint8) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_read_uint16", func_rbuffer_read_uint16) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_read_uint32", func_rbuffer_read_uint32) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_read_uint64", func_rbuffer_read_uint64) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_read_int8", func_rbuffer_read_int8) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_read_int16", func_rbuffer_read_int16) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_read_int32", func_rbuffer_read_int32) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_read_int64", func_rbuffer_read_int64) == RET_OK);

  ENSURE(fscript_register_func("rbuffer_read_float", func_rbuffer_read_float) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_read_double", func_rbuffer_read_double) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_read_string", func_rbuffer_read_string) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_read_binary", func_rbuffer_read_binary) == RET_OK);

  ENSURE(fscript_register_func("rbuffer_get_data", func_rbuffer_get_data) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_get_cursor", func_rbuffer_get_cursor) == RET_OK);
  ENSURE(fscript_register_func("rbuffer_get_capacity", func_rbuffer_get_capacity) == RET_OK);

  return RET_OK;
}
