/**
 * File:   color_component.
 * Author: AWTK Develop Team
 * Brief:  color_component
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
 * 2018-08-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/image_manager.h"
#include "base/pixel_pack_unpack.h"
#include "color_picker/rgb_and_hsv.h"
#include "color_picker/color_component.h"

static ret_t color_component_update_h(widget_t* widget);
static ret_t color_component_update_sv(widget_t* widget);
static ret_t color_component_ensure_image(widget_t* widget);
static ret_t color_component_set_type(widget_t* widget, const char* type);

static ret_t color_component_update_pressed(widget_t* widget, pointer_event_t* e) {
  point_t p = {e->x, e->y};
  event_t evt = event_init(EVT_VALUE_CHANGING, widget);
  color_component_t* color_component = COLOR_COMPONENT(widget);

  widget_to_local(widget, &p);
  color_component->color_x = tk_clampi(p.x, 0, widget->w);
  color_component->color_y = tk_clampi(p.y, 0, widget->h);
  widget_invalidate_force(widget, NULL);
  widget_dispatch(widget, &evt);

  return RET_OK;
}

static ret_t color_component_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  color_component_t* component = COLOR_COMPONENT(widget);
  return_value_if_fail(component != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t* evt = (pointer_event_t*)e;
      color_component_update_pressed(widget, evt);
      widget_grab(widget->parent, widget);
      component->pressed = TRUE;
    } break;
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (component->pressed) {
        color_component_update_pressed(widget, evt);
        return RET_STOP;
      }
      break;
    }
    case EVT_POINTER_UP: {
      widget_ungrab(widget->parent, widget);
      if (component->pressed) {
        event_t changed = event_init(EVT_VALUE_CHANGED, widget);
        widget_dispatch(widget, &changed);
        component->pressed = FALSE;
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t color_component_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t src;
  rect_t dst;
  wh_t w = widget->w;
  wh_t h = widget->h;
  bitmap_t* image = NULL;
  color_component_t* color_component = COLOR_COMPONENT(widget);
  xy_t x = tk_max(0, tk_min(color_component->color_x, (w - 1)));
  xy_t y = tk_max(0, tk_min(color_component->color_y, (h - 1)));

  dst = rect_init(0, 0, w, h);
  if (color_component_ensure_image(widget) == RET_OK) {
    image = (color_component->image);
    src = rect_init(0, 0, image->w, image->h);
    canvas_draw_image(c, image, &src, &dst);
  }

  if (color_component->update == color_component_update_sv) {
    canvas_set_stroke_color(c, color_init(0, 0, 0, 0xff));
    canvas_draw_hline(c, 0, y, w);
    canvas_set_stroke_color(c, color_init(0xd0, 0xd0, 0xd0, 0xff));
    if (y > 0) {
      canvas_draw_hline(c, 0, y - 1, w);
    }
    if ((y + 1) < h) {
      canvas_draw_hline(c, 0, y + 1, w);
    }

    canvas_set_stroke_color(c, color_init(0, 0, 0, 0xff));
    canvas_draw_vline(c, x, 0, h);
    canvas_set_stroke_color(c, color_init(0xd0, 0xd0, 0xd0, 0xff));
    if (x > 0) {
      canvas_draw_vline(c, x - 1, 0, h);
    }
    if ((x + 1) < w) {
      canvas_draw_vline(c, x + 1, 0, h);
    }
  } else {
    canvas_set_stroke_color(c, color_init(0, 0, 0, 0xff));
    canvas_draw_hline(c, 0, y, w);
  }

  return RET_OK;
}

static ret_t color_component_on_destroy(widget_t* widget) {
  color_component_t* color_component = COLOR_COMPONENT(widget);

  bitmap_destroy((color_component->image));

  return RET_OK;
}

TK_DECL_VTABLE(color_component) = {.size = sizeof(color_component_t),
                                   .inputable = TRUE,
                                   .type = WIDGET_TYPE_COLOR_COMPONENT,
                                   .parent = TK_PARENT_VTABLE(widget),
                                   .create = color_component_create,
                                   .on_destroy = color_component_on_destroy,
                                   .on_event = color_component_on_event,
                                   .on_paint_self = color_component_on_paint_self};

static bitmap_t* color_component_create_image(int32_t w, int32_t h) {
#ifdef WITH_BITMAP_BGRA
  bitmap_format_t format = BITMAP_FMT_BGRA8888;
#else
  bitmap_format_t format = BITMAP_FMT_RGBA8888;
#endif /*WITH_BITMAP_BGRA*/

  return bitmap_create_ex(w, h, 0, format);
}

static ret_t color_component_ensure_image(widget_t* widget) {
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(color_component != NULL, RET_BAD_PARAMS);

  if (widget->w < 1 || widget->h < 1) {
    return RET_FAIL;
  }

  if (color_component->image != NULL) {
    if (color_component->image->w != widget->w || color_component->image->h != widget->h) {
      bitmap_destroy(color_component->image);
      color_component->image = NULL;
    }
  }

  if (color_component->image == NULL) {
    color_component->need_update = TRUE;
    color_component->image = color_component_create_image(widget->w, widget->h);
  }

  return_value_if_fail(color_component->image != NULL, RET_FAIL);

  if (color_component->update == NULL) {
    color_component_set_type(widget, widget->name);
  }

  if (color_component->need_update && color_component->update != NULL) {
    bitmap_t* image = color_component->image;

    color_component->update(widget);
    image->flags |= BITMAP_FLAG_CHANGED;
    color_component->need_update = FALSE;
  }

  return RET_OK;
}

