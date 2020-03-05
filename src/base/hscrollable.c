/**
 * File:   hscrollable.c
 * Author: AWTK Develop Team
 * Brief:  hscrollable
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
 * 2019-05-30 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "base/hscrollable.h"
#include "base/widget_vtable.h"

#ifndef WITHOUT_WIDGET_ANIMATORS
#include "widget_animators/widget_animator_scroll.h"
#endif /*WITHOUT_WIDGET_ANIMATORS*/

static widget_t* hscrollable_get_widget(hscrollable_t* hscrollable) {
  return hscrollable != NULL ? hscrollable->widget : NULL;
}

static ret_t hscrollable_on_pointer_down(hscrollable_t* hscrollable, pointer_event_t* e) {
  velocity_t* v = &(hscrollable->velocity);

  velocity_reset(v);
  hscrollable->down.x = e->x;
  hscrollable->down.y = e->y;
  hscrollable->xoffset_save = hscrollable->xoffset;

  velocity_update(v, e->e.time, e->x, e->y);

  return RET_OK;
}

static ret_t hscrollable_on_pointer_move(hscrollable_t* hscrollable, pointer_event_t* e) {
  velocity_t* v = &(hscrollable->velocity);
  int32_t dx = e->x - hscrollable->down.x;

  velocity_update(v, e->e.time, e->x, e->y);

  hscrollable->xoffset = hscrollable->xoffset_save - dx;

  if (!(hscrollable->always_scrollable)) {
    widget_t* widget = hscrollable_get_widget(hscrollable);
    if ((hscrollable->xoffset + widget->w) > hscrollable->virtual_w) {
      hscrollable->xoffset = hscrollable->virtual_w - widget->w;
    }
    if (hscrollable->xoffset < 0) {
      hscrollable->xoffset = 0;
    }
  }

  return RET_OK;
}

#ifndef WITHOUT_WIDGET_ANIMATORS
static ret_t hscrollable_on_scroll_done(void* ctx, event_t* e) {
  hscrollable_t* hscrollable = (hscrollable_t*)(ctx);
  return_value_if_fail(hscrollable != NULL, RET_BAD_PARAMS);

  hscrollable->wa = NULL;
  widget_invalidate_force(hscrollable_get_widget(hscrollable), NULL);

  return RET_REMOVE;
}
#endif /*WITHOUT_WIDGET_ANIMATORS*/

static ret_t hscrollable_fix_end_offset_default(hscrollable_t* hscrollable) {
  int32_t xoffset_end = 0;
  widget_t* widget = hscrollable_get_widget(hscrollable);
  return_value_if_fail(hscrollable != NULL && widget != NULL, RET_BAD_PARAMS);

  xoffset_end = hscrollable->xoffset_end;
  xoffset_end = tk_max(xoffset_end, 0);
  xoffset_end = tk_min(xoffset_end, (hscrollable->virtual_w - widget->w));

  if (hscrollable->virtual_w <= widget->w) {
    xoffset_end = 0;
  }

  hscrollable->xoffset_end = xoffset_end;

  return RET_OK;
}

ret_t hscrollable_scroll_to(hscrollable_t* hscrollable, int32_t xoffset_end, int32_t duration) {
  int32_t xoffset = 0;
  widget_t* widget = hscrollable_get_widget(hscrollable);
  return_value_if_fail(hscrollable != NULL && widget != NULL, RET_FAIL);

  hscrollable_fix_end_offset_default(hscrollable);
  xoffset_end = hscrollable->xoffset_end;
  xoffset = hscrollable->xoffset;
  if (xoffset == xoffset_end) {
    return RET_OK;
  }

#ifndef WITHOUT_WIDGET_ANIMATORS
  hscrollable->xoffset_end = xoffset_end;
  xoffset_end = hscrollable->xoffset_end;

  hscrollable->wa = widget_animator_scroll_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT);
  return_value_if_fail(hscrollable->wa != NULL, RET_OOM);

  widget_animator_scroll_set_params(hscrollable->wa, xoffset, 0, xoffset_end, 0);
  widget_animator_on(hscrollable->wa, EVT_ANIM_END, hscrollable_on_scroll_done, hscrollable);
  widget_animator_start(hscrollable->wa);
#else
  hscrollable->xoffset = xoffset_end;
#endif /*WITHOUT_WIDGET_ANIMATORS*/

  return RET_OK;
}

#define SPEED_SCALE 2
#define MIN_DELTA 10

static ret_t hscrollable_on_pointer_up(hscrollable_t* hscrollable, pointer_event_t* e) {
  velocity_t* v = &(hscrollable->velocity);
  int32_t move_dx = e->x - hscrollable->down.x;

  velocity_update(v, e->e.time, e->x, e->y);
  if (move_dx) {
    int xv = tk_min(v->xv, 100);

    hscrollable->xoffset_end = hscrollable->xoffset - xv;
    hscrollable_scroll_to(hscrollable, hscrollable->xoffset_end, 300);
  }

  return RET_OK;
}

