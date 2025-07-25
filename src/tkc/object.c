﻿/**
 * File:   object.c
 * Author: AWTK Develop Team
 * Brief:  reference count object
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#ifndef WITHOUT_FSCRIPT
#include "tkc/fscript.h"
#endif /*WITHOUT_FSCRIPT*/

#include "tkc/named_value.h"

ret_t tk_object_set_name(tk_object_t* obj, const char* name) {
  ret_t ret = RET_OK;
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);

  obj->name = tk_str_copy(obj->name, name);

  return ret;
}

static ret_t object_destroy(tk_object_t* obj) {
  ret_t ret = RET_OK;
  event_t e = event_init(EVT_DESTROY, obj);
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);

  emitter_dispatch((emitter_t*)obj, (event_t*)(&e));

  if (obj->vt->on_destroy != NULL) {
    ret = obj->vt->on_destroy(obj);
  }

  emitter_deinit((emitter_t*)obj);
  TKMEM_FREE(obj->name);

  memset(obj, 0x00, obj->vt->size);
  TKMEM_FREE(obj);

  return ret;
}

tk_object_t* tk_object_create_ex(const object_vtable_t* vt, uint32_t extra_data_size) {
  uint32_t size = 0;
  tk_object_t* obj = NULL;
  return_value_if_fail(vt != NULL && vt->size >= sizeof(tk_object_t), NULL);

  size = vt->size + extra_data_size;
  obj = (tk_object_t*)TKMEM_ALLOC(size);
  return_value_if_fail(obj != NULL, NULL);

  memset(obj, 0x00, size);

  obj->vt = vt;
  obj->ref_count = 1;
  emitter_init((emitter_t*)obj);

  return obj;
}

tk_object_t* tk_object_create(const object_vtable_t* vt) {
  tk_object_t* obj = NULL;
  return_value_if_fail(vt != NULL && vt->size >= sizeof(tk_object_t), NULL);

  obj = (tk_object_t*)TKMEM_ALLOC(vt->size);
  return_value_if_fail(obj != NULL, NULL);

  memset(obj, 0x00, vt->size);

  obj->vt = vt;
  obj->ref_count = 1;
  emitter_init((emitter_t*)obj);

  return obj;
}

ret_t tk_object_unref(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count > 0, RET_BAD_PARAMS);
  return_value_if_fail(!(obj->visiting), RET_BUSY);

  if (obj->ref_count == 1) {
    object_destroy(obj);
  } else {
    obj->ref_count--;
  }

  return RET_OK;
}

tk_object_t* tk_object_ref(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, NULL);

  obj->ref_count++;

  return obj;
}

tk_object_t* tk_object_clone(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->vt->clone != NULL, NULL);

  return obj->vt->clone(obj);
}

#ifndef WITHOUT_FSCRIPT
static ret_t object_fscript_exec_func(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  ret_t ret = RET_OK;
  const char* cmd = NULL;
  const char* param = NULL;
  tk_object_t* obj = TK_OBJECT(fscript->curr->ctx);
  FSCRIPT_FUNC_CHECK(obj != NULL, RET_BAD_PARAMS);
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  cmd = value_str(args->args);
  param = value_str(args->args + 1);

  ret = tk_object_exec(obj, cmd, param);
  value_set_int(v, ret);

  return RET_OK;
}

static ret_t object_fscript_exec_ex_func(fscript_t* fscript, fscript_args_t* args, value_t* v) {
  const char* cmd = NULL;
  const char* param = NULL;
  tk_object_t* obj = TK_OBJECT(fscript->curr->ctx);
  FSCRIPT_FUNC_CHECK(obj != NULL, RET_BAD_PARAMS);
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  cmd = value_str(args->args);
  param = value_str(args->args + 1);
  value_set_int(v, 0);

  return tk_object_exec_ex(obj, cmd, param, v);
}
#endif/*WITHOUT_FSCRIPT*/

static ret_t object_get_prop_by_name(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && v != NULL, RET_BAD_PARAMS);

  value_set_str(v, NULL);
  if (obj->vt->get_prop != NULL) {
    ret = obj->vt->get_prop(obj, name, v);
  }
  
