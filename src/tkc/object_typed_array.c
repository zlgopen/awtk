/**
 * File:   object_typed_array.c
 * Author: AWTK Develop Team
 * Brief:  wrap typed array to an object.
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/value.h"
#include "tkc/utils.h"
#include "object_typed_array.h"

static ret_t object_typed_array_on_destroy(tk_object_t* obj) {
  object_typed_array_t* o = OBJECT_TYPED_ARRAY(obj);
  return_value_if_fail(o != NULL && o->arr != NULL, RET_BAD_PARAMS);

  typed_array_destroy(o->arr);
  o->arr = NULL;

  return RET_OK;
}

static ret_t object_typed_array_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  object_typed_array_t* o = OBJECT_TYPED_ARRAY(obj);
  return_value_if_fail(o != NULL && o->arr != NULL, RET_BAD_PARAMS);

  if (*name == '[') {
    int32_t index = tk_atoi(name + 1);
    return typed_array_set(o->arr, index, v);
  }

  return RET_NOT_FOUND;
}

static ret_t object_typed_array_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_typed_array_t* o = OBJECT_TYPED_ARRAY(obj);
  return_value_if_fail(o != NULL && o->arr != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, "size") || tk_str_eq(name, TK_OBJECT_PROP_SIZE)) {
    value_set_uint32(v, o->arr->size);
    ret = RET_OK;
  } else if (tk_str_eq(name, "capacity")) {
    value_set_uint32(v, o->arr->capacity);
    ret = RET_OK;
  } else if (tk_str_eq(name, "bytes") || tk_str_eq(name, "size_in_bytes")) {
    value_set_uint32(v, o->arr->size * o->arr->element_size);
    ret = RET_OK;
  } else if (tk_str_eq(name, "capacity_in_bytes")) {
    value_set_uint32(v, o->arr->capacity * o->arr->element_size);
    ret = RET_OK;
  } else if (tk_str_eq(name, "data")) {
    value_set_pointer(v, o->arr->data);
    ret = RET_OK;
  } else if (*name == '[') {
    int32_t index = tk_atoi(name + 1);
    ret = typed_array_get(o->arr, index, v);
  }

  return ret;
}

static const object_vtable_t s_object_typed_array_vtable = {
    .type = "object_typed_array",
    .desc = "object_typed_array",
    .size = sizeof(object_typed_array_t),
    .is_collection = FALSE,
    .on_destroy = object_typed_array_on_destroy,
    .get_prop = object_typed_array_get_prop,
    .set_prop = object_typed_array_set_prop};

tk_object_t* object_typed_array_create(value_type_t type, uint32_t capacity) {
  tk_object_t* o = NULL;
  object_typed_array_t* wrapper = NULL;
  o = tk_object_create(&s_object_typed_array_vtable);
  return_value_if_fail(o != NULL, NULL);

  wrapper = OBJECT_TYPED_ARRAY(o);
  return_value_if_fail(wrapper != NULL, NULL);

  wrapper->arr = typed_array_create(type, capacity);
  if (wrapper->arr == NULL) {
    TK_OBJECT_UNREF(o);
  }

  return o;
}

object_typed_array_t* object_typed_array_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_typed_array_vtable, NULL);

  return (object_typed_array_t*)(obj);
}
