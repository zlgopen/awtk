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
#include "base/date_time.h"
#include "base/widget_vtable.h"
#include "time_clock/digit_clock.h"

static ret_t digit_clock_update_time(widget_t* widget) {
  char str[128];
  date_time_t dt;
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);
  const char* format = digit_clock->format;

  date_time_init(&dt);
  memset(str, 0x00, sizeof(str));
  if (digit_clock->show_date && digit_clock->show_time) {
    format = format != NULL ? format : "%d-%02d-%02d %02d:%02d:%02d";
    tk_snprintf(str, sizeof(str) - 1, format, dt.year, dt.month, dt.day, dt.hour, dt.minute,
                dt.second);
  } else if (digit_clock->show_date) {
    format = format != NULL ? format : "%d-%02d-%02d";
    tk_snprintf(str, sizeof(str) - 1, format, dt.year, dt.month, dt.day);
  } else {
    format = format != NULL ? format : "%02d:%02d:%02d";
    tk_snprintf(str, sizeof(str) - 1, format, dt.hour, dt.minute, dt.second);
  }

  widget_set_text_utf8(widget, str);

  return RET_OK;
}

ret_t digit_clock_set_show_date(widget_t* widget, bool_t show_date) {
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  digit_clock->show_date = show_date;
  digit_clock_update_time(widget);

  return RET_OK;
}

ret_t digit_clock_set_show_time(widget_t* widget, bool_t show_time) {
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  digit_clock->show_time = show_time;
  digit_clock_update_time(widget);

  return RET_OK;
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

  if (tk_str_eq(name, DIGIT_CLOCK_PROP_SHOW_DATE)) {
    value_set_bool(v, digit_clock->show_date);
    return RET_OK;
  } else if (tk_str_eq(name, DIGIT_CLOCK_PROP_SHOW_TIME)) {
    value_set_bool(v, digit_clock->show_time);
    return RET_OK;
  } else if (tk_str_eq(name, DIGIT_CLOCK_PROP_FORMAT)) {
    value_set_str(v, digit_clock->format);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t digit_clock_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, DIGIT_CLOCK_PROP_SHOW_DATE)) {
    return digit_clock_set_show_date(widget, value_bool(v));
  } else if (tk_str_eq(name, DIGIT_CLOCK_PROP_SHOW_TIME)) {
    return digit_clock_set_show_time(widget, value_bool(v));
  } else if (tk_str_eq(name, DIGIT_CLOCK_PROP_FORMAT)) {
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

static const char* s_digit_clock_properties[] = {
    DIGIT_CLOCK_PROP_SHOW_DATE, DIGIT_CLOCK_PROP_SHOW_TIME, DIGIT_CLOCK_PROP_FORMAT, NULL};

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

  digit_clock->show_date = TRUE;
  digit_clock->show_time = TRUE;
  digit_clock_update_time(widget);
  widget_add_timer(widget, digit_clock_on_timer, 1000);

  return widget;
}

widget_t* digit_clock_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_digit_clock_vtable, NULL);

  return widget;
}
