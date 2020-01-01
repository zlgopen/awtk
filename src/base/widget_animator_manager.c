/**
 * File:   widget_animator_manager.c
 * Author: AWTK Develop Team
 * Brief:  animator manager
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
 * 2018-11-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/widget_animator_manager.h"

static widget_animator_manager_t* s_animator_manager;

static ret_t widget_animator_manager_set_to_dispatch(widget_animator_manager_t* am) {
  widget_animator_t* iter = am->first;

  while (iter != NULL) {
    iter->to_dispatch = TRUE;
    iter = iter->next;
  }

  return RET_OK;
}

static widget_animator_t* widget_animator_manager_get_to_dispatch(widget_animator_manager_t* am) {
  widget_animator_t* iter = am->first;

  while (iter != NULL) {
    if (iter->to_dispatch) {
      return iter;
    }
    iter = iter->next;
  }

  return NULL;
}

ret_t widget_animator_manager_time_elapse(widget_animator_manager_t* am, uint32_t delta_time) {
  widget_animator_t* iter = NULL;
  return_value_if_fail(am != NULL, RET_BAD_PARAMS);

  delta_time = delta_time * am->time_scale;
  widget_animator_manager_set_to_dispatch(am);

  iter = widget_animator_manager_get_to_dispatch(am);
  while (iter != NULL) {
    iter->to_dispatch = FALSE;
    widget_animator_time_elapse(iter, delta_time);
    iter = widget_animator_manager_get_to_dispatch(am);
  }

  return RET_OK;
}

widget_animator_manager_t* widget_animator_manager(void) {
  return s_animator_manager;
}

ret_t widget_animator_manager_set(widget_animator_manager_t* am) {
  s_animator_manager = am;

  return RET_OK;
}

widget_animator_manager_t* widget_animator_manager_create(void) {
  widget_animator_manager_t* am = TKMEM_ZALLOC(widget_animator_manager_t);

  return widget_animator_manager_init(am);
}

static ret_t widget_animator_manager_on_timer(const timer_info_t* info) {
  widget_animator_manager_t* am = (widget_animator_manager_t*)(info->ctx);
  uint32_t last_dispatch_time = am->last_dispatch_time ? am->last_dispatch_time : info->now;
  uint32_t elapsed_time = info->now - last_dispatch_time;

  widget_animator_manager_time_elapse(am, elapsed_time);

  am->last_dispatch_time = info->now;

  return RET_REPEAT;
}

widget_animator_manager_t* widget_animator_manager_init(widget_animator_manager_t* am) {
  return_value_if_fail(am != NULL, NULL);

  am->time_scale = 1;
  am->timer_id = timer_add(widget_animator_manager_on_timer, am, 1000 / 60);

  return am;
}

ret_t widget_animator_manager_reset(widget_animator_manager_t* am) {
  widget_animator_t* iter = NULL;
  return_value_if_fail(am != NULL, RET_BAD_PARAMS);

  iter = am->first;
  while (iter != NULL) {
    widget_animator_t* next = iter->next;

    widget_animator_destroy(iter);
    iter = next;
  }
  am->first = NULL;

  return RET_OK;
}

ret_t widget_animator_manager_deinit(widget_animator_manager_t* am) {
  return_value_if_fail(am != NULL, RET_BAD_PARAMS);

  timer_remove(am->timer_id);

  return RET_OK;
}

ret_t widget_animator_manager_destroy(widget_animator_manager_t* am) {
  return_value_if_fail(am != NULL, RET_BAD_PARAMS);

  widget_animator_manager_deinit(am);
  TKMEM_FREE(am);

  return RET_OK;
}

ret_t widget_animator_manager_add(widget_animator_manager_t* am, widget_animator_t* animator) {
  widget_animator_t* iter = NULL;
  return_value_if_fail(am != NULL, RET_BAD_PARAMS);
  return_value_if_fail(animator != NULL, RET_BAD_PARAMS);

  iter = am->first;
  if (iter == NULL) {
    am->first = animator;
  } else {
    while (iter->next != NULL) {
      iter = iter->next;
    }
    iter->next = animator;
  }
  animator->widget_animator_manager = am;

  return RET_OK;
}

ret_t widget_animator_manager_remove(widget_animator_manager_t* am, widget_animator_t* animator) {
  ret_t ret = RET_NOT_FOUND;
  widget_animator_t* iter = NULL;
  widget_animator_t* prev = NULL;
  return_value_if_fail(am != NULL, RET_BAD_PARAMS);
  return_value_if_fail(animator != NULL, RET_BAD_PARAMS);

  iter = am->first;
  return_value_if_fail(iter != NULL, ret);

  if (iter == animator) {
    am->first = iter->next;
    ret = RET_OK;
  } else {
    prev = iter;
    iter = iter->next;
    while (iter != NULL) {
      if (iter == animator) {
        prev->next = iter->next;
        iter->widget_animator_manager = NULL;
        ret = RET_OK;
        break;
      }
      prev = iter;
      iter = iter->next;
    }
  }

  return ret;
}

static bool_t animator_is_equal(widget_animator_t* iter, widget_t* widget, const char* name) {
  if (iter->widget == widget || widget == NULL) {
    if (name == NULL || (iter->name != NULL && tk_str_eq(name, iter->name))) {
      return TRUE;
    }
  }

  return FALSE;
}

ret_t widget_animator_manager_remove_all(widget_animator_manager_t* am, widget_t* widget,
                                         const char* name) {
  ret_t ret = RET_NOT_FOUND;
  widget_animator_t* iter = NULL;
  widget_animator_t* prev = NULL;
  return_value_if_fail(am != NULL, RET_BAD_PARAMS);

  prev = am->first;
  iter = am->first;
  while (iter != NULL) {
    if (animator_is_equal(iter, widget, name)) {
      iter->widget_animator_manager = NULL;
      if (iter == am->first) {
        am->first = iter->next;
        widget_animator_destroy(iter);

        prev = am->first;
        iter = am->first;
      } else {
        prev->next = iter->next;
        widget_animator_destroy(iter);
        iter = prev->next;
      }

      ret = RET_OK;
    } else {
      prev = iter;
      iter = prev->next;
    }
  }

  return ret;
}

typedef enum _action_t { ACTION_START = 0, ACTION_STOP, ACTION_PAUSE } action_t;

uint32_t widget_animator_manager_exec(widget_animator_manager_t* am, widget_t* widget,
                                      const char* name, action_t a) {
  uint32_t nr = 0;
  widget_animator_t* iter = NULL;
  return_value_if_fail(am != NULL, 0);

  iter = am->first;
  while (iter != NULL) {
    if (animator_is_equal(iter, widget, name)) {
      switch (a) {
        case ACTION_START: {
          widget_animator_start(iter);
          break;
        }
        case ACTION_STOP: {
          widget_animator_stop(iter);
          break;
        }
        case ACTION_PAUSE: {
          widget_animator_pause(iter);
          break;
        }
        default:
          break;
      }
      nr++;
    }

    iter = iter->next;
  }

  return nr;
}

ret_t widget_animator_manager_stop(widget_animator_manager_t* am, widget_t* widget,
                                   const char* name) {
  return widget_animator_manager_exec(am, widget, name, ACTION_STOP) > 0 ? RET_OK : RET_NOT_FOUND;
}

ret_t widget_animator_manager_pause(widget_animator_manager_t* am, widget_t* widget,
                                    const char* name) {
  return widget_animator_manager_exec(am, widget, name, ACTION_PAUSE) > 0 ? RET_OK : RET_NOT_FOUND;
}

ret_t widget_animator_manager_start(widget_animator_manager_t* am, widget_t* widget,
                                    const char* name) {
  return widget_animator_manager_exec(am, widget, name, ACTION_START) > 0 ? RET_OK : RET_NOT_FOUND;
}

ret_t widget_animator_manager_set_time_scale(widget_animator_manager_t* am, widget_t* widget,
                                             const char* name, float_t time_scale) {
  widget_animator_t* iter = NULL;
  return_value_if_fail(am != NULL, RET_BAD_PARAMS);

  if (widget == NULL && name == NULL) {
    am->time_scale = time_scale;
    return RET_OK;
  }

  iter = am->first;
  while (iter != NULL) {
    if (animator_is_equal(iter, widget, name)) {
      widget_animator_set_time_scale(iter, time_scale);
    }

    iter = iter->next;
  }

  return RET_OK;
}

uint32_t widget_animator_manager_count(widget_animator_manager_t* am) {
  uint32_t count = 0;
  widget_animator_t* iter = NULL;
  return_value_if_fail(am != NULL, 0);

  iter = am->first;
  while (iter != NULL) {
    count++;
    iter = iter->next;
  }

  return count;
}

widget_animator_t* widget_animator_manager_find(widget_animator_manager_t* am, widget_t* widget,
                                                const char* name) {
  widget_animator_t* iter = NULL;
  return_value_if_fail(am != NULL && widget != NULL, NULL);

  iter = am->first;
  while (iter != NULL) {
    if (animator_is_equal(iter, widget, name)) {
      return iter;
    }
    iter = iter->next;
  }

  return NULL;
}
