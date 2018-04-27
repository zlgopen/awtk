/**
 * File:   demo_main.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  demo main
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

#include "tk.h"
#include "resource.h"
#include "base/system_info.h"

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
  system_info_t* info = system_info();

  tk_init(320, 480, s_heap_mem, sizeof(s_heap_mem));

  log_debug("system_info: lcd_w=%d lcd_h=%d lcd_type=%d device_pixel_ratio=%f\n", 
    info->lcd_w, info->lcd_h, info->lcd_type, info->device_pixel_ratio);
  resource_init();
  application_init();

  tk_run();

  return 0;
}
