/**
 * File:   demo1_main.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  demo1 main
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "resource.h"
#include "base/platform.h"

ret_t application_init(void);

#ifdef WITH_STM32F103ZE_RAW
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
#include "main_loop/main_loop_stm32_raw.h"

static __align(8) uint32_t s_heap_mem[2048];
#elif defined(WITH_RT_THREAD)
#include "main_loop/main_loop_rtthread.h"
static uint32_t s_heap_mem[2048];
#else
#include "main_loop/main_loop_sdl2.h"
static uint32_t s_heap_mem[2048];
#endif

#ifdef WITH_RT_THREAD
#include "base/mem.h"
int gui_app_start(void* params) {
#elif defined(WIN32)
#include <windows.h>
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
#else
#include "base/mem.h"
int main(void) {
#endif

  platform_prepare();
  mem_init(s_heap_mem, sizeof(s_heap_mem));

  resource_init();
  application_init();

#ifdef WITH_STM32F103ZE_RAW
  main_loop_stm32_raw_init(320, 480);
#elif defined(WITH_RT_THREAD)
  main_loop_rtthread_init(320, 480);
#else
  main_loop_sdl2_init(320, 480);
#endif

  application_init();

  main_loop_run(default_main_loop());

  main_loop_destroy(default_main_loop());

  return 0;
}
