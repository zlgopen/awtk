/**
 * File:   object_locker.c
 * Author: AWTK Develop Team
 * Brief:  decorate object with lock
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-06-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/value.h"
#include "tkc/utils.h"
#include "tkc/object_locker.h"

static ret_t object_locker_on_destroy(object_t* obj) {
  object_locker_t* o = OBJECT_LOCKER(obj);
  tk_mutex_destroy(o->mutex);
  OBJECT_UNREF(o->obj);

  return RET_OK;
}

static int32_t object_locker_compare(object_t* obj, object_t* other) {
  object_locker_t* o = OBJECT_LOCKER(obj);

  return object_compare(o->obj, other);
}

static ret_t object_locker_remove_prop(object_t* obj, const char* name) {
  ret_t ret = RET_NOT_FOUND;
  object_locker_t* o = OBJECT_LOCKER(obj);

  if (tk_mutex_lock(o->mutex) == RET_OK) {
    ret = object_remove_prop(o->obj, name);
    tk_mutex_unlock(o->mutex);
  }

  return ret;
}

static ret_t object_locker_set_prop(object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_locker_t* o = OBJECT_LOCKER(obj);

  if (tk_mutex_lock(o->mutex) == RET_OK) {
    ret = object_set_prop(o->obj, name, v);
    tk_mutex_unlock(o->mutex);
  }

  return ret;
}

static ret_t object_locker_get_prop(object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_locker_t* o = OBJECT_LOCKER(obj);

  if (tk_mutex_lock(o->mutex) == RET_OK) {
    ret = object_get_prop(o->obj, name, v);
    tk_mutex_unlock(o->mutex);
  }

  return ret;
}

static ret_t object_locker_foreach_prop(object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_OK;
  object_locker_t* o = OBJECT_LOCKER(obj);

  if (tk_mutex_lock(o->mutex) == RET_OK) {
    ret = object_foreach_prop(o->obj, on_prop, ctx);
    tk_mutex_unlock(o->mutex);
  }

  return ret;
}

static bool_t object_locker_can_exec(object_t* obj, const char* name, const char* args) {
  bool_t ret = RET_OK;
  object_locker_t* o = OBJECT_LOCKER(obj);

  if (tk_mutex_lock(o->mutex) == RET_OK) {
    ret = object_can_exec(o->obj, name, args);
    tk_mutex_unlock(o->mutex);
  }

  return ret;
}

static ret_t object_locker_exec(object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_OK;
  object_locker_t* o = OBJECT_LOCKER(obj);

  if (tk_str_eq(name, "lock")) {
    return tk_mutex_lock(o->mutex);
  } else if (tk_str_eq(name, "unlock")) {
    return tk_mutex_unlock(o->mutex);
  } else if (tk_mutex_lock(o->mutex) == RET_OK) {
    ret = object_exec(o->obj, name, args);
    tk_mutex_unlock(o->mutex);
  }

  return ret;
}

static const object_vtable_t s_object_locker_vtable = {.type = "object_locker",
                                                       .desc = "object_locker",
                                                       .size = sizeof(object_locker_t),
                                                       .is_collection = FALSE,
                                                       .on_destroy = object_locker_on_destroy,
                                                       .exec = object_locker_exec,
                                                       .can_exec = object_locker_can_exec,
                                                       .compare = object_locker_compare,
                                                       .get_prop = object_locker_get_prop,
                                                       .set_prop = object_locker_set_prop,
                                                       .remove_prop = object_locker_remove_prop,
                                                       .foreach_prop = object_locker_foreach_prop};

object_t* object_locker_create(object_t* obj) {
  object_t* o = NULL;
  object_locker_t* wrapper = NULL;
  return_value_if_fail(obj != NULL, NULL);
  o = object_create(&s_object_locker_vtable);

  wrapper = OBJECT_LOCKER(o);
  return_value_if_fail(wrapper != NULL, NULL);

  wrapper->mutex = tk_mutex_create();
  if (wrapper->mutex != NULL) {
    wrapper->obj = object_ref(obj);
  } else {
    OBJECT_UNREF(wrapper->obj);
  }

  return o;
}

object_locker_t* object_locker_cast(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_locker_vtable, NULL);

  return (object_locker_t*)(obj);
}
