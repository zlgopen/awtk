/**
 * File:   object.c
 * Author: AWTK Develop Team
 * Brief:  reference count object
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-01-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/event.h"
#include "tkc/object.h"

ret_t object_set_name(object_t* obj, const char* name) {
  ret_t ret = RET_OK;
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);

  obj->name = tk_str_copy(obj->name, name);

  return ret;
}

static ret_t object_destroy(object_t* obj) {
  ret_t ret = RET_OK;
  event_t e = event_init(EVT_DESTROY, obj);
  return_value_if_fail(obj != NULL && obj->vt != NULL, RET_BAD_PARAMS);

  emitter_dispatch((emitter_t*)obj, (event_t*)(&e));

  if (obj->vt->on_destroy != NULL) {
    ret = obj->vt->on_destroy(obj);
  }

  obj->vt = NULL;
  obj->ref_count = 0;
  emitter_deinit((emitter_t*)obj);

  TKMEM_FREE(obj);

  return ret;
}

object_t* object_create(const object_vtable_t* vt) {
  object_t* obj = NULL;
  return_value_if_fail(vt != NULL && vt->size >= sizeof(object_t), NULL);

  obj = (object_t*)TKMEM_ALLOC(vt->size);
  return_value_if_fail(obj != NULL, NULL);

  memset(obj, 0x00, vt->size);

  obj->vt = vt;
  obj->ref_count = 1;
  emitter_init((emitter_t*)obj);

  return obj;
}

ret_t object_unref(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count > 0, RET_BAD_PARAMS);
  return_value_if_fail(!(obj->visiting), RET_BUSY);

  obj->ref_count--;
  if (obj->ref_count < 1) {
    object_destroy(obj);
  }

  return RET_OK;
}

object_t* object_ref(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, NULL);

  obj->ref_count++;

  return obj;
}

ret_t object_get_prop(object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);
  return_value_if_fail(!(obj->visiting), RET_BUSY);

  if (obj->vt->get_prop != NULL) {
    ret = obj->vt->get_prop(obj, name, v);
  }

  return ret;
}

ret_t object_set_prop(object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(name != NULL && v != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);
  return_value_if_fail(!(obj->visiting), RET_BUSY);

  if (obj->vt->set_prop != NULL) {
    prop_change_event_t e;

    e.name = name;
    e.value = v;

    e.e = event_init(EVT_PROP_WILL_CHANGE, obj);
    emitter_dispatch((emitter_t*)obj, (event_t*)(&e));

    ret = obj->vt->set_prop(obj, name, v);

    e.e = event_init(EVT_PROP_CHANGED, obj);
    emitter_dispatch((emitter_t*)obj, (event_t*)(&e));
  }

  return ret;
}

ret_t object_remove_prop(object_t* obj, const char* name) {
  ret_t ret = RET_NOT_FOUND;
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);
  return_value_if_fail(!(obj->visiting), RET_BUSY);

  if (obj->vt->remove_prop != NULL) {
    ret = obj->vt->remove_prop(obj, name);
  }

  return ret;
}

ret_t object_foreach_prop(object_t* obj, tk_visit_t on_prop, void* ctx) {
  ret_t ret = RET_NOT_IMPL;
  return_value_if_fail(on_prop != NULL, RET_BAD_PARAMS);
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, RET_BAD_PARAMS);
  return_value_if_fail(!(obj->visiting), RET_BUSY);

  if (obj->vt->foreach_prop != NULL) {
    obj->visiting = TRUE;
    ret = obj->vt->foreach_prop(obj, on_prop, ctx);
    obj->visiting = FALSE;
  }

  return ret;
}

int32_t object_compare(object_t* obj, object_t* other) {
  int32_t ret = -1;
  return_value_if_fail(obj != NULL && obj->vt != NULL && obj->ref_count >= 0, -1);
  return_value_if_fail(other != NULL && other->vt != NULL && other->ref_count >= 0, -1);

  if (obj->vt->compare != NULL) {
    ret = obj->vt->compare(obj, other);
  }

  return ret;
}
