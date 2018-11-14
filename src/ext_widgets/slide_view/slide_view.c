/**
 * File:   slide_view.h
 * Author: AWTK Develop Team
 * Brief:  slide_view
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-06-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/timer.h"
#include "base/easing.h"
#include "slide_view/slide_view.h"
#include "widget_animators/widget_animator_scroll.h"

#define ANIMATING_TIME 500

static ret_t slide_view_invalidate(slide_view_t* slide_view);

static ret_t canvas_set_clip_rect_with_offset(canvas_t* c, rect_t* r, int ox, int oy) {
  rect_t rr = *r;
  rr.x += ox;
  rr.y += oy;
  rr.w -= 1;
  rr.h -= 1;

  return canvas_set_clip_rect(c, &rr);
}

widget_t* slide_view_get_prev(slide_view_t* slide_view) {
  widget_t* widget = WIDGET(slide_view);
  uint32_t active = slide_view->active;
  uint32_t nr = widget_count_children(widget);

  if (active > 0) {
    return widget_get_child(widget, active - 1);
  } else if (slide_view->auto_play && nr > 1) {
    return widget_get_child(widget, nr - 1);
  } else {
    return NULL;
  }
}

widget_t* slide_view_get_next(slide_view_t* slide_view) {
  widget_t* widget = WIDGET(slide_view);
  uint32_t active = slide_view->active;
  uint32_t nr = widget_count_children(widget);

  if ((active + 1) < nr) {
    return widget_get_child(widget, active + 1);
  } else if (slide_view->auto_play && nr > 1) {
    return widget_get_child(widget, 0);
  } else {
    return NULL;
  }
}

ret_t slide_view_activate_prev(slide_view_t* slide_view) {
  widget_t* widget = WIDGET(slide_view);
  uint32_t active = slide_view->active;
  uint32_t nr = widget_count_children(widget);

  if (active > 0) {
    return slide_view_set_active(widget, active - 1);
  } else if (slide_view->auto_play && nr > 1) {
    return slide_view_set_active(widget, nr - 1);
  } else {
    return RET_FAIL;
  }
}

ret_t slide_view_activate_next(slide_view_t* slide_view) {
  widget_t* widget = WIDGET(slide_view);
  uint32_t active = slide_view->active;
  uint32_t nr = widget_count_children(widget);

  if ((active + 1) < nr) {
    return slide_view_set_active(widget, active + 1);
  } else if (slide_view->auto_play && nr > 1) {
    return slide_view_set_active(widget, 0);
  } else {
    return RET_FAIL;
  }
}

static ret_t slide_view_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t slide_view_on_pointer_down(slide_view_t* slide_view, pointer_event_t* e) {
  velocity_t* v = &(slide_view->velocity);

  velocity_reset(v);
  slide_view->down.x = e->x;
  slide_view->down.y = e->y;
  velocity_update(v, e->e.time, e->x, e->y);

  return RET_OK;
}

static ret_t slide_view_on_scroll_done(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  slide_view_t* slide_view = SLIDE_VIEW(ctx);

  if (slide_view->xoffset > 0 || slide_view->yoffset > 0) {
    slide_view_activate_prev(slide_view);
  } else if (slide_view->xoffset < 0 || slide_view->yoffset < 0) {
    slide_view_activate_next(slide_view);
  }

  slide_view->xoffset = 0;
  slide_view->yoffset = 0;
  slide_view->animating = FALSE;

  widget->dirty = FALSE;
  widget_invalidate(widget, NULL);

  return RET_OK;
}

static ret_t slide_view_on_pointer_up(slide_view_t* slide_view, pointer_event_t* e) {
  uint32_t v_threshhold = 100;
  widget_t* widget = WIDGET(slide_view);
  velocity_t* v = &(slide_view->velocity);
  widget_animator_t* animator =
      widget_animator_scroll_create(widget, ANIMATING_TIME, 0, EASING_SIN_INOUT);

  velocity_update(v, e->e.time, e->x, e->y);
  if (slide_view->vertical) {
    int h = widget->h;
    int yoffset_end = 0;
    int yv = tk_abs(v->yv);
    int yoffset = slide_view->yoffset;
    bool_t rollback = (yv < v_threshhold) && (tk_abs(yoffset) < h / 2);

    if (yoffset > 0 && slide_view_get_prev(slide_view) == NULL) {
      rollback = TRUE;
    } else if (yoffset < 0 && slide_view_get_next(slide_view) == NULL) {
      rollback = TRUE;
    }

    if (!rollback) {
      yoffset_end = yoffset > 0 ? h : -h;
    }

    widget_animator_scroll_set_params(animator, 0, yoffset, 0, yoffset_end);
  } else {
    int w = widget->w;
    int xoffset_end = 0;
    int xv = tk_abs(v->xv);
    int xoffset = slide_view->xoffset;
    bool_t rollback = (xv < v_threshhold) && (tk_abs(xoffset) < w / 2);

    if (xoffset > 0 && slide_view_get_prev(slide_view) == NULL) {
      rollback = TRUE;
    } else if (xoffset < 0 && slide_view_get_next(slide_view) == NULL) {
      rollback = TRUE;
    }

    if (!rollback) {
      xoffset_end = xoffset > 0 ? w : -w;
    }

    widget_animator_scroll_set_params(animator, xoffset, 0, xoffset_end, 0);
  }

  slide_view->animating = TRUE;
  widget_animator_on(animator, EVT_ANIM_END, slide_view_on_scroll_done, slide_view);
  widget_animator_start(animator);

  return RET_OK;
}

static ret_t slide_view_on_pointer_move(slide_view_t* slide_view, pointer_event_t* e) {
  velocity_t* v = &(slide_view->velocity);
  velocity_update(v, e->e.time, e->x, e->y);

  if (slide_view->vertical) {
    slide_view->xoffset = 0;
    slide_view->yoffset = (e->y - slide_view->down.y);
  } else {
    slide_view->yoffset = 0;
    slide_view->xoffset = (e->x - slide_view->down.x);
  }

  return RET_OK;
}

static ret_t slide_view_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  slide_view_t* slide_view = SLIDE_VIEW(widget);

  if (slide_view->animating) {
    return RET_OK;
  }

  switch (type) {
    case EVT_POINTER_DOWN:
      widget_grab(widget->parent, widget);
      slide_view_on_pointer_down(slide_view, (pointer_event_t*)e);
      break;
    case EVT_POINTER_UP: {
      if (slide_view->xoffset || slide_view->yoffset) {
        slide_view_on_pointer_up(slide_view, (pointer_event_t*)e);
      }
      widget_ungrab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (evt->pressed) {
        slide_view_on_pointer_move(slide_view, evt);
        slide_view_invalidate(slide_view);
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static widget_t* slide_view_find_target(widget_t* widget, xy_t x, xy_t y) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  if (slide_view->xoffset || slide_view->yoffset) {
    return NULL;
  }

  return widget_get_child(widget, slide_view->active);
}

static ret_t slide_view_get_prop(widget_t* widget, const char* name, value_t* v) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE) || tk_str_eq(name, WIDGET_PROP_ACTIVE)) {
    value_set_int(v, slide_view->active);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    value_set_bool(v, slide_view->vertical);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_PLAY)) {
    value_set_int(v, slide_view->auto_play);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    value_set_int(v, slide_view->xoffset);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    value_set_int(v, slide_view->yoffset);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t widget_calc_children_rect(widget_t* widget, rect_t* r) {
  uint32_t i = 0;
  uint32_t nr = widget != NULL ? widget_count_children(widget) : 0;

  memset(r, 0x0, sizeof(rect_t));

  for (i = 0; i < nr; i++) {
    rect_t rc;
    widget_t* iter = widget_get_child(widget, i);
    rc = rect_init(iter->x, iter->y, iter->w, iter->h);
    rect_merge(r, &rc);
  }

  return RET_OK;
}

static ret_t slide_view_calc_dirty_rect(slide_view_t* slide_view, rect_t* r) {
  rect_t r_next;
  rect_t r_prev;
  rect_t r_active;
  int xoffset = slide_view->xoffset;
  int yoffset = slide_view->yoffset;
  widget_t* widget = WIDGET(slide_view);
  widget_t* active = widget_get_child(widget, slide_view->active);
  widget_t* next = slide_view_get_next(slide_view);
  widget_t* prev = slide_view_get_prev(slide_view);

  memset(r, 0x00, sizeof(rect_t));
  widget_calc_children_rect(active, &r_active);
  widget_calc_children_rect(next, &r_next);
  widget_calc_children_rect(prev, &r_prev);

  if (xoffset == 0 && yoffset == 0) {
    *r = r_active;
  } else if (xoffset) {
    /*FIXME: optimize*/
    r_active.x = 0;
    r_active.w = widget->w;

    rect_merge(r, &r_active);
    if (xoffset < 0) {
      if (next) {
        r_next.x = 0;
        r_next.w = widget->w;
        rect_merge(r, &r_next);
      }
    } else {
      if (prev) {
        r_prev.x = 0;
        r_prev.w = widget->w;
        rect_merge(r, &r_prev);
      }
    }
  } else if (yoffset) {
    r_active.y = 0;
    r_active.h = widget->h;

    rect_merge(r, &r_active);
    rect_merge(r, &r_next);
  }

  return RET_OK;
}

