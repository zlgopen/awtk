/**
 * File:   guage.h
 * Author: AWTK Develop Team
 * Brief:  guage
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
 * 2018-09-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/matrix.h"
#include "guage/guage.h"
#include "base/image_manager.h"

ret_t guage_set_angle(widget_t* widget, int32_t angle) {
  guage_t* guage = GUAGE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  guage->angle = angle;

  return RET_OK;
}

ret_t guage_set_pointer_image(widget_t* widget, const char* pointer_image) {
  guage_t* guage = GUAGE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(guage->pointer_image);
  guage->pointer_image = tk_strdup(pointer_image);

  return RET_OK;
}

ret_t guage_set_bg_image(widget_t* widget, const char* bg_image) {
  guage_t* guage = GUAGE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(guage->bg_image);
  guage->bg_image = tk_strdup(bg_image);

  return RET_OK;
}

static ret_t guage_get_prop(widget_t* widget, const char* name, value_t* v) {
  guage_t* guage = GUAGE(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, GUAGE_PROP_ANGLE) || tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_int(v, guage->angle);
    return RET_OK;
  } else if (tk_str_eq(name, GUAGE_PROP_POINTER_IMAGE)) {
    value_set_str(v, guage->pointer_image);
    return RET_OK;
  } else if (tk_str_eq(name, GUAGE_PROP_BG_IMAGE)) {
    value_set_str(v, guage->bg_image);
  }

  return RET_NOT_FOUND;
}

static ret_t guage_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, GUAGE_PROP_ANGLE) || tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return guage_set_angle(widget, value_int(v));
  } else if (tk_str_eq(name, GUAGE_PROP_POINTER_IMAGE)) {
    return guage_set_pointer_image(widget, value_str(v));
  } else if (tk_str_eq(name, GUAGE_PROP_BG_IMAGE)) {
    return guage_set_bg_image(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t guage_destroy(widget_t* widget) {
  guage_t* guage = GUAGE(widget);

  TKMEM_FREE(guage->bg_image);
  TKMEM_FREE(guage->pointer_image);

  return RET_OK;
}

static ret_t guage_load_image(const char* name, bitmap_t* bitmap) {
  if (name != NULL && bitmap != NULL) {
    return image_manager_load(image_manager(), name, bitmap);
  }

  return RET_FAIL;
}

static ret_t guage_on_paint_self(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  float_t rotation = 0;
  float_t anchor_x = 0;
  float_t anchor_y = 0;
  matrix_t matrix;
  matrix_t* m = matrix_init(&matrix);
  guage_t* guage = GUAGE(widget);
  rect_t dst = rect_init(0, 0, widget->w, widget->h);

  if (guage_load_image(guage->bg_image, &bitmap) == RET_OK) {
    canvas_draw_image_ex(c, &bitmap, IMAGE_DRAW_CENTER, &dst);
  }

  if (guage_load_image(guage->pointer_image, &bitmap) == RET_OK) {
    float_t dx = (dst.w - bitmap.w) / 2.0f;
    float_t dy = (dst.h - bitmap.h) / 2.0f;

    rotation = (2 * M_PI * guage->angle) / 360;
    anchor_x = bitmap.w / 2.0f;
    anchor_y = bitmap.h / 2.0f;

    matrix_identity(m);
    matrix_translate(m, c->ox + dx, c->oy + dy);
    matrix_translate(m, anchor_x, anchor_y);
    matrix_rotate(m, rotation);
    matrix_translate(m, -anchor_x, -anchor_y);

    canvas_draw_image_matrix(c, &bitmap, &matrix);
  }

  return RET_OK;
}

static const char* s_guage_properties[] = {GUAGE_PROP_ANGLE, GUAGE_PROP_POINTER_IMAGE,
                                           GUAGE_PROP_BG_IMAGE, NULL};

static const widget_vtable_t s_guage_vtable = {.size = sizeof(guage_t),
                                               .type = WIDGET_TYPE_GUAGE,
                                               .clone_properties = s_guage_properties,
                                               .persistent_properties = s_guage_properties,
                                               .create = guage_create,
                                               .on_paint_self = guage_on_paint_self,
                                               .set_prop = guage_set_prop,
                                               .get_prop = guage_get_prop,
                                               .destroy = guage_destroy};

widget_t* guage_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  guage_t* guage = TKMEM_ZALLOC(guage_t);
  widget_t* widget = WIDGET(guage);
  return_value_if_fail(guage != NULL, NULL);

  return widget_init(widget, parent, &s_guage_vtable, x, y, w, h);
}

widget_t* guage_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_guage_vtable, NULL);

  return widget;
}
