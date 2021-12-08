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
  tk_object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  TK_OBJECT_REF(obj);
  value_set_object(result, obj);

  return RET_OK;
}

static ret_t func_object_unref(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  TK_OBJECT_UNREF(obj);

  return RET_OK;
}

static ret_t func_object_get_prop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  if (tk_object_get_prop(obj, value_str(args->args + 1), result) != RET_OK) {
    if (args->size > 2) {
      value_copy(result, args->args + 2);
    } else {
      result->type = VALUE_TYPE_INVALID;
    }
  }

  return RET_OK;
}

static ret_t func_object_remove_prop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  value_set_bool(result, tk_object_remove_prop(obj, value_str(args->args + 1)) == RET_OK);

  return RET_OK;
}

static ret_t func_object_set_prop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  value_set_bool(result,
                 tk_object_set_prop(obj, value_str(args->args + 1), args->args + 2) == RET_OK);

  return RET_OK;
}

static ret_t func_object_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_object_t* obj = object_default_create();
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  value_set_object(result, obj);
  result->free_handle = TRUE;

  return RET_OK;
}

static ret_t func_object_forward_events(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_object_t* src_obj = NULL;
  tk_object_t* dst_obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  src_obj = value_object(args->args);
  dst_obj = value_object(args->args + 1);
  return_value_if_fail(src_obj != NULL && dst_obj != NULL, RET_BAD_PARAMS);

  emitter_on(EMITTER(src_obj), EVT_PROPS_CHANGED, (event_func_t)emitter_dispatch, dst_obj);
  emitter_on(EMITTER(src_obj), EVT_PROP_CHANGED, (event_func_t)emitter_dispatch, dst_obj);
  emitter_on(EMITTER(src_obj), EVT_ITEMS_CHANGED, (event_func_t)emitter_dispatch, dst_obj);

  return RET_OK;
}

FACTORY_TABLE_BEGIN(s_ext_object)
FACTORY_TABLE_ENTRY("object_create", func_object_create)
FACTORY_TABLE_ENTRY("object_ref", func_object_ref)
FACTORY_TABLE_ENTRY("object_unref", func_object_unref)
FACTORY_TABLE_ENTRY("object_set", func_object_set_prop)
FACTORY_TABLE_ENTRY("object_get", func_object_get_prop)
FACTORY_TABLE_ENTRY("object_remove", func_object_remove_prop)
FACTORY_TABLE_ENTRY("object_forward_events", func_object_forward_events)
FACTORY_TABLE_END()

ret_t fscript_object_register(void) {
  return fscript_register_funcs(s_ext_object);
}
