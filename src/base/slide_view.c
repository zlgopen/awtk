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
#include "base/easing.h"
#include "base/slide_view.h"
#include "widget_animators/widget_animator_scroll.h"

static widget_t* slide_view_get_next(slide_view_t* slide_view) {
  widget_t* widget = WIDGETP(slide_view);
  uint32_t active = slide_view->active;
  uint32_t nr = widget_count_children(widget);

  return (active + 1) < nr ? widget_get_child(widget, active + 1) : NULL;
}

static widget_t* slide_view_get_prev(slide_view_t* slide_view) {
  widget_t* widget = WIDGETP(slide_view);
  uint32_t active = slide_view->active;

  return active > 0 ? widget_get_child(widget, active - 1) : NULL;
}

static ret_t slide_view_activate_next(slide_view_t* slide_view) {
  widget_t* widget = WIDGETP(slide_view);
  return_value_if_fail((slide_view->active + 1) < widget_count_children(widget), RET_BAD_PARAMS);

  return slide_view_set_active(widget, slide_view->active + 1);
}

static ret_t slide_view_activate_prev(slide_view_t* slide_view) {
  widget_t* widget = WIDGETP(slide_view);
  return_value_if_fail(slide_view->active > 0, RET_BAD_PARAMS);

  return slide_view_set_active(widget, slide_view->active - 1);
}
static ret_t slide_view_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t slide_view_on_pointer_down(slide_view_t* slide_view, pointer_event_t* e) {
  velocity_t* v = &(slide_view->velocity);

  slide_view->down.x = e->x;
  slide_view->down.y = e->y;
  velocity_reset(v);
  velocity_update(v, e->e.time, e->x, e->y);

  return RET_OK;
}

static ret_t slide_view_on_scroll_event(void* ctx, event_t* e) {
  if (e->type == EVT_ANIM_END) {
    slide_view_t* slide_view = SLIDE_VIEW(ctx);
    if (slide_view->vertical) {
    } else {
      if (slide_view->xoffset > 0) {
        slide_view_activate_prev(slide_view);
      } else if (slide_view->xoffset < 0) {
        slide_view_activate_next(slide_view);
      }
    }

    slide_view->xoffset = 0;
    slide_view->yoffset = 0;
  }

  return RET_OK;
}

static ret_t slide_view_on_pointer_up(slide_view_t* slide_view, pointer_event_t* e) {
  widget_t* widget = WIDGETP(slide_view);
  velocity_t* v = &(slide_view->velocity);

  velocity_update(v, e->e.time, e->x, e->y);
  if (slide_view->vertical) {
  } else {
    int xoffset = slide_view->xoffset;
    int xoffset_end = xoffset > 0 ? widget->w : -widget->w;
    widget_animator_t* animator = widget_animator_scroll_create(widget, 500, 0, EASING_SIN_INOUT);
    widget_animator_scroll_set_params(animator, xoffset, 0, xoffset_end, 0);
    widget_animator_on(animator, EVT_ANIM_END, slide_view_on_scroll_event, slide_view);
    widget_animator_start(animator);
  }

  return RET_OK;
}

static ret_t slide_view_on_pointer_move(slide_view_t* slide_view, pointer_event_t* e) {
  velocity_t* v = &(slide_view->velocity);

  velocity_update(v, e->e.time, e->x, e->y);
  slide_view->xoffset =
      slide_view->vertical ? (e->y - slide_view->down.y) : (e->x - slide_view->down.x);

  return RET_OK;
}

