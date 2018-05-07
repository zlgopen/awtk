/**
 * File:   stm32f103ze_raw.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  stm32f103ze-raw platforma
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-05-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/timer.h"
#include "base/platform.h"
#include "stm32f4xx_hal.h"

ret_t platform_prepare() {
  timer_init(get_time_ms);

  return RET_OK;
}

uint32_t get_time_ms() { return HAL_GetTick(); }
