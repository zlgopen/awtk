/**
 * File:   digit_clock.h
 * Author: AWTK Develop Team
 * Brief:  digit_clock
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "base/locale_info.h"
#include "base/widget_vtable.h"
#include "time_clock/digit_clock.h"

#define DATE_TIME_MAX_LEN 127

static uint32_t count_char(const char* p, char c) {
  uint32_t nr = 0;
  while (*p++ == c) {
    nr++;
  }

  return nr;
}

static wchar_t* digit_clock_translate_wday(wchar_t* str, uint32_t size, uint32_t wday) {
  return_value_if_fail(wday < 7, NULL);

  static const char* wdays[] = {
      "Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat",
  };

  const char* utf8 = locale_info_tr(locale_info(), wdays[wday]);
  return utf8_to_utf16(utf8, str, size);
}

static wchar_t* digit_clock_translate_month(wchar_t* str, uint32_t size, uint32_t month) {
  return_value_if_fail(month < 13, NULL);

  static const char* months[] = {"Jan", "Feb", "Mar",  "Apr", "May", "Jun",
                                 "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};

  const char* utf8 = locale_info_tr(locale_info(), months[month - 1]);
  return utf8_to_utf16(utf8, str, size);
}

ret_t digit_clock_format_time(widget_t* widget, const char* format, date_time_t* dt) {
  wchar_t temp[32];
  wstr_t* str = NULL;
  const char* p = format;
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  str = &(widget->text);

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
          digit_clock_translate_month(temp, ARRAY_SIZE(temp), dt->month);
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
        digit_clock_translate_wday(temp, ARRAY_SIZE(temp), dt->wday);
        wstr_append(str, temp);
        break;
      }
      default: {
        wstr_push(str, *p);
        break;
      }
    }
    p += repeat;
  }

  return RET_OK;
}

static ret_t digit_clock_update_time(widget_t* widget) {
  date_time_t dt;
  const char* format = NULL;
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);
  return_value_if_fail(widget != NULL && digit_clock != NULL, RET_BAD_PARAMS);

  format = digit_clock->format ? digit_clock->format : "Y-M-D h:m:s";
  return_value_if_fail(strlen(format) < 64, RET_BAD_PARAMS);

  date_time_init(&dt);

  return digit_clock_format_time(widget, format, &dt);
}

ret_t digit_clock_set_format(widget_t* widget, const char* format) {
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  digit_clock->format = tk_str_copy(digit_clock->format, format);
  digit_clock_update_time(widget);

  return RET_OK;
}

static ret_t digit_clock_get_prop(widget_t* widget, const char* name, value_t* v) {
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    value_set_str(v, digit_clock->format);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t digit_clock_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    return digit_clock_set_format(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t digit_clock_on_timer(const timer_info_t* info) {
  widget_t* widget = NULL;
  return_value_if_fail(info != NULL, RET_REMOVE);

  widget = WIDGET(info->ctx);
  return_value_if_fail(widget != NULL, RET_REMOVE);

  digit_clock_update_time(widget);
  widget_invalidate(widget, NULL);

  return RET_REPEAT;
}

static ret_t digit_clock_on_destroy(widget_t* widget) {
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);
  return_value_if_fail(widget != NULL && digit_clock != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(digit_clock->format);

  return RET_OK;
}

static const char* s_digit_clock_properties[] = {WIDGET_PROP_FORMAT, NULL};

TK_DECL_VTABLE(digit_clock) = {.size = sizeof(digit_clock_t),
                               .type = WIDGET_TYPE_DIGIT_CLOCK,
                               .clone_properties = s_digit_clock_properties,
                               .persistent_properties = s_digit_clock_properties,
                               .parent = TK_PARENT_VTABLE(widget),
                               .create = digit_clock_create,
                               .on_paint_self = widget_on_paint_self_default,
                               .set_prop = digit_clock_set_prop,
                               .get_prop = digit_clock_get_prop,
                               .on_destroy = digit_clock_on_destroy};

widget_t* digit_clock_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(digit_clock), x, y, w, h);
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);

  return_value_if_fail(digit_clock != NULL, NULL);

  digit_clock_update_time(widget);
  widget_add_timer(widget, digit_clock_on_timer, 1000);

  return widget;
}

widget_t* digit_clock_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, digit_clock), NULL);

  return widget;
}
