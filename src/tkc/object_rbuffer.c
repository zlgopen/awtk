/**
 * File:   object_rbuffer.c
 * Author: AWTK Develop Team
 * Brief:  wrap rbuffer to an object.
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-01-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/value.h"
#include "tkc/utils.h"
#include "object_rbuffer.h"

static ret_t object_rbuffer_on_destroy(object_t* obj) {
  return RET_OK;
}

static ret_t object_rbuffer_set_prop(object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t object_rbuffer_get_prop(object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_rbuffer_t* o = OBJECT_RBUFFER(obj);
  return_value_if_fail(o != NULL && o->rbuffer != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, "cursor")) {
    value_set_uint32(v, o->rbuffer->cursor);
    ret = RET_OK;
  } else if (tk_str_eq(name, "capacity")) {
    value_set_uint32(v, o->rbuffer->capacity);
    ret = RET_OK;
  } else if (tk_str_eq(name, "data")) {
    value_set_pointer(v, (void*)(o->rbuffer->data));
    ret = RET_OK;
  }

  return ret;
}

static const object_vtable_t s_object_rbuffer_vtable = {.type = "object_rbuffer",
                                                        .desc = "object_rbuffer",
                                                        .size = sizeof(object_rbuffer_t),
                                                        .is_collection = FALSE,
                                                        .on_destroy = object_rbuffer_on_destroy,
                                                        .get_prop = object_rbuffer_get_prop,
                                                        .set_prop = object_rbuffer_set_prop};

object_t* object_rbuffer_create(const uint8_t* data, uint32_t capacity) {
  object_t* o = NULL;
  object_rbuffer_t* wrapper = NULL;
  o = object_create(&s_object_rbuffer_vtable);
  return_value_if_fail(o != NULL, NULL);

  wrapper = OBJECT_RBUFFER(o);
  return_value_if_fail(wrapper != NULL, NULL);

  wrapper->rbuffer = rbuffer_init(&(wrapper->arbuffer), data, capacity);

  return o;
}

object_rbuffer_t* object_rbuffer_cast(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_rbuffer_vtable, NULL);

  return (object_rbuffer_t*)(obj);
}
