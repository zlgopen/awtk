/**
 * File:   layer_window.h
 * Author: AWTK Develop Team
 * Brief:  layer_window
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
 * 2021-06-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "base/widget_vtable.h"
#include "layer_window/layer_manager.h"
#include "layer_window/layer_window.h"

static ret_t layer_window_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, LAYER_WINDOW_PROP_LAYER_NAME)) {
    return layer_window_set_layer_name(widget, value_str(v));
  }

  return window_base_set_prop(widget, name, v);
}

static ret_t layer_window_get_prop(widget_t* widget, const char* name, value_t* v) {
  layer_window_t* layer_window = LAYER_WINDOW(widget);
  return_value_if_fail(layer_window != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, LAYER_WINDOW_PROP_LAYER_NAME)) {
    value_set_str(v, layer_window->layer_name);
    return RET_OK;
  }

  return window_base_get_prop(widget, name, v);
}

static const char* const s_layer_window_properties[] = {LAYER_WINDOW_PROP_LAYER_NAME, NULL};

static ret_t layer_window_invalidate(widget_t* widget, const rect_t* rect) {
#ifdef WITH_MULTI_LAYERS
  layer_window_t* layer_window = LAYER_WINDOW(widget);
  rect_t r = rect != NULL ? *rect : rect_init(0, 0, widget->w, widget->h);
  layer_t* layer = layer_manager_find(layer_manager(), layer_window->layer_name);
  return_value_if_fail(layer != NULL, RET_BAD_PARAMS);
  
  r.x += widget->x - layer->x;
  r.y += widget->y - layer->y;

  return layer_invalidate(layer, &r);
#else
  return widget_invalidate_default(widget, rect);
#endif /*WITH_MULTI_LAYERS*/
}

static ret_t layer_window_on_event(widget_t* widget, event_t* e) {
#ifdef WITH_MULTI_LAYERS
  layer_window_t* layer_window = LAYER_WINDOW(widget);
  return_value_if_fail(layer_window != NULL, RET_BAD_PARAMS);

  switch (e->type) {
    case EVT_WINDOW_WILL_OPEN: {
      layer_t* layer = layer_manager_find(layer_manager(), layer_window->layer_name);
      assert(layer != NULL);
      widget->visible = FALSE;
      layer_add_layer_window(layer, widget);
      break;
    }
    case EVT_WINDOW_CLOSE: {
      layer_t* layer = layer_manager_find(layer_manager(), layer_window->layer_name);
      assert(layer != NULL);
      layer_remove_layer_window(layer, widget);
      break;
    }
    default:
      break;
  }
#endif /*WITH_MULTI_LAYERS*/

  return window_base_on_event(widget, e);
}

static ret_t layer_window_on_destroy(widget_t* widget) {
  layer_window_t* layer_window = LAYER_WINDOW(widget);
  return_value_if_fail(layer_window != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(layer_window->layer_name);

  return window_base_on_destroy(widget);
}

TK_DECL_VTABLE(layer_window) = {.type = WIDGET_TYPE_LAYER_WINDOW,
                                .size = sizeof(layer_window_t),
                                .is_window = TRUE,
                                .parent = TK_PARENT_VTABLE(window_base),
                                .create = layer_window_create,
                                .clone_properties = s_layer_window_properties,
                                .persistent_properties = s_layer_window_properties,
                                .on_event = layer_window_on_event,
                                .on_paint_self = window_base_on_paint_self,
                                .on_paint_begin = window_base_on_paint_begin,
                                .on_paint_end = window_base_on_paint_end,
                                .set_prop = layer_window_set_prop,
                                .get_prop = layer_window_get_prop,
                                .invalidate = layer_window_invalidate,
                                .on_destroy = layer_window_on_destroy};

widget_t* layer_window_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return window_base_create(parent, TK_REF_VTABLE(layer_window), x, y, w, h);
}

widget_t* layer_window_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, layer_window), NULL);

  return widget;
}

ret_t layer_window_set_layer_name(widget_t* widget, const char* layer_name) {
  layer_window_t* layer_window = LAYER_WINDOW(widget);
  return_value_if_fail(layer_window != NULL, RET_BAD_PARAMS);

  layer_window->layer_name = tk_str_copy(layer_window->layer_name, layer_name);

  return RET_OK;
}
