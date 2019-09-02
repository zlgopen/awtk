/**
 * File:   prop_desc.h
 * Author: AWTK Develop Team
 * Brief:  prop_desc
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
 * 2019-08-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PROP_DESC_H
#define TK_PROP_DESC_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @enum prop_desc_type_t
 * 属性描述类型常量定义。
 */
typedef enum _prop_desc_type_t {
  /**
   * @const PROP_DESC_TYPE_INT8
   * 有符号8位整数类型。
   */
  PROP_DESC_TYPE_INT8,
  /**
   * @const PROP_DESC_TYPE_UINT8
   * 无符号8位整数类型。
   */
  PROP_DESC_TYPE_UINT8,
  /**
   * @const PROP_DESC_TYPE_INT16
   * 有符号16位整数类型。
   */
  PROP_DESC_TYPE_INT16,
  /**
   * @const PROP_DESC_TYPE_UINT16
   * 无符号16位整数类型。
   */
  PROP_DESC_TYPE_UINT16,
  /**
   * @const PROP_DESC_TYPE_INT32
   * 有符号32位整数类型。
   */
  PROP_DESC_TYPE_INT32,
  /**
   * @const PROP_DESC_TYPE_UINT32
   * 无符号32位整数类型。
   */
  PROP_DESC_TYPE_UINT32,
  /**
   * @const PROP_DESC_TYPE_INT64
   * 有符号64位整数类型。
   */
  PROP_DESC_TYPE_INT64,
  /**
   * @const PROP_DESC_TYPE_UINT64
   * 无符号64位整数类型。
   */
  PROP_DESC_TYPE_UINT64,
  /**
   * @const PROP_DESC_TYPE_FLOAT
   * 单精度浮点数类型。
   */
  PROP_DESC_TYPE_FLOAT,
  /**
   * @const PROP_DESC_TYPE_DOUBLE
   * 双精度浮点数类型。
   */
  PROP_DESC_TYPE_DOUBLE,
  /**
   * @const PROP_DESC_TYPE_BOOL
   * 布尔类型。
   */
  PROP_DESC_TYPE_BOOL,
  /**
   * @const PROP_DESC_TYPE_STRING
   * 字符串类型。
   */
  PROP_DESC_TYPE_STRING,
  /**
   * @const PROP_DESC_TYPE_INT_ENUMS
   * 整数枚举类型。
   */
  PROP_DESC_TYPE_INT_ENUMS,
  /**
   * @const PROP_DESC_TYPE_STRING_ENUMS
   * 字符串枚举类型。
   */
  PROP_DESC_TYPE_STRING_ENUMS
} prop_desc_type_t;

/**
 * @enum prop_format_type_t
 * 属性描述格式常量定义。
 */
typedef enum _prop_format_type_t {
  /**
   * @const PROP_FORMAT_TYPE_NONE
   * 无特殊格式。
   */
  PROP_FORMAT_TYPE_NONE,
  /**
   * @const PROP_FORMAT_TYPE_HEX
   * 16进制格式。
   */
  PROP_FORMAT_TYPE_HEX,
  /**
   * @const PROP_FORMAT_TYPE_HEX
   * 非负格式(用于进一步限制float/double)。
   */
  PROP_FORMAT_TYPE_UNSIGNED,
  /**
   * @const PROP_FORMAT_TYPE_PHONE
   * 手机号码格式。
   */
  PROP_FORMAT_TYPE_PHONE,
  /**
   * @const PROP_FORMAT_TYPE_PHONE
   * 邮件格式。
   */
  PROP_FORMAT_TYPE_EMAIL,
  /**
   * @const PROP_FORMAT_TYPE_IP4_ADDRESS
   * ip4地址格式。
   */
  PROP_FORMAT_TYPE_IP4_ADDRESS,
  /**
   * @const PROP_FORMAT_TYPE_DOMAIN_NAME
   * 域名格式。
   */
  PROP_FORMAT_TYPE_DOMAIN_NAME,
  /**
   * @const PROP_FORMAT_TYPE_URL
   * URL格式。
   */
  PROP_FORMAT_TYPE_URL
} prop_format_type_t;

/**
 * @enum prop_desc_scope_t
 * 属性描述范围常量定义。
 */
typedef enum _prop_desc_scope_t {
  /**
   * @const PROP_DESC_SCOPE_OBJECT
   * 单个对象的属性。
   */
  PROP_DESC_SCOPE_OBJECT = 0,
  /**
   * @const PROP_DESC_SCOPE_CLASS
   * 同一个类所有实例共享属性。
   */
  PROP_DESC_SCOPE_CLASS,
  /**
   * @const PROP_DESC_SCOPE_GLOBAL
   * 全局属性。
   */
  PROP_DESC_SCOPE_GLOBAL
} prop_desc_scope_t;

