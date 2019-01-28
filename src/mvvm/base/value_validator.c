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

#include "tkc/object_default.h"
#include "mvvm/base/value_validator.h"

bool_t value_validator_is_valid(value_validator_t* validator, const value_t* value, str_t* msg) {
  return_value_if_fail(validator != NULL && validator->object.vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(validator->is_valid != NULL && validator->object.ref_count > 0,
                       RET_BAD_PARAMS);
  return_value_if_fail(value != NULL && msg != NULL, RET_BAD_PARAMS);

  return validator->is_valid(validator, value, msg);
}

static object_t* s_value_validator_creators;

value_validator_t* value_validator_create(const char* name) {
  tk_create_t create = NULL;
  return_value_if_fail(name != NULL, NULL);
  return_value_if_fail(value_validator_init() == RET_OK, NULL);

  create = (tk_create_t)object_get_prop_pointer(s_value_validator_creators, name);
  return_value_if_fail(create != NULL, NULL);

  return (value_validator_t*)create();
}

ret_t value_validator_register(const char* name, tk_create_t create) {
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(create != NULL, RET_BAD_PARAMS);
  return_value_if_fail(value_validator_init() == RET_OK, RET_BAD_PARAMS);

  return object_set_prop_pointer(s_value_validator_creators, name, create);
}

ret_t value_validator_init(void) {
  if (s_value_validator_creators == NULL) {
    s_value_validator_creators = object_default_create();
  }

  return s_value_validator_creators != NULL ? RET_OK : RET_FAIL;
}

ret_t value_validator_deinit(void) {
  return_value_if_fail(s_value_validator_creators != NULL, RET_BAD_PARAMS);

  object_unref(s_value_validator_creators);
  s_value_validator_creators = NULL;

  return RET_OK;
}
