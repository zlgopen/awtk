/**
 * File:   widget_animator_scale.h
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
 * 2018-05-31 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "widget_animators/widget_animator_scale.h"

static ret_t widget_animator_scale_update(widget_animator_t* animator, float_t percent) {
  float_t scale_x = 0;
  float_t scale_y = 0;
  widget_animator_scale_t* scale = (widget_animator_scale_t*)animator;
  return_value_if_fail(scale != NULL, RET_BAD_PARAMS);

  scale_x = scale->x_from + (scale->x_to - scale->x_from) * percent;
  scale_y = scale->y_from + (scale->y_to - scale->y_from) * percent;
  widget_set_scale(animator->widget, scale_x, scale_y);

  return RET_OK;
}

widget_animator_t* widget_animator_scale_create(widget_t* widget, uint32_t duration, uint32_t delay,
                                               easing_func_t easing) {
  widget_animator_t* animator = NULL;
  return_value_if_fail(widget != NULL && duration > 0, NULL);

  animator = (widget_animator_t*)TKMEM_ZALLOC(widget_animator_scale_t);
  return_value_if_fail(widget_animator_init(animator, widget, duration, delay, easing) == RET_OK, NULL);
  animator->update = widget_animator_scale_update;

  return animator;
}

ret_t widget_animator_scale_set_params(widget_animator_t* animator, float_t x_from, float_t y_from,
                                      float_t x_to, float_t y_to) {
  widget_animator_scale_t* scale = (widget_animator_scale_t*)animator;
  return_value_if_fail(scale != NULL, RET_BAD_PARAMS);

  scale->x_to = x_to;
  scale->y_to = y_to;
  scale->x_from = x_from;
  scale->y_from = y_from;

  return RET_OK;
}
