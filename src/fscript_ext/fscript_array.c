/**
 * File:   fscript_array.c
 * Author: AWTK Develop Team
 * Brief:  array functions for fscript
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
#include "tkc/object_array.h"
static ret_t func_array_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = object_array_create();
  value_set_object(result, obj);
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_array_push(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  uint32_t i = 0;
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size > 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  for (i = 1; i < args->size; i++) {
    ret_t ret = object_array_push(obj, args->args + i);
    if (ret != RET_OK) {
      break;
    }
  }

  value_set_uint32(result, i - 1);

  return RET_OK;
}

static ret_t func_array_pop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  return object_array_pop(obj, result);
}

static ret_t func_array_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result,
                 object_array_set(obj, value_uint32(args->args + 1), args->args + 2) == RET_OK);

  return RET_OK;
}

static ret_t func_array_get(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  return object_array_get(obj, value_uint32(args->args + 1), result);
}

static ret_t func_array_insert(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result,
                 object_array_insert(obj, value_uint32(args->args + 1), args->args + 2) == RET_OK);

  return RET_OK;
}

static ret_t func_array_remove(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result, object_array_remove(obj, value_uint32(args->args + 1)) == RET_OK);

  return RET_OK;
}

static ret_t func_array_clear(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);

  value_set_bool(result, object_array_clear_props(obj) == RET_OK);

  return RET_OK;
}

static ret_t func_array_join(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t str;
  uint32_t i = 0;
  char buff[64];
  bool_t oom = FALSE;
  object_t* obj = NULL;
  const char* sep = NULL;
  object_array_t* arr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  arr = OBJECT_ARRAY(obj);
  return_value_if_fail(arr != NULL, RET_BAD_PARAMS);
  sep = value_str(args->args + 1);

  str_init(&str, 100);
  for (i = 0; i < arr->size; i++) {
    value_t* v = arr->props + i;
    if (i > 0 && sep != NULL) {
      if (str_append(&str, sep) == RET_OOM) {
        oom = TRUE;
        break;
      }
    }
    if (str_append(&str, value_str_ex(v, buff, sizeof(buff) - 1)) == RET_OOM) {
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

ret_t fscript_array_register(void) {
  ENSURE(fscript_register_func("array_create", func_array_create) == RET_OK);
  ENSURE(fscript_register_func("array_push", func_array_push) == RET_OK);
  ENSURE(fscript_register_func("array_pop", func_array_pop) == RET_OK);
  ENSURE(fscript_register_func("array_get", func_array_get) == RET_OK);
  ENSURE(fscript_register_func("array_set", func_array_set) == RET_OK);
  ENSURE(fscript_register_func("array_insert", func_array_insert) == RET_OK);
  ENSURE(fscript_register_func("array_remove", func_array_remove) == RET_OK);
  ENSURE(fscript_register_func("array_clear", func_array_clear) == RET_OK);
  ENSURE(fscript_register_func("array_join", func_array_join) == RET_OK);

  return RET_OK;
}
