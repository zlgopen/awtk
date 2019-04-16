/**
 * File:   switch.c
 * Author: AWTK Develop Team
 * Brief:  switch
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
  return_value_if_fail(widget != NULL && aswitch != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN: {
      aswitch->point_down_aborted = FALSE;
      widget_grab(widget->parent, widget);
      switch_on_pointer_down(aswitch, (pointer_event_t*)e);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      aswitch->point_down_aborted = TRUE;
      aswitch->xoffset = aswitch->xoffset_save;
      widget_ungrab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_UP: {
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
      if (evt->pressed && !aswitch->point_down_aborted) {
        switch_on_pointer_move(aswitch, evt);
        widget_invalidate(widget, NULL);
      }
    }
    default:
      break;
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
  round_radius = aswitch->round_radius / hscale;

  h = ih;
  w = iw * (1 - aswitch->max_xoffset_ratio);
  wscale = (float_t)(widget->w) / (float_t)w;

#ifdef WITH_NANOVG_SOFT
  if (round_radius < 5) {
    int32_t x = (widget->w - w) >> 1;
    int32_t y = (widget->h - ih) >> 1;
    rect_t src = rect_init(xoffset, 0, w, ih);
    rect_t dst = rect_init(x, y, w, ih);

    return canvas_draw_image(c, img, &src, &dst);
  }
#endif /*WITH_NANOVG_SOFT*/

  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_scale(vg, wscale, hscale);
  vgcanvas_translate(vg, -xoffset, 0);
  vgcanvas_rounded_rect(vg, xoffset, 0, w, h, round_radius);
  vgcanvas_paint(vg, FALSE, img);
  vgcanvas_restore(vg);

  return RET_OK;
}

static ret_t switch_on_paint_background(widget_t* widget, canvas_t* c) {
  style_t* style = NULL;
  const char* image_name = NULL;
  return_value_if_fail(widget != NULL && widget->astyle != NULL, RET_BAD_PARAMS);

  style = widget->astyle;
  image_name = style_get_str(style, STYLE_ID_BG_IMAGE, "switch");

  if (image_name != NULL) {
    bitmap_t img;
    if (widget_load_image(widget, image_name, &img) == RET_OK) {
      return switch_on_paint_background_img(widget, c, &img);
    }
  }

  return RET_FAIL;
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
  } else if (tk_str_eq(name, SWITCH_PROP_ROUND_RADIUS)) {
    value_set_int(v, aswitch->round_radius);
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
  } else if (tk_str_eq(name, SWITCH_PROP_ROUND_RADIUS)) {
    aswitch->round_radius = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, SWITCH_PROP_MAX_XOFFSET_RATIO)) {
    aswitch->max_xoffset_ratio = value_float(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const char* s_switch_properties[] = {WIDGET_PROP_VALUE, WIDGET_PROP_XOFFSET,
                                            SWITCH_PROP_ROUND_RADIUS, SWITCH_PROP_MAX_XOFFSET_RATIO,
                                            NULL};

TK_DECL_VTABLE(switch) = {
    .size = sizeof(switch_t),
    .type = WIDGET_TYPE_SWITCH,
    .clone_properties = s_switch_properties,
    .persistent_properties = s_switch_properties,
    .parent = TK_PARENT_VTABLE(widget),
    .create = switch_create,
    .on_event = switch_on_event,
    .on_paint_background = switch_on_paint_background,
    .get_prop = switch_get_prop,
    .set_prop = switch_set_prop,
};

widget_t* switch_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(switch), x, y, w, h);
  switch_t* aswitch = SWITCH(widget);
  return_value_if_fail(aswitch != NULL, NULL);

  aswitch->value = TRUE;
  aswitch->round_radius = 0;
  aswitch->max_xoffset_ratio = 0.34f;

  return widget;
}

widget_t* switch_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && (widget->vt == TK_REF_VTABLE(switch)), NULL);

  return widget;
}
