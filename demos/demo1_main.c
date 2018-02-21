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

#ifdef WITH_STM32F103ZE_RAW
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "button.h"
#include "lcd_driver.h"
#include "gui.h"
#include "tim.h"
#include "rtc.h"
#include "stdlib.h"
#include "flash.h"
#include "touch.h"
#include "main_loop/main_loop_stm32_raw.h"

static __align(8) uint32_t s_heap_mem[2048];
#else
#include "main_loop/main_loop_sdl2.h"

static uint32_t s_heap_mem[2048];
#endif

#ifdef WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
#else

#include "base/mem.h"

int mem_test() {
  char* str = NULL;
  char* p[32];
  uint32_t i = 0;

  str = (char*)mem_alloc(100);
  mem_free(str);

  for (i = 0; i < ARRAY_SIZE(p); i++) {
    p[i] = (char*)MEM_ALLOC(i);
  }

  for (i = 0; i < ARRAY_SIZE(p); i++) {
    MEM_FREE(p[i]);
  }

  return 0;
}

ret_t application_init(void);

int main(void) {
#endif
  platform_init();
  mem_init(s_heap_mem, sizeof(s_heap_mem));

  resource_init();
  application_init();

#ifdef WITH_STM32F103ZE_RAW
  main_loop_stm32_raw_init(320, 480);
#else
  main_loop_sdl2_init(320, 480);
#endif

  application_init();

  main_loop_run(default_main_loop());

  main_loop_destroy(default_main_loop());

  return 0;
}
