/**
 * File:   scroll_view.c
 * Author: AWTK Develop Team
 * Brief:  scroll_view
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/layout.h"
#include "base/velocity.h"
#include "tkc/time_now.h"
#include "scroll_view/scroll_view.h"
#include "base/widget_vtable.h"
#include "base/image_manager.h"
#include "widget_animators/widget_animator_scroll.h"

#define SCROLL_VIEW_DEFAULT_XSPEED_SCALE 2.0f
#define SCROLL_VIEW_DEFAULT_YSPEED_SCALE 2.0f

static uint32_t scroll_view_get_curr_page(widget_t* widget);
static uint32_t scroll_view_get_page_max_number(widget_t* widget);

static ret_t scroll_view_get_item_rect(widget_t* parent, widget_t* widget, rect_t* item_rect) {
  rect_t r;
  point_t p;

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  memset(&p, 0x0, sizeof(point_t));
  widget_to_screen_ex(iter, parent, &p);
  r = rect_init(p.x, p.y, iter->w, iter->h);
  rect_merge(item_rect, &r);
  scroll_view_get_item_rect(parent, iter, item_rect);
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

static ret_t scroll_view_update_virtual_size(widget_t* widget) {
  int32_t virtual_w = 0;
  int32_t virtual_h = 0;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL && widget != NULL, RET_BAD_PARAMS);

  virtual_w = tk_max(scroll_view->virtual_w, widget->w);
  virtual_h = tk_max(scroll_view->virtual_h, widget->h);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  int32_t r = 0;
  int32_t b = 0;
  if (scroll_view->recursive) {
    rect_t rect = rect_init(0, 0, iter->w, iter->h);
    scroll_view_get_item_rect(iter, iter, &rect);
    r = iter->x + rect.x + rect.w;
    b = iter->y + rect.y + rect.h;
  } else {
    r = iter->x + iter->w;
    b = iter->y + iter->h;
  }
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
  return_value_if_fail(widget != NULL && scroll_view != NULL, RET_BAD_PARAMS);

  if (scroll_view->on_layout_children) {
    scroll_view->on_layout_children(widget);
  } else {
    widget_layout_children_default(widget);
    scroll_view_update_virtual_size(widget);
    scroll_view_set_offset(widget,
                           tk_min(scroll_view->xoffset, (scroll_view->virtual_w - widget->w)),
                           tk_min(scroll_view->yoffset, (scroll_view->virtual_h - widget->h)));
  }
  if (scroll_view->snap_to_page) {
    int32_t curr_page = scroll_view_get_curr_page(widget);
    uint32_t max_page = scroll_view_get_page_max_number(widget);
    scroll_view->xoffset_end = scroll_view->xoffset;
    scroll_view->yoffset_end = scroll_view->yoffset;
    scroll_view->fix_end_offset(widget);
    scroll_view->xoffset = scroll_view->xoffset_end;
    scroll_view->yoffset = scroll_view->yoffset_end;
    if (scroll_view->curr_page != curr_page || scroll_view->max_page != max_page) {
      scroll_view->max_page = max_page;
      scroll_view->curr_page = curr_page;
      widget_dispatch_simple_event(widget, EVT_PAGE_CHANGED);
    }
  }

  return RET_OK;
}

static ret_t scroll_view_on_pointer_down(scroll_view_t* scroll_view, pointer_event_t* e) {
  velocity_t* v = &(scroll_view->velocity);

  velocity_reset(v);
  scroll_view->down.x = e->x;
  scroll_view->down.y = e->y;
  scroll_view->xoffset_save = scroll_view->xoffset;
  scroll_view->yoffset_save = scroll_view->yoffset;
  scroll_view->xoffset_end = scroll_view->xoffset;
  scroll_view->yoffset_end = scroll_view->yoffset;

  velocity_update(v, e->e.time, e->x, e->y);

  return RET_OK;
}

static ret_t scroll_view_on_scroll_done(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  scroll_view_t* scroll_view = SCROLL_VIEW(ctx);
  return_value_if_fail(widget != NULL && scroll_view != NULL, RET_BAD_PARAMS);

  scroll_view->wa = NULL;
  scroll_view->curr_page = scroll_view_get_curr_page(widget);
  widget_invalidate_force(widget, NULL);
  widget_dispatch_simple_event(widget, EVT_SCROLL_END);
  widget_dispatch_simple_event(widget, EVT_PAGE_CHANGED);

  return RET_REMOVE;
}

static ret_t scroll_view_fix_end_offset_default(widget_t* widget) {
  int32_t xoffset_end = 0;
  int32_t yoffset_end = 0;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL && scroll_view != NULL, RET_BAD_PARAMS);

  xoffset_end = scroll_view->xoffset_end;
  yoffset_end = scroll_view->yoffset_end;

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

static int32_t scroll_view_get_snap_to_page_offset_value(widget_t* widget, int32_t offset_end) {
  uint32_t tmp = 0;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, offset_end);
  if (scroll_view->xslidable && !scroll_view->yslidable) {
    tmp = widget->w;
  } else if (!scroll_view->xslidable && scroll_view->yslidable) {
    tmp = widget->h;
  }
  if (tmp != 0) {
    int32_t n = (int32_t)((offset_end + tmp * 0.5f) / tmp);
    if (scroll_view->move_to_page) {
      if (scroll_view->curr_page - n > 1) {
        n = scroll_view->curr_page - 1;
      } else if (n - scroll_view->curr_page > 1) {
        n = scroll_view->curr_page + 1;
      }
    }
    offset_end = n * tmp;
  }
  return offset_end;
}

ret_t scroll_view_scroll_to(widget_t* widget, int32_t xoffset_end, int32_t yoffset_end,
                            int32_t duration) {
  int32_t xoffset = 0;
  int32_t yoffset = 0;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_FAIL);

  if (scroll_view->snap_to_page) {
    xoffset_end = scroll_view_get_snap_to_page_offset_value(widget, xoffset_end);
    yoffset_end = scroll_view_get_snap_to_page_offset_value(widget, yoffset_end);
  }

  if (scroll_view->fix_end_offset) {
    scroll_view->xoffset_end = xoffset_end;
    scroll_view->yoffset_end = yoffset_end;
    scroll_view->fix_end_offset(widget);
    xoffset_end = scroll_view->xoffset_end;
    yoffset_end = scroll_view->yoffset_end;
  }

  if (xoffset_end == scroll_view->xoffset && yoffset_end == scroll_view->yoffset) {
    return RET_OK;
  }

  xoffset = scroll_view->xoffset;
  yoffset = scroll_view->yoffset;

  if (scroll_view->on_scroll_to) {
    scroll_view->on_scroll_to(widget, xoffset_end, yoffset_end, duration);
  }

#ifndef WITHOUT_WIDGET_ANIMATORS
  if (scroll_view->wa != NULL) {
    widget_animator_scroll_t* wa = (widget_animator_scroll_t*)scroll_view->wa;
    if (xoffset_end != scroll_view->xoffset) {
      bool_t changed = wa->x_to != xoffset_end;
      bool_t in_range = wa->x_to > 0 && wa->x_to < (scroll_view->virtual_w - widget->w);
      if (changed && in_range) {
        wa->x_to = xoffset_end;
        wa->x_from = scroll_view->xoffset;
      } else if (!scroll_view->yslidable) {
        return RET_OK;
      }
    }

    if (yoffset_end != scroll_view->yoffset) {
      bool_t changed = wa->y_to != yoffset_end;
      bool_t in_range = wa->y_to > 0 && wa->y_to < (scroll_view->virtual_h - widget->h);
      if (changed && in_range) {
        wa->y_to = yoffset_end;
        wa->y_from = scroll_view->yoffset;
      } else if (!scroll_view->xslidable) {
        return RET_OK;
      }
    }

    wa->base.now = 0;
    wa->base.start_time = 0;
  } else {
    scroll_view->wa = widget_animator_scroll_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT);
    return_value_if_fail(scroll_view->wa != NULL, RET_OOM);

    widget_animator_scroll_set_params(scroll_view->wa, xoffset, yoffset, xoffset_end, yoffset_end);
    widget_animator_on(scroll_view->wa, EVT_ANIM_END, scroll_view_on_scroll_done, scroll_view);
    widget_animator_start(scroll_view->wa);
    widget_dispatch_simple_event(widget, EVT_SCROLL_START);
  }
#else
  scroll_view->xoffset = xoffset_end;
  scroll_view->yoffset = yoffset_end;
  scroll_view_on_scroll_done(widget, NULL);
#endif /*WITHOUT_WIDGET_ANIMATORS*/
  return RET_OK;
}

