/**
 * File:   fscript_typed_array.c
 * Author: AWTK Develop Team
 * Brief:  typed_array functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/fscript.h"
#include "tkc/object_typed_array.h"

static ret_t func_array_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  uint32_t capacity = 0;
  const char* stype = NULL;
  value_type_t type = VALUE_TYPE_INT8;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  stype = value_str(args->args);
  capacity = value_uint32(args->args + 1);

  if (tk_str_eq(stype, "i8")) {
    type = VALUE_TYPE_INT8;
  } else if (tk_str_eq(stype, "i16")) {
    type = VALUE_TYPE_INT16;
  } else if (tk_str_eq(stype, "i32")) {
    type = VALUE_TYPE_INT32;
  } else if (tk_str_eq(stype, "i64")) {
    type = VALUE_TYPE_INT64;
  } else if (tk_str_eq(stype, "u8")) {
    type = VALUE_TYPE_UINT8;
  } else if (tk_str_eq(stype, "u16")) {
    type = VALUE_TYPE_UINT16;
  } else if (tk_str_eq(stype, "u32")) {
    type = VALUE_TYPE_UINT32;
  } else if (tk_str_eq(stype, "u64")) {
    type = VALUE_TYPE_UINT64;
  } else if (tk_str_eq(stype, "f32")) {
    type = VALUE_TYPE_FLOAT32;
  } else if (tk_str_eq(stype, "float")) {
    type = VALUE_TYPE_DOUBLE;
  } else {
    type = VALUE_TYPE_INT32;
  }

  obj = object_typed_array_create(type, capacity);
  value_set_object(result, obj);

  return RET_OK;
}

static typed_array_t* get_typed_array(fscript_t* fscript, fscript_args_t* args) {
  object_t* obj = value_object(args->args);
  return_value_if_fail(obj != NULL, NULL);

  return OBJECT_TYPED_ARRAY(obj)->arr;
}

static ret_t func_array_push(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result, typed_array_push(arr, args->args + 1) == RET_OK);

  return RET_OK;
}

static ret_t func_array_pop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  return typed_array_pop(arr, result);
}

static ret_t func_array_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result,
                 typed_array_set(arr, value_uint32(args->args + 1), args->args + 2) == RET_OK);

  return RET_OK;
}

static ret_t func_array_get(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  typed_array_get(arr, value_uint32(args->args + 1), result);

  return RET_OK;
}

static ret_t func_array_size(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_uint32(result, arr->size);

  return RET_OK;
}

static ret_t func_array_byte_size(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_uint32(result, arr->size * arr->element_size);

  return RET_OK;
}

static ret_t func_array_insert(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result,
                 typed_array_insert(arr, value_uint32(args->args + 1), args->args + 2) == RET_OK);

  return RET_OK;
}

static ret_t func_array_remove(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result, typed_array_remove(arr, value_uint32(args->args + 1)) == RET_OK);

  return RET_OK;
}

ret_t fscript_typed_array_register(void) {
  ENSURE(fscript_register_func("typed_array_create", func_array_create) == RET_OK);
  ENSURE(fscript_register_func("typed_array_push", func_array_push) == RET_OK);
  ENSURE(fscript_register_func("typed_array_pop", func_array_pop) == RET_OK);
  ENSURE(fscript_register_func("typed_array_get", func_array_get) == RET_OK);
  ENSURE(fscript_register_func("typed_array_set", func_array_set) == RET_OK);
  ENSURE(fscript_register_func("typed_array_size", func_array_size) == RET_OK);
  ENSURE(fscript_register_func("typed_array_byte_size", func_array_byte_size) == RET_OK);
  ENSURE(fscript_register_func("typed_array_insert", func_array_insert) == RET_OK);
  ENSURE(fscript_register_func("typed_array_remove", func_array_remove) == RET_OK);

  return RET_OK;
}
