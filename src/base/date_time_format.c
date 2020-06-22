/**
 * File:   date_time_format.c
 * Author: AWTK Develop Team
 * Brief:  date_time_format
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utf8.h"
#include "base/locale_info.h"
#include "base/date_time_format.h"

static uint32_t count_char(const wchar_t* p, wchar_t c) {
  uint32_t nr = 0;

  while (*p++ == c) {
    nr++;
  }

  return nr;
}

static wchar_t* translate_wday(wchar_t* str, uint32_t size, uint32_t wday) {
  return_value_if_fail(wday < 7, NULL);

  static const char* const wdays[] = {
      "Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat",
  };

  const char* utf8 = locale_info_tr(locale_info(), wdays[wday]);
  return tk_utf8_to_utf16(utf8, str, size);
}

static wchar_t* translate_month(wchar_t* str, uint32_t size, uint32_t month) {
  return_value_if_fail(month < 13 && month > 0, NULL);

  static const char* const months[] = {"Jan", "Feb", "Mar",  "Apr", "May", "Jun",
                                       "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};

  const char* utf8 = locale_info_tr(locale_info(), months[month - 1]);
  return tk_utf8_to_utf16(utf8, str, size);
}

ret_t wstr_format_date_time(wstr_t* str, const char* format, const date_time_t* dt) {
  wchar_t temp[32];
  return_value_if_fail(format != NULL && str != NULL, RET_BAD_PARAMS);

  wstr_t wformat;
  wstr_init(&wformat, strlen(format) + 2);
  wstr_set_utf8(&wformat, format);
  const wchar_t* p = wformat.str;

  str->size = 0;
  memset(temp, 0x00, sizeof(temp));
  while (*p) {
    int32_t repeat = count_char(p, *p);

    switch (*p) {
      case 'Y': {
        if (repeat == 2) {
          wstr_push_int(str, "%02d", (dt->year % 100));
        } else {
          wstr_push_int(str, "%d", dt->year);
        }
        break;
      }
      case 'M': {
        if (repeat == 2) {
          wstr_push_int(str, "%02d", dt->month);
        } else if (repeat == 3) {
          translate_month(temp, ARRAY_SIZE(temp), dt->month);
          wstr_append(str, temp);
        } else {
          wstr_push_int(str, "%d", dt->month);
        }
        break;
      }
      case 'D': {
        if (repeat == 2) {
          wstr_push_int(str, "%02d", dt->day);
        } else {
          wstr_push_int(str, "%d", dt->day);
        }
        break;
      }
      case 'h': {
        if (repeat == 2) {
          wstr_push_int(str, "%02d", dt->hour);
        } else {
          wstr_push_int(str, "%d", dt->hour);
        }
        break;
      }
      case 'm': {
        if (repeat == 2) {
          wstr_push_int(str, "%02d", dt->minute);
        } else {
          wstr_push_int(str, "%d", dt->minute);
        }
        break;
      }
      case 's': {
        if (repeat == 2) {
          wstr_push_int(str, "%02d", dt->second);
        } else {
          wstr_push_int(str, "%d", dt->second);
        }
        break;
      }
      case 'w': {
        wstr_push_int(str, "%d", dt->wday);
        break;
      }
      case 'W': {
        translate_wday(temp, ARRAY_SIZE(temp), dt->wday);
        wstr_append(str, temp);
        break;
      }
      default: {
        wstr_append_with_len(str, p, repeat);
        break;
      }
    }
    p += repeat;
  }
  wstr_reset(&wformat);

  return RET_OK;
}

ret_t wstr_format_time(wstr_t* str, const char* format, uint64_t time) {
  date_time_t dt;
  return_value_if_fail(str != NULL && format != NULL, RET_BAD_PARAMS);

  memset(&dt, 0x00, sizeof(dt));
  date_time_from_time(&dt, time);

  return wstr_format_date_time(str, format, &dt);
}
