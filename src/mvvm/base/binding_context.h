/**
 * File:   binding_context.h
 * Author: AWTK Develop Team
 * Brief:  binding context
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
 * 2019-01-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_BINDING_CONTEXT_H
#define TK_BINDING_CONTEXT_H

#include "tkc/darray.h"
#include "mvvm/base/types_def.h"
#include "mvvm/base/view_model.h"

BEGIN_C_DECLS

typedef ret_t (*binding_context_bind_t)(binding_context_t* ctx, view_model_t* vm, void* widget);
typedef ret_t (*binding_context_update_to_view_t)(binding_context_t* ctx);
typedef ret_t (*binding_context_update_to_model_t)(binding_context_t* ctx);
typedef ret_t (*binding_context_destroy_t)(binding_context_t* ctx);

typedef struct _binding_context_vtable_t {
  binding_context_bind_t bind;
  binding_context_update_to_view_t update_to_view;
  binding_context_update_to_model_t update_to_model;
  binding_context_destroy_t destroy;
} binding_context_vtable_t;

/**
 * @class binding_context_t
 * @annotation ["scriptable"]
 *
 */
struct _binding_context_t {
  darray_t command_bindings;
  darray_t data_bindings;
  view_model_t* vm;
  widget_t* widget;
  widget_t* current_widget;

  bool_t bound;
  bool_t updating_view;
  bool_t updating_model;
  const binding_context_vtable_t* vt;
};

/**
 * @method binding_context_init
 * 初始化。
 *
 * @param {binding_context_t*} ctx ctx对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_init(binding_context_t* ctx);

/**
 * @method binding_context_bind
 * 绑定vm与widget
 *
 * @annotation ["scriptable"]
 * @param {binding_context_t*} ctx ctx对象。
 * @param {view_model_t*} vm view model对象。
 * @param {void*} widget widget对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_bind(binding_context_t* ctx, view_model_t* vm, void* widget);

/**
 * @method binding_context_update_to_view
 * 更新数据到视图。
 *
 * @annotation ["scriptable"]
 * @param {binding_context_t*} ctx ctx对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_update_to_view(binding_context_t* ctx);

/**
 * @method binding_context_update_to_model
 * 更新数据到模型。
 *
 * @annotation ["scriptable"]
 * @param {binding_context_t*} ctx ctx对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_update_to_model(binding_context_t* ctx);

/**
 * @method binding_context_destroy
 * 销毁binding context对象。
 *
 * @annotation ["scriptable"]
 * @param {binding_context_t*} ctx ctx对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t binding_context_destroy(binding_context_t* ctx);

END_C_DECLS

#endif /*TK_BINDING_CONTEXT_H*/
