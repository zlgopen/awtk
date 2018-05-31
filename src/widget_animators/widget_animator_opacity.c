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

#include "base/mem.h"
#include "widget_animators/widget_animator_opacity.h"

static ret_t widget_animator_opacity_update(widget_animator_t* animator, float_t percent) {
  uint8_t new_opacity = 0;
  widget_animator_opacity_t* opacity = (widget_animator_opacity_t*)animator;
  return_value_if_fail(opacity != NULL, RET_BAD_PARAMS);

  new_opacity = opacity->from + (opacity->to - opacity->from) * percent;
  widget_set_opacity(animator->widget, new_opacity);

  return RET_OK;
}

widget_animator_t* widget_animator_opacity_create(widget_t* widget, uint32_t duration,
                                                  uint32_t delay, easing_type_t easing) {
  widget_animator_t* animator = NULL;
  return_value_if_fail(widget != NULL && duration > 0, NULL);

  animator = (widget_animator_t*)TKMEM_ZALLOC(widget_animator_opacity_t);
  return_value_if_fail(
      widget_animator_init(animator, widget, duration, delay, easing_get(easing)) == RET_OK, NULL);
  animator->update = widget_animator_opacity_update;

  return animator;
}

ret_t widget_animator_opacity_set_params(widget_animator_t* animator, uint8_t from, uint8_t to) {
  widget_animator_opacity_t* opacity = (widget_animator_opacity_t*)animator;
  return_value_if_fail(opacity != NULL, RET_BAD_PARAMS);

  opacity->to = to;
  opacity->from = from;

  return RET_OK;
}
