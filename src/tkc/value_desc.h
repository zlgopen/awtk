/**
 * File:   value_desc.h
 * Author: AWTK Develop Team
 * Brief:  value_desc
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-08-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_VALUE_DESC_H
#define TK_VALUE_DESC_H

#include "value.h"
#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @enum value_desc_type_t
 * 属性描述类型常量定义。
 */
typedef enum _value_desc_type_t {
  /**
   * @const VALUE_DESC_TYPE_INT8
   * 有符号8位整数类型。
   */
  VALUE_DESC_TYPE_INT8,
  /**
   * @const VALUE_DESC_TYPE_UINT8
   * 无符号8位整数类型。
   */
  VALUE_DESC_TYPE_UINT8,
  /**
   * @const VALUE_DESC_TYPE_INT16
   * 有符号16位整数类型。
   */
  VALUE_DESC_TYPE_INT16,
  /**
   * @const VALUE_DESC_TYPE_UINT16
   * 无符号16位整数类型。
   */
  VALUE_DESC_TYPE_UINT16,
  /**
   * @const VALUE_DESC_TYPE_INT32
   * 有符号32位整数类型。
   */
  VALUE_DESC_TYPE_INT32,
  /**
   * @const VALUE_DESC_TYPE_UINT32
   * 无符号32位整数类型。
   */
  VALUE_DESC_TYPE_UINT32,
  /**
   * @const VALUE_DESC_TYPE_INT64
   * 有符号64位整数类型。
   */
  VALUE_DESC_TYPE_INT64,
  /**
   * @const VALUE_DESC_TYPE_UINT64
   * 无符号64位整数类型。
   */
  VALUE_DESC_TYPE_UINT64,
  /**
   * @const VALUE_DESC_TYPE_FLOAT
   * 单精度浮点数类型。
   */
  VALUE_DESC_TYPE_FLOAT,
  /**
   * @const VALUE_DESC_TYPE_DOUBLE
   * 双精度浮点数类型。
   */
  VALUE_DESC_TYPE_DOUBLE,
  /**
   * @const VALUE_DESC_TYPE_BOOL
   * 布尔类型。
   */
  VALUE_DESC_TYPE_BOOL,
  /**
   * @const VALUE_DESC_TYPE_STRING
   * 字符串类型。
   */
  VALUE_DESC_TYPE_STRING,
  /**
   * @const VALUE_DESC_TYPE_BINARY
   * 二进制类型。
   */
  VALUE_DESC_TYPE_BINARY,
  /**
   * @const VALUE_DESC_TYPE_INT_ENUMS
   * 整数枚举类型。
   */
  VALUE_DESC_TYPE_INT_ENUMS,
  /**
   * @const VALUE_DESC_TYPE_STRING_ENUMS
   * 字符串枚举类型。
   */
  VALUE_DESC_TYPE_STRING_ENUMS
} value_desc_type_t;

/**
 * @enum value_format_type_t
 * 属性描述格式常量定义。
 */
typedef enum _value_format_type_t {
  /**
   * @const VALUE_FORMAT_TYPE_NONE
   * 无特殊格式。
   */
  VALUE_FORMAT_TYPE_NONE,
  /**
   * @const VALUE_FORMAT_TYPE_HEX
   * 16进制格式。
   */
  VALUE_FORMAT_TYPE_HEX,
  /**
   * @const VALUE_FORMAT_TYPE_HEX
   * 非负格式(用于进一步限制float/double)。
   */
  VALUE_FORMAT_TYPE_UNSIGNED,
  /**
   * @const VALUE_FORMAT_TYPE_PHONE
   * 手机号码格式。
   */
  VALUE_FORMAT_TYPE_PHONE,
  /**
   * @const VALUE_FORMAT_TYPE_PHONE
   * 邮件格式。
   */
  VALUE_FORMAT_TYPE_EMAIL,
  /**
   * @const VALUE_FORMAT_TYPE_IP4_ADDRESS
   * ip4地址格式。
   */
  VALUE_FORMAT_TYPE_IP4_ADDRESS,
  /**
   * @const VALUE_FORMAT_TYPE_DOMAIN_NAME
   * 域名格式。
   */
  VALUE_FORMAT_TYPE_DOMAIN_NAME,
  /**
   * @const VALUE_FORMAT_TYPE_URL
   * URL格式。
   */
  VALUE_FORMAT_TYPE_URL,
  /**
   * @const VALUE_FORMAT_TYPE_JSON
   * JSON格式。
   */
  VALUE_FORMAT_TYPE_JSON,
  /**
   * @const VALUE_FORMAT_TYPE_XML
   * XML格式。
   */
  VALUE_FORMAT_TYPE_XML,
  /**
   * @const VALUE_FORMAT_TYPE_UBJSON
   * UBJSON格式。
   */
  VALUE_FORMAT_TYPE_UBJSON
} value_format_type_t;

