/**
 * File:   platform_default.c
 * Author: AWTK Develop Team
 * Brief:  default platform
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/platform.h"
#include "tkc/date_time.h"
#include "tkc/mem.h"

#ifdef WIN32
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

static int32_t get_local_timezone() {
  int32_t timezone = 0;
  struct tm tlocal;
  struct tm tgmt;
  time_t tutc = 0;

  time(&tutc);
  memset(&tlocal, 0x00, sizeof(tlocal));
  localtime_s(&tlocal, &tutc);
  memset(&tgmt, 0x00, sizeof(tgmt));
  gmtime_s(&tgmt, &tutc);

  timezone = tlocal.tm_hour - tgmt.tm_hour;
  if (timezone < -11) {
    timezone += 24;
  } else if (timezone > 12) {
    timezone -= 24;
  }

  return timezone;
}


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
  SYSTEMTIME wtm;
  memset(&wtm, 0x00, sizeof(wtm));

  wtm.wMinute = dt->minute;
  wtm.wSecond = dt->second;
  wtm.wHour = dt->hour;
  wtm.wDay = dt->day;
  wtm.wMonth = dt->month;
  wtm.wYear = dt->year;

  if (SetLocalTime(&wtm)) {
    return RET_OK;
  } else {
    uint32_t last_error = (uint32_t)GetLastError();
    log_debug("SetLocalTime(%d-%d-%d %d:%d:%d) failed: %u\n", (int)wtm.wYear, (int)wtm.wMonth,
              (int)wtm.wDay, (int)wtm.wHour, (int)wtm.wMinute, (int)wtm.wSecond, last_error);
    return RET_FAIL;
  }
}

static uint64_t date_time_to_time_impl(date_time_t* dt) {
  struct tm t;
  time_t tvalue = 0;
  int32_t timezone = get_local_timezone();
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);

  memset(&t, 0x00, sizeof(t));
  t.tm_sec = dt->second;
  t.tm_min = dt->minute;
  t.tm_hour = dt->hour;
  t.tm_mday = dt->day;
  t.tm_mon = dt->month - 1;
  t.tm_year = dt->year - 1900;
  t.tm_wday = dt->wday;
  t.tm_isdst = -1;

  if (t.tm_hour < timezone) {
    int32_t temp = timezone;
    timezone = t.tm_hour;
    t.tm_hour = temp;
  }
  tvalue = mktime(&t) + timezone * 60 * 60;

  return tvalue;
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
  (void)t;
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

static uint64_t date_time_to_time_impl(date_time_t* dt) {
  struct tm t;
  time_t tvalue = 0;
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);

  memset(&t, 0x00, sizeof(t));
  t.tm_sec = dt->second;
  t.tm_min = dt->minute;
  t.tm_hour = dt->hour;
  t.tm_mday = dt->day;
  t.tm_mon = dt->month - 1;
  t.tm_year = dt->year - 1900;
  t.tm_wday = dt->wday;

  tvalue = timegm(&t);

  return tvalue;
}

#endif

static ret_t date_time_from_time_impl(date_time_t* dt, uint64_t timeval) {
  time_t tm = timeval;
  struct tm* t = gmtime(&tm);
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

uint64_t stm_now_ms();
uint64_t stm_now_us();
void stm_time_init(void);

uint64_t get_time_ms64() {
  return stm_now_ms();
}

uint64_t get_time_us64() {
  return stm_now_us();
}

static const date_time_vtable_t s_date_time_vtable = {
    date_time_get_now_impl,
    date_time_set_now_impl,
    date_time_from_time_impl,
    date_time_to_time_impl,
};

void sleep_ms(uint32_t ms) {
#ifdef WIN32
  Sleep(ms);
#else
  usleep(ms * 1000);
#endif
}

ret_t platform_prepare(void) {
  static bool_t inited = FALSE;
  if (inited) {
    return RET_OK;
  }
  inited = TRUE;

  stm_time_init();

#ifndef HAS_STD_MALLOC
#ifndef TK_HEAP_MEM_SIZE
#define TK_HEAP_MEM_SIZE 12 * 1024 * 1024
#endif /*TK_HEAP_MEM_SIZE*/

  static uint32_t s_heap_mem[TK_HEAP_MEM_SIZE / 4];
  tk_mem_init(s_heap_mem, sizeof(s_heap_mem));
#endif /*HAS_STD_MALLOC*/

  date_time_global_init_ex(&s_date_time_vtable);
  srandom(time(0));

  return RET_OK;
}
