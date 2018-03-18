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
 * 2018-02-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/timer.h"
#include "base/platform.h"
#include "button.h"
#include "delay.h"
#include "flash.h"
#include "gui.h"
#include "lcd_driver.h"
#include "led.h"
#include "rtc.h"
#include "stdlib.h"
#include "sys.h"
#include "tim.h"
#include "touch.h"
#include "usart.h"

ret_t platform_prepare() {
  delay_init();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  led_init();
  button_init();
  TFT_Init();
  TFT_ClearScreen(BLACK);
  FLASH_Init();
  TOUCH_Init();

  TIM3_Init(50, 7199);
  rtc_init();

  timer_init(get_time_ms);

  return RET_OK;
}

uint32_t get_time_ms() { return RTC_GetCounter(); }
