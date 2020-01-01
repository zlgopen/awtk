/**
 * File:   widget_animator_scroll.h
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its xoffset/yoffset.
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
 * 2018-05-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_ANIMATOR_SCROLL_H
#define TK_WIDGET_ANIMATOR_SCROLL_H

#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class widget_animator_scroll_t
 * 滚动控件的动画。
 * 本动画也可以用widget_animator_prop2实现，但滚动控件需要访问内部数据结构，出于可读性考虑保留独立实现。
 */
typedef struct _widget_animator_scroll_t {
  widget_animator_t base;

  xy_t x_to;
  xy_t y_to;
  xy_t x_from;
  xy_t y_from;
} widget_animator_scroll_t;

/**
 * @method widget_animator_scroll_create
 * 创建动画对象。
 * @param {widget_t*} widget 控件对象。
 * @param {uint32_t} duration 动画持续时间。
 * @param {uint32_t} delay 动画执行时间。
 * @param {easing_type_t} easing 插值函数类型。
 *
 * @return {widget_animator_t*} 成功返回动画对象，失败返回NULL。
 */
widget_animator_t* widget_animator_scroll_create(widget_t* widget, uint32_t duration,
                                                 uint32_t delay, easing_type_t easing);

/**
 * @method widget_animator_scroll_set_params
 * 设置动画对象的参数。
 * @param {widget_animator_t*} animator 动画对象本身。
 * @param {xy_t} x_from x起点值。
 * @param {xy_t} y_from y起点值。
 * @param {xy_t} x_to x终点值。
 * @param {xy_t} y_to y终点值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t widget_animator_scroll_set_params(widget_animator_t* animator, xy_t x_from, xy_t y_from,
                                        xy_t x_to, xy_t y_to);

END_C_DECLS

#endif /*TK_WIDGET_ANIMATOR_SCROLL_H*/