static ret_t color_component_update_sv(widget_t* widget) {
  rgba_t rgba;
  float H = 0;
  float S = 0;
  float V = 0;
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = 0;
  uint32_t* dst = NULL;
  bitmap_t* image = NULL;
  uint8_t* image_data = NULL;
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(widget != NULL && color_component != NULL, RET_BAD_PARAMS);

  rgba = color_component->c.rgba;
  image = color_component->image;
  w = image->w;
  h = image->h;
  image_data = bitmap_lock_buffer_for_write(image);
  dst = (uint32_t*)(image_data);

  if (color_component->last_hue != -1) {
    H = color_component->last_hue;
  } else {
    convertRGBtoHSV(rgba.r, rgba.g, rgba.b, &H, &S, &V);
    color_component->last_hue = H;
  }

  for (y = 0; y < h; y++) {
    dst = (uint32_t*)(image_data + y * image->line_length);
    for (x = 0; x < w; x++) {
      V = (float)x / (float)w;
      S = 1 - (float)y / (float)h;
      convertHSVtoRGB(H, S, V, &r, &g, &b);
      *dst++ = rgb_to_image8888(r, g, b);
    }
  }

  bitmap_unlock_buffer(image);

  return RET_OK;
}

static ret_t color_component_update_h(widget_t* widget) {
  float H = 0;
  float S = 1;
  float V = 1;
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  int32_t x = 0;
  int32_t y = 0;
  uint32_t v = 0;
  int32_t w = 0;
  int32_t h = 0;
  uint32_t* dst = NULL;
  bitmap_t* image = NULL;
  uint8_t* image_data = NULL;
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(widget != NULL && color_component != NULL, RET_BAD_PARAMS);

  image = color_component->image;
  w = image->w;
  h = image->h;
  image_data = bitmap_lock_buffer_for_write(image);
  dst = (uint32_t*)(image_data);

  for (y = 0; y < h; y++) {
    dst = (uint32_t*)(image_data + y * image->line_length);
    H = (1 - (float)y / (float)h) * 360;
    convertHSVtoRGB(H, S, V, &r, &g, &b);
    v = rgb_to_image8888(r, g, b);
    for (x = 0; x < w; x++) {
      *dst++ = v;
    }
  }
  bitmap_unlock_buffer(image);

  return RET_OK;
}

widget_t* color_component_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(color_component), x, y, w, h);
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(color_component != NULL, NULL);

  color_component->c = color_init(0xff, 0xff, 0xff, 0xff);
  color_component->last_hue = -1;

  return widget;
}

ret_t color_component_set_color(widget_t* widget, color_t c) {
  float H = 0;
  float S = 0;
  float V = 0;
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(color_component != NULL, RET_BAD_PARAMS);

  color_component->c = c;
  convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &H, &S, &V);
  if (color_component->update == color_component_update_sv) {
    if ((int32_t)H != color_component->last_hue) {
      color_component->need_update = TRUE;
      log_debug("hue changed(%d != %d)\n", color_component->last_hue, (int32_t)H);
      color_component->last_hue = -1;
    }
    color_component->color_x = V * widget->w;
    color_component->color_y = (1 - S) * widget->h;
  } else {
    color_component->color_y = (1 - H / 360.0f) * widget->h;
  }
  widget_invalidate(widget, NULL);

  return RET_OK;
}

ret_t color_component_set_hsv(widget_t* widget, float h, float s, float v) {
  color_t* c;
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(color_component != NULL, RET_BAD_PARAMS);

  c = &(color_component->c);
  convertHSVtoRGB(h, s, v, &(c->rgba.r), &(c->rgba.g), &(c->rgba.b));

  if (color_component->update == color_component_update_sv) {
    if ((int32_t)h != color_component->last_hue) {
      color_component->need_update = TRUE;
      log_debug("hue changed(%d != %d)\n", color_component->last_hue, (int32_t)h);
      color_component->last_hue = h;
    }
    color_component->color_x = v * widget->w;
    color_component->color_y = (1 - s) * widget->h;
  } else {
    color_component->color_y = (1 - h / 360.0f) * widget->h;
  }
  widget_invalidate(widget, NULL);

  return RET_OK;
}

static ret_t color_component_set_type(widget_t* widget, const char* type) {
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(color_component != NULL && type != NULL, RET_BAD_PARAMS);

  color_component->image->name = type;

  if (tk_str_eq(type, COLOR_PICKER_CHILD_SV)) {
    color_component->update = color_component_update_sv;
  } else if (tk_str_eq(type, COLOR_PICKER_CHILD_H)) {
    color_component->update = color_component_update_h;
  } else {
    log_debug("not supported color type\n");
  }

  return RET_OK;
}

float color_component_get_h(widget_t* widget) {
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(widget != NULL && color_component != NULL, 0);

  return 360 - color_component->color_y * 360 / widget->h;
}

float color_component_get_s(widget_t* widget) {
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(widget != NULL && color_component != NULL, 0);

  return 1 - (float)(color_component->color_y) / (float)(widget->h);
}

float color_component_get_v(widget_t* widget) {
  color_component_t* color_component = COLOR_COMPONENT(widget);
  return_value_if_fail(widget != NULL && color_component != NULL, 0);

  return (float)(color_component->color_x) / (float)(widget->w);
}

widget_t* color_component_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, color_component), NULL);

  return widget;
}