/**
 * @enum prop_desc_flags_t
 * 属性描述范围常量定义。
 */
typedef enum _prop_desc_flags_t {
  /**
   * @const PROP_DESC_FLAG_OBJECT
   * 单个对象的属性。
   */
  PROP_DESC_FLAG_OBJECT = 1,
  /**
   * @const PROP_DESC_FLAG_CLASS
   * 同一个类所有实例共享属性。
   */
  PROP_DESC_FLAG_CLASS = (1 << 1),
  /**
   * @const PROP_DESC_FLAG_GLOBAL
   * 全局属性。
   */
  PROP_DESC_FLAG_GLOBAL = (1 << 2),
  /**
   * @const PROP_DESC_FLAG_READABLE,
   * 可读取。
   */
  PROP_DESC_FLAG_READABLE = (1 << 3),
  /**
   * @const PROP_DESC_FLAG_WRITBALE,
   * 可修改。
   */
  PROP_DESC_FLAG_WRITBALE = (1 << 4),
  /**
   * @const PROP_DESC_FLAG_CONFIGURABLE
   * 可配置。
   */
  PROP_DESC_FLAG_CONFIGURABLE = (1 << 5),
  /**
   * @const PROP_DESC_FLAG_PERSISTENT
   * 需要持久化。
   */
  PROP_DESC_FLAG_PERSISTENT = (1 << 6)
} prop_desc_flags_t;

#define PROP_FLAGS_DEFAULT \
  (PROP_DESC_FLAG_OBJECT | PROP_DESC_FLAG_READABLE | PROP_DESC_FLAG_WRITBALE)
#define PROP_FLAGS_CONFIGURABLE \
  (PROP_FLAGS_DEFAULT | PROP_DESC_FLAG_CONFIGURABLE | PROP_DESC_FLAG_PERSISTENT)

/**
 * @enum arg_desc_flags_t
 * 参数描述范围常量定义。
 */
typedef enum _arg_desc_flags_t {
  /**
   * @const ARG_DESC_FLAGS_REQUIRED
   * 必须提供。
   */
  ARG_DESC_FLAGS_REQUIRED = 1
} arg_desc_flags_t;

/**
 * @class value_desc_t
 * 属性描述。
 */
typedef struct _value_desc_t {
  /**
   * @property {uint8_t} type
   * @annotation ["readable"]
   * 类型。
   */
  uint8_t type;
  /**
   * @property {uint8_t} format
   * @annotation ["readable"]
   * 格式。
   */
  uint8_t format;
  /**
   * @property {uint8_t} flags
   * @annotation ["readable"]
   * flags。
   */
  uint16_t flags;
  /**
   * @property {const char*} name
   * @annotation ["readable"]
   * 名称。
   */
  const char* name;
  /**
   * @property {const char*} display_name
   * @annotation ["readable"]
   * 显示名称。
   */
  const char* display_name;
  /**
   * @property {const char*} desc
   * @annotation ["readable"]
   * 描述。
   */
  const char* desc;
} value_desc_t, prop_desc_t, arg_desc_t, ret_desc_t;

/**
 * @class value_desc_int8_t
 * @parent value_desc_t
 * 有符号8位整数类型属性描述。
 */
typedef struct _value_desc_int8_t {
  value_desc_t value_desc;

  /**
   * @property {int8_t} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  int8_t defvalue;
  /**
   * @property {int8_t} min
   * @annotation ["readable"]
   * 最小值。
   */
  int8_t min;
  /**
   * @property {int8_t} max
   * @annotation ["readable"]
   * 最大值。
   */
  int8_t max;
  /**
   * @property {int8_t} step
   * @annotation ["readable"]
   * 步长。
   */
  int8_t step;
  /**
   * @property {const char*} unit
   * @annotation ["readable"]
   * 单位。
   */
  const char* unit;
} value_desc_int8_t, prop_desc_int8_t, arg_desc_int8_t;

/**
 * @class value_desc_uint8_t
 * @parent value_desc_t
 * 无符号8位整数类型属性描述。
 */
typedef struct _value_desc_uint8_t {
  value_desc_t value_desc;

  /**
   * @property {uint8_t} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  uint8_t defvalue;
  /**
   * @property {uint8_t} min
   * @annotation ["readable"]
   * 最小值。
   */
  uint8_t min;
  /**
   * @property {uint8_t} max
   * @annotation ["readable"]
   * 最大值。
   */
  uint8_t max;
  /**
   * @property {uint8_t} step
   * @annotation ["readable"]
   * 步长。
   */
  uint8_t step;
  /**
   * @property {const char*} unit
   * @annotation ["readable"]
   * 单位。
   */
  const char* unit;
} value_desc_uint8_t, prop_desc_uint8_t, arg_desc_uint8_t;

