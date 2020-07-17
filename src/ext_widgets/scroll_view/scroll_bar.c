/**
 * File:   scroll_bar.h
 * Author: AWTK Develop Team
 * Brief:  scroll_bar
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
 * 2018-01-28 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "widgets/button.h"
#include "base/layout.h"
#include "widgets/dragger.h"
#include "scroll_view/scroll_bar.h"
#include "widget_animators/widget_animator_value.h"
#include "widget_animators/widget_animator_opacity.h"

#define CHILD_UP "up"
#define CHILD_DOWN "down"
#define CHILD_DRAGGER "dragger"

#ifndef TK_DRAGGER_MIN_SIZE
#define TK_DRAGGER_MIN_SIZE 10
#endif /*TK_DRAGGER_MIN_SIZE*/

static ret_t scroll_bar_update_dragger(widget_t* widget);
widget_t* scroll_bar_create_desktop_self(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

/*mobile*/
static ret_t scroll_bar_mobile_get_dragger_size(widget_t* widget, rect_t* r) {
  int64_t x = 0;
  int64_t y = 0;
  int64_t w = 0;
  int64_t h = 0;
  int64_t virtual_size = 0;
  int64_t value = 0;
  int64_t widget_w = widget->w;
  int64_t widget_h = widget->h;
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);

  if (scroll_bar->virtual_size <= 0) {
    return RET_OK;
  }

  virtual_size = scroll_bar->virtual_size;
  value = scroll_bar->value;
  if (widget_w > widget_h) {
    /*horizon*/
    return_value_if_fail(virtual_size >= widget_w, RET_BAD_PARAMS);
    y = 1;
    h = widget_h - 2;
    w = (widget_w * widget_w) / virtual_size;
    w = tk_max(w, 4);
    x = (widget_w - w) * value / virtual_size;
  } else {
    /*vertical*/
    return_value_if_fail(virtual_size >= widget_h, RET_BAD_PARAMS);
    x = 1;
    w = widget_w - 2;
    h = (widget_h * widget_h) / virtual_size;
    h = tk_max(h, 4);
    y = (widget_h - h) * value / virtual_size;
  }

  r->x = x;
  r->y = y;
  r->w = w;
  r->h = h;

  return RET_OK;
}

static ret_t scroll_bar_mobile_on_paint_self(widget_t* widget, canvas_t* c) {
  rect_t r = rect_init(0, 0, 0, 0);
  style_t* style = widget->astyle;
  color_t trans = color_init(80, 80, 80, 0xff);
  color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, trans);

  return_value_if_fail(scroll_bar_mobile_get_dragger_size(widget, &r) == RET_OK, RET_FAIL);

  canvas_set_fill_color(c, fg);
  canvas_fill_rect(c, r.x, r.y, r.w, r.h);

  return RET_OK;
}

/*destkop*/
static ret_t scroll_bar_desktop_on_click(widget_t* widget, pointer_event_t* e) {
  int32_t delta = 0;
  point_t p = {e->x, e->y};
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);

  if (widget->target != NULL) {
    return RET_OK;
  }

  widget_to_local(widget, &p);
  if (widget->w > widget->h) {
    if (p.x < scroll_bar->dragger->x) {
      delta = -widget->w;
    } else {
      delta = widget->w;
    }
  } else {
    if (p.y < scroll_bar->dragger->y) {
      delta = -widget->h;
    } else {
      delta = widget->h;
    }
  }

  if (delta > 0) {
    delta -= scroll_bar->row;
  } else {
    delta += scroll_bar->row;
  }
  scroll_bar_scroll_delta(widget, delta);

  return RET_OK;
}

