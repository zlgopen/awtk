/**
 * File:   object_default.c
 * Author: AWTK Develop Team
 * Brief:  default object
 *
 * Copyright (c) 2019 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static value_t* object_default_find_prop_by_name(tk_object_t* obj, const char* name) {
  named_value_t* nv = NULL;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL && name != NULL, NULL);

  if (*name == '[') {
    uint32_t index = tk_atoi(name + 1);
    return_value_if_fail(index < o->props.size, NULL);
    nv = (named_value_t*)(o->props.elms[index]);
  } else {
    nv = darray_bsearch(&(o->props), (tk_compare_t)named_value_compare_by_name, (void*)name);
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

  index = darray_bsearch_index(&(o->props), (tk_compare_t)named_value_compare_by_name, (void*)name);
  if (index >= 0) {
    return darray_remove_index(&(o->props), index);
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t object_default_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  value_t* vv = NULL;
  ret_t ret = RET_NOT_FOUND;
  object_default_t* o = OBJECT_DEFAULT(obj);

  if (o->props.size > 0 && o->enable_path) {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_set_prop(sub, name, v);
    }
  }

  vv = object_default_find_prop_by_name(obj, name);
  if (vv != NULL) {
    ret = RET_OK;
    value_reset(vv);
    value_deep_copy(vv, v);
  } else {
    named_value_t* nv = named_value_create_ex(name, v);
    return_value_if_fail(nv != NULL, RET_OOM);
    ret = darray_sorted_insert(&(o->props), nv, (tk_compare_t)named_value_compare, FALSE);
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

value_t* object_default_find_prop(tk_object_t* obj, tk_compare_t cmp, const void* ctx) {
  named_value_t* nv = NULL;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL && cmp != NULL, NULL);
  nv = (named_value_t*)darray_find_ex(&(o->props), cmp, (void*)ctx);
  return nv != NULL ? &(nv->value) : NULL;
}

static const object_vtable_t s_object_default_vtable = {
    .type = "object_default",
    .desc = "object_default",
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
    .foreach_prop = object_default_foreach_prop};

tk_object_t* object_default_create_ex(bool_t enable_path) {
  tk_object_t* obj = tk_object_create(&s_object_default_vtable);
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(obj != NULL, NULL);

  o->enable_path = enable_path;
  darray_init(&(o->props), 5, (tk_destroy_t)named_value_destroy, (tk_compare_t)named_value_compare);

  return obj;
}

tk_object_t* object_default_create(void) {
  return object_default_create_ex(TRUE);
}

tk_object_t* object_default_clone(object_default_t* o) {
  uint32_t i = 0;
  tk_object_t* dup = NULL;
  object_default_t* dupo = NULL;
  return_value_if_fail(o != NULL, NULL);

  dup = object_default_create();
  return_value_if_fail(dup != NULL, NULL);

  dupo = OBJECT_DEFAULT(dup);
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

object_default_t* object_default_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_default_vtable, NULL);
  return (object_default_t*)(obj);
}

ret_t object_default_unref(tk_object_t* obj) {
  return tk_object_unref(obj);
}
