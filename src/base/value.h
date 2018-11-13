/**
 * File:   value.h
 * Author: AWTK Develop Team
 * Brief:  generic value type
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-27 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "base/types_def.h"

#ifndef VALUE_H
#define VALUE_H

BEGIN_C_DECLS

/**
 * @enum value_type_t
 * @annotation ["scriptable"]
 * 类型常量定义。
 */
typedef enum _value_type_t {
  /**
   * @const VALUE_TYPE_INVALID
   * 无效类型。
   */
  VALUE_TYPE_INVALID = 0,
  /**
   * @const VALUE_TYPE_BOOL
   * BOOL类型。
   */
  VALUE_TYPE_BOOL,
  /**
   * @const VALUE_TYPE_INT8
   * int8_t类型。
   */
  VALUE_TYPE_INT8,
  /**
   * @const VALUE_TYPE_UINT8
   * uint8_t类型。
   */
  VALUE_TYPE_UINT8,
  /**
   * @const VALUE_TYPE_INT16
   * int16_t类型。
   */
  VALUE_TYPE_INT16,
  /**
   * @const VALUE_TYPE_UINT16
   * uint16_t类型。
   */
  VALUE_TYPE_UINT16,
  /**
   * @const VALUE_TYPE_INT32
   * int32_t类型。
   */
  VALUE_TYPE_INT32,
  /**
   * @const VALUE_TYPE_UINT32
   * uint32_t类型。
   */
  VALUE_TYPE_UINT32,
  /**
   * @const VALUE_TYPE_INT64
   * int64_t类型。
   */
  VALUE_TYPE_INT64,
  /**
   * @const VALUE_TYPE_UINT64
   * uint64_t类型。
   */
  VALUE_TYPE_UINT64,
  /**
   * @const VALUE_TYPE_POINTER
   * void*类型。
   */
  VALUE_TYPE_POINTER,
  /**
   * @const VALUE_TYPE_FLOAT
   * float_t类型。
   */
  VALUE_TYPE_FLOAT,
  /**
   * @const VALUE_TYPE_FLOAT32
   * float类型。
   */
  VALUE_TYPE_FLOAT32,
  /**
   * @const VALUE_TYPE_DOUBLE
   * double类型。
   */
  VALUE_TYPE_DOUBLE,
  /**
   * @const VALUE_TYPE_STRING
   * char*类型。
   */
  VALUE_TYPE_STRING,
  /**
   * @const VALUE_TYPE_WSTRING
   * wchar_t*类型。
   */
  VALUE_TYPE_WSTRING
} value_type_t;

/**
 * @class value_t
 * @annotation ["scriptable"]
 * 一个通用数据类型，用来存放整数、浮点数、obj_t、str_t和其它对象。
 */
typedef struct _value_t {
  uint8_t type;
  union {
    int8_t i8;
    uint8_t u8;
    int16_t i16;
    uint16_t u16;
    int32_t i32;
    uint32_t u32;
    int64_t i64;
    uint64_t u64;
    float f;
    float f32;
    double f64;
    bool_t b;
    pointer_t ptr;
    const char* str;
    const wchar_t* wstr;
  } value;
} value_t;

/**
 * @property {int8_t} type
 * @annotation ["readable", "scriptable"]
 * 类型。
 */

/**
 * @method value_set_bool
 * 设置类型为bool的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 * @param {bool_t}   value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_bool(value_t* v, bool_t value);

/**
 * @method value_bool
 * 获取类型为bool的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {bool_t} 值。
 */
bool_t value_bool(const value_t* v);

/**
 * @method value_set_int8
 * 设置类型为int8的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {int8_t}   value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_int8(value_t* v, int8_t value);

/**
 * @method value_int8
 * 获取类型为int8的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {int8_t} 值。
 */
int8_t value_int8(const value_t* v);

/**
 * @method value_set_uint8
 * 设置类型为uint8的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {uint8_t}  value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_uint8(value_t* v, uint8_t value);

/**
 * @method value_uint8
 * 获取类型为uint8的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {int8_t} 值。
 */
uint8_t value_uint8(const value_t* v);

/**
 * @method value_set_int16
 * 设置类型为int16的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {int16_t}  value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_int16(value_t* v, int16_t value);

/**
 * @method value_int16
 * 获取类型为int16的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {int16_t} 值。
 */
int16_t value_int16(const value_t* v);