static ret_t slide_view_invalidate(slide_view_t* slide_view) {
  rect_t r;
  widget_t* widget = WIDGET(slide_view);
  slide_view_calc_dirty_rect(slide_view, &r);

  if (r.x < 0) {
    r.x = 0;
  }

  if (r.x >= widget->w) {
    r.x = 0;
    r.w = 0;
  }

  if (r.y < 0) {
    r.y = 0;
  }

  if (r.y >= widget->h) {
    r.y = 0;
    r.h = 0;
  }

  if ((r.x + r.w) > widget->w) {
    r.w = widget->w - r.x;
  }

  if ((r.y + r.h) > widget->h) {
    r.h = widget->h - r.y;
  }

  return widget_invalidate(widget, &r);
}

static ret_t slide_view_set_prop(widget_t* widget, const char* name, const value_t* v) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE) || tk_str_eq(name, WIDGET_PROP_ACTIVE)) {
    return slide_view_set_active(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    return slide_view_set_vertical(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    slide_view->xoffset = value_int(v);
    slide_view_invalidate(slide_view);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_PLAY)) {
    return slide_view_set_auto_play(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    slide_view->yoffset = value_int(v);
    slide_view_invalidate(slide_view);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t slide_view_paint_children_v_gt(slide_view_t* slide_view, canvas_t* c) {
  rect_t r;
  int ox = c->ox;
  int oy = c->oy;
  xy_t w = WIDGET(slide_view)->w;
  xy_t h = WIDGET(slide_view)->h;
  int yoffset = slide_view->yoffset;
  int r_yoffset = h - slide_view->yoffset;
  widget_t* prev = slide_view_get_prev(slide_view);
  widget_t* active = widget_get_child(WIDGET(slide_view), slide_view->active);

  r = rect_init(0, 0, w, yoffset);
  if (prev) {
    canvas_translate(c, 0, -r_yoffset);
    canvas_set_clip_rect_with_offset(c, &r, ox, oy);
    widget_paint(prev, c);
    canvas_untranslate(c, 0, -r_yoffset);
  }

  canvas_translate(c, 0, yoffset);
  r = rect_init(0, yoffset, w, r_yoffset);
  canvas_set_clip_rect_with_offset(c, &r, ox, oy);
  widget_paint(active, c);
  canvas_untranslate(c, 0, yoffset);

  return RET_OK;
}

static ret_t slide_view_paint_children_v_lt(slide_view_t* slide_view, canvas_t* c) {
  rect_t r;
  int ox = c->ox;
  int oy = c->oy;
  xy_t w = WIDGET(slide_view)->w;
  xy_t h = WIDGET(slide_view)->h;
  int yoffset = tk_abs(slide_view->yoffset);
  int r_yoffset = h - tk_abs(slide_view->yoffset);
  widget_t* next = slide_view_get_next(slide_view);
  widget_t* active = widget_get_child(WIDGET(slide_view), slide_view->active);

  canvas_translate(c, 0, -yoffset);
  r = rect_init(0, 0, w, r_yoffset);
  canvas_set_clip_rect_with_offset(c, &r, ox, oy);
  widget_paint(active, c);
  canvas_untranslate(c, 0, -yoffset);

  r = rect_init(0, r_yoffset, w, yoffset);
  canvas_set_clip_rect_with_offset(c, &r, ox, oy);
  if (next) {
    canvas_translate(c, 0, r_yoffset);
    widget_paint(next, c);
    canvas_untranslate(c, 0, r_yoffset);
  }

  return RET_OK;
}

static ret_t slide_view_paint_children_h_gt(slide_view_t* slide_view, canvas_t* c) {
  rect_t r;
  int ox = c->ox;
  int oy = c->oy;
  xy_t w = WIDGET(slide_view)->w;
  xy_t h = WIDGET(slide_view)->h;
  int xoffset = slide_view->xoffset;
  int r_xoffset = w - slide_view->xoffset;
  widget_t* prev = slide_view_get_prev(slide_view);
  widget_t* active = widget_get_child(WIDGET(slide_view), slide_view->active);

  r = rect_init(0, 0, xoffset, h);
  if (prev) {
    canvas_translate(c, -r_xoffset, 0);
    canvas_set_clip_rect_with_offset(c, &r, ox, oy);
    widget_paint(prev, c);
    canvas_untranslate(c, -r_xoffset, 0);
  }

  canvas_translate(c, xoffset, 0);
  r = rect_init(xoffset, 0, r_xoffset, h);
  canvas_set_clip_rect_with_offset(c, &r, ox, oy);
  widget_paint(active, c);
  canvas_untranslate(c, xoffset, 0);

  return RET_OK;
}

static ret_t slide_view_paint_children_h_lt(slide_view_t* slide_view, canvas_t* c) {
  rect_t r;
  int ox = c->ox;
  int oy = c->oy;
  xy_t w = WIDGET(slide_view)->w;
  xy_t h = WIDGET(slide_view)->h;
  int xoffset = tk_abs(slide_view->xoffset);
  int r_xoffset = w - tk_abs(slide_view->xoffset);
  widget_t* next = slide_view_get_next(slide_view);
  widget_t* active = widget_get_child(WIDGET(slide_view), slide_view->active);

  canvas_translate(c, -xoffset, 0);
  r = rect_init(0, 0, r_xoffset, h);
  canvas_set_clip_rect_with_offset(c, &r, ox, oy);
  widget_paint(active, c);
  canvas_untranslate(c, -xoffset, 0);

  r = rect_init(r_xoffset, 0, xoffset, h);
  canvas_set_clip_rect_with_offset(c, &r, ox, oy);
  if (next) {
    canvas_translate(c, r_xoffset, 0);
    widget_paint(next, c);
    canvas_untranslate(c, r_xoffset, 0);
  }

  return RET_OK;
}

static ret_t slide_view_paint_indicator_one(widget_t* widget, canvas_t* c, rect_t* r,
                                            uint32_t index, bool_t is_active) {
  bitmap_t img;
  style_t* style = widget->astyle;
  color_t trans = color_init(0, 0, 0, 0);
  color_t fg = style_get_color(style, STYLE_ID_FG_COLOR, trans);
  const char* icon = style_get_str(style, STYLE_ID_ICON, NULL);
  const char* active_icon = style_get_str(style, STYLE_ID_ACTIVE_ICON, NULL);

  if (is_active) {
    if (active_icon && widget_load_image(widget, active_icon, &img) == RET_OK) {
      int32_t x = r->x + (r->w >> 1);
      int32_t y = r->y + (r->h >> 1);
      canvas_draw_icon(c, &img, x, y);
    } else if (fg.rgba.a) {
      canvas_set_fill_color(c, fg);
      canvas_fill_rect(c, r->x, r->y, r->w, r->h);
    }
  } else {
    if (icon && widget_load_image(widget, icon, &img) == RET_OK) {
      int32_t x = r->x + (r->w >> 1);
      int32_t y = r->y + (r->h >> 1);
      canvas_draw_icon(c, &img, x, y);
    } else if (fg.rgba.a) {
      canvas_set_stroke_color(c, fg);
      canvas_stroke_rect(c, r->x, r->y, r->w, r->h);
    }
  }

  return RET_OK;
}

static ret_t slide_view_paint_indicator(widget_t* widget, canvas_t* c) {
  rect_t r;
  uint32_t i = 0;
  uint32_t size = 8;
  uint32_t offset = 0;
  int cx = widget->w >> 1;
  int cy = widget->h >> 1;
  uint32_t half_size = size >> 1;
  uint32_t double_size = size << 1;
  uint32_t nr = widget_count_children(widget);
  slide_view_t* slide_view = SLIDE_VIEW(widget);

  if (nr % 2) {
    offset = ((nr / 2) * double_size + half_size);
  } else {
    offset = ((nr / 2 - 1) * double_size + half_size);
  }

  if (slide_view->vertical) {
    r = rect_init(tk_max(10, widget->w * 0.1), cy - offset, size, size);
  } else {
    r = rect_init(cx - offset, tk_min(widget->h - double_size, widget->h * 0.9), size, size);
  }

  for (i = 0; i < nr; i++) {
    slide_view_paint_indicator_one(widget, c, &r, i, i == slide_view->active);
    if (slide_view->vertical) {
      r.y += double_size;
    } else {
      r.x += double_size;
    }
  }

  return RET_OK;
}

static ret_t slide_view_on_paint_children(widget_t* widget, canvas_t* c) {
  rect_t save_r;
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  widget_t* active = widget_get_child(widget, slide_view->active);

  return_value_if_fail(active != NULL, RET_BAD_PARAMS);

  canvas_get_clip_rect(c, &save_r);
  if (slide_view->vertical) {
    if (slide_view->yoffset > 0) {
      slide_view_paint_children_v_gt(slide_view, c);
    } else if (slide_view->yoffset < 0) {
      slide_view_paint_children_v_lt(slide_view, c);
    } else {
      widget_paint(active, c);
    }
  } else {
    if (slide_view->xoffset > 0) {
      slide_view_paint_children_h_gt(slide_view, c);
    } else if (slide_view->xoffset < 0) {
      slide_view_paint_children_h_lt(slide_view, c);
    } else {
      widget_paint(active, c);
    }
  }
  canvas_set_clip_rect(c, &save_r);
  slide_view_paint_indicator(widget, c);

  return RET_OK;
}

static ret_t slide_view_destroy(widget_t* widget) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  if (slide_view->timer_id) {
    timer_remove(slide_view->timer_id);
    slide_view->timer_id = 0;
  }

  return RET_OK;
}

static const char* s_slide_view_properties[] = {WIDGET_PROP_VALUE,     WIDGET_PROP_VERTICAL,
                                                WIDGET_PROP_XOFFSET,   WIDGET_PROP_YOFFSET,
                                                WIDGET_PROP_AUTO_PLAY, NULL};
static const widget_vtable_t s_slide_view_vtable = {
    .size = sizeof(slide_view_t),
    .type = WIDGET_TYPE_SLIDE_VIEW,
    .clone_properties = s_slide_view_properties,
    .persistent_properties = s_slide_view_properties,
    .create = slide_view_create,
    .on_event = slide_view_on_event,
    .get_prop = slide_view_get_prop,
    .set_prop = slide_view_set_prop,
    .find_target = slide_view_find_target,
    .on_paint_children = slide_view_on_paint_children,
    .on_paint_self = slide_view_on_paint_self,
    .destroy = slide_view_destroy};

ret_t slide_view_set_active(widget_t* widget, uint32_t active) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(slide_view != NULL, RET_BAD_PARAMS);

  if (slide_view->active != active) {
    event_t evt = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &evt);
    slide_view->active = active;
    evt = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &evt);
    widget_invalidate(widget, NULL);
  }

  return RET_OK;
}