/**
 * @class value_desc_int16_t
 * @parent value_desc_t
 * 有符号16位整数类型属性描述。
 */
typedef struct _value_desc_int16_t {
  value_desc_t value_desc;

  /**
   * @property {int16_t} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  int16_t defvalue;
  /**
   * @property {int16_t} min
   * @annotation ["readable"]
   * 最小值。
   */
  int16_t min;
  /**
   * @property {int16_t} max
   * @annotation ["readable"]
   * 最大值。
   */
  int16_t max;
  /**
   * @property {int16_t} step
   * @annotation ["readable"]
   * 步长。
   */
  int16_t step;
  /**
   * @property {const char*} unit
   * @annotation ["readable"]
   * 单位。
   */
  const char* unit;
} value_desc_int16_t, prop_desc_int16_t, arg_desc_int16_t;

/**
 * @class value_desc_uint16_t
 * @parent value_desc_t
 * 无符号16位整数类型属性描述。
 */
typedef struct _value_desc_uint16_t {
  value_desc_t value_desc;

  /**
   * @property {uint16_t} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  uint16_t defvalue;
  /**
   * @property {uint16_t} min
   * @annotation ["readable"]
   * 最小值。
   */
  uint16_t min;
  /**
   * @property {uint16_t} max
   * @annotation ["readable"]
   * 最大值。
   */
  uint16_t max;
  /**
   * @property {uint16_t} step
   * @annotation ["readable"]
   * 步长。
   */
  uint16_t step;
  /**
   * @property {const char*} unit
   * @annotation ["readable"]
   * 单位。
   */
  const char* unit;
} value_desc_uint16_t, prop_desc_uint16_t, arg_desc_uint16_t;

/**
 * @class value_desc_int32_t
 * @parent value_desc_t
 * 有符号32位整数类型属性描述。
 */
typedef struct _value_desc_int32_t {
  value_desc_t value_desc;

  /**
   * @property {int32_t} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  int32_t defvalue;
  /**
   * @property {int32_t} min
   * @annotation ["readable"]
   * 最小值。
   */
  int32_t min;
  /**
   * @property {int32_t} max
   * @annotation ["readable"]
   * 最大值。
   */
  int32_t max;
  /**
   * @property {int32_t} step
   * @annotation ["readable"]
   * 步长。
   */
  int32_t step;
  /**
   * @property {const char*} unit
   * @annotation ["readable"]
   * 单位。
   */
  const char* unit;
} value_desc_int32_t, prop_desc_int32_t, arg_desc_int32_t;

/**
 * @class value_desc_uint32_t
 * @parent value_desc_t
 * 无符号32位整数类型属性描述。
 */
typedef struct _value_desc_uint32_t {
  value_desc_t value_desc;

  /**
   * @property {uint32_t} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  uint32_t defvalue;
  /**
   * @property {uint32_t} min
   * @annotation ["readable"]
   * 最小值。
   */
  uint32_t min;
  /**
   * @property {uint32_t} max
   * @annotation ["readable"]
   * 最大值。
   */
  uint32_t max;
  /**
   * @property {uint32_t} step
   * @annotation ["readable"]
   * 步长。
   */
  uint32_t step;
  /**
   * @property {const char*} unit
   * @annotation ["readable"]
   * 单位。
   */
  const char* unit;
} value_desc_uint32_t, prop_desc_uint32_t, arg_desc_uint32_t;

/**
 * @class value_desc_int64_t
 * @parent value_desc_t
 * 有符号64位整数类型属性描述。
 */
typedef struct _value_desc_int64_t {
  value_desc_t value_desc;

  /**
   * @property {int64_t} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  int64_t defvalue;
  /**
   * @property {int64_t} min
   * @annotation ["readable"]
   * 最小值。
   */
  int64_t min;
  /**
   * @property {int64_t} max
   * @annotation ["readable"]
   * 最大值。
   */
  int64_t max;
  /**
   * @property {int64_t} step
   * @annotation ["readable"]
   * 步长。
   */
  int64_t step;
  /**
   * @property {const char*} unit
   * @annotation ["readable"]
   * 单位。
   */
  const char* unit;
} value_desc_int64_t, prop_desc_int64_t, arg_desc_int64_t;

/**
 * @class value_desc_uint64_t
 * @parent value_desc_t
 * 无符号64位整数类型属性描述。
 */
