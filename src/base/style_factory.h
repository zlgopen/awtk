/**
 * File:   style_factory.h
 * Author: AWTK Develop Team
 * Brief:  style factory
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
 * 2018-10-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_STYLE_FACTORY_H
#define TK_STYLE_FACTORY_H

#include "base/style.h"

BEGIN_C_DECLS

struct _style_factory_t;
typedef struct _style_factory_t style_factory_t;

typedef style_t* (*style_factory_create_style_t)(style_factory_t* factory, widget_t* widget);

/**
 * @class style_factory_t
 * style工厂接口，主要用于创建style。
 *
 *> 在特殊情况下，可以实现自己的style工厂，比如支持CSS和可实时修改的style。
 *
 */
struct _style_factory_t {
  style_factory_create_style_t create_style;
};

/**
 * @method style_factory_create_style
 * 创建style。
 * @param {style_factory_t*} factory factory对象。
 * @param {widget_t*} widget 控件对象。
 *
 * @return {style_t*} 返回style对象。
 */
style_t* style_factory_create_style(style_factory_t* factory, widget_t* widget);

/**
 * @method style_factory
 * 获取缺省的style factory。
 * @alias style_factory_instance
 * @annotation ["constructor"]
 * @return {style_factory_t*} 返回style factory对象。
 */
style_factory_t* style_factory(void);

/**
 * @method style_factory_set
 * 设置缺省的style factory(需要自定义的style factory才设置)。
 * @param {style_factory_t*}  factory factory对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t style_factory_set(style_factory_t* factory);

END_C_DECLS

#endif /*TK_STYLE_FACTORY_H*/
