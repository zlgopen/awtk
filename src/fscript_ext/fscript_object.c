/**
 * File:   fscript_ext.h
 * Author: AWTK Develop Team
 * Brief:  ext functions for fscript
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
#include "tkc/object_default.h"
#include "fscript_ext/fscript_object.h"

static ret_t func_object_ref(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  OBJECT_REF(obj);
  value_set_object(result, obj);

  return RET_OK;
}

static ret_t func_object_unref(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t func_object_get_prop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  if (object_get_prop(obj, value_str(args->args + 1), result) != RET_OK) {
    result->type = VALUE_TYPE_INVALID;
  }

  return RET_OK;
}

static ret_t func_object_remove_prop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  value_set_bool(result, object_remove_prop(obj, value_str(args->args + 1)) == RET_OK);

  return RET_OK;
}

static ret_t func_object_set_prop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  value_set_bool(result, object_set_prop(obj, value_str(args->args + 1), args->args + 2) == RET_OK);

  return RET_OK;
}

static ret_t func_object_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  object_t* obj = object_default_create();
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  value_set_object(result, obj);
  result->free_handle = TRUE;

  return RET_OK;
}

ret_t fscript_object_register(void) {
  ENSURE(fscript_register_func("object_create", func_object_create) == RET_OK);
  ENSURE(fscript_register_func("object_ref", func_object_ref) == RET_OK);
  ENSURE(fscript_register_func("object_unref", func_object_unref) == RET_OK);
  ENSURE(fscript_register_func("object_set", func_object_set_prop) == RET_OK);
  ENSURE(fscript_register_func("object_get", func_object_get_prop) == RET_OK);
  ENSURE(fscript_register_func("object_remove", func_object_remove_prop) == RET_OK);

  return RET_OK;
}