ret_t scroll_view_scroll_delta_to(widget_t* widget, int32_t xoffset_delta, int32_t yoffset_delta,
                                  int32_t duration) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_FAIL);

  scroll_view->xoffset_end = scroll_view->xoffset + xoffset_delta;
  scroll_view->yoffset_end = scroll_view->yoffset + yoffset_delta;

  return scroll_view_scroll_to(widget, scroll_view->xoffset_end, scroll_view->yoffset_end,
                               duration);
}

static ret_t scroll_view_on_pointer_down_abort(scroll_view_t* scroll_view, pointer_event_t* e) {
  widget_t* widget = WIDGET(scroll_view);
  (void)e;

  if (scroll_view->xslidable || scroll_view->yslidable) {
    scroll_view_scroll_to(widget, scroll_view->xoffset_end, scroll_view->yoffset_end,
                          TK_ANIMATING_TIME);
  }

  return RET_OK;
}

static ret_t scroll_view_on_pointer_up(scroll_view_t* scroll_view, pointer_event_t* e) {
  widget_t* widget = WIDGET(scroll_view);
  velocity_t* v = &(scroll_view->velocity);
  int32_t move_dx = e->x - scroll_view->down.x;
  int32_t move_dy = e->y - scroll_view->down.y;

  velocity_update(v, e->e.time, e->x, e->y);
  if (scroll_view->xslidable || scroll_view->yslidable) {
#ifndef WITHOUT_WIDGET_ANIMATORS
    int yv = v->yv;
    int xv = v->xv;
#else
    int yv = 0;
    int xv = 0;
#endif /*WITHOUT_WIDGET_ANIMATORS*/
    if (scroll_view->wa != NULL) {
      widget_animator_scroll_t* wa = (widget_animator_scroll_t*)scroll_view->wa;
      int32_t dx = wa->x_to - scroll_view->xoffset;
      int32_t dy = wa->y_to - scroll_view->yoffset;
      xv -= dx;
      yv -= dy;
      log_debug("dx=%d xv=%d dy=%d yv=%d\n", dx, xv, dy, yv);
    }

    if (scroll_view->xslidable) {
      if (tk_abs(move_dx) > TK_CLICK_TOLERANCE) {
        scroll_view->xoffset_end = scroll_view->xoffset - xv * scroll_view->xspeed_scale;
      } else {
        scroll_view->xoffset_end = scroll_view->xoffset - xv / scroll_view->xspeed_scale;
      }
    }

    if (scroll_view->yslidable) {
      if (tk_abs(move_dy) > TK_CLICK_TOLERANCE) {
        scroll_view->yoffset_end = scroll_view->yoffset - yv * scroll_view->yspeed_scale;
      } else {
        scroll_view->yoffset_end = scroll_view->yoffset - yv / scroll_view->yspeed_scale;
      }
    }

    scroll_view_scroll_to(widget, scroll_view->xoffset_end, scroll_view->yoffset_end,
                          TK_ANIMATING_TIME);
  }

  return RET_OK;
}

