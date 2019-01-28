/**
 * File:   value_converter_delegate.c
 * Author: AWTK Develop Team
 * Brief:  value converter delegate
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

#include "mvvm/base/value_converter_delegate.h"

static const object_vtable_t s_value_converter_delegate_vtable = {
    .type = "value_converter_delegate",
    .desc = "value_converter_delegate",
    .size = sizeof(value_converter_delegate_t),
    .is_collection = FALSE};

static ret_t value_converter_delegate_to_view(value_converter_t* c, const value_t* from,
                                              value_t* to) {
  value_converter_delegate_t* value_convert_delegate = VALUE_CONVERTER_DELEGATE(c);

  return value_convert_delegate->to_view(from, to);
}

static ret_t value_converter_delegate_to_model(value_converter_t* c, const value_t* from,
                                               value_t* to) {
  value_converter_delegate_t* value_convert_delegate = VALUE_CONVERTER_DELEGATE(c);

  return value_convert_delegate->to_model(from, to);
}

value_converter_t* value_converter_delegate_create(value_convert_t to_model,
                                                   value_convert_t to_view) {
  object_t* obj = NULL;
  value_converter_t* value_convert = NULL;
  value_converter_delegate_t* value_convert_delegate = NULL;
  return_value_if_fail(to_model != NULL && to_view != NULL, NULL);

  obj = object_create(&s_value_converter_delegate_vtable);
  return_value_if_fail(obj != NULL, NULL);

  value_convert = VALUE_CONVERTER(obj);
  value_convert->to_view = value_converter_delegate_to_view;
  value_convert->to_model = value_converter_delegate_to_model;

  value_convert_delegate = VALUE_CONVERTER_DELEGATE(obj);
  value_convert_delegate->to_view = to_view;
  value_convert_delegate->to_model = to_model;

  return value_convert;
}
