/**
 * File:   widget_animator_scroll.h
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its position.
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

#include "tkc/mem.h"
#include "widget_animators/widget_animator_scroll.h"

static ret_t widget_animator_scroll_update(widget_animator_t* animator, float_t percent) {
  int64_t xoffset = 0;
  int64_t yoffset = 0;
  value_t v;
  widget_animator_scroll_t* scroll = (widget_animator_scroll_t*)animator;
  return_value_if_fail(scroll != NULL, RET_BAD_PARAMS);

  if (scroll->x_to != scroll->x_from) {
    xoffset = scroll->x_from + (double)(scroll->x_to - scroll->x_from) * percent;
    value_set_int64(&v, xoffset);
    widget_set_prop(animator->widget, WIDGET_PROP_XOFFSET, &v);
  }

  if (scroll->y_to != scroll->y_from) {
    yoffset = scroll->y_from + (double)(scroll->y_to - scroll->y_from) * percent;
    value_set_int64(&v, yoffset);
    widget_set_prop(animator->widget, WIDGET_PROP_YOFFSET, &v);
  }

  return RET_OK;
}

widget_animator_t* widget_animator_scroll_create(widget_t* widget, uint32_t duration,
                                                 uint32_t delay, easing_type_t easing) {
  widget_animator_t* animator = NULL;
  return_value_if_fail(widget != NULL && duration > 0, NULL);

  animator = (widget_animator_t*)TKMEM_ZALLOC(widget_animator_scroll_t);
  return_value_if_fail(
      widget_animator_init(animator, widget, duration, delay, easing_get(easing)) == RET_OK, NULL);
  animator->update = widget_animator_scroll_update;

  return animator;
}

ret_t widget_animator_scroll_set_params(widget_animator_t* animator, xy_t x_from, xy_t y_from,
                                        xy_t x_to, xy_t y_to) {
  widget_animator_scroll_t* scroll = (widget_animator_scroll_t*)animator;
  return_value_if_fail(scroll != NULL, RET_BAD_PARAMS);

  scroll->x_to = x_to;
  scroll->y_to = y_to;
  scroll->x_from = x_from;
  scroll->y_from = y_from;

  return RET_OK;
}
