/**
 * File:   progress_circle.c
 * Author: AWTK Develop Team
 * Brief:  progress_circle
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
 * 2018-11-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"
#include "progress_circle/progress_circle.h"

static ret_t progress_circle_on_paint_self(widget_t* widget, canvas_t* c) {
  bitmap_t img;
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  color_t color = style_get_color(style, STYLE_ID_FG_COLOR, trans);
  const char* image_name = style_get_str(style, STYLE_ID_FG_IMAGE, NULL);
  bool_t has_image = image_name && widget_load_image(widget, image_name, &img) == RET_OK;

  if (progress_circle->value > progress_circle->max) {
    progress_circle->value = progress_circle->max;
  }

  if (vg != NULL && (has_image || color.rgba.a)) {
    xy_t cx = widget->w / 2;
    xy_t cy = widget->h / 2;
    float_t end_angle = 0;
    float_t r = tk_min(cx, cy) - progress_circle->line_width / 2;
    bool_t ccw = progress_circle->counter_clock_wise;
    float_t start_angle = TK_D2R(progress_circle->start_angle);
    float_t angle = (M_PI * 2 * progress_circle->value) / progress_circle->max;

    if (ccw) {
      end_angle = start_angle - angle + M_PI * 2;
      if (fabs(end_angle - start_angle) < 0.001f) {
        end_angle = start_angle + 0.001f;
      }
    } else {
      end_angle = start_angle + angle;
    }

    vgcanvas_save(vg);
    vgcanvas_translate(vg, c->ox, c->oy);
    vgcanvas_set_stroke_color(vg, color);
    vgcanvas_set_line_width(vg, progress_circle->line_width);
    if (tk_str_eq(progress_circle->line_cap, VGCANVAS_LINE_CAP_ROUND)) {
      vgcanvas_set_line_cap(vg, progress_circle->line_cap);
    } else {
      vgcanvas_set_line_cap(vg, VGCANVAS_LINE_CAP_SQUARE);
    }
    vgcanvas_begin_path(vg);
    if (end_angle > start_angle) {
      vgcanvas_arc(vg, cx, cy, r, start_angle, end_angle, ccw);
      if (has_image) {
        vgcanvas_paint(vg, TRUE, &img);
      } else {
        vgcanvas_stroke(vg);
      }
    }

    vgcanvas_restore(vg);
  }

  color = style_get_color(style, STYLE_ID_TEXT_COLOR, trans);
  if (progress_circle->show_text && color.rgba.a) {
    char s[TK_NUM_MAX_LEN + TK_NUM_MAX_LEN + 1];
    const char* unit = progress_circle->unit != NULL ? progress_circle->unit : "";
    tk_snprintf(s, sizeof(s), "%u%s", (uint32_t)(progress_circle->value), unit);

    widget_set_text_utf8(widget, s);
    widget_paint_helper(widget, c, NULL, NULL);
  }

  return RET_OK;
}

ret_t progress_circle_set_value(widget_t* widget, float_t value) {
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  return_value_if_fail(progress_circle != NULL, RET_BAD_PARAMS);

  if (progress_circle->value != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    progress_circle->value = value;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t progress_circle_set_max(widget_t* widget, uint32_t max) {
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  return_value_if_fail(progress_circle != NULL, RET_BAD_PARAMS);

  progress_circle->max = max;

  return widget_invalidate(widget, NULL);
}

ret_t progress_circle_set_line_width(widget_t* widget, uint32_t line_width) {
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  return_value_if_fail(progress_circle != NULL, RET_BAD_PARAMS);

  progress_circle->line_width = line_width;

  return widget_invalidate(widget, NULL);
}

ret_t progress_circle_set_start_angle(widget_t* widget, int32_t start_angle) {
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  return_value_if_fail(progress_circle != NULL, RET_BAD_PARAMS);

  progress_circle->start_angle = start_angle;

  return widget_invalidate(widget, NULL);
}

ret_t progress_circle_set_unit(widget_t* widget, const char* unit) {
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  return_value_if_fail(progress_circle != NULL, RET_BAD_PARAMS);

  progress_circle->unit = tk_str_copy(progress_circle->unit, unit);

  return widget_invalidate(widget, NULL);
}

ret_t progress_circle_set_line_cap(widget_t* widget, const char* line_cap) {
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  return_value_if_fail(progress_circle != NULL, RET_BAD_PARAMS);

  progress_circle->line_cap = tk_str_copy(progress_circle->line_cap, line_cap);

  return widget_invalidate(widget, NULL);
}

ret_t progress_circle_set_show_text(widget_t* widget, bool_t show_text) {
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  return_value_if_fail(progress_circle != NULL, RET_BAD_PARAMS);

  progress_circle->show_text = show_text;

  return widget_invalidate(widget, NULL);
}

ret_t progress_circle_set_counter_clock_wise(widget_t* widget, bool_t counter_clock_wise) {
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  return_value_if_fail(progress_circle != NULL, RET_BAD_PARAMS);

  progress_circle->counter_clock_wise = counter_clock_wise;

  return widget_invalidate(widget, NULL);
}

static ret_t progress_circle_on_destroy(widget_t* widget) {
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  return_value_if_fail(widget != NULL && progress_circle != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(progress_circle->unit);
  TKMEM_FREE(progress_circle->line_cap);

  return RET_OK;
}

static ret_t progress_circle_get_prop(widget_t* widget, const char* name, value_t* v) {
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  return_value_if_fail(progress_circle != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_float(v, progress_circle->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    value_set_uint32(v, progress_circle->max);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SHOW_TEXT)) {
    value_set_bool(v, progress_circle->show_text);
    return RET_OK;
  } else if (tk_str_eq(name, PROGRESS_CIRCLE_PROP_UNIT)) {
    value_set_str(v, progress_circle->unit);
    return RET_OK;
  } else if (tk_str_eq(name, PROGRESS_CIRCLE_PROP_LINE_CAP)) {
    value_set_str(v, progress_circle->line_cap);
    return RET_OK;
  } else if (tk_str_eq(name, PROGRESS_CIRCLE_PROP_LINE_WIDTH)) {
    value_set_uint32(v, progress_circle->line_width);
    return RET_OK;
  } else if (tk_str_eq(name, PROGRESS_CIRCLE_PROP_START_ANGLE)) {
    value_set_int(v, progress_circle->start_angle);
    return RET_OK;
  } else if (tk_str_eq(name, PROGRESS_CIRCLE_PROP_COUNTER_CLOCK_WISE)) {
    value_set_bool(v, progress_circle->counter_clock_wise);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t progress_circle_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return progress_circle_set_value(widget, value_float(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    return progress_circle_set_max(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_SHOW_TEXT)) {
    return progress_circle_set_show_text(widget, value_bool(v));
  } else if (tk_str_eq(name, PROGRESS_CIRCLE_PROP_COUNTER_CLOCK_WISE)) {
    return progress_circle_set_counter_clock_wise(widget, value_bool(v));
  } else if (tk_str_eq(name, PROGRESS_CIRCLE_PROP_LINE_WIDTH)) {
    return progress_circle_set_line_width(widget, value_int(v));
  } else if (tk_str_eq(name, PROGRESS_CIRCLE_PROP_START_ANGLE)) {
    return progress_circle_set_start_angle(widget, value_int(v));
  } else if (tk_str_eq(name, PROGRESS_CIRCLE_PROP_UNIT)) {
    return progress_circle_set_unit(widget, value_str(v));
  } else if (tk_str_eq(name, PROGRESS_CIRCLE_PROP_LINE_CAP)) {
    return progress_circle_set_line_cap(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static const char* s_progress_circle_clone_properties[] = {WIDGET_PROP_VALUE,
                                                           WIDGET_PROP_MAX,
                                                           PROGRESS_CIRCLE_PROP_COUNTER_CLOCK_WISE,
                                                           PROGRESS_CIRCLE_PROP_LINE_WIDTH,
                                                           PROGRESS_CIRCLE_PROP_START_ANGLE,
                                                           PROGRESS_CIRCLE_PROP_UNIT,
                                                           WIDGET_PROP_SHOW_TEXT,
                                                           NULL};
TK_DECL_VTABLE(progress_circle) = {.size = sizeof(progress_circle_t),
                                   .type = WIDGET_TYPE_PROGRESS_CIRCLE,
                                   .clone_properties = s_progress_circle_clone_properties,
                                   .parent = TK_PARENT_VTABLE(widget),
                                   .create = progress_circle_create,
                                   .on_paint_self = progress_circle_on_paint_self,
                                   .on_destroy = progress_circle_on_destroy,
                                   .get_prop = progress_circle_get_prop,
                                   .set_prop = progress_circle_set_prop};

widget_t* progress_circle_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(progress_circle), x, y, w, h);
  progress_circle_t* progress_circle = PROGRESS_CIRCLE(widget);
  return_value_if_fail(progress_circle != NULL, NULL);

  progress_circle->max = 100;
  progress_circle->line_width = 8;
  progress_circle->start_angle = -90;
  progress_circle->show_text = TRUE;
  progress_circle->counter_clock_wise = FALSE;
  progress_circle_set_line_cap(widget, VGCANVAS_LINE_CAP_ROUND);

  return widget;
}

widget_t* progress_circle_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, progress_circle), NULL);

  return widget;
}
