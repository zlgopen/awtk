/**
 * File:   progress_bar.c
 * Author: AWTK Develop Team
 * Brief:  progress_bar
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
 * 2018-02-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"
#include "widgets/progress_bar.h"

static double progress_bar_get_progress(widget_t* widget) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL, 0);
  double range = progress_bar->max > 0 ? progress_bar->max : 100;

  range = tk_max(range, progress_bar->value);

  return progress_bar->value / range;
}

uint32_t progress_bar_get_percent(widget_t* widget) {
  double percent = progress_bar_get_progress(widget) * 100;

  return tk_roundi(percent);
}

static ret_t progress_bar_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t r;
  style_t* style = widget->astyle;
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  uint32_t radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);
  const char* bg_image = style_get_str(style, STYLE_ID_BG_IMAGE, NULL);
  image_draw_type_t draw_type = progress_bar->vertical ? IMAGE_DRAW_PATCH3_Y : IMAGE_DRAW_PATCH3_X;
  double progress = progress_bar_get_progress(widget);
  uint32_t progress_w = widget->w * progress;
  uint32_t progress_h = widget->h * progress;
  bool_t reverse = progress_bar->reverse;

  if (progress_bar->vertical) {
    r.x = 0;
    r.w = widget->w;
    r.h = widget->h - progress_h;
    r.y = reverse ? progress_h : 0;
  } else {
    r.y = 0;
    r.h = widget->h;
    r.w = widget->w - progress_w;
    r.x = reverse ? 0 : (widget->w - r.w);
  }

  if (radius || bg_image != NULL) {
    r = rect_init(0, 0, widget->w, widget->h);
  }

  widget_fill_bg_rect(widget, c, &r, draw_type);

  if (progress_bar->vertical) {
    r.x = 0;
    r.w = widget->w;
    r.h = progress_h;
    r.y = reverse ? 0 : widget->h - r.h;
  } else {
    r.h = widget->h;
    r.w = progress_w;
    r.y = 0;
    r.x = reverse ? (widget->w - progress_w) : 0;
  }
  widget_fill_fg_rect(widget, c, &r, draw_type);

  if (progress_bar->show_text) {
    char s[TK_NUM_MAX_LEN + TK_NUM_MAX_LEN + 1];
    const char* format = progress_bar->format != NULL ? progress_bar->format : "%d";
    if (strchr(format, 'd') != NULL || strchr(format, 'x') != NULL || strchr(format, 'X') != NULL) {
      tk_snprintf(s, sizeof(s), format, tk_roundi(progress_bar->value));
    } else {
      tk_snprintf(s, sizeof(s), format, progress_bar->value);
    }

    wstr_set_utf8(&(widget->text), s);
    return widget_paint_helper(widget, c, NULL, NULL);
  }

  r = rect_init(0, 0, widget->w, widget->h);
  widget_stroke_border_rect(widget, c, &r);

  return RET_OK;
}

static ret_t progress_bar_update_text(widget_t* widget) {
  char str[TK_NUM_MAX_LEN + 1];
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL, RET_BAD_PARAMS);

  tk_snprintf(str, TK_NUM_MAX_LEN, "%d%%", progress_bar_get_percent(widget));
  return widget_set_text_utf8(widget, str);
}

ret_t progress_bar_set_value(widget_t* widget, double value) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL, RET_BAD_PARAMS);

  if (progress_bar->value != value) {
    value_change_event_t evt;
    value_change_event_init(&evt, EVT_VALUE_WILL_CHANGE, widget);
    value_set_float(&(evt.old_value), progress_bar->value);
    value_set_float(&(evt.new_value), value);

    if (widget_dispatch(widget, (event_t*)&evt) != RET_STOP) {
      progress_bar->value = value;
      progress_bar_update_text(widget);
      evt.e.type = EVT_VALUE_CHANGED;
      widget_dispatch(widget, (event_t*)&evt);
      widget_invalidate(widget, NULL);
    }
  }

  return RET_OK;
}

ret_t progress_bar_set_vertical(widget_t* widget, bool_t vertical) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL, RET_BAD_PARAMS);

  progress_bar->vertical = vertical;

  return widget_invalidate(widget, NULL);
}

ret_t progress_bar_set_show_text(widget_t* widget, bool_t show_text) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL, RET_BAD_PARAMS);

  progress_bar->show_text = show_text;

  return widget_invalidate(widget, NULL);
}

ret_t progress_bar_set_reverse(widget_t* widget, bool_t reverse) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL, RET_BAD_PARAMS);

  progress_bar->reverse = reverse;

  return widget_invalidate(widget, NULL);
}

static ret_t progress_bar_on_destroy(widget_t* widget) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(widget != NULL && progress_bar != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(progress_bar->format);

  return RET_OK;
}

static ret_t progress_bar_get_prop(widget_t* widget, const char* name, value_t* v) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_float(v, progress_bar->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    value_set_float(v, progress_bar->max);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    value_set_str(v, progress_bar->format);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    value_set_bool(v, progress_bar->vertical);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SHOW_TEXT)) {
    value_set_bool(v, progress_bar->show_text);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_REVERSE)) {
    value_set_bool(v, progress_bar->reverse);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t progress_bar_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return progress_bar_set_value(widget, value_float(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    return progress_bar_set_max(widget, value_float(v));
  } else if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    return progress_bar_set_format(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    return progress_bar_set_vertical(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_SHOW_TEXT)) {
    return progress_bar_set_show_text(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_REVERSE)) {
    return progress_bar_set_reverse(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}

static const char* s_progress_bar_clone_properties[] = {WIDGET_PROP_VALUE,     WIDGET_PROP_MAX,
                                                        WIDGET_PROP_FORMAT,    WIDGET_PROP_VERTICAL,
                                                        WIDGET_PROP_SHOW_TEXT, NULL};
TK_DECL_VTABLE(progress_bar) = {.size = sizeof(progress_bar_t),
                                .type = WIDGET_TYPE_PROGRESS_BAR,
                                .clone_properties = s_progress_bar_clone_properties,
                                .parent = TK_PARENT_VTABLE(widget),
                                .create = progress_bar_create,
                                .on_paint_self = progress_bar_on_paint_self,
                                .on_paint_background = widget_on_paint_null,
                                .on_destroy = progress_bar_on_destroy,
                                .get_prop = progress_bar_get_prop,
                                .set_prop = progress_bar_set_prop};

widget_t* progress_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(progress_bar), x, y, w, h);
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL, NULL);

  progress_bar->max = 100;
  progress_bar->value = 0;
  progress_bar->vertical = FALSE;
  progress_bar->show_text = FALSE;

  return widget;
}

widget_t* progress_bar_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, progress_bar), NULL);

  return widget;
}

ret_t progress_bar_set_max(widget_t* widget, double max) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL, RET_BAD_PARAMS);

  if (progress_bar->max != max) {
    progress_bar->max = max;

    progress_bar_update_text(widget);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t progress_bar_set_format(widget_t* widget, const char* format) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL && format != NULL, RET_BAD_PARAMS);

  progress_bar->format = tk_str_copy(progress_bar->format, format);

  return widget_invalidate(widget, NULL);
}
