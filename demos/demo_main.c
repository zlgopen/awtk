/**
 * File:   demo_main.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  demo main
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/mem.h"
#include "resource.h"
#include "base/system_info.h"

ret_t application_init(void);

#ifndef HAS_STD_MALLOC
#ifndef GUI_HEAP_SIZE
#define GUI_HEAP_SIZE 2048
#endif/*GUI_HEAP_SIZE*/
static uint32_t s_heap_mem[GUI_HEAP_SIZE];
#else
static uint32_t s_heap_mem[1];
#endif/*HAS_STD_MALLOC*/

#ifdef USE_GUI_MAIN
int gui_app_start(void* params) {
#elif defined(WIN32)
#include <windows.h>
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
#else
int main(void) {
#endif

  tk_init(320, 480, s_heap_mem, sizeof(s_heap_mem));

  resource_init();
  application_init();

  tk_run();

  return 0;
}
