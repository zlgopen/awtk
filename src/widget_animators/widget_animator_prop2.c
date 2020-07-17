/**
 * File:   widget_animator_prop2.h
 * Author: AWTK Develop Team
 * Brief:  animate widget by change its 2 props.
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
#include "widget_animators/widget_animator_prop2.h"

static ret_t widget_animator_prop2_update(widget_animator_t* animator, float_t percent) {
  value_t v;
  widget_animator_prop2_t* prop2 = (widget_animator_prop2_t*)animator;
  return_value_if_fail(prop2 != NULL, RET_BAD_PARAMS);

  if (prop2->to1 != prop2->from1) {
    value_set_double(&v, prop2->from1 + (prop2->to1 - prop2->from1) * percent);
    widget_set_prop(animator->widget, prop2->prop1_name, &v);
  }

  if (prop2->to2 != prop2->from2) {
    value_set_double(&v, prop2->from2 + (prop2->to2 - prop2->from2) * percent);
    widget_set_prop(animator->widget, prop2->prop2_name, &v);
  }

  return RET_OK;
}

widget_animator_t* widget_animator_prop2_create(widget_t* widget, uint32_t duration, uint32_t delay,
                                                easing_type_t easing, const char* prop1_name,
                                                const char* prop2_name) {
  widget_animator_t* animator = NULL;
  widget_animator_prop2_t* prop2 = NULL;
  return_value_if_fail(widget != NULL && duration > 0 && prop1_name != NULL && prop2_name != NULL,
                       NULL);

  animator = (widget_animator_t*)TKMEM_ZALLOC(widget_animator_prop2_t);
  return_value_if_fail(animator != NULL, NULL);

  return_value_if_fail(
      widget_animator_init(animator, widget, duration, delay, easing_get(easing)) == RET_OK, NULL);

  prop2 = (widget_animator_prop2_t*)animator;
  animator->update = widget_animator_prop2_update;
  tk_strncpy(prop2->prop1_name, prop1_name, TK_NAME_LEN);
  tk_strncpy(prop2->prop2_name, prop2_name, TK_NAME_LEN);

  return animator;
}

ret_t widget_animator_prop2_set_params(widget_animator_t* animator, double from1, double from2,
                                       double to1, double to2) {
  widget_animator_prop2_t* prop2 = (widget_animator_prop2_t*)animator;
  return_value_if_fail(prop2 != NULL, RET_BAD_PARAMS);

  prop2->to1 = to1;
  prop2->to2 = to2;
  prop2->from1 = from1;
  prop2->from2 = from2;

  return RET_OK;
}
