/**
 * File:   layer.c
 * Author: AWTK Develop Team
 * Brief:  LCD layer。
 *
 * Copyright (c) Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/str.h"
#include "tkc/utils.h"
#include "tkc/time_now.h"
#include "base/dirty_rects.inc"

#include "layer_window/layer.h"

layer_t* layer_cast(object_t* obj);
#define LAYER(obj) layer_cast(OBJECT(obj))

ret_t layer_set_name(layer_t* layer, const char* name) {
  return_value_if_fail(layer != NULL, RET_BAD_PARAMS);

  layer->name = tk_str_copy(layer->name, name);

  return RET_OK;
}

ret_t layer_set_x(layer_t* layer, uint32_t x) {
  return_value_if_fail(layer != NULL, RET_BAD_PARAMS);

  layer->x = x;

  return RET_OK;
}

ret_t layer_set_y(layer_t* layer, uint32_t y) {
  return_value_if_fail(layer != NULL, RET_BAD_PARAMS);

  layer->y = y;

  return RET_OK;
}

ret_t layer_set_max_fps(layer_t* layer, uint32_t max_fps) {
  return_value_if_fail(layer != NULL, RET_BAD_PARAMS);

  layer->max_fps = max_fps;

  return RET_OK;
}

ret_t layer_set_prop(layer_t* layer, const char* name, const value_t* v) {
  return_value_if_fail(layer != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(LAYER_PROP_NAME, name)) {
    layer_set_name(layer, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(LAYER_PROP_X, name)) {
    layer_set_x(layer, value_uint32(v));
    return RET_OK;
  } else if (tk_str_eq(LAYER_PROP_Y, name)) {
    layer_set_y(layer, value_uint32(v));
    return RET_OK;
  } else if (tk_str_eq(LAYER_PROP_MAX_FPS, name)) {
    layer_set_max_fps(layer, value_uint32(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t layer_set_prop_obj(object_t* obj, const char* name, const value_t* v) {
  layer_t* layer = LAYER(obj);
  return_value_if_fail(layer != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return layer_set_prop(layer, name, v);
}

ret_t layer_get_prop(layer_t* layer, const char* name, value_t* v) {
  return_value_if_fail(layer != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(LAYER_PROP_NAME, name)) {
    value_set_str(v, layer->name);
    return RET_OK;
  } else if (tk_str_eq(LAYER_PROP_X, name)) {
    value_set_uint32(v, layer->x);
    return RET_OK;
  } else if (tk_str_eq(LAYER_PROP_Y, name)) {
    value_set_uint32(v, layer->y);
    return RET_OK;
  } else if (tk_str_eq(LAYER_PROP_W, name)) {
    value_set_uint32(v, canvas_get_width(layer->canvas));
    return RET_OK;
  } else if (tk_str_eq(LAYER_PROP_H, name)) {
    value_set_uint32(v, canvas_get_height(layer->canvas));
    return RET_OK;
  } else if (tk_str_eq(LAYER_PROP_MAX_FPS, name)) {
    value_set_uint32(v, layer->max_fps);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t layer_get_prop_obj(object_t* obj, const char* name, value_t* v) {
  layer_t* layer = LAYER(obj);
  return_value_if_fail(layer != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return layer_get_prop(layer, name, v);
}

static ret_t layer_deinit(layer_t* layer) {
  return_value_if_fail(layer != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(layer->name);
  darray_deinit(&(layer->widgets));
  dirty_rects_deinit(&(layer->dirty_rects));

  return RET_OK;
}

static ret_t layer_on_destroy(object_t* obj) {
  return layer_deinit(LAYER(obj));
}

static const object_vtable_t s_layer_vtable = {.type = "layer",
                                               .desc = "layer",
                                               .size = sizeof(layer_t),
                                               .is_collection = FALSE,
                                               .on_destroy = layer_on_destroy,
                                               .get_prop = layer_get_prop_obj,
                                               .set_prop = layer_set_prop_obj};

static layer_t* layer_init(layer_t* layer, const char* name, canvas_t* canvas, uint32_t x, uint32_t y,
                           uint32_t max_fps) {
  return_value_if_fail(layer != NULL, NULL);

  layer->x = x;
  layer->y = y;
  layer->canvas = canvas;
  layer->max_fps = max_fps;
  dirty_rects_init(&(layer->dirty_rects));
  darray_init(&(layer->widgets), 0, NULL, NULL);
  layer->name = tk_str_copy(layer->name, name);

  return layer;
}

layer_t* layer_create(const char* name, canvas_t* canvas, uint32_t x, uint32_t y, uint32_t max_fps) {
  object_t* o = object_create(&s_layer_vtable);
  return_value_if_fail(o != NULL, NULL);

  return layer_init(LAYER(o), name, canvas, x, y, max_fps);
}

layer_t* layer_cast(object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_layer_vtable, NULL);

  return (layer_t*)(obj);
}

static ret_t layer_on_widget_destroy(void* ctx, event_t* e) {
  layer_t* layer = LAYER(ctx);
  widget_t* widget = WIDGET(e->target);

  darray_remove_all(&(layer->widgets), NULL, widget);

  return RET_REMOVE;
}

ret_t layer_remove_widget(layer_t* layer, widget_t* widget) {
  return_value_if_fail(layer != NULL && widget != NULL, RET_BAD_PARAMS);

  widget_off_by_ctx(widget, layer);
  darray_remove_all(&(layer->widgets), NULL, widget);
  
  return RET_OK;
}

ret_t layer_add_widget(layer_t* layer, widget_t* widget) {
  return_value_if_fail(layer != NULL && widget != NULL, RET_BAD_PARAMS);
  layer_remove_widget(layer, widget);
  widget_on(widget, EVT_DESTROY, layer_on_widget_destroy, layer);

  return darray_push(&(layer->widgets), widget);
}

ret_t layer_paint_widget(layer_t* layer, widget_t* widget, canvas_t* c) {
  uint32_t ox = 0;
  uint32_t oy = 0;
  point_t p = {0, 0};
  dirty_rects_t* dirty_rects = &(layer->dirty_rects);

  widget_to_screen(widget, &p);
  ox = -layer->x;
  oy = -layer->y;
 
  canvas_translate(c, ox, oy);
  dirty_rects_paint(dirty_rects, widget, c, widget_paint);
  canvas_untranslate(c, ox, oy);

  return RET_OK;
}

ret_t layer_paint(layer_t* layer) {
  uint32_t i = 0;
  canvas_t* c = layer->canvas;
  darray_t* arr = &(layer->widgets);
  rect_t r = layer->dirty_rects.max;
  bool_t dirty = r.w > 0 && r.h > 0;

  if(arr->size > 0 && dirty) {
    log_debug("layer_paint: %d %d %d %d\n", r.x, r.y, r.w, r.h);
    canvas_begin_frame(c, &r, LCD_DRAW_NORMAL); 
    for(i = 0; i < arr->size; i++) {
      widget_t* iter = WIDGET(arr->elms[i]);

      iter->visible = TRUE;
      layer_paint_widget(layer, iter, c);
      iter->visible = FALSE;
    }

    canvas_end_frame(c);
  }
  dirty_rects_reset(&(layer->dirty_rects));

  return RET_OK;
}

ret_t layer_invalidate(layer_t* layer, const rect_t* rect) {
  return_value_if_fail(layer != NULL && rect != NULL, RET_BAD_PARAMS);

  log_debug("layer_invalidate: %d %d %d %d\n", rect->x, rect->y, rect->w, rect->h);
  return dirty_rects_add(&(layer->dirty_rects), rect);
}

static ret_t layer_on_timer(const timer_info_t* info) {
  layer_paint(LAYER(info->ctx));

  return RET_REPEAT;
}

ret_t layer_start(layer_t* layer) {
  uint32_t duration = 16;
  return_value_if_fail(layer != NULL, RET_BAD_PARAMS);
  return_value_if_fail(layer->max_fps > 0, RET_BAD_PARAMS);
  return_value_if_fail(layer->timer_id == TK_INVALID_ID, RET_BAD_PARAMS);

  duration = 1000/layer->max_fps;
  layer->timer_id = timer_add(layer_on_timer, layer, duration);

  log_debug("layer %s started duration=%d\n", layer->name, duration);

  return RET_OK;
}

