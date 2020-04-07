/**
 * File:   widget_vtable.c
 * Author: AWTK Develop Team
 * Brief:  widget vtable default impl
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
 * 2018-01-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/widget_vtable.h"
#include "tkc/mem.h"

ret_t widget_invalidate_default(widget_t* widget, rect_t* r) {
  if (widget->vt->scrollable) {
    int32_t ox = widget_get_prop_int(widget, WIDGET_PROP_XOFFSET, 0);
    int32_t oy = widget_get_prop_int(widget, WIDGET_PROP_YOFFSET, 0);
    rect_t r_self = rect_init(0, 0, widget->w, widget->h);

    if (ox > 0) {
      r->x -= ox;
      r->w += ox + 1;
    }
    if (oy > 0) {
      r->y -= oy;
      r->h += oy + 1;
    }

    *r = rect_intersect(r, &r_self);
  }

  if (r->w <= 0 || r->h <= 0) {
    return RET_OK;
  }

  r->x += widget->x;
  r->y += widget->y;

  if (widget->astyle != NULL) {
    int32_t tolerance = widget->dirty_rect_tolerance;

    if (tolerance > 0) {
      r->x -= tolerance;
      r->y -= tolerance;
      r->w += 2 * tolerance + 1;
      r->h += 2 * tolerance + 1;
    }
  }

  if (r->x < 0) {
    r->w = r->w + r->x;
  }
  if (r->y < 0) {
    r->h = r->h + r->y;
  }

  r->x = tk_max(0, r->x);
  r->y = tk_max(0, r->y);
  r->w = tk_max(0, r->w);
  r->h = tk_max(0, r->h);

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
  if (style_is_valid(widget->astyle)) {
    return widget_paint_helper(widget, c, NULL, NULL);
  }
  return RET_OK;
}

ret_t widget_on_paint_children_default(widget_t* widget, canvas_t* c) {
  return_value_if_fail(widget != NULL && c != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)

  if (!iter->visible) {
    iter->dirty = FALSE;
    continue;
  }

  /* int32_t left = c->ox + iter->x;
  int32_t top = c->oy + iter->y;
  int32_t bottom = top + iter->h;
  int32_t right = left + iter->w;
  if (left > c->clip_right || right < c->clip_left || top > c->clip_bottom ||
      bottom < c->clip_top) {
    iter->dirty = FALSE;
    continue;
  }*/

  widget_paint(iter, c);
  WIDGET_FOR_EACH_CHILD_END();

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

widget_t* widget_find_target_default(widget_t* widget, xy_t x, xy_t y) {
  xy_t xx = 0;
  xy_t yy = 0;
  point_t p = {x, y};
  return_value_if_fail(widget != NULL, NULL);

  if (widget->grab_widget != NULL) {
    return widget->grab_widget;
  }

  widget_to_local(widget, &p);
  xx = p.x;
  yy = p.y;
  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (yy < iter->y || yy >= (iter->y + iter->h)) {
    continue;
  }

  if (xx < iter->x || xx >= (iter->x + iter->w)) {
    continue;
  }

  if (!iter->sensitive || !iter->enable) {
    continue;
  }

  return iter;
  WIDGET_FOR_EACH_CHILD_END();

  return NULL;
}

static ret_t widget_copy_props(widget_t* clone, widget_t* widget, const char* const* properties) {
  if (properties != NULL) {
    value_t v;
    value_t defval;
    uint32_t i = 0;
    for (i = 0; properties[i] != NULL; i++) {
      const char* prop = properties[i];
      if (widget_get_prop(widget, prop, &v) == RET_OK) {
        if (widget_get_prop_default_value(widget, prop, &defval) == RET_OK) {
          if (!value_equal(&v, &defval)) {
            widget_set_prop(clone, prop, &v);
          }
        } else {
          widget_set_prop(clone, prop, &v);
        }
      }
    }
  }

  return RET_OK;
}

ret_t widget_on_copy_default(widget_t* widget, widget_t* other) {
  return widget_copy_props(widget, other, widget->vt->clone_properties);
}

ret_t widget_on_destroy_default(widget_t* widget) {
  (void)widget;
  return RET_OK;
}

ret_t widget_on_paint_null(widget_t* widget, canvas_t* c) {
  (void)widget;
  (void)c;
  return RET_OK;
}

TK_DECL_VTABLE(widget) = {.size = sizeof(widget_t),
                          .type = WIDGET_TYPE_NONE,
                          .parent = NULL,
                          .on_copy = widget_on_copy_default,
                          .invalidate = widget_invalidate_default,
                          .on_event = widget_on_event_default,
                          .on_paint_self = widget_on_paint_self_default,
                          .on_paint_children = widget_on_paint_children_default,
                          .on_keydown = widget_on_keydown_default,
                          .on_keyup = widget_on_keyup_default,
                          .on_pointer_down = widget_on_pointer_down_default,
                          .on_pointer_move = widget_on_pointer_move_default,
                          .on_pointer_up = widget_on_pointer_up_default,
                          .get_prop = widget_get_prop_default,
                          .set_prop = widget_set_prop_default,
                          .find_target = widget_find_target_default,
                          .on_destroy = widget_on_destroy_default};

const widget_vtable_t* widget_vtable_default() {
  return TK_REF_VTABLE(widget);
}
