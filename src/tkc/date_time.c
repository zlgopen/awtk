/**
 * File:   date_time.c
 * Author: AWTK Develop Team
 * Brief:  date time
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-11-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/date_time.h"

static date_time_get_now_t s_date_time_get_now;
static date_time_set_now_t s_date_time_set_now;
static date_time_from_time_t s_date_time_from_time;
static date_time_to_time_t s_date_time_to_time;

ret_t date_time_global_init_ex(const date_time_vtable_t* vt) {
  return_value_if_fail(vt != NULL, RET_BAD_PARAMS);

  s_date_time_get_now = vt->get_now;
  s_date_time_set_now = vt->set_now;
  s_date_time_from_time = vt->from_time;
  s_date_time_to_time = vt->to_time;

  return RET_OK;
}

ret_t date_time_global_init(date_time_get_now_t get, date_time_set_now_t set) {
  s_date_time_get_now = get;
  s_date_time_set_now = set;

  return RET_OK;
}

ret_t date_time_set_impl(date_time_get_now_t date_time_get_now) {
  s_date_time_get_now = date_time_get_now;

  return RET_OK;
}

date_time_t* date_time_create(void) {
  date_time_t* dt = TKMEM_ZALLOC(date_time_t);

  return date_time_init(dt);
}

ret_t date_time_from_time(date_time_t* dt, uint64_t time) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(s_date_time_from_time != NULL, RET_BAD_PARAMS);

  return s_date_time_from_time(dt, (time_t)time);
}

date_time_t* date_time_init(date_time_t* dt) {
  return_value_if_fail(dt != NULL, NULL);

  memset(dt, 0x00, sizeof(date_time_t));
  if (s_date_time_get_now != NULL) {
    s_date_time_get_now(dt);
    return dt;
  } else {
    return NULL;
  }
}

ret_t date_time_set(date_time_t* dt) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(s_date_time_set_now != NULL, RET_NOT_IMPL);

  return s_date_time_set_now(dt);
}

ret_t date_time_destroy(date_time_t* dt) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(dt);

  return RET_OK;
}

bool_t date_time_is_leap(uint32_t year) {
  if ((year % 100) == 0) {
    return (year % 400) == 0;
  } else {
    return (year % 4) == 0;
  }
}

int32_t date_time_get_days(uint32_t year, uint32_t month) {
  int days = 0;
  int days_of_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  return_value_if_fail(month >= 1 && month <= 12, -1);

  days = days_of_month[month - 1];
  if (days == 28) {
    if (date_time_is_leap(year)) {
      days = 29;
    }
  }

  return days;
}
/**
 * 实现参考：https://github.com/Franceskynov/zeller/blob/master/zeller.js
 */
int32_t date_time_get_wday(uint32_t year, uint32_t month, uint32_t day) {
  int w = 0;
  int a = 0;
  int y = 0;
  int m = 0;
  return_value_if_fail(year <= 10000, -1);
  return_value_if_fail(day >= 1 && day <= 31, -1);
  return_value_if_fail(month >= 1 && month <= 12, -1);
  return_value_if_fail(date_time_get_days(year, month) >= day && day > 0, -1);

  a = ((14 - month) / 12);
  y = (year - a);
  m = (month + (12 * a) - 2);
  w = (((day + y + (y / 4)) - (y / 100) + (y / 400) + (31 * m) / 12) % 7);

  return w;
}

static const char* s_en_month_names[] = {"Jan", "Feb", "Mar",  "Apr", "May", "Jun",
                                         "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};

const char* date_time_get_month_name(uint32_t month) {
  return_value_if_fail(month >= 1 && month <= 12, NULL);

  return s_en_month_names[month - 1];
}

static const char* s_en_wday_names[] = {
    "Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat",
};

const char* date_time_get_wday_name(uint32_t wday) {
  return_value_if_fail(wday <= 6, NULL);

  return s_en_wday_names[wday];
}

ret_t date_time_add_delta(date_time_t* dt, int64_t delta) {
  uint64_t t = 0;
  return_value_if_fail(dt != NULL && s_date_time_to_time != NULL, RET_BAD_PARAMS);
  t = s_date_time_to_time(dt) + delta;

  return s_date_time_from_time(dt, t);
}

uint64_t date_time_to_time(date_time_t* dt) {
  return_value_if_fail(dt != NULL && s_date_time_to_time != NULL, RET_BAD_PARAMS);

  return s_date_time_to_time(dt);
}

ret_t date_time_set_year(date_time_t* dt, uint32_t year) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);

  dt->year = year;
  dt->wday = date_time_get_wday(dt->year, dt->month, dt->day);

  return RET_OK;
}

ret_t date_time_set_month(date_time_t* dt, uint32_t month) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(month > 0 && month <= 31, RET_BAD_PARAMS);

  dt->month = month;
  dt->wday = date_time_get_wday(dt->year, dt->month, dt->day);

  return RET_OK;
}

ret_t date_time_set_day(date_time_t* dt, uint32_t day) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);

  dt->day = day;
  dt->wday = date_time_get_wday(dt->year, dt->month, dt->day);

  return RET_OK;
}

ret_t date_time_set_hour(date_time_t* dt, uint32_t hour) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(hour < 24, RET_BAD_PARAMS);

  dt->hour = hour;

  return RET_OK;
}

ret_t date_time_set_minute(date_time_t* dt, uint32_t minute) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(minute < 60, RET_BAD_PARAMS);

  dt->minute = minute;

  return RET_OK;
}

ret_t date_time_set_second(date_time_t* dt, uint32_t second) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(second < 60, RET_BAD_PARAMS);

  dt->second = second;

  return RET_OK;
}
