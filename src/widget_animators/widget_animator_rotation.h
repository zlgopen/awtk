/**
 * File:   widget_animator_rotation.h
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its rotation(just wrap widget_animator_prop)
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
 * 2018-05-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_ANIMATOR_ROTATION_H
#define TK_WIDGET_ANIMATOR_ROTATION_H

#include "widget_animators/widget_animator_prop.h"

BEGIN_C_DECLS

#define widget_animator_rotation_create(widget, duration, delay, easing) \
  widget_animator_prop_create(widget, duration, delay, easing, WIDGET_PROP_ROTATION)

#define widget_animator_rotation_set_params widget_animator_prop_set_params

END_C_DECLS

#endif /*TK_WIDGET_ANIMATOR_ROTATION_H*/
