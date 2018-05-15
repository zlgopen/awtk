/**
 * File:   widget_vtable.c
 * Author: AWTK Develop Team
 * Brief:  widget vtable default impl
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
 * 2018-01-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/widget_vtable.h"
#include "base/mem.h"

ret_t widget_invalidate_default(widget_t* widget, rect_t* r) {
  r->x += widget->x;
  r->y += widget->y;
  if (widget->parent) {
    widget_t* parent = widget->parent;
    if (parent->vt && parent->vt->invalidate) {
      parent->vt->invalidate(parent, r);
    } else {
      widget_invalidate_default(parent, r);
    }
  }

  return RET_OK;
}

ret_t widget_on_event_default(widget_t* widget, event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t widget_on_paint_self_default(widget_t* widget, canvas_t* c) {
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t widget_on_paint_children_default(widget_t* widget, canvas_t* c) {
  uint32_t i = 0;
  uint32_t nr = 0;
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  if (widget->children != NULL) {
    for (i = 0, nr = widget->children->size; i < nr; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);
      if (iter->visible) {
        widget_paint(iter, c);
      }
    }
  }

  return RET_OK;
}

ret_t widget_on_keydown_default(widget_t* widget, key_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t widget_on_keyup_default(widget_t* widget, key_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t widget_on_click_default(widget_t* widget, pointer_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t widget_on_pointer_down_default(widget_t* widget, pointer_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t widget_on_pointer_move_default(widget_t* widget, pointer_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t widget_on_pointer_up_default(widget_t* widget, pointer_event_t* e) {
  return_value_if_fail(widget != NULL && e != NULL, RET_BAD_PARAMS);

  return RET_OK;
}

ret_t widget_get_prop_default(widget_t* widget, const char* name, value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return RET_NOT_FOUND;
}

ret_t widget_set_prop_default(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  return RET_NOT_FOUND;
}

ret_t widget_grab_default(widget_t* widget, widget_t* child) {
  widget_t* parent = widget->parent;

  if (parent != NULL && parent->vt != NULL && parent->vt->grab != NULL) {
    parent->vt->grab(parent, child);
  }

  return RET_OK;
}

ret_t widget_ungrab_default(widget_t* widget, widget_t* child) {
  widget_t* parent = widget->parent;

  if (parent != NULL && parent->vt != NULL && parent->vt->ungrab != NULL) {
    parent->vt->ungrab(parent, child);
  }

  return RET_OK;
}

widget_t* widget_find_target_default(widget_t* widget, xy_t x, xy_t y) {
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
    }
  }

  return NULL;
}

ret_t widget_destroy_default(widget_t* widget) {
  (void)widget;
  return RET_OK;
}

ret_t widget_on_paint_background_null(widget_t* widget, canvas_t* c) {
  (void)widget;
  (void)c;
  return RET_OK;
}

ret_t widget_on_paint_done_null(widget_t* widget, canvas_t* c) {
  (void)widget;
  (void)c;
  return RET_OK;
}

static const widget_vtable_t s_vtable = {.invalidate = widget_invalidate_default,
                                         .on_event = widget_on_event_default,
                                         .on_paint_self = widget_on_paint_self_default,
                                         .on_paint_children = widget_on_paint_children_default,
                                         .on_keydown = widget_on_keydown_default,
                                         .on_keyup = widget_on_keyup_default,
                                         .on_click = widget_on_click_default,
                                         .on_pointer_down = widget_on_pointer_down_default,
                                         .on_pointer_move = widget_on_pointer_move_default,
                                         .on_pointer_up = widget_on_pointer_up_default,
                                         .get_prop = widget_get_prop_default,
                                         .set_prop = widget_set_prop_default,
                                         .grab = widget_grab_default,
                                         .ungrab = widget_ungrab_default,
                                         .find_target = widget_find_target_default,
                                         .destroy = widget_destroy_default};

const widget_vtable_t* widget_vtable_default() { return &s_vtable; }
