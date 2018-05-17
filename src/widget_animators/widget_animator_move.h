/**
 * File:   widget_animator_move.h
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its position.
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
 * 2018-05-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_ANIMATOR_MOVE_H
#define TK_WIDGET_ANIMATOR_MOVE_H

#include "base/widget_animator.h"

BEGIN_C_DECLS

/**
 * @class widget_animator_move_t
 * 移动控件位置动画接口。
 */
typedef struct _widget_animator_move_t {
  widget_animator_t base;

  xy_t x_to;
  xy_t y_to;
  xy_t x_from;
  xy_t y_from;
} widget_animator_move_t;

widget_animator_t* widget_animator_move_create(widget_t* widget, uint32_t duration,
                                               easing_func_t easing);
ret_t widget_animator_move_set_params(widget_animator_t* animator, xy_t x_from, xy_t y_from,
                                      xy_t x_to, xy_t y_to);

END_C_DECLS

#endif /*TK_WIDGET_ANIMATOR_MOVE_H*/