static ret_t scroll_view_notify_scrolled(scroll_view_t* scroll_view) {
  widget_t* widget = WIDGET(scroll_view);

  if (scroll_view->on_scroll) {
    scroll_view->on_scroll(widget, scroll_view->xoffset, scroll_view->yoffset);
  }
  widget_dispatch_simple_event(widget, EVT_SCROLL);

  return RET_OK;
}

static ret_t scroll_view_on_pointer_move(scroll_view_t* scroll_view, pointer_event_t* e) {
  int32_t dx = 0;
  int32_t dy = 0;
  velocity_t* v = NULL;
  return_value_if_fail(scroll_view != NULL && e != NULL, RET_BAD_PARAMS);
  v = &(scroll_view->velocity);
  dx = e->x - scroll_view->down.x;
  dy = e->y - scroll_view->down.y;
  velocity_update(v, e->e.time, e->x, e->y);

  if (scroll_view->wa == NULL) {
    if (scroll_view->xslidable && dx) {
      scroll_view->xoffset = scroll_view->xoffset_save - dx;
    }

    if (scroll_view->yslidable && dy) {
      scroll_view->yoffset = scroll_view->yoffset_save - dy;
    }

    scroll_view_notify_scrolled(scroll_view);
  }
  scroll_view->first_move_after_down = FALSE;

  return RET_OK;
}

