/**
 * File:   navigator_handler.c
 * Author: AWTK Develop Team
 * Brief:  navigator handler
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
#include "mvvm/base/navigator_handler.h"

static const object_vtable_t s_navigator_handler_vtable = {
    .type = "navigator_handler",
    .desc = "navigator_handler",
    .size = sizeof(navigator_handler_t),
    .is_collection = FALSE,
};

navigator_handler_t* navigator_handler_create(navigator_handler_on_request_t on_request) {
  object_t* obj = NULL;
  navigator_handler_t* handler = NULL;
  return_value_if_fail(on_request != NULL, NULL);

  obj = object_create(&s_navigator_handler_vtable);
  handler = NAVIGATOR_HANDLER(obj);

  handler->on_request = on_request;

  return handler;
}

ret_t navigator_handler_on_request(navigator_handler_t* handler, navigator_request_t* req) {
  return_value_if_fail(handler != NULL && req != NULL, RET_BAD_PARAMS);

  return handler->on_request(handler, req);
}
