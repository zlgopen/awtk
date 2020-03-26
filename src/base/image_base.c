/**
 * File:   image_base.h
 * Author: AWTK Develop Team
 * Brief:  image base
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-11-22 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/image_base.h"

ret_t image_base_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(image != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN:
      if (image->clickable) {
        widget_set_state(widget, WIDGET_STATE_PRESSED);
        widget_invalidate(widget, NULL);
      }
      break;
    case EVT_POINTER_UP: {
      if (image->clickable) {
        pointer_event_t evt = *(pointer_event_t*)e;
        evt.e = event_init(EVT_CLICK, widget);
        widget_dispatch(widget, (event_t*)&evt);
      }

      if (image->selectable) {
        image->selected = !image->selected;
      }

      if (image->selected) {
        widget_set_state(widget, WIDGET_STATE_SELECTED);
      } else {
        widget_set_state(widget, WIDGET_STATE_NORMAL);
      }
      widget_invalidate(widget, NULL);
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

ret_t image_base_get_prop(widget_t* widget, const char* name, value_t* v) {
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(image != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    value_set_str(v, image->image);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCALE_X)) {
    value_set_float(v, image->scale_x);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCALE_Y)) {
    value_set_float(v, image->scale_y);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_X)) {
    value_set_float(v, image->anchor_x);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_Y)) {
    value_set_float(v, image->anchor_y);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ROTATION)) {
    value_set_float(v, image->rotation);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTABLE)) {
    value_set_bool(v, image->selectable);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTED)) {
    value_set_bool(v, image->selected);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLICKABLE)) {
    value_set_bool(v, image->clickable);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t image_base_set_prop(widget_t* widget, const char* name, const value_t* v) {
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(image != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_IMAGE)) {
    return image_base_set_image(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_SCALE_X)) {
    image->scale_x = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCALE_Y)) {
    image->scale_y = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_X)) {
    image->anchor_x = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_Y)) {
    image->anchor_y = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ROTATION)) {
    image->rotation = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTABLE)) {
    image->selectable = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SELECTED)) {
    return image_base_set_selected(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_CLICKABLE)) {
    image->clickable = value_bool(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t image_base_on_destroy(widget_t* widget) {
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(image != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(image->image);

  return RET_OK;
}

widget_t* image_base_init(widget_t* widget) {
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(image != NULL, NULL);

  image->scale_x = 1;
  image->scale_y = 1;
  image->rotation = 0;
  image->anchor_x = 0.5;
  image->anchor_y = 0.5;

  return widget;
}

ret_t image_base_set_image(widget_t* widget, const char* name) {
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(widget != NULL && name != NULL, RET_BAD_PARAMS);

  image->image = tk_str_copy(image->image, name);

  return widget_invalidate(widget, NULL);
}

ret_t image_base_set_rotation(widget_t* widget, float_t rotation) {
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->rotation = rotation;

  return widget_invalidate(widget, NULL);
}

ret_t image_base_set_scale(widget_t* widget, float_t scale_x, float_t scale_y) {
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->scale_x = scale_x;
  image->scale_y = scale_y;

  return widget_invalidate(widget, NULL);
}

ret_t image_base_set_anchor(widget_t* widget, float_t anchor_x, float_t anchor_y) {
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->anchor_x = anchor_x;
  image->anchor_y = anchor_y;

  return widget_invalidate(widget, NULL);
}

ret_t image_base_set_selected(widget_t* widget, bool_t selected) {
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->selected = selected;

  if (image->selected) {
    widget_set_state(widget, WIDGET_STATE_SELECTED);
  } else {
    widget_set_state(widget, WIDGET_STATE_NORMAL);
  }

  return widget_invalidate(widget, NULL);
}

ret_t image_base_set_selectable(widget_t* widget, bool_t selectable) {
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->selectable = selectable;

  return widget_invalidate(widget, NULL);
}

ret_t image_base_set_clickable(widget_t* widget, bool_t clickable) {
  image_base_t* image = IMAGE_BASE(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image->clickable = clickable;

  return widget_invalidate(widget, NULL);
}

TK_DECL_VTABLE(image_base) = {.size = sizeof(image_base_t), .parent = TK_PARENT_VTABLE(widget)};

widget_t* image_base_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, image_base), NULL);

  return widget;
}

bool_t image_need_transform(widget_t* widget) {
  image_base_t* image_base = IMAGE_BASE(widget);
  return_value_if_fail(widget != NULL, FALSE);

  return !tk_fequal(image_base->scale_x, 1) || !tk_fequal(image_base->scale_y, 1) ||
         !tk_fequal(image_base->rotation, 0);
}

ret_t image_transform(widget_t* widget, canvas_t* c) {
  float_t anchor_x = 0;
  float_t anchor_y = 0;
  image_base_t* image_base = IMAGE_BASE(widget);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);

  return_value_if_fail(widget != NULL && vg != NULL, RET_BAD_PARAMS);

  anchor_x = image_base->anchor_x * widget->w;
  anchor_y = image_base->anchor_y * widget->h;

  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_translate(vg, anchor_x, anchor_y);
  vgcanvas_rotate(vg, image_base->rotation);
  vgcanvas_scale(vg, image_base->scale_x, image_base->scale_y);
  vgcanvas_translate(vg, -anchor_x, -anchor_y);

  return RET_OK;
}

ret_t image_base_on_copy(widget_t* widget, widget_t* other) {
  image_base_t* image = IMAGE_BASE(widget);
  image_base_t* image_other = IMAGE_BASE(other);

  image->anchor_x = image_other->anchor_x;
  image->anchor_y = image_other->anchor_y;
  image->scale_x = image_other->scale_x;
  image->scale_y = image_other->scale_y;
  image->rotation = image_other->rotation;
  image->clickable = image_other->clickable;
  image->selectable = image_other->selectable;
  image->selected = image_other->selected;
  image->image = tk_str_copy(image->image, image_other->image);

  return RET_OK;
}
