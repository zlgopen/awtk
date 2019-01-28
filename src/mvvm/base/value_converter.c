/**
 * File:   value_converter.c
 * Author: AWTK Develop Team
 * Brief:  value converter
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
#include "mvvm/base/value_converter.h"

ret_t value_converter_to_view(value_converter_t* converter, const value_t* from, value_t* to) {
  return_value_if_fail(converter != NULL && converter->object.vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(converter->to_view != NULL && converter->object.ref_count > 0,
                       RET_BAD_PARAMS);
  return_value_if_fail(from != NULL && to != NULL, RET_BAD_PARAMS);

  return converter->to_view(converter, from, to);
}

ret_t value_converter_to_model(value_converter_t* converter, const value_t* from, value_t* to) {
  return_value_if_fail(converter != NULL && converter->object.vt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(converter->to_model != NULL && converter->object.ref_count > 0,
                       RET_BAD_PARAMS);
  return_value_if_fail(from != NULL && to != NULL, RET_BAD_PARAMS);

  return converter->to_model(converter, from, to);
}

static object_t* s_value_converter_creators;

value_converter_t* value_converter_create(const char* name) {
  tk_create_t create = NULL;
  return_value_if_fail(name != NULL, NULL);
  return_value_if_fail(value_converter_init() == RET_OK, NULL);

  create = (tk_create_t)object_get_prop_pointer(s_value_converter_creators, name);
  return_value_if_fail(create != NULL, NULL);

  return (value_converter_t*)create();
}

ret_t value_converter_register(const char* name, tk_create_t create) {
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(create != NULL, RET_BAD_PARAMS);
  return_value_if_fail(value_converter_init() == RET_OK, RET_BAD_PARAMS);

  return object_set_prop_pointer(s_value_converter_creators, name, create);
}

ret_t value_converter_init(void) {
  if (s_value_converter_creators == NULL) {
    s_value_converter_creators = object_default_create();
  }

  return s_value_converter_creators != NULL ? RET_OK : RET_FAIL;
}

ret_t value_converter_deinit(void) {
  return_value_if_fail(s_value_converter_creators != NULL, RET_BAD_PARAMS);

  object_unref(s_value_converter_creators);
  s_value_converter_creators = NULL;

  return RET_OK;
}
