/**
 * File:   object_array.c
 * Author: AWTK Develop Team
 * Brief:  object array
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/named_value.h"
#include "tkc/object_array.h"

static ret_t object_array_clean_invalid_props(object_t* obj) {
  object_array_t* o = OBJECT_ARRAY(obj);

  if (o->props_size > 0) {
    uint32_t i = 0;
    value_t* dst = o->props;

    for (i = 0; i < o->props_size; i++) {
      value_t* iter = o->props + i;

      if (iter->type != VALUE_TYPE_INVALID) {
        if (dst != iter) {
          memcpy(dst, iter, sizeof(value_t));
        }
        dst++;
      }
    }

    o->props_size = dst - o->props;
  }

  return RET_OK;
}

ret_t object_array_clear_props(object_t* obj) {
  uint32_t i = 0;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  for (i = 0; i < o->props_size; i++) {
    value_t* iter = o->props + i;
    value_reset(iter);
  }

  o->props_size = 0;

  return RET_OK;
}

static ret_t object_array_on_destroy(object_t* obj) {
  object_array_t* o = OBJECT_ARRAY(obj);

  object_array_clear_props(obj);
  o->props_capacity = 0;
  TKMEM_FREE(o->props);

  return RET_OK;
}

static int32_t object_array_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t object_array_extend(object_t* obj) {
  ret_t ret = RET_OOM;
  object_array_t* o = OBJECT_ARRAY(obj);

  if (o->props_size < o->props_capacity) {
    ret = RET_OK;
  } else {
    value_t* props = NULL;
    uint32_t capacity = o->props_capacity + (o->props_capacity >> 1) + 1;
    props = TKMEM_REALLOCT(value_t, o->props, capacity);

    if (props != NULL) {
      o->props = props;
      o->props_capacity = capacity;
      ret = RET_OK;
    }
  }

  return ret;
}

static ret_t object_array_append(object_t* obj, const value_t* v) {
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(object_array_extend(obj) == RET_OK, RET_OOM);

  value_deep_copy(o->props + o->props_size, v);
  o->props_size++;

  return RET_OK;
}

static ret_t object_array_remove_prop(object_t* obj, const char* name) {
  ret_t ret = RET_NOT_FOUND;
  int32_t index = tk_atoi(name);
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(isdigit(*name), RET_NOT_FOUND);

  if (index >= 0 && index < o->props_size) {
    value_t* iter = o->props + index;
    value_reset(iter);
    ret = object_array_clean_invalid_props(obj);
  }

  return ret;
}

static ret_t object_array_set_prop(object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_array_t* o = OBJECT_ARRAY(obj);
  int32_t index = tk_atoi(name);
  return_value_if_fail(object_array_extend(obj) == RET_OK, RET_OOM);

  if (isdigit(*name) && index >= 0 && index < o->props_size) {
    value_t* iter = o->props + index;
    value_reset(iter);
    ret = value_deep_copy(iter, v);
  } else if (index == -1) {
    ret = object_array_append(obj, v);
  } else {
    ret = RET_BAD_PARAMS;
  }

  return ret;
}

static ret_t object_array_get_prop(object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_array_t* o = OBJECT_ARRAY(obj);

  if (tk_str_eq(name, "length") || tk_str_eq(name, "size")) {
    value_set_int(v, o->props_size);
    ret = RET_OK;
  } else if (isdigit(*name)) {
    int32_t index = tk_atoi(name);
    if (index >= 0 && index < o->props_size) {
      value_t* iter = o->props + index;
      ret = value_copy(v, iter);
    }
  }

  return ret;
}

static ret_t object_array_foreach_prop(object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_OK;
  object_array_t* o = OBJECT_ARRAY(obj);

  if (o->props_size > 0) {
    uint32_t i = 0;
    named_value_t nv;
    char name[TK_NAME_LEN + 1];

    nv.name = name;
    for (i = 0; i < o->props_size; i++) {
      value_t* iter = o->props + i;
      tk_snprintf(name, TK_NAME_LEN, "%u", i);

      value_copy(&(nv.value), iter);
      ret = on_prop(ctx, &nv);

      if (ret == RET_REMOVE) {
        value_reset(iter);
      } else if (ret != RET_OK) {
        break;
      }
    }

    object_array_clean_invalid_props(obj);
  }

  return ret;
}

static const object_vtable_t s_object_array_vtable = {.type = "object_array",
                                                      .desc = "object_array",
                                                      .size = sizeof(object_array_t),
                                                      .is_collection = TRUE,
                                                      .on_destroy = object_array_on_destroy,

                                                      .compare = object_array_compare,
                                                      .get_prop = object_array_get_prop,
                                                      .set_prop = object_array_set_prop,
                                                      .remove_prop = object_array_remove_prop,
                                                      .foreach_prop = object_array_foreach_prop};

static object_t* object_array_create_with_capacity(uint32_t init_capacity) {
  object_t* obj = object_create(&s_object_array_vtable);

  if (init_capacity > 0) {
    object_array_t* o = OBJECT_ARRAY(obj);

    o->props = TKMEM_ZALLOCN(value_t, init_capacity);
    if (o->props != NULL) {
      o->props_capacity = init_capacity;
    }
  }

  return obj;
}

object_t* object_array_create(void) {
  return object_array_create_with_capacity(5);
}

object_t* object_array_clone(object_array_t* o) {
  object_t* dup = NULL;
  return_value_if_fail(o != NULL, NULL);

  dup = object_array_create_with_capacity(o->props_capacity);
  return_value_if_fail(dup != NULL, NULL);

  if (o->props_size > 0) {
    uint32_t i = 0;
    object_array_t* dup_o = OBJECT_ARRAY(dup);

    for (i = 0; i < o->props_size; i++) {
      value_t* src = o->props + i;
      value_t* dst = dup_o->props + i;

      value_deep_copy(dst, src);
    }
    dup_o->props_size = o->props_size;
  }

  return dup;
}

object_array_t* object_array_cast(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_array_vtable, NULL);

  return (object_array_t*)(obj);
}

ret_t object_array_unref(object_t* obj) {
  return object_unref(obj);
}