static bool_t scroll_view_is_dragged(widget_t* widget, pointer_event_t* evt) {
  int32_t delta = 0;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, FALSE);
  if (scroll_view->xslidable && scroll_view->yslidable) {
    int32_t xdelta = evt->x - scroll_view->down.x;
    int32_t ydelta = evt->y - scroll_view->down.y;
    delta = tk_abs(xdelta) > tk_abs(ydelta) ? xdelta : ydelta;
  } else if (scroll_view->yslidable) {
    delta = evt->y - scroll_view->down.y;
  } else {
    delta = evt->x - scroll_view->down.x;
  }

  return scroll_view->snap_to_page || (tk_abs(delta) >= TK_DRAG_THRESHOLD);
}

static ret_t scroll_view_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  uint16_t type = e->type;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN:
      scroll_view->pressed = TRUE;
      scroll_view->dragged = FALSE;
      widget_grab(widget->parent, widget);
      scroll_view->first_move_after_down = TRUE;
      scroll_view_on_pointer_down(scroll_view, (pointer_event_t*)e);
      break;
    case EVT_POINTER_DOWN_ABORT:
      scroll_view_on_pointer_down_abort(scroll_view, (pointer_event_t*)e);
      if (scroll_view->pressed) {
        widget_ungrab(widget->parent, widget);
      }
      scroll_view->pressed = FALSE;
      scroll_view->dragged = FALSE;
      break;
    case EVT_POINTER_UP: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (scroll_view->pressed && scroll_view_is_dragged(widget, evt)) {
        scroll_view_on_pointer_up(scroll_view, (pointer_event_t*)e);
      }
      scroll_view->pressed = FALSE;
      scroll_view->dragged = FALSE;
      widget_ungrab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (!scroll_view->pressed || !(scroll_view->xslidable || scroll_view->yslidable)) {
        break;
      }

      if (scroll_view->dragged) {
        scroll_view_on_pointer_move(scroll_view, evt);
        widget_invalidate_force(widget, NULL);
      } else {
        if (scroll_view_is_dragged(widget, evt)) {
          pointer_event_t abort = *evt;

          abort.e.type = EVT_POINTER_DOWN_ABORT;
          widget_dispatch_event_to_target_recursive(widget, (event_t*)(&abort));

          scroll_view->dragged = TRUE;
        }
      }

      ret = scroll_view->dragged ? RET_STOP : RET_OK;
      break;
    }
    default:
      break;
  }

  return ret;
}

static ret_t scroll_view_on_paint_children(widget_t* widget, canvas_t* c) {
  rect_t r;
  rect_t r_save;
  int32_t xoffset = 0;
  int32_t yoffset = 0;
  vgcanvas_t* vg = NULL;
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL && c != NULL && scroll_view != NULL, RET_BAD_PARAMS);
  vg = canvas_get_vgcanvas(c);
  xoffset = -scroll_view->xoffset;
  yoffset = -scroll_view->yoffset;
  r = rect_init(c->ox, c->oy, widget->w, widget->h);

  canvas_translate(c, xoffset, yoffset);
  canvas_get_clip_rect(c, &r_save);

  r = rect_intersect(&r, &r_save);

  if (vg != NULL) {
    vgcanvas_save(vg);
    vgcanvas_clip_rect(vg, (float_t)r.x, (float_t)r.y, (float_t)r.w, (float_t)r.h);
  }

  canvas_set_clip_rect(c, &r);
  if (scroll_view->on_paint_children) {
    scroll_view->on_paint_children(widget, c);
  } else {
    widget_on_paint_children_default(widget, c);
  }
  canvas_set_clip_rect(c, &r_save);
  canvas_untranslate(c, xoffset, yoffset);

  if (vg != NULL) {
    vgcanvas_clip_rect(vg, (float_t)r_save.x, (float_t)r_save.y, (float_t)r_save.w,
                       (float_t)r_save.h);
    vgcanvas_restore(vg);
  }
  return RET_OK;
}

static ret_t scroll_view_on_add_child(widget_t* widget, widget_t* child) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_BAD_PARAMS);

  if (scroll_view->on_add_child) {
    return scroll_view->on_add_child(widget, child);
  } else {
    return RET_CONTINUE;
  }
}

static widget_t* scroll_view_find_target(widget_t* widget, xy_t x, xy_t y) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, NULL);

  if (scroll_view->find_target) {
    return scroll_view->find_target(widget, x, y);
  } else {
    return widget_find_target_default(widget, x, y);
  }
}

static uint32_t scroll_view_get_page_max_number(widget_t* widget) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL && scroll_view != NULL, 0);
  if (scroll_view->xslidable && !scroll_view->yslidable) {
    return scroll_view->virtual_w / widget->w;
  } else if (!scroll_view->xslidable && scroll_view->yslidable) {
    return scroll_view->virtual_h / widget->h;
  }
  return 0;
}