ret_t hscrollable_on_event(hscrollable_t* hscrollable, event_t* e) {
  ret_t ret = RET_OK;
  uint16_t type = e->type;
  widget_t* widget = hscrollable_get_widget(hscrollable);
  return_value_if_fail(hscrollable != NULL && widget != NULL && e != NULL, RET_BAD_PARAMS);

  if (hscrollable->wa != NULL) {
    log_debug("animating ignore event\n");

    return RET_STOP;
  }

  switch (type) {
    case EVT_POINTER_DOWN:
      hscrollable->dragged = FALSE;
      widget_grab(widget->parent, widget);
      hscrollable_on_pointer_down(hscrollable, (pointer_event_t*)e);
      break;
    case EVT_POINTER_UP: {
      pointer_event_t* evt = (pointer_event_t*)e;
      int32_t dx = evt->x - hscrollable->down.x;
      if (dx) {
        hscrollable_on_pointer_up(hscrollable, (pointer_event_t*)e);
      }
      hscrollable->dragged = FALSE;
      widget_ungrab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (!evt->pressed) {
        break;
      }

      if (hscrollable->dragged) {
        hscrollable_on_pointer_move(hscrollable, evt);
        widget_invalidate_force(hscrollable_get_widget(hscrollable), NULL);
      } else {
        int32_t delta = evt->x - hscrollable->down.x;

        if (tk_abs(delta) >= TK_DRAG_THRESHOLD) {
          pointer_event_t abort = *evt;

          abort.e.type = EVT_POINTER_DOWN_ABORT;
          widget_dispatch_event_to_target_recursive(widget, (event_t*)(&abort));

          hscrollable->dragged = TRUE;
        }
      }

      ret = hscrollable->dragged ? RET_STOP : RET_OK;
      break;
    }
    default:
      break;
  }

  return ret;
}

ret_t hscrollable_on_paint_children(hscrollable_t* hscrollable, canvas_t* c) {
  rect_t r_save;
  widget_t* widget = hscrollable_get_widget(hscrollable);
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  return_value_if_fail(hscrollable != NULL && c != NULL && widget != NULL, RET_BAD_PARAMS);

  rect_t r = rect_init(c->ox, c->oy, widget->w, widget->h);

  int32_t xoffset = -hscrollable->xoffset;

  canvas_translate(c, xoffset, 0);
  canvas_get_clip_rect(c, &r_save);

  r = rect_intersect(&r, &r_save);

  if (vg != NULL) {
    vgcanvas_save(vg);
    vgcanvas_clip_rect(vg, (float_t)r.x, (float_t)r.y, (float_t)r.w, (float_t)r.h);
  }

  canvas_set_clip_rect(c, &r);
  widget_on_paint_children_default(widget, c);
  canvas_set_clip_rect(c, &r_save);
  canvas_untranslate(c, xoffset, 0);

  if (vg != NULL) {
    vgcanvas_clip_rect(vg, (float_t)r_save.x, (float_t)r_save.y, (float_t)r_save.w,
                       (float_t)r_save.h);
    vgcanvas_restore(vg);
  }
  return RET_OK;
}

ret_t hscrollable_get_prop(hscrollable_t* hscrollable, const char* name, value_t* v) {
  widget_t* widget = hscrollable_get_widget(hscrollable);
  return_value_if_fail(widget != NULL && hscrollable != NULL && name != NULL && v != NULL,
                       RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_W) || tk_str_eq(name, WIDGET_PROP_LAYOUT_W)) {
    value_set_int(v, tk_max(widget->w, hscrollable->virtual_w));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_H) || tk_str_eq(name, WIDGET_PROP_LAYOUT_H)) {
    value_set_int(v, widget->h);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    value_set_int(v, hscrollable->xoffset);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    value_set_int(v, 0);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XSLIDABLE)) {
    value_set_bool(v, TRUE);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YSLIDABLE)) {
    value_set_bool(v, FALSE);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

ret_t hscrollable_set_prop(hscrollable_t* hscrollable, const char* name, const value_t* v) {
  return_value_if_fail(hscrollable != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VIRTUAL_W)) {
    hscrollable->virtual_w = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XSLIDABLE)) {
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    hscrollable->xoffset = value_int(v);
    widget_invalidate_force(hscrollable_get_widget(hscrollable), NULL);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

hscrollable_t* hscrollable_create(widget_t* widget) {
  hscrollable_t* hscrollable = NULL;
  return_value_if_fail(widget != NULL, NULL);

  hscrollable = TKMEM_ZALLOC(hscrollable_t);
  return_value_if_fail(hscrollable != NULL, NULL);

  hscrollable->widget = widget;

  return hscrollable;
}

ret_t hscrollable_set_always_scrollable(hscrollable_t* hscrollable, bool_t always_scrollable) {
  return_value_if_fail(hscrollable != NULL, RET_BAD_PARAMS);
  hscrollable->always_scrollable = always_scrollable;

  return RET_OK;
}

ret_t hscrollable_set_xoffset(hscrollable_t* hscrollable, int32_t xoffset) {
  return_value_if_fail(hscrollable != NULL, RET_BAD_PARAMS);
  hscrollable->xoffset = xoffset;

  return RET_OK;
}

ret_t hscrollable_set_virtual_w(hscrollable_t* hscrollable, int32_t virtual_w) {
  return_value_if_fail(hscrollable != NULL, RET_BAD_PARAMS);
  hscrollable->virtual_w = virtual_w;

  return RET_OK;
}

ret_t hscrollable_destroy(hscrollable_t* hscrollable) {
  return_value_if_fail(hscrollable != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(hscrollable);

  return RET_OK;
}