typedef struct _value_desc_uint64_t {
  value_desc_t value_desc;

  /**
   * @property {uint64_t} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  uint64_t defvalue;
  /**
   * @property {uint64_t} min
   * @annotation ["readable"]
   * 最小值。
   */
  uint64_t min;
  /**
   * @property {uint64_t} max
   * @annotation ["readable"]
   * 最大值。
   */
  uint64_t max;
  /**
   * @property {uint64_t} step
   * @annotation ["readable"]
   * 步长。
   */
  uint64_t step;
  /**
   * @property {const char*} unit
   * @annotation ["readable"]
   * 单位。
   */
  const char* unit;
} value_desc_uint64_t, prop_desc_uint64_t, arg_desc_uint64_t;

/**
 * @class value_desc_float_t
 * @parent value_desc_t
 * 单精度浮点数类型属性描述。
 */
typedef struct _value_desc_float_t {
  value_desc_t value_desc;

  /**
   * @property {float} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  float defvalue;
  /**
   * @property {float} min
   * @annotation ["readable"]
   * 最小值。
   */
  float min;
  /**
   * @property {float} max
   * @annotation ["readable"]
   * 最大值。
   */
  float max;
  /**
   * @property {float_t} step
   * @annotation ["readable"]
   * 步长。
   */
  float_t step;
  /**
   * @property {const char*} unit
   * @annotation ["readable"]
   * 单位。
   */
  const char* unit;
} value_desc_float_t, prop_desc_float_t, arg_desc_float_t;

/**
 * @class value_desc_double_t
 * @parent value_desc_t
 * 单精度浮点数类型属性描述。
 */
typedef struct _value_desc_double_t {
  value_desc_t value_desc;

  /**
   * @property {double} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  double defvalue;
  /**
   * @property {double} min
   * @annotation ["readable"]
   * 最小值。
   */
  double min;
  /**
   * @property {double} max
   * @annotation ["readable"]
   * 最大值。
   */
  double max;
  /**
   * @property {double} step
   * @annotation ["readable"]
   * 步长。
   */
  double step;
  /**
   * @property {const char*} unit
   * @annotation ["readable"]
   * 单位。
   */
  const char* unit;
} value_desc_double_t, prop_desc_double_t, arg_desc_double_t;

/**
 * @class value_desc_bool_t
 * @parent value_desc_t
 * 布尔类型属性描述。
 */
typedef struct _value_desc_bool_t {
  value_desc_t value_desc;

  /**
   * @property {bool_t} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  bool_t defvalue;
} value_desc_bool_t, prop_desc_bool_t, arg_desc_bool_t;

/**
 * @class value_desc_string_t
 * @parent value_desc_t
 * 字符串类型属性描述。
 */
typedef struct _value_desc_string_t {
  value_desc_t value_desc;

  /**
   * @property {const char*} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  const char* defvalue;
  /**
   * @property {uint32_t} min
   * @annotation ["readable"]
   * 最小长度。
   */
  uint32_t min;
  /**
   * @property {uint32_t} max
   * @annotation ["readable"]
   * 最大长度。
   */
  uint32_t max;
} value_desc_string_t, prop_desc_string_t, arg_desc_string_t;

/**
 * @class value_desc_binary_t
 * @parent value_desc_t
 * 二进制类型属性描述。
 */
typedef struct _value_desc_binary_t {
  value_desc_t value_desc;
  /**
   * @property {uint32_t} min
   * @annotation ["readable"]
   * 最小长度。
   */
  uint32_t min;
  /**
   * @property {uint32_t} max
   * @annotation ["readable"]
   * 最大长度。
   */
  uint32_t max;
} value_desc_binary_t, prop_desc_binary_t, arg_desc_binary_t;

/**
 * @class value_desc_int_enums_t
 * @parent value_desc_t
 * 整数枚举类型属性描述。
 */
typedef struct _value_desc_int_enums_t {
  value_desc_t value_desc;

  /**
   * @property {int32_t} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  int32_t defvalue;

  /**
   * @property {const char**} enums
   * @annotation ["readable"]
   * 枚举值（每一项用英文冒号分隔值和名称）。
   */
  const char* enums[];
} value_desc_int_enums_t, prop_desc_int_enums_t, arg_desc_int_enums_t;

/**
 * @class value_desc_string_enums_t
 *
 * 字符串枚举类型属性描述。
 */
typedef struct _value_desc_string_enums_t {
  value_desc_t value_desc;

  /**
   * @property {const  char*} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  const char* defvalue;
  /**
   * @property {const char**} enums
   * @annotation ["readable"]
   * 枚举值。
   */
  const char* enums[];
} value_desc_string_enums_t, prop_desc_string_enums_t, arg_desc_string_enums_t;

ret_t value_desc_validate(value_desc_t* schema, value_t* v);

END_C_DECLS

#endif /*TK_VALUE_DESC_H*/
