/**
 * File:   navigator_handler.h
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

#ifndef TK_NAVIGATOR_HANDLER_H
#define TK_NAVIGATOR_HANDLER_H

#include "mvvm/base/navigator_request.h"

BEGIN_C_DECLS

struct _navigator_handler_t;
typedef struct _navigator_handler_t navigator_handler_t;

typedef ret_t (*navigator_handler_on_request_t)(navigator_handler_t* handler,
                                                navigator_request_t* req);

/**
 * @class navigator_handler_t
 * @parent object_t
 * @annotation ["scriptable"]
 *
 * 处理导航请求
 *
 */
struct _navigator_handler_t {
  object_t object;

  /*private*/
  navigator_handler_on_request_t on_request;
};

/**
 * @method navigator_handler_create
 * 创建handler对象。
 *
 * @annotation ["scriptable:custom", "constructor"]
 *
 * @param {const char*} target 目标窗口的名称。
 * @param {navigator_handler_on_request_t} on_request 用于非模态窗口返回结果的回调函数。
 *
 * @return {ret_t} 返回handler对象。
 */
navigator_handler_t* navigator_handler_create(navigator_handler_on_request_t on_request);

/**
 * @method navigator_handler_on_request
 * 调用本函数处理请求。
 *
 * @param {navigator_handler_t*} handler handler对象。
 * @param {navigator_request_t*} req 处理请求。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_handler_on_request(navigator_handler_t* handler, navigator_request_t* req);

#define NAVIGATOR_HANDLER(handler) ((navigator_handler_t*)(handler))

#define NAVIGATOR_DEFAULT_HANDLER "default_handler"

END_C_DECLS

#endif /*TK_NAVIGATOR_HANDLER_H*/
