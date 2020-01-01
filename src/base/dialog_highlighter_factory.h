/**
 * File:   dialog_highlighter_factory.h
 * Author: AWTK Develop Team
 * Brief:  dialog highlighter factory
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
 * 2018-03-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_DIALOG_HIGHLIGHTER_FACTORY_H
#define TK_DIALOG_HIGHLIGHTER_FACTORY_H

#include "tkc/darray.h"
#include "base/dialog_highlighter.h"

BEGIN_C_DECLS

/**
 * @class dialog_highlighter_factory_t
 * 对话框高亮策略工厂。
 *
 */
typedef struct _dialog_highlighter_factory_t {
  /*private*/
  darray_t creators;
} dialog_highlighter_factory_t;

/**
 * @method dialog_highlighter_factory
 * 获取缺省的对话框高亮策略工厂对象。
 * @annotation ["constructor"]
 * @return {dialog_highlighter_factory_t*} 返回对话框高亮策略工厂对象。
 */
dialog_highlighter_factory_t* dialog_highlighter_factory(void);

/**
 * @method dialog_highlighter_factory_set
 * 设置缺省的对话框高亮策略工厂对象。
 * @param {dialog_highlighter_factory_t*} factory 对话框高亮策略工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_highlighter_factory_set(dialog_highlighter_factory_t* factory);

/**
 * @method dialog_highlighter_factory_create
 * 创建对话框高亮策略工厂对象。
 * @annotation ["constructor"]
 *
 * @return {dialog_highlighter_factory_t*} 返回对话框高亮策略工厂对象。
 */
dialog_highlighter_factory_t* dialog_highlighter_factory_create(void);

/**
 * @method dialog_highlighter_factory_register
 * 注册对话框高亮策略创建函数。
 * @param {dialog_highlighter_factory_t*} factory highlighter工厂对象。
 * @param {const char*} type 对话框高亮策略类型。
 * @param {dialog_highlighter_create_t} create 对话框高亮策略创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_highlighter_factory_register(dialog_highlighter_factory_t* factory, const char* type,
                                          dialog_highlighter_create_t create);
/**
 * @method dialog_highlighter_factory_create_highlighter
 * 创建指定类型的对话框高亮策略对象。
 * @annotation ["constructor"]
 * @param {dialog_highlighter_factory_t*} factory 对话框高亮策略工厂对象。
 * @param {const char*} args 参数。如default(alpha=90)
 * @param {widget_t*} dialog 对话框对象。
 *
 * @return {dialog_highlighter_t*} 返回窗口动画对象。
 */
dialog_highlighter_t* dialog_highlighter_factory_create_highlighter(
    dialog_highlighter_factory_t* factory, const char* args, widget_t* dialog);

/**
 * @method dialog_highlighter_factory_destroy
 * 析构并释放对话框高亮策略工厂对象。
 * @param {dialog_highlighter_factory_t*} factory 对话框高亮策略工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_highlighter_factory_destroy(dialog_highlighter_factory_t* factory);

/*public for internal use*/
ret_t dialog_highlighter_on_dialog_destroy(void* ctx, event_t* e);

END_C_DECLS

#endif /*TK_DIALOG_HIGHLIGHTER_FACTORY_H*/
