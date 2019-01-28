/**
 * File:   model_delegate.h
 * Author: AWTK Develop Team
 * Brief:  model_delegate
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
 * 2019-01-24 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_MODEL_DELEGATE_H
#define TK_MODEL_DELEGATE_H

#include "mvvm/base/model.h"

BEGIN_C_DECLS

struct _model_delegate_t;
typedef struct _model_delegate_t model_delegate_t;

typedef ret_t (*model_get_prop_t)(void* obj, value_t* value);
typedef ret_t (*model_set_prop_t)(void* obj, const value_t* value);
typedef ret_t (*model_exec_t)(void* obj, const char* args);
typedef bool_t (*model_can_exec_t)(void* obj, const char* args);

/**
 * @class model_delegate_t
 * @parent model_t
 *
 * 将简单对象包装成model接口。
 *
 */
struct _model_delegate_t {
  model_t model;

  /*private*/
  void* obj;
  tk_destroy_t obj_destroy;

  object_t* props;
  object_t* commands;
};

/**
 * @method model_delegate_create
 * 创建model_delegate对象。
 *
 * @annotation ["scriptable"]
 * @param {void*} obj 被包装的对象。
 * @param {tk_destroy_t} obj_destroy 被包装的对象的析构函数。
 *
 * @return {model_t} 返回model对象。
 */
model_t* model_delegate_create(void* obj, tk_destroy_t obj_destroy);

/**
 * @method model_delegate_install_command
 * 安装一个命令。
 *
 * @param {model_t*} model model对象。
 * @param {const char*} name 命令名称。
 * @param {model_exec_t} exec 执行函数。
 * @param {model_can_exec_t} can_exec 检查是否可以执行的函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t model_delegate_install_command(model_t* model, const char* name, model_exec_t exec,
                                     model_can_exec_t can_exec);

/**
 * @method model_delegate_install_prop
 * 安装一个属性。在访问一个属性时，优先使用get/set函数，其次通过地址访问。
 *
 * @param {model_t*} model model对象。
 * @param {const char*} name 属性名称。
 * @param {value_type_t} type 属性的类型(可选)。
 * @param {void*} addr 属性的地址(可选)。
 * @param {model_get_prop_t} get get函数(可选)。
 * @param {model_set_prop_t} set set函数(可选)。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t model_delegate_install_prop(model_t* model, const char* name, value_type_t type, void* addr,
                                  model_get_prop_t get, model_set_prop_t set);

#define MODEL_DELEGATE(model) ((model_delegate_t*)(model))

#define MODEL_COMMAND(model, name, exec, can_exec) \
  model_delegate_install_command(model, name, (model_exec_t)exec, (model_can_exec_t)can_exec)

#define MODEL_PROP(model, name, get, set)                                                   \
  model_delegate_install_prop(model, name, VALUE_TYPE_INVALID, NULL, (model_get_prop_t)get, \
                              (model_set_prop_t)set)

#define MODEL_SIMPLE_PROP(model, name, type, addr) \
  model_delegate_install_prop(model, name, type, addr, NULL, NULL)

END_C_DECLS

#endif /*TK_MODEL_DELEGATE_H*/
