/**
 * File:   slide_indicator.h
 * Author: AWTK Develop Team
 * Brief:  slide_indicator
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
 * 2019-05-29 Xu ChaoZe <xuchaoze@zlg.cn> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "base/layout.h"
#include "base/enums.h"
#include "tkc/easing.h"
#include "tkc/func_call_parser.h"
#include "slide_view/slide_indicator.h"
#include "widget_animators/widget_animator_opacity.h"
#include "base/widget_animator_manager.h"

static ret_t slide_indicator_reset_indicated_widget(widget_t* widget);
static ret_t slide_indicator_reset_indicated_widget(widget_t* widget);
static ret_t slide_indicator_set_indicated_widget(widget_t* widget, widget_t* target);

#define _RADIAN(cx, cy, x, y) atan2(-y + (cy), x - (cx))
#define _DISTANCE(x1, y1, x2, y2) sqrt((x1 - (x2)) * (x1 - (x2)) + (y1 - (y2)) * (y1 - (y2)))

static const key_type_value_t indicator_default_paint_value[] = {
    {"auto", 0, INDICATOR_DEFAULT_PAINT_AUTO},
    {"stroke_dot", 0, INDICATOR_DEFAULT_PAINT_STROKE_DOT},
    {"fill_dot", 0, INDICATOR_DEFAULT_PAINT_FILL_DOT},
    {"stroke_rect", 0, INDICATOR_DEFAULT_PAINT_STROKE_RECT},
    {"fill_rect", 0, INDICATOR_DEFAULT_PAINT_FILL_RECT}};

const key_type_value_t* indicator_default_paint_find(const char* name) {
  return find_item(indicator_default_paint_value, ARRAY_SIZE(indicator_default_paint_value), name);
}

ret_t slide_indicator_set_anchor_x(widget_t* widget, const char* anchor) {
  float_t val = 0.0f;
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL && anchor != NULL, RET_BAD_PARAMS);

  val = tk_atof(anchor);
  if (strrchr(anchor, '%')) {
    val = val * widget->w;
  }
  slide_indicator->anchor_x = val;
  slide_indicator->anchor_x_fixed = TRUE;
  return RET_OK;
}

ret_t slide_indicator_set_anchor_y(widget_t* widget, const char* anchor) {
  float_t val = 0.0f;
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL && anchor != NULL, RET_BAD_PARAMS);

  val = tk_atof(anchor);
  if (strrchr(anchor, '%')) {
    val = val * widget->h;
  }
  slide_indicator->anchor_y = val;
  slide_indicator->anchor_y_fixed = TRUE;
  return RET_OK;
}

static ret_t slide_indicator_get_prop(widget_t* widget, const char* name, value_t* v) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE) || tk_str_eq(name, WIDGET_PROP_ACTIVE)) {
    value_set_uint32(v, slide_indicator->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    value_set_uint32(v, slide_indicator->max);
    return RET_OK;
  } else if (tk_str_eq(name, SLIDE_INDICATOR_PROP_DEFAULT_PAINT)) {
    value_set_int(v, slide_indicator->default_paint);
    return RET_OK;
  } else if (tk_str_eq(name, SLIDE_INDICATOR_PROP_AUTO_HIDE)) {
    value_set_int(v, slide_indicator->auto_hide);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MARGIN)) {
    value_set_int(v, slide_indicator->margin);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SPACING)) {
    value_set_float(v, slide_indicator->spacing);
    return RET_OK;
  } else if (tk_str_eq(name, SLIDE_INDICATOR_PROP_SIZE)) {
    value_set_uint32(v, slide_indicator->size);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_X)) {
    value_set_int(v, slide_indicator->anchor_x);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_Y)) {
    value_set_int(v, slide_indicator->anchor_y);
    return RET_OK;
  } else if (tk_str_eq(name, SLIDE_INDICATOR_PROP_INDICATED_TARGET)) {
    value_set_str(v, slide_indicator->indicated_target);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t slide_indicator_set_prop(widget_t* widget, const char* name, const value_t* v) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE) || tk_str_eq(name, WIDGET_PROP_ACTIVE)) {
    return slide_indicator_set_value(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MAX)) {
    return slide_indicator_set_max(widget, value_uint32(v));
  } else if (tk_str_eq(name, SLIDE_INDICATOR_PROP_DEFAULT_PAINT)) {
    if (v->type == VALUE_TYPE_STRING) {
      const key_type_value_t* kv = indicator_default_paint_find(value_str(v));
      if (kv != NULL) {
        slide_indicator_set_default_paint(widget, (indicator_default_paint_t)(kv->value));
      }
    } else {
      slide_indicator_set_default_paint(widget, (indicator_default_paint_t)value_int(v));
    }
    return RET_OK;
  } else if (tk_str_eq(name, SLIDE_INDICATOR_PROP_AUTO_HIDE)) {
    return slide_indicator_set_auto_hide(widget, value_uint16(v));
  } else if (tk_str_eq(name, WIDGET_PROP_MARGIN)) {
    return slide_indicator_set_margin(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_SPACING)) {
    return slide_indicator_set_spacing(widget, value_float(v));
  } else if (tk_str_eq(name, SLIDE_INDICATOR_PROP_SIZE)) {
    return slide_indicator_set_size(widget, value_uint32(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_X)) {
    return slide_indicator_set_anchor_x(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ANCHOR_Y)) {
    return slide_indicator_set_anchor_y(widget, value_str(v));
  } else if (tk_str_eq(name, SLIDE_INDICATOR_PROP_INDICATED_TARGET)) {
    return slide_indicator_set_indicated_target(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t slide_indicator_fix_anchor(widget_t* widget) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  if (!slide_indicator->anchor_x_fixed || !slide_indicator->anchor_y_fixed) {
    point_t p = {0, 0};
    widget_t* target = slide_indicator->indicated_widget;
    return_value_if_fail(target != NULL, RET_BAD_PARAMS);

    widget_to_screen(target, &p);
    p.x += target->w / 2;
    p.y += target->h / 2;
    widget_to_local(widget, &p);

    slide_indicator->anchor_x = slide_indicator->anchor_x_fixed ? slide_indicator->anchor_x : p.x;
    slide_indicator->anchor_y = slide_indicator->anchor_y_fixed ? slide_indicator->anchor_y : p.y;
  }

  return RET_OK;
}

static ret_t slide_indicator_on_layout_children(widget_t* widget) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  if (slide_indicator->indicated_widget != NULL &&
      slide_indicator->indicated_widget->parent == widget->parent) {
    slide_indicator_fix_anchor(widget);
  } else {
    if (slide_indicator_reset_indicated_widget(widget) == RET_OK) {
      widget_t* target =
          widget_lookup_by_type(widget->parent, slide_indicator->indicated_target, FALSE);
      if (target) {
        slide_indicator_set_indicated_widget(widget, target);
      }
    }
  }

  return widget_layout_children_default(widget);
}

static ret_t slide_indicator_fill_rect(canvas_t* c, rect_t* r, color_t color) {
  canvas_set_fill_color(c, color);
  canvas_fill_rect(c, r->x, r->y, r->w, r->h);
  return RET_OK;
}

static ret_t slide_indicator_stroke_rect(canvas_t* c, rect_t* r, color_t color) {
  canvas_set_stroke_color(c, color);
  canvas_stroke_rect(c, r->x, r->y, r->w, r->h);
  return RET_OK;
}

static ret_t slide_indicator_fill_dot(canvas_t* c, rect_t* r, color_t color) {
  float_t radius = tk_min(r->w, r->h) / 2;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_begin_path(vg);
  vgcanvas_set_fill_color(vg, color);
  vgcanvas_arc(vg, r->x + radius, r->y + radius, radius, 0, M_PI * 2, FALSE);
  vgcanvas_fill(vg);
  vgcanvas_restore(vg);
  return RET_OK;
}

static ret_t slide_indicator_stroke_dot(canvas_t* c, rect_t* r, color_t color) {
  float_t radius = tk_min(r->w, r->h) / 2;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  vgcanvas_save(vg);
  vgcanvas_translate(vg, c->ox, c->oy);
  vgcanvas_begin_path(vg);
  vgcanvas_set_stroke_color(vg, color);
  vgcanvas_arc(vg, r->x + radius, r->y + radius, radius, 0, M_PI * 2, FALSE);
  vgcanvas_stroke(vg);
  vgcanvas_restore(vg);
  return RET_OK;
}

static ret_t slide_indicator_default_paint_indicator(widget_t* widget, canvas_t* c, rect_t* r,
                                                     color_t color) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  switch (slide_indicator->default_paint) {
    case INDICATOR_DEFAULT_PAINT_STROKE_DOT:
      slide_indicator_stroke_dot(c, r, color);
      break;
    case INDICATOR_DEFAULT_PAINT_STROKE_RECT:
      slide_indicator_stroke_rect(c, r, color);
      break;
    case INDICATOR_DEFAULT_PAINT_FILL_RECT:
      slide_indicator_fill_rect(c, r, color);
      break;
    default:
      slide_indicator_fill_dot(c, r, color);
  }

  return RET_OK;
}

static ret_t slide_indicator_default_paint_active_indicator(widget_t* widget, canvas_t* c,
                                                            rect_t* r, color_t color) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  switch (slide_indicator->default_paint) {
    case INDICATOR_DEFAULT_PAINT_STROKE_RECT:
    case INDICATOR_DEFAULT_PAINT_FILL_RECT:
      slide_indicator_fill_rect(c, r, color);
      break;
    default:
      slide_indicator_fill_dot(c, r, color);
  }

  return RET_OK;
}

static ret_t slide_indicator_paint_one(widget_t* widget, canvas_t* c, rect_t* r, bool_t active) {
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  color_t color =
      style_get_color(style, active ? STYLE_ID_SELECTED_FG_COLOR : STYLE_ID_FG_COLOR, trans);
  const char* icon = style_get_str(style, active ? STYLE_ID_ACTIVE_ICON : STYLE_ID_ICON, NULL);
  bitmap_t img;

  if (icon && widget_load_image(widget, icon, &img) == RET_OK) {
    int32_t x = r->x + (r->w >> 1);
    int32_t y = r->y + (r->h >> 1);
    canvas_draw_icon(c, &img, x, y);
  } else if (color.rgba.a) {
    if (active) {
      slide_indicator_default_paint_active_indicator(widget, c, r, color);
    } else {
      slide_indicator_default_paint_indicator(widget, c, r, color);
    }
  }

  return RET_OK;
}

static ret_t slide_indicator_paint_arc(widget_t* widget, canvas_t* c) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  bool_t ccw = FALSE;
  float_t cx = slide_indicator->anchor_x;
  float_t cy = slide_indicator->anchor_y;
  uint32_t nr = slide_indicator->max;
  float_t margin = slide_indicator->margin;
  float_t spacing = (slide_indicator->spacing / 180.0f * M_PI);
  uint32_t size = slide_indicator->size;
  float_t radius = _DISTANCE(cx, cy, widget->w / 2, widget->h / 2) - margin - size / 2;
  float_t center = _RADIAN(cx, cy, widget->w / 2, widget->h / 2);
  float_t offset = 0;
  uint32_t i = 0;
  rect_t r;

  if (cx > widget->w / 2 || cy < widget->h / 2) {
    ccw = TRUE;
  }

  if (nr % 2) {
    offset = ((nr / 2) * spacing);
  } else {
    offset = ((nr / 2) * spacing - spacing / 2);
  }
  offset = ccw ? (center - offset - 3 * M_PI / 2) : (center + offset + M_PI / 2);

  r = rect_init(0, 0, size, size);

  for (i = 0; i < nr; i++) {
    if (ccw) {
      r.x = cx + radius * sin(offset + i * spacing) - size / 2;
      r.y = cy + radius * cos(-offset - i * spacing) - size / 2;
    } else {
      r.x = cx + radius * sin(offset - i * spacing) - size / 2;
      r.y = cy + radius * cos(-offset + i * spacing) - size / 2;
    }
    slide_indicator_paint_one(widget, c, &r, i == slide_indicator->value);
  }

  return RET_OK;
}

static ret_t slide_indicator_paint_linear(widget_t* widget, canvas_t* c) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  bool_t ccw = FALSE;
  float_t cx = slide_indicator->anchor_x;
  float_t cy = slide_indicator->anchor_y;
  uint32_t nr = slide_indicator->max;
  uint32_t margin = slide_indicator->margin;
  uint32_t spacing = slide_indicator->spacing;
  uint32_t size = slide_indicator->size;
  uint32_t half_size = size >> 1;
  bool_t vertical = widget->w < widget->h;
  uint32_t offset = 0;
  uint32_t i = 0;
  rect_t r;

  if (cx > widget->w / 2 || cy < widget->h / 2) {
    ccw = TRUE;
  }

  if (nr % 2) {
    offset = ((nr / 2) * spacing + half_size);
  } else {
    offset = ((nr / 2) * spacing - spacing / 2 + half_size);
  }

  if (vertical) {
    if (ccw) {
      r = rect_init(widget->w - margin - size, cy - offset, size, size);
    } else {
      r = rect_init(margin, cy - offset, size, size);
    }
  } else {
    if (ccw) {
      r = rect_init(cx - offset, margin, size, size);
    } else {
      r = rect_init(cx - offset, widget->h - margin - size, size, size);
    }
  }

  for (i = 0; i < nr; i++) {
    slide_indicator_paint_one(widget, c, &r, i == slide_indicator->value);
    if (vertical) {
      r.y += spacing;
    } else {
      r.x += spacing;
    }
  }

  return RET_OK;
}

static ret_t slide_indicator_on_animate_end(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(widget != NULL && slide_indicator != NULL, RET_REMOVE);

  slide_indicator->wa_opactiy = NULL;
  widget_set_visible(widget, FALSE, FALSE);
  widget_set_opacity(widget, 0xff);
  return RET_REMOVE;
}

static ret_t slide_indicator_set_visible(widget_t* widget, bool_t visible) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(widget != NULL && slide_indicator != NULL, RET_REMOVE);

  if (slide_indicator->wa_opactiy != NULL) {
    widget_animator_destroy(slide_indicator->wa_opactiy);
    slide_indicator->wa_opactiy = NULL;
  }

  if (!visible && slide_indicator->auto_hide) {
    slide_indicator->wa_opactiy =
        widget_animator_opacity_create(widget, 1000, slide_indicator->auto_hide, EASING_SIN_INOUT);
    widget_animator_on(slide_indicator->wa_opactiy, EVT_ANIM_END, slide_indicator_on_animate_end,
                       slide_indicator);
    widget_animator_opacity_set_params(slide_indicator->wa_opactiy, 0xff, 0);
    widget_set_opacity(widget, 0xff);
    widget_animator_start(slide_indicator->wa_opactiy);
  } else {
    widget_set_visible(widget, visible, FALSE);
  }

  return RET_OK;
}

static ret_t slide_indicator_target_on_value_changed(void* ctx, event_t* e) {
  value_t v;
  widget_t* widget = WIDGET(e->target);
  widget_t* indicator = WIDGET(ctx);
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(indicator);
  return_value_if_fail(widget != NULL && slide_indicator != NULL, RET_BAD_PARAMS);

  if (widget_get_prop(widget, WIDGET_PROP_ACTIVE, &v) != RET_OK) {
    widget_get_prop(widget, WIDGET_PROP_VALUE, &v);
  }
  widget_set_prop(indicator, WIDGET_PROP_VALUE, &v);

  if (slide_indicator->auto_hide) {
    slide_indicator_set_visible(indicator, FALSE);
  }

  return RET_OK;
}

static ret_t slide_indicator_target_on_move_resize(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  widget_t* indicator = WIDGET(ctx);
  return_value_if_fail(widget != NULL && indicator != NULL, RET_BAD_PARAMS);
  slide_indicator_fix_anchor(indicator);
  return widget_invalidate_force(indicator, NULL);
}

static ret_t slide_indicator_target_on_pointer_down(void* ctx, event_t* e) {
  widget_t* indicator = WIDGET(ctx);
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(indicator);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  if (slide_indicator->auto_hide) {
    slide_indicator_set_visible(indicator, TRUE);
  }

  return RET_OK;
}

static ret_t slide_indicator_target_on_pointer_down_abort(void* ctx, event_t* e) {
  widget_t* indicator = WIDGET(ctx);
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(indicator);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  if (slide_indicator->auto_hide) {
    slide_indicator_set_visible(indicator, FALSE);
  }

  return RET_OK;
}

static ret_t slide_indicator_target_on_pointer_up(void* ctx, event_t* e) {
  widget_animator_t* wa = NULL;
  widget_t* widget = WIDGET(e->target);
  widget_t* indicator = WIDGET(ctx);
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(indicator);
  return_value_if_fail(widget != NULL && slide_indicator != NULL, RET_BAD_PARAMS);

  wa = widget_animator_manager_find(widget_animator_manager(), widget, NULL);
  if (wa == NULL) {
    if (slide_indicator->auto_hide) {
      slide_indicator_set_visible(indicator, FALSE);
    }
  }

  return RET_OK;
}

static ret_t slide_indicator_target_on_destroy(void* ctx, event_t* e) {
  widget_t* indicator = WIDGET(ctx);
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(indicator);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator->indicated_widget = NULL;

  return RET_OK;
}

static ret_t slide_indicator_reset_indicated_widget(widget_t* widget) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  if (slide_indicator->indicated_widget != NULL) {
    widget_t* target = slide_indicator->indicated_widget;
    widget_off_by_func(target, EVT_MOVE, slide_indicator_target_on_move_resize, widget);
    widget_off_by_func(target, EVT_MOVE_RESIZE, slide_indicator_target_on_move_resize, widget);
    widget_off_by_func(target, EVT_RESIZE, slide_indicator_target_on_move_resize, widget);
    widget_off_by_func(target, EVT_VALUE_CHANGED, slide_indicator_target_on_value_changed, widget);
    widget_off_by_func(target, EVT_DESTROY, slide_indicator_target_on_destroy, widget);
    if (slide_indicator->auto_hide) {
      widget_off_by_func(target, EVT_POINTER_DOWN, slide_indicator_target_on_pointer_down, widget);
      widget_off_by_func(target, EVT_POINTER_DOWN_ABORT,
                         slide_indicator_target_on_pointer_down_abort, widget);
      widget_off_by_func(target, EVT_POINTER_UP, slide_indicator_target_on_pointer_up, widget);
    }

    slide_indicator->indicated_widget = NULL;
  }

  return RET_OK;
}

static ret_t slide_indicator_set_indicated_widget(widget_t* widget, widget_t* target) {
  value_t v;
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL && target != NULL, RET_BAD_PARAMS);

  slide_indicator->indicated_widget = target;

  if (widget_get_prop(target, WIDGET_PROP_MAX, &v) != RET_OK) {
    value_set_uint32(&v, widget_count_children(target));
    slide_indicator->chilren_indicated = TRUE;
  }
  slide_indicator->max = value_uint32(&v);

  if (widget_get_prop(target, WIDGET_PROP_ACTIVE, &v) != RET_OK) {
    widget_get_prop(target, WIDGET_PROP_VALUE, &v);
  }
  slide_indicator_set_value(widget, value_uint32(&v));

  widget_on(target, EVT_MOVE, slide_indicator_target_on_move_resize, widget);
  widget_on(target, EVT_MOVE_RESIZE, slide_indicator_target_on_move_resize, widget);
  widget_on(target, EVT_RESIZE, slide_indicator_target_on_move_resize, widget);
  widget_on(target, EVT_VALUE_CHANGED, slide_indicator_target_on_value_changed, widget);
  widget_on(target, EVT_DESTROY, slide_indicator_target_on_destroy, widget);
  if (slide_indicator->auto_hide) {
    widget_on(target, EVT_POINTER_DOWN, slide_indicator_target_on_pointer_down, widget);
    widget_on(target, EVT_POINTER_DOWN_ABORT, slide_indicator_target_on_pointer_down_abort, widget);
    widget_on(target, EVT_POINTER_UP, slide_indicator_target_on_pointer_up, widget);
  }

  slide_indicator_fix_anchor(widget);

  return RET_OK;
}

static ret_t slide_indicator_init_if_not_inited(widget_t* widget) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  if (!(slide_indicator->inited)) {
    if (slide_indicator->auto_hide) {
      widget_set_visible(widget, TRUE, FALSE);
      slide_indicator_set_visible(widget, FALSE);
    }

    slide_indicator->inited = TRUE;
  }

  return RET_OK;
}

ret_t slide_indicator_on_paint_begin(widget_t* widget, canvas_t* c) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(widget != NULL && slide_indicator != NULL, RET_BAD_PARAMS);

  if (slide_indicator->chilren_indicated) {
    slide_indicator->max = widget_count_children(slide_indicator->indicated_widget);
  }

  return RET_OK;
}

static ret_t slide_indicator_on_paint_self(widget_t* widget, canvas_t* c) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(widget != NULL && slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator_init_if_not_inited(widget);
  slide_indicator_paint_linear(widget, c);
  return RET_OK;
}

static ret_t slide_indicator_arc_on_paint_self(widget_t* widget, canvas_t* c) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(widget != NULL && slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator_init_if_not_inited(widget);
  slide_indicator_paint_arc(widget, c);
  return RET_OK;
}

ret_t slide_indicator_on_destroy(widget_t* widget) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(widget != NULL && slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator_reset_indicated_widget(widget);
  TKMEM_FREE(slide_indicator->indicated_target);

  return RET_OK;
}

static const char* s_slide_indicator_properties[] = {WIDGET_PROP_VALUE,
                                                     WIDGET_PROP_MAX,
                                                     SLIDE_INDICATOR_PROP_DEFAULT_PAINT,
                                                     SLIDE_INDICATOR_PROP_AUTO_HIDE,
                                                     SLIDE_INDICATOR_PROP_INDICATED_TARGET,
                                                     WIDGET_PROP_MARGIN,
                                                     WIDGET_PROP_SPACING,
                                                     SLIDE_INDICATOR_PROP_SIZE,
                                                     WIDGET_PROP_ANCHOR_X,
                                                     WIDGET_PROP_ANCHOR_Y,
                                                     NULL};

TK_DECL_VTABLE(slide_indicator_linear) = {.size = sizeof(slide_indicator_t),
                                          .type = WIDGET_TYPE_SLIDE_INDICATOR,
                                          .clone_properties = s_slide_indicator_properties,
                                          .persistent_properties = s_slide_indicator_properties,
                                          .parent = TK_PARENT_VTABLE(widget),
                                          .create = slide_indicator_create,
                                          .get_prop = slide_indicator_get_prop,
                                          .set_prop = slide_indicator_set_prop,
                                          .on_layout_children = slide_indicator_on_layout_children,
                                          .on_paint_begin = slide_indicator_on_paint_begin,
                                          .on_paint_self = slide_indicator_on_paint_self,
                                          .on_destroy = slide_indicator_on_destroy};

TK_DECL_VTABLE(slide_indicator_arc) = {.size = sizeof(slide_indicator_t),
                                       .type = WIDGET_TYPE_SLIDE_INDICATOR_ARC,
                                       .clone_properties = s_slide_indicator_properties,
                                       .persistent_properties = s_slide_indicator_properties,
                                       .parent = TK_PARENT_VTABLE(widget),
                                       .create = slide_indicator_create_arc,
                                       .get_prop = slide_indicator_get_prop,
                                       .set_prop = slide_indicator_set_prop,
                                       .on_layout_children = slide_indicator_on_layout_children,
                                       .on_paint_begin = slide_indicator_on_paint_begin,
                                       .on_paint_self = slide_indicator_arc_on_paint_self,
                                       .on_destroy = slide_indicator_on_destroy};

/*create*/

