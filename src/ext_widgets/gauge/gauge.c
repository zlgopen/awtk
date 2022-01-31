/**
 * File:   gauge.h
 * Author: AWTK Develop Team
 * Brief:  gauge
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "base/enums.h"
#include "tkc/utils.h"
#include "gauge/gauge.h"
#include "base/image_manager.h"

static ret_t gauge_get_prop(widget_t* widget, const char* name, value_t* v) {
  gauge_t* gauge = GAUGE(widget);
  return_value_if_fail(gauge != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    value_set_str(v, gauge->image);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DRAW_TYPE)) {
    value_set_int(v, gauge->draw_type);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t gauge_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return gauge_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_DRAW_TYPE)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = image_draw_type_find(value_str(v));
      if (kv != NULL) {
        return gauge_set_draw_type(widget, (image_draw_type_t)(kv->value));
      }
    } else {
      return gauge_set_draw_type(widget, (image_draw_type_t)value_int(v));
    }

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t gauge_on_destroy(widget_t* widget) {
  gauge_t* gauge = GAUGE(widget);
  return_value_if_fail(gauge != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(gauge->image);

  return RET_OK;
}

ret_t gauge_set_image(widget_t* widget, const char* name) {
  gauge_t* gauge = GAUGE(widget);
  return_value_if_fail(gauge != NULL && name != NULL, RET_BAD_PARAMS);

  gauge->image = tk_str_copy(gauge->image, name);

  return RET_OK;
}

ret_t gauge_set_draw_type(widget_t* widget, image_draw_type_t draw_type) {
  gauge_t* gauge = GAUGE(widget);
  return_value_if_fail(gauge != NULL, RET_BAD_PARAMS);

  gauge->draw_type = draw_type;

  return RET_OK;
}

static const char* s_gauge_properties[] = {WIDGET_PROP_DRAW_TYPE, WIDGET_PROP_IMAGE, NULL};

static ret_t gauge_on_paint_self(widget_t* widget, canvas_t* c) {
  bitmap_t bitmap;
  gauge_t* gauge = GAUGE(widget);
  return_value_if_fail(widget != NULL && gauge != NULL && gauge->image != NULL, RET_BAD_PARAMS);

  if (widget_load_image(widget, gauge->image, &bitmap) == RET_OK) {
    rect_t dst = rect_init(0, 0, widget->w, widget->h);
    canvas_draw_image_ex(c, &bitmap, gauge->draw_type, &dst);
  }

  return RET_OK;
}

TK_DECL_VTABLE(gauge) = {.size = sizeof(gauge_t),
                         .type = WIDGET_TYPE_GAUGE,
                         .clone_properties = s_gauge_properties,
                         .persistent_properties = s_gauge_properties,
                         .parent = TK_PARENT_VTABLE(widget),
                         .create = gauge_create,
                         .on_paint_self = gauge_on_paint_self,
                         .set_prop = gauge_set_prop,
                         .get_prop = gauge_get_prop,
                         .on_destroy = gauge_on_destroy};

widget_t* gauge_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(gauge), x, y, w, h);
  gauge_t* gauge = GAUGE(widget);

  return_value_if_fail(gauge != NULL, NULL);

  gauge->draw_type = IMAGE_DRAW_CENTER;

  return widget;
}

widget_t* gauge_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, gauge), NULL);

  return widget;
}