static ret_t scroll_bar_desktop_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;

  switch (type) {
    case EVT_POINTER_UP: {
      scroll_bar_desktop_on_click(widget, (pointer_event_t*)e);
      break;
    }
    case EVT_POINTER_LEAVE:
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    case EVT_POINTER_ENTER:
      widget_set_state(widget, WIDGET_STATE_OVER);
      break;
    case EVT_RESIZE:
    case EVT_MOVE_RESIZE: {
      widget_t* up = widget_lookup(widget, CHILD_UP, FALSE);
      widget_t* down = widget_lookup(widget, CHILD_DOWN, FALSE);
      bool_t horizon = widget->w > widget->h;

      if (up != NULL) {
        widget_use_style(up, horizon ? "scroll_left" : "scroll_up");
      }

      if (down != NULL) {
        widget_use_style(down, horizon ? "scroll_right" : "scroll_down");
      }

      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t scroll_bar_destop_get_dragger_size(widget_t* widget, rect_t* r) {
  int64_t x = 0;
  int64_t y = 0;
  int64_t w = 0;
  int64_t h = 0;
  int64_t virtual_size = 0;
  int64_t value = 0;
  int64_t widget_w = widget->w;
  int64_t widget_h = widget->h;
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);

  memset(r, 0x00, sizeof(rect_t));
  if (scroll_bar->virtual_size <= 0) {
    return RET_OK;
  }

  value = scroll_bar->value;
  if (widget_w > widget_h) {
    int64_t max_bar_w = widget_w - 2 * widget_h;
    /*horizon*/
    virtual_size = tk_max(widget_w, scroll_bar->virtual_size);

    y = 1;
    h = widget_h - 2;
    w = (widget_w * max_bar_w) / virtual_size;
    w = tk_max(w, TK_DRAGGER_MIN_SIZE);
    x = (widget_w - w - 2 * widget_h) * value / virtual_size + widget_h;
  } else {
    /*vertical*/
    int64_t max_bar_h = widget_h - 2 * widget_w;
    virtual_size = tk_max(widget_h, scroll_bar->virtual_size);

    x = 1;
    w = widget_w - 2;
    h = (widget_h * max_bar_h) / virtual_size;
    h = tk_max(h, TK_DRAGGER_MIN_SIZE);
    y = (widget_h - h - 2 * widget_w) * value / virtual_size + widget_w;
  }

  r->x = x;
  r->y = y;
  r->w = w;
  r->h = h;

  return RET_OK;
}

ret_t scroll_bar_add_delta_ex(widget_t* widget, int32_t d, bool_t animatable) {
  int64_t delta = 0;
  int64_t new_value = 0;
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);

  if (widget->w > widget->h) {
    if (scroll_bar->virtual_size > widget->w) {
      double scale = (double)(scroll_bar->virtual_size) / (scroll_bar->virtual_size - widget->w);
      delta = d * scale;
    }
  } else {
    if (scroll_bar->virtual_size > widget->h) {
      double scale = (double)(scroll_bar->virtual_size) / (scroll_bar->virtual_size - widget->h);
      delta = d * scale;
    }
  }

  new_value = scroll_bar->value + delta;
  new_value = tk_max(new_value, 0);
  new_value = tk_min(new_value, scroll_bar->virtual_size);

  if (scroll_bar->value != new_value) {
    if (scroll_bar->animatable && animatable) {
      scroll_bar_scroll_to(widget, new_value, 500);
    } else {
      scroll_bar_set_value(widget, new_value);
    }
  }

  return RET_OK;
}

ret_t scroll_bar_scroll_delta(widget_t* widget, int32_t delta) {
  return scroll_bar_add_delta_ex(widget, delta, TRUE);
}

ret_t scroll_bar_add_delta(widget_t* widget, int32_t delta) {
  return scroll_bar_add_delta_ex(widget, delta, FALSE);
}

static ret_t scroll_bar_on_up_button_clicked(void* ctx, event_t* e) {
  return scroll_bar_scroll_delta(WIDGET(ctx), -SCROLL_BAR(ctx)->row);
}

static ret_t scroll_bar_on_down_button_clicked(void* ctx, event_t* e) {
  return scroll_bar_scroll_delta(WIDGET(ctx), SCROLL_BAR(ctx)->row);
}

static ret_t scroll_bar_on_drag(void* ctx, event_t* e) {
  int64_t value = 0;
  widget_t* widget = WIDGET(ctx);
  int64_t widget_w = widget->w;
  int64_t widget_h = widget->h;
  scroll_bar_t* scroll_bar = SCROLL_BAR(ctx);
  widget_t* dragger = scroll_bar->dragger;

  if (widget_w > widget_h) {
    int64_t x = scroll_bar->dragger->x;
    int64_t max_x = (widget_w - 2 * widget_h - dragger->w);
    value = (x - widget_h) * scroll_bar->virtual_size / max_x;
  } else {
    int64_t y = scroll_bar->dragger->y;
    int64_t max_y = (widget_h - 2 * widget_w - dragger->h);
    value = (y - widget_w) * scroll_bar->virtual_size / max_y;
  }

  scroll_bar_set_value(widget, value);

  return RET_OK;
}

