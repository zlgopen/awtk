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
 * 值合法性检查。
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

#define VALUE_VALIDATOR(validator) ((value_validator_t*)(validator))

END_C_DECLS

#endif /*TK_VALUE_VALIDATOR_H*/
