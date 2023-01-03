/**
 * File:   digit_clock.c
 * Author: AWTK Develop Team
 * Brief:  digit_clock
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "base/locale_info.h"
#include "base/widget_vtable.h"
#include "base/date_time_format.h"
#include "widgets/digit_clock.h"

ret_t digit_clock_format_time(widget_t* widget, const char* format, date_time_t* dt) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return wstr_format_date_time(&(widget->text), format, dt);
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

static ret_t digit_clock_display_time(widget_t* widget) {
  ret_t ret = RET_OK;
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);
  return_value_if_fail(digit_clock != NULL, RET_BAD_PARAMS);

  ret = digit_clock_update_time(widget);

  if (ret == RET_OK && !wstr_equal(&(digit_clock->last_time), &(widget->text))) {
    widget_invalidate_force(widget, NULL);
    wstr_set(&(digit_clock->last_time), widget->text.str);
  }

  return ret;
}

static ret_t digit_clock_on_display_time(void* ctx, event_t* e) {
  (void)e;
  digit_clock_display_time(WIDGET(ctx));
  return RET_OK;
}

ret_t digit_clock_set_format(widget_t* widget, const char* format) {
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  digit_clock->format = tk_str_copy(digit_clock->format, format);
  digit_clock_display_time(widget);

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

  digit_clock_display_time(widget);

  return RET_REPEAT;
}

static ret_t digit_clock_on_destroy(widget_t* widget) {
  digit_clock_t* digit_clock = DIGIT_CLOCK(widget);
  return_value_if_fail(widget != NULL && digit_clock != NULL, RET_BAD_PARAMS);

  locale_info_off(locale_info(), digit_clock->local_changed_event_id);

  TKMEM_FREE(digit_clock->format);
  wstr_reset(&(digit_clock->last_time));

  return RET_OK;
}

static const char* const s_digit_clock_properties[] = {WIDGET_PROP_FORMAT, NULL};

TK_DECL_VTABLE(digit_clock) = {.size = sizeof(digit_clock_t),
                               .type = WIDGET_TYPE_DIGIT_CLOCK,
                               .clone_properties = s_digit_clock_properties,
                               .persistent_properties = s_digit_clock_properties,
                               .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
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
  wstr_init(&(digit_clock->last_time), 32);
  digit_clock->local_changed_event_id =
      locale_info_on(locale_info(), EVT_LOCALE_CHANGED, digit_clock_on_display_time, widget);

  return widget;
}

widget_t* digit_clock_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, digit_clock), NULL);

  return widget;
}
