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

static ret_t widget_animator_stop(widget_animator_t* animator);

static ret_t widget_animator_update(widget_animator_t* animator, float_t percent);

static ret_t widget_animator_on_widget_destroy(void* ctx, event_t* e) {
  widget_animator_t* animator = (widget_animator_t*)ctx;

  animator->widget_destroy_id = 0;
  widget_animator_destroy(animator);

  return RET_OK;
}

ret_t widget_animator_init(widget_animator_t* animator, widget_t* widget, uint32_t duration,
                           uint32_t delay, easing_func_t easing) {
  return_value_if_fail(animator != NULL && widget != NULL, RET_BAD_PARAMS);

  animator->widget = widget;
  animator->delay = delay;
  animator->duration = duration;
  emitter_init(&(animator->emitter));
  animator->easing = easing != NULL ? easing : easing_get(EASING_LINEAR);
  animator->widget_destroy_id =
      widget_on(widget, EVT_DESTROY, widget_animator_on_widget_destroy, animator);

  return RET_OK;
}

static ret_t widget_animator_on_timer(const timer_info_t* timer) {
  uint32_t now = timer->now;
  widget_animator_t* animator = (widget_animator_t*)(timer->ctx);
  uint32_t end_time = animator->start_time + animator->duration;
  float_t time_percent = ((float_t)(now - animator->start_time)) / animator->duration;

  if (time_percent > 1) {
    time_percent = 1;
  }

  if (animator->reversed) {
    time_percent = 1 - time_percent;
  }

  widget_animator_update(animator, animator->easing(time_percent));

  if (now >= end_time) {
    if (animator->repeat_times > 0) {
      event_t e = event_init(EVT_ANIM_ONCE, animator);
      animator->start_time = now;
      animator->repeat_times--;

      emitter_dispatch(&(animator->emitter), &e);
    } else if (animator->yoyo_times > 0) {
      event_t e = event_init(EVT_ANIM_ONCE, animator);
      animator->start_time = now;
      animator->reversed = !animator->reversed;
      animator->yoyo_times--;

      emitter_dispatch(&(animator->emitter), &e);
    } else {
      event_t e = event_init(EVT_ANIM_END, animator);
      emitter_dispatch(&(animator->emitter), &e);

      animator->timer_id = 0;
      widget_animator_destroy(animator);

      return RET_REMOVE;
    }
  }

  return RET_REPEAT;
}

static ret_t widget_animator_on_delay_timer(const timer_info_t* timer) {
  widget_animator_t* animator = (widget_animator_t*)timer->ctx;

  animator->start_time = timer_now();
  animator->timer_id = timer_add(widget_animator_on_timer, animator, 0);

  return RET_REMOVE;
}

ret_t widget_animator_start(widget_animator_t* animator) {
  event_t e = event_init(EVT_ANIM_START, animator);
  return_value_if_fail(animator != NULL && animator->timer_id == 0, RET_BAD_PARAMS);

  emitter_dispatch(&(animator->emitter), &e);

  if (animator->delay) {
    animator->start_time = timer_now() + animator->delay;
    animator->timer_id = timer_add(widget_animator_on_delay_timer, animator, animator->delay);
  } else {
    animator->start_time = timer_now();
    animator->timer_id = timer_add(widget_animator_on_timer, animator, 0);
  }

  return RET_OK;
}

ret_t widget_animator_stop(widget_animator_t* animator) {
  event_t e = event_init(EVT_ANIM_STOP, animator);
  return_value_if_fail(animator != NULL && animator->timer_id > 0, RET_BAD_PARAMS);

  timer_remove(animator->timer_id);
  animator->timer_id = 0;
  emitter_dispatch(&(animator->emitter), &e);

  return RET_OK;
}

static ret_t widget_animator_update(widget_animator_t* animator, float_t percent) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  return animator->update(animator, percent);
}

ret_t widget_animator_set_yoyo(widget_animator_t* animator, uint32_t yoyo_times) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  animator->yoyo_times = yoyo_times;

  return RET_OK;
}

ret_t widget_animator_set_repeat(widget_animator_t* animator, uint32_t repeat_times) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  animator->repeat_times = repeat_times;

  return RET_OK;
}

ret_t widget_animator_set_reversed(widget_animator_t* animator, bool_t value) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  animator->reversed = value;

  return RET_OK;
}

uint32_t widget_animator_on(widget_animator_t* animator, event_type_t type, event_func_t on_event,
                            void* ctx) {
  return_value_if_fail(animator != NULL && on_event != NULL, 0);

  return emitter_on(&(animator->emitter), type, on_event, ctx);
}

ret_t widget_animator_off(widget_animator_t* animator, uint32_t id) {
  return_value_if_fail(animator != NULL, RET_BAD_PARAMS);

  return emitter_off(&(animator->emitter), id);
}

ret_t widget_animator_destroy(widget_animator_t* animator) {
  return_value_if_fail(animator != NULL, RET_BAD_PARAMS);

  if (animator->widget_destroy_id) {
    widget_off(animator->widget, animator->widget_destroy_id);
    animator->widget_destroy_id = 0;
  }

  if (animator->timer_id) {
    widget_animator_stop(animator);
    animator->timer_id = 0;
  }

  emitter_deinit(&(animator->emitter));
  if (animator->destroy != NULL) {
    return animator->destroy(animator);
  } else {
    TKMEM_FREE(animator);
  }

  return RET_OK;
}
