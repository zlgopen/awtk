/**
 * File:   value_converter_delegate.h
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

#ifndef TK_VALUE_CONVERTER_DELEGATE_H
#define TK_VALUE_CONVERTER_DELEGATE_H

#include "mvvm/base/value_converter.h"

BEGIN_C_DECLS

typedef ret_t (*value_convert_t)(const value_t* from, value_t* to);

/**
 * @class value_converter_delegate_t
 * @parent value_converter_t
 *
 * 把两个转换函数包装成value_converter_t接口。
 *
 */
typedef struct _value_converter_delegate_t {
  value_converter_t value_converter;

  /*private*/
  value_convert_t to_model;
  value_convert_t to_view;
} value_converter_delegate_t;

/**
 * @method value_converter_delegate_create
 *
 * @annotation ["constructor"]
 * @param {value_convert_t} to_model 到模型的转换函数。
 * @param {value_convert_t} to_view 到视图的转换函数。
 *
 * @return {value_converter_t*} 返回value_converter对象。
 */
value_converter_t* value_converter_delegate_create(value_convert_t to_model,
                                                   value_convert_t to_view);

#define VALUE_CONVERTER_DELEGATE(converter) ((value_converter_delegate_t*)(converter))

END_C_DECLS

#endif /*TK_VALUE_CONVERTER_DELEGATE_H*/
