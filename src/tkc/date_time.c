/**
 * File:   date_time.c
 * Author: AWTK Develop Team
 * Brief:  date time
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
 * 2018-11-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/date_time.h"

static date_time_get_now_t s_date_time_get_now;
static date_time_set_now_t s_date_time_set_now;
static date_time_from_time_t s_date_time_from_time;

ret_t date_time_global_init_ex(const date_time_vtable_t* vt) {
  return_value_if_fail(vt != NULL, RET_BAD_PARAMS);

  s_date_time_get_now = vt->get_now;
  s_date_time_set_now = vt->set_now;
  s_date_time_from_time = vt->from_time;

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

int32_t date_time_get_wday(uint32_t year, uint32_t month, uint32_t day) {
  int32_t w = 0;
  uint32_t y = year;
  uint32_t m = month;
  uint32_t d = day;

  return_value_if_fail(month >= 1 && month <= 12, -1);
  return_value_if_fail(date_time_get_days(year, month) >= day && day > 0, -1);

  /*参考：https://baike.baidu.com/item/%E8%94%A1%E5%8B%92%E5%85%AC%E5%BC%8F/10491767*/
  if (y >= 1582 && m >= 10 && d > 4) {
    /*1582年10月4日后：w = (d + 1+ 2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7;*/
    w = (d + 1 + 2 * m + 3 * (m + 1) / 5 + y + y / 4 - y / 100 + y / 400) % 7;
  } else {
    /*1582年10月4日前：w = (d+1+2*m+3*(m+1)/5+y+y/4+5) % 7;*/
    w = (d + 1 + 2 * m + 3 * (m + 1) / 5 + y + y / 4 + 5) % 7;
  }

  /*Sunday = 0*/
  w = (w + 1) % 7;

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
  return_value_if_fail(wday >= 0 && wday <= 6, NULL);

  return s_en_wday_names[wday];
}
