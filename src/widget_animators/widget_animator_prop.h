/**
 * File:   widget_animator_prop.h
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its prop
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
 * 2018-12-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_ANIMATOR_PROP_H
#define TK_WIDGET_ANIMATOR_PROP_H

#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class widget_animator_prop_t
 * 通过修改对象的指定属性形成动画效果。
 */
typedef struct _widget_animator_prop_t {
  widget_animator_t base;

  double to;
  double from;
  char prop_name[TK_NAME_LEN + 1];
} widget_animator_prop_t;

/**
 * @method widget_animator_prop_create
 * 创建单属性动画对象。
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} duration 动画持续时间。
 * @param {uint32_t} delay 动画执行时间。
 * @param {easing_type_t} easing 插值函数类型。
 * @param {const char*} prop_name 属性的名称。
 *
 * @return {widget_animator_t*} 成功返回动画对象，失败返回NULL。
 */
widget_animator_t* widget_animator_prop_create(widget_t* widget, uint32_t duration, uint32_t delay,
                                               easing_type_t easing, const char* prop_name);

/**
 * @method widget_animator_prop_set_params
 * 设置动画对象的参数。
 * @param {widget_animator_t*} animator 动画对象本身。
 * @param {double} from prop起始值。
 * @param {double} to prop结束值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_prop_set_params(widget_animator_t* animator, double from, double to);

END_C_DECLS

#endif /*TK_WIDGET_ANIMATOR_PROP_H*/