/**
 * @method value_set_uint16
 * 设置类型为uint16的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {uint16_t} value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_uint16(value_t* v, uint16_t value);

/**
 * @method value_uint16
 * 获取类型为uint16的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {uint16_t} 值。
 */
uint16_t value_uint16(const value_t* v);

/**
 * @method value_set_int32
 * 设置类型为int32的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {int32_t}  value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_int32(value_t* v, int32_t value);

/**
 * @method value_int32
 * 获取类型为int32的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {int32_t} 值。
 */
int32_t value_int32(const value_t* v);

/**
 * @method value_set_uint32
 * 设置类型为uint32的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {uint32_t} value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_uint32(value_t* v, uint32_t value);

/**
 * @method value_uint32
 * 获取类型为uint32的值。
 * @param {value_t*} v value对象。
 *
 * @return {uint32_t} 值。
 */
uint32_t value_uint32(const value_t* v);

/**
 * @method value_set_int64
 * 设置类型为int64的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {int64_t}  value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_int64(value_t* v, int64_t value);

/**
 * @method value_int64
 * 获取类型为int64的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {int64_t} 值。
 */
int64_t value_int64(const value_t* v);

/**
 * @method value_set_uint64
 * 设置类型为uint64的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {uint64_t} value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_uint64(value_t* v, uint64_t value);

/**
 * @method value_uint64
 * 获取类型为uint64的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {uint64_t} 值。
 */
uint64_t value_uint64(const value_t* v);

/**
 * @method value_set_pointer
 * 设置类型为pointer的值。
 * @param {value_t*}  v       value对象。
 * @param {pointer_t}     value   待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_pointer(value_t* v, pointer_t value);

/**
 * @method value_pointer
 * 获取类型为pointer的值。
 * @param {value_t*} v value对象。
 *
 * @return {void*} 值。
 */
pointer_t value_pointer(const value_t* v);

/**
 * @method value_set_float
 * 设置类型为float的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {float_t}    value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_float(value_t* v, float_t value);

/**
 * @method value_float
 * 获取类型为float的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {float} 值。
 */
float_t value_float(const value_t* v);

/**
 * @method value_set_float32
 * 设置类型为float的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {float}    value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_float32(value_t* v, float value);

/**
 * @method value_float32
 * 获取类型为float的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {float} 值。
 */
float value_float32(const value_t* v);

/**
 * @method value_set_double
 * 设置类型为double的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {double}   value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_double(value_t* v, double value);

/**
 * @method value_double
 * 获取类型为double的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {double} 值。
 */
double value_double(const value_t* v);

/**
 * @method value_set_str
 * 设置类型为char*的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {char*}   value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_str(value_t* v, const char* value);

value_t* value_set_wstr(value_t* v, const wchar_t* value);

/**
 * @method value_str
 * 获取类型为str_t的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {const char*} 值。
 */
const char* value_str(const value_t* v);

const wchar_t* value_wstr(const value_t* v);

/**
 * @method value_is_null
 * 判断value是否为空值。
 * @annotation ["scriptable"]
 * @param {value_t*} value value对象。
 *
 * @return {bool_t} 为空值返回TRUE，否则返回FALSE。
 */
bool_t value_is_null(value_t* value);

/**
 * @method value_equal
 * 判断两个value是否相同。
 * @param {value_t*} value value对象。
 * @param {value_t*} other value对象。
 *
 * @return {bool_t} 为空值返回TRUE，否则返回FALSE。
 */
bool_t value_equal(const value_t* value, const value_t* other);

/**
 * @method value_int
 * 转换为int的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {int} 值。
 */
int value_int(const value_t* v);

/**
 * @method value_copy
 * 拷贝value的值。
 * @param {value_t*} dst 目的value对象。
 * @param {value_t*} src 源value对象。
 *
 * @return {void} 值。
 */
void value_copy(value_t* dst, const value_t* src);

/**
 * @method value_set_int
 * 设置类型为int的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v  value对象。
 * @param {int32_t}  value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_int(value_t* v, int32_t value);

/**
 * @method value_create
 * @annotation ["constructor", "scriptable"]
 * 创建value对象
 *
 * @return {value_t*} 对象。
 */
value_t* value_create(void);

/**
 * @method value_destroy
 * @deconstructor
 * 销毁value对象
 * @annotation ["deconstructor", "scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {void}
 */
void value_destroy(value_t* v);

END_C_DECLS

#endif
