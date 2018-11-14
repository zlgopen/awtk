/**
 * File:   slider.c
 * Author: AWTK Develop Team
 * Brief:  slider
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
 * 2018-04-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/rect.h"
#include "base/utils.h"
#include "base/slider.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"

static ret_t slider_set_value_internal(widget_t* widget, uint16_t value, event_type_t etype,
                                       bool_t force);

static ret_t slider_get_dragger_rect(widget_t* widget, rect_t* r) {
  slider_t* slider = SLIDER(widget);
  uint16_t value = slider->value - slider->min;
  uint16_t range = slider->max - slider->min;
  float fvalue = (float)value / (float)range;
  return_value_if_fail(r != NULL, RET_BAD_PARAMS);

  r->w = tk_min(widget->w, widget->h);
  r->h = r->w;
  if (slider->vertical) {
    r->x = 0;
    r->y = (widget->h - r->h) * (1 - fvalue);
  } else {
    r->y = 0;
    r->x = (widget->w - r->w) * fvalue;
  }

  return RET_OK;
}

static ret_t slider_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t r;
  xy_t x = 0;
  xy_t y = 0;
  wh_t w = 0;
  wh_t h = 0;
  bitmap_t img;
  color_t color;
  float fvalue = 0;
  const char* image_name = NULL;
  style_t* style = widget->astyle;
  slider_t* slider = SLIDER(widget);
  color_t trans = color_init(0, 0, 0, 0);
  uint16_t value = slider->value - slider->min;
  uint16_t range = slider->max - slider->min;
  image_draw_type_t draw_type = IMAGE_DRAW_CENTER;

  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);
  return_value_if_fail(slider->max > slider->min, RET_BAD_PARAMS);
  return_value_if_fail(slider->value >= slider->min && slider->value <= slider->max,
                       RET_BAD_PARAMS);

  fvalue = (float)value / (float)range;

  if (slider->vertical) {
    w = widget->w >> 1;
    h = (widget->h - widget->w) * fvalue + w;
    x = widget->w >> 2;
    y = widget->h - h;
  } else {
    x = 0;
    h = widget->h >> 1;
    w = (widget->w - widget->h) * fvalue + h;
    y = widget->h >> 2;
  }

  r = rect_init(x, y, w, h);
  color = style_get_color(style, STYLE_ID_FG_COLOR, trans);
  if (color.rgba.a) {
    canvas_set_fill_color(c, color);
    canvas_fill_rect(c, r.x, r.y, r.w, r.h);
  }

  image_name = style_get_str(style, STYLE_ID_FG_IMAGE, NULL);
  draw_type =
      (image_draw_type_t)style_get_int(style, STYLE_ID_FG_IMAGE_DRAW_TYPE, IMAGE_DRAW_PATCH3_X);
  if (image_name && image_manager_load(image_manager(), image_name, &img) == RET_OK) {
    if (slider->vertical) {
      r.x = 0;
      r.w = widget->w;
    } else {
      r.y = 0;
      r.h = widget->h;
    }
    canvas_draw_image_ex(c, &img, draw_type, &r);
  }

  if (slider->vertical) {
    h = widget->h - h;
    w = widget->w >> 1;
    x = widget->w >> 2;
    y = 0;
  } else {
    x = w;
    w = widget->w - w;
    h = widget->h >> 1;
    y = widget->h >> 2;
  }

  r = rect_init(x, y, w, h);
  color = style_get_color(style, STYLE_ID_BG_COLOR, trans);
  if (color.rgba.a) {
    canvas_set_fill_color(c, color);
    canvas_fill_rect(c, r.x, r.y, r.w, r.h);
  }
  image_name = style_get_str(style, STYLE_ID_BG_IMAGE, NULL);
  draw_type =
      (image_draw_type_t)style_get_int(style, STYLE_ID_BG_IMAGE_DRAW_TYPE, IMAGE_DRAW_PATCH3_X);
  if (image_name && image_manager_load(image_manager(), image_name, &img) == RET_OK) {
    if (slider->vertical) {
      r.x = 0;
      r.w = widget->w;
    } else {
      r.y = 0;
      r.h = widget->h;
    }
    canvas_draw_image_ex(c, &img, draw_type, &r);
  }

  slider_get_dragger_rect(widget, &r);
  color = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);
  if (color.rgba.a) {
    canvas_set_fill_color(c, color);
    canvas_fill_rect(c, r.x, r.y, r.w, r.h);
  }
  image_name = style_get_str(style, STYLE_ID_ICON, NULL);
  if (image_name && image_manager_load(image_manager(), image_name, &img) == RET_OK) {
    canvas_draw_image_ex(c, &img, IMAGE_DRAW_CENTER, &r);
  }

  return RET_OK;
}

static ret_t slider_on_event(widget_t* widget, event_t* e) {
  rect_t r;
  uint16_t type = e->type;
  slider_t* slider = SLIDER(widget);

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t* evt = (pointer_event_t*)e;
      point_t p = {evt->x, evt->y};

      slider_get_dragger_rect(widget, &r);
      widget_to_local(widget, &p);
      if (rect_contains(&r, p.x, p.y)) {
        slider->dragging = TRUE;
        widget_set_state(widget, WIDGET_STATE_PRESSED);
        widget_grab(widget->parent, widget);
        widget_invalidate(widget, NULL);
      }
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      point_t p = {evt->x, evt->y};
      uint16_t value = 0;
      if (slider->dragging) {
        float fvalue = 0;
        widget_to_local(widget, &p);

        if (slider->vertical) {
          fvalue = 1 - (float)(p.y) / widget->h;
        } else {
          fvalue = (float)(p.x) / widget->w;
        }
        if (fvalue < 0) {
          fvalue = 0;
        }
        if (fvalue > 1) {
          fvalue = 1;
        }
        value = fvalue * (slider->max - slider->min) + slider->min;
        slider_set_value_internal(widget, value, EVT_VALUE_CHANGING, FALSE);
      }

      break;
    }
    case EVT_POINTER_UP: {
      if (slider->dragging) {
        slider->dragging = FALSE;
        widget_ungrab(widget->parent, widget);
        slider_set_value_internal(widget, slider->value, EVT_VALUE_CHANGED, TRUE);
      }
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      widget_invalidate(widget, NULL);
      break;
    }
    case EVT_POINTER_LEAVE:
      widget_set_state(widget, slider->dragging ? WIDGET_STATE_PRESSED : WIDGET_STATE_NORMAL);
      break;
    case EVT_POINTER_ENTER:
      widget_set_state(widget, slider->dragging ? WIDGET_STATE_PRESSED : WIDGET_STATE_OVER);
      break;
    default:
      break;
  }

  return RET_OK;
}

static ret_t slider_set_value_internal(widget_t* widget, uint16_t value, event_type_t etype,
                                       bool_t force) {
  uint16_t step = 0;
  uint16_t offset = 0;
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  step = slider->step;
  value = tk_max(tk_min(value, slider->max), slider->min);

  if (step > 1) {
    offset = value - slider->min;
    value = slider->min + ((offset + (step >> 1) - 1) / step) * step;
  }

  if (slider->value != value || force) {
    event_t evt = event_init(etype, widget);

    slider->value = value;
    widget_invalidate(widget, NULL);
    widget_dispatch(widget, &evt);
  }

  return RET_OK;
}

ret_t slider_set_value(widget_t* widget, uint16_t value) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL, RET_BAD_PARAMS);

  if (slider->value != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);

    return slider_set_value_internal(widget, value, EVT_VALUE_CHANGED, FALSE);
  }

  return RET_OK;
}

ret_t slider_set_min(widget_t* widget, uint16_t min) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  slider->min = min;

  return RET_OK;
}

ret_t slider_set_max(widget_t* widget, uint16_t max) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  slider->max = max;

  return RET_OK;
}

ret_t slider_set_step(widget_t* widget, uint16_t step) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL && step > 0, RET_BAD_PARAMS);

  slider->step = step;

  return RET_OK;
}

ret_t slider_set_vertical(widget_t* widget, bool_t vertical) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  slider->vertical = vertical;

  return RET_OK;
}

static ret_t slider_get_prop(widget_t* widget, const char* name, value_t* v) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_int(v, slider->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    value_set_bool(v, slider->vertical);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    value_set_int(v, slider->min);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    value_set_int(v, slider->max);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    value_set_int(v, slider->step);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t slider_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return slider_set_value(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    return slider_set_vertical(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    return slider_set_min(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    return slider_set_max(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    return slider_set_step(widget, value_int(v));
  }

  return RET_NOT_FOUND;
}

static const char* s_slider_properties[] = {WIDGET_PROP_VALUE, WIDGET_PROP_VERTICAL,
                                            WIDGET_PROP_MIN,   WIDGET_PROP_MAX,
                                            WIDGET_PROP_STEP,  NULL};
static const widget_vtable_t s_slider_vtable = {.size = sizeof(slider_t),
                                                .type = WIDGET_TYPE_SLIDER,
                                                .clone_properties = s_slider_properties,
                                                .persistent_properties = s_slider_properties,
                                                .create = slider_create,
                                                .on_event = slider_on_event,
                                                .on_paint_self = slider_on_paint_self,
                                                .on_paint_border = widget_on_paint_null,
                                                .on_paint_background = widget_on_paint_null,
                                                .get_prop = slider_get_prop,
                                                .set_prop = slider_set_prop};

widget_t* slider_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  slider_t* slider = TKMEM_ZALLOC(slider_t);
  widget_t* widget = WIDGET(slider);
  return_value_if_fail(slider != NULL, NULL);

  widget_init(widget, parent, &s_slider_vtable, x, y, w, h);

  slider->min = 0;
  slider->max = 100;
  slider->step = 1;
  slider->value = 0;

  return widget;
}

widget_t* slider_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_slider_vtable, NULL);

  return widget;
}
