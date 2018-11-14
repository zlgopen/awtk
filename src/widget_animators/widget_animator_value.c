/**
 * File:   widget_animator_value.h
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its value
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
 * 2018-05-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "widget_animators/widget_animator_value.h"

static ret_t widget_animator_value_update(widget_animator_t* animator, float_t percent) {
  value_t v;
  float_t new_value = 0;
  widget_animator_value_t* value = (widget_animator_value_t*)animator;
  return_value_if_fail(value != NULL, RET_BAD_PARAMS);

  new_value = value->from + (value->to - value->from) * percent;
  widget_set_prop(animator->widget, WIDGET_PROP_VALUE, value_set_float(&v, new_value));
  widget_invalidate(animator->widget, NULL);

  return RET_OK;
}

widget_animator_t* widget_animator_value_create(widget_t* widget, uint32_t duration, uint32_t delay,
                                                easing_type_t easing) {
  widget_animator_t* animator = NULL;
  return_value_if_fail(widget != NULL && duration > 0, NULL);

  animator = (widget_animator_t*)TKMEM_ZALLOC(widget_animator_value_t);
  return_value_if_fail(
      widget_animator_init(animator, widget, duration, delay, easing_get(easing)) == RET_OK, NULL);
  animator->update = widget_animator_value_update;

  return animator;
}

ret_t widget_animator_value_set_params(widget_animator_t* animator, int32_t from, int32_t to) {
  widget_animator_value_t* value = (widget_animator_value_t*)animator;
  return_value_if_fail(value != NULL, RET_BAD_PARAMS);

  value->to = to;
  value->from = from;

  return RET_OK;
}