#ifndef WITHOUT_FSCRIPT
  if (ret == RET_NOT_FOUND) {
    if (tk_str_eq(name, TK_OBJECT_EXEC)) {
      value_set_func(v, (void*)object_fscript_exec_func);
      return RET_OK;
    } else if (tk_str_eq(name, TK_OBJECT_EXEC_EX)) {
      value_set_func(v, (void*)object_fscript_exec_ex_func);
      return RET_OK;
    }
  }
#endif/*WITHOUT_FSCRIPT*/
  return ret;
}

static ret_t object_get_prop_by_path_with_len(tk_object_t* obj, const char* path, uint32_t len,
                                              value_t* v) {
  char* p = NULL;
  char temp[MAX_PATH + 1];
  const char* name = temp;
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(len <= MAX_PATH, RET_BAD_PARAMS);

  memcpy(temp, path, len);
  temp[len] = '\0';

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
      ret = RET_BAD_PARAMS;
      break;
    }

    name = p + 1;
  } while (p != NULL);

  return ret;
}

ret_t tk_object_get_prop_by_path(tk_object_t* obj, const char* path, value_t* v) {
  uint32_t len = 0;
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(path != NULL && v != NULL, RET_BAD_PARAMS);

  len = strlen(path);
  return object_get_prop_by_path_with_len(obj, path, len, v);
}

ret_t tk_object_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  return object_get_prop_by_name(obj, name, v);
}

const char* tk_object_get_prop_str(tk_object_t* obj, const char* name) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_str(&v);
  } else {
    return NULL;
  }
}

void* tk_object_get_prop_pointer(tk_object_t* obj, const char* name) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_pointer(&v);
  } else {
    return NULL;
  }
}

tk_object_t* tk_object_get_prop_object(tk_object_t* obj, const char* name) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_object(&v);
  } else {
    return NULL;
  }
}

int32_t tk_object_get_prop_int(tk_object_t* obj, const char* name, int32_t defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_int(&v);
  } else {
    return defval;
  }
}

bool_t tk_object_get_prop_bool(tk_object_t* obj, const char* name, bool_t defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_bool(&v);
  } else {
    return defval;
  }
}

float_t tk_object_get_prop_float(tk_object_t* obj, const char* name, float_t defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_float(&v);
  } else {
    return defval;
  }
}

double tk_object_get_prop_double(tk_object_t* obj, const char* name, double defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_double(&v);
  } else {
    return defval;
  }
}

ret_t tk_object_notify_changed(tk_object_t* obj) {
  event_t e = event_init(EVT_PROPS_CHANGED, obj);

  return emitter_dispatch((emitter_t*)obj, &e);
}

ret_t tk_object_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
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

