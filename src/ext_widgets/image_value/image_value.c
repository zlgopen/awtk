/**
 * File:   image_value.h
 * Author: AWTK Develop Team
 * Brief:  image_value
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
 * 2018-12-01 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "image_value/image_value.h"

static ret_t image_value_draw_images(widget_t* widget, canvas_t* c, bitmap_t* bitmap, uint32_t nr) {
  int32_t x = 0;
  int32_t y = 0;
  uint32_t i = 0;
  uint32_t w = 0;
  uint32_t h = 0;
  float_t ratio = c->lcd->ratio;

  for (i = 0; i < nr; i++) {
    bitmap_t* b = bitmap + i;
    w += b->w;
    if (h < b->h) {
      h = b->h;
    }
  }

  w = w / ratio;
  h = h / ratio;
  x = (widget->w - w) >> 1;
  y = (widget->h - h) >> 1;

  for (i = 0; i < nr; i++) {
    bitmap_t* b = bitmap + i;
    rect_t s = rect_init(0, 0, b->w, b->h);
    rect_t d = rect_init(x, y, b->w / ratio, b->h / ratio);

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
  if (strchr(format, 'd') != NULL) {
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
    value_set_float(v, image_value->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    value_set_str(v, image_value->image);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    value_set_str(v, image_value->format);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t image_value_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return image_value_set_value(widget, value_float(v));
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return image_value_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_FORMAT)) {
    return image_value_set_format(widget, value_str(v));
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

TK_DECL_VTABLE(image_value) = {.size = sizeof(image_value_t),
                               .type = WIDGET_TYPE_IMAGE_VALUE,
                               .parent = TK_PARENT_VTABLE(widget),
                               .create = image_value_create,
                               .on_destroy = image_value_on_destroy,
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

ret_t image_value_set_value(widget_t* widget, float_t value) {
  image_value_t* image_value = IMAGE_VALUE(widget);
  return_value_if_fail(image_value != NULL, RET_BAD_PARAMS);

  if (image_value->value != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);

    widget_dispatch(widget, &e);
    image_value->value = value;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

widget_t* image_value_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, image_value), NULL);

  return widget;
}
