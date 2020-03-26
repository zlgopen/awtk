/**
 * File:   time_clock.h
 * Author: AWTK Develop Team
 * Brief:  time_clock
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
 * 2018-08-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/timer.h"
#include "tkc/utils.h"
#include "tkc/matrix.h"
#include "tkc/date_time.h"
#include "base/image_manager.h"
#include "time_clock/time_clock.h"

static ret_t time_clock_update_time(time_clock_t* time_clock);

static bool_t time_clock_value_is_anchor_px(const char* value) {
  const char* tmp = NULL;
  size_t len = strlen(value);
  return_value_if_fail(len > 2, FALSE);

  tmp = value + len - 2;
  if (tk_str_eq(tmp, "px") != 0 || tk_str_eq(tmp, "PX") != 0) {
    return TRUE;
  }
  return FALSE;
}

ret_t time_clock_set_anchor_for_str(float_t max_size, const char* anchor, float_t* image_anchor) {
  float_t anchor_tmp = 0.0f;
  bool_t is_anchor_px = TRUE;

  return_value_if_fail(anchor != NULL, RET_BAD_PARAMS);

  anchor_tmp = tk_atof(anchor);
  is_anchor_px = time_clock_value_is_anchor_px(anchor);

  if (is_anchor_px) {
    return_value_if_fail(0 <= anchor_tmp && anchor_tmp <= max_size, RET_BAD_PARAMS);
  } else {
    return_value_if_fail(0 <= anchor_tmp && anchor_tmp <= 1.0f, RET_BAD_PARAMS);
  }

  *image_anchor = is_anchor_px ? anchor_tmp : anchor_tmp * max_size;

  return RET_OK;
}

ret_t time_clock_set_hour_anchor(widget_t* widget, const char* anchor_x, const char* anchor_y) {
  time_clock_t* time_clock = TIME_CLOCK(widget);

  return_value_if_fail(time_clock != NULL, RET_BAD_PARAMS);

  if (anchor_x != NULL) {
    time_clock->hour_anchor_x = tk_str_copy(time_clock->hour_anchor_x, anchor_x);
  }
  if (anchor_y != NULL) {
    time_clock->hour_anchor_y = tk_str_copy(time_clock->hour_anchor_y, anchor_y);
  }

  return RET_OK;
}

ret_t time_clock_set_minute_anchor(widget_t* widget, const char* anchor_x, const char* anchor_y) {
  time_clock_t* time_clock = TIME_CLOCK(widget);

  return_value_if_fail(time_clock != NULL, RET_BAD_PARAMS);

  if (anchor_x != NULL) {
    time_clock->minute_anchor_x = tk_str_copy(time_clock->minute_anchor_x, anchor_x);
  }
  if (anchor_y != NULL) {
    time_clock->minute_anchor_y = tk_str_copy(time_clock->minute_anchor_y, anchor_y);
  }

  return RET_OK;
}

ret_t time_clock_set_second_anchor(widget_t* widget, const char* anchor_x, const char* anchor_y) {
  time_clock_t* time_clock = TIME_CLOCK(widget);

  return_value_if_fail(time_clock != NULL, RET_BAD_PARAMS);

  if (anchor_x != NULL) {
    time_clock->second_anchor_x = tk_str_copy(time_clock->second_anchor_x, anchor_x);
  }
  if (anchor_y != NULL) {
    time_clock->second_anchor_y = tk_str_copy(time_clock->second_anchor_y, anchor_y);
  }

  return RET_OK;
}

ret_t time_clock_set_hour(widget_t* widget, int32_t hour) {
  time_clock_t* time_clock = TIME_CLOCK(widget);
  return_value_if_fail(time_clock != NULL, RET_BAD_PARAMS);

  time_clock->hour = hour;

  return RET_OK;
}

ret_t time_clock_set_minute(widget_t* widget, int32_t minute) {
  time_clock_t* time_clock = TIME_CLOCK(widget);
  return_value_if_fail(time_clock != NULL, RET_BAD_PARAMS);

  time_clock->minute = minute;

  return RET_OK;
}

ret_t time_clock_set_second(widget_t* widget, int32_t second) {
  time_clock_t* time_clock = TIME_CLOCK(widget);
  return_value_if_fail(time_clock != NULL, RET_BAD_PARAMS);

  time_clock->second = second;

  return RET_OK;
}

ret_t time_clock_set_hour_image(widget_t* widget, const char* hour_image) {
  time_clock_t* time_clock = TIME_CLOCK(widget);
  return_value_if_fail(time_clock != NULL, RET_BAD_PARAMS);

  time_clock->hour_image = tk_str_copy(time_clock->hour_image, hour_image);

  return RET_OK;
}

ret_t time_clock_set_minute_image(widget_t* widget, const char* minute_image) {
  time_clock_t* time_clock = TIME_CLOCK(widget);
  return_value_if_fail(time_clock != NULL, RET_BAD_PARAMS);

  time_clock->minute_image = tk_str_copy(time_clock->minute_image, minute_image);

  return RET_OK;
}

ret_t time_clock_set_second_image(widget_t* widget, const char* second_image) {
  time_clock_t* time_clock = TIME_CLOCK(widget);
  return_value_if_fail(time_clock != NULL, RET_BAD_PARAMS);

  time_clock->second_image = tk_str_copy(time_clock->second_image, second_image);

  return RET_OK;
}

ret_t time_clock_set_bg_image(widget_t* widget, const char* bg_image) {
  time_clock_t* time_clock = TIME_CLOCK(widget);
  return_value_if_fail(time_clock != NULL, RET_BAD_PARAMS);

  time_clock->bg_image = tk_str_copy(time_clock->bg_image, bg_image);

  return RET_OK;
}

ret_t time_clock_set_image(widget_t* widget, const char* image) {
  time_clock_t* time_clock = TIME_CLOCK(widget);
  return_value_if_fail(time_clock != NULL, RET_BAD_PARAMS);

  time_clock->image = tk_str_copy(time_clock->image, image);

  return RET_OK;
}

static ret_t time_clock_get_prop(widget_t* widget, const char* name, value_t* v) {
  time_clock_t* time_clock = TIME_CLOCK(widget);
  return_value_if_fail(time_clock != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, TIME_CLOCK_PROP_HOUR)) {
    value_set_int(v, time_clock->hour);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_MINUTE)) {
    value_set_int(v, time_clock->minute);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_SECOND)) {
    value_set_int(v, time_clock->second);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_HOUR_IMAGE)) {
    value_set_str(v, time_clock->hour_image);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_SECOND_IMAGE)) {
    value_set_str(v, time_clock->second_image);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_MINUTE_IMAGE)) {
    value_set_str(v, time_clock->minute_image);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_BG_IMAGE)) {
    value_set_str(v, time_clock->bg_image);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_IMAGE)) {
    value_set_str(v, time_clock->image);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_HOUR_ANCHOR_X)) {
    value_set_str(v, time_clock->hour_anchor_x);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_HOUR_ANCHOR_Y)) {
    value_set_str(v, time_clock->hour_anchor_y);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_MINUTE_ANCHOR_X)) {
    value_set_str(v, time_clock->minute_anchor_x);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_MINUTE_ANCHOR_Y)) {
    value_set_str(v, time_clock->minute_anchor_y);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_SECOND_ANCHOR_X)) {
    value_set_str(v, time_clock->second_anchor_x);
    return RET_OK;
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_SECOND_ANCHOR_Y)) {
    value_set_str(v, time_clock->second_anchor_y);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t time_clock_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, TIME_CLOCK_PROP_HOUR)) {
    return time_clock_set_hour(widget, value_int(v));
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_MINUTE)) {
    return time_clock_set_minute(widget, value_int(v));
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_SECOND)) {
    return time_clock_set_second(widget, value_int(v));
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_HOUR_IMAGE)) {
    return time_clock_set_hour_image(widget, value_str(v));
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_SECOND_IMAGE)) {
    return time_clock_set_second_image(widget, value_str(v));
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_MINUTE_IMAGE)) {
    return time_clock_set_minute_image(widget, value_str(v));
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_BG_IMAGE)) {
    return time_clock_set_bg_image(widget, value_str(v));
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_IMAGE)) {
    return time_clock_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_HOUR_ANCHOR_X)) {
    return time_clock_set_hour_anchor(widget, value_str(v), NULL);
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_HOUR_ANCHOR_Y)) {
    return time_clock_set_hour_anchor(widget, NULL, value_str(v));
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_MINUTE_ANCHOR_X)) {
    return time_clock_set_minute_anchor(widget, value_str(v), NULL);
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_MINUTE_ANCHOR_Y)) {
    return time_clock_set_minute_anchor(widget, NULL, value_str(v));
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_SECOND_ANCHOR_X)) {
    return time_clock_set_second_anchor(widget, value_str(v), NULL);
  } else if (tk_str_eq(name, TIME_CLOCK_PROP_SECOND_ANCHOR_Y)) {
    return time_clock_set_second_anchor(widget, NULL, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t time_clock_on_timer(const timer_info_t* info) {
  widget_t* widget = NULL;
  time_clock_t* time_clock = NULL;
  return_value_if_fail(info != NULL, RET_REMOVE);

  widget = WIDGET(info->ctx);
  time_clock = TIME_CLOCK(widget);
  return_value_if_fail(widget != NULL && time_clock != NULL, RET_BAD_PARAMS);

  if (time_clock_update_time(time_clock) != RET_OK) {
    time_clock->second++;
    if (time_clock->second >= 60) {
      time_clock->second = 0;
      time_clock->minute++;
      if (time_clock->minute >= 60) {
        time_clock->minute = 0;
        time_clock->hour++;
        if (time_clock->hour >= 12) {
          time_clock->hour = 0;
        }
      }
    }
  }

  widget_invalidate_force(widget, NULL);

  return RET_REPEAT;
}

static ret_t time_clock_on_destroy(widget_t* widget) {
  time_clock_t* time_clock = TIME_CLOCK(widget);
  return_value_if_fail(widget != NULL && time_clock != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(time_clock->image);
  TKMEM_FREE(time_clock->bg_image);
  TKMEM_FREE(time_clock->hour_image);
  TKMEM_FREE(time_clock->minute_image);
  TKMEM_FREE(time_clock->second_image);

  TKMEM_FREE(time_clock->hour_anchor_x);
  TKMEM_FREE(time_clock->hour_anchor_y);
  TKMEM_FREE(time_clock->minute_anchor_x);
  TKMEM_FREE(time_clock->minute_anchor_y);
  TKMEM_FREE(time_clock->second_anchor_x);
  TKMEM_FREE(time_clock->second_anchor_y);

  return RET_OK;
}

static ret_t time_clock_load_image(widget_t* widget, const char* name, bitmap_t* bitmap) {
  if (name != NULL && bitmap != NULL) {
    return widget_load_image(widget, name, bitmap);
  }

  return RET_FAIL;
}

static ret_t time_clock_draw_image(widget_t* widget, canvas_t* c, bitmap_t* img, float_t dx,
                                   float_t dy, float_t anchor_x, float_t anchor_y,
                                   float_t rotation) {
  vgcanvas_t* vg = canvas_get_vgcanvas(c);

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox + dx, c->oy + dy);
  vgcanvas_translate(vg, anchor_x, anchor_y);
  vgcanvas_rotate(vg, rotation);
  vgcanvas_translate(vg, -anchor_x, -anchor_y);
  vgcanvas_draw_image(vg, img, 0, 0, img->w, img->h, 0, 0, img->w, img->h);
  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t time_clock_on_paint_self(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  float_t rotation = 0;
  float_t anchor_x = 0;
  float_t anchor_y = 0;
  time_clock_t* time_clock = TIME_CLOCK(widget);
  rect_t dst = rect_init(0, 0, widget->w, widget->h);

  if (time_clock_load_image(widget, time_clock->bg_image, &bitmap) == RET_OK) {
    canvas_draw_image_ex(c, &bitmap, IMAGE_DRAW_CENTER, &dst);
  }

  if (time_clock_load_image(widget, time_clock->hour_image, &bitmap) == RET_OK &&
      time_clock_set_anchor_for_str(bitmap.w, time_clock->hour_anchor_x, &anchor_x) == RET_OK &&
      time_clock_set_anchor_for_str(bitmap.h, time_clock->hour_anchor_y, &anchor_y) == RET_OK) {
    float_t dx = dst.w / 2 - anchor_x;
    float_t dy = dst.h / 2 - anchor_y;
    float_t hour = time_clock->hour + time_clock->minute / 60.0f;

    rotation = (2 * M_PI * hour) / 12.0f;

    time_clock_draw_image(widget, c, &bitmap, dx, dy, anchor_x, anchor_y, rotation);
  }

  if (time_clock_load_image(widget, time_clock->minute_image, &bitmap) == RET_OK &&
      time_clock_set_anchor_for_str(bitmap.w, time_clock->minute_anchor_x, &anchor_x) == RET_OK &&
      time_clock_set_anchor_for_str(bitmap.h, time_clock->minute_anchor_y, &anchor_y) == RET_OK) {
    float_t dx = dst.w / 2 - anchor_x;
    float_t dy = dst.h / 2 - anchor_y;
    float_t minute = time_clock->minute + time_clock->second / 60.0f;

    rotation = (2 * M_PI * minute) / 60.0f;

    time_clock_draw_image(widget, c, &bitmap, dx, dy, anchor_x, anchor_y, rotation);
  }

  if (time_clock_load_image(widget, time_clock->second_image, &bitmap) == RET_OK &&
      time_clock_set_anchor_for_str(bitmap.w, time_clock->second_anchor_x, &anchor_x) == RET_OK &&
      time_clock_set_anchor_for_str(bitmap.h, time_clock->second_anchor_y, &anchor_y) == RET_OK) {
    float_t dx = dst.w / 2 - anchor_x;
    float_t dy = dst.h / 2 - anchor_y;

    rotation = (2 * M_PI * time_clock->second) / 60.0f;

    time_clock_draw_image(widget, c, &bitmap, dx, dy, anchor_x, anchor_y, rotation);
  }

  if (time_clock_load_image(widget, time_clock->image, &bitmap) == RET_OK) {
    canvas_draw_image_ex(c, &bitmap, IMAGE_DRAW_CENTER, &dst);
  }

  return RET_OK;
}

static const char* s_time_clock_properties[] = {
    TIME_CLOCK_PROP_HOUR,
    TIME_CLOCK_PROP_MINUTE,
    TIME_CLOCK_PROP_SECOND,
    TIME_CLOCK_PROP_IMAGE,
    TIME_CLOCK_PROP_BG_IMAGE,
    TIME_CLOCK_PROP_HOUR_IMAGE,
    TIME_CLOCK_PROP_MINUTE_IMAGE,
    TIME_CLOCK_PROP_SECOND_IMAGE,
    TIME_CLOCK_PROP_HOUR_ANCHOR_X TIME_CLOCK_PROP_HOUR_ANCHOR_Y,
    TIME_CLOCK_PROP_MINUTE_ANCHOR_X,
    TIME_CLOCK_PROP_MINUTE_ANCHOR_Y TIME_CLOCK_PROP_SECOND_ANCHOR_X,
    TIME_CLOCK_PROP_SECOND_ANCHOR_Y,
    NULL};

TK_DECL_VTABLE(time_clock) = {.size = sizeof(time_clock_t),
                              .type = WIDGET_TYPE_TIME_CLOCK,
                              .clone_properties = s_time_clock_properties,
                              .persistent_properties = s_time_clock_properties,
                              .parent = TK_PARENT_VTABLE(widget),
                              .create = time_clock_create,
                              .on_paint_self = time_clock_on_paint_self,
                              .set_prop = time_clock_set_prop,
                              .get_prop = time_clock_get_prop,
                              .on_destroy = time_clock_on_destroy};

static ret_t time_clock_update_time(time_clock_t* time_clock) {
  date_time_t dt;

  if (date_time_init(&dt) != NULL) {
    time_clock->hour = dt.hour % 12;
    time_clock->minute = dt.minute;
    time_clock->second = dt.second;

    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

widget_t* time_clock_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(time_clock), x, y, w, h);
  time_clock_t* time_clock = TIME_CLOCK(widget);
  return_value_if_fail(time_clock != NULL, NULL);

  time_clock_update_time(time_clock);
  widget_add_timer(widget, time_clock_on_timer, 1000);

  time_clock->hour_anchor_x = tk_str_copy(NULL, "0.5");
  time_clock->hour_anchor_y = tk_str_copy(NULL, "0.5");
  time_clock->minute_anchor_x = tk_str_copy(NULL, "0.5");
  time_clock->minute_anchor_y = tk_str_copy(NULL, "0.5");
  time_clock->second_anchor_x = tk_str_copy(NULL, "0.5");
  time_clock->second_anchor_y = tk_str_copy(NULL, "0.5");

  return widget;
}

widget_t* time_clock_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, time_clock), NULL);

  return widget;
}
