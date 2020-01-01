/**
 * File:   func_desc.h
 * Author: AWTK Develop Team
 * Brief:  func_desc
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-08-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FUNC_DESC_H
#define TK_FUNC_DESC_H

#include "tkc/value_desc.h"

BEGIN_C_DECLS

typedef ret_t (*func_exec_t)(void* obj, value_t* ret, object_t* args);

/**
 * @class func_desc_t
 * 函数描述。
 */
typedef struct _func_desc_t {
  /**
   * @property {const char*} name
   * @annotation ["readable"]
   * 名称。
   */
  const char* name;
  /**
   * @property {const char*} desc
   * @annotation ["readable"]
   * 描述。
   */
  const char* desc;
  /**
   * @property {func_exec_t} exec
   * @annotation ["readable"]
   * 函数指针。
   */
  func_exec_t exec;
  /**
   * @property {arg_desc_t**} args
   * @annotation ["readable"]
   * 函数参数描述。
   */
  const arg_desc_t** args;
  /**
   * @property {value_desc_t*} return_value
   * @annotation ["readable"]
   * 函数返回值描述。
   */
  const value_desc_t* return_value;
} func_desc_t;

END_C_DECLS

#endif /*TK_FUNC_DESC_H*/
