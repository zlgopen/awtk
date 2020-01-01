/**
 * File:   sys_tick.c
 * Author: AWTK Develop Team
 * Brief:  use sys tick to implement sleep/get_time_ms64.
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
 * 2018-10-20 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "cmsis_os.h"
#include "tkc/types_def.h"

static volatile uint64_t g_sys_tick;

#ifdef _TOS_CONFIG_H_
#include "tos.h"
void rtos_tick(void) {
  if (tos_knl_is_running()) {
    tos_knl_irq_enter();
    tos_tick_handler();
    tos_knl_irq_leave();
  }
}
#define rtos_task_delay(ms) tos_task_delay(ms);
#endif /*_TOS_CONFIG_H_*/

void SysTick_Handler(void) {
  g_sys_tick++;
  rtos_tick();
}

uint64_t get_time_ms64() {
  return g_sys_tick;
}

static void sleep_ms_raw(uint32_t ms) {
  uint32_t count = 0;
  uint64_t start = get_time_ms64();

  while (get_time_ms64() < (start + ms)) {
    count++;
  }
}

void sleep_ms(uint32_t ms) {
  if (tos_knl_is_running()) {
    rtos_task_delay(ms);
  } else {
    sleep_ms_raw(ms);
  }
}

void delay_ms(uint32_t ms) {
  sleep_ms(ms);
}

void delay_us(uint32_t us) {
  sleep_ms(1);
}
