/**
 * File:   rtos.c
 * Author: AWTK Develop Team
 * Brief:  rtos
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
 * 2019-11-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/types_def.h"
#include "platforms/common/rtos.h"

#undef EAGAIN

#include "rthw.h"
#include "rtthread.h"

static bool_t s_kernel_inited = FALSE;

static bool_t rtos_is_inited(void) {
  return s_kernel_inited;
}

static uint32_t s_heap[4 * 1024];

ret_t rtos_init(void) {
  rt_hw_interrupt_disable();

  /* show version */
  rt_show_version();

#ifdef RT_USING_HEAP
  rt_system_heap_init((void*)s_heap, s_heap + sizeof(s_heap) / sizeof(s_heap[0]));
#endif

  /* initialize scheduler system */
  rt_system_scheduler_init();

  /* initialize timer */
  rt_system_timer_init();

  /* initialize timer thread */
  rt_system_timer_thread_init();

  /* initialize idle thread */
  rt_thread_idle_init();

  s_kernel_inited = TRUE;

  return RET_OK;
}

ret_t rtos_start(void) {
  /* start scheduler */
  rt_system_scheduler_start();

  return RET_OK;
}

void rtos_tick(void) {
  if (rtos_is_inited()) {
    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();
  }
}

void rtos_delay(uint32_t ms) {
  rt_thread_delay(ms);
}

bool_t rtos_is_running(void) {
  return s_kernel_inited;
}
