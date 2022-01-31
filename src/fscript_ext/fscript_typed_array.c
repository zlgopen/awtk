/**
 * File:   fscript_typed_array.c
 * Author: AWTK Develop Team
 * Brief:  typed_array functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static ret_t func_typed_array_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_object_t* obj = NULL;
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
  result->free_handle = TRUE;

  return RET_OK;
}

static typed_array_t* get_typed_array(fscript_t* fscript, fscript_args_t* args) {
  tk_object_t* obj = value_object(args->args);
  return_value_if_fail(obj != NULL, NULL);

  return OBJECT_TYPED_ARRAY(obj)->arr;
}

static ret_t func_typed_array_push(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    ret_t ret = typed_array_push(arr, args->args + i);
    if (ret != RET_OK) {
      break;
    }
  }

  value_set_uint32(result, i - 1);

  return RET_OK;
}

static ret_t func_typed_array_pop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  return typed_array_pop(arr, result);
}

static ret_t func_typed_array_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result,
                 typed_array_set(arr, value_uint32(args->args + 1), args->args + 2) == RET_OK);

  return RET_OK;
}

static ret_t func_typed_array_get(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  typed_array_get(arr, value_uint32(args->args + 1), result);

  return RET_OK;
}

static ret_t func_typed_array_insert(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result,
                 typed_array_insert(arr, value_uint32(args->args + 1), args->args + 2) == RET_OK);

  return RET_OK;
}

static ret_t func_typed_array_remove(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result, typed_array_remove(arr, value_uint32(args->args + 1)) == RET_OK);

  return RET_OK;
}

static ret_t func_typed_array_clear(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result, typed_array_clear(arr) == RET_OK);

  return RET_OK;
}

static ret_t func_typed_array_join(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t str;
  value_t v;
  char buff[64];
  uint32_t i = 0;
  bool_t oom = FALSE;
  const char* sep = NULL;
  typed_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  arr = get_typed_array(fscript, args);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);
  sep = value_str(args->args + 1);

  str_init(&str, 100);
  for (i = 0; i < arr->size; i++) {
    if (i > 0 && sep != NULL) {
      if (str_append(&str, sep) == RET_OOM) {
        oom = TRUE;
        break;
      }
    }
    typed_array_get(arr, i, &v);
    if (str_append(&str, value_str_ex(&v, buff, sizeof(buff) - 1)) == RET_OOM) {
      oom = TRUE;
      break;
    }
  }

  if (!oom) {
    value_set_str(result, str.str);
    result->free_handle = TRUE;
  } else {
    str_reset(&str);
  }

  return RET_OK;
}

FACTORY_TABLE_BEGIN(s_ext_typed_array)
FACTORY_TABLE_ENTRY("typed_array_create", func_typed_array_create)
FACTORY_TABLE_ENTRY("typed_array_push", func_typed_array_push)
FACTORY_TABLE_ENTRY("typed_array_pop", func_typed_array_pop)
FACTORY_TABLE_ENTRY("typed_array_get", func_typed_array_get)
FACTORY_TABLE_ENTRY("typed_array_set", func_typed_array_set)
FACTORY_TABLE_ENTRY("typed_array_insert", func_typed_array_insert)
FACTORY_TABLE_ENTRY("typed_array_remove", func_typed_array_remove)
FACTORY_TABLE_ENTRY("typed_array_clear", func_typed_array_clear)
FACTORY_TABLE_ENTRY("typed_array_join", func_typed_array_join)
FACTORY_TABLE_END()

ret_t fscript_typed_array_register(void) {
  return fscript_register_funcs(s_ext_typed_array);
}
