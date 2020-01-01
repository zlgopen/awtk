/**
 * File:   widget_factory.h
 * Author: AWTK Develop Team
 * Brief:  widget factory
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
 * 2018-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_FACTORY_H
#define TK_WIDGET_FACTORY_H

#include "tkc/darray.h"
#include "tkc/emitter.h"
#include "base/widget.h"

BEGIN_C_DECLS

/**
 * @class widget_factory_t
 * @parent emitter
 * 控件工厂对象。
 *
 * 提供通过类型创建控件的能力。
 * 用户注册自定义控件，可以获得内置控件同等待遇。
 *
 */
typedef struct _widget_factory_t {
  emitter_t emitter;
  /*private*/
  darray_t creators;
} widget_factory_t;

/**
 * @method widget_factory
 * 获取缺省的控件工厂对象。
 * @alias widget_factory_instance
 * @annotation ["constructor"]
 * @return {widget_factory_t*} 返回控件工厂对象。
 */
widget_factory_t* widget_factory(void);

/**
 * @method widget_factory_set
 * 设置缺省的控件工厂对象。
 * @param {widget_factory_t*} factory 控件工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_factory_set(widget_factory_t* factory);

/**
 * @method widget_factory_create
 * 创建控件工厂对象。
 * @annotation ["constructor"]
 *
 * @return {widget_factory_t*} 返回控件工厂对象。
 */
widget_factory_t* widget_factory_create(void);

/**
 * @method widget_factory_register
 * 注册控件创建函数。
 * @param {widget_factory_t*} factory 控件工厂对象。
 * @param {const char*} type 控件类型。
 * @param {widget_create_t} create 创建函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_factory_register(widget_factory_t* factory, const char* type, widget_create_t create);

/**
 * @method widget_factory_create_widget
 * 创建指定类型的控件。
 * @annotation ["constructor"]
 * @param {widget_factory_t*} factory 控件工厂对象。
 * @param {const char*} type 控件类型。
 * @param {xy_t} x 初始X坐标。
 * @param {xy_t} y 初始Y坐标。
 * @param {xy_t} w 初始宽度。
 * @param {xy_t} h 初始高度。
 *
 * @return {widget_t*} 返回控件对象。
 */
widget_t* widget_factory_create_widget(widget_factory_t* factory, const char* type,
                                       widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/**
 * @method widget_factory_destroy
 * 析构并释放控件工厂对象。
 * @param {widget_factory_t*} factory 控件工厂对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_factory_destroy(widget_factory_t* factory);

END_C_DECLS

#endif /*TK_WIDGET_FACTORY_H*/
