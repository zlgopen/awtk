/**
 * File:   enums.h
 * Author: AWTK Develop Team
 * Brief:  enumerations
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ENUMS_H
#define TK_ENUMS_H

#include "base/types_def.h"

BEGIN_C_DECLS

typedef struct _key_type_value_t {
  const char* name;
  uint32_t type;
  uint32_t value;
} key_type_value_t;

enum { TYPE_INT = 0, TYPE_COLOR, TYPE_STRING, TYPE_WSTRING, TYPE_BOOL };
/**
 * @class enum_helper
 * @annotation ["fake"]
 */
const key_type_value_t* input_type_find(const char* name);
const key_type_value_t* window_closable_type_find(const char* name);
const key_type_value_t* align_v_type_find(const char* name);
const key_type_value_t* align_h_type_find(const char* name);
const key_type_value_t* asset_type_find(const char* name);

/**
 * @method asset_type_find_by_value
 *
 * 获取资源类型枚值和名称的对应关系。
 *
 * @param {uint32_t} value 资源的类型。
 *
 * @return {const key_type_value_t*} 返回资源类型枚值和名称的对应关系。
 */
const key_type_value_t* asset_type_find_by_value(uint32_t value);

const key_type_value_t* image_draw_type_find(const char* name);
const key_type_value_t* image_draw_type_find_by_value(uint32_t value);

/**
 * @method find_item
 *
 * 获取特定枚值和名称的对应关系。
 *
 * @param {const key_type_value_t*} items 特定枚举值对应列表。
 * @param {uint32_t} nr 列表长度。
 * @param {const char*} name 名称。
 *
 * @return {const key_type_value_t*} 返回资源类型枚值和名称的对应关系。
 */
const key_type_value_t* find_item(const key_type_value_t* items, uint32_t nr, const char* name);

/**
 * @method find_item_by_value
 *
 * 获取特定枚值和名称的对应关系。
 *
 * @param {const key_type_value_t*} items 特定枚举值对应列表。
 * @param {uint32_t} nr 列表长度。
 * @param {uint32_t} value 资源的类型。
 *
 * @return {const key_type_value_t*} 返回资源类型枚值和名称的对应关系。
 */
const key_type_value_t* find_item_by_value(const key_type_value_t* items, uint32_t nr,
                                           uint32_t value);
const key_type_value_t* easing_type_find(const char* name);
const key_type_value_t* easing_type_find_by_value(uint32_t value);

/**
 * @method keys_type_find
 *
 * 获取按键值和名称的对应关系。
 *
 * @param {const char*} name 按键名。
 *
 * @return {const key_type_value_t*} 返回按键值和名称的对应关系。
 */
const key_type_value_t* keys_type_find(const char* name);
/**
 * @method keys_type_find_by_value
 *
 * 获取按键值和名称的对应关系。
 *
 * @param {uint32_t} value 按键值。
 *
 * @return {const key_type_value_t*} 返回按键值和名称的对应关系。
 */
const key_type_value_t* keys_type_find_by_value(uint32_t value);
END_C_DECLS

#endif /*TK_ENUMS_H*/
