/**
 * File:   platform_default.c
 * Author: AWTK Develop Team
 * Brief:  default platform
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
 * 2018-02-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/timer.h"
#include "base/platform.h"

#ifdef WIN32
#include <time.h>
#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

int gettimeofday(struct timeval* tp, void* tzp) {
  time_t clock;
  struct tm tm;
  SYSTEMTIME wtm;
  GetLocalTime(&wtm);
  tm.tm_year = wtm.wYear - 1900;
  tm.tm_mon = wtm.wMonth - 1;
  tm.tm_mday = wtm.wDay;
  tm.tm_hour = wtm.wHour;
  tm.tm_min = wtm.wMinute;
  tm.tm_sec = wtm.wSecond;
  tm.tm_isdst = -1;
  clock = mktime(&tm);
  tp->tv_sec = (long)clock;
  tp->tv_usec = wtm.wMilliseconds * 1000;
  return (0);
}
#else
#include <sys/time.h>
#include <unistd.h>
#endif

uint32_t get_time_ms() {
  struct timeval tv;
  gettimeofday(&tv, NULL);

  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void sleep_ms(uint32_t ms) {
#ifdef WIN32
  Sleep(ms);
#else
  usleep(ms * 1000);
#endif
}

#ifndef HAS_STD_MALLOC
static uint32_t s_heap_mem[1024 * 1024];
#endif /*HAS_STD_MALLOC*/

ret_t platform_prepare(void) {
#ifndef HAS_STD_MALLOC
  tk_mem_init(s_heap_mem, sizeof(s_heap_mem));
#endif /*HAS_STD_MALLOC*/

  return RET_OK;
}
