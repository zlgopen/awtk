/**
 * File:   slide_view.h
 * Author: AWTK Develop Team
 * Brief:  slide_view
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
 * 2018-06-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "tkc/easing.h"
#include "slide_view/slide_view.h"
#include "widget_animators/widget_animator_scroll.h"

static bool_t anim_hint_is_overlap(slide_view_t* slide_view) {
  return tk_str_eq(slide_view->anim_hint, "overlap") ||
         tk_str_eq(slide_view->anim_hint, "overlap_with_alpha");
}

static bool_t anim_hint_is_overlap_with_alpha(slide_view_t* slide_view) {
  return tk_str_eq(slide_view->anim_hint, "overlap_with_alpha");
}

static bool_t slide_view_is_loopable(slide_view_t* slide_view) {
  return slide_view->auto_play || slide_view->loop;
}

static ret_t canvas_set_clip_rect_with_offset(canvas_t* c, rect_t* r, rect_t* save_r, int32_t ox,
                                              int32_t oy) {
  rect_t rr = *r;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  rr.x += ox;
  rr.y += oy;

  rr = rect_intersect(&rr, save_r);

  vgcanvas_clip_rect(vg, rr.x, rr.y, rr.w, rr.h);
  return canvas_set_clip_rect(c, &rr);
}

widget_t* slide_view_get_prev(slide_view_t* slide_view) {
  widget_t* widget = WIDGET(slide_view);
  uint32_t active = slide_view->active;
  uint32_t nr = widget_count_children(widget);

  if (active > 0) {
    return widget_get_child(widget, active - 1);
  } else if (slide_view_is_loopable(slide_view) && nr > 1) {
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
  } else if (slide_view_is_loopable(slide_view) && nr > 1) {
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
  } else if (slide_view_is_loopable(slide_view) && nr > 1) {
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
  } else if (slide_view_is_loopable(slide_view) && nr > 1) {
    return slide_view_set_active(widget, 0);
  } else {
    return RET_FAIL;
  }
}

static ret_t slide_view_on_paint_self(widget_t* widget, canvas_t* c) {
  (void)widget;
  (void)c;
  return RET_OK;
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
  return_value_if_fail(widget != NULL && slide_view != NULL, RET_BAD_PARAMS);

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
      widget_animator_scroll_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT);

  velocity_update(v, e->e.time, e->x, e->y);
  if (slide_view->vertical) {
    int32_t h = widget->h;
    int32_t yoffset_end = 0;
    int32_t yv = tk_abs(v->yv);
    int32_t yoffset = slide_view->yoffset;
    bool_t rollback = (yv < v_threshhold) && (tk_abs(yoffset) < h / 2);

    if (yoffset > 0 && slide_view_get_prev(slide_view) == NULL) {
      rollback = TRUE;
    } else if (yoffset < 0 && slide_view_get_next(slide_view) == NULL) {
      rollback = TRUE;
    }
    if (tk_abs(yoffset) < TK_DRAG_THRESHOLD) {
      rollback = TRUE;
    }

    if (!rollback) {
      yoffset_end = yoffset > 0 ? h : -h;
    }

    widget_animator_scroll_set_params(animator, 0, yoffset, 0, yoffset_end);
  } else {
    int32_t w = widget->w;
    int32_t xoffset_end = 0;
    int32_t xv = tk_abs(v->xv);
    int32_t xoffset = slide_view->xoffset;
    bool_t rollback = (xv < v_threshhold) && (tk_abs(xoffset) < w / 2);

    if (xoffset > 0 && slide_view_get_prev(slide_view) == NULL) {
      rollback = TRUE;
    } else if (xoffset < 0 && slide_view_get_next(slide_view) == NULL) {
      rollback = TRUE;
    }
    if (tk_abs(xoffset) < TK_DRAG_THRESHOLD) {
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
  ret_t ret = RET_OK;
  uint16_t type = e->type;
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(widget != NULL && slide_view != NULL, RET_BAD_PARAMS);

  if (slide_view->animating) {
    return RET_OK;
  }

  switch (type) {
    case EVT_POINTER_DOWN:
      slide_view->pressed = TRUE;
      widget_grab(widget->parent, widget);
      slide_view_on_pointer_down(slide_view, (pointer_event_t*)e);
      break;
    case EVT_POINTER_UP: {
      if (slide_view->pressed) {
        if (slide_view->dragged) {
          slide_view_on_pointer_up(slide_view, (pointer_event_t*)e);
        }
        slide_view->dragged = FALSE;
        slide_view->pressed = FALSE;
        widget_ungrab(widget->parent, widget);
      }
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      if (slide_view->pressed) {
        slide_view->xoffset = 0;
        slide_view->yoffset = 0;
        slide_view->pressed = FALSE;
        slide_view->dragged = FALSE;
        widget_ungrab(widget->parent, widget);
      }
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      if (slide_view->dragged) {
        slide_view_on_pointer_move(slide_view, evt);
        widget_invalidate(widget, NULL);
      } else if (evt->pressed && slide_view->pressed) {
        int32_t delta = 0;

        if (slide_view->vertical) {
          delta = evt->y - slide_view->down.y;
        } else {
          delta = evt->x - slide_view->down.x;
        }

        if (tk_abs(delta) > TK_DRAG_THRESHOLD) {
          pointer_event_t abort;
          pointer_event_init(&abort, EVT_POINTER_DOWN_ABORT, widget, evt->x, evt->y);
          widget_dispatch_event_to_target_recursive(widget, (event_t*)(&abort));
          slide_view->dragged = TRUE;
        }
      }

      break;
    }
    case EVT_KEY_UP: {
      key_event_t* evt = (key_event_t*)e;
      if (evt->key == TK_KEY_LEFT) {
        ret = RET_STOP;
        slide_view_activate_prev(slide_view);
      } else if (evt->key == TK_KEY_RIGHT) {
        ret = RET_STOP;
        slide_view_activate_next(slide_view);
      }
    }
    default:
      break;
  }

  if (ret == RET_OK) {
    if (slide_view->dragged) {
      ret = RET_STOP;
    }
  }

  return ret;
}

static widget_t* slide_view_find_target(widget_t* widget, xy_t x, xy_t y) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(widget != NULL && slide_view != NULL, NULL);

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
  } else if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    value_set_str(v, slide_view->anim_hint);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_LOOP)) {
    value_set_bool(v, slide_view->loop);
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

static ret_t slide_view_set_prop(widget_t* widget, const char* name, const value_t* v) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE) || tk_str_eq(name, WIDGET_PROP_ACTIVE)) {
    return slide_view_set_active(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_VERTICAL)) {
    return slide_view_set_vertical(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    return slide_view_set_anim_hint(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_LOOP)) {
    return slide_view_set_loop(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_XOFFSET)) {
    slide_view->xoffset = value_int(v);
    widget_invalidate(widget, NULL);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_PLAY)) {
    return slide_view_set_auto_play(widget, value_int(v));
  } else if (tk_str_eq(name, WIDGET_PROP_YOFFSET)) {
    slide_view->yoffset = value_int(v);
    widget_invalidate(widget, NULL);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t slide_view_paint_prev_next_v_translate(slide_view_t* slide_view, canvas_t* c,
                                                    rect_t* save_r, widget_t* prev, widget_t* next,
                                                    int32_t yoffset) {
  rect_t r;
  int32_t ox = c->ox;
  int32_t oy = c->oy;
  xy_t w = WIDGET(slide_view)->w;
  xy_t h = WIDGET(slide_view)->h;
  int32_t r_yoffset = h - yoffset;

  if (prev != NULL) {
    if (yoffset > h) {
      r = rect_init(0, yoffset - h, w, h + h - yoffset);
    } else {
      r = rect_init(0, 0, w, yoffset);
    }

    if (r.h > 0) {
      canvas_save(c);
      canvas_translate(c, 0, -r_yoffset);
      canvas_set_clip_rect_with_offset(c, &r, save_r, ox, oy);
      widget_paint(prev, c);
      canvas_untranslate(c, 0, -r_yoffset);
      canvas_restore(c);
    }
  }

  if (next != NULL) {
    if (r_yoffset > h) {
      r = rect_init(0, 0, w, h + h - r_yoffset);
    } else {
      r = rect_init(0, yoffset, w, r_yoffset);
    }

    if (r.h > 0) {
      canvas_save(c);
      canvas_translate(c, 0, yoffset);
      canvas_set_clip_rect_with_offset(c, &r, save_r, ox, oy);
      widget_paint(next, c);
      canvas_untranslate(c, 0, yoffset);
      canvas_restore(c);
    }
  }

  return RET_OK;
}

static ret_t slide_view_set_global_alpha(slide_view_t* slide_view, canvas_t* c, int32_t offset,
                                         int32_t total) {
  if (anim_hint_is_overlap_with_alpha(slide_view)) {
    uint8_t a = 0xff - (0xff * offset) / total;
    canvas_set_global_alpha(c, a);
  }

  return RET_OK;
}

static ret_t slide_view_paint_prev_next_v_overlap(slide_view_t* slide_view, canvas_t* c,
                                                  rect_t* save_r, widget_t* prev, widget_t* next,
                                                  int32_t yoffset) {
  rect_t r;
  int32_t ox = c->ox;
  int32_t oy = c->oy;
  xy_t w = WIDGET(slide_view)->w;
  xy_t h = WIDGET(slide_view)->h;
  int32_t r_yoffset = h - yoffset;

  if (next != NULL && r_yoffset >= 0) {
    canvas_save(c);
    if (r_yoffset > h) {
      widget_paint(next, c);
    } else {
      slide_view_set_global_alpha(slide_view, c, yoffset, h);
      widget_paint(next, c);
      canvas_set_global_alpha(c, 0xff);
    }
    canvas_restore(c);
  }

  if (prev != NULL) {
    if (yoffset > h) {
      r_yoffset = 0;
      yoffset = h;
    }

    r = rect_init(0, 0, w, yoffset);

    if (r.h > 0) {
      canvas_save(c);
      canvas_translate(c, 0, -r_yoffset);
      canvas_set_clip_rect_with_offset(c, &r, save_r, ox, oy);
      widget_paint(prev, c);
      canvas_untranslate(c, 0, -r_yoffset);
      canvas_restore(c);
    }
  }

  return RET_OK;
}

static ret_t slide_view_paint_children_v_gt(slide_view_t* slide_view, canvas_t* c, rect_t* save_r) {
  int32_t yoffset = tk_abs(slide_view->yoffset);
  widget_t* prev = slide_view_get_prev(slide_view);
  widget_t* active = widget_get_child(WIDGET(slide_view), slide_view->active);

  if (anim_hint_is_overlap(slide_view)) {
    return slide_view_paint_prev_next_v_overlap(slide_view, c, save_r, prev, active, yoffset);
  } else {
    return slide_view_paint_prev_next_v_translate(slide_view, c, save_r, prev, active, yoffset);
  }
}

static ret_t slide_view_paint_children_v_lt(slide_view_t* slide_view, canvas_t* c, rect_t* save_r) {
  int32_t yoffset = WIDGET(slide_view)->h - tk_abs(slide_view->yoffset);
  widget_t* next = slide_view_get_next(slide_view);
  widget_t* active = widget_get_child(WIDGET(slide_view), slide_view->active);

  if (anim_hint_is_overlap(slide_view)) {
    return slide_view_paint_prev_next_v_overlap(slide_view, c, save_r, active, next, yoffset);
  } else {
    return slide_view_paint_prev_next_v_translate(slide_view, c, save_r, active, next, yoffset);
  }
}

static ret_t slide_view_paint_prev_next_h_translate(slide_view_t* slide_view, canvas_t* c,
                                                    rect_t* save_r, widget_t* prev, widget_t* next,
                                                    int32_t xoffset) {
  rect_t r;
  int32_t ox = c->ox;
  int32_t oy = c->oy;
  xy_t w = WIDGET(slide_view)->w;
  xy_t h = WIDGET(slide_view)->h;
  int32_t r_xoffset = w - xoffset;

  if (prev != NULL) {
    if (xoffset > w) {
      r = rect_init(xoffset - w, 0, w + w - xoffset, h);
    } else {
      r = rect_init(0, 0, xoffset, h);
    }

    if (r.w > 0) {
      canvas_save(c);
      canvas_translate(c, -r_xoffset, 0);
      canvas_set_clip_rect_with_offset(c, &r, save_r, ox, oy);
      widget_paint(prev, c);
      canvas_untranslate(c, -r_xoffset, 0);
      canvas_restore(c);
    }
  }

  if (next != NULL) {
    if (r_xoffset > w) {
      r = rect_init(0, 0, w + w - r_xoffset, h);
    } else {
      r = rect_init(xoffset, 0, r_xoffset, h);
    }

    if (r.w > 0) {
      canvas_save(c);
      canvas_translate(c, xoffset, 0);
      canvas_set_clip_rect_with_offset(c, &r, save_r, ox, oy);
      widget_paint(next, c);
      canvas_untranslate(c, xoffset, 0);
      canvas_restore(c);
    }
  }

  return RET_OK;
}

static ret_t slide_view_paint_prev_next_h_overlap(slide_view_t* slide_view, canvas_t* c,
                                                  rect_t* save_r, widget_t* prev, widget_t* next,
                                                  int32_t xoffset) {
  rect_t r;
  int32_t ox = c->ox;
  int32_t oy = c->oy;
  xy_t w = WIDGET(slide_view)->w;
  xy_t h = WIDGET(slide_view)->h;
  int32_t r_xoffset = w - xoffset;

  if (prev != NULL && xoffset >= 0) {
    canvas_save(c);
    if (xoffset > w) {
      widget_paint(prev, c);
    } else {
      slide_view_set_global_alpha(slide_view, c, r_xoffset, w);
      widget_paint(prev, c);
      canvas_set_global_alpha(c, 0xff);
    }
    canvas_restore(c);
  }

  if (next != NULL) {
    if (r_xoffset > w) {
      xoffset = 0;
      r_xoffset = w;
    }

    r = rect_init(xoffset, 0, r_xoffset, h);

    if (r.w > 0) {
      canvas_save(c);
      canvas_translate(c, xoffset, 0);
      canvas_set_clip_rect_with_offset(c, &r, save_r, ox, oy);
      widget_paint(next, c);
      canvas_untranslate(c, xoffset, 0);
      canvas_restore(c);
    }
  }

  return RET_OK;
}

static ret_t slide_view_paint_children_h_gt(slide_view_t* slide_view, canvas_t* c, rect_t* save_r) {
  int32_t xoffset = tk_abs(slide_view->xoffset);
  widget_t* prev = slide_view_get_prev(slide_view);
  widget_t* active = widget_get_child(WIDGET(slide_view), slide_view->active);

  if (anim_hint_is_overlap(slide_view)) {
    return slide_view_paint_prev_next_h_overlap(slide_view, c, save_r, prev, active, xoffset);
  } else {
    return slide_view_paint_prev_next_h_translate(slide_view, c, save_r, prev, active, xoffset);
  }
}

static ret_t slide_view_paint_children_h_lt(slide_view_t* slide_view, canvas_t* c, rect_t* save_r) {
  int32_t xoffset = WIDGET(slide_view)->w - tk_abs(slide_view->xoffset);
  widget_t* next = slide_view_get_next(slide_view);
  widget_t* active = widget_get_child(WIDGET(slide_view), slide_view->active);

  if (anim_hint_is_overlap(slide_view)) {
    return slide_view_paint_prev_next_h_overlap(slide_view, c, save_r, active, next, xoffset);
  } else {
    return slide_view_paint_prev_next_h_translate(slide_view, c, save_r, active, next, xoffset);
  }
}

static ret_t slide_view_on_paint_children(widget_t* widget, canvas_t* c) {
  rect_t save_r;
  widget_t* active = NULL;
  uint8_t save_a = c->lcd->global_alpha;
  vgcanvas_t* vg = canvas_get_vgcanvas(c);
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(widget != NULL && slide_view != NULL, RET_BAD_PARAMS);

  active = widget_get_child(widget, slide_view->active);
  return_value_if_fail(active != NULL, RET_BAD_PARAMS);

  canvas_get_clip_rect(c, &save_r);

  if (slide_view->vertical) {
    if (slide_view->yoffset > 0) {
      slide_view_paint_children_v_gt(slide_view, c, &save_r);
    } else if (slide_view->yoffset < 0) {
      slide_view_paint_children_v_lt(slide_view, c, &save_r);
    } else {
      widget_paint(active, c);
    }
  } else {
    if (slide_view->xoffset > 0) {
      slide_view_paint_children_h_gt(slide_view, c, &save_r);
    } else if (slide_view->xoffset < 0) {
      slide_view_paint_children_h_lt(slide_view, c, &save_r);
    } else {
      widget_paint(active, c);
    }
  }

  vgcanvas_clip_rect(vg, save_r.x, save_r.y, save_r.w, save_r.h);
  canvas_set_clip_rect(c, &save_r);
  canvas_set_global_alpha(c, save_a);

  return RET_OK;
}

static ret_t slide_view_on_destroy(widget_t* widget) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(widget != NULL && slide_view != NULL, RET_BAD_PARAMS);

  if (slide_view->timer_id) {
    timer_remove(slide_view->timer_id);
    slide_view->timer_id = 0;
  }
  TKMEM_FREE(slide_view->anim_hint);

  return RET_OK;
}

static const char* s_slide_view_properties[] = {WIDGET_PROP_VALUE,     WIDGET_PROP_LOOP,
                                                WIDGET_PROP_VERTICAL,  WIDGET_PROP_ANIM_HINT,
                                                WIDGET_PROP_AUTO_PLAY, NULL};
TK_DECL_VTABLE(slide_view) = {.size = sizeof(slide_view_t),
                              .inputable = TRUE,
                              .type = WIDGET_TYPE_SLIDE_VIEW,
                              .only_active_child_visible = TRUE,
                              .clone_properties = s_slide_view_properties,
                              .persistent_properties = s_slide_view_properties,
                              .parent = TK_PARENT_VTABLE(widget),
                              .create = slide_view_create,
                              .on_event = slide_view_on_event,
                              .get_prop = slide_view_get_prop,
                              .set_prop = slide_view_set_prop,
                              .find_target = slide_view_find_target,
                              .on_paint_children = slide_view_on_paint_children,
                              .on_paint_self = slide_view_on_paint_self,
                              .on_destroy = slide_view_on_destroy};

static ret_t slide_view_on_target_destroy(void* ctx, event_t* evt) {
  widget_t* view = WIDGET(ctx);

  widget_set_prop_pointer(view, "save_target", NULL);

  return RET_REMOVE;
}

static ret_t slide_view_save_target(widget_t* widget) {
  widget_t* target = NULL;
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  widget_t* active_view = widget_get_child(widget, slide_view->active);

  if (active_view != NULL) {
    target = active_view;
    while (target->target != NULL) {
      target = target->target;
    }

    if (target != NULL) {
      widget_set_prop_pointer(active_view, "save_target", target);
      widget_on(target, EVT_DESTROY, slide_view_on_target_destroy, active_view);
    }
  }

  return RET_OK;
}

static ret_t slide_view_restore_target(widget_t* widget) {
  widget_t* target = NULL;
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  widget_t* active_view = widget_get_child(widget, slide_view->active);

  if (active_view != NULL) {
    target = WIDGET(widget_get_prop_pointer(active_view, "save_target"));

    if (target == NULL || target->parent == NULL) {
      const char* default_focused_child =
          widget_get_prop_str(active_view, "default_focused_child", NULL);
      if (default_focused_child != NULL) {
        target = widget_lookup(active_view, default_focused_child, TRUE);
        if (target == NULL) {
          target = widget_lookup_by_type(active_view, default_focused_child, TRUE);
        }
      }
    }

    if (target == NULL || target->parent == NULL) {
      target = active_view;
    }
    widget_off_by_func(target, EVT_DESTROY, slide_view_on_target_destroy, active_view);

    log_debug("target=%s\n", target->vt->type);
    while (target->parent != NULL) {
      target->parent->target = target;
      target->parent->key_target = target;
      target = target->parent;
      if (target == widget) {
        break;
      }
    }
  }

  return RET_OK;
}

ret_t slide_view_set_active(widget_t* widget, uint32_t active) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(slide_view != NULL, RET_BAD_PARAMS);

  if (slide_view->active != active && widget->children != NULL) {
    event_t evt = event_init(EVT_VALUE_WILL_CHANGE, widget);

    slide_view_save_target(widget);
    widget_dispatch(widget, &evt);
    slide_view->active = active;
    evt = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &evt);
    slide_view_restore_target(widget);

    widget_invalidate(widget, NULL);
  } else {
    slide_view->active = active;
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

ret_t slide_view_set_anim_hint(widget_t* widget, const char* anim_hint) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(slide_view != NULL, RET_BAD_PARAMS);

  slide_view->anim_hint = tk_str_copy(slide_view->anim_hint, anim_hint);

  return RET_OK;
}

ret_t slide_view_set_loop(widget_t* widget, bool_t loop) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(slide_view != NULL, RET_BAD_PARAMS);

  slide_view->loop = loop;

  return RET_OK;
}

widget_t* slide_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(slide_view), x, y, w, h);
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(slide_view != NULL, NULL);

  slide_view->loop = FALSE;
  slide_view->auto_play = 0;
  slide_view->vertical = FALSE;

  return widget;
}

static ret_t slide_view_on_timer_next(const timer_info_t* timer) {
  widget_t* widget = WIDGET(timer->ctx);
  slide_view_t* slide_view = SLIDE_VIEW(timer->ctx);
  return_value_if_fail(slide_view != NULL, RET_BAD_PARAMS);

  widget_animator_t* animator =
      widget_animator_scroll_create(widget, TK_ANIMATING_TIME, 0, EASING_SIN_INOUT);

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
        timer_add(slide_view_on_timer_next, slide_view, auto_play + TK_ANIMATING_TIME);

    return slide_view->timer_id ? RET_OK : RET_OOM;
  }

  return RET_OK;
}

widget_t* slide_view_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, slide_view), NULL);

  return widget;
}
