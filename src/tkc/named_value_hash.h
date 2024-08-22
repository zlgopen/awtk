/**
 * File:   named_value_hash.h
 * Author: AWTK Develop Team
 * Brief:  带有散列值的named value
 *
 * Copyright (c) 2024 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2024-08-21 Shen ZhaoKun <shenzhaokun@zlg.cn> created
 *
 */

#ifndef TK_NAMED_VALUE_HASH_H
#define TK_NAMED_VALUE_HASH_H

#include "tkc/named_value.h"

BEGIN_C_DECLS

/**
 * @const HASH_BASE_DEFAULT
 * 默认字符串散列进制。
 */
#define HASH_BASE_DEFAULT 13331

/**
 * @class named_value_hash_t
 * @parent named_value_t
 * @annotation ["scriptable"]
 * 带有散列值的命名的值。
 *
 */
typedef struct _named_value_hash_t {
  named_value_t base;
  /**
   * @property {uint64_t} hash
   * @annotation ["readable"]
   * 散列值。
   */
  uint64_t hash;
} named_value_hash_t;

/**
 * @method named_value_hash_create
 *
 * 创建named_value_hash对象。
 * @annotation ["constructor", "scriptable", "gc"]
 *
 * @return {named_value_hash_t*} 返回named_value_hash对象。
 */
named_value_hash_t* named_value_hash_create(void);

/**
 * @method named_value_hash_create_ex
 *
 * 创建named_value_hash对象。
 * @annotation ["constructor"]
 *
 * @param {const char*} name 名称。
 * @param {const value_t*} value 值。
 * @param {uint64_t} hash_base 字符串散列进制。
 *
 * @return {named_value_hash_t*} 返回named_value_hash对象。
 */
named_value_hash_t* named_value_hash_create_ex(const char* name, const value_t* value,
                                               uint64_t hash_base);

/**
 * @method named_value_hash_init
 *
 * 初始化。
 * @annotation ["constructor"]
 *
 * @param {named_value_hash_t*} nvh named_value_hash对象。
 * @param {const char*} name 名称。
 * @param {const value_t*} value 值。
 * @param {uint64_t} hash_base 字符串散列进制。
 *
 * @return {named_value_hash_t*} 返回named_value_hash对象。
 */
named_value_hash_t* named_value_hash_init(named_value_hash_t* nvh, const char* name,
                                          const value_t* value, uint64_t hash_base);

/**
 * @method named_value_hash_set_name
 *
 * 设置散列值。
 * @annotation ["scriptable"]
 *
 * @param {named_value_hash_t*} nvh named_value_hash对象。
 * @param {const char*} name 名称。
 * @param {uint64_t} hash_base 字符串散列进制。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t named_value_hash_set_name(named_value_hash_t* nvh, const char* name, uint64_t hash_base);

/**
 * @method named_value_hash_deinit
 *
 * 重置named_value_hash对象。
 * @annotation ["deconstructor"]
 *
 * @param {named_value_hash_t*} nvh named_value_hash对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t named_value_hash_deinit(named_value_hash_t* nvh);

/**
 * @method named_value_hash_compare
 *
 * 比较。
 *
 * @param {named_value_hash_t*} nvh named_value_hash对象。
 * @param {const named_value_hash_t*} other named_value_hash对象。
 *
 * @return {int32_t} 返回RET_OK表示成功，否则表示失败。
 */
int32_t named_value_hash_compare(named_value_hash_t* nvh, const named_value_hash_t* other);

/**
 * @method named_value_hash_compare_by_hash
 *
 * 比较。
 *
 * @param {named_value_hash_t*} nvh named_value_hash对象。
 * @param {uint64_t} hash 散列值。
 *
 * @return {int32_t} 返回RET_OK表示成功，否则表示失败。
 */
int32_t named_value_hash_compare_by_hash(named_value_hash_t* nvh, uint64_t hash);

/**
 * @method named_value_hash_destroy
 *
 * 销毁named_value_hash对象。
 * @annotation ["deconstructor", "scriptable", "gc"]
 *
 * @param {named_value_hash_t*} nvh named_value_hash对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t named_value_hash_destroy(named_value_hash_t* nvh);

/**
 * @method named_value_hash_clone
 *
 * 克隆named_value_hash对象。
 * @annotation ["scriptable"]
 * 
 * @param {named_value_hash_t*} nvh named_value_hash对象。
 *
 * @return {named_value_hash_t*} 返回named_value_hash对象。
 */
named_value_hash_t* named_value_hash_clone(named_value_hash_t* nvh);

/**
 * @method named_value_hash_get_hash_from_str
 *
 * 获取字符串散列值。
 * @annotation ["static", "scriptable"]
 *
 * @param {const char*} str 字符串。
 * @param {uint64_t} base 进制。
 *
 * @return {uint64_t} 返回散列值。
 */
uint64_t named_value_hash_get_hash_from_str(const char* str, uint64_t base);

END_C_DECLS

#endif /*TK_NAMED_VALUE_HASH_H*/