static ret_t scroll_bar_on_layout_children(widget_t* widget) {
  int32_t widget_w = widget->w;
  int32_t widget_h = widget->h;
  rect_t r = rect_init(0, 0, 0, 0);
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);
  widget_t* dragger = scroll_bar->dragger;
  widget_t* up = widget_lookup(widget, CHILD_UP, FALSE);
  widget_t* down = widget_lookup(widget, CHILD_DOWN, FALSE);

  if (widget->w > widget->h) {
    if (up != NULL) {
      widget_move_resize(up, 0, 0, widget->h, widget->h);
    }

    if (down != NULL) {
      widget_move_resize(down, widget->w - widget->h, 0, widget->h, widget->h);
    }
  } else {
    if (up != NULL) {
      widget_move_resize(up, 0, 0, widget->w, widget->w);
    }

    if (down != NULL) {
      widget_move_resize(down, 0, widget->h - widget->w, widget->w, widget->w);
    }
  }

  if (scroll_bar->virtual_size <= 0) {
    return RET_OK;
  }
  return_value_if_fail(scroll_bar_destop_get_dragger_size(widget, &r) == RET_OK, RET_FAIL);

  if (dragger != NULL) {
    if (widget->w > widget->h) {
      int32_t max_x = widget_h + (widget_w - 2 * widget_h - r.w);
      dragger_set_range(dragger, widget_h, r.y, max_x, r.y);
    } else {
      int32_t max_y = widget_w + (widget_h - 2 * widget_w - r.h);
      dragger_set_range(dragger, r.x, widget_w, r.x, max_y);
    }

    widget_move_resize(WIDGET(dragger), r.x, r.y, r.w, r.h);
  }

  widget_invalidate_force(widget, NULL);

  return RET_OK;
}

static ret_t scroll_bar_create_children(widget_t* widget) {
  widget_t* up = NULL;
  widget_t* down = NULL;
  widget_t* dragger = NULL;
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);
  return_value_if_fail(widget != NULL && scroll_bar != NULL, RET_BAD_PARAMS);

  if (scroll_bar->dragger != NULL) {
    return RET_OK;
  }

  up = button_create(widget, 0, 0, 0, 0);
  up->auto_created = TRUE;
  button_set_repeat(up, 300);
  widget_set_name(up, CHILD_UP);
  widget_on(up, EVT_CLICK, scroll_bar_on_up_button_clicked, widget);

  down = button_create(widget, 0, 0, 0, 0);
  down->auto_created = TRUE;
  button_set_repeat(down, 300);
  widget_set_name(down, CHILD_DOWN);
  widget_on(down, EVT_CLICK, scroll_bar_on_down_button_clicked, widget);

  dragger = dragger_create(widget, 0, 0, 0, 0);
  dragger->auto_created = TRUE;
  widget_set_name(dragger, CHILD_DRAGGER);
  widget_use_style(dragger, "scroll_bar");
  widget_on(dragger, EVT_DRAG, scroll_bar_on_drag, widget);

  if (widget->w > widget->h) {
    widget_use_style(up, "scroll_left");
    widget_use_style(down, "scroll_right");
  } else {
    widget_use_style(up, "scroll_up");
    widget_use_style(down, "scroll_down");
  }

  scroll_bar->row = 30;
  scroll_bar->dragger = dragger;
  widget_set_need_relayout_children(widget);

  return RET_OK;
}

/*share*/
ret_t scroll_bar_set_params(widget_t* widget, int32_t virtual_size, int32_t row) {
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);
  return_value_if_fail(scroll_bar != NULL, RET_BAD_PARAMS);

  widget_set_need_relayout_children(widget);
  scroll_bar->virtual_size = virtual_size;
  scroll_bar->row = row;

  if (scroll_bar->value >= virtual_size) {
    scroll_bar->value = virtual_size - row;
  }

  return RET_OK;
}

