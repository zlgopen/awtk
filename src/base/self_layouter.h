/**
 * File:   self_layouter.h
 * Author: AWTK Develop Team
 * Brief:  self layouter
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
 * 2018-12-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_SELF_LAYOUTER_H
#define TK_SELF_LAYOUTER_H

#include "tkc/str.h"
#include "tkc/rect.h"
#include "tkc/value.h"
#include "base/types_def.h"
#include "base/layout_def.h"

BEGIN_C_DECLS

struct _self_layouter_t;
typedef struct _self_layouter_t self_layouter_t;

typedef const char* (*self_layouter_to_string_t)(self_layouter_t* layouter);
typedef ret_t (*self_layouter_layout_t)(self_layouter_t* layouter, widget_t* widget, rect_t* area);
typedef ret_t (*self_layouter_get_param_t)(self_layouter_t* layouter, const char* name, value_t* v);
typedef ret_t (*self_layouter_set_param_t)(self_layouter_t* layouter, const char* name,
                                           const value_t* v);
typedef ret_t (*self_layouter_destroy_t)(self_layouter_t* layouter);

typedef self_layouter_t* (*self_layouter_create_t)(void);
typedef self_layouter_t* (*self_layouter_clone_t)(self_layouter_t* layouter);

typedef struct _self_layouter_vtable_t {
  const char* type;
  self_layouter_to_string_t to_string;
  self_layouter_layout_t layout;
  self_layouter_get_param_t get_param;
  self_layouter_set_param_t set_param;
  self_layouter_clone_t clone;
  self_layouter_destroy_t destroy;
} self_layouter_vtable_t;

/**
 * @class self_layouter_t
 * 控件自身排版布局器的接口。
 *
 * 按特定算法对控件进行排版布局，子类需要实现vtable中的函数。
 */
struct _self_layouter_t {
  str_t params;
  const self_layouter_vtable_t* vt;
};

/**
 * @method self_layouter_to_string
 * 获取全部参数。
 * @param {self_layouter_t*} layouter layouter对象。
 *
 * @return {ret_t} 返回字符串格式的参数。
 */
const char* self_layouter_to_string(self_layouter_t* layouter);

/**
 * @method self_layouter_layout
 * 对控件自身进行布局。
 * @param {self_layouter_t*} layouter layouter对象。
 * @param {widget_t*} widget 控件。
 * @param {rect_t*} area 可以使用的区域。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t self_layouter_layout(self_layouter_t* layouter, widget_t* widget, rect_t* area);

/**
 * @method self_layouter_get_param
 * 获取指定的参数。
 * @param {self_layouter_t*} layouter layouter对象。
 * @param {const char*} name 参数名。
 * @param {value_t*} v 返回参数的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t self_layouter_get_param(self_layouter_t* layouter, const char* name, value_t* v);

/**
 * @method self_layouter_set_param
 * 获取指定的参数。
 * @param {self_layouter_t*} layouter layouter对象。
 * @param {const char*} name 参数名。
 * @param {const value_t*} v 参数的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t self_layouter_set_param(self_layouter_t* layouter, const char* name, const value_t* v);

/**
 * @method self_layouter_set_param_str
 * 设置字符串格式的参数。
 * @param {self_layouter_t*} layouter layouter对象。
 * @param {const char*} name 参数名。
 * @param {const char*} value 参数值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t self_layouter_set_param_str(self_layouter_t* layouter, const char* name, const char* value);

/**
 * @method self_layouter_get_param_float
 * 获取指定的浮点格式的参数。
 * @param {self_layouter_t*} layouter layouter对象。
 * @param {const char*} name 参数名。
 * @param {float_t} defval 缺省值。
 *
 * @return {ret_t} 成功返回参数的值，失败返回缺省值。
 */
float_t self_layouter_get_param_float(self_layouter_t* layouter, const char* name, float_t defval);

/**
 * @method self_layouter_get_param_int
 * 获取指定的整数格式的参数。
 * @param {self_layouter_t*} layouter layouter对象。
 * @param {const char*} name 参数名。
 * @param {int32_t} defval 缺省值。
 *
 * @return {ret_t} 成功返回参数的值，失败返回缺省值。
 */
int32_t self_layouter_get_param_int(self_layouter_t* layouter, const char* name, int32_t defval);

/**
 * @method self_layouter_destroy
 * 销毁layouter对象。
 * @param {self_layouter_t*} layouter layouter对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t self_layouter_destroy(self_layouter_t* layouter);

/**
 * @method self_layouter_create
 * 创建layouter对象。
 * @param {const char*} params 参数。
 *
 * @return {self_layouter_t*} 返回layouter对象。
 */
self_layouter_t* self_layouter_create(const char* params);

/**
 * @method self_layouter_clone
 * 克隆layouter对象。
 *
 * @param {const char*} layouter 被克隆的对象。
 *
 * @return {self_layouter_t*} 返回layouter对象。
 */
self_layouter_t* self_layouter_clone(self_layouter_t* layouter);

END_C_DECLS

#endif /*TK_SELF_LAYOUTER_H*/
