/**
 * File:   sys_tick.c
 * Author: AWTK Develop Team
 * Brief:  use sys tick to implement sleep/get_time_ms64.
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
 * 2018-06-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/types_def.h"
#include "../common/sys_tick_common.inc"

/* SysTick_Handler 1 milliseconds */
void SysTick_Handler(void) {
  tk_sys_tick_inc(1);
}

uint64_t get_time_ms64() {
  return tk_sys_tick_get_tick64();
}

void sleep_ms(uint32_t ms) {
  tk_sys_tick_sleep_tick(ms);
}
