/**
 * File:   sys_tick_common.inc
 * Author: AWTK Develop Team
 * Brief:  sys tick common function.
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-09-01 Luo Zhiming <luozhiming@zlg.cn> created
 *
 */

#include "tkc/types_def.h"

static volatile uint64_t g_sys_tick;
static volatile uint32_t g_tick_irq_flag;

/**
 * @method tk_tick_inc
 * 增加多少个 tick (每个 tick 的单位为 1ms)
 * @param {uint32_t} tick_period 增加的 tick 个数。
 */
void tk_sys_tick_inc(uint32_t tick_period) {
  g_tick_irq_flag = 0;
  g_sys_tick += tick_period;
}

static uint64_t tk_sys_tick_get_tick64() {
  uint64_t result;
  do {
    g_tick_irq_flag = 1;
    result = g_sys_tick;
  } while (!g_tick_irq_flag);
  return result;
}

static void tk_sys_tick_sleep_tick(uint32_t tick) {
  uint64_t count = 0;
  uint64_t start = tk_sys_tick_get_tick64();

  while (tk_sys_tick_get_tick64() < (start + tick)) {
    count++;
  }
}