static uint32_t scroll_view_get_curr_page(widget_t* widget) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL && scroll_view != NULL, 0);
  if (scroll_view->xslidable && !scroll_view->yslidable) {
    return scroll_view->xoffset / widget->w;
  } else if (!scroll_view->xslidable && scroll_view->yslidable) {
    return scroll_view->yoffset / widget->h;
  }
  return 0;
}

static ret_t scroll_view_set_curr_page(widget_t* widget, int32_t new_page) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(widget != NULL && scroll_view != NULL, RET_BAD_PARAMS);

  if (scroll_view->xslidable && !scroll_view->yslidable) {
    scroll_view->xoffset_end = new_page * widget->w;
  } else if (!scroll_view->xslidable && scroll_view->yslidable) {
    scroll_view->xoffset_end = new_page * widget->h;
  }
  scroll_view->snap_to_page = FALSE;
  scroll_view_scroll_to(widget, scroll_view->xoffset_end, scroll_view->yoffset_end,
                        TK_ANIMATING_TIME);
  scroll_view->snap_to_page = TRUE;
  return RET_OK;
}

static ret_t scroll_view_get_prop(widget_t* widget, const char* name, value_t* v) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

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
  } else if (tk_str_eq(name, SCROLL_VIEW_X_SPEED_SCALE)) {
    value_set_float(v, scroll_view->xspeed_scale);
    return RET_OK;
  } else if (tk_str_eq(name, SCROLL_VIEW_Y_SPEED_SCALE)) {
    value_set_float(v, scroll_view->yspeed_scale);
    return RET_OK;
  } else if (tk_str_eq(name, SCROLL_VIEW_RECURSIVE)) {
    value_set_bool(v, scroll_view->recursive);
    return RET_OK;
  } else if (tk_str_eq(name, SCROLL_VIEW_MOVE_TO_PAGE)) {
    value_set_bool(v, scroll_view->move_to_page);
    return RET_OK;
  } else if (tk_str_eq(name, SCROLL_VIEW_SNAP_TO_PAGE)) {
    value_set_bool(v, scroll_view->snap_to_page);
    return RET_OK;
  }
  if (scroll_view->snap_to_page) {
    if (tk_str_eq(name, WIDGET_PROP_PAGE_MAX_NUMBER)) {
      value_set_uint32(v, scroll_view_get_page_max_number(widget));
      return RET_OK;
    } else if (tk_str_eq(name, WIDGET_PROP_CURR_PAGE)) {
      value_set_uint32(v, scroll_view_get_curr_page(widget));
      return RET_OK;
    }
  }

  return RET_NOT_FOUND;
}

static ret_t scroll_view_set_prop(widget_t* widget, const char* name, const value_t* v) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

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
    scroll_view_notify_scrolled(scroll_view);
    widget_invalidate_force(widget, NULL);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    scroll_view->yoffset = value_int(v);
    scroll_view_notify_scrolled(scroll_view);
    widget_invalidate_force(widget, NULL);
    return RET_OK;
  } else if (tk_str_eq(name, SCROLL_VIEW_X_SPEED_SCALE)) {
    scroll_view->xspeed_scale = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, SCROLL_VIEW_Y_SPEED_SCALE)) {
    scroll_view->yspeed_scale = value_float(v);
    return RET_OK;
  } else if (tk_str_eq(name, SCROLL_VIEW_RECURSIVE)) {
    return scroll_view_set_recursive(widget, value_bool(v));
  } else if (tk_str_eq(name, SCROLL_VIEW_MOVE_TO_PAGE)) {
    return scroll_view_set_move_to_page(widget, value_bool(v));
  } else if (tk_str_eq(name, SCROLL_VIEW_SNAP_TO_PAGE)) {
    return scroll_view_set_snap_to_page(widget, value_bool(v));
  } else if (scroll_view->snap_to_page && tk_str_eq(name, WIDGET_PROP_CURR_PAGE)) {
    return scroll_view_set_curr_page(widget, value_int(v));
  }

  return RET_NOT_FOUND;
}

static const char* s_scroll_view_clone_properties[] = {
    WIDGET_PROP_VIRTUAL_W,     WIDGET_PROP_VIRTUAL_H,     WIDGET_PROP_XSLIDABLE,
    WIDGET_PROP_YSLIDABLE,     WIDGET_PROP_XOFFSET,       WIDGET_PROP_YOFFSET,
    SCROLL_VIEW_X_SPEED_SCALE, SCROLL_VIEW_Y_SPEED_SCALE, SCROLL_VIEW_RECURSIVE,
    SCROLL_VIEW_MOVE_TO_PAGE,  SCROLL_VIEW_SNAP_TO_PAGE,  NULL};
