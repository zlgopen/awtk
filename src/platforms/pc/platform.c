/**
 * File:   platform_default.c
 * Author: AWTK Develop Team
 * Brief:  default platform
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
 * 2018-02-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <time.h>
#include <stdio.h>
#include "base/timer.h"
#include "tkc/platform.h"
#include "tkc/date_time.h"
#include "tkc/mem.h"

#ifdef WIN32
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

static ret_t date_time_get_now_impl(date_time_t* dt) {
  SYSTEMTIME wtm;
  GetLocalTime(&wtm);

  dt->second = wtm.wSecond;
  dt->minute = wtm.wMinute;
  dt->hour = wtm.wHour;
  dt->day = wtm.wDay;
  dt->wday = wtm.wDayOfWeek;
  dt->month = wtm.wMonth;
  dt->year = wtm.wYear;

  return RET_OK;
}

static ret_t date_time_set_now_impl(date_time_t* dt) {
  return RET_NOT_IMPL;
}

static ret_t date_time_from_time_impl(date_time_t* dt, uint64_t timeval) {
  /*TODO*/
  return RET_NOT_IMPL;
}

#else
#include <sys/time.h>
#include <unistd.h>
static ret_t date_time_get_now_impl(date_time_t* dt) {
  time_t now = time(0);
  struct tm* t = localtime(&now);

  dt->second = t->tm_sec;
  dt->minute = t->tm_min;
  dt->hour = t->tm_hour;
  dt->day = t->tm_mday;
  dt->month = t->tm_mon + 1;
  dt->year = t->tm_year + 1900;
  dt->wday = t->tm_wday;

  return RET_OK;
}

static ret_t date_time_set_now_impl(date_time_t* dt) {
  struct tm tms;
  time_t t = 0;
  memset(&tms, 0x00, sizeof(tms));

  tms.tm_year = dt->year - 1900;
  tms.tm_mon = dt->month - 1;
  tms.tm_mday = dt->day;
  tms.tm_hour = dt->hour;
  tms.tm_min = dt->minute;
  tms.tm_sec = dt->second;

  t = mktime(&tms);
#ifdef LINUX
  {
    struct timeval tv;
    tv.tv_sec = t;
    tv.tv_usec = 0;
    if (settimeofday(&tv, (struct timezone*)0) < 0) {
      perror("stime failed\n");
      return RET_FAIL;
    }
  }
#endif /*LINUX*/

  return RET_OK;
}

static ret_t date_time_from_time_impl(date_time_t* dt, uint64_t timeval) {
  time_t tm = timeval;
  struct tm* t = localtime(&tm);
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);

  memset(dt, 0x00, sizeof(date_time_t));

  dt->second = t->tm_sec;
  dt->minute = t->tm_min;
  dt->hour = t->tm_hour;
  dt->day = t->tm_mday;
  dt->month = t->tm_mon + 1;
  dt->year = t->tm_year + 1900;
  dt->wday = t->tm_wday;

  return RET_OK;
}

#endif

uint64_t stm_now_ms();
void stm_time_init(void);

uint64_t get_time_ms64() {
  return stm_now_ms();
}

static const date_time_vtable_t s_date_time_vtable = {
    date_time_get_now_impl, date_time_set_now_impl, date_time_from_time_impl};

void sleep_ms(uint32_t ms) {
#ifdef WIN32
  Sleep(ms);
#else
  usleep(ms * 1000);
#endif
}

#ifndef HAS_STD_MALLOC
static uint32_t s_heap_mem[3 * 1024 * 1024];
#endif /*HAS_STD_MALLOC*/

ret_t platform_prepare(void) {
  stm_time_init();

#ifndef HAS_STD_MALLOC
  tk_mem_init(s_heap_mem, sizeof(s_heap_mem));
#endif /*HAS_STD_MALLOC*/

  date_time_global_init_ex(&s_date_time_vtable);

  return RET_OK;
}
