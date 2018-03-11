/**
 * File:   window_manager.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  window manager
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/window_manager.h"

static ret_t on_window_destroy(void* ctx, event_t* e) {
  widget_t* wm = WIDGETP(ctx);
  window_manager_remove_child(wm, e->target);

  return RET_OK;
}

ret_t window_manager_add_child(widget_t* wm, widget_t* window) {
  return_value_if_fail(wm != NULL && window != NULL, RET_BAD_PARAMS);

  if (window->type == WIDGET_NORMAL_WINDOW) {
    widget_move(window, 0, 0);
    widget_resize(window, wm->w, wm->h);
  } else if (window->type == WIDGET_DIALOG) {
    xy_t x = (wm->w - window->w) >> 1;
    xy_t y = (wm->h - window->h) >> 1;
    widget_move_resize(window, x, y, window->w, window->h);
  }

  widget_invalidate(window, NULL);
  widget_on(window, EVT_DESTROY, on_window_destroy, wm);

  return widget_add_child(wm, window);
}

ret_t window_manager_remove_child(widget_t* wm, widget_t* window) {
  ret_t ret = RET_OK;
  return_value_if_fail(wm != NULL && window != NULL, RET_BAD_PARAMS);

  ret = widget_remove_child(wm, window);
  if (ret == RET_OK) {
    widget_t* top = window_manager_get_top_window(wm);
    if (top) {
      widget_invalidate(top, NULL);
    }
  }

  return ret;
}

widget_t* window_manager_find_target(widget_t* widget, xy_t x, xy_t y) {
  uint32_t i = 0;
  uint32_t n = 0;
  point_t p = {x, y};
  return_value_if_fail(widget != NULL, NULL);

  widget_to_local(widget, &p);
  if (widget->children != NULL && widget->children->size > 0) {
    xy_t xx = p.x;
    xy_t yy = p.y;
    n = widget->children->size;
    for (i = n; i > 0; i--) {
      widget_t* iter = (widget_t*)(widget->children->elms[i - 1]);
      xy_t r = iter->x + iter->w;
      xy_t b = iter->y + iter->h;

      if (xx >= iter->x && yy >= iter->y && xx <= r && yy <= b) {
        return iter;
      }

      if (iter->type == WIDGET_NORMAL_WINDOW || iter->type == WIDGET_DIALOG) {
        break;
      }
    }
  }

  return NULL;
}

ret_t window_manager_paint(widget_t* widget, canvas_t* c) {
  rect_t r;
  rect_t* dr = NULL;
  rect_t* ldr = NULL;
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL && c != NULL, RET_BAD_PARAMS);

  dr = &(wm->dirty_rect);

  if (dr->w && dr->h) {
    ldr = &(wm->last_dirty_rect);

    r = *dr;
    rect_merge(&r, ldr);

    if (r.w > 0 && r.h > 0) {
      ENSURE(canvas_begin_frame(c, &r) == RET_OK);
      ENSURE(widget_paint(WIDGETP(wm), c) == RET_OK);
      ENSURE(canvas_end_frame(c) == RET_OK);
    }
    log_debug("%s x=%d y=%d w=%d h=%d\n", __func__, r.x, r.y, r.w, r.h);
  }

  wm->last_dirty_rect = wm->dirty_rect;
  rectp_init(dr, widget->w, widget->h, 0, 0);

  return RET_OK;
}

widget_t* default_wm() {
  static window_manager_t* wm = NULL;
  if (wm == NULL) {
    wm = MEM_ZALLOC(window_manager_t);
    window_manager_init(wm);
  }

  return WIDGETP(wm);
}

static ret_t window_manager_grab(widget_t* widget, widget_t* child) { return RET_OK; }

static ret_t window_manager_ungrab(widget_t* widget, widget_t* child) { return RET_OK; }

static ret_t window_manager_invalidate(widget_t* widget, rect_t* r) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  rect_t* dr = &(wm->dirty_rect);

  rect_merge(dr, r);

  return RET_OK;
}

int32_t window_manager_find_top_window_index(widget_t* widget) {
  int32_t i = 0;
  int32_t nr = 0;
  return_value_if_fail(widget != NULL, -1);

  if (widget->children != NULL && widget->children->size > 0) {
    nr = widget->children->size;
    for (i = nr - 1; i >= 0; i--) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (iter->type == WIDGET_NORMAL_WINDOW) {
        return i;
      }
    }
  }

  return -1;
}

widget_t* window_manager_get_top_window(widget_t* widget) {
  int32_t index = window_manager_find_top_window_index(widget);

  return widget_get_child(widget, index);
}

ret_t window_manager_on_paint_children(widget_t* widget, canvas_t* c) {
  int32_t i = 0;
  int32_t nr = 0;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  if (widget->children != NULL && widget->children->size > 0) {
    nr = widget->children->size;
#ifdef FAST_MODE
    for (i = nr - 1; i >= 0; i--) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (iter->visible) {
        widget_paint(iter, c);
        break;
      }
    }
#else
    for (i = nr - 1; i >= 0; i--) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (iter->type == WIDGET_NORMAL_WINDOW) {
        break;
      }
    }

    if (i < 0) {
      i = 0;
    }

    for (; i < nr; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (iter->visible) {
        widget_paint(iter, c);
      }
    }
#endif /*FAST_MODE*/
  }

  return RET_OK;
}

static const widget_vtable_t s_wm_vtable = {.invalidate = window_manager_invalidate,
                                            .on_paint_children = window_manager_on_paint_children,
                                            .grab = window_manager_grab,
                                            .find_target = window_manager_find_target,
                                            .ungrab = window_manager_ungrab};

widget_t* window_manager_init(window_manager_t* wm) {
  widget_t* w = &(wm->widget);
  return_value_if_fail(wm != NULL, NULL);

  widget_init(w, NULL, WIDGET_WINDOW_MANAGER);
  array_init(&(wm->graps), 5);
  w->vt = &s_wm_vtable;

  return w;
}

ret_t window_manager_resize(widget_t* widget, wh_t w, wh_t h) {
  window_manager_t* wm = WINDOW_MANAGER(widget);
  return_value_if_fail(wm != NULL, RET_BAD_PARAMS);

  wm->dirty_rect.x = 0;
  wm->dirty_rect.y = 0;
  wm->dirty_rect.w = w;
  wm->dirty_rect.h = h;
  wm->last_dirty_rect = wm->dirty_rect;
  widget_move_resize(widget, 0, 0, w, h);

  return RET_OK;
}
