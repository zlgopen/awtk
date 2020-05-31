/**
 * File:   timer_info.h
 * Author: AWTK Develop Team
 * Brief:  timer info
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
 * 2018-02-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/timer_info.h"
#include "tkc/timer_manager.h"

#ifdef AWTK_WEB_JS
#include <emscripten.h>
#endif /*AWTK_WEB_JS*/

static ret_t timer_info_on_destroy(timer_info_t* info) {
  if (info->on_destroy != NULL) {
    info->on_destroy(info);
  }

#ifdef AWTK_WEB_JS
  EM_ASM_INT({ return TBrowser.releaseFunction($0); }, info->on_timer);
#endif /*AWTK_WEB_JS*/

  return RET_OK;
}

static const object_vtable_t s_timer_info_vtable = {
    .type = "timer_info",
    .desc = "timer_info",
    .size = sizeof(timer_info_t),
    .is_collection = FALSE,
    .on_destroy = (object_on_destroy_t)timer_info_on_destroy};

timer_info_t* timer_info_create(timer_manager_t* tm, timer_func_t on_timer, void* ctx,
                                uint32_t duration) {
  object_t* obj = object_create(&s_timer_info_vtable);
  timer_info_t* timer = TIMER_INFO(obj);
  return_value_if_fail(timer != NULL, NULL);

  timer->ctx = ctx;
  timer->on_timer = on_timer;
  timer->duration = duration;

  if (tm != NULL) {
    timer->timer_manager = tm;
    timer->start = tm->get_time();
    timer->id = tm->next_timer_id++;
    if (timer->id == TK_INVALID_ID) {
      timer->id = tm->next_timer_id++;
    }
    timer_manager_append(tm, timer);
  }

  return timer;
}

int timer_info_compare(const void* a, const void* b) {
  return ((const timer_info_t*)a)->id - ((const timer_info_t*)b)->id;
}

timer_info_t* timer_info_init_dummy(timer_info_t* timer, uint32_t id) {
  return_value_if_fail(timer != NULL, NULL);
  memset(timer, 0x00, sizeof(timer_info_t));

  timer->id = id;

  return timer;
}

timer_info_t* timer_info_cast(timer_info_t* timer) {
  return_value_if_fail(timer != NULL, NULL);

  return timer;
}

ret_t timer_info_on_timer(timer_info_t* timer, uint64_t now) {
  ret_t ret = RET_OK;
  return_value_if_fail(timer != NULL && timer->on_timer != NULL, RET_BAD_PARAMS);

  if (timer->busy) {
    return RET_BUSY;
  }

  timer->busy = TRUE;
  timer->now = now;
  ret = timer->on_timer(timer);
  timer->busy = FALSE;

  return ret;
}

bool_t timer_info_is_available(timer_info_t* timer, uint64_t now) {
  return timer != NULL && !(timer->busy) && timer->now != now;
}
