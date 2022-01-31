/**
 * File:   object_locker.c
 * Author: AWTK Develop Team
 * Brief:  decorate object with lock
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

static ret_t object_locker_on_destroy(tk_object_t* obj) {
  object_locker_t* o = OBJECT_LOCKER(obj);
  return_value_if_fail(o != NULL && o->mutex != NULL, RET_BAD_PARAMS);
  tk_mutex_nest_destroy(o->mutex);
  emitter_off_by_ctx(EMITTER(o->obj), o);
  TK_OBJECT_UNREF(o->obj);

  return RET_OK;
}

static int32_t object_locker_compare(tk_object_t* obj, tk_object_t* other) {
  object_locker_t* o = OBJECT_LOCKER(obj);
  return_value_if_fail(o != NULL && o->obj != NULL, -1);
  return tk_object_compare(o->obj, other);
}

static ret_t object_locker_remove_prop(tk_object_t* obj, const char* name) {
  ret_t ret = RET_NOT_FOUND;
  object_locker_t* o = OBJECT_LOCKER(obj);
  return_value_if_fail(o != NULL && o->mutex != NULL, RET_BAD_PARAMS);
  if (tk_mutex_nest_lock(o->mutex) == RET_OK) {
    ret = tk_object_remove_prop(o->obj, name);
    tk_mutex_nest_unlock(o->mutex);
  }

  return ret;
}

static ret_t object_locker_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_locker_t* o = OBJECT_LOCKER(obj);
  return_value_if_fail(o != NULL && o->mutex != NULL, RET_BAD_PARAMS);
  if (tk_mutex_nest_lock(o->mutex) == RET_OK) {
    ret = tk_object_set_prop(o->obj, name, v);
    tk_mutex_nest_unlock(o->mutex);
  }

  return ret;
}

static ret_t object_locker_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_locker_t* o = OBJECT_LOCKER(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (tk_mutex_nest_lock(o->mutex) == RET_OK) {
    ret = tk_object_get_prop(o->obj, name, v);
    tk_mutex_nest_unlock(o->mutex);
  }

  return ret;
}

static ret_t object_locker_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_OK;
  object_locker_t* o = OBJECT_LOCKER(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (tk_mutex_nest_lock(o->mutex) == RET_OK) {
    ret = tk_object_foreach_prop(o->obj, on_prop, ctx);
    tk_mutex_nest_unlock(o->mutex);
  }

  return ret;
}

static bool_t object_locker_can_exec(tk_object_t* obj, const char* name, const char* args) {
  bool_t ret = RET_OK;
  object_locker_t* o = OBJECT_LOCKER(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (tk_mutex_nest_lock(o->mutex) == RET_OK) {
    ret = tk_object_can_exec(o->obj, name, args);
    tk_mutex_nest_unlock(o->mutex);
  }

  return ret;
}

static ret_t object_locker_exec(tk_object_t* obj, const char* name, const char* args) {
  ret_t ret = RET_OK;
  object_locker_t* o = OBJECT_LOCKER(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (tk_str_eq(name, "lock")) {
    return tk_mutex_nest_lock(o->mutex);
  } else if (tk_str_eq(name, "unlock")) {
    return tk_mutex_nest_unlock(o->mutex);
  } else if (tk_mutex_nest_lock(o->mutex) == RET_OK) {
    ret = tk_object_exec(o->obj, name, args);
    tk_mutex_nest_unlock(o->mutex);
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

static ret_t object_locker_forward_events(void* ctx, event_t* e) {
  object_locker_t* o = OBJECT_LOCKER(ctx);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  emitter_dispatch_simple_event(EMITTER(o), e->type);

  return RET_OK;
}

tk_object_t* object_locker_create(tk_object_t* obj) {
  tk_object_t* o = NULL;
  object_locker_t* wrapper = NULL;
  return_value_if_fail(obj != NULL, NULL);
  o = tk_object_create(&s_object_locker_vtable);
  return_value_if_fail(o != NULL, NULL);
  wrapper = OBJECT_LOCKER(o);
  return_value_if_fail(wrapper != NULL, NULL);

  wrapper->mutex = tk_mutex_nest_create();
  if (wrapper->mutex != NULL) {
    wrapper->obj = tk_object_ref(obj);
    emitter_on(EMITTER(obj), EVT_ITEMS_CHANGED, object_locker_forward_events, o);
    emitter_on(EMITTER(obj), EVT_PROPS_CHANGED, object_locker_forward_events, o);
  }

  return o;
}

object_locker_t* object_locker_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_locker_vtable, NULL);

  return (object_locker_t*)(obj);
}
