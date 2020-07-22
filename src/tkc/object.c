/**
 * File:   object.c
 * Author: AWTK Develop Team
 * Brief:  reference count object
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/event.h"
#include "tkc/object.h"
#include "tkc/expr_eval.h"

ret_t object_set_name(object_t* obj, const char* name) {
  ret_t ret = RET_OK;
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);

  obj->name = tk_str_copy(obj->name, name);

  return ret;
}

static ret_t object_destroy(object_t* obj) {
  ret_t ret = RET_OK;
  event_t e = event_init(EVT_DESTROY, obj);
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);

  emitter_dispatch((emitter_t*)obj, (event_t*)(&e));

  if (obj->vt->on_destroy != NULL) {
    ret = obj->vt->on_destroy(obj);
  }

  obj->vt = NULL;
  obj->ref_count = 0;
  emitter_deinit((emitter_t*)obj);
  TKMEM_FREE(obj->name);
  TKMEM_FREE(obj);

  return ret;
}

object_t* object_create(const object_vtable_t* vt) {
  object_t* obj = NULL;
  return_value_if_fail(vt != NULL && vt->size >= sizeof(object_t), NULL);

  obj = (object_t*)TKMEM_ALLOC(vt->size);
  return_value_if_fail(obj != NULL, NULL);

  memset(obj, 0x00, vt->size);

  obj->vt = vt;
  obj->ref_count = 1;
  emitter_init((emitter_t*)obj);

  return obj;
}

ret_t object_unref(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count > 0, RET_BAD_PARAMS);
  return_value_if_fail(!(obj->visiting), RET_BUSY);

  if (obj->ref_count == 1) {
    object_destroy(obj);
  } else {
    obj->ref_count--;
  }

  return RET_OK;
}

object_t* object_ref(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, NULL);

  obj->ref_count++;

  return obj;
}

static ret_t object_get_prop_by_name(object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && v != NULL, RET_BAD_PARAMS);

  value_set_int(v, 0);
  if (obj->vt->get_prop != NULL) {
    ret = obj->vt->get_prop(obj, name, v);
  }

  return ret;
}

ret_t object_get_prop_by_path(object_t* obj, const char* name, value_t* v) {
  char* p = NULL;
  uint32_t len = 0;
  char path[MAX_PATH + 1];
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && v != NULL, RET_BAD_PARAMS);

  len = strlen(name);
  return_value_if_fail(len <= MAX_PATH, RET_BAD_PARAMS);

  memcpy(path, name, len + 1);

  name = path;
  do {
    p = strchr(name, '.');
    if (p != NULL) {
      *p = '\0';
    }

    if (object_get_prop_by_name(obj, name, v) != RET_OK) {
      break;
    }

    if (p == NULL) {
      ret = RET_OK;
      break;
    }

    if (v->type == VALUE_TYPE_OBJECT) {
      obj = value_object(v);
    } else {
      break;
    }

    name = p + 1;
  } while (p != NULL);

  return ret;
}

ret_t object_get_prop(object_t* obj, const char* name, value_t* v) {
  return object_get_prop_by_name(obj, name, v);
}

const char* object_get_prop_str(object_t* obj, const char* name) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_str(&v);
  } else {
    return NULL;
  }
}

void* object_get_prop_pointer(object_t* obj, const char* name) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_pointer(&v);
  } else {
    return NULL;
  }
}

object_t* object_get_prop_object(object_t* obj, const char* name) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_object(&v);
  } else {
    return NULL;
  }
}

int32_t object_get_prop_int(object_t* obj, const char* name, int32_t defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_int(&v);
  } else {
    return defval;
  }
}

bool_t object_get_prop_bool(object_t* obj, const char* name, bool_t defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_bool(&v);
  } else {
    return defval;
  }
}

float_t object_get_prop_float(object_t* obj, const char* name, float_t defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_float(&v);
  } else {
    return defval;
  }
}

ret_t object_notify_changed(object_t* obj) {
  event_t e = event_init(EVT_PROPS_CHANGED, obj);

  return emitter_dispatch((emitter_t*)obj, &e);
}

ret_t object_set_prop(object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);
  return_value_if_fail(!(obj->visiting), RET_BUSY);

  if (obj->vt->set_prop != NULL) {
    prop_change_event_t e;

    e.name = name;
    e.value = v;

    e.e = event_init(EVT_PROP_WILL_CHANGE, obj);
    emitter_dispatch((emitter_t*)obj, (event_t*)(&e));

    ret = obj->vt->set_prop(obj, name, v);
    if (ret == RET_OK) {
      e.e = event_init(EVT_PROP_CHANGED, obj);
      emitter_dispatch((emitter_t*)obj, (event_t*)(&e));
    }
  }

  return ret;
}

ret_t object_set_prop_str(object_t* obj, const char* name, const char* value) {
  value_t v;
  value_set_str(&v, value);

  return object_set_prop(obj, name, &v);
}

ret_t object_set_prop_pointer(object_t* obj, const char* name, void* value) {
  value_t v;
  value_set_pointer(&v, value);

  return object_set_prop(obj, name, &v);
}

ret_t object_set_prop_object(object_t* obj, const char* name, object_t* value) {
  value_t v;
  value_set_object(&v, value);

  return object_set_prop(obj, name, &v);
}

ret_t object_set_prop_int(object_t* obj, const char* name, int32_t value) {
  value_t v;
  value_set_int(&v, value);

  return object_set_prop(obj, name, &v);
}

ret_t object_set_prop_bool(object_t* obj, const char* name, bool_t value) {
  value_t v;
  value_set_bool(&v, value);

  return object_set_prop(obj, name, &v);
}

ret_t object_set_prop_float(object_t* obj, const char* name, float_t value) {
  value_t v;
  value_set_float(&v, value);

  return object_set_prop(obj, name, &v);
}

ret_t object_remove_prop(object_t* obj, const char* name) {
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);
  return_value_if_fail(!(obj->visiting), RET_BUSY);

  if (obj->vt->remove_prop != NULL) {
    ret = obj->vt->remove_prop(obj, name);
  }

  return ret;
}

ret_t object_foreach_prop(object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_NOT_IMPL;
  return_value_if_fail(on_prop != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);
  return_value_if_fail(!(obj->visiting), RET_BUSY);

  if (obj->vt->foreach_prop != NULL) {
    obj->visiting = TRUE;
    ret = obj->vt->foreach_prop(obj, on_prop, ctx);
    obj->visiting = FALSE;
  }

  return ret;
}

int object_compare(object_t* obj, object_t* other) {
  int32_t ret = -1;
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, -1);
  return_value_if_fail(other != NULL && other->vt != NULL && other->ref_count >= 0, -1);

  if (obj->vt->compare != NULL) {
    ret = obj->vt->compare(obj, other);
  }

  return ret;
}

bool_t object_can_exec(object_t* obj, const char* name, const char* args) {
  bool_t ret = FALSE;
  cmd_exec_event_t e;
  return_value_if_fail(name != NULL, FALSE);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, FALSE);

  if (emitter_dispatch(EMITTER(obj), cmd_exec_event_init(&e, EVT_CMD_CAN_EXEC, name, args)) !=
      RET_OK) {
    return e.can_exec;
  }

  if (obj->vt->can_exec != NULL) {
    ret = obj->vt->can_exec(obj, name, args);
  }

  return ret;
}

ret_t object_exec(object_t* obj, const char* name, const char* args) {
  cmd_exec_event_t e;
  event_t* evt = NULL;
  ret_t ret = RET_NOT_IMPL;
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);

  if (emitter_dispatch(EMITTER(obj), cmd_exec_event_init(&e, EVT_CMD_WILL_EXEC, name, args)) !=
      RET_OK) {
    return RET_FAIL;
  }

  if (obj->vt->exec != NULL) {
    ret = obj->vt->exec(obj, name, args);
  }

  evt = cmd_exec_event_init(&e, EVT_CMD_EXECED, name, args);
  e.result = ret;
  emitter_dispatch(EMITTER(obj), evt);

  return e.result;
}

bool_t object_has_prop(object_t* obj, const char* name) {
  value_t v;
  ret_t ret = RET_OK;
  return_value_if_fail(name != NULL, FALSE);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, FALSE);

  ret = object_get_prop(obj, name, &v);
  if (ret == RET_OK) {
    value_reset(&v);
  }

  return ret == RET_OK;
}

ret_t object_copy_prop(object_t* obj, object_t* src, const char* name) {
  value_t v;
  ret_t ret = RET_FAIL;
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);
  return_value_if_fail(src != NULL && src->vt != NULL && src->ref_count >= 0, RET_BAD_PARAMS);

  if (object_get_prop(src, name, &v) == RET_OK) {
    ret = object_set_prop(obj, name, &v);
    value_reset(&v);
  }

  return ret;
}

#ifndef AWTK_LITE
static EvalFunc obj_get_func(const char* name, void* user_data) {
  const EvalHooks* hooks = eval_default_hooks();

  return hooks->get_func(name, user_data);
}

static EvalResult obj_get_variable(const char* name, void* user_data, ExprValue* output) {
  value_t value;
  object_t* obj = (object_t*)user_data;
  const EvalHooks* hooks = eval_default_hooks();

  if (object_get_prop(obj, name, &value) == RET_OK) {
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

ret_t object_eval(object_t* obj, const char* expr, value_t* v) {
  return_value_if_fail(expr != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);

  if (tk_is_valid_name(expr)) {
    return object_get_prop(obj, expr, v);
  } else {
    EvalHooks hooks;
    ExprValue result;
    EvalResult ret;

    hooks.get_func = obj_get_func;
    hooks.get_variable = obj_get_variable;

    ret = eval_execute(expr, &hooks, (void*)obj, &result);
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
#else
ret_t object_eval(object_t* obj, const char* expr, value_t* v) {
  return_value_if_fail(expr != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);

  if (tk_is_valid_name(expr)) {
    return object_get_prop(obj, expr, v);
  } else {
    return RET_FAIL;
  }
}
#endif /*AWTK_LITE*/

