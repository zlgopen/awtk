/**
 * File:   object_array.c
 * Author: AWTK Develop Team
 * Brief:  object array
 *
 * Copyright (c) 2019 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static ret_t object_array_clean_invalid_props(tk_object_t* obj) {
  object_array_t* o = OBJECT_ARRAY(obj);
  event_t e = event_init(EVT_ITEMS_CHANGED, o);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (o->size > 0) {
    uint32_t i = 0;
    value_t* dst = o->props;

    for (i = 0; i < o->size; i++) {
      value_t* iter = o->props + i;

      if (iter->type != VALUE_TYPE_INVALID) {
        if (dst != iter) {
          memcpy(dst, iter, sizeof(value_t));
        }
        dst++;
      }
    }

    o->size = dst - o->props;
  }

  emitter_dispatch(EMITTER(o), &e);

  return RET_OK;
}

ret_t object_array_clear_props(tk_object_t* obj) {
  uint32_t i = 0;
  object_array_t* o = OBJECT_ARRAY(obj);
  event_t e = event_init(EVT_ITEMS_CHANGED, o);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  for (i = 0; i < o->size; i++) {
    value_t* iter = o->props + i;
    value_reset(iter);
  }

  o->size = 0;
  emitter_dispatch(EMITTER(o), &e);

  return RET_OK;
}

static ret_t object_array_on_destroy(tk_object_t* obj) {
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  object_array_clear_props(obj);
  o->capacity = 0;
  TKMEM_FREE(o->props);

  return RET_OK;
}

static int32_t object_array_compare(tk_object_t* obj, tk_object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t object_array_extend(tk_object_t* obj) {
  ret_t ret = RET_OOM;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->size < o->capacity) {
    ret = RET_OK;
  } else {
    value_t* props = NULL;
    uint32_t capacity = o->capacity + (o->capacity >> 1) + 1;
    props = TKMEM_REALLOCT(value_t, o->props, capacity);

    if (props != NULL) {
      o->props = props;
      o->capacity = capacity;
      ret = RET_OK;
    }
  }

  return ret;
}

ret_t object_array_insert(tk_object_t* obj, uint32_t index, const value_t* v) {
  value_t* s = NULL;
  value_t* d = NULL;
  value_t* p = NULL;
  object_array_t* o = OBJECT_ARRAY(obj);
  event_t e = event_init(EVT_ITEMS_CHANGED, o);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return_value_if_fail(o != NULL && v != NULL, RET_BAD_PARAMS);
  index = tk_min(index, o->size);
  return_value_if_fail(object_array_extend(obj) == RET_OK, RET_OOM);

  p = o->props + index;
  d = o->props + o->size;
  s = d - 1;

  while (s >= p) {
    *d-- = *s--;
  }
  value_deep_copy(p, v);
  o->size++;
  emitter_dispatch(EMITTER(o), &e);

  return RET_OK;
}

ret_t object_array_push(tk_object_t* obj, const value_t* v) {
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return object_array_insert(obj, o->size, v);
}

static int32_t object_array_parse_index(const char* name) {
  if (tk_isdigit(*name)) {
    return tk_atoi(name);
  } else if (*name == '[') {
    return tk_atoi(name + 1);
  } else if (tk_str_eq(name, "-1")) {
    return -1;
  } else {
    return -2;
  }
}

int32_t object_array_index_of(tk_object_t* obj, const value_t* v) {
  int32_t i = 0;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, -1);

  for (i = 0; i < o->size; i++) {
    value_t* iter = o->props + i;
    if (value_equal(iter, v)) {
      return i;
    }
  }

  return -1;
}

int32_t object_array_last_index_of(tk_object_t* obj, const value_t* v) {
  int32_t i = 0;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, -1);

  if (o->size > 0) {
    for (i = o->size - 1; i >= 0; i--) {
      value_t* iter = o->props + i;
      if (value_equal(iter, v)) {
        return i;
      }
    }
  }

  return -1;
}

ret_t object_array_remove(tk_object_t* obj, uint32_t index) {
  return object_array_get_and_remove(obj, index, NULL);
}

ret_t object_array_get_and_remove(tk_object_t* obj, uint32_t index, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (index < o->size) {
    value_t* iter = o->props + index;
    if (v != NULL) {
      *v = *iter;
      memset(iter, 0x00, sizeof(*iter));
    } else {
      value_reset(iter);
    }
    ret = object_array_clean_invalid_props(obj);
  }

  return ret;
}

ret_t object_array_shift(tk_object_t* obj, value_t* v) {
  return object_array_get_and_remove(obj, 0, v);
}

static ret_t object_array_remove_prop(tk_object_t* obj, const char* name) {
  int32_t index = object_array_parse_index(name);

  return object_array_remove(obj, index);
}

ret_t object_array_pop(tk_object_t* obj, value_t* v) {
  value_t* last = NULL;
  object_array_t* o = OBJECT_ARRAY(obj);
  event_t e = event_init(EVT_ITEMS_CHANGED, o);
  return_value_if_fail(o != NULL && o->size > 0, RET_BAD_PARAMS);
  last = o->props + o->size - 1;
  *v = *last;
  memset(last, 0x00, sizeof(value_t));
  o->size--;

  emitter_dispatch(EMITTER(o), &e);

  return RET_OK;
}

ret_t object_array_set(tk_object_t* obj, uint32_t index, const value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_array_t* o = OBJECT_ARRAY(obj);
  event_t e = event_init(EVT_ITEMS_CHANGED, o);
  return_value_if_fail(object_array_extend(obj) == RET_OK, RET_OOM);

  if (index < o->size) {
    value_t* iter = o->props + index;
    value_reset(iter);
    ret = value_deep_copy(iter, v);
    emitter_dispatch(EMITTER(o), &e);
  } else if (index == 0xffffffff) {
    ret = object_array_push(obj, v);
  } else {
    ret = RET_BAD_PARAMS;
  }

  return ret;
}

static ret_t object_array_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  int32_t index = -1;
  ret_t ret = RET_NOT_FOUND;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
  if (sub != NULL) {
    return tk_object_set_prop(sub, name, v);
  }

  index = object_array_parse_index(name);
  if (index != -2) {
    ret = object_array_set(obj, index, v);
  }

  return ret;
}

ret_t object_array_get(tk_object_t* obj, uint32_t i, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (i < o->size) {
    value_t* iter = o->props + i;
    ret = value_copy(v, iter);
  }

  return ret;
}

static ret_t object_array_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, "length") || tk_str_eq(name, "size") || tk_str_eq(name, "#size")) {
    value_set_int(v, o->size);
    ret = RET_OK;
  } else if (tk_str_eq(name, "capacity")) {
    value_set_int(v, o->capacity);
    ret = RET_OK;
  } else if (o->size > 0) {
    tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
    if (sub != NULL) {
      return tk_object_get_prop(sub, name, v);
    }

    int32_t index = object_array_parse_index(name);
    ret = object_array_get(obj, index, v);
  }

  return ret;
}

static bool_t object_array_can_exec(tk_object_t* obj, const char* name, const char* args) {
  bool_t ret = FALSE;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
  if (sub != NULL) {
    return tk_object_can_exec(sub, name, args);
  }

  return ret;
}

static ret_t object_array_exec(tk_object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_NOT_FOUND;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  tk_object_t* sub = tk_object_get_child_object(obj, name, &name);
  if (sub != NULL) {
    return tk_object_exec(sub, name, args);
  }

  return ret;
}

static ret_t object_array_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_OK;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->size > 0) {
    uint32_t i = 0;
    named_value_t nv;
    char name[TK_NAME_LEN + 1];

    nv.name = name;
    for (i = 0; i < o->size; i++) {
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
                                                      .can_exec = object_array_can_exec,
                                                      .exec = object_array_exec,
                                                      .remove_prop = object_array_remove_prop,
                                                      .foreach_prop = object_array_foreach_prop};

static tk_object_t* object_array_create_with_capacity(uint32_t init_capacity) {
  tk_object_t* obj = tk_object_create(&s_object_array_vtable);
  return_value_if_fail(obj != NULL, NULL);
  if (init_capacity > 0) {
    object_array_t* o = OBJECT_ARRAY(obj);

    o->props = TKMEM_ZALLOCN(value_t, init_capacity);
    if (o->props != NULL) {
      o->capacity = init_capacity;
    }
  }

  return obj;
}

tk_object_t* object_array_create(void) {
  return object_array_create_with_capacity(5);
}

tk_object_t* object_array_clone(tk_object_t* o) {
  object_array_t* arr = OBJECT_ARRAY(o);
  return_value_if_fail(arr != NULL, NULL);

  return object_array_dup(TK_OBJECT(o), 0, arr->size);
}

object_array_t* object_array_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_array_vtable, NULL);

  return (object_array_t*)(obj);
}

ret_t object_array_unref(tk_object_t* obj) {
  return tk_object_unref(obj);
}

tk_object_t* object_array_create_with_str(const char* str, const char* sep, value_type_t type) {
  str_t s;
  value_t v;
  tk_object_t* obj = NULL;
  const char* p = NULL;
  const char* end = NULL;
  return_value_if_fail(str != NULL && sep != NULL && type != VALUE_TYPE_INVALID, NULL);
  obj = object_array_create();
  return_value_if_fail(obj != NULL, NULL);

  if (str_init(&s, 100) == NULL) {
    TK_OBJECT_UNREF(obj);
    return NULL;
  }

  p = str;
  do {
    end = strstr(p, sep);
    if (end != NULL) {
      str_set_with_len(&s, p, end - p);
    } else {
      str_set(&s, p);
    }

    switch (type) {
      case VALUE_TYPE_UINT8: {
        value_set_uint8(&v, tk_atoi(s.str));
        break;
      }
      case VALUE_TYPE_UINT16: {
        value_set_uint16(&v, tk_atoi(s.str));
        break;
      }
      case VALUE_TYPE_UINT32: {
        value_set_uint32(&v, tk_atoi(s.str));
        break;
      }
      case VALUE_TYPE_UINT64: {
        value_set_uint64(&v, tk_atoul(s.str));
        break;
      }
      case VALUE_TYPE_INT8: {
        value_set_int8(&v, tk_atoi(s.str));
        break;
      }
      case VALUE_TYPE_INT16: {
        value_set_int16(&v, tk_atoi(s.str));
        break;
      }
      case VALUE_TYPE_INT32: {
        value_set_int32(&v, tk_atoi(s.str));
        break;
      }
      case VALUE_TYPE_INT64: {
        value_set_int64(&v, tk_atoul(s.str));
        break;
      }
      case VALUE_TYPE_FLOAT: {
        value_set_float(&v, tk_atof(s.str));
        break;
      }
      case VALUE_TYPE_FLOAT32: {
        value_set_float32(&v, tk_atof(s.str));
        break;
      }
      case VALUE_TYPE_DOUBLE: {
        value_set_double(&v, tk_atof(s.str));
        break;
      }
      default: {
        value_set_str(&v, s.str);
        break;
      }
    }
    object_array_push(obj, &v);
    if (end != NULL) {
      p = end + strlen(sep);
    }
  } while (end != NULL && *p);

  str_reset(&s);

  return obj;
}

ret_t object_array_join(tk_object_t* obj, const char* sep, str_t* result) {
  str_t s;
  uint32_t i = 0;
  ret_t ret = RET_OK;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL && sep != NULL && result != NULL, RET_BAD_PARAMS);

  str_set(result, "");
  if (o->size < 1) {
    return RET_OK;
  }

  str_init(&s, 50);
  for (i = 0; i < o->size; i++) {
    value_t* iter = o->props + i;
    str_from_value(&s, iter);
    if (i > 0) {
      if (str_append(result, sep) != RET_OK) {
        ret = RET_OOM;
        break;
      }
    }
    if (str_append(result, s.str) != RET_OK) {
      ret = RET_OOM;
      break;
    }
  }
  str_reset(&s);

  return ret;
}

tk_object_t* object_array_dup(tk_object_t* obj, uint32_t start, uint32_t end) {
  uint32_t i = 0;
  tk_object_t* dup = NULL;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, NULL);

  end = tk_min(end, o->size);
  if (start == end || o->size == 0) {
    return object_array_create();
  }

  return_value_if_fail(start < end, NULL);
  dup = object_array_create_with_capacity(end - start);
  return_value_if_fail(dup != NULL, NULL);

  for (i = start; i < end; i++) {
    value_t* iter = o->props + i;
    object_array_push(dup, iter);
  }

  return dup;
}

static int value_cmp_as_int(const void* a, const void* b) {
  return value_int((const value_t*)a) - value_int((const value_t*)b);
}

static int value_cmp_as_int_r(const void* a, const void* b) {
  return value_int((const value_t*)b) - value_int((const value_t*)a);
}

static int value_cmp_as_double(const void* a, const void* b) {
  return value_double((const value_t*)a) - value_double((const value_t*)b);
}

static int value_cmp_as_double_r(const void* a, const void* b) {
  return value_double((const value_t*)b) - value_double((const value_t*)a);
}

static int value_cmp_as_str(const void* a, const void* b) {
  return strcmp(value_str((const value_t*)a), value_str((const value_t*)b));
}

static int value_cmp_as_str_r(const void* a, const void* b) {
  return strcmp(value_str((const value_t*)b), value_str((const value_t*)a));
}

static int value_cmp_as_str_i(const void* a, const void* b) {
  return strcasecmp(value_str((const value_t*)a), value_str((const value_t*)b));
}

static int value_cmp_as_str_i_r(const void* a, const void* b) {
  return strcasecmp(value_str((const value_t*)b), value_str((const value_t*)a));
}

ret_t object_array_sort(tk_object_t* obj, tk_compare_t cmp) {
  object_array_t* o = OBJECT_ARRAY(obj);
  event_t e = event_init(EVT_ITEMS_CHANGED, o);
  return_value_if_fail(obj != NULL && cmp != NULL, RET_BAD_PARAMS);

  qsort(o->props, o->size, sizeof(value_t), cmp);
  emitter_dispatch(EMITTER(o), &e);

  return RET_OK;
}

ret_t object_array_sort_as_int(tk_object_t* obj, bool_t ascending) {
  if (ascending) {
    return object_array_sort(obj, value_cmp_as_int);
  } else {
    return object_array_sort(obj, value_cmp_as_int_r);
  }
}

ret_t object_array_sort_as_double(tk_object_t* obj, bool_t ascending) {
  if (ascending) {
    return object_array_sort(obj, value_cmp_as_double);
  } else {
    return object_array_sort(obj, value_cmp_as_double_r);
  }
}

ret_t object_array_sort_as_str(tk_object_t* obj, bool_t ascending, bool_t ignore_case) {
  if (ascending) {
    if (ignore_case) {
      return object_array_sort(obj, value_cmp_as_str_i);
    } else {
      return object_array_sort(obj, value_cmp_as_str);
    }
  } else {
    if (ignore_case) {
      return object_array_sort(obj, value_cmp_as_str_i_r);
    } else {
      return object_array_sort(obj, value_cmp_as_str_r);
    }
  }
}

ret_t object_array_min(tk_object_t* obj, value_t* result) {
  int32_t i = 0;
  double value = 0;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  value_set_double(result, 0);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (o->size == 0) {
    return RET_OK;
  }
  value = value_double(o->props);
  for (i = 1; i < o->size; i++) {
    double t = value_double(o->props + i);
    if (value > t) {
      value = t;
    }
  }

  value_set_double(result, value);

  return RET_OK;
}

ret_t object_array_max(tk_object_t* obj, value_t* result) {
  int32_t i = 0;
  double value = 0;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  value_set_double(result, 0);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (o->size == 0) {
    return RET_OK;
  }
  value = value_double(o->props);
  for (i = 1; i < o->size; i++) {
    double t = value_double(o->props + i);
    if (value < t) {
      value = t;
    }
  }

  value_set_double(result, value);

  return RET_OK;
}

ret_t object_array_sum(tk_object_t* obj, value_t* result) {
  int32_t i = 0;
  double value = 0;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  value_set_double(result, 0);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  for (i = 0; i < o->size; i++) {
    value += value_double(o->props + i);
  }

  value_set_double(result, value);

  return RET_OK;
}

ret_t object_array_avg(tk_object_t* obj, value_t* result) {
  int32_t i = 0;
  double value = 0;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);
  value_set_double(result, 0);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->size > 0) {
    for (i = 0; i < o->size; i++) {
      value += value_double(o->props + i);
    }
    value /= o->size;
  }

  value_set_double(result, value);

  return RET_OK;
}

ret_t object_array_reverse(tk_object_t* obj) {
  uint32_t start = 0;
  uint32_t end = 0;
  object_array_t* o = OBJECT_ARRAY(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  if (o->size < 2) {
    return RET_OK;
  }

  start = 0;
  end = o->size - 1;

  while (start < end) {
    value_t v = o->props[start];
    o->props[start] = o->props[end];
    o->props[end] = v;
    start++;
    end--;
  }

  return RET_OK;
}
