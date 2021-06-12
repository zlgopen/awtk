/**
 * File:   image_value.h
 * Author: AWTK Develop Team
 * Brief:  image_value
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
 * 2018-12-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "image_value/image_value.h"

static ret_t image_value_draw_images(widget_t* widget, canvas_t* c, bitmap_t* bitmap, uint32_t nr) {
  int32_t x = 0;
  int32_t y = 0;
  int32_t i = 0;
  int32_t w = 0;
  int32_t h = 0;
  float scale_w = 1;
  float scale_h = 1;
  float_t ratio = c->lcd->ratio;
  style_t* style = widget->astyle;
  rect_t content_r = widget_get_content_area(widget);
  int32_t align_v = style_get_int(style, STYLE_ID_TEXT_ALIGN_V, ALIGN_V_MIDDLE);
  int32_t align_h = style_get_int(style, STYLE_ID_TEXT_ALIGN_H, ALIGN_H_CENTER);
  image_draw_type_t draw_type =
      (image_draw_type_t)style_get_int(style, STYLE_ID_FG_IMAGE_DRAW_TYPE, IMAGE_DRAW_DEFAULT);

  for (i = 0; i < nr; i++) {
    bitmap_t* b = bitmap + i;
    w += b->w;
    if (h < b->h) {
      h = b->h;
    }
  }

  w = w / ratio;
  h = h / ratio;
  return_value_if_fail(w > 0 && h > 0 && content_r.w > 0 && content_r.h > 0, RET_BAD_PARAMS);

  scale_h = (float)(content_r.h) / (float)h;
  scale_w = (float)(content_r.w) / (float)w;
  switch (draw_type) {
    case IMAGE_DRAW_SCALE_AUTO: {
      scale_w = tk_min(scale_w, scale_h);
      scale_h = scale_w;
      break;
    }
    case IMAGE_DRAW_SCALE_W: {
      scale_h = scale_w;
      break;
    }
    case IMAGE_DRAW_SCALE_H: {
      scale_w = scale_h;
      break;
    }
    case IMAGE_DRAW_SCALE: {
      break;
    }
    default: {
      scale_w = 1;
      scale_h = 1;
      break;
    }
  }

  w = scale_w * w;
  h = scale_h * h;

  switch (align_h) {
    case ALIGN_H_LEFT: {
      x = content_r.x;
      break;
    }
    case ALIGN_H_RIGHT: {
      x = content_r.x + content_r.w - w;
      break;
    }
    default: {
      x = (widget->w - w) / 2;
      break;
    }
  }

  switch (align_v) {
    case ALIGN_V_TOP: {
      y = content_r.y;
      break;
    }
    case ALIGN_V_BOTTOM: {
      y = content_r.y + content_r.h - h;
      break;
    }
    default: {
      y = (widget->h - h) / 2;
      break;
    }
  }

  for (i = 0; i < nr; i++) {
    bitmap_t* b = bitmap + i;
    rect_t s = rect_init(0, 0, b->w, b->h);
    rect_t d = rect_init(x, y, scale_w * b->w / ratio, scale_h * b->h / ratio);

    canvas_draw_image(c, b, &s, &d);

    x += d.w;
  }

  return RET_OK;
}

static ret_t image_value_on_paint_self(widget_t* widget, canvas_t* c) {
  uint32_t i = 0;
  uint32_t nr = 0;
  char sub_name[8];
  const char* format = NULL;
  char name[TK_NAME_LEN + 1];
  char str[IMAGE_VALUE_MAX_CHAR_NR + 1];
  bitmap_t bitmap[IMAGE_VALUE_MAX_CHAR_NR];
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL && widget != NULL, RET_BAD_PARAMS);

  format = image_value->format != NULL ? image_value->format : "%d";
  return_value_if_fail(image_value->image != NULL, RET_BAD_PARAMS);

  memset(bitmap, 0x00, sizeof(bitmap));
  memset(sub_name, 0x00, sizeof(sub_name));
  if (strchr(format, 'd') != NULL || strchr(format, 'x') != NULL || strchr(format, 'X') != NULL) {
    tk_snprintf(str, IMAGE_VALUE_MAX_CHAR_NR, format, tk_roundi(image_value->value));
  } else {
    tk_snprintf(str, IMAGE_VALUE_MAX_CHAR_NR, format, image_value->value);
  }

  nr = strlen(str);
  return_value_if_fail(nr > 0, RET_BAD_PARAMS);

  name[TK_NAME_LEN] = '\0';
  for (i = 0; i < nr; i++) {
    if (str[i] == '.') {
      strcpy(sub_name, IMAGE_VALUE_MAP_DOT);
    } else if (str[i] == '/') {
      strcpy(sub_name, IMAGE_VALUE_MAP_SLASH);
    } else {
      sub_name[0] = str[i];
      sub_name[1] = '\0';
    }

    tk_snprintf(name, TK_NAME_LEN, "%s%s", image_value->image, sub_name);
    return_value_if_fail(widget_load_image(widget, name, bitmap + i) == RET_OK, RET_BAD_PARAMS);
  }

  return image_value_draw_images(widget, c, bitmap, nr);
}

static ret_t image_value_get_prop(widget_t* widget, const char* name, value_t* v) {
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_double(v, image_value->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    value_set_double(v, image_value->min);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    value_set_double(v, image_value->max);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    value_set_str(v, image_value->image);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    value_set_str(v, image_value->format);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLICK_ADD_DELTA)) {
    value_set_double(v, image_value->click_add_delta);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t image_value_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return image_value_set_value(widget, value_double(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MIN)) {
    return image_value_set_min(widget, value_double(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    return image_value_set_max(widget, value_double(v));
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return image_value_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    return image_value_set_format(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_CLICK_ADD_DELTA)) {
    return image_value_set_click_add_delta(widget, value_double(v));
  }

  return RET_NOT_FOUND;
}

static ret_t image_value_on_destroy(widget_t* widget) {
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(image_value->image);
  TKMEM_FREE(image_value->format);

  return RET_OK;
}

ret_t image_value_add_delta(widget_t* widget) {
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL, RET_BAD_PARAMS);

  if (image_value->click_add_delta) {
    double value = image_value->value;
    double min = tk_min(image_value->min, image_value->max);
    double max = tk_max(image_value->min, image_value->max);
    return_value_if_fail(min < max, RET_BAD_PARAMS);

    value = image_value->value + image_value->click_add_delta;
    if (value > max) {
      value = min;
    } else if (value < min) {
      value = max;
    }

    image_value_set_value(widget, value);
  }

  return RET_OK;
}

static ret_t image_value_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL && widget != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN: {
      image_value->pressed = TRUE;
      widget_grab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      image_value->pressed = FALSE;
      widget_ungrab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_UP: {
      if (image_value->pressed) {
        image_value_add_delta(widget);
      }
      image_value->pressed = FALSE;
      widget_ungrab(widget->parent, widget);
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static const char* s_image_value_properties[] = {WIDGET_PROP_VALUE,
                                                 WIDGET_PROP_MIN,
                                                 WIDGET_PROP_MAX,
                                                 WIDGET_PROP_FORMAT,
                                                 WIDGET_PROP_IMAGE,
                                                 WIDGET_PROP_CLICK_ADD_DELTA,
                                                 NULL};

TK_DECL_VTABLE(image_value) = {.size = sizeof(image_value_t),
                               .type = WIDGET_TYPE_IMAGE_VALUE,
                               .parent = TK_PARENT_VTABLE(widget),
                               .inputable = TRUE,
                               .clone_properties = s_image_value_properties,
                               .persistent_properties = s_image_value_properties,
                               .create = image_value_create,
                               .on_destroy = image_value_on_destroy,
                               .on_event = image_value_on_event,
                               .get_prop = image_value_get_prop,
                               .set_prop = image_value_set_prop,
                               .on_paint_self = image_value_on_paint_self};

widget_t* image_value_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(image_value), x, y, w, h);
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL, NULL);

  return widget;
}

ret_t image_value_set_image(widget_t* widget, const char* image) {
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL && image != NULL, RET_BAD_PARAMS);

  image_value->image = tk_str_copy(image_value->image, image);

  return widget_invalidate(widget, NULL);
}

ret_t image_value_set_format(widget_t* widget, const char* format) {
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL && format != NULL, RET_BAD_PARAMS);

  image_value->format = tk_str_copy(image_value->format, format);

  return widget_invalidate(widget, NULL);
}

ret_t image_value_set_value(widget_t* widget, double value) {
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL, RET_BAD_PARAMS);

  if (image_value->value != value) {
    value_change_event_t evt;
    value_change_event_init(&evt, EVT_VALUE_WILL_CHANGE, widget);
    value_set_double(&(evt.old_value), image_value->value);
    value_set_double(&(evt.new_value), value);

    if (widget_dispatch(widget, (event_t*)&evt) != RET_STOP) {
      image_value->value = value;
      evt.e.type = EVT_VALUE_CHANGED;
      widget_dispatch(widget, (event_t*)&evt);
      widget_invalidate(widget, NULL);
    }
  }

  return RET_OK;
}

ret_t image_value_set_click_add_delta(widget_t* widget, double click_add_delta) {
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL, RET_BAD_PARAMS);

  image_value->click_add_delta = click_add_delta;

  return RET_OK;
}

ret_t image_value_set_min(widget_t* widget, double min) {
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL, RET_BAD_PARAMS);

  image_value->min = min;

  return RET_OK;
}

ret_t image_value_set_max(widget_t* widget, double max) {
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL, RET_BAD_PARAMS);

  image_value->max = max;

  return RET_OK;
}

widget_t* image_value_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, image_value), NULL);

  return widget;
}
