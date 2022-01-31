/**
 * File:   rtos.c
 * Author: AWTK Develop Team
 * Brief:  rtos for freertos
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
 * 2021-05-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "FreeRTOS.h"
#include "task.h"
#include "platforms/common/rtos.h"

static bool_t s_is_running = FALSE;

ret_t rtos_init(void) {
  return RET_OK;
}

ret_t rtos_start(void) {
  if (!s_is_running) {
    s_is_running = TRUE;
    vTaskStartScheduler();
  }

  return RET_OK;
}

/*implemented in port.c*/
void xPortSysTickHandler(void);

/* 
 * modify FreeRTOSConfig.h remove the following line
 * #define xPortSysTickHandler SysTick_Handler
 */

void rtos_tick(void) {
  if (s_is_running) {
    xPortSysTickHandler();
  }
}

void rtos_delay(uint32_t ms) {
  uint32_t ticks = ms * portTICK_PERIOD_MS;

  vTaskDelay(ticks);
}

bool_t rtos_is_running(void) {
  return s_is_running;
}
