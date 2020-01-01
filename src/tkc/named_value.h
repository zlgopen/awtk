/**
 * File:   named_value.h
 * Author: AWTK Develop Team
 * Brief:  named value
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
 * 2019-01-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_NAMED_VALUE_H
#define TK_NAMED_VALUE_H

#include "tkc/value.h"

BEGIN_C_DECLS

/**
 * @class named_value_t
 * @annotation ["scriptable"]
 * 命名的值。
 *
 */
typedef struct _named_value_t {
  /**
   * @property {char*} name
   * @annotation ["readable", "scriptable"]
   * 名称。
   */
  char* name;

  /**
   * @property {value_t} value
   * @annotation ["readable"]
   * 值。
   */
  value_t value;
} named_value_t;

/**
 * @method named_value_create
 *
 * 创建named_value对象。
 * @annotation ["constructor", "scriptable", "gc"]
 *
 * @return {named_value_t*} 返回named_value对象。
 */
named_value_t* named_value_create(void);

/**
 * @method named_value_init
 *
 * 初始化。
 * @annotation ["constructor"]
 *
 * @param {named_value_t*} nv named_value对象。
 * @param {const char*} name 名称。
 * @param {const value_t*} value 值。
 *
 * @return {ret_t} 返回named_value对象。
 */
named_value_t* named_value_init(named_value_t* nv, const char* name, const value_t* value);

/**
 * @method named_value_cast
 *
 * 转换为named_value对象(供脚本语言使用)。
 * @annotation ["cast", "scriptable"]
 *
 * @param {named_value_t*} nv named_value对象。
 *
 * @return {named_value_t*} 返回named_value对象。
 */
named_value_t* named_value_cast(named_value_t* nv);

/**
 * @method named_value_set_name
 *
 * 设置名称。
 * @annotation ["scriptable"]
 *
 * @param {named_value_t*} nv named_value对象。
 * @param {const char*} name 名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t named_value_set_name(named_value_t* nv, const char* name);

/**
 * @method named_value_set_value
 *
 * 设置值。
 * @annotation ["scriptable"]
 *
 * @param {named_value_t*} nv named_value对象。
 * @param {const value_t*} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t named_value_set_value(named_value_t* nv, const value_t* value);

/**
 * @method named_value_get_value
 *
 * 获取值对象(主要给脚本语言使用)。
 * @annotation ["scriptable"]
 *
 * @param {named_value_t*} nv named_value对象。
 *
 * @return {value_t*} 返回值对象。
 *
 */
value_t* named_value_get_value(named_value_t* nv);

/**
 * @method named_value_deinit
 *
 * 重置named_value对象。
 * @annotation ["deconstructor"]
 *
 * @param {named_value_t*} nv named_value对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t named_value_deinit(named_value_t* nv);

/**
 * @method named_value_destroy
 *
 * 销毁named_value对象。
 * @annotation ["deconstructor", "scriptable", "gc"]
 *
 * @param {named_value_t*} nv named_value对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t named_value_destroy(named_value_t* nv);

END_C_DECLS

#endif /*TK_NAMED_VALUE_H*/
