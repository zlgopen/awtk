/**
 * File:   value.h
 * Author: AWTK Develop Team
 * Brief:  generic value type
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/types_def.h"

#ifndef TK_VALUE_H
#define TK_VALUE_H

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
  VALUE_TYPE_WSTRING,
  /**
   * @const VALUE_TYPE_OBJECT
   * object_t*类型。
   */
  VALUE_TYPE_OBJECT,
  /**
   * @const VALUE_TYPE_SIZED_STRING
   * 带长度的字符串。
   */
  VALUE_TYPE_SIZED_STRING,
  /**
   * @const VALUE_TYPE_BINARY
   * 二进制数据。
   */
  VALUE_TYPE_BINARY,
  /**
   * @const VALUE_TYPE_UBJSON
   * 二进制数据(UBJSON)。
   */
  VALUE_TYPE_UBJSON,
  /**
   * @const VALUE_TYPE_TOKEN
   * 特殊用途。
   */
  VALUE_TYPE_TOKEN,
} value_type_t;

typedef struct _binary_data_t {
  uint32_t size;
  void* data;
} binary_data_t;

typedef struct _sized_str_t {
  uint32_t size;
  char* str;
} sized_str_t;

/**
 * @class value_t
 * @order -9
 * @annotation ["scriptable"]
 * 一个通用数据类型，用来存放整数、浮点数、字符串和其它对象。
 *
 * 在C/C++中，一般不需动态创建对象，直接声明并初始化即可。如：
 *
 * ```c
 * value_t v;
 * value_set_int(&v, 100);
 * ```
 *
 *> 在脚本语言中，需要动态创建对象。
 *
 */
struct _value_t {
  uint32_t type : 8;
  uint32_t free_handle : 1;

  union {
    int8_t i8;
    uint8_t u8;
    int16_t i16;
    uint16_t u16;
    int32_t i32;
    uint32_t u32;
    int64_t i64;
    uint64_t u64;
    uint32_t token;
    float f;
    float f32;
    double f64;
    bool_t b;
    void* ptr;
    const char* str;
    const wchar_t* wstr;
    object_t* object;
    binary_data_t binary_data;
    sized_str_t sized_str;
  } value;
};

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
 * @param {void*}     value   待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_pointer(value_t* v, void* value);

/**
 * @method value_pointer
 * 获取类型为pointer的值。
 * @param {value_t*} v value对象。
 *
 * @return {void*} 值。
 */
void* value_pointer(const value_t* v);

/**
 * @method value_set_float
 * 设置类型为float\_t的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {float_t}    value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_float(value_t* v, float_t value);

/**
 * @method value_float
 * 获取类型为float\_t的值。
 * @param {value_t*} v value对象。
 *
 * @return {float_t} 值。
 */
float_t value_float(const value_t* v);

/**
 * @method value_set_float32
 * 设置类型为float的值。
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
 * @alias value_set_float64
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
 * @alias value_float64
 * @param {value_t*} v value对象。
 *
 * @return {double} 值。
 */
double value_double(const value_t* v);

/**
 * @method value_set_str
 * 设置类型为字符串的值。
 * @param {value_t*} v     value对象。
 * @param {const char*}   value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_str(value_t* v, const char* value);

/**
 * @method value_dup_str
 * 设置类型为字符串的值(并拷贝字符串)。
 *
 * > 供脚本语言使用。
 * @alias value_set_str
 * @annotation ["scriptable"]
 * @param {value_t*} v     value对象。
 * @param {const char*}   value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_dup_str(value_t* v, const char* value);

/**
 * @method value_set_wstr
 * 设置类型为宽字符串的值。
 * @param {value_t*} v     value对象。
 * @param {const wchar_t*}   value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_wstr(value_t* v, const wchar_t* value);

/**
 * @method value_str
 * 获取类型为字符串的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {const char*} 值。
 */
const char* value_str(const value_t* v);

/**
 * @method value_wstr
 * 获取类型为宽字符串的值。
 * @param {value_t*} v value对象。
 *
 * @return {const wchar_t*} 值。
 */
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
 * @param {value_t*} v value对象。
 *
 * @return {int} 值。
 */
int value_int(const value_t* v);

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
 * @method value_set_object
 * 设置类型为object的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v  value对象。
 * @param {object_t*}  value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_object(value_t* v, object_t* value);

/**
 * @method value_object
 * 转换为object的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {object_t*} 值。
 */
object_t* value_object(const value_t* v);

/**
 * @method value_set_token
 * 设置类型为token的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v  value对象。
 * @param {uint32_t}  value 待设置的值。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_token(value_t* v, uint32_t value);

/**
 * @method value_token
 * 获取token的值。
 * @annotation ["scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {uint32_t} 值。
 */
uint32_t value_token(const value_t* v);

/**
 * @method value_set_sized_str
 * 设置类型为带长度的字符串的值。
 * @param {value_t*} v  value对象。
 * @param {char*}  str 待设置的值。
 * @param {uint32_t}  size 长度。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_sized_str(value_t* v, char* str, uint32_t size);

/**
 * @method value_sized_str
 * 获取为sized_str的值。
 * @param {value_t*} v value对象。
 *
 * @return {sized_str_t*} 值。
 */
sized_str_t* value_sized_str(const value_t* v);

/**
 * @method value_set_binary_data
 * 设置类型为binary_data的值。
 * @param {value_t*} v  value对象。
 * @param {void*}  value 待设置的值。
 * @param {uint32_t}  size 长度。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_binary_data(value_t* v, void* data, uint32_t size);

/**
 * @method value_binary_data
 * 获取为binary_data的值。
 * @param {value_t*} v value对象。
 *
 * @return {binary_data_t*} 值。
 */
binary_data_t* value_binary_data(const value_t* v);

/**
 * @method value_set_ubjson
 * 设置类型为ubjson的值。
 * @param {value_t*} v  value对象。
 * @param {void*}  value 待设置的值。
 * @param {uint32_t}  size 长度。
 *
 * @return {value_t*} value对象本身。
 */
value_t* value_set_ubjson(value_t* v, void* data, uint32_t size);

/**
 * @method value_ubjson
 * 获取为ubjson的值。
 * @param {value_t*} v value对象。
 *
 * @return {binary_data_t*} 值。
 */
binary_data_t* value_ubjson(const value_t* v);

/**
 * @method value_copy
 * 拷贝value的值。
 * @param {value_t*} dst 目的value对象。
 * @param {value_t*} src 源value对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_copy(value_t* dst, const value_t* src);

/**
 * @method value_deep_copy
 * 深拷贝value的值。
 * @param {value_t*} dst 目的value对象。
 * @param {value_t*} src 源value对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_deep_copy(value_t* dst, const value_t* src);

/**
 * @method value_create
 * @annotation ["constructor", "scriptable", "gc"]
 * 创建value对象。
 *
 * @return {value_t*} 对象。
 */
value_t* value_create(void);

/**
 * @method value_destroy
 * 销毁value对象。
 * @annotation ["deconstructor", "scriptable", "gc"]
 * @param {value_t*} v value对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_destroy(value_t* v);

/**
 * @method value_reset
 * 重置value对象。
 * @annotation ["deconstructor", "scriptable"]
 * @param {value_t*} v value对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t value_reset(value_t* v);

/**
 * @method value_cast
 * 转换为value对象。
 *
 * > 供脚本语言使用
 * @annotation ["cast", "scriptable"]
 * @param {value_t*} value value对象。
 *
 * @return {value_t*} 对象。
 */
value_t* value_cast(value_t* value);

END_C_DECLS

#endif /*TK_VALUE_H*/
