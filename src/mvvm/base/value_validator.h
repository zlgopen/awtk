/**
 * File:   value_validator.h
 * Author: AWTK Develop Team
 * Brief:  value validator
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
 * 2019-01-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VALUE_VALIDATOR_H
#define TK_VALUE_VALIDATOR_H

#include "tkc/str.h"
#include "tkc/object.h"

BEGIN_C_DECLS

struct _value_validator_t;
typedef struct _value_validator_t value_validator_t;

typedef ret_t (*value_validator_is_valid_t)(value_validator_t* validator, const value_t* value,
                                            str_t* msg);

/**
 * @class value_validator_t
 * @parent object_t
 * @annotation ["scriptable"]
 *
 * 值校验器。
 *
 */
struct _value_validator_t {
  object_t object;

  /*private*/
  value_validator_is_valid_t is_valid;
};

/**
 * @method value_validator_is_valid
 * 检查值是否有效。
 *
 * @annotation ["scriptable"]
 * @param {value_validator_t*} validator validator对象。
 * @param {value_t*} value 待验证的值。
 * @param {str_t*} str 返回错误信息。
 *
 * @return {bool_t} 返回TRUE表示有效，否则表示无效。
 */
bool_t value_validator_is_valid(value_validator_t* validator, const value_t* value, str_t* msg);

/**
 * @method value_validator_create
 * 创建指定名称的值校验器。
 *
 * @annotation ["scriptable", "static"]
 * @param {const char*} name 名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
value_validator_t* value_validator_create(const char* name);

/**
 * @method value_validator_register
 *
 * 注册值校验器的创建函数。
 * @annotation ["scriptable", "static"]
 * @param {const char*} name 名称。
 * @param {tk_create_t} create 创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_validator_register(const char* name, tk_create_t create);

/**
 * @method value_validator_init
 *
 * 初始化值校验器的全局对象。
 * @annotation ["scriptable", "static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_validator_init(void);

/**
 * @method value_validator_deinit
 *
 * 释放值校验器的全局对象。
 * @annotation ["scriptable", "static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_validator_deinit(void);

#define VALUE_VALIDATOR(validator) ((value_validator_t*)(validator))

END_C_DECLS

#endif /*TK_VALUE_VALIDATOR_H*/