ret_t slide_view_set_vertical(widget_t* widget, bool_t vertical) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(slide_view != NULL, RET_BAD_PARAMS);

  slide_view->vertical = vertical;
  widget_invalidate(widget, NULL);

  return RET_OK;
}

widget_t* slide_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  slide_view_t* slide_view = TKMEM_ZALLOC(slide_view_t);
  widget_t* widget = WIDGET(slide_view);
  return_value_if_fail(slide_view != NULL, NULL);

  return widget_init(widget, parent, &s_slide_view_vtable, x, y, w, h);
}

static ret_t slide_view_on_timer_next(const timer_info_t* timer) {
  widget_t* widget = WIDGET(timer->ctx);
  slide_view_t* slide_view = SLIDE_VIEW(timer->ctx);

  widget_animator_t* animator =
      widget_animator_scroll_create(widget, ANIMATING_TIME, 0, EASING_SIN_INOUT);

  if (slide_view->vertical) {
    widget_animator_scroll_set_params(animator, 0, 0, 0, -widget->h);
  } else {
    widget_animator_scroll_set_params(animator, 0, 0, -widget->w, 0);
  }

  slide_view->animating = TRUE;
  widget_animator_on(animator, EVT_ANIM_END, slide_view_on_scroll_done, slide_view);
  widget_animator_start(animator);

  return RET_REPEAT;
}

ret_t slide_view_set_auto_play(widget_t* widget, uint16_t auto_play) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(slide_view != NULL, RET_BAD_PARAMS);

  slide_view->auto_play = auto_play;
  if (slide_view->timer_id) {
    timer_remove(slide_view->timer_id);
    slide_view->timer_id = 0;
  }

  if (auto_play) {
    slide_view->timer_id =
        timer_add(slide_view_on_timer_next, slide_view, auto_play + ANIMATING_TIME);

    return slide_view->timer_id ? RET_OK : RET_OOM;
  }

  return RET_OK;
}

widget_t* slide_view_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_slide_view_vtable, NULL);

  return widget;
}
