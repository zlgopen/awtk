/**
 * File:   widget_animator_opacity.h
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its opacity
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
 * 2018-05-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_ANIMATOR_OPACITY_H
#define TK_WIDGET_ANIMATOR_OPACITY_H

#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class widget_animator_opacity_t
 * 改变控件透明度的动画。
 */
typedef struct _widget_animator_opacity_t {
  widget_animator_t base;

  uint8_t to;
  uint8_t from;
} widget_animator_opacity_t;

/**
 * @method widget_animator_opacity_create
 * 创建动画对象。
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} duration 动画持续时间。
 * @param {uint32_t} delay 动画执行时间。
 * @param {easing_type_t} easing 插值函数类型。
 *
 * @return {widget_animator_t*} 成功返回动画对象，失败返回NULL。
 */
widget_animator_t* widget_animator_opacity_create(widget_t* widget, uint32_t duration,
                                                  uint32_t delay, easing_type_t easing);

/**
 * @method widget_animator_opacity_set_params
 * 设置动画对象的参数。
 * @param {widget_animator_t*} animator 动画对象本身。
 * @param {uint8_t} from opacity起始值。
 * @param {uint8_t} to opacity结束值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_opacity_set_params(widget_animator_t* animator, uint8_t from, uint8_t to);

END_C_DECLS

#endif /*TK_WIDGET_ANIMATOR_OPACITY_H*/
