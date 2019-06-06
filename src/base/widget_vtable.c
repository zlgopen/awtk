/**
 * File:   widget_vtable.c
 * Author: AWTK Develop Team
 * Brief:  widget vtable default impl
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  if (r->w <= 0 || r->h <= 0) {
    return RET_OK;
  }

  r->x += widget->x;
  r->y += widget->y;
  if (widget->astyle != NULL) {
    int32_t ox = tk_abs(style_get_int(widget->astyle, STYLE_ID_X_OFFSET, 0));
    int32_t oy = tk_abs(style_get_int(widget->astyle, STYLE_ID_Y_OFFSET, 0));
    int32_t br = tk_abs(style_get_int(widget->astyle, STYLE_ID_ROUND_RADIUS, 0));
    int32_t bw = tk_abs(style_get_int(widget->astyle, STYLE_ID_BORDER_WIDTH, 1)) >> 1;

    if (br > 0) {
      ox++;
      oy++;
    }

    if (bw > 0) {
      ox += bw;
      oy += bw;
    }

    if (ox > 0) {
      r->x -= ox;
      r->w += ox + ox + 1;
    }
    if (oy > 0) {
      r->y -= oy;
      r->h += oy + oy + 1;
    }
  }

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
  int32_t left = c->ox + iter->x;
  int32_t top = c->oy + iter->y;
  int32_t bottom = top + iter->h;
  int32_t right = left + iter->w;

  if (!iter->visible) {
    iter->dirty = FALSE;
    continue;
  }

  if (left > c->clip_right || right < c->clip_left || top > c->clip_bottom ||
      bottom < c->clip_top) {
    iter->dirty = FALSE;
    continue;
  }

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
  point_t p = {x, y};
  return_value_if_fail(widget != NULL, NULL);

  if (widget->grab_widget != NULL) {
    return widget->grab_widget;
  }

  widget_to_local(widget, &p);
  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  xy_t xx = p.x;
  xy_t yy = p.y;
  xy_t r = iter->x + iter->w;
  xy_t b = iter->y + iter->h;

  if (iter->sensitive && iter->enable && xx >= iter->x && yy >= iter->y && xx <= r && yy <= b) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return NULL;
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
