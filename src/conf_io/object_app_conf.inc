/**
 * File:   object_app_conf.h
 * Author: AWTK Develop Team
 * Brief:  object app conf
 *
 * Copyright (c) 2020 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2024-09-17 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/value.h"
#include "tkc/utils.h"

#include "object_app_conf.h"

static ret_t object_app_conf_on_destroy(tk_object_t* obj) {
  object_app_conf_t* o = OBJECT_APP_CONF(obj);
  return_value_if_fail(o != NULL && o->user_obj != NULL, RET_BAD_PARAMS);
  emitter_off_by_ctx(EMITTER(o->user_obj), o);
  TK_OBJECT_UNREF(o->user_obj);
  TK_OBJECT_UNREF(o->default_obj);

  return RET_OK;
}

static int32_t object_app_conf_compare(tk_object_t* obj, tk_object_t* other) {
  object_app_conf_t* o = OBJECT_APP_CONF(obj);
  return_value_if_fail(o != NULL && o->user_obj != NULL, -1);
  return tk_object_compare(o->user_obj, other);
}

static ret_t object_app_conf_remove_prop(tk_object_t* obj, const char* name) {
  object_app_conf_t* o = OBJECT_APP_CONF(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  
  return tk_object_remove_prop(o->user_obj, name);
}

static ret_t object_app_conf_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  object_app_conf_t* o = OBJECT_APP_CONF(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  return tk_object_set_prop(o->user_obj, name, v);
}

static ret_t object_app_conf_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_app_conf_t* o = OBJECT_APP_CONF(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  ret = tk_object_get_prop(o->user_obj, name, v);
  if (ret != RET_OK) {
    ret = tk_object_get_prop(o->default_obj, name, v);
  }

  return ret;
}

static ret_t object_app_conf_foreach_prop(tk_object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_OK;
  uint32_t i = 0;
  tk_object_t* objs[2] = {NULL};
  object_app_conf_t* o = OBJECT_APP_CONF(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);

  objs[0] = o->user_obj;
  objs[1] = o->default_obj;

  for (i = 0; i < ARRAY_SIZE(objs); i++) {
    ret = tk_object_foreach_prop(objs[i], on_prop, ctx);
    TK_FOREACH_VISIT_RESULT_PROCESSING(
        ret, log_warn("%s: result type REMOVE is not supported!\n", __FUNCTION__));
  }

  return ret;
}

static bool_t object_app_conf_can_exec(tk_object_t* obj, const char* name, const char* args) {
  object_app_conf_t* o = OBJECT_APP_CONF(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  
  return tk_object_can_exec(o->user_obj, name, args);
}

static ret_t object_app_conf_exec(tk_object_t* obj, const char* name, const char* args) {
  object_app_conf_t* o = OBJECT_APP_CONF(obj);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
    
  return tk_object_exec(o->user_obj, name, args);
}

static const object_vtable_t s_object_app_conf_vtable = {.type = OBJECT_APP_CONF_TYPE,
                                                       .desc = OBJECT_APP_CONF_TYPE,
                                                       .size = sizeof(object_app_conf_t),
                                                       .is_collection = FALSE,
                                                       .on_destroy = object_app_conf_on_destroy,
                                                       .exec = object_app_conf_exec,
                                                       .can_exec = object_app_conf_can_exec,
                                                       .compare = object_app_conf_compare,
                                                       .get_prop = object_app_conf_get_prop,
                                                       .set_prop = object_app_conf_set_prop,
                                                       .remove_prop = object_app_conf_remove_prop,
                                                       .foreach_prop = object_app_conf_foreach_prop};

static ret_t object_app_conf_forward_events(void* ctx, event_t* e) {
  object_app_conf_t* o = OBJECT_APP_CONF(ctx);
  return_value_if_fail(o != NULL, RET_BAD_PARAMS);
  emitter_dispatch_simple_event(EMITTER(o), e->type);

  return RET_OK;
}

tk_object_t* object_app_conf_create(tk_object_t* user_obj, tk_object_t* default_obj) {
  tk_object_t* o = NULL;
  object_app_conf_t* wrapper = NULL;
  return_value_if_fail(user_obj != NULL, NULL);
  o = tk_object_create(&s_object_app_conf_vtable);
  return_value_if_fail(o != NULL, NULL);
  wrapper = OBJECT_APP_CONF(o);
  return_value_if_fail(wrapper != NULL, NULL);

  wrapper->user_obj = tk_object_ref(user_obj);
  wrapper->default_obj = tk_object_ref(default_obj);
  emitter_on(EMITTER(user_obj), EVT_ITEMS_CHANGED, object_app_conf_forward_events, o);
  emitter_on(EMITTER(user_obj), EVT_PROPS_CHANGED, object_app_conf_forward_events, o);

  return o;
}

object_app_conf_t* object_app_conf_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_app_conf_vtable, NULL);

  return (object_app_conf_t*)(obj);
}
