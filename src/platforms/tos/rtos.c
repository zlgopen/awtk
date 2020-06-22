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

#include "tos_k.h"
#include "platforms/common/rtos.h"

ret_t rtos_init(void) {
  tos_knl_init();

  return RET_OK;
}

ret_t rtos_start(void) {
  tos_knl_start();

  return RET_OK;
}

void rtos_tick(void) {
  if (tos_knl_is_running()) {
    tos_knl_irq_enter();
    tos_tick_handler();
    tos_knl_irq_leave();
  }
}

void rtos_delay(uint32_t ms) {
  tos_task_delay(ms);
}

bool_t rtos_is_running(void) {
  return tos_knl_is_running();
}
