/**
 * File:   widget_animator_rotation.h
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its rotation
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-05-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_ANIMATOR_ROTATION_H
#define TK_WIDGET_ANIMATOR_ROTATION_H

#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class widget_animator_rotation_t
 * 改变控件旋转角度的动画。
 */
typedef struct _widget_animator_rotation_t {
  widget_animator_t base;

  float_t to;
  float_t from;
} widget_animator_rotation_t;

/**
 * @method widget_animator_value_create
 * 创建动画对象。
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} duration 动画持续时间。
 * @param {uint32_t} delay 动画执行时间。
 * @param {easing_type_t} easing 插值函数类型。
 *
 * @return {widget_animator_t*} 成功返回动画对象，失败返回NULL。
 */
widget_animator_t* widget_animator_rotation_create(widget_t* widget, uint32_t duration,
                                                   uint32_t delay, easing_type_t easing);

/**
 * @method widget_animator_rotation_set_params
 * 设置动画对象的参数。
 * @param {widget_animator_t*} animator 动画对象本身。
 * @param {float_t} from rotation起始值。
 * @param {float_t} to rotation结束值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_rotation_set_params(widget_animator_t* animator, float_t from, float_t to);

END_C_DECLS

#endif /*TK_WIDGET_ANIMATOR_ROTATION_H*/
