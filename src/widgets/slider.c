/**
 * File:   slider.c
 * Author: AWTK Develop Team
 * Brief:  slider
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
 * History:aod
 * 2018-04-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/rect.h"
#include "tkc/utils.h"
#include "base/keys.h"
#include "widgets/slider.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"

static ret_t slider_load_icon(widget_t* widget, bitmap_t* img) {
  style_t* style = widget->astyle;
  const char* image_name = style_get_str(style, STYLE_ID_ICON, NULL);
  if (image_name && widget_load_image(widget, image_name, img) == RET_OK) {
    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

static ret_t slider_update_dragger_rect(widget_t* widget, canvas_t* c) {
  bitmap_t img;
  double fvalue = 0;
  double value = 0;
  double range = 0;
  float_t ratio = c->lcd->ratio;
  slider_t* slider = SLIDER(widget);
  rect_t* r = &(slider->dragger_rect);
  return_value_if_fail(slider != NULL && r != NULL, RET_BAD_PARAMS);

  value = slider->value - slider->min;
  range = slider->max - slider->min;
  fvalue = (double)value / (double)range;

  if (slider_load_icon(widget, &img) == RET_OK) {
    r->w = img.w / ratio;
    r->h = img.h / ratio;
  } else {
    r->w = tk_min(widget->w, widget->h);
    r->h = r->w;
  }

  if (slider->vertical) {
    r->x = (widget->w - r->w) / 2;
    r->y = tk_roundi((widget->h - r->h) * (1 - fvalue));
  } else {
    r->y = (widget->h - r->h) / 2;
    r->x = tk_roundi((widget->w - r->w) * fvalue);
  }

  return RET_OK;
}

static ret_t slider_paint_dragger(widget_t* widget, canvas_t* c) {
  bitmap_t img;
  color_t color;
  style_t* style = widget->astyle;
  slider_t* slider = SLIDER(widget);
  rect_t* r = &(slider->dragger_rect);
  color_t trans = color_init(0, 0, 0, 0);

  slider_update_dragger_rect(widget, c);
  color = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);
  if (color.rgba.a) {
    canvas_set_fill_color(c, color);
    canvas_fill_rect(c, r->x, r->y, r->w, r->h);
  } else {
    if (slider_load_icon(widget, &img) == RET_OK) {
      canvas_draw_image_ex(c, &img, IMAGE_DRAW_ICON, r);
    }
  }

  slider->dragger_size = slider->vertical ? r->h : r->w;

  return RET_OK;
}

static uint32_t slider_get_bar_size(widget_t* widget) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL && widget != NULL, 0);

  if (slider->vertical) {
    return slider->bar_size ? slider->bar_size : (widget->w >> 1);
  } else {
    return slider->bar_size ? slider->bar_size : (widget->h >> 1);
  }
}

static ret_t slider_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t r;
  style_t* style = NULL;
  double range = 0;
  double value = 0;
  float_t fvalue = 0;
  uint32_t radius = 0;
  const char* bg_image = 0;
  image_draw_type_t draw_type;
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL && slider != NULL, RET_BAD_PARAMS);

  style = widget->astyle;
  range = slider->max - slider->min;
  value = slider->value - slider->min;
  fvalue = (float_t)value / (float_t)range;
  radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);
  bg_image = style_get_str(style, STYLE_ID_BG_IMAGE, NULL);
  draw_type = slider->vertical ? IMAGE_DRAW_PATCH3_Y : IMAGE_DRAW_PATCH3_X;

  if (slider->vertical) {
    r.y = 0;
    r.w = slider_get_bar_size(widget);
    r.x = (widget->w - r.w) / 2;
    r.h = widget->h - (widget->h * fvalue);
  } else {
    r.h = slider_get_bar_size(widget);
    r.y = (widget->h - r.h) / 2;
    r.w = widget->w - (widget->w * fvalue);
    r.x = widget->w - r.w;
  }

  if (radius || bg_image != NULL) {
    r = rect_init(0, 0, widget->w, widget->h);
  }

  widget_fill_bg_rect(widget, c, &r, draw_type);

  if (slider->vertical) {
    r.w = slider_get_bar_size(widget);
    r.x = (widget->w - r.w) / 2;
    r.h = (widget->h * fvalue);
    r.y = widget->h - r.h;
  } else {
    r.h = slider_get_bar_size(widget);
    r.y = (widget->h - r.h) / 2;
    r.w = (widget->w * fvalue);
    r.x = 0;
  }
  widget_fill_fg_rect(widget, c, &r, draw_type);

  slider_paint_dragger(widget, c);

  if (slider->vertical) {
    r.x = widget->w >> 2;
    r.w = widget->w >> 1;
    r.h = (widget->h * fvalue);
    r.y = widget->h - r.h;
  } else {
    r.h = widget->h >> 1;
    r.w = (widget->w * fvalue);
    r.y = widget->h >> 2;
    r.x = 0;
  }

  return RET_OK;
}

static ret_t slider_pointer_up_cleanup(widget_t* widget) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL && slider != NULL, RET_BAD_PARAMS);

  slider->dragging = FALSE;
  widget_ungrab(widget->parent, widget);
  widget_set_state(widget, WIDGET_STATE_NORMAL);
  widget_invalidate(widget, NULL);

  return RET_OK;
}

static ret_t slider_add_value(widget_t* widget, double delta) {
  double new_value = 0;
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL && slider != NULL, RET_BAD_PARAMS);

  new_value = slider->value + delta;

  if (new_value < slider->min) {
    new_value = slider->min;
  }

  if (new_value > slider->max) {
    new_value = slider->max;
  }

  return slider_set_value(widget, new_value);
}

ret_t slider_inc(widget_t* widget) {
  ret_t ret = RET_OK;
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL && slider != NULL, RET_BAD_PARAMS);

  if (slider->step) {
    ret = slider_add_value(widget, slider->step);
  } else {
    ret = slider_add_value(widget, 1);
  }

  return ret;
}

ret_t slider_dec(widget_t* widget) {
  ret_t ret = RET_OK;
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL && slider != NULL, RET_BAD_PARAMS);

  if (slider->step) {
    ret = slider_add_value(widget, -slider->step);
  } else {
    ret = slider_add_value(widget, -1);
  }

  return ret;
}

static ret_t slider_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint16_t type = e->type;
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL && slider != NULL, RET_BAD_PARAMS);

  ret = slider->dragging ? RET_STOP : RET_OK;
  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t* evt = (pointer_event_t*)e;
      point_t p = {evt->x, evt->y};
      rect_t* r = &(slider->dragger_rect);

      widget_to_local(widget, &p);
      if (rect_contains(r, p.x, p.y)) {
        slider->dragging = TRUE;
        widget_set_state(widget, WIDGET_STATE_PRESSED);
        widget_grab(widget->parent, widget);
        widget_invalidate(widget, NULL);
      }
      slider->down = p;
      slider->saved_value = slider->value;
      ret = slider->dragging ? RET_STOP : RET_OK;
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      slider_pointer_up_cleanup(widget);
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      point_t p = {evt->x, evt->y};
      double value = 0;
      if (slider->dragging) {
        double delta = 0;
        widget_to_local(widget, &p);
        double range = slider->max - slider->min;

        if (slider->vertical) {
          delta = range * (slider->down.y - p.y) / (widget->h - slider->dragger_size);
        } else {
          delta = range * (p.x - slider->down.x) / (widget->w - slider->dragger_size);
        }

        value = slider->saved_value + delta;
        value = tk_clampi(value, slider->min, slider->max);
        slider_set_value_internal(widget, (double)value, EVT_VALUE_CHANGING, FALSE);
      }

      break;
    }
    case EVT_POINTER_UP: {
      if (slider->dragging) {
        slider_set_value_internal(widget, slider->value, EVT_VALUE_CHANGED, TRUE);
      }
      slider_pointer_up_cleanup(widget);
      break;
    }
    case EVT_POINTER_LEAVE:
      widget_set_state(widget, slider->dragging ? WIDGET_STATE_PRESSED : WIDGET_STATE_NORMAL);
      break;
    case EVT_POINTER_ENTER:
      widget_set_state(widget, slider->dragging ? WIDGET_STATE_PRESSED : WIDGET_STATE_OVER);
      break;
    case EVT_KEY_DOWN: {
      key_event_t* evt = (key_event_t*)e;
      if (slider->vertical) {
        if (evt->key == TK_KEY_UP) {
          slider_inc(widget);
        } else if (evt->key == TK_KEY_DOWN) {
          slider_dec(widget);
        }
      } else {
        if (evt->key == TK_KEY_LEFT) {
          slider_dec(widget);
        } else if (evt->key == TK_KEY_RIGHT) {
          slider_inc(widget);
        }
      }
      break;
    }
    default: {
      ret = RET_OK;
      break;
    }
  }

  return ret;
}

ret_t slider_set_value_internal(widget_t* widget, double value, event_type_t etype, bool_t force) {
  double step = 0;
  double offset = 0;
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL, RET_BAD_PARAMS);

  step = slider->step;
  value = tk_clampi(value, slider->min, slider->max);

  if (step > 0) {
    offset = value - slider->min;
    offset = tk_roundi(offset / step) * step;
    value = slider->min + offset;
  }

  if (slider->value != value || force) {
    event_t evt = event_init(etype, widget);

    slider->value = value;
    widget_invalidate(widget, NULL);
    widget_dispatch(widget, &evt);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t slider_set_value(widget_t* widget, double value) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL, RET_BAD_PARAMS);

  if (slider->value != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);

    return slider_set_value_internal(widget, value, EVT_VALUE_CHANGED, FALSE);
  }

  return RET_OK;
}

ret_t slider_set_min(widget_t* widget, double min) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL, RET_BAD_PARAMS);

  slider->min = min;

  return widget_invalidate(widget, NULL);
}

ret_t slider_set_max(widget_t* widget, double max) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL, RET_BAD_PARAMS);

  slider->max = max;

  return widget_invalidate(widget, NULL);
}

ret_t slider_set_step(widget_t* widget, double step) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL && step >= 0, RET_BAD_PARAMS);

  slider->step = step;

  return widget_invalidate(widget, NULL);
}

ret_t slider_set_bar_size(widget_t* widget, uint32_t bar_size) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL && bar_size > 0, RET_BAD_PARAMS);

  slider->bar_size = bar_size;

  return widget_invalidate(widget, NULL);
}

ret_t slider_set_vertical(widget_t* widget, bool_t vertical) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL, RET_BAD_PARAMS);

  slider->vertical = vertical;

  return widget_invalidate(widget, NULL);
}

static ret_t slider_get_prop(widget_t* widget, const char* name, value_t* v) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_double(v, slider->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    value_set_bool(v, slider->vertical);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    value_set_double(v, slider->min);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    value_set_double(v, slider->max);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    value_set_double(v, slider->step);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_BAR_SIZE)) {
    value_set_int(v, slider->bar_size);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t slider_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return slider_set_value(widget, value_double(v));
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    return slider_set_vertical(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    return slider_set_min(widget, value_double(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    return slider_set_max(widget, value_double(v));
  } else if (tk_str_eq(name, WIDGET_PROP_STEP)) {
    return slider_set_step(widget, value_double(v));
  } else if (tk_str_eq(name, WIDGET_PROP_BAR_SIZE)) {
    return slider_set_bar_size(widget, value_int(v));
  }

  return RET_NOT_FOUND;
}

static const char* s_slider_properties[] = {WIDGET_PROP_VALUE, WIDGET_PROP_VERTICAL,
                                            WIDGET_PROP_MIN,   WIDGET_PROP_MAX,
                                            WIDGET_PROP_STEP,  NULL};
TK_DECL_VTABLE(slider) = {.size = sizeof(slider_t),
                          .type = WIDGET_TYPE_SLIDER,
                          .inputable = TRUE,
                          .clone_properties = s_slider_properties,
                          .persistent_properties = s_slider_properties,
                          .parent = TK_PARENT_VTABLE(widget),
                          .create = slider_create,
                          .on_event = slider_on_event,
                          .on_paint_self = slider_on_paint_self,
                          .on_paint_border = widget_on_paint_null,
                          .on_paint_background = widget_on_paint_null,
                          .get_prop = slider_get_prop,
                          .set_prop = slider_set_prop};

widget_t* slider_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(slider), x, y, w, h);
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL, NULL);

  slider->min = 0;
  slider->max = 100;
  slider->step = 1;
  slider->value = 0;

  return widget;
}

widget_t* slider_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, slider), NULL);

  return widget;
}
