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
 * animate widget by change its rotation。
 */
typedef struct _widget_animator_rotation_t {
  widget_animator_t base;

  float_t to;
  float_t from;
} widget_animator_rotation_t;

widget_animator_t* widget_animator_rotation_create(widget_t* widget, uint32_t duration, uint32_t delay,
                                                  easing_func_t easing);
ret_t widget_animator_rotation_set_params(widget_animator_t* animator, float_t from, float_t to);

END_C_DECLS

#endif /*TK_WIDGET_ANIMATOR_ROTATION_H*/
