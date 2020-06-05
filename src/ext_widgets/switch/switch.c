/**
 * File:   switch.c
 * Author: AWTK Develop Team
 * Brief:  switch
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
 * 2018-09-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "switch/switch.h"
#include "base/image_manager.h"
#include "base/widget_vtable.h"
#include "widget_animators/widget_animator_scroll.h"

#define ANIMATING_TIME 300

static ret_t switch_on_pointer_down(switch_t* aswitch, pointer_event_t* e) {
  velocity_t* v = &(aswitch->velocity);

  velocity_reset(v);
  aswitch->xdown = e->x;
  aswitch->xoffset_save = aswitch->xoffset;

  velocity_update(v, e->e.time, e->x, e->y);

  return RET_OK;
}

static ret_t switch_on_pointer_move(switch_t* aswitch, pointer_event_t* e) {
  velocity_t* v = &(aswitch->velocity);
  widget_t* widget = WIDGET(aswitch);
  int32_t dx = e->x - aswitch->xdown;
  int32_t max_xoffset = aswitch->max_xoffset_ratio * widget->w;

  velocity_update(v, e->e.time, e->x, e->y);
  if (aswitch->wa == NULL && dx) {
    aswitch->xoffset = aswitch->xoffset_save - dx;
  }

  aswitch->xoffset = tk_max(0, aswitch->xoffset);
  aswitch->xoffset = tk_min(aswitch->xoffset, max_xoffset);

  return RET_OK;
}

static ret_t switch_on_scroll_done(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  switch_t* aswitch = SWITCH(ctx);
  return_value_if_fail(widget != NULL && aswitch != NULL, RET_BAD_PARAMS);

  aswitch->wa = NULL;
  switch_set_value(widget, aswitch->xoffset == 0);

  return RET_REMOVE;
}

static ret_t switch_scroll_to(widget_t* widget, int32_t xoffset_end) {
  int32_t xoffset = 0;
  switch_t* aswitch = SWITCH(widget);
  return_value_if_fail(aswitch != NULL, RET_FAIL);

  xoffset = aswitch->xoffset;
  if (xoffset == xoffset_end) {
    switch_set_value(widget, aswitch->xoffset == 0);
    return RET_OK;
  }

  aswitch->wa = widget_animator_scroll_create(widget, ANIMATING_TIME, 0, EASING_SIN_INOUT);
  return_value_if_fail(aswitch->wa != NULL, RET_OOM);

  widget_animator_scroll_set_params(aswitch->wa, xoffset, 0, xoffset_end, 0);
  widget_animator_on(aswitch->wa, EVT_ANIM_END, switch_on_scroll_done, aswitch);
  widget_animator_start(aswitch->wa);

  return RET_OK;
}

static ret_t switch_on_pointer_up(switch_t* aswitch, pointer_event_t* e) {
  velocity_t* v = NULL;
  int32_t xoffset_end = 0;
  int32_t min_xoffset = 0;
  int32_t max_xoffset = 0;
  widget_t* widget = WIDGET(aswitch);
  return_value_if_fail(widget != NULL && aswitch != NULL, RET_BAD_PARAMS);

  v = &(aswitch->velocity);
  max_xoffset = aswitch->max_xoffset_ratio * widget->w;

  velocity_update(v, e->e.time, e->x, e->y);
  xoffset_end = aswitch->xoffset - v->yv;

  if (e->x == aswitch->xdown) {
    /*click*/
    if (aswitch->value) {
      xoffset_end = max_xoffset;
    } else {
      xoffset_end = min_xoffset;
    }
  } else {
    if (xoffset_end < max_xoffset / 2) {
      xoffset_end = min_xoffset;
    } else {
      xoffset_end = max_xoffset;
    }
  }

  switch_scroll_to(widget, xoffset_end);

  return RET_OK;
}

static ret_t switch_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  switch_t* aswitch = SWITCH(widget);
  ret_t ret = RET_OK;
  return_value_if_fail(widget != NULL && aswitch != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN: {
      aswitch->pressed = TRUE;
      aswitch->point_down_aborted = FALSE;
      widget_grab(widget->parent, widget);
      switch_on_pointer_down(aswitch, (pointer_event_t*)e);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      if (aswitch->pressed) {
        aswitch->pressed = FALSE;
        aswitch->point_down_aborted = TRUE;
        aswitch->xoffset = aswitch->xoffset_save;
        widget_ungrab(widget->parent, widget);
      }
      break;
    }
    case EVT_POINTER_UP: {
      aswitch->pressed = FALSE;
      if (!aswitch->point_down_aborted) {
        switch_on_pointer_up(aswitch, (pointer_event_t*)e);
        widget_ungrab(widget->parent, widget);
      } else {
        aswitch->xoffset = aswitch->xoffset_save;
      }
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (aswitch->pressed && !aswitch->point_down_aborted) {
        switch_on_pointer_move(aswitch, evt);
        widget_invalidate(widget, NULL);
        ret = RET_STOP;
      }
      break;
    }
    case EVT_RESIZE:
    case EVT_MOVE_RESIZE: {
      if (aswitch->value) {
        aswitch->xoffset = 0;
      } else {
        aswitch->xoffset = aswitch->max_xoffset_ratio * widget->w;
      }
      break;
    }
    default:
      break;
  }

  return ret;
}

