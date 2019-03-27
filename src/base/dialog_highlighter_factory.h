/**
 * File:   dialog_highlighter_factory.h
 * Author: AWTK Develop Team
 * Brief:  dialog highlighter factory
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
 * highlighter工厂对象。
 *
 *
 */
typedef struct _dialog_highlighter_factory_t {
  /*private*/
  darray_t creators;
} dialog_highlighter_factory_t;

/**
 * @method dialog_highlighter_factory
 * 获取缺省的highlighter工厂对象。
 * @alias dialog_highlighter_factory_instance
 * @annotation ["constructor"]
 * @return {dialog_highlighter_factory_t*} 返回highlighter工厂对象。
 */
dialog_highlighter_factory_t* dialog_highlighter_factory(void);

/**
 * @method dialog_highlighter_factory_set
 * 设置缺省的highlighter工厂对象。
 * @param {dialog_highlighter_factory_t*} factory highlighter工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_highlighter_factory_set(dialog_highlighter_factory_t* factory);

/**
 * @method dialog_highlighter_factory_create
 * 创建highlighter工厂对象。
 * @annotation ["constructor"]
 *
 * @return {dialog_highlighter_factory_t*} 返回highlighter工厂对象。
 */
dialog_highlighter_factory_t* dialog_highlighter_factory_create(void);

/**
 * @method dialog_highlighter_factory_register
 * 注册窗口动画创建函数。
 * @param {dialog_highlighter_factory_t*} factory highlighter工厂对象。
 * @param {const char*} type 窗口动画类型。
 * @param {dialog_highlighter_create_t} create 创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_highlighter_factory_register(dialog_highlighter_factory_t* factory, const char* type,
                                       dialog_highlighter_create_t create);

/**
 * @method dialog_highlighter_factory_create_highlighter
 * 创建指定类型的窗口动画。
 * @annotation ["constructor"]
 * @param {dialog_highlighter_factory_t*} factory highlighter工厂对象。
 * @param {const char*} args 参数。
 *
 * @return {dialog_highlighter_t*} 返回窗口动画对象。
 */
dialog_highlighter_t* dialog_highlighter_factory_create_highlighter(
    dialog_highlighter_factory_t* factory, const char* args);

/**
 * @method dialog_highlighter_factory_destroy
 * 析构并释放highlighter工厂对象。
 * @param {dialog_highlighter_factory_t*} factory highlighter工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t dialog_highlighter_factory_destroy(dialog_highlighter_factory_t* factory);

END_C_DECLS

#endif /*TK_DIALOG_HIGHLIGHTER_FACTORY_H*/
