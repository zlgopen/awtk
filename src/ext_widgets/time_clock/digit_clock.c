/**
 * File:   digit_clock.h
 * Author: AWTK Develop Team
 * Brief:  digit_clock
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/timer.h"
#include "base/utils.h"
#include "base/widget_vtable.h"
#include "time_clock/digit_clock.h"

#define DATE_TIME_MAX_LEN 127

ret_t digit_clock_format_time(widget_t* widget, const char* format, date_time_t* dt) {
  wstr_t* str = &(widget->text);
  const char* p = format;

  str->size = 0;
  while (*p) {
    switch (*p) {
      case 'Y': {
        wstr_push_int(str, "%d", dt->year);
        break;
      }
      case 'M': {
        wstr_push_int(str, "%d", dt->month);
        break;
      }
      case 'D': {
        wstr_push_int(str, "%d", dt->day);
        break;
      }
      case 'h': {
        wstr_push_int(str, "%d", dt->hour);
        break;
      }
      case 'm': {
        wstr_push_int(str, "%d", dt->minute);
        break;
      }
      case 's': {
        wstr_push_int(str, "%d", dt->second);
        break;
      }
      default: {
        wstr_push(str, *p);
        break;
      }
    }
    p++;
  }

  return RET_OK;
}

static ret_t digit_clock_update_time(widget_t* widget) {
  date_time_t dt;
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);
  const char* format = digit_clock->format ? digit_clock->format : "Y-M-D h:m:s";
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

  if (tk_str_eq(name, DIGIT_CLOCK_PROP_FORMAT)) {
    value_set_str(v, digit_clock->format);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t digit_clock_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, DIGIT_CLOCK_PROP_FORMAT)) {
    return digit_clock_set_format(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t digit_clock_on_timer(const timer_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);

  digit_clock_update_time(widget);
  widget_invalidate(widget, NULL);

  return RET_REPEAT;
}

static ret_t digit_clock_destroy(widget_t* widget) {
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);

  TKMEM_FREE(digit_clock->format);

  return RET_OK;
}

static const char* s_digit_clock_properties[] = {DIGIT_CLOCK_PROP_FORMAT, NULL};

static const widget_vtable_t s_digit_clock_vtable = {
    .size = sizeof(digit_clock_t),
    .type = WIDGET_TYPE_DIGIT_CLOCK,
    .clone_properties = s_digit_clock_properties,
    .persistent_properties = s_digit_clock_properties,
    .create = digit_clock_create,
    .on_paint_self = widget_on_paint_self_default,
    .set_prop = digit_clock_set_prop,
    .get_prop = digit_clock_get_prop,
    .destroy = digit_clock_destroy};

widget_t* digit_clock_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  digit_clock_t* digit_clock = TKMEM_ZALLOC(digit_clock_t);
  widget_t* widget = WIDGET(digit_clock);
  return_value_if_fail(digit_clock != NULL, NULL);

  return_value_if_fail(widget_init(widget, parent, &s_digit_clock_vtable, x, y, w, h) != NULL,
                       widget);

  digit_clock_update_time(widget);
  widget_add_timer(widget, digit_clock_on_timer, 1000);

  return widget;
}

widget_t* digit_clock_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_digit_clock_vtable, NULL);

  return widget;
}
