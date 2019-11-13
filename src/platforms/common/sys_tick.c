/**
 * File:   sys_tick.c
 * Author: AWTK Develop Team
 * Brief:  use sys tick to implement sleep/get_time_ms64.
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "rtos.h"

static volatile uint64_t g_sys_tick;
static bool_t s_sys_tick_enable = FALSE;

void sys_tick_enable(bool_t enable) {
  s_sys_tick_enable = enable;
}

void SysTick_Handler(void) {
  if (s_sys_tick_enable) {
    g_sys_tick++;
    rtos_tick();
  }
}

uint64_t get_time_ms64() {
  return g_sys_tick;
}

void sleep_ms(uint32_t ms) {
  rtos_delay(ms);
}