ret_t switch_fill_rect_color(widget_t* widget, canvas_t* c, rect_t* r, bool_t bg) {
  style_t* style = widget->astyle;
  switch_t* aswitch = SWITCH(widget);
  color_t trans = color_init(0, 0, 0, 0);
  uint32_t radius = style_get_int(style, STYLE_ID_ROUND_RADIUS, 0);
  const char* color_key = bg ? (aswitch->value ? STYLE_ID_SELECTED_BG_COLOR : STYLE_ID_BG_COLOR)
                             : (aswitch->value ? STYLE_ID_SELECTED_FG_COLOR : STYLE_ID_FG_COLOR);

  color_t color = style_get_color(style, color_key, trans);

  if (color.rgba.a == 0 && aswitch->value) {
    color_key = bg ? STYLE_ID_BG_COLOR : STYLE_ID_FG_COLOR;
    color = style_get_color(style, color_key, trans);
  }

  if (color.rgba.a && r->w > 0 && r->h > 0) {
    canvas_set_fill_color(c, color);
    if (radius > 3) {
      vgcanvas_t* vg = canvas_get_vgcanvas(c);
      if (vg != NULL) {
        xy_t x = r->x + 0.5;
        xy_t y = r->y + 0.5;
        vgcanvas_set_fill_color(vg, color);
        vgcanvas_translate(vg, c->ox, c->oy);
        vgcanvas_rounded_rect(vg, x, y, r->w, r->h, radius);
        vgcanvas_translate(vg, -c->ox, -c->oy);
        vgcanvas_fill(vg);
      } else {
        canvas_fill_rect(c, r->x, r->y, r->w, r->h);
      }
    } else {
      canvas_fill_rect(c, r->x, r->y, r->w, r->h);
    }
  }

  return RET_OK;
}

static ret_t switch_on_paint_background_img(widget_t* widget, canvas_t* c, bitmap_t* img) {
  int32_t w = 0;
  int32_t h = 0;
  int32_t iw = 0;
  int32_t ih = 0;
  float_t wscale = 0;
  float_t hscale = 0;
  int32_t xoffset = 0;
  int32_t round_radius = 0;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  switch_t* aswitch = SWITCH(widget);
  return_value_if_fail(img != NULL && widget != NULL && aswitch != NULL && vg != NULL,
                       RET_BAD_PARAMS);

  iw = img->w;
  ih = img->h;
  wscale = (float_t)(widget->w) / (float_t)iw;
  hscale = (float_t)(widget->h) / (float_t)ih;
  xoffset = (float_t)(aswitch->xoffset) / wscale;
  round_radius = style_get_int(widget->astyle, STYLE_ID_ROUND_RADIUS, 0) / hscale;

  h = ih;
  w = iw * (1 - aswitch->max_xoffset_ratio);
  wscale = (float_t)(widget->w) / (float_t)w;

  if (vg == NULL || (round_radius < 5 && hscale == 1 && wscale == 1)) {
    int32_t x = (widget->w - w) >> 1;
    int32_t y = (widget->h - ih) >> 1;
    rect_t src = rect_init(xoffset, 0, w, ih);
    rect_t dst = rect_init(x, y, w, ih);

    dst.y = tk_max(0, y);
    dst.h = tk_min(dst.h, widget->h);

    return canvas_draw_image(c, img, &src, &dst);
  }

  if (vg != NULL) {
    vgcanvas_save(vg);
    vgcanvas_translate(vg, c->ox, c->oy);
    vgcanvas_scale(vg, wscale, hscale);
    vgcanvas_translate(vg, -xoffset, 0);
    vgcanvas_rounded_rect(vg, xoffset, 0, w, h, round_radius);
    vgcanvas_paint(vg, FALSE, img);
    vgcanvas_restore(vg);
  }

  return RET_OK;
}

static ret_t switch_on_paint_background(widget_t* widget, canvas_t* c) {
  style_t* style = NULL;
  const char* image_name = NULL;
  switch_t* aswitch = SWITCH(widget);
  return_value_if_fail(aswitch != NULL && widget->astyle != NULL, RET_BAD_PARAMS);

  style = widget->astyle;
  image_name = style_get_str(style, STYLE_ID_BG_IMAGE, NULL);

  if (image_name != NULL && *image_name) {
    bitmap_t img;
    if (widget_load_image(widget, image_name, &img) == RET_OK) {
      return switch_on_paint_background_img(widget, c, &img);
    }
  } else {
    int32_t w = widget->w;  //tk_roundi(widget->w * 1.5 * (1 - aswitch->max_xoffset_ratio));
    rect_t r = rect_init((widget->w - w) / 2.0f, 0, w, widget->h);
    return switch_fill_rect_color(widget, c, &r, TRUE);
  }

  return RET_FAIL;
}

