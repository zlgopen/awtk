/**
 * File:   timer_info.h
 * Author: AWTK Develop Team
 * Brief:  timer info
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
    .on_destroy = (tk_object_on_destroy_t)timer_info_on_destroy};

timer_info_t* timer_info_create(timer_manager_t* tm, timer_func_t on_timer, void* ctx,
                                uint32_t duration, uint16_t timer_info_type) {
  tk_object_t* obj = tk_object_create(&s_timer_info_vtable);
  timer_info_t* timer = TIMER_INFO(obj);
  return_value_if_fail(timer != NULL, NULL);

  timer->ctx = ctx;
  timer->suspend = FALSE;
  timer->on_timer = on_timer;
  timer->duration = duration;
  timer->timer_info_type = timer_info_type;

  if (tm != NULL) {
    uint32_t id = timer_manager_get_next_timer_id(tm);
    timer->id = id;
    timer->timer_manager = tm;
    timer->start = tm->get_time();

    if (id != TK_INVALID_ID) {
      timer_manager_append(tm, timer);
    } else {
      tk_object_unref(obj);
      return_value_if_fail(id != TK_INVALID_ID, NULL);
    }
  }

  return timer;
}

int timer_info_compare_by_id(const void* a, const void* b) {
  return ((const timer_info_t*)a)->id - ((const timer_info_t*)b)->id;
}

int timer_info_compare_by_ctx(const void* a, const void* b) {
  const timer_info_t* info_a = (const timer_info_t*)a;
  return (char*)(info_a->ctx) - (char*)(b);
}

int timer_info_compare_by_ctx_and_type(const void* a, const void* b) {
  const timer_info_t* info_a = (const timer_info_t*)a;
  const timer_info_t* info_b = (const timer_info_t*)b;
  if (info_a->timer_info_type == info_b->timer_info_type) {
    return (char*)(info_a->ctx) - (char*)(info_b->ctx);
  }
  return -1;
}

timer_info_t* timer_info_init_dummy_with_ctx_and_type(timer_info_t* timer, uint16_t type,
                                                      void* ctx) {
  return_value_if_fail(timer != NULL, NULL);
  memset(timer, 0x00, sizeof(timer_info_t));

  timer->ctx = ctx;
  timer->timer_info_type = type;

  return timer;
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
