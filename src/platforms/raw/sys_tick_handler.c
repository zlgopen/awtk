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
 * 2018-06-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/types_def.h"

static volatile uint64_t g_sys_tick;

void SysTick_Handler(void) {
  g_sys_tick++;
}

uint64_t get_time_ms64() {
  return g_sys_tick;
}

void sleep_ms(uint32_t ms) {
  uint32_t count = 0;
  uint64_t start = get_time_ms64();

  while (get_time_ms64() < (start + ms)) {
    count++;
  }
}