const char* object_get_type(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL, NULL);

  return obj->vt->type;
}

const char* object_get_desc(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL, NULL);

  return obj->vt->desc;
}

bool_t object_is_collection(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL, FALSE);

  return obj->vt->is_collection;
}

uint32_t object_get_size(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL, 0);

  return obj->vt->size;
}

const char* object_get_prop_str_by_path(object_t* obj, const char* path) {
  value_t v;
  if (object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_str(&v);
  } else {
    return NULL;
  }
}

void* object_get_prop_pointer_by_path(object_t* obj, const char* path) {
  value_t v;
  if (object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_pointer(&v);
  } else {
    return NULL;
  }
}

object_t* object_get_prop_object_by_path(object_t* obj, const char* path) {
  value_t v;
  if (object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_object(&v);
  } else {
    return NULL;
  }
}

int32_t object_get_prop_int_by_path(object_t* obj, const char* path, int32_t defval) {
  value_t v;
  if (object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_int(&v);
  } else {
    return defval;
  }
}

bool_t object_get_prop_bool_by_path(object_t* obj, const char* path, bool_t defval) {
  value_t v;
  if (object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_bool(&v);
  } else {
    return defval;
  }
}

float_t object_get_prop_float_by_path(object_t* obj, const char* path, float_t defval) {
  value_t v;
  if (object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_float(&v);
  } else {
    return defval;
  }
}
