/**
 * File:   color_picker.
 * Author: AWTK Develop Team
 * Brief:  color_picker
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
#include "widgets/edit.h"
#include "widgets/slider.h"
#include "widgets/color_tile.h"
#include "tkc/color_parser.h"
#include "color_picker/rgb_and_hsv.h"
#include "color_picker/color_picker.h"
#include "color_picker/color_component.h"

static ret_t color_picker_on_paint_begin(widget_t* widget, canvas_t* c);

static ret_t color_picker_get_prop(widget_t* widget, const char* name, value_t* v) {
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(color_picker != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_str(v, color_picker->value);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t color_picker_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return color_picker_set_color(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

TK_DECL_VTABLE(color_picker) = {.size = sizeof(color_picker_t),
                                .inputable = TRUE,
                                .type = WIDGET_TYPE_COLOR_PICKER,
                                .set_prop = color_picker_set_prop,
                                .get_prop = color_picker_get_prop,
                                .parent = TK_PARENT_VTABLE(widget),
                                .on_paint_begin = color_picker_on_paint_begin,
                                .create = color_picker_create};

static ret_t color_picker_update_child(void* ctx, const void* iter) {
  color_t c;
  float h = 0;
  float s = 0;
  float v = 0;
  int32_t value = 0;
  widget_t* child = NULL;
  const char* name = NULL;
  const char* type = NULL;
  color_picker_t* color_picker = COLOR_PICKER(ctx);
  return_value_if_fail(ctx != NULL && color_picker != NULL && iter != NULL, RET_REMOVE);

  child = WIDGET(iter);
  return_value_if_fail(child != NULL, RET_REMOVE);

  name = child->name;
  type = child->vt->type;
  c = color_picker->c;

  if (color_picker->trigger_child == child) {
    return RET_OK;
  }

  if (child->emitter) {
    emitter_disable(child->emitter);
  }

  if (tk_str_eq(type, WIDGET_TYPE_EDIT) || tk_str_eq(type, WIDGET_TYPE_SPIN_BOX)) {
    if (tk_str_eq(name, COLOR_PICKER_CHILD_R)) {
      value = c.rgba.r;
      edit_set_int(child, value);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_G)) {
      value = c.rgba.g;
      edit_set_int(child, value);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_B)) {
      value = c.rgba.b;
      edit_set_int(child, value);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_H)) {
      value = color_picker->last_hue;
      edit_set_int(child, value);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_S)) {
      convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &h, &s, &v);
      value = s * 100;
      edit_set_int(child, value);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_V)) {
      convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &h, &s, &v);
      value = v * 100;
      edit_set_int(child, value);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_NEW)) {
      char hex[TK_COLOR_HEX_LEN + 1];
      color_hex_str(color_picker->c, hex);
      widget_set_text_utf8(child, hex + 1);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_OLD)) {
      char hex[TK_COLOR_HEX_LEN + 1];
      color_hex_str(color_picker->init_c, hex);
      widget_set_text_utf8(child, hex + 1);
    }
  } else if (tk_str_eq(type, WIDGET_TYPE_SLIDER)) {
    if (tk_str_eq(name, COLOR_PICKER_CHILD_R)) {
      value = c.rgba.r * 100 / 255;
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_G)) {
      value = c.rgba.g * 100 / 255;
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_B)) {
      value = c.rgba.b * 100 / 255;
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_H)) {
      value = color_picker->last_hue * 100 / 360;
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_S)) {
      convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &h, &s, &v);
      value = s * 100;
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_V)) {
      convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &h, &s, &v);
      value = v * 100;
    }
    widget_set_value(child, value);
  } else if (tk_str_eq(type, WIDGET_TYPE_COLOR_COMPONENT)) {
    if (tk_str_eq(name, COLOR_PICKER_CHILD_SV)) {
      convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &h, &s, &v);
      color_component_set_hsv(child, color_picker->last_hue, s, v);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_H)) {
      convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &h, &s, &v);
      color_component_set_hsv(child, color_picker->last_hue, s, v);
    }
  } else if (tk_str_eq(type, WIDGET_TYPE_COLOR_TILE)) {
    if (tk_str_eq(name, COLOR_PICKER_CHILD_NEW)) {
      color_tile_set_value(child, c);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_OLD)) {
      color_tile_set_value(child, color_picker->init_c);
    }
  }

  if (child->emitter) {
    emitter_enable(child->emitter);
  }

  return RET_OK;
}

static ret_t color_picker_sync_children(widget_t* widget) {
  widget_foreach(widget, color_picker_update_child, widget);

  return RET_OK;
}

static ret_t color_picker_update_color(widget_t* widget, color_t color);

static ret_t color_picker_set_color_r(widget_t* widget, uint8_t r) {
  color_t c;
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(widget != NULL && color_picker != NULL, RET_BAD_PARAMS);

  c = color_picker->c;
  c.rgba.r = r;

  return color_picker_update_color(widget, c);
}

static ret_t color_picker_set_color_g(widget_t* widget, uint8_t g) {
  color_t c;
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(widget != NULL && color_picker != NULL, RET_BAD_PARAMS);

  c = color_picker->c;
  c.rgba.g = g;

  return color_picker_update_color(widget, c);
}

static ret_t color_picker_set_color_b(widget_t* widget, uint8_t b) {
  color_t c;
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(widget != NULL && color_picker != NULL, RET_BAD_PARAMS);

  c = color_picker->c;
  c.rgba.b = b;

  return color_picker_update_color(widget, c);
}

static ret_t color_picker_set_color_h(widget_t* widget, float hh) {
  color_t c;
  float h = 0;
  float s = 0;
  float v = 0;
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(widget != NULL && color_picker != NULL, RET_BAD_PARAMS);

  color_picker->last_hue = hh;
  c = color_picker->c;
  convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &h, &s, &v);
  convertHSVtoRGB(hh, s, v, &(c.rgba.r), &(c.rgba.g), &(c.rgba.b));

  return color_picker_update_color(widget, c);
}

static ret_t color_picker_set_color_s(widget_t* widget, float ss) {
  color_t c;
  float h = 0;
  float s = 0;
  float v = 0;
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(widget != NULL && color_picker != NULL, RET_BAD_PARAMS);

  c = color_picker->c;
  convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &h, &s, &v);
  convertHSVtoRGB(h, ss, v, &(c.rgba.r), &(c.rgba.g), &(c.rgba.b));

  return color_picker_update_color(widget, c);
}

static ret_t color_picker_set_color_v(widget_t* widget, float vv) {
  color_t c;
  float h = 0;
  float s = 0;
  float v = 0;
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(widget != NULL && color_picker != NULL, RET_BAD_PARAMS);

  c = color_picker->c;
  convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &h, &s, &v);
  convertHSVtoRGB(h, s, vv, &(c.rgba.r), &(c.rgba.g), &(c.rgba.b));

  return color_picker_update_color(widget, c);
}

static ret_t color_picker_set_color_sv(widget_t* widget, float ss, float vv) {
  color_t c;
  float h = 0;
  float s = 0;
  float v = 0;
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(widget != NULL && color_picker != NULL, RET_BAD_PARAMS);

  c = color_picker->c;
  convertRGBtoHSV(c.rgba.r, c.rgba.g, c.rgba.b, &h, &s, &v);
  if (color_picker->last_hue != -1) h = color_picker->last_hue;
  convertHSVtoRGB(h, ss, vv, &(c.rgba.r), &(c.rgba.g), &(c.rgba.b));

  return color_picker_update_color(widget, c);
}

static ret_t color_picker_on_child_value_changing(void* ctx, event_t* e) {
  widget_t* widget = NULL;
  widget_t* child = NULL;
  const char* name = NULL;
  const char* type = NULL;
  color_picker_t* color_picker = COLOR_PICKER(ctx);
  return_value_if_fail(e != NULL && color_picker != NULL, RET_BAD_PARAMS);

  color_picker->last_hue = -1;
  widget = WIDGET(ctx);
  child = WIDGET(e->target);
  name = child->name;
  return_value_if_fail(child != NULL && child->vt != NULL, RET_BAD_PARAMS);

  type = widget_get_type(child);
  color_picker->trigger_child = child;
  if (tk_str_eq(type, WIDGET_TYPE_EDIT) || tk_str_eq(type, WIDGET_TYPE_SPIN_BOX)) {
    int32_t v = edit_get_int(child);

    if (tk_str_eq(name, COLOR_PICKER_CHILD_R)) {
      color_picker_set_color_r(widget, v);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_G)) {
      color_picker_set_color_g(widget, v);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_B)) {
      color_picker_set_color_b(widget, v);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_H)) {
      color_picker_set_color_h(widget, v);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_S)) {
      color_picker_set_color_s(widget, (float)v / 100.0f);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_V)) {
      color_picker_set_color_v(widget, (float)v / 100.0f);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_NEW)) {
      char color[32];
      memset(color, 0x00, sizeof(color));
      color[0] = '#';
      wstr_get_utf8(&(child->text), color + 1, sizeof(color) - 2);
      if (strlen(color) == 7) {
        color_picker_set_color(widget, color);
      }
    }
  } else if (tk_str_eq(type, WIDGET_TYPE_SLIDER)) {
    int32_t v = widget_get_value(child);
    if (tk_str_eq(name, COLOR_PICKER_CHILD_R)) {
      color_picker_set_color_r(widget, v * 255 / 100);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_G)) {
      color_picker_set_color_g(widget, v * 255 / 100);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_B)) {
      color_picker_set_color_b(widget, v * 255 / 100);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_H)) {
      color_picker_set_color_h(widget, v * 360 / 100);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_S)) {
      color_picker_set_color_s(widget, (float)v / 100.0f);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_V)) {
      color_picker_set_color_v(widget, (float)v / 100.0f);
    }
  } else if (tk_str_eq(type, WIDGET_TYPE_COLOR_COMPONENT)) {
    if (tk_str_eq(name, COLOR_PICKER_CHILD_SV)) {
      color_component_t* color_cmp = COLOR_COMPONENT(child);
      float s = color_component_get_s(child);
      float v = color_component_get_v(child);
      color_picker->last_hue = color_cmp->last_hue;
      color_picker_set_color_sv(widget, s, v);
    } else if (tk_str_eq(name, COLOR_PICKER_CHILD_H)) {
      float h = color_component_get_h(child);
      color_picker_set_color_h(widget, h);
    }
  }
  color_picker->trigger_child = NULL;

  return RET_OK;
}

static ret_t color_picker_hook_children(void* ctx, const void* iter) {
  const char* name = NULL;
  const char* type = NULL;
  widget_t* child = WIDGET(iter);
  return_value_if_fail(child != NULL, RET_BAD_PARAMS);

  name = child->name;
  type = widget_get_type(child);

  if (name != NULL) {
    if (tk_str_eq(type, WIDGET_TYPE_EDIT) || tk_str_eq(type, WIDGET_TYPE_SLIDER) ||
        tk_str_eq(type, WIDGET_TYPE_COLOR_COMPONENT) || tk_str_eq(type, WIDGET_TYPE_SPIN_BOX)) {
      widget_on(WIDGET(iter), EVT_VALUE_CHANGING, color_picker_on_child_value_changing, ctx);
      widget_on(WIDGET(iter), EVT_VALUE_CHANGED, color_picker_on_child_value_changing, ctx);
    }
  }

  return RET_OK;
}

widget_t* color_picker_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(color_picker), x, y, w, h);
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(color_picker != NULL, NULL);

  color_picker->inited = FALSE;
  color_picker_set_color(widget, "gold");

  return widget;
}

static ret_t color_picker_update_color(widget_t* widget, color_t color) {
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(widget != NULL && color_picker != NULL, RET_BAD_PARAMS);

  if (color_picker->last_hue == -1) {
    float h = 0;
    float s = 0;
    float v = 0;
    convertRGBtoHSV(color.rgba.r, color.rgba.g, color.rgba.b, &h, &s, &v);
    color_picker->last_hue = h;
  }

  if (color_picker->c.color != color.color) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    color_picker->c = color;
    color_hex_str(color_picker->c, color_picker->value);
    color_picker_sync_children(widget);
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t color_picker_set_color(widget_t* widget, const char* color) {
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(color_picker != NULL && color != NULL, RET_BAD_PARAMS);

  float h = 0;
  float s = 0;
  float v = 0;
  color_picker->init_c = color_parse(color);
  convertRGBtoHSV(color_picker->init_c.rgba.r, color_picker->init_c.rgba.g,
                  color_picker->init_c.rgba.b, &h, &s, &v);
  color_picker->last_hue = h;
  color_picker_update_color(widget, color_picker->init_c);
  return RET_OK;
}

widget_t* color_picker_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, color_picker), NULL);

  return widget;
}

static ret_t color_picker_init_if_not_inited(widget_t* widget) {
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(color_picker != NULL, RET_BAD_PARAMS);

  if (!(color_picker->inited)) {
    widget_foreach(WIDGET(widget), color_picker_hook_children, widget);
    color_picker_sync_children(WIDGET(widget));
    color_picker->inited = TRUE;
  }

  return RET_OK;
}

static ret_t color_picker_on_paint_begin(widget_t* widget, canvas_t* c) {
  return color_picker_init_if_not_inited(widget);
}
