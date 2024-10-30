/**
 * File:   object_hash.c
 * Author: AWTK Develop Team
 * Brief:  用散列值查询属性的object
 *
 * Copyright (c) 2024 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2024-08-19 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/named_value_hash.h"

#include "tkc/object_hash.h"

static int32_t object_hash_find_prop_index_by_name(tk_object_t* obj, const char* name,
                                                   bool_t* p_is_gen_hash, uint64_t* p_hash) {
  int32_t ret = -1;
  object_hash_t* o = OBJECT_HASH(obj);
  return_value_if_fail(o != NULL && name != NULL, -1);

  if (*name == '[' && tk_isdigit(name[1]) && tk_str_end_with(name, "]")) {
    ret = tk_atoi(name + 1);
    return_value_if_fail(ret < o->props.size, -1);
  } else {
    uint64_t hash = named_value_hash_get_hash_from_str(name);
    if (p_hash != NULL) {
      (*p_hash) = hash;
    }
    if (p_is_gen_hash != NULL) {
      (*p_is_gen_hash) = TRUE;
    }
    ret = darray_bsearch_index(&(o->props), (tk_compare_t)named_value_hash_compare_by_hash,
                               (void*)hash);

    if (ret >= 0) {
      named_value_hash_t* nvh = (named_value_hash_t*)darray_get(&o->props, ret);
      if (!tk_str_eq(name, nvh->base.name)) {
        darray_t bucket;
        darray_init(&bucket, 8, NULL, (tk_compare_t)named_value_compare);

        log_debug("%s: Hashing collision!\r\n", __FUNCTION__);
        log_debug("Hash: %lld\r\n", hash);
        log_debug("Wrong name: %s\r\n", nvh->base.name);
        log_debug("Target name: %s\r\n\r\n", name);

        ret = -1;
        if (RET_OK == darray_find_all(&o->props, (tk_compare_t)named_value_hash_compare_by_hash,
                                      (void*)hash, &bucket)) {
          named_value_hash_t* right_nvh = NULL;
          darray_sort(&bucket, (tk_compare_t)named_value_compare);
          right_nvh = (named_value_hash_t*)darray_bsearch(
              &bucket, (tk_compare_t)named_value_compare_by_name, (void*)name);
          if (right_nvh != NULL) {
            ret = darray_find_index_ex(&o->props, pointer_compare, (void*)right_nvh);
          }
        }

        darray_deinit(&bucket);
      }
    }
  }

  return ret;
}

static value_t* object_hash_find_prop_by_name(tk_object_t* obj, const char* name,
                                              bool_t* p_is_gen_hash, uint64_t* p_hash) {
  named_value_hash_t* nvh = NULL;
  object_hash_t* o = OBJECT_HASH(obj);
  int32_t index = -1;
  return_value_if_fail(o != NULL && name != NULL, NULL);

  index = object_hash_find_prop_index_by_name(obj, name, p_is_gen_hash, p_hash);
  if (index >= 0) {
    nvh = (named_value_hash_t*)darray_get(&o->props, index);
  }

  return nvh != NULL ? &(nvh->base.value) : NULL;
}

static ret_t object_hash_clear_props(tk_object_t* obj) {
  object_hash_t* o = OBJECT_HASH(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return darray_clear(&(o->props));
}

static ret_t object_hash_on_destroy(tk_object_t* obj) {
  object_hash_t* o = OBJECT_HASH(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return darray_deinit(&(o->props));
}

static ret_t object_hash_remove_prop(tk_object_t* obj, const char* name) {
  int32_t index = 0;
  object_hash_t* o = OBJECT_HASH(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->props.size > 0 && o->enable_path) {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_remove_prop(sub, name);
    }
  }

  index = object_hash_find_prop_index_by_name(obj, name, NULL, NULL);
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

static ret_t object_hash_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  value_t* vv = NULL;
  ret_t ret = RET_NOT_FOUND;
  object_hash_t* o = OBJECT_HASH(obj);
  bool_t is_gen_hash = FALSE;
  uint64_t hash = 0;
  ENSURE(o);

  if (o->props.size > 0 && o->enable_path) {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_set_prop(sub, name, v);
    }
  }

  vv = object_hash_find_prop_by_name(obj, name, &is_gen_hash, &hash);
  if (vv != NULL) {
    if (o->keep_prop_type) {
      ret = value_deep_copy_keep_type(vv, v);
    } else {
      value_reset(vv);
      ret = value_deep_copy(vv, v);
    }
  } else {
    named_value_hash_t* nvh = named_value_hash_create_ex(NULL, v);
    return_value_if_fail(nvh != NULL, RET_OOM);

    if (is_gen_hash) {
      named_value_set_name(&nvh->base, name);
      nvh->hash = hash;
    } else {
      named_value_hash_set_name(nvh, name);
    }

    ret = darray_sorted_insert(&(o->props), nvh, (tk_compare_t)named_value_hash_compare, FALSE);
    if (ret != RET_OK) {
      named_value_hash_destroy(nvh);
    }
  }

  return ret;
}

static ret_t object_hash_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_hash_t* o = OBJECT_HASH(obj);
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
    const value_t* vv = object_hash_find_prop_by_name(obj, name, NULL, NULL);
    if (vv != NULL) {
      ret = value_copy(v, vv);
    } else {
      ret = RET_NOT_FOUND;
    }
  }

  return ret;
}

static bool_t object_hash_can_exec(tk_object_t* obj, const char* name, const char* args) {
  bool_t ret = FALSE;
  object_hash_t* o = OBJECT_HASH(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->enable_path) {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_can_exec(sub, name, args);
    }
  }

  return ret;
}

static ret_t object_hash_exec(tk_object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_NOT_FOUND;
  object_hash_t* o = OBJECT_HASH(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->enable_path) {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_exec(sub, name, args);
    }
  }

  return ret;
}

static ret_t object_hash_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_OK;
  object_hash_t* o = OBJECT_HASH(obj);
  return_value_if_fail(o != NULL && on_prop != NULL, RET_BAD_PARAMS);
  if (o->props.size > 0) {
    uint32_t i = 0;
    for (i = 0; i < o->props.size; i++) {
      named_value_hash_t* iter = (named_value_hash_t*)(o->props.elms[i]);
      ret = on_prop(ctx, iter);
      if (ret == RET_REMOVE) {
        named_value_hash_destroy(iter);
        o->props.elms[i] = NULL;
      } else if (ret != RET_OK) {
        break;
      }
    }
    darray_remove_all(&(o->props), pointer_compare, NULL);
  }

  return ret;
}

static value_t* object_hash_find_prop(tk_object_t* obj, tk_compare_t cmp, const void* data) {
  named_value_hash_t* nvh = NULL;
  object_hash_t* o = OBJECT_HASH(obj);
  return_value_if_fail(o != NULL && cmp != NULL, NULL);
  nvh = (named_value_hash_t*)darray_find_ex(&(o->props), cmp, (void*)data);
  return nvh != NULL ? &(nvh->base.value) : NULL;
}

static tk_object_t* object_hash_clone(object_hash_t* o) {
  uint32_t i = 0;
  tk_object_t* dup = NULL;
  object_hash_t* dupo = NULL;
  return_value_if_fail(o != NULL, NULL);

  dup = object_hash_create();
  return_value_if_fail(dup != NULL, NULL);

  dupo = OBJECT_HASH(dup);

  dupo->enable_path = o->enable_path;
  dupo->keep_prop_type = o->keep_prop_type;

  for (i = 0; i < o->props.size; i++) {
    named_value_hash_t* iter = (named_value_hash_t*)(o->props.elms[i]);
    named_value_hash_t* nvh = named_value_hash_clone(iter);

    if (darray_push(&(dupo->props), nvh) != RET_OK) {
      named_value_hash_destroy(nvh);
      break;
    }
  }

  return dup;
}

static const object_vtable_t s_object_hash_vtable = {
    .type = OBJECT_HASH_TYPE,
    .desc = OBJECT_HASH_TYPE,
    .size = sizeof(object_hash_t),
    .is_collection = FALSE,
    .on_destroy = object_hash_on_destroy,
    .clone = (tk_object_clone_t)object_hash_clone,
    .compare = tk_object_compare_name_without_nullptr,
    .get_prop = object_hash_get_prop,
    .set_prop = object_hash_set_prop,
    .can_exec = object_hash_can_exec,
    .exec = object_hash_exec,
    .remove_prop = object_hash_remove_prop,
    .foreach_prop = object_hash_foreach_prop,
    .clear_props = object_hash_clear_props,
    .find_prop = object_hash_find_prop,
};

tk_object_t* object_hash_create_ex(bool_t enable_path) {
  tk_object_t* obj = tk_object_create(&s_object_hash_vtable);
  object_hash_t* o = OBJECT_HASH(obj);
  ENSURE(o);
  return_value_if_fail(obj != NULL, NULL);

  o->enable_path = enable_path;
  darray_init(&(o->props), 8, (tk_destroy_t)named_value_hash_destroy,
              (tk_compare_t)named_value_hash_compare);

  return obj;
}

tk_object_t* object_hash_create(void) {
  return object_hash_create_ex(TRUE);
}

ret_t object_hash_set_keep_prop_type(tk_object_t* obj, bool_t keep_prop_type) {
  object_hash_t* o = OBJECT_HASH(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  o->keep_prop_type = keep_prop_type;

  return RET_OK;
}

object_hash_t* object_hash_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_hash_vtable, NULL);
  return (object_hash_t*)(obj);
}
