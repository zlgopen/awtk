/**
 * File:   object_widget.h
 * Author: AWTK Develop Team
 * Brief:  wrap widget to an object.
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-09-22 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#ifndef TK_OBJECT_WIDGET_H
#define TK_OBJECT_WIDGET_H

#include "widget.h"
#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @class object_widget_t
 * @parent tk_object_t
 *
 * 将widget包装成object。
 * > 备注：主要用于 fscript 实现 widget.prop 方式访问控件属性。
 */
typedef struct _object_widget_t {
  tk_object_t object;

  /**
   * @property {widget_t*} widget
   * @annotation ["readable"]
   * widget对象。
   *
   */
  widget_t* widget;
} object_widget_t;

/**
 * @method object_widget_create
 * 创建widget对象。
 *
 * @annotation ["constructor"]
 * @param {widget_t*} widget 控件。
 *
 * @return {tk_object_t*} 返回object对象。
 *
 */
tk_object_t* object_widget_create(widget_t* widget);

object_widget_t* object_widget_cast(tk_object_t* obj);
#define OBJECT_WIDGET(obj) object_widget_cast(obj)

#define OBJECT_WIDGET_PROP_NATIVE_WIDGET "$widget"

END_C_DECLS

#endif /*TK_OBJECT_WIDGET_H*/
