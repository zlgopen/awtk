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

#include "mvvm/value_converter.h"

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
