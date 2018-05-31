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

#include "base/mem.h"
#include "widget_animators/widget_animator_move.h"

static ret_t widget_animator_move_update(widget_animator_t* animator, float_t percent) {
  widget_animator_move_t* move = (widget_animator_move_t*)animator;
  return_value_if_fail(move != NULL, RET_BAD_PARAMS);

  animator->widget->x = move->x_from + (move->x_to - move->x_from) * percent;
  animator->widget->y = move->y_from + (move->y_to - move->y_from) * percent;
  widget_invalidate(animator->widget, NULL);

  return RET_OK;
}

widget_animator_t* widget_animator_move_create(widget_t* widget, uint32_t duration, uint32_t delay,
                                               easing_type_t easing) {
  widget_animator_t* animator = NULL;
  return_value_if_fail(widget != NULL && duration > 0, NULL);

  animator = (widget_animator_t*)TKMEM_ZALLOC(widget_animator_move_t);
  return_value_if_fail(
      widget_animator_init(animator, widget, duration, delay, easing_get(easing)) == RET_OK, NULL);
  animator->update = widget_animator_move_update;

  return animator;
}

ret_t widget_animator_move_set_params(widget_animator_t* animator, xy_t x_from, xy_t y_from,
                                      xy_t x_to, xy_t y_to) {
  widget_animator_move_t* move = (widget_animator_move_t*)animator;
  return_value_if_fail(move != NULL, RET_BAD_PARAMS);

  move->x_to = x_to;
  move->y_to = y_to;
  move->x_from = x_from;
  move->y_from = y_from;

  return RET_OK;
}
