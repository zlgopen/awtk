/**
 * File:   widget_animator_prop.c
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its prop
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
 * 2018-12-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "widget_animators/widget_animator_prop.h"

static ret_t widget_animator_prop_update(widget_animator_t* animator, float_t percent) {
  value_t v;
  double new_prop = 0;
  widget_animator_prop_t* prop = (widget_animator_prop_t*)animator;
  return_value_if_fail(prop != NULL, RET_BAD_PARAMS);

  new_prop = prop->from + (prop->to - prop->from) * percent;
  widget_set_prop(animator->widget, prop->prop_name, value_set_double(&v, new_prop));

  return RET_OK;
}

widget_animator_t* widget_animator_prop_create(widget_t* widget, uint32_t duration, uint32_t delay,
                                               easing_type_t easing, const char* prop_name) {
  widget_animator_t* animator = NULL;
  widget_animator_prop_t* prop = NULL;
  return_value_if_fail(widget != NULL && duration > 0 && prop_name != NULL, NULL);

  animator = (widget_animator_t*)TKMEM_ZALLOC(widget_animator_prop_t);
  return_value_if_fail(animator != NULL, NULL);

  return_value_if_fail(
      widget_animator_init(animator, widget, duration, delay, easing_get(easing)) == RET_OK, NULL);

  prop = (widget_animator_prop_t*)animator;
  animator->update = widget_animator_prop_update;
  tk_strncpy(prop->prop_name, prop_name, TK_NAME_LEN);

  return animator;
}

ret_t widget_animator_prop_set_params(widget_animator_t* animator, double from, double to) {
  widget_animator_prop_t* prop = (widget_animator_prop_t*)animator;
  return_value_if_fail(prop != NULL, RET_BAD_PARAMS);

  prop->to = to;
  prop->from = from;

  return RET_OK;
}