static ret_t slide_view_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  switch (type) {
    case EVT_POINTER_DOWN:
      slide_view_on_pointer_down(slide_view, (pointer_event_t*)e);
      break;
    case EVT_POINTER_UP: {
      if (slide_view->xoffset || slide_view->yoffset) {
        slide_view_on_pointer_up(slide_view, (pointer_event_t*)e);
      }
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (evt->pressed) {
        slide_view_on_pointer_move(slide_view, evt);
        widget_invalidate(widget, NULL);
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

  return widget_get_child(widget, slide_view->active);
}

static ret_t slide_view_get_prop(widget_t* widget, const char* name, value_t* v) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (str_fast_equal(name, WIDGET_PROP_VALUE) || str_fast_equal(name, WIDGET_PROP_ACTIVE)) {
    value_set_int(v, slide_view->active);
    return RET_OK;
  } else if (str_fast_equal(name, WIDGET_PROP_VERTICAL)) {
    value_set_bool(v, slide_view->vertical);
  } else if (str_fast_equal(name, WIDGET_PROP_XOFFSET)) {
    value_set_int(v, slide_view->xoffset);
  } else if (str_fast_equal(name, WIDGET_PROP_YOFFSET)) {
    value_set_int(v, slide_view->yoffset);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t slide_view_set_prop(widget_t* widget, const char* name, const value_t* v) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (str_fast_equal(name, WIDGET_PROP_VALUE) || str_fast_equal(name, WIDGET_PROP_ACTIVE)) {
    return slide_view_set_active(widget, value_int(v));
  } else if (str_fast_equal(name, WIDGET_PROP_VERTICAL)) {
    return slide_view_set_vertical(widget, value_bool(v));
  } else if (str_fast_equal(name, WIDGET_PROP_XOFFSET)) {
    slide_view->xoffset = value_int(v);
    widget_invalidate(widget, NULL);
  } else if (str_fast_equal(name, WIDGET_PROP_YOFFSET)) {
    slide_view->yoffset = value_int(v);
    widget_invalidate(widget, NULL);
  }

  return RET_NOT_FOUND;
}

static ret_t slide_view_paint_children_v_gt(slide_view_t* slide_view, canvas_t* c) {
  int xoffset = slide_view->xoffset;
  widget_t* prev = slide_view_get_prev(slide_view);
  widget_t* active = widget_get_child(WIDGETP(slide_view), slide_view->active);

  return RET_OK;
}

static ret_t slide_view_paint_children_v_lt(slide_view_t* slide_view, canvas_t* c) {
  int xoffset = slide_view->xoffset;
  widget_t* next = slide_view_get_next(slide_view);
  widget_t* active = widget_get_child(WIDGETP(slide_view), slide_view->active);

  return RET_OK;
}

static ret_t slide_view_paint_children_h_gt(slide_view_t* slide_view, canvas_t* c) {
  rect_t r;
  widget_t* widget = WIDGETP(slide_view);
  xy_t x = widget->x;
  xy_t y = widget->y;
  xy_t w = widget->w;
  xy_t h = widget->h;
  int xoffset = slide_view->xoffset;
  int r_xoffset = w - xoffset;
  widget_t* prev = slide_view_get_prev(slide_view);
  widget_t* active = widget_get_child(WIDGETP(slide_view), slide_view->active);

  rect_init(r, x, y, xoffset, h);
  if (prev) {
    canvas_translate(c, -r_xoffset, 0);
    canvas_set_clip_rect(c, &r);
    widget_paint(prev, c);
    canvas_translate(c, r_xoffset, 0);
  } else {
    canvas_fill_rect(c, r.x, r.y, xoffset, r.h);
  }

  canvas_translate(c, xoffset, 0);
  rect_init(r, xoffset, y, r_xoffset, h);
  canvas_set_clip_rect(c, &r);
  widget_paint(active, c);
  canvas_translate(c, -xoffset, 0);

  return RET_OK;
}

static ret_t slide_view_paint_children_h_lt(slide_view_t* slide_view, canvas_t* c) {
  rect_t r;
  widget_t* widget = WIDGETP(slide_view);
  xy_t x = widget->x;
  xy_t y = widget->y;
  xy_t w = widget->w;
  xy_t h = widget->h;
  int xoffset = -slide_view->xoffset;
  int r_xoffset = w - xoffset;
  widget_t* next = slide_view_get_next(slide_view);
  widget_t* active = widget_get_child(WIDGETP(slide_view), slide_view->active);

  canvas_translate(c, -xoffset, 0);
  rect_init(r, 0, y, r_xoffset, h);
  canvas_set_clip_rect(c, &r);
  widget_paint(active, c);
  canvas_translate(c, xoffset, 0);

  rect_init(r, x + r_xoffset, y, xoffset, h);
  canvas_set_clip_rect(c, &r);
  if (next) {
    canvas_translate(c, r_xoffset, 0);
    widget_paint(next, c);
    canvas_translate(c, -r_xoffset, 0);
  } else {
    canvas_fill_rect(c, r.x + r_xoffset, r.y, xoffset, r.h);
  }

  return RET_OK;
}

static ret_t slide_view_on_paint_children(widget_t* widget, canvas_t* c) {
  rect_t save_r;
  int xoffset = 0;
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  widget_t* active = widget_get_child(widget, slide_view->active);

  return_value_if_fail(active != NULL, RET_BAD_PARAMS);

  canvas_get_clip_rect(c, &save_r);
  xoffset = slide_view->xoffset;
  if (xoffset == 0) {
    widget_paint(active, c);
  } else if (xoffset > 0) {
    if (slide_view->vertical) {
      slide_view_paint_children_v_gt(slide_view, c);
    } else {
      slide_view_paint_children_h_gt(slide_view, c);
    }
  } else {
    if (slide_view->vertical) {
      slide_view_paint_children_v_lt(slide_view, c);
    } else {
      slide_view_paint_children_h_lt(slide_view, c);
    }
  }
  canvas_set_clip_rect(c, &save_r);

  return RET_OK;
}

static const widget_vtable_t s_slide_view_vtable = {
    .on_event = slide_view_on_event,
    .get_prop = slide_view_get_prop,
    .set_prop = slide_view_set_prop,
    .find_target = slide_view_find_target,
    .on_paint_children = slide_view_on_paint_children,
    .on_paint_self = slide_view_on_paint_self};

ret_t slide_view_set_active(widget_t* widget, uint32_t active) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(slide_view != NULL, RET_BAD_PARAMS);

  slide_view->active = active;
  widget_invalidate(widget, NULL);

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
  widget_t* widget = NULL;
  slide_view_t* slide_view = TKMEM_ZALLOC(slide_view_t);
  return_value_if_fail(slide_view != NULL, NULL);

  widget = WIDGETP(slide_view);
  widget_init(widget, parent, WIDGET_SLIDE_VIEW);
  widget_move_resize(widget, x, y, w, h);
  widget->vt = &s_slide_view_vtable;

  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return widget;
}
