/**
 * File:   guage_pointer.h
 * Author: AWTK Develop Team
 * Brief:  guage_pointer
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
#include "guage/guage_pointer.h"
#include "base/image_manager.h"

ret_t guage_pointer_set_angle(widget_t* widget, int32_t angle) {
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (guage_pointer->angle != angle) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    guage_pointer->angle = angle;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget->parent, NULL);
  }

  return RET_OK;
}

ret_t guage_pointer_set_image(widget_t* widget, const char* image) {
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  guage_pointer->image = tk_str_copy(guage_pointer->image, image);

  return RET_OK;
}

static ret_t guage_pointer_get_prop(widget_t* widget, const char* name, value_t* v) {
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, GUAGE_POINTER_PROP_ANGLE) || tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_int(v, guage_pointer->angle);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    value_set_str(v, guage_pointer->image);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t guage_pointer_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, GUAGE_POINTER_PROP_ANGLE) || tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return guage_pointer_set_angle(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return guage_pointer_set_image(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t guage_pointer_destroy(widget_t* widget) {
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);

  TKMEM_FREE(guage_pointer->image);

  return RET_OK;
}

static ret_t guage_pointer_on_paint_self(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  float_t rotation = 0;
  float_t anchor_x = 0;
  float_t anchor_y = 0;
  matrix_t matrix;
  matrix_t* m = matrix_init(&matrix);
  guage_pointer_t* guage_pointer = GUAGE_POINTER(widget);
  rect_t dst = rect_init(0, 0, widget->w, widget->h);
  return_value_if_fail(guage_pointer->image != NULL, RET_BAD_PARAMS);

  if (widget_load_image(widget, guage_pointer->image, &bitmap) == RET_OK) {
    anchor_x = dst.w * 0.5f;
    anchor_y = dst.h * 0.5f;
    rotation = TK_D2R(guage_pointer->angle);

    matrix_identity(m);
    matrix_translate(m, c->ox, c->oy);
    matrix_translate(m, anchor_x, anchor_y);
    matrix_rotate(m, rotation);
    matrix_translate(m, -anchor_x, -anchor_y);

    canvas_draw_image_matrix(c, &bitmap, &matrix);
  }

  return RET_OK;
}

static const char* s_guage_pointer_properties[] = {GUAGE_POINTER_PROP_ANGLE, WIDGET_PROP_IMAGE,
                                                   NULL};

static const widget_vtable_t s_guage_pointer_vtable = {
    .size = sizeof(guage_pointer_t),
    .type = WIDGET_TYPE_GUAGE_POINTER,
    .clone_properties = s_guage_pointer_properties,
    .persistent_properties = s_guage_pointer_properties,
    .create = guage_pointer_create,
    .on_paint_self = guage_pointer_on_paint_self,
    .set_prop = guage_pointer_set_prop,
    .get_prop = guage_pointer_get_prop,
    .destroy = guage_pointer_destroy};

widget_t* guage_pointer_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  guage_pointer_t* guage_pointer = TKMEM_ZALLOC(guage_pointer_t);
  widget_t* widget = WIDGET(guage_pointer);
  return_value_if_fail(guage_pointer != NULL, NULL);

  return widget_init(widget, parent, &s_guage_pointer_vtable, x, y, w, h);
}

widget_t* guage_pointer_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_guage_pointer_vtable, NULL);

  return widget;
}
