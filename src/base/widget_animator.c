/**
 * File:   widget_animator.h
 * Author: AWTK Develop Team
 * Brief:  widget animator interface
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
#include "base/time.h"
#include "base/timer.h"
#include "base/widget_animator.h"

ret_t widget_animator_init(widget_animator_t* animator, widget_t* widget, uint32_t duration, easing_func_t easing) {
  return_value_if_fail(animator != NULL && widget != NULL, RET_BAD_PARAMS);
  
  animator->widget = widget;
  animator->duration = duration;
  animator->easing = easing != NULL ? easing : easing_get(EASING_LINEAR);

  return RET_OK;
}

static ret_t widget_animator_on_timer(const timer_info_t* timer) {
  uint32_t now = timer->now;
  widget_animator_t* animator = (widget_animator_t*)(timer->ctx);
  uint32_t end_time = animator->start_time + animator->duration;
  float_t time_percent = ((float_t)(now - animator->start_time))/animator->duration;

  if(time_percent > 1) {
    time_percent = 1;
  }

  if(animator->reversed) {
    time_percent = 1 - time_percent;
  }

  widget_animator_update(animator, animator->easing(time_percent));

  if(now >= end_time) {
    if(animator->repeat) {
      animator->start_time = now;
    } else if(animator->yoyo) {
      animator->start_time = now;
      animator->reversed = !animator->reversed;
    } else {
      widget_animator_destroy(animator);
      return RET_REMOVE;
    }
  }

  return RET_REPEAT;
}

ret_t widget_animator_start(widget_animator_t* animator) {
  return_value_if_fail(animator != NULL && animator->timer_id == 0, RET_BAD_PARAMS);

  animator->start_time = time_now_ms();
  animator->timer_id = timer_add(widget_animator_on_timer, animator, 0); 

  return RET_OK;
}

ret_t widget_animator_stop(widget_animator_t* animator) {
  return_value_if_fail(animator != NULL && animator->timer_id > 0, RET_BAD_PARAMS);

  timer_remove(animator->timer_id);
  animator->timer_id = 0;

  return RET_OK;
}

ret_t widget_animator_update(widget_animator_t* animator, float_t percent) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  return animator->update(animator, percent);
}

ret_t widget_animator_set_yoyo(widget_animator_t* animator, bool_t value) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  animator->yoyo = value;

  return RET_OK;
}

ret_t widget_animator_set_repeat(widget_animator_t* animator, bool_t value) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  animator->repeat = value;

  return RET_OK;
}

ret_t widget_animator_set_reversed(widget_animator_t* animator, bool_t value) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  animator->reversed = value;

  return RET_OK;
}

ret_t widget_animator_destroy(widget_animator_t* animator) {
  return_value_if_fail(animator != NULL, RET_BAD_PARAMS);

  if(animator->destroy != NULL) {
    return animator->destroy(animator);
  } else {
    TKMEM_FREE(animator);
  }

  return RET_OK;
}

