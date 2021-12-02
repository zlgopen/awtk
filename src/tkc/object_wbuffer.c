/**
 * File:   object_wbuffer.c
 * Author: AWTK Develop Team
 * Brief:  wrap wbuffer to an object.
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
#include "object_wbuffer.h"

static ret_t object_wbuffer_on_destroy(tk_object_t* obj) {
  object_wbuffer_t* o = OBJECT_WBUFFER(obj);
  return_value_if_fail(o != NULL && o->wbuffer != NULL, RET_BAD_PARAMS);

  wbuffer_deinit(o->wbuffer);
  o->wbuffer = NULL;

  return RET_OK;
}

static ret_t object_wbuffer_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t object_wbuffer_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_wbuffer_t* o = OBJECT_WBUFFER(obj);
  return_value_if_fail(o != NULL && o->wbuffer != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, "cursor")) {
    value_set_uint32(v, o->wbuffer->cursor);
    ret = RET_OK;
  } else if (tk_str_eq(name, "capacity")) {
    value_set_uint32(v, o->wbuffer->capacity);
    ret = RET_OK;
  } else if (tk_str_eq(name, "data")) {
    value_set_pointer(v, o->wbuffer->data);
    ret = RET_OK;
  }

  return ret;
}

static const object_vtable_t s_object_wbuffer_vtable = {.type = "object_wbuffer",
                                                        .desc = "object_wbuffer",
                                                        .size = sizeof(object_wbuffer_t),
                                                        .is_collection = FALSE,
                                                        .on_destroy = object_wbuffer_on_destroy,
                                                        .get_prop = object_wbuffer_get_prop,
                                                        .set_prop = object_wbuffer_set_prop};

tk_object_t* object_wbuffer_create_ex(bool_t extendable, uint8_t* data, uint32_t capacity) {
  tk_object_t* o = NULL;
  object_wbuffer_t* wrapper = NULL;
  o = tk_object_create(&s_object_wbuffer_vtable);
  return_value_if_fail(o != NULL, NULL);

  wrapper = OBJECT_WBUFFER(o);
  return_value_if_fail(wrapper != NULL, NULL);

  if (extendable) {
    wrapper->wbuffer = wbuffer_init_extendable(&(wrapper->awbuffer));
  } else {
    wrapper->wbuffer = wbuffer_init(&(wrapper->awbuffer), data, capacity);
  }

  return o;
}

tk_object_t* object_wbuffer_create(uint8_t* data, uint32_t capacity) {
  return_value_if_fail(data != NULL, NULL);

  return object_wbuffer_create_ex(FALSE, data, capacity);
}

tk_object_t* object_wbuffer_create_extendable(void) {
  return object_wbuffer_create_ex(TRUE, NULL, 0);
}

object_wbuffer_t* object_wbuffer_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_wbuffer_vtable, NULL);

  return (object_wbuffer_t*)(obj);
}