static ret_t scroll_bar_get_prop(widget_t* widget, const char* name, value_t* v) {
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);
  return_value_if_fail(scroll_bar != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    value_set_int(v, scroll_bar->virtual_size);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ROW)) {
    value_set_int(v, scroll_bar->row);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANIMATABLE)) {
    value_set_bool(v, scroll_bar->animatable);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_int(v, scroll_bar->value);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t scroll_bar_set_prop(widget_t* widget, const char* name, const value_t* v) {
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);
  return_value_if_fail(scroll_bar != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    scroll_bar->virtual_size = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ROW)) {
    scroll_bar->row = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANIMATABLE)) {
    scroll_bar->animatable = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    scroll_bar_set_value(widget, value_int(v));
    scroll_bar_update_dragger(widget);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const char* s_scroll_bar_clone_properties[] = {
    WIDGET_PROP_MAX, WIDGET_PROP_ROW, WIDGET_PROP_ANIMATABLE, WIDGET_PROP_VALUE, NULL};
static const char* s_scroll_bar_persitent_properties[] = {WIDGET_PROP_ANIMATABLE, NULL};

TK_DECL_VTABLE(scroll_bar_mobile) = {.size = sizeof(scroll_bar_t),
                                     .inputable = TRUE,
                                     .type = WIDGET_TYPE_SCROLL_BAR_MOBILE,
                                     .clone_properties = s_scroll_bar_clone_properties,
                                     .parent = TK_PARENT_VTABLE(widget),
                                     .create = scroll_bar_create_mobile,
                                     .set_prop = scroll_bar_set_prop,
                                     .get_prop = scroll_bar_get_prop,
                                     .on_layout_children = scroll_bar_on_layout_children,
                                     .on_paint_self = scroll_bar_mobile_on_paint_self};

TK_DECL_VTABLE(scroll_bar_desktop) = {.size = sizeof(scroll_bar_t),
                                      .inputable = TRUE,
                                      .type = WIDGET_TYPE_SCROLL_BAR_DESKTOP,
                                      .clone_properties = s_scroll_bar_clone_properties,
                                      .persistent_properties = s_scroll_bar_persitent_properties,
                                      .parent = TK_PARENT_VTABLE(widget),
                                      .create = scroll_bar_create_desktop_self,
                                      .on_event = scroll_bar_desktop_on_event,
                                      .on_layout_children = scroll_bar_on_layout_children,
                                      .set_prop = scroll_bar_set_prop,
                                      .get_prop = scroll_bar_get_prop};

bool_t scroll_bar_is_mobile(widget_t* widget) {
  return widget && WIDGET_IS_INSTANCE_OF(widget, scroll_bar_mobile);
}

static ret_t scroll_bar_on_value_animate_end(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  scroll_bar_t* scroll_bar = SCROLL_BAR(ctx);
  return_value_if_fail(widget != NULL && scroll_bar != NULL, RET_REMOVE);

  scroll_bar->wa_value = NULL;
  return RET_REMOVE;
}

static ret_t scroll_bar_on_opactiy_animate_end(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  scroll_bar_t* scroll_bar = SCROLL_BAR(ctx);
  return_value_if_fail(widget != NULL && scroll_bar != NULL, RET_REMOVE);

  scroll_bar->wa_opactiy = NULL;

  if (scroll_bar_is_mobile(widget)) {
    widget_set_visible_only(widget, FALSE);
  }

  return RET_REMOVE;
}

ret_t scroll_bar_scroll_to(widget_t* widget, int32_t value, int32_t duration) {
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);
  return_value_if_fail(scroll_bar != NULL, RET_BAD_PARAMS);

  if (scroll_bar->wa_value != NULL) {
    widget_animator_destroy(scroll_bar->wa_value);
    scroll_bar->wa_value = NULL;
  }

  if (scroll_bar->wa_opactiy != NULL) {
    widget_animator_destroy(scroll_bar->wa_opactiy);
    scroll_bar->wa_opactiy = NULL;
  }

  widget_set_opacity(widget, 0xff);
  widget_set_visible_only(widget, TRUE);
  widget_invalidate_force(widget, NULL);

  if (scroll_bar->value == value) {
    scroll_bar_hide_by_opacity_animation(widget, duration);
    return RET_OK;
  }

  scroll_bar->wa_value = widget_animator_value_create(widget, duration, 0, EASING_SIN_INOUT);
  return_value_if_fail(scroll_bar->wa_value != NULL, RET_OOM);
  widget_animator_value_set_params(scroll_bar->wa_value, scroll_bar->value, value);
  widget_animator_start(scroll_bar->wa_value);
  widget_animator_on(scroll_bar->wa_value, EVT_ANIM_END, scroll_bar_on_value_animate_end,
                     scroll_bar);

  if (scroll_bar_is_mobile(widget)) {
    scroll_bar_hide_by_opacity_animation(widget, duration);
  } else {
    scroll_bar->wa_opactiy = NULL;
  }

  return RET_OK;
}

