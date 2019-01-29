/**
 * File:   navigator_request.h
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

#ifndef TK_NAVIGATOR_REQUEST_H
#define TK_NAVIGATOR_REQUEST_H

#include "tkc/object.h"

BEGIN_C_DECLS

struct _navigator_request_t;
typedef struct _navigator_request_t navigator_request_t;

typedef ret_t (*navigator_request_on_result_t)(navigator_request_t* req, const value_t* result);

/**
 * @class navigator_request_t
 * @parent object_t
 * @annotation ["scriptable"]
 *
 * 导航请求，请求打开指定的窗口。
 * 如果需要传递参数给要打开的窗口或需要返回结果给调用者时，才需要本类，否则指定窗口的名字就行了。
 *
 */
struct _navigator_request_t {
  object_t object;

  /**
   * @property {char*} target
   * @annotation ["readable"]
   * 目标窗口的名称。
   */
  char target[TK_NAME_LEN + 1];

  /**
   * @property {value_t} result
   * @annotation ["readable"]
   * 用于模态窗口返回结果。
   */
  value_t result;

  /**
   * @property {navigator_request_on_result_t} on_result
   * @annotation ["readable"]
   * 用于非模态窗口返回结果。
   */
  navigator_request_on_result_t on_result;

  void* user_data;
  /*private*/
  object_t* args;
};

/**
 * @method navigator_request_create
 * 创建request对象。
 *
 * @annotation ["scriptable:custom", "constructor"]
 *
 * @param {const char*} target 目标窗口的名称。
 * @param {navigator_request_on_result_t} on_result 用于非模态窗口返回结果的回调函数。
 *
 * @return {ret_t} 返回request对象。
 */
navigator_request_t* navigator_request_create(const char* target,
                                              navigator_request_on_result_t on_result);

/**
 * @method navigator_request_on_result
 * navigator hander调用本函数返回结果。
 *
 * @annotation ["scriptable"]
 *
 * @param {navigator_request_t*} req request对象。
 * @param {const value_t*} result 结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t navigator_request_on_result(navigator_request_t* req, const value_t* result);

#define NAVIGATOR_REQUEST(req) ((navigator_request_t*)(req))

#define NAVIGATOR_REQ_TOAST "toast"
#define NAVIGATOR_REQ_INFO "info"
#define NAVIGATOR_REQ_WARN "warn"
#define NAVIGATOR_REQ_CONFIRM "confirm"
#define NAVIGATOR_REQ_INPUT_TEXT "input_text"
#define NAVIGATOR_REQ_INPUT_FLOAT "input_float"
#define NAVIGATOR_REQ_INPUT_INT "input_int"
#define NAVIGATOR_REQ_PICK_COLOR "pick_color"
#define NAVIGATOR_REQ_PICK_FILE "pick_file"
#define NAVIGATOR_REQ_PICK_DIR "pick_dir"

#define NAVIGATOR_ARG_TARGET "target"

#define NAVIGATOR_ARG_MIN "min"
#define NAVIGATOR_ARG_MAX "max"
#define NAVIGATOR_ARG_TITLE "title"
#define NAVIGATOR_ARG_TIPS "tips"
#define NAVIGATOR_ARG_DEFAULT "default"
#define NAVIGATOR_ARG_TIMEOUT "timeout"
#define NAVIGATOR_ARG_CONTENT "content"
#define NAVIGATOR_ARG_FOR_SAVE "for_save"
#define NAVIGATOR_ARG_MINE_TYPES "mime_types"

END_C_DECLS

#endif /*TK_NAVIGATOR_REQUEST_H*/
