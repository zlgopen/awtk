/**
 * File:   object_default.c
 * Author: AWTK Develop Team
 * Brief:  default object
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
#include "tkc/object_default.h"

static void object_default_check(object_default_t* o) {
#ifndef NDEBUG
  uint32_t i = 0;

  for (i = 0; i < o->props_size; i++) {
    named_value_t* iter = o->props + i;
    if (i > 0) {
      named_value_t* prev = o->props + i - 1;
      assert(strcmp(prev->name, iter->name) < 0);
    }
  }
#else
  (void)o;
#endif
}

static ret_t object_default_clean_invalid_props(object_t* obj) {
  object_default_t* o = OBJECT_DEFAULT(obj);

  if (o->props_size > 0) {
    uint32_t i = 0;
    named_value_t* dst = o->props;

    for (i = 0; i < o->props_size; i++) {
      named_value_t* iter = o->props + i;

      if (iter->name != NULL) {
        if (dst != iter) {
          memcpy(dst, iter, sizeof(named_value_t));
        }
        dst++;
      }
    }

    o->props_size = dst - o->props;
  }

  return RET_OK;
}

ret_t object_default_clear_props(object_t* obj) {
  uint32_t i = 0;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  for (i = 0; i < o->props_size; i++) {
    named_value_t* iter = o->props + i;
    named_value_deinit(iter);
  }

  o->props_size = 0;

  return RET_OK;
}

static ret_t object_default_on_destroy(object_t* obj) {
  object_default_t* o = OBJECT_DEFAULT(obj);

  object_default_clear_props(obj);
  o->props_capacity = 0;
  TKMEM_FREE(o->props);

  return RET_OK;
}

static int32_t object_default_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static int32_t object_default_find(named_value_t* start, uint32_t nr, const char* name) {
  int32_t low = 0;
  int32_t mid = 0;
  int32_t result = 0;
  int32_t high = nr - 1;
  named_value_t* iter = NULL;

  while (low <= high) {
    mid = low + ((high - low) >> 1);
    iter = start + mid;
    result = tk_str_cmp(name, iter->name);

    if (result == 0) {
      return mid;
    } else if (result > 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return low;
}

static ret_t object_default_extend(object_t* obj) {
  ret_t ret = RET_OOM;
  object_default_t* o = OBJECT_DEFAULT(obj);

  if (o->props_size < o->props_capacity) {
    ret = RET_OK;
  } else {
    named_value_t* props = NULL;
    uint32_t capacity = o->props_capacity + (o->props_capacity >> 1) + 1;
    props = TKMEM_REALLOCT(named_value_t, o->props, capacity);

    if (props != NULL) {
      o->props = props;
      o->props_capacity = capacity;
      ret = RET_OK;
    }
  }

  return ret;
}

static ret_t object_default_insert_prop_at(object_t* obj, int32_t index, const char* name,
                                           const value_t* v) {
  int32_t i = 0;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(object_default_extend(obj) == RET_OK, RET_OOM);

  for (i = o->props_size; i > index; i--) {
    named_value_t* iter = o->props + i;
    named_value_t* prev = o->props + i - 1;

    memcpy(iter, prev, sizeof(named_value_t));
  }

  o->props_size++;
  named_value_init(o->props + index, name, v);
  object_default_check(o);

  return RET_OK;
}

static ret_t object_default_remove_prop(object_t* obj, const char* name) {
  ret_t ret = RET_NOT_FOUND;
  object_default_t* o = OBJECT_DEFAULT(obj);

  if (o->props_size > 0) {
    named_value_t* iter = NULL;
    int32_t index = object_default_find(o->props, o->props_size, name);

    if (index < 0 || index >= o->props_size) {
      return RET_NOT_FOUND;
    }

    iter = o->props + index;
    if (tk_str_eq(iter->name, name)) {
      named_value_deinit(iter);
      ret = object_default_clean_invalid_props(obj);
    }
    object_default_check(o);
  }

  return ret;
}

static ret_t object_default_set_prop(object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(object_default_extend(obj) == RET_OK, RET_OOM);

  if (o->props_size > 0) {
    int32_t index = object_default_find(o->props, o->props_size, name);
    named_value_t* iter = o->props + index;

    if (index >= o->props_size) {
      ret = object_default_insert_prop_at(obj, o->props_size, name, v);
    } else if (index < 0) {
      ret = object_default_insert_prop_at(obj, 0, name, v);
    } else if (tk_str_eq(iter->name, name)) {
      ret = named_value_set_value(iter, v);
    } else {
      int32_t i = 0;
      index = tk_max(0, index - 1);

      for (i = index; i < o->props_size; i++) {
        named_value_t* iter = o->props + i;
        int32_t result = tk_str_cmp(name, iter->name);

        if (result < 0) {
          ret = object_default_insert_prop_at(obj, i, name, v);
          break;
        }
      }
    }
  } else {
    ret = object_default_insert_prop_at(obj, 0, name, v);
  }

  return ret;
}

static ret_t object_default_get_prop(object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_default_t* o = OBJECT_DEFAULT(obj);

  if (o->props_size > 0) {
    named_value_t* iter = NULL;
    int32_t index = object_default_find(o->props, o->props_size, name);

    if (index < 0 || index >= o->props_size) {
      return RET_NOT_FOUND;
    }

    iter = o->props + index;
    if (tk_str_eq(iter->name, name)) {
      ret = value_copy(v, &(iter->value));
    }
  }

  return ret;
}

static ret_t object_default_foreach_prop(object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_OK;
  object_default_t* o = OBJECT_DEFAULT(obj);

  if (o->props_size > 0) {
    uint32_t i = 0;

    for (i = 0; i < o->props_size; i++) {
      named_value_t* iter = o->props + i;
      ret = on_prop(ctx, iter);

      if (ret == RET_REMOVE) {
        named_value_deinit(iter);
      } else if (ret != RET_OK) {
        break;
      }
    }

    object_default_clean_invalid_props(obj);
  }

  return ret;
}

value_t* object_default_find_prop(object_t* obj, tk_compare_t cmp, const void* ctx) {
  object_default_t* o = OBJECT_DEFAULT(obj);
  return_value_if_fail(o != NULL && cmp != NULL, NULL);

  if (o->props_size > 0) {
    uint32_t i = 0;
    for (i = 0; i < o->props_size; i++) {
      named_value_t* iter = o->props + i;
      if (cmp(iter, ctx) == 0) {
        return &(iter->value);
      }
    }
  }

  return NULL;
}

static const object_vtable_t s_object_default_vtable = {
    .type = "object_default",
    .desc = "object_default",
    .size = sizeof(object_default_t),
    .is_collection = FALSE,
    .on_destroy = object_default_on_destroy,

    .compare = object_default_compare,
    .get_prop = object_default_get_prop,
    .set_prop = object_default_set_prop,
    .remove_prop = object_default_remove_prop,
    .foreach_prop = object_default_foreach_prop};

object_t* object_default_create(void) {
  uint32_t init_capacity = 5;
  object_t* obj = object_create(&s_object_default_vtable);

  if (init_capacity > 0) {
    object_default_t* o = OBJECT_DEFAULT(obj);

    o->props = TKMEM_ZALLOCN(named_value_t, init_capacity);
    if (o->props != NULL) {
      o->props_capacity = init_capacity;
    }
  }

  return obj;
}

object_t* object_default_clone(object_default_t* o) {
  object_t* dup = NULL;
  return_value_if_fail(o != NULL, NULL);

  dup = object_default_create();
  return_value_if_fail(dup != NULL, NULL);

  if (o->props_size > 0) {
    uint32_t i = 0;
    for (i = 0; i < o->props_size; i++) {
      named_value_t* iter = o->props + i;

      object_set_prop(dup, iter->name, &(iter->value));
    }
  }

  return dup;
}

object_default_t* object_default_cast(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_default_vtable, NULL);

  return (object_default_t*)(obj);
}

ret_t object_default_unref(object_t* obj) {
  return object_unref(obj);
}