static ret_t scroll_bar_update_dragger(widget_t* widget) {
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);
  return_value_if_fail(scroll_bar != NULL, RET_BAD_PARAMS);

  if (!scroll_bar_is_mobile(widget)) {
    rect_t r = rect_init(0, 0, 0, 0);
    return_value_if_fail(scroll_bar_destop_get_dragger_size(widget, &r) == RET_OK, RET_FAIL);
    widget_move_resize(scroll_bar->dragger, r.x, r.y, r.w, r.h);
  }

  return RET_OK;
}

ret_t scroll_bar_set_value(widget_t* widget, int32_t value) {
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);
  return_value_if_fail(scroll_bar != NULL, RET_BAD_PARAMS);

  if (scroll_bar->value != value) {
    event_t e = event_init(EVT_VALUE_CHANGED, widget);

    scroll_bar_set_value_only(widget, value);
    widget_dispatch(widget, &e);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t scroll_bar_set_value_only(widget_t* widget, int32_t value) {
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);
  return_value_if_fail(scroll_bar != NULL, RET_BAD_PARAMS);

  if (value < 0) {
    value = 0;
  }

  if (value > scroll_bar->virtual_size) {
    value = scroll_bar->virtual_size;
  }

  scroll_bar->value = value;

  if (!scroll_bar_is_mobile(widget)) {
    widget_set_need_relayout_children(widget);
  }

  return RET_OK;
}

/*create*/

static widget_t* scroll_bar_create_internal(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h,
                                            const widget_vtable_t* vt) {
  widget_t* widget = widget_create(parent, vt, x, y, w, h);
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);
  return_value_if_fail(scroll_bar != NULL, NULL);

  scroll_bar->animatable = TRUE;
  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return widget;
}

widget_t* scroll_bar_create_mobile(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return scroll_bar_create_internal(parent, x, y, w, h, TK_REF_VTABLE(scroll_bar_mobile));
}

widget_t* scroll_bar_create_desktop_self(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return scroll_bar_create_internal(parent, x, y, w, h, TK_REF_VTABLE(scroll_bar_desktop));
}

widget_t* scroll_bar_create_desktop(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = scroll_bar_create_desktop_self(parent, x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);

  scroll_bar_create_children(widget);

  return widget;
}

widget_t* scroll_bar_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
#ifdef WITH_DESKTOP_STYLE
  return scroll_bar_create_desktop(parent, x, y, w, h);
#else
  return scroll_bar_create_mobile(parent, x, y, w, h);
#endif /*WITH_DESKTOP_STYLE*/
}

widget_t* scroll_bar_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, scroll_bar_mobile) ||
                           WIDGET_IS_INSTANCE_OF(widget, scroll_bar_desktop),
                       NULL);

  return widget;
}

ret_t scroll_bar_hide_by_opacity_animation(widget_t* widget, int32_t duration) {
  scroll_bar_t* scroll_bar = SCROLL_BAR(widget);
  return_value_if_fail(scroll_bar != NULL, RET_BAD_PARAMS);
  if (scroll_bar_is_mobile(widget)) {
    scroll_bar->wa_opactiy =
        widget_animator_opacity_create(widget, duration, duration, EASING_SIN_INOUT);
    widget_animator_on(scroll_bar->wa_opactiy, EVT_ANIM_END, scroll_bar_on_opactiy_animate_end,
                       scroll_bar);
    widget_animator_opacity_set_params(scroll_bar->wa_opactiy, 0xff, 0);
    widget_animator_start(scroll_bar->wa_opactiy);
  }
  return RET_OK;
}