TK_DECL_VTABLE(scroll_view) = {.size = sizeof(scroll_view_t),
                               .type = WIDGET_TYPE_SCROLL_VIEW,
                               .scrollable = TRUE,
                               .clone_properties = s_scroll_view_clone_properties,
                               .parent = TK_PARENT_VTABLE(widget),
                               .create = scroll_view_create,
                               .on_event = scroll_view_on_event,
                               .on_layout_children = scroll_view_on_layout_children,
                               .on_paint_children = scroll_view_on_paint_children,
                               .on_add_child = scroll_view_on_add_child,
                               .find_target = scroll_view_find_target,
                               .get_prop = scroll_view_get_prop,
                               .set_prop = scroll_view_set_prop};

widget_t* scroll_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(scroll_view), x, y, w, h);
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, NULL);

  scroll_view->snap_to_page = FALSE;
  scroll_view->xspeed_scale = SCROLL_VIEW_DEFAULT_XSPEED_SCALE;
  scroll_view->yspeed_scale = SCROLL_VIEW_DEFAULT_YSPEED_SCALE;
  scroll_view->fix_end_offset = scroll_view_fix_end_offset_default;

  return widget;
}

ret_t scroll_view_set_virtual_w(widget_t* widget, wh_t w) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_BAD_PARAMS);

  scroll_view->virtual_w = w;

  return RET_OK;
}

ret_t scroll_view_set_virtual_h(widget_t* widget, wh_t h) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_BAD_PARAMS);

  scroll_view->virtual_h = h;

  return RET_OK;
}

ret_t scroll_view_set_speed_scale(widget_t* widget, float_t xspeed_scale, float_t yspeed_scale) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_FAIL);

  scroll_view->xspeed_scale = xspeed_scale;
  scroll_view->yspeed_scale = yspeed_scale;

  return RET_OK;
}

ret_t scroll_view_set_offset(widget_t* widget, int32_t xoffset, int32_t yoffset) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_FAIL);

  if (scroll_view->snap_to_page) {
    xoffset = scroll_view_get_snap_to_page_offset_value(widget, xoffset);
    yoffset = scroll_view_get_snap_to_page_offset_value(widget, yoffset);
  }

  scroll_view->xoffset = xoffset;
  scroll_view->yoffset = yoffset;

  widget_invalidate_force(widget, NULL);

  return RET_OK;
}

ret_t scroll_view_set_xslidable(widget_t* widget, bool_t xslidable) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_FAIL);

  scroll_view->xslidable = xslidable;

  return RET_OK;
}

ret_t scroll_view_set_yslidable(widget_t* widget, bool_t yslidable) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_FAIL);

  scroll_view->yslidable = yslidable;

  return RET_OK;
}

ret_t scroll_view_set_move_to_page(widget_t* widget, bool_t move_to_page) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_FAIL);

  scroll_view->move_to_page = move_to_page;
  return RET_OK;
}

ret_t scroll_view_set_snap_to_page(widget_t* widget, bool_t snap_to_page) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_FAIL);

  scroll_view->snap_to_page = snap_to_page;
  if (snap_to_page) {
    if (scroll_view->xspeed_scale == SCROLL_VIEW_DEFAULT_XSPEED_SCALE) {
      scroll_view->xspeed_scale /= 2;
    }
    if (scroll_view->yspeed_scale == SCROLL_VIEW_DEFAULT_YSPEED_SCALE) {
      scroll_view->yspeed_scale /= 2;
    }
  }

  return RET_OK;
}

ret_t scroll_view_set_recursive_only(widget_t* widget, bool_t recursive) {
  scroll_view_t* scroll_view = SCROLL_VIEW(widget);
  return_value_if_fail(scroll_view != NULL, RET_FAIL);

  scroll_view->recursive = recursive;
  return RET_OK;
}

ret_t scroll_view_set_recursive(widget_t* widget, bool_t recursive) {
  ret_t ret = RET_OK;
  ret = scroll_view_set_recursive_only(widget, recursive);
  return_value_if_fail(ret == RET_OK, ret);
  return widget_layout(widget);
}

widget_t* scroll_view_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, scroll_view), NULL);

  return widget;
}
