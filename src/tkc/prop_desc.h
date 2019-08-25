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
   * @const PROP_DESC_TYPE_INT32
   * 有符号32位整数类型。
   */
  PROP_DESC_TYPE_INT32 = 0,
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
   * @const PROP_DESC_TYPE_PHONE
   * 手机号码类型。
   */
  PROP_DESC_TYPE_PHONE,
  /**
   * @const PROP_DESC_TYPE_PHONE
   * 邮件类型。
   */
  PROP_DESC_TYPE_EMAIL,
  /**
   * @const PROP_DESC_TYPE_IP4_ADDRESS
   * ip4地址类型。
   */
  PROP_DESC_TYPE_IP4_ADDRESS,
  /**
   * @const PROP_DESC_TYPE_DOMAIN_NAME
   * 域名类型。
   */
  PROP_DESC_TYPE_DOMAIN_NAME,
  /**
   * @const PROP_DESC_TYPE_URL
   * URL类型。
   */
  PROP_DESC_TYPE_URL,
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
   * @property {uint8_t} scope
   * @annotation ["readable"]
   * 范围。
   */
  uint8_t scope : 1;
  /**
   * @property {uint8_t} persistent
   * @annotation ["readable"]
   * 需要存储。
   */
  uint8_t persistent : 1;
  /**
   * @property {uint16_t} reserved
   * @annotation ["readable"]
   * 保留。
   */
  uint16_t reserved;
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
 * @class prop_desc_phone_t
 * @parent prop_desc_string_t
 * 电话号码类型属性描述。
 */
typedef struct _prop_desc_phone_t {
  prop_desc_string_t prop_desc_string;
} prop_desc_phone_t;

/**
 * @class prop_desc_email_t
 * @parent prop_desc_string_t
 * 邮箱地址类型属性描述。
 */
typedef struct _prop_desc_email_t {
  prop_desc_string_t prop_desc_string;
} prop_desc_email_t;

/**
 * @class prop_desc_ip4_address_t
 * @parent prop_desc_string_t
 * ip4地址类型属性描述。
 */
typedef struct _prop_desc_ip4_address_t {
  prop_desc_string_t prop_desc_string;
} prop_desc_ip4_address_t;

/**
 * @class prop_desc_domain_name_t
 * @parent prop_desc_string_t
 * 域名类型属性描述。
 */
typedef struct _prop_desc_domain_name_t {
  prop_desc_string_t prop_desc_string;
} prop_desc_domain_name_t;

/**
 * @class prop_desc_url_t
 * @parent prop_desc_string_t
 * URL类型属性描述。
 */
typedef struct _prop_desc_url_t {
  prop_desc_string_t prop_desc_string;
} prop_desc_url_t;

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
  const char** enums;
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
  const char** enums;
} prop_desc_string_enums_t;

END_C_DECLS

#endif /*TK_PROP_DESC_H*/
