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

#include "base/mem.h"
#include "widget_animators/widget_animator_rotation.h"

static ret_t widget_animator_rotation_update(widget_animator_t* animator, float_t percent) {
  value_t v;
  float_t new_rotation = 0;
  widget_animator_rotation_t* rotation = (widget_animator_rotation_t*)animator;
  return_value_if_fail(rotation != NULL, RET_BAD_PARAMS);

  new_rotation = rotation->from + (rotation->to - rotation->from) * percent;

  value_set_float(&v, new_rotation);
  widget_set_prop(animator->widget, WIDGET_PROP_ROTATION, &v);

  return RET_OK;
}

widget_animator_t* widget_animator_rotation_create(widget_t* widget, uint32_t duration,
                                                   uint32_t delay, easing_type_t easing) {
  widget_animator_t* animator = NULL;
  return_value_if_fail(widget != NULL && duration > 0, NULL);

  animator = (widget_animator_t*)TKMEM_ZALLOC(widget_animator_rotation_t);
  return_value_if_fail(
      widget_animator_init(animator, widget, duration, delay, easing_get(easing)) == RET_OK, NULL);
  animator->update = widget_animator_rotation_update;

  return animator;
}

ret_t widget_animator_rotation_set_params(widget_animator_t* animator, float_t from, float_t to) {
  widget_animator_rotation_t* rotation = (widget_animator_rotation_t*)animator;
  return_value_if_fail(rotation != NULL, RET_BAD_PARAMS);

  rotation->to = to;
  rotation->from = from;

  return RET_OK;
}
