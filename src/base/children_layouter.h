/**
 * File:   children_layouter.h
 * Author: AWTK Develop Team
 * Brief:  children layouter
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

#ifndef TK_CHILDREN_LAYOUTER_H
#define TK_CHILDREN_LAYOUTER_H

#include "tkc/str.h"
#include "tkc/value.h"
#include "base/types_def.h"

BEGIN_C_DECLS

struct _children_layouter_t;
typedef struct _children_layouter_t children_layouter_t;

typedef const char* (*children_layouter_to_string_t)(children_layouter_t* layouter);
typedef ret_t (*children_layouter_layout_t)(children_layouter_t* layouter, widget_t* widget);
typedef ret_t (*children_layouter_get_param_t)(children_layouter_t* layouter, const char* name,
                                               value_t* v);
typedef ret_t (*children_layouter_set_param_t)(children_layouter_t* layouter, const char* name,
                                               const value_t* v);
typedef bool_t (*children_layouter_is_valid_t)(children_layouter_t* layouter);
typedef ret_t (*children_layouter_destroy_t)(children_layouter_t* layouter);

typedef children_layouter_t* (*children_layouter_create_t)(void);
typedef children_layouter_t* (*children_layouter_clone_t)(children_layouter_t* layouter);

typedef struct _children_layouter_vtable_t {
  const char* type;
  children_layouter_to_string_t to_string;
  children_layouter_layout_t layout;
  children_layouter_is_valid_t is_valid;
  children_layouter_get_param_t get_param;
  children_layouter_set_param_t set_param;
  children_layouter_clone_t clone;
  children_layouter_destroy_t destroy;
} children_layouter_vtable_t;

/**
 * @class children_layouter_t
 * 子控件排版布局器的接口。
 *
 * 按特定算法对子控件进行排版布局，子类需要实现vtable中的函数。
 *
 */
struct _children_layouter_t {
  str_t params;
  const children_layouter_vtable_t* vt;
};

/**
 * @method children_layouter_to_string
 * 获取全部参数。
 * @param {children_layouter_t*} layouter layouter对象。
 *
 * @return {ret_t} 返回字符串格式的参数。
 */
const char* children_layouter_to_string(children_layouter_t* layouter);

/**
 * @method children_layouter_layout
 * 对控件的子控件进行布局。
 *
 * @param {children_layouter_t*} layouter layouter对象。
 * @param {widget_t*} widget 控件。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t children_layouter_layout(children_layouter_t* layouter, widget_t* widget);

/**
 * @method children_layouter_get_param
 * 获取指定的参数。
 *
 * @param {children_layouter_t*} layouter layouter对象。
 * @param {const char*} name 参数名。
 * @param {value_t*} v 返回参数的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t children_layouter_get_param(children_layouter_t* layouter, const char* name, value_t* v);

/**
 * @method children_layouter_set_param
 * 获取指定的参数。
 *
 * @param {children_layouter_t*} layouter layouter对象。
 * @param {const char*} name 参数名。
 * @param {const value_t*} v 参数的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t children_layouter_set_param(children_layouter_t* layouter, const char* name,
                                  const value_t* v);

/**
 * @method children_layouter_set_param_str
 * 设置字符串格式的参数。
 *
 * @param {children_layouter_t*} layouter layouter对象。
 * @param {const char*} name 参数名。
 * @param {const char*} value 参数值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t children_layouter_set_param_str(children_layouter_t* layouter, const char* name,
                                      const char* value);

/**
 * @method children_layouter_get_param_float
 * 获取指定的浮点格式的参数。
 *
 * @param {children_layouter_t*} layouter layouter对象。
 * @param {const char*} name 参数名。
 * @param {float_t} defval 缺省值。
 *
 * @return {ret_t} 成功返回参数的值，失败返回缺省值。
 */
float_t children_layouter_get_param_float(children_layouter_t* layouter, const char* name,
                                          float_t defval);

/**
 * @method children_layouter_get_param_int
 * 获取指定的整数格式的参数。
 *
 * @param {children_layouter_t*} layouter layouter对象。
 * @param {const char*} name 参数名。
 * @param {int32_t} defval 缺省值。
 *
 * @return {ret_t} 成功返回参数的值，失败返回缺省值。
 */
int32_t children_layouter_get_param_int(children_layouter_t* layouter, const char* name,
                                        int32_t defval);
/**
 * @method children_layouter_is_valid
 * 判断layouter对象是否有效。
 *
 * @param {children_layouter_t*} layouter layouter对象。
 *
 * @return {bool_t} 返回TRUE表示有效，否则表示无效。
 */
bool_t children_layouter_is_valid(children_layouter_t* layouter);

/**
 * @method children_layouter_destroy
 * 销毁layouter对象。
 *
 * @param {children_layouter_t*} layouter layouter对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t children_layouter_destroy(children_layouter_t* layouter);

/**
 * @method children_layouter_create
 * 创建layouter对象。
 * @param {const char*} params 参数。
 *
 * @return {children_layouter_t*} 返回layouter对象。
 */
children_layouter_t* children_layouter_create(const char* params);

/**
 * @method children_layouter_clone
 * 克隆layouter对象。
 * @param {children_layouter_t*} layouter 被克隆的对象。
 *
 * @return {children_layouter_t*} 返回layouter对象。
 */
children_layouter_t* children_layouter_clone(children_layouter_t* layouter);

END_C_DECLS

#endif /*TK_CHILDREN_LAYOUTER_H*/