widget_t* slide_indicator_create_internal(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h,
                                          const widget_vtable_t* vt) {
  widget_t* widget = widget_create(parent, vt, x, y, w, h);
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, NULL);

  slide_indicator->inited = FALSE;
  slide_indicator->default_paint = INDICATOR_DEFAULT_PAINT_AUTO;
  slide_indicator->auto_hide = 0;
  slide_indicator->margin = 0;
  slide_indicator->spacing = 16;
  slide_indicator->size = 8;
  slide_indicator->anchor_x = 0;
  slide_indicator->anchor_y = 0;
  slide_indicator->wa_opactiy = NULL;
  slide_indicator->anchor_x_fixed = FALSE;
  slide_indicator->anchor_y_fixed = FALSE;
  slide_indicator->chilren_indicated = FALSE;
  slide_indicator->indicated_widget = NULL;
  slide_indicator->indicated_target =
      tk_str_copy(slide_indicator->indicated_target, WIDGET_TYPE_SLIDE_VIEW);

  return widget;
}

widget_t* slide_indicator_create_linear(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return slide_indicator_create_internal(parent, x, y, w, h, TK_REF_VTABLE(slide_indicator_linear));
}

widget_t* slide_indicator_create_arc(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget =
      slide_indicator_create_internal(parent, x, y, w, h, TK_REF_VTABLE(slide_indicator_arc));
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, NULL);

  slide_indicator->spacing = 5;
  return widget;
}

