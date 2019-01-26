/**
 * File:   value_converter.h
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

#ifndef TK_VALUE_CONVERTER_H
#define TK_VALUE_CONVERTER_H

#include "tkc/object.h"

BEGIN_C_DECLS

struct _value_converter_t;
typedef struct _value_converter_t value_converter_t;

typedef ret_t (*value_converter_to_model_t)(value_converter_t* converter, const value_t* from,
                                            value_t* to);
typedef ret_t (*value_converter_to_view_t)(value_converter_t* converter, const value_t* from,
                                           value_t* to);

/**
 * @class value_converter_t
 * @parent object_t
 * @annotation ["scriptable"]
 *
 * 值转换器。
 *
 */
struct _value_converter_t {
  object_t object;

  /*private*/
  value_converter_to_model_t to_model;
  value_converter_to_view_t to_view;
};

/**
 * @method value_converter_to_model
 * 将value转换成适合model存储的格式。
 *
 * @annotation ["scriptable"]
 * @param {value_converter_t*} converter converter对象。
 * @param {value_t*} from 源value。
 * @param {value_t*} to 转换结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_converter_to_model(value_converter_t* converter, const value_t* from, value_t* to);

/**
 * @method value_converter_to_view
 * 将value转换成适合view显示的格式。
 *
 * @annotation ["scriptable"]
 * @param {value_converter_t*} converter converter对象。
 * @param {value_t*} from 源value。
 * @param {value_t*} to 转换结果。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_converter_to_view(value_converter_t* converter, const value_t* from, value_t* to);

/**
 * @method value_converter_create
 * 创建指定名称的值转换器。
 *
 * @annotation ["scriptable", "static"]
 * @param {const char*} name 名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
value_converter_t* value_converter_create(const char* name);

/**
 * @method value_converter_register
 *
 * 注册值转换器的创建函数。
 * @annotation ["scriptable", "static"]
 * @param {const char*} name 名称。
 * @param {tk_create_t} create 创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_converter_register(const char* name, tk_create_t create);

/**
 * @method value_converter_init
 *
 * 初始化值转换器的全局对象。
 * @annotation ["scriptable", "static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_converter_init(void);

/**
 * @method value_converter_deinit
 *
 * 释放值转换器的全局对象。
 * @annotation ["scriptable", "static"]
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_converter_deinit(void);

#define VALUE_CONVERTER(converter) ((value_converter_t*)(converter))

END_C_DECLS

#endif /*TK_VALUE_CONVERTER_H*/
