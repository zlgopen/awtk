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
#include "base/slide_view.h"

static ret_t slide_view_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t slide_view_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;

  switch (type) {
    case EVT_POINTER_DOWN:
      widget_set_state(widget, WIDGET_STATE_PRESSED);
      break;
    case EVT_POINTER_UP: {
      pointer_event_t evt = *(pointer_event_t*)e;
      evt.e.type = EVT_CLICK;
      evt.e.target = widget;
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      widget_dispatch(widget, (event_t*)&evt);
      break;
    }
    case EVT_POINTER_LEAVE:
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    case EVT_POINTER_ENTER:
      widget_set_state(widget, WIDGET_STATE_OVER);
      break;
    default:
      break;
  }

  return RET_OK;
}

static widget_t* slide_view_find_target(widget_t* widget, xy_t x, xy_t y) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  widget_t* active = widget_get_child(widget, slide_view->active);

  return active;
}

static ret_t slide_view_get_prop(widget_t* widget, const char* name, value_t* v) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (str_fast_equal(name, WIDGET_PROP_VALUE) || str_fast_equal(name, WIDGET_PROP_ACTIVE)) {
    value_set_int(v, slide_view->active);
    return RET_OK;
  } else if (str_fast_equal(name, WIDGET_PROP_VERTICAL)) {
    value_set_bool(v, slide_view->vertical);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t slide_view_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (str_fast_equal(name, WIDGET_PROP_VALUE) || str_fast_equal(name, WIDGET_PROP_ACTIVE)) {
    return slide_view_set_active(widget, value_int(v));
  } else if (str_fast_equal(name, WIDGET_PROP_VERTICAL)) {
    return slide_view_set_vertical(widget, value_bool(v));
  }

  return RET_NOT_FOUND;
}

static ret_t slide_view_on_paint_children(widget_t* widget, canvas_t* c) {
  slide_view_t* slide_view = SLIDE_VIEW(widget);
  widget_t* active = widget_get_child(widget, slide_view->active);
  return_value_if_fail(active != NULL, RET_BAD_PARAMS);

  return widget_paint(active, c);
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
