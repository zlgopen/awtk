/**
 * File:   value_validator.c
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

#include "mvvm/value_validator.h"

bool_t value_validator_is_valid(value_validator_t* validator, const value_t* value, str_t* msg) {
  return_value_if_fail(validator != NULL && validator->object.vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(validator->is_valid != NULL && validator->object.ref_count > 0,
                       RET_BAD_PARAMS);
  return_value_if_fail(value != NULL && msg != NULL, RET_BAD_PARAMS);

  return validator->is_valid(validator, value, msg);
}