ret_t tk_object_set_prop_str(tk_object_t* obj, const char* name, const char* value) {
  value_t v;
  value_set_str(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

ret_t tk_object_set_prop_pointer(tk_object_t* obj, const char* name, void* value) {
  return tk_object_set_prop_pointer_ex(obj, name, value, NULL);
}

ret_t tk_object_set_prop_pointer_ex(tk_object_t* obj, const char* name, void* value,
                                    tk_destroy_t destroy) {
  value_t v;
  ret_t ret = RET_OK;
  value_set_int(&v, 0);
  ret = tk_object_set_prop(obj, name, value_set_pointer_ex(&v, value, destroy));
  value_reset(&v);

  return ret;
}

ret_t tk_object_set_prop_object(tk_object_t* obj, const char* name, tk_object_t* value) {
  value_t v;

  return tk_object_set_prop(obj, name, value_set_object(&v, value));
}

ret_t tk_object_set_prop_int(tk_object_t* obj, const char* name, int32_t value) {
  value_t v;
  value_set_int(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

ret_t tk_object_set_prop_bool(tk_object_t* obj, const char* name, bool_t value) {
  value_t v;
  value_set_bool(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

ret_t tk_object_set_prop_float(tk_object_t* obj, const char* name, float_t value) {
  value_t v;
  value_set_float(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

ret_t tk_object_set_prop_double(tk_object_t* obj, const char* name, double value) {
  value_t v;
  value_set_double(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

ret_t tk_object_remove_prop(tk_object_t* obj, const char* name) {
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);
  return_value_if_fail(!(obj->visiting), RET_BUSY);

  if (obj->vt->remove_prop != NULL) {
    ret = obj->vt->remove_prop(obj, name);
  }

  if (RET_OK == ret) {
    tk_object_notify_changed(obj);
  }

  return ret;
}

ret_t tk_object_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
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

int32_t tk_object_compare(tk_object_t* obj, tk_object_t* other) {
  int32_t ret = -1;
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, -1);
  return_value_if_fail(other != NULL && other->vt != NULL && other->ref_count >= 0, -1);

  ret = pointer_compare(obj, other);

  if (ret != 0) {
    if (obj->vt->compare != NULL) {
      ret = obj->vt->compare(obj, other);
    }
  }

  return ret;
}

int32_t tk_object_compare_name_without_nullptr(tk_object_t* obj, tk_object_t* other) {
  return_value_if_fail(obj != NULL && other != NULL, -1);
  if (obj->name == NULL && other->name == NULL) {
    return pointer_compare(obj, other);
  } else {
    return tk_str_cmp(obj->name, other->name);
  }
}

bool_t tk_object_can_exec(tk_object_t* obj, const char* name, const char* args) {
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

ret_t tk_object_exec(tk_object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_OK;
  value_t result;
  value_set_int(&result, 0);
  ret = tk_object_exec_ex(obj, name, args, &result);
  value_reset(&result);
  return ret;
}

ret_t tk_object_exec_ex(tk_object_t* obj, const char* name, const char* args, value_t* result) {
  cmd_exec_event_t e;
  event_t* evt = NULL;
  bool_t not_found = TRUE;
  ret_t ret = RET_NOT_IMPL;
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);

  if (emitter_dispatch(EMITTER(obj), cmd_exec_event_init(&e, EVT_CMD_WILL_EXEC, name, args)) !=
      RET_OK) {
    return RET_FAIL;
  }

  if (obj->vt->exec_ex != NULL) {
    ret = obj->vt->exec_ex(obj, name, args, result);
    not_found = (RET_NOT_FOUND == ret || RET_NOT_IMPL == ret);
  }

  if (not_found) {
    if (obj->vt->exec != NULL) {
      ret = obj->vt->exec(obj, name, args);
    }
    value_set_int(result, ret);
  }

  evt = cmd_exec_event_init(&e, EVT_CMD_EXECED, name, args);
  e.result = ret;
  emitter_dispatch(EMITTER(obj), evt);

  return e.result;
}

bool_t tk_object_has_prop(tk_object_t* obj, const char* name) {
  value_t v;
  ret_t ret = RET_OK;

  ret = tk_object_get_prop(obj, name, &v);
  if (ret == RET_OK) {
    value_reset(&v);
  }

  return ret == RET_OK;
}

ret_t tk_object_copy_prop(tk_object_t* obj, tk_object_t* src, const char* name) {
  value_t v;
  ret_t ret = RET_FAIL;
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);
  return_value_if_fail(src != NULL && src->vt != NULL && src->ref_count >= 0, RET_BAD_PARAMS);

  if (tk_object_get_prop(src, name, &v) == RET_OK) {
    ret = tk_object_set_prop(obj, name, &v);
    value_reset(&v);
  }

  return ret;
}

typedef struct _copy_ctx_t {
  bool_t overwrite;
  tk_object_t* dst;
} copy_ctx_t;

static ret_t on_copy_on_prop(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  copy_ctx_t* info = (copy_ctx_t*)ctx;

  if (info->overwrite) {
    tk_object_set_prop(info->dst, nv->name, &(nv->value));
  } else {
    value_t v;
    if (tk_object_get_prop(info->dst, nv->name, &v) != RET_OK) {
      tk_object_set_prop(info->dst, nv->name, &(nv->value));
    }
  }
  return RET_OK;
}

static inline ret_t tk_object_copy_props_default(tk_object_t* obj, tk_object_t* src,
                                                 bool_t overwrite) {
  copy_ctx_t ctx = {overwrite, obj};
  return tk_object_foreach_prop(src, on_copy_on_prop, &ctx);
}

ret_t tk_object_copy_props(tk_object_t* obj, tk_object_t* src, bool_t overwrite) {
  ret_t ret = RET_NOT_IMPL;
  return_value_if_fail(obj != NULL && src != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj->vt != NULL, RET_BAD_PARAMS);

  if (obj->vt->copy_props != NULL) {
    ret = obj->vt->copy_props(obj, src, overwrite);
  }

  if (RET_NOT_IMPL == ret) {
    ret = tk_object_copy_props_default(obj, src, overwrite);
  }

  return ret;
}

#ifndef WITHOUT_FSCRIPT
ret_t tk_object_eval(tk_object_t* obj, const char* expr, value_t* v) {
  return fscript_eval(obj, expr, v);
}
#else
ret_t tk_object_eval(tk_object_t* obj, const char* expr, value_t* v) {
  return_value_if_fail(expr != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);

  if (tk_is_valid_name(expr)) {
    return tk_object_get_prop(obj, expr, v);
  } else {
    return RET_FAIL;
  }
}
#endif /*WITHOUT_FSCRIPT*/

const char* tk_object_get_type(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL, NULL);

  return obj->vt->type;
}

const char* tk_object_get_desc(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL, NULL);

  return obj->vt->desc;
}

bool_t tk_object_is_collection(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL, FALSE);

  return obj->vt->is_collection;
}

uint32_t tk_object_get_size(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL, 0);

  return obj->vt->size;
}

bool_t tk_object_has_prop_by_path(tk_object_t* obj, const char* path) {
  value_t v;
  ret_t ret = RET_OK;

  ret = tk_object_get_prop_by_path(obj, path, &v);
  if (ret == RET_OK) {
    value_reset(&v);
  }

  return ret == RET_OK;
}

const char* tk_object_get_prop_str_by_path(tk_object_t* obj, const char* path) {
  value_t v;
  if (tk_object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_str(&v);
  } else {
    return NULL;
  }
}

void* tk_object_get_prop_pointer_by_path(tk_object_t* obj, const char* path) {
  value_t v;
  if (tk_object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_pointer(&v);
  } else {
    return NULL;
  }
}

tk_object_t* tk_object_get_prop_object_by_path(tk_object_t* obj, const char* path) {
  value_t v;
  if (tk_object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_object(&v);
  } else {
    return NULL;
  }
}

int32_t tk_object_get_prop_int_by_path(tk_object_t* obj, const char* path, int32_t defval) {
  value_t v;
  if (tk_object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_int(&v);
  } else {
    return defval;
  }
}

bool_t tk_object_get_prop_bool_by_path(tk_object_t* obj, const char* path, bool_t defval) {
  value_t v;
  if (tk_object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_bool(&v);
  } else {
    return defval;
  }
}

float_t tk_object_get_prop_float_by_path(tk_object_t* obj, const char* path, float_t defval) {
  value_t v;
  if (tk_object_get_prop_by_path(obj, path, &v) == RET_OK) {
    return value_float(&v);
  } else {
    return defval;
  }
}

ret_t tk_object_set_prop_by_path(tk_object_t* obj, const char* path, const value_t* value) {
  const char* name;
  return_value_if_fail(path != NULL, RET_BAD_PARAMS);

  name = path + strlen(path);
  while (path <= --name) {
    if (name[0] == '.') break;
  }
  name++;

  if (name == path) {
    return tk_object_set_prop(obj, name, value);
  } else {
    ret_t ret = RET_OK;
    value_t v;
    uint32_t len = name - path - 1;
    return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);
    return_value_if_fail(value != NULL, RET_BAD_PARAMS);

    ret = object_get_prop_by_path_with_len(obj, path, len, &v);
    if (ret != RET_OK) {
      return ret;
    }
    return tk_object_set_prop(value_object(&v), name, value);
  }
}

ret_t tk_object_set_prop_str_by_path(tk_object_t* obj, const char* name, const char* value) {
  value_t v;
  value_set_str(&v, value);

  return tk_object_set_prop_by_path(obj, name, &v);
}

ret_t tk_object_set_prop_pointer_by_path(tk_object_t* obj, const char* name, void* value) {
  value_t v;
  value_set_pointer(&v, value);

  return tk_object_set_prop_by_path(obj, name, &v);
}

ret_t tk_object_set_prop_object_by_path(tk_object_t* obj, const char* name, tk_object_t* value) {
  value_t v;
  value_set_object(&v, value);

  return tk_object_set_prop_by_path(obj, name, &v);
}

ret_t tk_object_set_prop_int_by_path(tk_object_t* obj, const char* name, int32_t value) {
  value_t v;
  value_set_int(&v, value);

  return tk_object_set_prop_by_path(obj, name, &v);
}

ret_t tk_object_set_prop_bool_by_path(tk_object_t* obj, const char* name, bool_t value) {
  value_t v;
  value_set_bool(&v, value);

  return tk_object_set_prop_by_path(obj, name, &v);
}

ret_t tk_object_set_prop_float_by_path(tk_object_t* obj, const char* name, float_t value) {
  value_t v;
  value_set_float(&v, value);

  return tk_object_set_prop_by_path(obj, name, &v);
}

bool_t tk_object_can_exec_by_path(tk_object_t* obj, const char* path, const char* args) {
  const char* name;
  return_value_if_fail(path != NULL, FALSE);

  name = path + strlen(path);
  while (path <= --name) {
    if (name[0] == '.') break;
  }
  name++;

  if (name == path) {
    return tk_object_can_exec(obj, name, args);
  } else {
    value_t v;
    uint32_t len = name - path - 1;
    return_value_if_fail(obj != NULL && obj->vt != NULL, FALSE);

    if (object_get_prop_by_path_with_len(obj, path, len, &v) != RET_OK) {
      return FALSE;
    }

    return tk_object_can_exec(value_object(&v), name, args);
  }
}

ret_t tk_object_exec_by_path(tk_object_t* obj, const char* path, const char* args) {
  const char* name;
  return_value_if_fail(path != NULL, RET_BAD_PARAMS);

  name = path + strlen(path);
  while (path <= --name) {
    if (name[0] == '.') break;
  }
  name++;

  if (name == path) {
    return tk_object_exec(obj, name, args);
  } else {
    ret_t ret = RET_OK;
    value_t v;
    uint32_t len = name - path - 1;
    return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);

    ret = object_get_prop_by_path_with_len(obj, path, len, &v);
    if (ret != RET_OK) {
      return ret;
    }

    return tk_object_exec(value_object(&v), name, args);
  }
}

ret_t tk_object_set_prop_int8(tk_object_t* obj, const char* name, int8_t value) {
  value_t v;
  value_set_int8(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

int8_t tk_object_get_prop_int8(tk_object_t* obj, const char* name, int8_t defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_int8(&v);
  } else {
    return defval;
  }
}

ret_t tk_object_set_prop_uint8(tk_object_t* obj, const char* name, uint8_t value) {
  value_t v;
  value_set_uint8(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

uint8_t tk_object_get_prop_uint8(tk_object_t* obj, const char* name, uint8_t defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_uint8(&v);
  } else {
    return defval;
  }
}

ret_t tk_object_set_prop_int16(tk_object_t* obj, const char* name, int16_t value) {
  value_t v;
  value_set_int16(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

int16_t tk_object_get_prop_int16(tk_object_t* obj, const char* name, int16_t defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_int16(&v);
  } else {
    return defval;
  }
}

ret_t tk_object_set_prop_uint16(tk_object_t* obj, const char* name, uint16_t value) {
  value_t v;
  value_set_uint16(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

uint16_t tk_object_get_prop_uint16(tk_object_t* obj, const char* name, uint16_t defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_uint16(&v);
  } else {
    return defval;
  }
}

ret_t tk_object_set_prop_int32(tk_object_t* obj, const char* name, int32_t value) {
  value_t v;
  value_set_int32(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

int32_t tk_object_get_prop_int32(tk_object_t* obj, const char* name, int32_t defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_int32(&v);
  } else {
    return defval;
  }
}

ret_t tk_object_set_prop_uint32(tk_object_t* obj, const char* name, uint32_t value) {
  value_t v;
  value_set_uint32(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

uint32_t tk_object_get_prop_uint32(tk_object_t* obj, const char* name, uint32_t defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_uint32(&v);
  } else {
    return defval;
  }
}

ret_t tk_object_set_prop_int64(tk_object_t* obj, const char* name, int64_t value) {
  value_t v;
  value_set_int64(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

int64_t tk_object_get_prop_int64(tk_object_t* obj, const char* name, int64_t defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_int64(&v);
  } else {
    return defval;
  }
}

ret_t tk_object_set_prop_uint64(tk_object_t* obj, const char* name, uint64_t value) {
  value_t v;
  value_set_uint64(&v, value);

  return tk_object_set_prop(obj, name, &v);
}

uint64_t tk_object_get_prop_uint64(tk_object_t* obj, const char* name, uint64_t defval) {
  value_t v;
  if (tk_object_get_prop(obj, name, &v) == RET_OK) {
    return value_uint64(&v);
  } else {
    return defval;
  }
}

ret_t tk_object_clear_props(tk_object_t* obj) {
  ret_t ret = RET_NOT_IMPL;
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);

  if (obj->vt->clear_props != NULL) {
    ret = obj->vt->clear_props(obj);
  }

  if (RET_OK == ret) {
    tk_object_notify_changed(obj);
  }

  return ret;
}

typedef struct _find_prop_default_ctx_t {
  tk_compare_t cmp;
  const void* data;
  value_t* value;
} find_prop_default_ctx_t;

static ret_t tk_object_find_prop_default_on_visit(void* ctx, const void* data) {
  const named_value_t* nv = (const named_value_t*)data;
  find_prop_default_ctx_t* actx = (find_prop_default_ctx_t*)(ctx);
  if (0 == actx->cmp(data, actx->data)) {
    actx->value = (value_t*)&nv->value;
    return RET_FOUND;
  }
  return RET_OK;
}

static value_t* tk_object_find_prop_default(tk_object_t* obj, tk_compare_t cmp, const void* data) {
  find_prop_default_ctx_t actx;
  actx.cmp = cmp;
  actx.data = data;
  actx.value = NULL;
  if (RET_FOUND == tk_object_foreach_prop(obj, tk_object_find_prop_default_on_visit, &actx)) {
    return actx.value;
  }
  return NULL;
}

value_t* tk_object_find_prop(tk_object_t* obj, tk_compare_t cmp, const void* data) {
  value_t* ret = NULL;
  return_value_if_fail(obj != NULL && obj->vt != NULL, NULL);
  return_value_if_fail(cmp != NULL, NULL);

  if (obj->vt->find_prop != NULL) {
    ret = obj->vt->find_prop(obj, cmp, data);
  } else {
    ret = tk_object_find_prop_default(obj, cmp, data);
  }

  return ret;
}

typedef struct _find_props_default_ctx_t {
  tk_compare_t cmp;
  const void* data;
  darray_t* matched;
} find_props_default_ctx_t;

static ret_t tk_object_find_props_default_on_visit(void* ctx, const void* data) {
  const named_value_t* nv = (const named_value_t*)data;
  find_props_default_ctx_t* actx = (find_props_default_ctx_t*)(ctx);
  if (0 == actx->cmp(data, actx->data)) {
    return darray_push(actx->matched, (value_t*)&nv->value);
  }
  return RET_OK;
}

static ret_t tk_object_find_props_default(tk_object_t* obj, tk_compare_t cmp, const void* data,
                                          darray_t* matched) {
  find_props_default_ctx_t actx;
  actx.cmp = cmp;
  actx.data = data;
  actx.matched = matched;
  return tk_object_foreach_prop(obj, tk_object_find_props_default_on_visit, &actx);
}

ret_t tk_object_find_props(tk_object_t* obj, tk_compare_t cmp, const void* data,
                           darray_t* matched) {
  ret_t ret = RET_OK;
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(cmp != NULL && matched != NULL, RET_BAD_PARAMS);

  if (obj->vt->find_props != NULL) {
    ret = obj->vt->find_props(obj, cmp, data, matched);
  } else {
    ret = tk_object_find_props_default(obj, cmp, data, matched);
  }

  return ret;
}

tk_object_t* tk_object_get_child_object(tk_object_t* obj, const char* path,
                                        const char** next_path) {
  return_value_if_fail(obj != NULL && path != NULL && next_path != NULL, NULL);

  const char* p = strchr(path, '.');
  if (p != NULL) {
    value_t v;
    uint32_t len = p - path;
    char subname[TK_OBJECT_PROP_NAME_MAX_LEN] = {0};
    uint32_t size = tk_min(TK_OBJECT_PROP_NAME_MAX_LEN - 1, len);
    return_value_if_fail(len < TK_OBJECT_PROP_NAME_MAX_LEN - 1, NULL);

    tk_memcpy(subname, path, size);
    subname[size] = '\0';
    if (tk_object_get_prop(obj, subname, &v) == RET_OK) {
      if (v.type == VALUE_TYPE_OBJECT) {
        *next_path = p + 1;
        return value_object(&v);
      }
    }
  }

  return NULL;
}

typedef struct _json_info_t {
  str_t* json;
  uint32_t indent;
  uint32_t level;
  uint32_t index;
  bool_t oneline : 1;
  bool_t is_array : 1;
} json_info_t;

static ret_t to_json(void* ctx, const void* data) {
  char buff[64] = {0};
  json_info_t* info = (json_info_t*)ctx;
  named_value_t* nv = (named_value_t*)data;
  str_t* s = info->json;
  value_t* v = &(nv->value);
  bool_t oneline = info->oneline;

  if (info->index > 0) {
    str_append_char(s, ',');
  }

  if (!oneline) {
    str_append_char(s, '\n');
  }

  info->index++;

  if (!oneline) {
    str_append_n_chars(s, ' ', info->indent * info->level);
  }

  if (!info->is_array) {
    str_append_json_str(s, nv->name);
    str_append(s, ": ");
  }

  switch (v->type) {
    case VALUE_TYPE_STRING: {
      str_append_json_str(s, value_str(v));
      break;
    }
    case VALUE_TYPE_OBJECT: {
      tk_object_to_json(value_object(v), s, info->indent + 1, info->level, oneline);
      break;
    }
    case VALUE_TYPE_INT8:
    case VALUE_TYPE_BOOL:
    case VALUE_TYPE_INT16:
    case VALUE_TYPE_INT32:
    case VALUE_TYPE_INT64:
    case VALUE_TYPE_UINT8:
    case VALUE_TYPE_UINT16:
    case VALUE_TYPE_UINT32:
    case VALUE_TYPE_UINT64:
    case VALUE_TYPE_FLOAT:
    case VALUE_TYPE_FLOAT32:
    case VALUE_TYPE_DOUBLE: {
      str_append(s, value_str_ex(v, buff, sizeof(buff)));
      break;
    }
    default: {
      str_append_json_str(s, value_str_ex(v, buff, sizeof(buff)));
      break;
    }
  }

  return RET_OK;
}

ret_t tk_object_to_json(tk_object_t* obj, str_t* json, uint32_t indent, uint32_t level,
                        bool_t oneline) {
  bool_t is_array = FALSE;
  json_info_t info = {
      .json = json,
      .indent = indent,
      .level = level,
      .oneline = oneline,
      .index = 0,
  };
  return_value_if_fail(obj != NULL && json != NULL, RET_BAD_PARAMS);

  if (!oneline) {
    str_append_n_chars(json, ' ', indent * level);
  }

  is_array = tk_object_is_collection(obj);
  info.is_array = is_array;
  str_append_char(json, is_array ? '[' : '{');
  info.level++;
  tk_object_foreach_prop(obj, to_json, &info);

  if (!oneline) {
    str_append_char(json, '\n');
    str_append_n_chars(json, ' ', indent * level);
  }
  str_append_char(json, is_array ? ']' : '}');

  return RET_OK;
}

ret_t tk_object_set_prop_str_with_format(tk_object_t* obj, const char* name, const char* format,
                                         ...) {
  char str[256] = {0};
  return_value_if_fail(obj != NULL && name != NULL && format != NULL, RET_BAD_PARAMS);

  va_list args;
  va_start(args, format);
  tk_vsnprintf(str, sizeof(str) - 1, format, args);
  va_end(args);

  return tk_object_set_prop_str(obj, name, str);
}

bool_t tk_object_is_instance_of(tk_object_t* obj, const char* type) {
  return_value_if_fail(obj != NULL && obj->vt != NULL, FALSE);

  return tk_str_eq(obj->vt->type, type);
}
