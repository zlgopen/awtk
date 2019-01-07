/**
 * File:   custom_props.h
 * Author: AWTK Develop Team
 * Brief:  custom_props
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_CUSTOM_PROPS_H
#define TK_CUSTOM_PROPS_H

#include "tkc/value.h"

BEGIN_C_DECLS

/**
 * @class custom_prop_t
 * widget自定义属性。
 */
typedef struct _custom_prop_t {
  char name[TK_NAME_LEN + 1];
  value_t value;
} custom_prop_t;

/**
 * @class custom_props_t
 * widget自定义属性集合。
 */
typedef struct _custom_props_t {
  uint32_t size;
  uint32_t capacity;
  custom_prop_t* props;
} custom_props_t;

/**
 * @method custom_props_create
 * @annotation ["constructor"]
 * 创建custom_props对象
 * @param {uint32_t} capacity 初始容量。
 *
 * @return {custom_props_t*} custom_props对象。
 */
custom_props_t* custom_props_create(uint32_t capacity);

/**
 * @method custom_props_get
 * 获取属性的值。
 * @param {custom_props_t} props 属性集合对象。
 * @param {char*} name 属性的名称。
 * @param {value_t*} v 用于返回属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t custom_props_get(custom_props_t* props, const char* name, value_t* v);

/**
 * @method custom_props_set
 * 设置属性的值。
 * @param {custom_props_t} props 属性集合对象。
 * @param {char*} name 属性的名称。
 * @param {value_t*} v 属性的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t custom_props_set(custom_props_t* props, const char* name, const value_t* v);

/**
 * @method custom_props_destroy
 * 销毁属性集合。
 * @param {custom_props_t} props 属性集合对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t custom_props_destroy(custom_props_t* props);

END_C_DECLS

#endif /*TK_CUSTOM_PROPS_H*/
