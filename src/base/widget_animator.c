/**
 * File:   widget_animator.h
 * Author: AWTK Develop Team
 * Brief:  widget animator interface
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
#include "tkc/utils.h"
#include "base/widget_animator.h"
#include "base/widget_animator_manager.h"

static ret_t widget_animator_update(widget_animator_t* animator, float_t percent);

static ret_t widget_animator_on_widget_destroy(void* ctx, event_t* e) {
  widget_animator_t* animator = (widget_animator_t*)ctx;

  animator->widget_destroy_id = TK_INVALID_ID;
  widget_animator_destroy(animator);

  return RET_OK;
}

ret_t widget_animator_init(widget_animator_t* animator, widget_t* widget, uint32_t duration,
                           uint32_t delay, easing_func_t easing) {
  return_value_if_fail(animator != NULL && widget != NULL, RET_BAD_PARAMS);

  animator->delay = delay;
  animator->widget = widget;
  animator->repeat_times = 0;
  animator->time_scale = 1;
  animator->duration = duration;
  animator->state = ANIMATOR_CREATED;
  emitter_init(&(animator->emitter));
  animator->easing = easing != NULL ? easing : easing_get(EASING_LINEAR);
  animator->widget_destroy_id =
      widget_on(widget, EVT_DESTROY, widget_animator_on_widget_destroy, animator);
  widget_animator_set_destroy_when_done(animator, TRUE);

  widget_animator_manager_add(widget_animator_manager(), animator);

  return RET_OK;
}

ret_t widget_animator_time_elapse(widget_animator_t* animator, uint32_t delta_time) {
  uint32_t end_time = 0;
  float_t time_percent = 0;
  uint32_t elapsed_time = 0;
  return_value_if_fail(animator != NULL, RET_BAD_PARAMS);

  if (animator->state != ANIMATOR_RUNNING) {
    return RET_OK;
  }

  elapsed_time = delta_time * animator->time_scale;
  if (animator->delay > 0) {
    int32_t delay = animator->delay - elapsed_time;
    if (delay > 0) {
      animator->delay = delay;
      return RET_OK;
    } else {
      elapsed_time = -delay;
      animator->now = 0;
      animator->delay = 0;
      animator->start_time = 0;
    }
  }

  animator->now += elapsed_time;
  end_time = animator->start_time + animator->duration;
  time_percent = ((float_t)(animator->now - animator->start_time)) / animator->duration;

  if (time_percent > 1) {
    time_percent = 1;
  }

  if (animator->reversed) {
    time_percent = 1 - time_percent;
  }

  widget_animator_update(animator, animator->easing(time_percent));

  if (animator->now >= end_time) {
    if (animator->repeat_times > 0) {
      event_t e = event_init(EVT_ANIM_ONCE, animator);
      animator->start_time = animator->now;
      animator->repeat_times--;

      if (animator->repeat_times == 0 && animator->forever) {
        animator->repeat_times = TK_UINT32_MAX;
      }

      emitter_dispatch(&(animator->emitter), &e);
    } else if (animator->yoyo_times > 0) {
      event_t e = event_init(EVT_ANIM_ONCE, animator);
      animator->start_time = animator->now;
      animator->reversed = !animator->reversed;

      if (!animator->reversed) {
        animator->yoyo_times--;
      }

      if (animator->yoyo_times == 0 && animator->forever) {
        animator->yoyo_times = TK_UINT32_MAX;
      }

      emitter_dispatch(&(animator->emitter), &e);
    }

    if (animator->repeat_times == 0 && animator->yoyo_times == 0) {
      event_t e = event_init(EVT_ANIM_END, animator);
      emitter_dispatch(&(animator->emitter), &e);

      animator->state = ANIMATOR_DONE;
      if (animator->destroy_when_done) {
        widget_animator_destroy(animator);
      }
    }
  }

  return RET_OK;
}

ret_t widget_animator_start(widget_animator_t* animator) {
  event_t e = event_init(EVT_ANIM_START, animator);
  return_value_if_fail(animator != NULL, RET_BAD_PARAMS);

  if (animator->state == ANIMATOR_RUNNING) {
    return RET_OK;
  }

  if (animator->state == ANIMATOR_DONE) {
    animator->now = 0;
    animator->start_time = 0;
    animator->reversed = FALSE;
    animator->yoyo_times = animator->total_yoyo_times;
    animator->repeat_times = animator->total_repeat_times;
  }

  animator->state = ANIMATOR_RUNNING;
  emitter_dispatch(&(animator->emitter), &e);
  widget_animator_update(animator, 0);

  return RET_OK;
}

ret_t widget_animator_stop(widget_animator_t* animator) {
  event_t e = event_init(EVT_ANIM_STOP, animator);
  return_value_if_fail(animator != NULL, RET_BAD_PARAMS);

  if (animator->state == ANIMATOR_RUNNING) {
    animator->now = 0;
    animator->start_time = 0;
    animator->reversed = FALSE;
    animator->state = ANIMATOR_STOPPED;
    emitter_dispatch(&(animator->emitter), &e);
    widget_animator_update(animator, 0);

    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

ret_t widget_animator_pause(widget_animator_t* animator) {
  event_t e = event_init(EVT_ANIM_PAUSE, animator);
  return_value_if_fail(animator != NULL, RET_BAD_PARAMS);

  if (animator->state == ANIMATOR_RUNNING) {
    animator->state = ANIMATOR_PAUSED;
    emitter_dispatch(&(animator->emitter), &e);

    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

static ret_t widget_animator_update(widget_animator_t* animator, float_t percent) {
  ret_t ret = RET_OK;
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  widget_invalidate_force(animator->widget, NULL);
  ret = animator->update(animator, percent);
  widget_invalidate_force(animator->widget, NULL);

  return ret;
}

ret_t widget_animator_set_yoyo(widget_animator_t* animator, uint32_t yoyo_times) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  animator->repeat_times = 0;
  animator->total_repeat_times = animator->repeat_times;
  animator->forever = yoyo_times == 0;
  animator->yoyo_times = animator->forever ? TK_UINT32_MAX : yoyo_times;
  animator->total_yoyo_times = animator->yoyo_times;

  return RET_OK;
}

ret_t widget_animator_set_name(widget_animator_t* animator, const char* name) {
  return_value_if_fail(animator != NULL, RET_BAD_PARAMS);

  animator->name = tk_str_copy(animator->name, name);

  return RET_OK;
}

ret_t widget_animator_set_repeat(widget_animator_t* animator, uint32_t repeat_times) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  animator->yoyo_times = 0;
  animator->total_yoyo_times = animator->yoyo_times;
  animator->forever = repeat_times == 0;
  animator->repeat_times = animator->forever ? TK_UINT32_MAX : repeat_times;
  animator->total_repeat_times = animator->repeat_times;

  return RET_OK;
}

ret_t widget_animator_set_reversed(widget_animator_t* animator, bool_t value) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  animator->reversed = value;

  return RET_OK;
}

ret_t widget_animator_set_time_scale(widget_animator_t* animator, float_t time_scale) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  animator->time_scale = time_scale;

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

ret_t widget_animator_set_destroy_when_done(widget_animator_t* animator, bool_t destroy_when_done) {
  return_value_if_fail(animator != NULL, RET_BAD_PARAMS);

  animator->destroy_when_done = destroy_when_done;

  return RET_OK;
}

static ret_t widget_animator_do_destroy(widget_animator_t* animator) {
  TKMEM_FREE(animator->name);
  emitter_deinit(&(animator->emitter));

  if (animator->destroy != NULL) {
    return animator->destroy(animator);
  } else {
    memset(animator, 0x00, sizeof(widget_animator_t));
    TKMEM_FREE(animator);
  }

  return RET_OK;
}

static ret_t widget_animator_do_destroy_async(const idle_info_t* info) {
  widget_animator_do_destroy((widget_animator_t*)(info->ctx));

  return RET_REMOVE;
}

ret_t widget_animator_destroy(widget_animator_t* animator) {
  return_value_if_fail(animator != NULL && animator->update != NULL, RET_BAD_PARAMS);

  if (animator->widget_destroy_id != TK_INVALID_ID) {
    widget_off(animator->widget, animator->widget_destroy_id);
    animator->widget_destroy_id = TK_INVALID_ID;
  }

  if (animator->widget_animator_manager != NULL) {
    widget_animator_manager_remove(animator->widget_animator_manager, animator);
  }

  animator->update = NULL;
  idle_add(widget_animator_do_destroy_async, animator);

  return RET_OK;
}