static ret_t switch_on_paint_self(widget_t* widget, canvas_t* c) {
  float_t xoffset, xoffset_ratio;
  int32_t max_xoffset;
  int32_t bar_size;
  int32_t w;
  rect_t r = rect_init(0, 0, 0, 0);
  int32_t margin = 0;
  int32_t margin_left = 0;
  int32_t margin_right = 0;
  int32_t margin_top = 0;
  int32_t margin_bottom = 0;
  style_t* style = NULL;
  const char* image_name = NULL;
  const char* image_key = NULL;
  switch_t* aswitch = SWITCH(widget);
  return_value_if_fail(aswitch != NULL && widget->astyle != NULL, RET_BAD_PARAMS);

  style = widget->astyle;
  image_key = STYLE_ID_FG_IMAGE;
  image_name = style_get_str(style, image_key, NULL);
  margin = style_get_int(style, STYLE_ID_MARGIN, 0);
  margin_top = style_get_int(style, STYLE_ID_MARGIN_TOP, margin);
  margin_left = style_get_int(style, STYLE_ID_MARGIN_LEFT, margin);
  margin_right = style_get_int(style, STYLE_ID_MARGIN_RIGHT, margin);
  margin_bottom = style_get_int(style, STYLE_ID_MARGIN_BOTTOM, margin);

  w = tk_roundi(widget->w * 1.5 * (1 - aswitch->max_xoffset_ratio));
  bar_size = tk_roundi(w / 2);
  max_xoffset = aswitch->max_xoffset_ratio * widget->w;
  xoffset_ratio = (float_t)(w - bar_size) / max_xoffset;
  xoffset = (float_t)(aswitch->xoffset) * xoffset_ratio;
  r.x = widget->w / 2.0 - xoffset + margin_left;
  r.y = margin_top;
  r.w = bar_size - margin_left - margin_right;
  r.h = widget->h - margin_top - margin_bottom;

  if (image_name == NULL || *image_name == 0) {
    switch_fill_rect_color(widget, c, &r, FALSE);
  } else {
    bitmap_t img;
    if (widget_load_image(widget, image_name, &img) == RET_OK) {
      image_draw_type_t draw_type =
          (image_draw_type_t)style_get_int(style, STYLE_ID_FG_IMAGE_DRAW_TYPE, IMAGE_DRAW_CENTER);
      canvas_draw_image_ex(c, &img, draw_type, &r);
    }
  }

  return RET_OK;
}

ret_t switch_set_value(widget_t* widget, bool_t value) {
  switch_t* aswitch = SWITCH(widget);
  return_value_if_fail(aswitch != NULL, RET_BAD_PARAMS);

  if (aswitch->value != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    aswitch->value = value;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  if (value) {
    aswitch->xoffset = 0;
  } else {
    aswitch->xoffset = aswitch->max_xoffset_ratio * widget->w;
  }

  widget_invalidate(widget, NULL);

  return RET_OK;
}

static ret_t switch_get_prop(widget_t* widget, const char* name, value_t* v) {
  switch_t* aswitch = SWITCH(widget);
  return_value_if_fail(aswitch != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_bool(v, aswitch->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    value_set_int(v, aswitch->xoffset);
    return RET_OK;
  } else if (tk_str_eq(name, SWITCH_PROP_MAX_XOFFSET_RATIO)) {
    value_set_float(v, aswitch->max_xoffset_ratio);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t switch_set_prop(widget_t* widget, const char* name, const value_t* v) {
  switch_t* aswitch = SWITCH(widget);
  return_value_if_fail(aswitch != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return switch_set_value(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    aswitch->xoffset = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    return RET_OK;
  } else if (tk_str_eq(name, SWITCH_PROP_MAX_XOFFSET_RATIO)) {
    aswitch->max_xoffset_ratio = value_float(v);
    if (aswitch->value) {
      aswitch->xoffset = 0;
    } else {
      aswitch->xoffset = aswitch->max_xoffset_ratio * widget->w;
    }
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const char* s_switch_properties[] = {WIDGET_PROP_VALUE, WIDGET_PROP_XOFFSET,
                                            SWITCH_PROP_MAX_XOFFSET_RATIO, NULL};

TK_DECL_VTABLE(switch) = {
    .inputable = TRUE,
    .size = sizeof(switch_t),
    .type = WIDGET_TYPE_SWITCH,
    .clone_properties = s_switch_properties,
    .persistent_properties = s_switch_properties,
    .parent = TK_PARENT_VTABLE(widget),
    .create = switch_create,
    .on_event = switch_on_event,
    .on_paint_background = switch_on_paint_background,
    .on_paint_self = switch_on_paint_self,
    .get_prop = switch_get_prop,
    .set_prop = switch_set_prop,
};

widget_t* switch_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(switch), x, y, w, h);
  switch_t* aswitch = SWITCH(widget);
  return_value_if_fail(aswitch != NULL, NULL);

  aswitch->value = TRUE;
  aswitch->pressed = FALSE;
  aswitch->max_xoffset_ratio = 0.34f;

  return widget;
}

widget_t* switch_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, switch), NULL);

  return widget;
}
