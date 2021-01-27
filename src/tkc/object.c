/**
 * File:   object.c
 * Author: AWTK Develop Team
 * Brief:  reference count object
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/fscript.h"

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

  emitter_deinit((emitter_t*)obj);
  TKMEM_FREE(obj->name);

  memset(obj, 0x00, obj->vt->size);
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

object_t* object_clone(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->vt->clone != NULL, NULL);

  return obj->vt->clone(obj);
}

static ret_t object_get_prop_by_name(object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && v != NULL, RET_BAD_PARAMS);

  value_set_str(v, NULL);
  if (obj->vt->get_prop != NULL) {
    ret = obj->vt->get_prop(obj, name, v);
  }

  return ret;
}

static ret_t object_get_prop_by_path_with_len(object_t* obj, const char* path, uint32_t len,
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

ret_t object_get_prop_by_path(object_t* obj, const char* path, value_t* v) {
  uint32_t len = 0;
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(path != NULL && v != NULL, RET_BAD_PARAMS);

  len = strlen(path);
  return object_get_prop_by_path_with_len(obj, path, len, v);
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

double object_get_prop_double(object_t* obj, const char* name, double defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_double(&v);
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

ret_t object_set_prop_double(object_t* obj, const char* name, double value) {
  value_t v;
  value_set_double(&v, value);

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
ret_t object_eval(object_t* obj, const char* expr, value_t* v) {
  return fscript_eval(obj, expr, v);
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

bool_t object_has_prop_by_path(object_t* obj, const char* path) {
  value_t v;
  ret_t ret = RET_OK;

  ret = object_get_prop_by_path(obj, path, &v);
  if (ret == RET_OK) {
    value_reset(&v);
  }

  return ret == RET_OK;
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

ret_t object_set_prop_by_path(object_t* obj, const char* path, const value_t* value) {
  const char* name;
  return_value_if_fail(path != NULL, RET_BAD_PARAMS);

  name = path + strlen(path);
  while (path <= --name) {
    if (name[0] == '.') break;
  }
  name++;

  if (name == path) {
    return object_set_prop(obj, name, value);
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
    return object_set_prop(value_object(&v), name, value);
  }
}

ret_t object_set_prop_str_by_path(object_t* obj, const char* name, const char* value) {
  value_t v;
  value_set_str(&v, value);

  return object_set_prop_by_path(obj, name, &v);
}

ret_t object_set_prop_pointer_by_path(object_t* obj, const char* name, void* value) {
  value_t v;
  value_set_pointer(&v, value);

  return object_set_prop_by_path(obj, name, &v);
}

ret_t object_set_prop_object_by_path(object_t* obj, const char* name, object_t* value) {
  value_t v;
  value_set_object(&v, value);

  return object_set_prop_by_path(obj, name, &v);
}

ret_t object_set_prop_int_by_path(object_t* obj, const char* name, int32_t value) {
  value_t v;
  value_set_int(&v, value);

  return object_set_prop_by_path(obj, name, &v);
}

ret_t object_set_prop_bool_by_path(object_t* obj, const char* name, bool_t value) {
  value_t v;
  value_set_bool(&v, value);

  return object_set_prop_by_path(obj, name, &v);
}

ret_t object_set_prop_float_by_path(object_t* obj, const char* name, float_t value) {
  value_t v;
  value_set_float(&v, value);

  return object_set_prop_by_path(obj, name, &v);
}

bool_t object_can_exec_by_path(object_t* obj, const char* path, const char* args) {
  const char* name;
  return_value_if_fail(path != NULL, FALSE);

  name = path + strlen(path);
  while (path <= --name) {
    if (name[0] == '.') break;
  }
  name++;

  if (name == path) {
    return object_can_exec(obj, name, args);
  } else {
    value_t v;
    uint32_t len = name - path - 1;
    return_value_if_fail(obj != NULL && obj->vt != NULL, FALSE);

    if (object_get_prop_by_path_with_len(obj, path, len, &v) != RET_OK) {
      return FALSE;
    }

    return object_can_exec(value_object(&v), name, args);
  }
}

ret_t object_exec_by_path(object_t* obj, const char* path, const char* args) {
  const char* name;
  return_value_if_fail(path != NULL, RET_BAD_PARAMS);

  name = path + strlen(path);
  while (path <= --name) {
    if (name[0] == '.') break;
  }
  name++;

  if (name == path) {
    return object_exec(obj, name, args);
  } else {
    ret_t ret = RET_OK;
    value_t v;
    uint32_t len = name - path - 1;
    return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);

    ret = object_get_prop_by_path_with_len(obj, path, len, &v);
    if (ret != RET_OK) {
      return ret;
    }

    return object_exec(value_object(&v), name, args);
  }
}

ret_t object_set_prop_int8(object_t* obj, const char* name, int8_t value) {
  value_t v;
  value_set_int8(&v, value);

  return object_set_prop(obj, name, &v);
}

int8_t object_get_prop_int8(object_t* obj, const char* name, int8_t defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_int8(&v);
  } else {
    return defval;
  }
}

ret_t object_set_prop_uint8(object_t* obj, const char* name, uint8_t value) {
  value_t v;
  value_set_uint8(&v, value);

  return object_set_prop(obj, name, &v);
}

uint8_t object_get_prop_uint8(object_t* obj, const char* name, uint8_t defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_uint8(&v);
  } else {
    return defval;
  }
}

ret_t object_set_prop_int16(object_t* obj, const char* name, int16_t value) {
  value_t v;
  value_set_int16(&v, value);

  return object_set_prop(obj, name, &v);
}

int16_t object_get_prop_int16(object_t* obj, const char* name, int16_t defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_int16(&v);
  } else {
    return defval;
  }
}

ret_t object_set_prop_uint16(object_t* obj, const char* name, uint16_t value) {
  value_t v;
  value_set_uint16(&v, value);

  return object_set_prop(obj, name, &v);
}

uint16_t object_get_prop_uint16(object_t* obj, const char* name, uint16_t defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_uint16(&v);
  } else {
    return defval;
  }
}

ret_t object_set_prop_int32(object_t* obj, const char* name, int32_t value) {
  value_t v;
  value_set_int32(&v, value);

  return object_set_prop(obj, name, &v);
}

int32_t object_get_prop_int32(object_t* obj, const char* name, int32_t defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_int32(&v);
  } else {
    return defval;
  }
}

ret_t object_set_prop_uint32(object_t* obj, const char* name, uint32_t value) {
  value_t v;
  value_set_uint32(&v, value);

  return object_set_prop(obj, name, &v);
}

uint32_t object_get_prop_uint32(object_t* obj, const char* name, uint32_t defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_uint32(&v);
  } else {
    return defval;
  }
}

ret_t object_set_prop_int64(object_t* obj, const char* name, int64_t value) {
  value_t v;
  value_set_int64(&v, value);

  return object_set_prop(obj, name, &v);
}

int64_t object_get_prop_int64(object_t* obj, const char* name, int64_t defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_int64(&v);
  } else {
    return defval;
  }
}

ret_t object_set_prop_uint64(object_t* obj, const char* name, uint64_t value) {
  value_t v;
  value_set_uint64(&v, value);

  return object_set_prop(obj, name, &v);
}

uint64_t object_get_prop_uint64(object_t* obj, const char* name, uint64_t defval) {
  value_t v;
  if (object_get_prop(obj, name, &v) == RET_OK) {
    return value_uint64(&v);
  } else {
    return defval;
  }
}
