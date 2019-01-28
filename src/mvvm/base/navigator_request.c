/**
 * File:   navigator_request.c
 * Author: AWTK Develop Team
 * Brief:  navigator request
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
 * 2019-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/value.h"
#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/navigator_request.h"

static ret_t navigator_request_on_destroy(object_t* obj) {
  navigator_request_t* req = NAVIGATOR_REQUEST(obj);

  value_reset(&(req->result));

  return RET_OK;
}

static int32_t navigator_request_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t navigator_request_set_prop(object_t* obj, const char* name, const value_t* v) {
  navigator_request_t* req = NAVIGATOR_REQUEST(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return object_set_prop(req->args, name, v);
}

static ret_t navigator_request_get_prop(object_t* obj, const char* name, value_t* v) {
  navigator_request_t* req = NAVIGATOR_REQUEST(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return object_get_prop(req->args, name, v);
}

static const object_vtable_t s_navigator_request_vtable = {
    .type = "navigator_request",
    .desc = "navigator_request",
    .size = sizeof(navigator_request_t),
    .is_collection = FALSE,
    .on_destroy = navigator_request_on_destroy,

    .compare = navigator_request_compare,
    .get_prop = navigator_request_get_prop,
    .set_prop = navigator_request_set_prop};

navigator_request_t* navigator_request_create(const char* target,
                                              navigator_request_on_result_t on_result) {
  object_t* obj = NULL;
  navigator_request_t* req = NULL;
  return_value_if_fail(target != NULL, NULL);

  obj = object_create(&s_navigator_request_vtable);
  req = NAVIGATOR_REQUEST(obj);

  req->on_result = on_result;
  req->args = object_default_create();
  tk_strncpy(req->target, target, TK_NAME_LEN);

  if (req->args == NULL) {
    object_unref(obj);
    obj = NULL;
  }

  return req;
}

ret_t navigator_request_on_result(navigator_request_t* req, const value_t* result) {
  return_value_if_fail(req != NULL && result != NULL, RET_BAD_PARAMS);

  value_deep_copy(&(req->result), result);
  if (req->on_result != NULL) {
    req->on_result(req, &(req->result));
  }

  return RET_OK;
}
