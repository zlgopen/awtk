/**
 * File:   object_default.c
 * Author: AWTK Develop Team
 * Brief:  default object
 *
 * Copyright (c) 2019 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/value.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"

static tk_compare_t object_default_get_cmp(object_default_t* o) {
  if (o->name_case_insensitive) {
    return (tk_compare_t)named_value_icompare;
  } else {
    return (tk_compare_t)named_value_compare;
  }
}

static tk_compare_t object_default_get_cmp_by_name(object_default_t* o) {
  if (o->name_case_insensitive) {
    return (tk_compare_t)named_value_icompare_by_name;
  } else {
    return (tk_compare_t)named_value_compare_by_name;
  }
}

static int32_t object_default_find_prop_index_by_name(tk_object_t* obj, const char* name) {
  int32_t ret = -1;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL && name != NULL, -1);

  if (*name == '[' && tk_isdigit(name[1]) && tk_str_end_with(name, "]")) {
    ret = tk_atoi(name + 1);
    return_value_if_fail(ret < o->props.size, -1);
  } else {
    ret = darray_bsearch_index(&(o->props), object_default_get_cmp_by_name(o), (void*)name);
  }

  return ret;
}

static value_t* object_default_find_prop_by_name(tk_object_t* obj, const char* name) {
  named_value_t* nv = NULL;
  object_default_t* o = OBJECT_DEFAULT(obj);
  int32_t index = -1;
  return_value_if_fail(o != NULL && name != NULL, NULL);

  index = object_default_find_prop_index_by_name(obj, name);
  if (index >= 0) {
    nv = (named_value_t*)darray_get(&o->props, index);
  }

  return nv != NULL ? &(nv->value) : NULL;
}

ret_t object_default_clear_props(tk_object_t* obj) {
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return darray_clear(&(o->props));
}

static ret_t object_default_on_destroy(tk_object_t* obj) {
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return darray_deinit(&(o->props));
}

static int32_t object_default_compare(tk_object_t* obj, tk_object_t* other) {
  return_value_if_fail(obj != NULL && other != NULL, -1);
  return tk_str_cmp(obj->name, other->name);
}

static ret_t object_default_remove_prop(tk_object_t* obj, const char* name) {
  int32_t index = 0;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->props.size > 0 && o->enable_path) {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_remove_prop(sub, name);
    }
  }

  index = object_default_find_prop_index_by_name(obj, name);
  if (index >= 0) {
    return darray_remove_index(&(o->props), index);
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t value_deep_copy_keep_type(value_t* dst, const value_t* src) {
  ret_t ret = RET_OK;
  return_value_if_fail(dst != NULL && src != NULL, RET_BAD_PARAMS);

  switch (dst->type) {
    case VALUE_TYPE_BOOL: {
      value_set_bool(dst, value_bool(src));
      break;
    }
    case VALUE_TYPE_INT8: {
      value_set_int8(dst, value_int8(src));
      break;
    }
    case VALUE_TYPE_UINT8: {
      value_set_uint8(dst, value_uint8(src));
      break;
    }
    case VALUE_TYPE_INT16: {
      value_set_int16(dst, value_int16(src));
      break;
    }
    case VALUE_TYPE_UINT16: {
      value_set_uint16(dst, value_uint16(src));
      break;
    }
    case VALUE_TYPE_INT32: {
      value_set_int32(dst, value_int32(src));
      break;
    }
    case VALUE_TYPE_UINT32: {
      value_set_uint32(dst, value_uint32(src));
      break;
    }
    case VALUE_TYPE_INT64: {
      value_set_int64(dst, value_int64(src));
      break;
    }
    case VALUE_TYPE_UINT64: {
      value_set_uint64(dst, value_uint64(src));
      break;
    }
    case VALUE_TYPE_FLOAT: {
      value_set_float(dst, value_float(src));
      break;
    }
    case VALUE_TYPE_DOUBLE: {
      value_set_double(dst, value_double(src));
      break;
    }
    case VALUE_TYPE_STRING: {
      value_reset(dst);
      value_dup_str(dst, value_str(src));
      break;
    }
    default: {
      if (dst->type == src->type) {
        value_reset(dst);
        value_deep_copy(dst, src);
      } else {
        ret = RET_FAIL;
        log_debug("not support type:%d\n", dst->type);
      }
      break;
    }
  }

  return ret;
}

static ret_t object_default_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  value_t* vv = NULL;
  ret_t ret = RET_NOT_FOUND;
  object_default_t* o = OBJECT_DEFAULT(obj);
  ENSURE(o);

  if (o->props.size > 0 && o->enable_path) {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_set_prop(sub, name, v);
    }
  }

  vv = object_default_find_prop_by_name(obj, name);
  if (vv != NULL) {
    if (o->keep_prop_type) {
      ret = value_deep_copy_keep_type(vv, v);
    } else {
      value_reset(vv);
      ret = value_deep_copy(vv, v);
    }
  } else {
    named_value_t* nv = named_value_create_ex(name, v);
    return_value_if_fail(nv != NULL, RET_OOM);
    ret = darray_sorted_insert(&(o->props), nv, object_default_get_cmp(o), FALSE);
    if (ret != RET_OK) {
      named_value_destroy(nv);
    }
  }

  return ret;
}

static ret_t object_default_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (tk_str_eq(name, TK_OBJECT_PROP_SIZE)) {
    value_set_uint32(v, o->props.size);
    return RET_OK;
  }

  if (o->props.size > 0 && o->enable_path) {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_get_prop(sub, name, v);
    }
  }

  if (o->props.size > 0) {
    const value_t* vv = object_default_find_prop_by_name(obj, name);
    if (vv != NULL) {
      ret = value_copy(v, vv);
    } else {
      ret = RET_NOT_FOUND;
    }
  }

  return ret;
}

static bool_t object_default_can_exec(tk_object_t* obj, const char* name, const char* args) {
  bool_t ret = FALSE;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->enable_path) {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_can_exec(sub, name, args);
    }
  }

  return ret;
}

static ret_t object_default_exec(tk_object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_NOT_FOUND;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->enable_path) {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_exec(sub, name, args);
    }
  }

  return ret;
}

static ret_t object_default_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_OK;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL && on_prop != NULL, RET_BAD_PARAMS);
  if (o->props.size > 0) {
    uint32_t i = 0;
    for (i = 0; i < o->props.size; i++) {
      named_value_t* iter = (named_value_t*)(o->props.elms[i]);
      ret = on_prop(ctx, iter);
      if (ret == RET_REMOVE) {
        named_value_destroy(iter);
        o->props.elms[i] = NULL;
      } else if (ret != RET_OK) {
        break;
      }
    }
    darray_remove_all(&(o->props), pointer_compare, NULL);
  }

  return ret;
}

value_t* object_default_find_prop(tk_object_t* obj, tk_compare_t cmp, const void* data) {
  named_value_t* nv = NULL;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL && cmp != NULL, NULL);
  nv = (named_value_t*)darray_find_ex(&(o->props), cmp, (void*)data);
  return nv != NULL ? &(nv->value) : NULL;
}

tk_object_t* object_default_clone(object_default_t* o) {
  uint32_t i = 0;
  tk_object_t* dup = NULL;
  object_default_t* dupo = NULL;
  return_value_if_fail(o != NULL, NULL);

  dup = object_default_create();
  return_value_if_fail(dup != NULL, NULL);

  dupo = OBJECT_DEFAULT(dup);

  dupo->enable_path = o->enable_path;
  dupo->keep_prop_type = o->keep_prop_type;
  dupo->name_case_insensitive = o->name_case_insensitive;

  for (i = 0; i < o->props.size; i++) {
    named_value_t* iter = (named_value_t*)(o->props.elms[i]);
    named_value_t* nv = named_value_create_ex(iter->name, &(iter->value));

    if (darray_push(&(dupo->props), nv) != RET_OK) {
      named_value_destroy(nv);
      break;
    }
  }

  return dup;
}

static const object_vtable_t s_object_default_vtable = {
    .type = OBJECT_DEFAULT_TYPE,
    .desc = OBJECT_DEFAULT_TYPE,
    .size = sizeof(object_default_t),
    .is_collection = FALSE,
    .on_destroy = object_default_on_destroy,
    .clone = (tk_object_clone_t)object_default_clone,
    .compare = object_default_compare,
    .get_prop = object_default_get_prop,
    .set_prop = object_default_set_prop,
    .can_exec = object_default_can_exec,
    .exec = object_default_exec,
    .remove_prop = object_default_remove_prop,
    .foreach_prop = object_default_foreach_prop,
    .clear_props = object_default_clear_props,
    .find_prop = object_default_find_prop,
};

tk_object_t* object_default_create_ex(bool_t enable_path) {
  tk_object_t* obj = tk_object_create(&s_object_default_vtable);
  object_default_t* o = OBJECT_DEFAULT(obj);
  ENSURE(o);
  return_value_if_fail(obj != NULL, NULL);

  o->enable_path = enable_path;
  darray_init(&(o->props), 5, (tk_destroy_t)named_value_destroy, (tk_compare_t)named_value_compare);

  return obj;
}

tk_object_t* object_default_create(void) {
  return object_default_create_ex(TRUE);
}

ret_t object_default_set_keep_prop_type(tk_object_t* obj, bool_t keep_prop_type) {
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  o->keep_prop_type = keep_prop_type;

  return RET_OK;
}

ret_t object_default_set_name_case_insensitive(tk_object_t* obj, bool_t name_case_insensitive) {
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  o->name_case_insensitive = name_case_insensitive;

  return RET_OK;
}

object_default_t* object_default_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_default_vtable, NULL);
  return (object_default_t*)(obj);
}

ret_t object_default_unref(tk_object_t* obj) {
  return tk_object_unref(obj);
}
