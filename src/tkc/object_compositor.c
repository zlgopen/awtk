/**
 * File:   object_compositor.c
 * Author: AWTK Develop Team
 * Brief:  compositor two objects into one
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-06-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/value.h"
#include "tkc/utils.h"
#include "tkc/object_compositor.h"

static ret_t object_compositor_on_destroy(tk_object_t* obj) {
  emitter_t* obj1 = NULL;
  emitter_t* obj2 = NULL;
  object_compositor_t* o = OBJECT_COMPOSITOR(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  obj1 = EMITTER(o->obj1);
  obj2 = EMITTER(o->obj2);
  return_value_if_fail(obj1 != NULL && obj2 != NULL, RET_BAD_PARAMS);

  emitter_off_by_ctx(obj1, o);
  emitter_off_by_ctx(obj2, o);
  TK_OBJECT_UNREF(o->obj1);
  TK_OBJECT_UNREF(o->obj2);

  return RET_OK;
}

static ret_t object_compositor_remove_prop(tk_object_t* obj, const char* name) {
  object_compositor_t* o = OBJECT_COMPOSITOR(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (tk_object_remove_prop(o->obj1, name) == RET_OK) {
    return RET_OK;
  }

  return tk_object_remove_prop(o->obj2, name);
}

static ret_t object_compositor_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  object_compositor_t* o = OBJECT_COMPOSITOR(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (tk_object_set_prop(o->obj1, name, v) == RET_OK) {
    return RET_OK;
  }

  return tk_object_set_prop(o->obj2, name, v);
}

static ret_t object_compositor_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  object_compositor_t* o = OBJECT_COMPOSITOR(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (tk_object_get_prop(o->obj1, name, v) == RET_OK) {
    return RET_OK;
  }

  return tk_object_get_prop(o->obj2, name, v);
}

static ret_t object_compositor_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_OK;
  uint32_t i = 0;
  tk_object_t* objs[2] = {NULL};
  object_compositor_t* o = OBJECT_COMPOSITOR(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  objs[0] = o->obj1;
  objs[1] = o->obj2;

  for (i = 0; i < ARRAY_SIZE(objs); i++) {
    ret = tk_object_foreach_prop(objs[i], on_prop, ctx);
    TK_FOREACH_VISIT_RESULT_PROCESSING(
        ret, log_warn("%s: result type REMOVE is not supported!\n", __FUNCTION__));
  }

  return ret;
}

static bool_t object_compositor_can_exec(tk_object_t* obj, const char* name, const char* args) {
  object_compositor_t* o = OBJECT_COMPOSITOR(obj);
  return_value_if_fail(o != NULL, FALSE);
  if (tk_object_can_exec(o->obj1, name, args)) {
    return TRUE;
  }

  return tk_object_can_exec(o->obj2, name, args);
}

static ret_t object_compositor_exec(tk_object_t* obj, const char* name, const char* args) {
  object_compositor_t* o = OBJECT_COMPOSITOR(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  if (tk_object_exec(o->obj1, name, args) == RET_OK) {
    return RET_OK;
  }

  return tk_object_exec(o->obj2, name, args);
}

static const object_vtable_t s_object_compositor_vtable = {
    .type = OBJECT_COMPOSITOR_TYPE,
    .desc = OBJECT_COMPOSITOR_TYPE,
    .size = sizeof(object_compositor_t),
    .is_collection = FALSE,
    .on_destroy = object_compositor_on_destroy,
    .exec = object_compositor_exec,
    .can_exec = object_compositor_can_exec,
    .compare = tk_object_compare_name_without_nullptr,
    .get_prop = object_compositor_get_prop,
    .set_prop = object_compositor_set_prop,
    .remove_prop = object_compositor_remove_prop,
    .foreach_prop = object_compositor_foreach_prop};

static ret_t object_compositor_forward_events(void* ctx, event_t* e) {
  object_compositor_t* o = OBJECT_COMPOSITOR(ctx);

  emitter_dispatch_simple_event(EMITTER(o), e->type);

  return RET_OK;
}

tk_object_t* object_compositor_create(tk_object_t* obj1, tk_object_t* obj2) {
  tk_object_t* o = NULL;
  object_compositor_t* wrapper = NULL;
  return_value_if_fail(obj1 != NULL && obj2 != NULL, NULL);

  o = tk_object_create(&s_object_compositor_vtable);
  wrapper = OBJECT_COMPOSITOR(o);
  return_value_if_fail(wrapper != NULL, NULL);

  wrapper->obj1 = tk_object_ref(obj1);
  emitter_on(EMITTER(obj1), EVT_ITEMS_CHANGED, object_compositor_forward_events, o);
  emitter_on(EMITTER(obj1), EVT_PROPS_CHANGED, object_compositor_forward_events, o);

  wrapper->obj2 = tk_object_ref(obj2);
  emitter_on(EMITTER(obj2), EVT_ITEMS_CHANGED, object_compositor_forward_events, o);
  emitter_on(EMITTER(obj2), EVT_PROPS_CHANGED, object_compositor_forward_events, o);

  return o;
}

object_compositor_t* object_compositor_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_compositor_vtable, NULL);

  return (object_compositor_t*)(obj);
}
