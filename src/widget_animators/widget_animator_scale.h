/**
 * File:   widget_animator_scale.h
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its scale.
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

#ifndef TK_WIDGET_ANIMATOR_SCALE_H
#define TK_WIDGET_ANIMATOR_SCALE_H

#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class widget_animator_scale_t
 * 控件缩放动画。
 */
typedef struct _widget_animator_scale_t {
  widget_animator_t base;

  float_t x_to;
  float_t y_to;
  float_t x_from;
  float_t y_from;
} widget_animator_scale_t;

/**
 * @method widget_animator_scale_create
 * 创建动画对象。
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} duration 动画持续时间。
 * @param {uint32_t} delay 动画执行时间。
 * @param {easing_type_t} easing 插值函数类型。
 *
 * @return {widget_animator_t*} 成功返回动画对象，失败返回NULL。
 */
widget_animator_t* widget_animator_scale_create(widget_t* widget, uint32_t duration, uint32_t delay,
                                                easing_type_t easing);

/**
 * @method widget_animator_scale_set_params
 * 设置动画对象的参数。
 * @param {widget_animator_t*} animator 动画对象本身。
 * @param {xy_t} x_from x方向起始缩放比例。
 * @param {xy_t} y_from y方向起始缩放比例。
 * @param {xy_t} x_to x方向结束缩放比例。
 * @param {xy_t} y_to y方向结束缩放比例。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_scale_set_params(widget_animator_t* animator, float_t x_from, float_t y_from,
                                       float_t x_to, float_t y_to);

END_C_DECLS

#endif /*TK_WIDGET_ANIMATOR_SCALE_H*/