widget_t* slide_indicator_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return slide_indicator_create_linear(parent, x, y, w, h);
}

ret_t slide_indicator_set_value(widget_t* widget, uint32_t value) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  if (slide_indicator->value != value) {
    event_t evt = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &evt);
    slide_indicator->value = value;
    evt = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &evt);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t slide_indicator_set_max(widget_t* widget, uint32_t max) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator->max = max;
  slide_indicator->chilren_indicated = FALSE;
  return widget_invalidate(widget, NULL);
}

ret_t slide_indicator_set_default_paint(widget_t* widget, indicator_default_paint_t default_paint) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator->default_paint = default_paint;
  widget_invalidate(widget, NULL);

  return RET_OK;
}

ret_t slide_indicator_set_auto_hide(widget_t* widget, uint16_t auto_hide) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator->auto_hide = auto_hide;
  return RET_OK;
}

ret_t slide_indicator_set_margin(widget_t* widget, int32_t margin) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator->margin = margin;
  return widget_invalidate(widget, NULL);
}

ret_t slide_indicator_set_spacing(widget_t* widget, float_t spacing) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator->spacing = spacing;
  return widget_invalidate(widget, NULL);
}

ret_t slide_indicator_set_size(widget_t* widget, uint32_t size) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator->size = size;
  return widget_invalidate(widget, NULL);
}

ret_t slide_indicator_set_anchor(widget_t* widget, const char* anchor_x, const char* anchor_y) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator_set_anchor_x(widget, anchor_x);
  slide_indicator_set_anchor_y(widget, anchor_y);
  return widget_invalidate(widget, NULL);
}

ret_t slide_indicator_set_indicated_target(widget_t* widget, const char* indicated_target) {
  slide_indicator_t* slide_indicator = SLIDE_INDICATOR(widget);
  return_value_if_fail(slide_indicator != NULL, RET_BAD_PARAMS);

  slide_indicator_reset_indicated_widget(widget);
  TKMEM_FREE(slide_indicator->indicated_target);

  slide_indicator->indicated_target =
      tk_str_copy(slide_indicator->indicated_target, indicated_target);

  slide_indicator->inited = FALSE;

  return RET_OK;
}

widget_t* slide_indicator_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, slide_indicator_linear) ||
                           WIDGET_IS_INSTANCE_OF(widget, slide_indicator_arc),
                       NULL);

  return widget;
}
