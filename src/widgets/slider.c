/**
 * File:   slider.c
 * Author: AWTK Develop Team
 * Brief:  slider
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
 * History:aod
 * 2018-04-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/rect.h"
#include "tkc/utils.h"
#include "tkc/time_now.h"
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

static uint32_t slider_get_bar_size(widget_t* widget) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL, 0);

  if (slider->vertical) {
    return slider->bar_size ? slider->bar_size : (widget->w >> 1);
  } else {
    return slider->bar_size ? slider->bar_size : (widget->h >> 1);
  }
}

static ret_t slider_update_dragger_rect(widget_t* widget, canvas_t* c) {
  bitmap_t img;
  double fvalue = 0;
  slider_t* slider = SLIDER(widget);
  rect_t* r = NULL;
  return_value_if_fail(slider != NULL, RET_BAD_PARAMS);

  r = &(slider->dragger_rect);
  fvalue = (double)(slider->value - slider->min) / (double)(slider->max - slider->min);

  if (slider->dragger_adapt_to_icon && c != NULL) {
    float_t ratio = c->lcd->ratio;
    if (slider_load_icon(widget, &img) == RET_OK) {
      slider->dragger_size = slider->vertical ? (img.h / ratio) : (img.w / ratio);
    }
  }

  if (slider->vertical) {
    r->w = widget->w;
    r->h = slider->dragger_size;
    r->x = 0;
    r->y = tk_roundi((widget->h - r->h) * (1 - fvalue));
  } else {
    r->w = slider->dragger_size;
    r->h = widget->h;
    r->x = tk_roundi((widget->w - r->w) * fvalue);
    r->y = 0;
  }

  return RET_OK;
}

static ret_t slider_fill_rect(widget_t* widget, canvas_t* c, rect_t* r, rect_t* br,
                              image_draw_type_t draw_type) {
  bitmap_t img;
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  uint32_t radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);
  const char* color_key = br ? STYLE_ID_FG_COLOR : STYLE_ID_BG_COLOR;
  const char* image_key = br ? STYLE_ID_FG_IMAGE : STYLE_ID_BG_IMAGE;
  const char* draw_type_key = br ? STYLE_ID_FG_IMAGE_DRAW_TYPE : STYLE_ID_BG_IMAGE_DRAW_TYPE;

  color_t color = style_get_color(style, color_key, trans);
  const char* image_name = style_get_str(style, image_key, NULL);

  if (color.rgba.a && r->w > 0 && r->h > 0) {
    canvas_set_fill_color(c, color);
    if (radius > 3) {
      canvas_fill_rounded_rect(c, r, br, &color, radius);
    } else {
      canvas_fill_rect(c, r->x, r->y, r->w, r->h);
    }
  }

  if (image_name != NULL && r->w > 0 && r->h > 0) {
    if (widget_load_image(widget, image_name, &img) == RET_OK) {
      draw_type = (image_draw_type_t)style_get_int(style, draw_type_key, draw_type);
      canvas_draw_image_ex(c, &img, draw_type, r);
    }
  }

  return RET_OK;
}

static ret_t slider_paint_dragger(widget_t* widget, canvas_t* c) {
  bitmap_t img;
  color_t color;
  uint32_t radius;
  style_t* style = widget->astyle;
  slider_t* slider = SLIDER(widget);
  rect_t* r = &(slider->dragger_rect);
  color_t trans = color_init(0, 0, 0, 0);

  color = style_get_color(style, STYLE_ID_BORDER_COLOR, trans);
  radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);

  if (color.rgba.a) {
    canvas_set_fill_color(c, color);
    if (radius > 3) {
      canvas_fill_rounded_rect(c, r, NULL, &color, radius);
    } else {
      canvas_fill_rect(c, r->x, r->y, r->w, r->h);
    }
  }

  if (slider_load_icon(widget, &img) == RET_OK) {
    canvas_draw_image_ex(c, &img, IMAGE_DRAW_ICON, r);
  }

  return RET_OK;
}

static ret_t slider_get_bar_rect(widget_t* widget, rect_t* br, rect_t* fr) {
  slider_t* slider = SLIDER(widget);
  uint32_t bar_size = 0;
  uint32_t radius = 0;
  const char* bg_image = 0;
  rect_t* dr = NULL;

  return_value_if_fail(widget != NULL && slider != NULL && br != NULL && fr != NULL,
                       RET_BAD_PARAMS);

  bar_size = slider_get_bar_size(widget);
  radius = style_get_int(widget->astyle, STYLE_ID_ROUND_RADIUS, 0);
  bg_image = style_get_str(widget->astyle, STYLE_ID_BG_IMAGE, NULL);
  dr = &(slider->dragger_rect);

  if (slider->vertical) {
    bar_size = tk_min(bar_size, widget->w);
    br->w = bar_size;
    br->x = (widget->w - br->w) / 2;
    br->h = (radius || bg_image != NULL) ? (widget->h - dr->h) : dr->y;
    br->y = dr->h / 2;

    fr->w = br->w;
    fr->x = br->x;
    fr->h = widget->h - dr->h - dr->y;
    fr->y = dr->y + dr->h / 2;
  } else {
    bar_size = tk_min(bar_size, widget->h);
    br->w = (radius || bg_image != NULL) ? (widget->w - dr->w) : (widget->w - dr->w - dr->x);
    br->x = (radius || bg_image != NULL) ? (dr->w / 2) : (dr->x + dr->w / 2);
    br->h = bar_size;
    br->y = (widget->h - br->h) / 2;

    fr->w = dr->x;
    fr->x = dr->w / 2;
    fr->h = br->h;
    fr->y = br->y;
  }

  return RET_OK;
}

static ret_t slider_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t br, fr;
  image_draw_type_t draw_type;
  slider_t* slider = SLIDER(widget);

  return_value_if_fail(widget != NULL && slider != NULL, RET_BAD_PARAMS);

  slider_update_dragger_rect(widget, c);

  draw_type = slider->vertical ? IMAGE_DRAW_PATCH3_Y : IMAGE_DRAW_PATCH3_X;

  return_value_if_fail(RET_OK == slider_get_bar_rect(widget, &br, &fr), RET_FAIL);

  slider_fill_rect(widget, c, &br, NULL, draw_type);
  slider_fill_rect(widget, c, &fr, &br, draw_type);
  slider_paint_dragger(widget, c);

  return RET_OK;
}

static ret_t slider_pointer_up_cleanup(widget_t* widget) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL && slider != NULL, RET_BAD_PARAMS);

  slider->pressed = FALSE;
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

static ret_t slider_change_value_by_pointer_event(widget_t* widget, pointer_event_t* evt) {
  double value = 0;
  slider_t* slider = SLIDER(widget);
  double range = slider->max - slider->min;
  point_t p = {evt->x, evt->y};

  widget_to_local(widget, &p);
  if (slider->vertical) {
    value = range - range * p.y / widget->h;
  } else {
    value = range * p.x / widget->w;
  }
  value += slider->min;
  value = tk_clamp(value, slider->min, slider->max);

  return slider_set_value_internal(widget, (double)value, EVT_VALUE_CHANGING, FALSE);
}

static ret_t slider_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint16_t type = e->type;
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(widget != NULL && slider != NULL, RET_BAD_PARAMS);

  ret = slider->dragging ? RET_STOP : RET_OK;
  switch (type) {
    case EVT_POINTER_DOWN: {
      if (!widget_find_animator(widget, WIDGET_PROP_VALUE)) {
        rect_t br, fr;
        pointer_event_t* evt = (pointer_event_t*)e;
        point_t p = {evt->x, evt->y};
        rect_t* dr = &(slider->dragger_rect);

        return_value_if_fail(RET_OK == slider_get_bar_rect(widget, &br, &fr), RET_STOP);

        widget_to_local(widget, &p);
        slider->saved_value = slider->value;
        if (slider->slide_with_bar || rect_contains(dr, p.x, p.y) || rect_contains(&br, p.x, p.y) ||
            rect_contains(&fr, p.x, p.y)) {
          slider_change_value_by_pointer_event(widget, evt);

          slider->down = p;
          slider->pressed = TRUE;
          slider->dragging = TRUE;
          widget_set_state(widget, WIDGET_STATE_PRESSED);
          widget_grab(widget->parent, widget);
          widget_invalidate(widget, NULL);
        }
      }
      ret = slider->dragging ? RET_STOP : RET_OK;
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      slider_pointer_up_cleanup(widget);
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (slider->dragging) {
        slider_change_value_by_pointer_event(widget, evt);
      }

      break;
    }
    case EVT_POINTER_UP: {
      if (slider->dragging || slider->pressed) {
        double value = 0;
        pointer_event_t* evt = (pointer_event_t*)e;

        slider_change_value_by_pointer_event(widget, evt);

        value = slider->value;
        slider->dragging = FALSE;
        slider->value = slider->saved_value;
        slider_set_value(widget, value);
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
      bool_t inc = FALSE;
      bool_t dec = FALSE;
      key_event_t* evt = (key_event_t*)e;
      keyboard_type_t keyboard_type = system_info()->keyboard_type;

      if (slider->vertical || keyboard_type == KEYBOARD_3KEYS) {
        if (evt->key == TK_KEY_UP) {
          inc = TRUE;
        } else if (evt->key == TK_KEY_DOWN) {
          dec = TRUE;
        }
      }

      if (!slider->vertical || keyboard_type == KEYBOARD_3KEYS) {
        if (evt->key == TK_KEY_RIGHT) {
          inc = TRUE;
        } else if (evt->key == TK_KEY_LEFT) {
          dec = TRUE;
        }
      }

      if (dec) {
        slider_dec(widget);
        ret = RET_STOP;
      } else if (inc) {
        slider_inc(widget);
        ret = RET_STOP;
      }
      break;
    }
    case EVT_KEY_UP: {
      key_event_t* evt = (key_event_t*)e;
      if (slider->vertical) {
        if (evt->key == TK_KEY_UP) {
          ret = RET_STOP;
        } else if (evt->key == TK_KEY_DOWN) {
          ret = RET_STOP;
        }
      } else {
        if (evt->key == TK_KEY_LEFT) {
          ret = RET_STOP;
        } else if (evt->key == TK_KEY_RIGHT) {
          ret = RET_STOP;
        }
      }
      break;
    }
    case EVT_RESIZE:
    case EVT_MOVE_RESIZE: {
      slider_update_dragger_rect(widget, NULL);
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
  value = tk_clamp(value, slider->min, slider->max);

  if (step > 0) {
    offset = value - slider->min;
    offset = tk_roundi(offset / step) * step;
    value = slider->min + offset;
  }

  if (slider->value != value || force) {
    value_change_event_t evt;
    value_change_event_init(&evt, etype, widget);
    value_set_double(&(evt.old_value), slider->value);
    value_set_double(&(evt.new_value), value);
    slider->value = value;
    widget_dispatch(widget, (event_t*)&evt);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t slider_set_value(widget_t* widget, double value) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL, RET_BAD_PARAMS);

  if (slider->dragging) {
    return RET_BUSY;
  }

  if (slider->value != value) {
    value_change_event_t evt;
    value_change_event_init(&evt, EVT_VALUE_WILL_CHANGE, widget);
    value_set_uint32(&(evt.old_value), slider->value);
    value_set_uint32(&(evt.new_value), value);
    if (widget_dispatch(widget, (event_t*)&evt) == RET_STOP) {
      return RET_OK;
    }

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
  return_value_if_fail(slider != NULL, RET_BAD_PARAMS);

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
    value_set_uint32(v, slider->bar_size);
    return RET_OK;
  } else if (tk_str_eq(name, SLIDER_PROP_DRAGGER_SIZE)) {
    value_set_uint32(v, slider->dragger_size);
    return RET_OK;
  } else if (tk_str_eq(name, SLIDER_PROP_DRAGGER_ADAPT_TO_ICON)) {
    value_set_bool(v, slider->dragger_adapt_to_icon);
    return RET_OK;
  } else if (tk_str_eq(name, SLIDER_PROP_SLIDE_WITH_BAR)) {
    value_set_bool(v, slider->slide_with_bar);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_INPUTING)) {
    value_set_bool(v, slider->dragging);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t slider_set_prop(widget_t* widget, const char* name, const value_t* v) {
  slider_t* slider = SLIDER(widget);
  return_value_if_fail(slider != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

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
    return slider_set_bar_size(widget, value_uint32(v));
  } else if (tk_str_eq(name, SLIDER_PROP_DRAGGER_SIZE)) {
    slider->dragger_size = value_uint32(v);
    return RET_OK;
  } else if (tk_str_eq(name, SLIDER_PROP_DRAGGER_ADAPT_TO_ICON)) {
    slider->dragger_adapt_to_icon = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, SLIDER_PROP_SLIDE_WITH_BAR)) {
    slider->slide_with_bar = value_bool(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const char* s_slider_properties[] = {WIDGET_PROP_VALUE,
                                            WIDGET_PROP_VERTICAL,
                                            WIDGET_PROP_MIN,
                                            WIDGET_PROP_MAX,
                                            WIDGET_PROP_STEP,
                                            WIDGET_PROP_BAR_SIZE,
                                            SLIDER_PROP_DRAGGER_SIZE,
                                            SLIDER_PROP_DRAGGER_ADAPT_TO_ICON,
                                            SLIDER_PROP_SLIDE_WITH_BAR,
                                            NULL};

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
  slider->dragger_size = 10;
  slider->dragger_adapt_to_icon = TRUE;
  slider->slide_with_bar = FALSE;

  return widget;
}

widget_t* slider_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, slider), NULL);

  return widget;
}
