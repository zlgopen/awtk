/**
 * File:   view_model.c
 * Author: AWTK Develop Team
 * Brief:  view_model
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-01-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "mvvm/base/view_model.h"
#include "tkc/expr_eval.h"
#include "tkc/utils.h"

static ret_t model_on_event(void* ctx, event_t* e) {
  view_model_t* vm = VIEW_MODEL(ctx);

  emitter_dispatch(EMITTER(vm), e);

  return RET_OK;
}

view_model_t* view_model_init(view_model_t* vm, view_model_type_t type, model_t* model) {
  return_value_if_fail(vm != NULL && model != NULL, NULL);

  vm->type = type;
  vm->model = model;

  emitter_on(EMITTER(vm->model), EVT_PROP_CHANGED, model_on_event, vm);

  return vm;
}

bool_t view_model_has_prop(view_model_t* vm, const char* name) {
  return_value_if_fail(vm != NULL && name != NULL, FALSE);

  return object_has_prop(OBJECT(vm), name) || object_has_prop(OBJECT(vm->model), name);
}

ret_t view_model_get_prop(view_model_t* vm, const char* name, value_t* value) {
  return_value_if_fail(vm != NULL && name != NULL && value != NULL, RET_BAD_PARAMS);

  if (object_get_prop(OBJECT(vm), name, value) == RET_OK) {
    return RET_OK;
  }

  return object_get_prop(OBJECT(vm->model), name, value);
}

ret_t view_model_set_prop(view_model_t* vm, const char* name, const value_t* value) {
  return_value_if_fail(vm != NULL && name != NULL && value != NULL, RET_BAD_PARAMS);

  if (object_has_prop(OBJECT(vm), name)) {
    if (object_set_prop(OBJECT(vm), name, value) == RET_OK) {
      return RET_OK;
    }
  }

  return object_set_prop(OBJECT(vm->model), name, value);
}

bool_t view_model_can_exec(view_model_t* vm, const char* name, const char* args) {
  return object_can_exec(OBJECT(vm), name, args) || object_can_exec(OBJECT(vm->model), name, args);
}

ret_t view_model_exec(view_model_t* vm, const char* name, const char* args) {
  return_value_if_fail(vm != NULL && name != NULL, RET_BAD_PARAMS);

  if (object_exec(OBJECT(vm), name, args) == RET_OK) {
    return RET_OK;
  }

  return object_exec(OBJECT(vm->model), name, args);
}

static EvalFunc vm_get_func(const char* name, void* user_data) {
  const EvalHooks* hooks = eval_default_hooks();

  return hooks->get_func(name, user_data);
}

static EvalResult vm_get_variable(const char* name, void* user_data, ExprValue* output) {
  value_t value;
  view_model_t* vm = (view_model_t*)user_data;
  const EvalHooks* hooks = eval_default_hooks();

  if (view_model_get_prop(vm, name, &value) == RET_OK) {
    if (value.type == VALUE_TYPE_STRING) {
      const char* str = value_str(&value);
      expr_value_set_string(output, str, strlen(str));
    } else {
      expr_value_set_number(output, value_double(&value));
    }

    return EVAL_RESULT_OK;
  }

  return hooks->get_variable(name, user_data, output);
}

ret_t view_model_eval(view_model_t* vm, const char* expr, value_t* v) {
  object_t* obj = OBJECT(vm);
  return_value_if_fail(expr != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);

  if (tk_is_valid_name(expr)) {
    return view_model_get_prop(vm, expr, v);
  } else {
    EvalHooks hooks;
    ExprValue result;
    EvalResult ret;

    hooks.get_func = vm_get_func;
    hooks.get_variable = vm_get_variable;

    ret = eval_execute(expr, &hooks, (void*)vm, &result);
    if (ret == EVAL_RESULT_OK) {
      if (result.type == EXPR_VALUE_TYPE_STRING) {
        value_dup_str(v, result.v.str.str);
      } else {
        double res = result.v.val;
        if (res > (int64_t)res) {
          value_set_double(v, res);
        } else {
          value_set_int64(v, (int64_t)res);
        }
      }
      expr_value_clear(&result);

      return RET_OK;
    } else {
      log_warn("expr error: %s\n", eval_result_to_string(ret));
      value_set_int(v, 0);
      return RET_FAIL;
    }
  }
}

ret_t view_model_deinit(view_model_t* vm) {
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  emitter_off_by_func(EMITTER(vm->model), EVT_PROP_CHANGED, model_on_event, vm);
  object_unref(OBJECT(vm->model));

  return RET_OK;
}
