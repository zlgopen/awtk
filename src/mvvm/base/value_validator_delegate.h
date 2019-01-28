/**
 * File:   value_validator_delegate.h
 * Author: AWTK Develop Team
 * Brief:  value validator delegate
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

#ifndef TK_VALUE_VALIDATOR_DELEGATE_H
#define TK_VALUE_VALIDATOR_DELEGATE_H

#include "mvvm/base/value_validator.h"

BEGIN_C_DECLS

typedef bool_t (*value_is_valid_t)(const value_t* value, str_t* msg);

/**
 * @class value_validator_delegate_t
 * @parent value_validator_t
 *
 * 把有效性检查函数包装成value_validator_t接口。
 *
 */
typedef struct _value_validator_delegate_t {
  value_validator_t value_validator;

  /*private*/
  value_is_valid_t is_valid;
} value_validator_delegate_t;

/**
 * @method value_validator_delegate_create
 *
 * 创建value_validator对象。
 *
 * @annotation ["constructor"]
 * @param {value_is_valid_t} is_valid 有效性检查函数。
 *
 * @return {value_validator_t*} 返回value_validator对象。
 */
value_validator_t* value_validator_delegate_create(value_is_valid_t is_valid);

#define VALUE_VALIDATOR_DELEGATE(validator) ((value_validator_delegate_t*)(validator))

END_C_DECLS

#endif /*TK_VALUE_VALIDATOR_DELEGATE_H*/