/**
 * @class prop_desc_t
 * 属性描述。
 */
typedef struct _prop_desc_t {
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
   * @property {uint8_t} scope
   * @annotation ["readable"]
   * 范围。
   */
  uint8_t scope : 1;
  /**
   * @property {uint8_t} persistent
   * @annotation ["readable"]
   * 是否需要存储。
   */
  uint8_t persistent : 1;
  /**
   * @property {uint8_t} configurable
   * @annotation ["readable"]
   * 是否用户可配置。
   */
  uint8_t configurable : 1;
  /**
   * @property {uint8_t} reserved
   * @annotation ["readable"]
   * 保留。
   */
  uint8_t reserved;
  /**
   * @property {const char*} name
   * @annotation ["readable"]
   * 名称。
   */
  const char* name;
  /**
   * @property {const char*} desc
   * @annotation ["readable"]
   * 描述。
   */
  const char* desc;
} prop_desc_t;

/**
 * @class prop_desc_int8_t
 * @parent prop_desc_t
 * 有符号8位整数类型属性描述。
 */
typedef struct _prop_desc_int8_t {
  prop_desc_t prop_desc;

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
} prop_desc_int8_t;

/**
 * @class prop_desc_uint8_t
 * @parent prop_desc_t
 * 无符号8位整数类型属性描述。
 */
typedef struct _prop_desc_uint8_t {
  prop_desc_t prop_desc;

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
} prop_desc_uint8_t;

/**
 * @class prop_desc_int16_t
 * @parent prop_desc_t
 * 有符号16位整数类型属性描述。
 */
typedef struct _prop_desc_int16_t {
  prop_desc_t prop_desc;

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
} prop_desc_int16_t;

/**
 * @class prop_desc_uint16_t
 * @parent prop_desc_t
 * 无符号16位整数类型属性描述。
 */
typedef struct _prop_desc_uint16_t {
  prop_desc_t prop_desc;

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
} prop_desc_uint16_t;

/**
 * @class prop_desc_int32_t
 * @parent prop_desc_t
 * 有符号32位整数类型属性描述。
 */
typedef struct _prop_desc_int32_t {
  prop_desc_t prop_desc;

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
} prop_desc_int32_t;

/**
 * @class prop_desc_uint32_t
 * @parent prop_desc_t
 * 无符号32位整数类型属性描述。
 */
typedef struct _prop_desc_uint32_t {
  prop_desc_t prop_desc;

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
} prop_desc_uint32_t;

/**
 * @class prop_desc_int64_t
 * @parent prop_desc_t
 * 有符号64位整数类型属性描述。
 */
typedef struct _prop_desc_int64_t {
  prop_desc_t prop_desc;

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
} prop_desc_int64_t;

/**
 * @class prop_desc_uint64_t
 * @parent prop_desc_t
 * 无符号64位整数类型属性描述。
 */
typedef struct _prop_desc_uint64_t {
  prop_desc_t prop_desc;

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
} prop_desc_uint64_t;

/**
 * @class prop_desc_float_t
 * @parent prop_desc_t
 * 单精度浮点数类型属性描述。
 */
typedef struct _prop_desc_float_t {
  prop_desc_t prop_desc;

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
} prop_desc_float_t;

/**
 * @class prop_desc_double_t
 * @parent prop_desc_t
 * 单精度浮点数类型属性描述。
 */
typedef struct _prop_desc_double_t {
  prop_desc_t prop_desc;

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
   * @property {double_t} step
   * @annotation ["readable"]
   * 步长。
   */
  double_t step;
} prop_desc_double_t;

/**
 * @class prop_desc_bool_t
 * @parent prop_desc_t
 * 布尔类型属性描述。
 */
typedef struct _prop_desc_bool_t {
  prop_desc_t prop_desc;

  /**
   * @property {bool_t} defvalue
   * @annotation ["readable"]
   * 缺省值。
   */
  bool_t defvalue;
} prop_desc_bool_t;

/**
 * @class prop_desc_string_t
 * @parent prop_desc_t
 * 字符串类型属性描述。
 */
typedef struct _prop_desc_string_t {
  prop_desc_t prop_desc;

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
} prop_desc_string_t;

/**
 * @class prop_desc_int_enums_t
 * @parent prop_desc_t
 * 整数枚举类型属性描述。
 */
typedef struct _prop_desc_int_enums_t {
  prop_desc_t prop_desc;

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
} prop_desc_int_enums_t;

/**
 * @class prop_desc_string_enums_t
 *
 * 字符串枚举类型属性描述。
 */
typedef struct _prop_desc_string_enums_t {
  prop_desc_t prop_desc;

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
} prop_desc_string_enums_t;

END_C_DECLS

#endif /*TK_PROP_DESC_H*/
