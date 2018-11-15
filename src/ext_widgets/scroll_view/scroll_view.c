/**
 * File:   scroll_view.c
 * Author: AWTK Develop Team
 * Brief:  scroll_view
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
 * 2018-07-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/layout.h"
#include "base/velocity.h"
#include "base/time_now.h"
#include "scroll_view/scroll_view.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"
#include "widget_animators/widget_animator_scroll.h"

#define ANIMATING_TIME 500

ret_t scroll_view_invalidate(widget_t* widget, rect_t* r) {
  rect_t r1 = *r;
  rect_t r2 = rect_init(0, 0, widget->w, widget->h);
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);

  r1.x -= scroll_view->xoffset;
  r1.y -= scroll_view->yoffset;
  *r = rect_intersect(&r1, &r2);

  if (r->w <= 0 || r->h <= 0) {
    return RET_OK;
  }

  r->x += widget->x;
  r->y += widget->y;

  if (widget->parent) {
    widget_invalidate(widget->parent, r);
  }

  return RET_OK;
}

ret_t scroll_view_invalidate_self(widget_t* widget) {
  rect_t r = rect_init(widget->x, widget->y, widget->w, widget->h);

  widget->dirty = FALSE;
  return widget_invalidate(widget->parent, &r);
}

static ret_t scroll_view_update_virtual_size(widget_t* widget) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  int32_t virtual_w = tk_max(scroll_view->virtual_w, widget->w);
  int32_t virtual_h = tk_max(scroll_view->virtual_h, widget->h);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  int32_t r = iter->x + iter->w;
  int32_t b = iter->y + iter->h;
  if (r > virtual_w) {
    virtual_w = r;
  }
  if (b > virtual_h) {
    virtual_h = b;
  }
  WIDGET_FOR_EACH_CHILD_END();

  scroll_view->virtual_w = virtual_w;
  scroll_view->virtual_h = virtual_h;

  return RET_OK;
}

static ret_t scroll_view_on_layout_children(widget_t* widget) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);

  if (scroll_view->on_layout_children) {
    scroll_view->on_layout_children(widget);
  } else {
    widget_layout_children_default(widget);
  }

  return scroll_view_update_virtual_size(widget);
}

static ret_t scroll_view_on_pointer_down(scroll_view_t* scroll_view, pointer_event_t* e) {
  velocity_t* v = &(scroll_view->velocity);

  velocity_reset(v);
  scroll_view->down.x = e->x;
  scroll_view->down.y = e->y;
  scroll_view->xoffset_save = scroll_view->xoffset;
  scroll_view->yoffset_save = scroll_view->yoffset;

  velocity_update(v, e->e.time, e->x, e->y);

  return RET_OK;
}

static ret_t scroll_view_on_scroll_done(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  scroll_view_t* scroll_view = SCROLL_VIEW(ctx);

  scroll_view->wa = NULL;
  scroll_view_invalidate_self(widget);

  return RET_REMOVE;
}

static ret_t scroll_view_fix_end_offset_default(widget_t* widget) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  int32_t xoffset_end = scroll_view->xoffset_end;
  int32_t yoffset_end = scroll_view->yoffset_end;

  xoffset_end = tk_max(xoffset_end, 0);
  yoffset_end = tk_max(yoffset_end, 0);
  xoffset_end = tk_min(xoffset_end, (scroll_view->virtual_w - widget->w));
  yoffset_end = tk_min(yoffset_end, (scroll_view->virtual_h - widget->h));

  if (scroll_view->virtual_w <= widget->w) {
    xoffset_end = 0;
  }

  if (scroll_view->virtual_h <= widget->h) {
    yoffset_end = 0;
  }

  scroll_view->xoffset_end = xoffset_end;
  scroll_view->yoffset_end = yoffset_end;

  return RET_OK;
}

ret_t scroll_view_scroll_to(widget_t* widget, int32_t xoffset_end, int32_t yoffset_end,
                            int32_t duration) {
  int32_t xoffset = 0;
  int32_t yoffset = 0;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL, RET_FAIL);

  xoffset = scroll_view->xoffset;
  yoffset = scroll_view->yoffset;
  if (xoffset == xoffset_end && yoffset == yoffset_end) {
    return RET_OK;
  }

  scroll_view->xoffset_end = xoffset_end;
  scroll_view->yoffset_end = yoffset_end;
  if (scroll_view->fix_end_offset) {
    scroll_view->fix_end_offset(widget);
    xoffset_end = scroll_view->xoffset_end;
    yoffset_end = scroll_view->yoffset_end;
  }

  if (scroll_view->on_scroll_to) {
    scroll_view->on_scroll_to(widget, xoffset_end, yoffset_end, duration);
  }

  if (scroll_view->wa != NULL) {
    widget_animator_scroll_t* wa = (widget_animator_scroll_t*)scroll_view->wa;
    if (scroll_view->xslidable) {
      if (wa->x_to > 0 && wa->x_to < (scroll_view->virtual_w - widget->w)) {
        wa->x_to = xoffset_end;
        wa->x_from = scroll_view->xoffset;
      }
    }

    if (scroll_view->yslidable) {
      if (wa->y_to > 0 && wa->y_to < (scroll_view->virtual_h - widget->h)) {
        wa->y_to = yoffset_end;
        wa->y_from = scroll_view->yoffset;
      }
    }
    
    wa->base.now = 0;
    wa->base.start_time = 0;
  } else {
    scroll_view->wa = widget_animator_scroll_create(widget, ANIMATING_TIME, 0, EASING_SIN_INOUT);
    return_value_if_fail(scroll_view->wa != NULL, RET_OOM);

    widget_animator_scroll_set_params(scroll_view->wa, xoffset, yoffset, xoffset_end, yoffset_end);
    widget_animator_on(scroll_view->wa, EVT_ANIM_END, scroll_view_on_scroll_done, scroll_view);
    widget_animator_start(scroll_view->wa);
  }

  return RET_OK;
}

#define SPEED_SCALE 2
#define MIN_DELTA 10

static ret_t scroll_view_on_pointer_up(scroll_view_t* scroll_view, pointer_event_t* e) {
  widget_t* widget = WIDGET(scroll_view);
  velocity_t* v = &(scroll_view->velocity);
  int32_t move_dx = e->x - scroll_view->down.x;
  int32_t move_dy = e->y - scroll_view->down.y;

  velocity_update(v, e->e.time, e->x, e->y);
  if (scroll_view->xslidable || scroll_view->yslidable) {
    int yv = v->yv;
    int xv = v->xv;

    if (scroll_view->wa != NULL) {
      widget_animator_scroll_t* wa = (widget_animator_scroll_t*)scroll_view->wa;
      int32_t dx = wa->x_to - scroll_view->xoffset;
      int32_t dy = wa->y_to - scroll_view->yoffset;
      xv -= dx;
      yv -= dy;
      log_debug("dx=%d xv=%d\n", dx, xv);
    }

    if (scroll_view->xslidable) {
      if (tk_abs(move_dx) > MIN_DELTA) {
        scroll_view->xoffset_end = scroll_view->xoffset - xv * SPEED_SCALE;
      } else {
        scroll_view->xoffset_end = scroll_view->xoffset - xv / SPEED_SCALE;
      }
    }

    if (scroll_view->yslidable) {
      if (tk_abs(move_dy) > MIN_DELTA) {
        scroll_view->yoffset_end = scroll_view->yoffset - yv * SPEED_SCALE;
      } else {
        scroll_view->yoffset_end = scroll_view->yoffset - yv / SPEED_SCALE;
      }
    }

    scroll_view_scroll_to(widget, scroll_view->xoffset_end, scroll_view->yoffset_end,
                          ANIMATING_TIME);
  }

  return RET_OK;
}

static ret_t scroll_view_abort_pointer_down(scroll_view_t* scroll_view, pointer_event_t* e) {
  widget_t* widget = WIDGET(scroll_view);
  if (scroll_view->first_move_after_down && widget->target) {
    pointer_event_t abort = *e;
    abort.e.type = EVT_POINTER_DOWN_ABORT;
    widget_dispatch(widget->target, (event_t*)(&abort));
    scroll_view->first_move_after_down = FALSE;
  }

  return RET_OK;
}

static ret_t scroll_view_on_pointer_move(scroll_view_t* scroll_view, pointer_event_t* e) {
  widget_t* widget = WIDGET(scroll_view);
  velocity_t* v = &(scroll_view->velocity);
  int32_t dx = e->x - scroll_view->down.x;
  int32_t dy = e->y - scroll_view->down.y;
  velocity_update(v, e->e.time, e->x, e->y);

  if (scroll_view->wa == NULL) {
    if (scroll_view->xslidable && dx) {
      scroll_view->xoffset = scroll_view->xoffset_save - dx;
      scroll_view_abort_pointer_down(scroll_view, e);
    }

    if (scroll_view->yslidable && dy) {
      scroll_view->yoffset = scroll_view->yoffset_save - dy;
      scroll_view_abort_pointer_down(scroll_view, e);
    }

    if (scroll_view->on_scroll) {
      scroll_view->on_scroll(widget, scroll_view->xoffset, scroll_view->yoffset);
    }
  }
  scroll_view->first_move_after_down = FALSE;

  return RET_OK;
}

static ret_t scroll_view_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);

  switch (type) {
    case EVT_POINTER_DOWN:
      widget_grab(widget->parent, widget);
      scroll_view->first_move_after_down = TRUE;
      scroll_view_on_pointer_down(scroll_view, (pointer_event_t*)e);
      break;
    case EVT_POINTER_UP: {
      pointer_event_t* evt = (pointer_event_t*)e;
      int32_t dx = evt->x - scroll_view->down.x;
      int32_t dy = evt->y - scroll_view->down.y;
      if (dx || dy) {
        scroll_view_on_pointer_up(scroll_view, (pointer_event_t*)e);
      }
      widget_ungrab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (evt->pressed && (scroll_view->xslidable || scroll_view->yslidable)) {
        scroll_view_on_pointer_move(scroll_view, evt);
        scroll_view_invalidate_self(widget);
      }
      break;
    }
    default:
      break;
  }

  return RET_OK;
}

static ret_t scroll_view_on_paint_children(widget_t* widget, canvas_t* c) {
  rect_t r_save;
  rect_t r = rect_init(c->ox, c->oy, widget->w, widget->h);

  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  int32_t xoffset = -scroll_view->xoffset;
  int32_t yoffset = -scroll_view->yoffset;

  canvas_translate(c, xoffset, yoffset);
  canvas_get_clip_rect(c, &r_save);

  r = rect_intersect(&r, &r_save);
  canvas_set_clip_rect(c, &r);
  widget_on_paint_children_default(widget, c);
  canvas_set_clip_rect(c, &r_save);
  canvas_untranslate(c, xoffset, yoffset);

  return RET_OK;
}

static ret_t scroll_view_get_prop(widget_t* widget, const char* name, value_t* v) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_W) || tk_str_eq(name, WIDGET_PROP_LAYOUT_W)) {
    value_set_int(v, tk_max(widget->w, scroll_view->virtual_w));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_H) || tk_str_eq(name, WIDGET_PROP_LAYOUT_H)) {
    value_set_int(v, tk_max(widget->h, scroll_view->virtual_h));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    value_set_int(v, scroll_view->xoffset);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    value_set_int(v, scroll_view->yoffset);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XSLIDABLE)) {
    value_set_bool(v, scroll_view->xslidable);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YSLIDABLE)) {
    value_set_bool(v, scroll_view->yslidable);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t scroll_view_set_prop(widget_t* widget, const char* name, const value_t* v) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_W)) {
    scroll_view->virtual_w = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_H)) {
    scroll_view->virtual_h = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XSLIDABLE)) {
    scroll_view->xslidable = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YSLIDABLE)) {
    scroll_view->yslidable = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    scroll_view->xoffset = value_int(v);
    scroll_view_invalidate_self(widget);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    scroll_view->yoffset = value_int(v);
    scroll_view_invalidate_self(widget);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const char* s_scroll_view_clone_properties[] = {WIDGET_PROP_VIRTUAL_W,
                                                       WIDGET_PROP_VIRTUAL_H,
                                                       WIDGET_PROP_XSLIDABLE,
                                                       WIDGET_PROP_YSLIDABLE,
                                                       WIDGET_PROP_XOFFSET,
                                                       WIDGET_PROP_YOFFSET,
                                                       NULL};
static const widget_vtable_t s_scroll_view_vtable = {
    .size = sizeof(scroll_view_t),
    .type = WIDGET_TYPE_SCROLL_VIEW,
    .clone_properties = s_scroll_view_clone_properties,
    .create = scroll_view_create,
    .on_event = scroll_view_on_event,
    .invalidate = scroll_view_invalidate,
    .on_layout_children = scroll_view_on_layout_children,
    .on_paint_children = scroll_view_on_paint_children,
    .get_prop = scroll_view_get_prop,
    .set_prop = scroll_view_set_prop};

widget_t* scroll_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  scroll_view_t* scroll_view = TKMEM_ZALLOC(scroll_view_t);
  widget_t* widget = WIDGET(scroll_view);
  return_value_if_fail(scroll_view != NULL, NULL);

  widget_init(widget, parent, &s_scroll_view_vtable, x, y, w, h);
  scroll_view->fix_end_offset = scroll_view_fix_end_offset_default;

  return widget;
}

ret_t scroll_view_set_virtual_w(widget_t* widget, wh_t w) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  scroll_view->virtual_w = w;

  return RET_OK;
}

ret_t scroll_view_set_virtual_h(widget_t* widget, wh_t h) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  scroll_view->virtual_h = h;

  return RET_OK;
}

ret_t scroll_view_set_offset(widget_t* widget, int32_t xoffset, int32_t yoffset) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL, RET_FAIL);

  scroll_view->xoffset = xoffset;
  scroll_view->yoffset = yoffset;

  scroll_view_invalidate_self(widget);

  return RET_OK;
}

ret_t scroll_view_set_xslidable(widget_t* widget, bool_t xslidable) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL, RET_FAIL);

  scroll_view->xslidable = xslidable;

  return RET_OK;
}

ret_t scroll_view_set_yslidable(widget_t* widget, bool_t yslidable) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL, RET_FAIL);

  scroll_view->yslidable = yslidable;

  return RET_OK;
}

widget_t* scroll_view_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_scroll_view_vtable, NULL);

  return widget;
}
