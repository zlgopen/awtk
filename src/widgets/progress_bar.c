/**
 * File:   progress_bar.c
 * Author: AWTK Develop Team
 * Brief:  progress_bar
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
 * 2018-02-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "widgets/progress_bar.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"

static ret_t progress_bar_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t r;
  style_t* style = widget->astyle;
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  uint32_t radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);
  const char* bg_image = style_get_str(style, STYLE_ID_BG_IMAGE, NULL);
  image_draw_type_t draw_type = progress_bar->vertical ? IMAGE_DRAW_PATCH3_Y : IMAGE_DRAW_PATCH3_X;

  if (progress_bar->vertical) {
    r.x = 0;
    r.y = 0;
    r.w = widget->w;
    r.h = widget->h - (widget->h * progress_bar->value) / 100;
  } else {
    r.y = 0;
    r.h = widget->h;
    r.w = widget->w - (widget->w * progress_bar->value) / 100;
    r.x = widget->w - r.w;
  }

  if (radius || bg_image != NULL) {
    r = rect_init(0, 0, widget->w, widget->h);
  }

  widget_fill_bg_rect(widget, c, &r, draw_type);

  if (progress_bar->vertical) {
    r.x = 0;
    r.w = widget->w;
    r.h = (widget->h * progress_bar->value) / 100;
    r.y = widget->h - r.h;
  } else {
    r.h = widget->h;
    r.w = (widget->w * progress_bar->value) / 100;
    r.y = 0;
    r.x = 0;
  }
  widget_fill_fg_rect(widget, c, &r, draw_type);

  if (progress_bar->show_text) {
    char str[TK_NUM_MAX_LEN + 1];

    tk_snprintf(str, TK_NUM_MAX_LEN, "%d%%", progress_bar->value);
    widget_set_text_utf8(widget, str);

    return widget_paint_helper(widget, c, NULL, NULL);
  }

  r = rect_init(0, 0, widget->w, widget->h);
  widget_stroke_border_rect(widget, c, &r);

  return RET_OK;
}

ret_t progress_bar_set_value(widget_t* widget, uint8_t value) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL && value <= 100, RET_BAD_PARAMS);

  if (progress_bar->value != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    progress_bar->value = value;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
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

static ret_t progress_bar_get_prop(widget_t* widget, const char* name, value_t* v) {
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_uint8(v, progress_bar->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    value_set_bool(v, progress_bar->vertical);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SHOW_TEXT)) {
    value_set_bool(v, progress_bar->show_text);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t progress_bar_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return progress_bar_set_value(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    return progress_bar_set_vertical(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_SHOW_TEXT)) {
    return progress_bar_set_show_text(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}

static const char* s_progress_bar_clone_properties[] = {WIDGET_PROP_VALUE, WIDGET_PROP_VERTICAL,
                                                        WIDGET_PROP_SHOW_TEXT, NULL};
TK_DECL_VTABLE(progress_bar) = {.size = sizeof(progress_bar_t),
                                .type = WIDGET_TYPE_PROGRESS_BAR,
                                .clone_properties = s_progress_bar_clone_properties,
                                .parent = TK_PARENT_VTABLE(widget),
                                .create = progress_bar_create,
                                .on_paint_self = progress_bar_on_paint_self,
                                .on_paint_background = widget_on_paint_null,
                                .get_prop = progress_bar_get_prop,
                                .set_prop = progress_bar_set_prop};

widget_t* progress_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(progress_bar), x, y, w, h);
  progress_bar_t* progress_bar = PROGRESS_BAR(widget);
  return_value_if_fail(progress_bar != NULL, NULL);

  progress_bar->value = 0;
  progress_bar->vertical = FALSE;
  progress_bar->show_text = FALSE;

  return widget;
}

widget_t* progress_bar_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, progress_bar), NULL);

  return widget;
}
