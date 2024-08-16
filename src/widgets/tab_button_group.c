/**
 * File:   tab_button_group.h
 * Author: AWTK Develop Team
 * Brief:  tab_button_group
 *
 * Copyright (c) 2018 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "widgets/tab_button.h"
#include "widgets/tab_button_group.h"

#define TK_TAB_BUTTON_DRAG_THRESHOLD 5

#define TAB_BUTTON_GROUP_SCROLLABLE_MODE_WHELL     1 << 0
#define TAB_BUTTON_GROUP_SCROLLABLE_MODE_DRAGGED   1 << 1
#define TAB_BUTTON_GROUP_SCROLLABLE_MODE_ALL       0xFFFFFFFF

#define TAB_BUTTON_GROUP_SCROLLABLE_MODE_WHELL_STRING     "wheel"
#define TAB_BUTTON_GROUP_SCROLLABLE_MODE_DRAGGED_STRING   "dragged"
#define TAB_BUTTON_GROUP_SCROLLABLE_MODE_ALL_STRING       "all"


static ret_t tab_button_group_on_layout_children_non_compact(widget_t* widget) {
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  bool_t first = TRUE;
  int32_t h = widget->h;
  int32_t visible_nr = 0;
  int32_t item_w = 0;
  int32_t first_w = 0;
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (widget_get_visible(iter)) {
    visible_nr++;
  }
  WIDGET_FOR_EACH_CHILD_END();
  if (visible_nr > 0) {
    item_w = widget->w / visible_nr;
    first_w = widget->w - (visible_nr - 1) * item_w;
  }

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (!widget_get_visible(iter)) {
    continue;
  }
  w = first ? first_w : item_w;
  first = FALSE;
  widget_move_resize(iter, x, y, w, h);
  x += w;
  widget_layout_children(iter);
  WIDGET_FOR_EACH_CHILD_END();

  if (tab_button_group->hscrollable != NULL) {
    if (tab_button_group->hscrollable->virtual_w != widget->w) {
      hscrollable_set_xoffset(tab_button_group->hscrollable, 0);
      hscrollable_set_virtual_w(tab_button_group->hscrollable, widget->w);
    }
    hscrollable_set_always_scrollable(tab_button_group->hscrollable, FALSE);
  }

  return RET_OK;
}

static ret_t tab_button_group_on_layout_children_compact(widget_t* widget) {
  value_t v;
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = widget->h;
  widget_t* active = NULL;
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (!widget_get_visible(iter)) {
    continue;
  }
  iter->h = h;
  if (widget_get_prop(iter, WIDGET_PROP_MIN_W, &v) == RET_OK) {
    w = value_int(&v);
  } else {
    w = iter->w;
  }

  if (widget_get_prop(iter, WIDGET_PROP_MAX_W, &v) == RET_OK) {
    w = tk_min(w, value_int(&v));
  }

  widget_move_resize(iter, x, y, w, h);
  widget_layout_children(iter);
  x += w;
  if (widget_get_value(iter)) {
    active = iter;
  }
  WIDGET_FOR_EACH_CHILD_END();

  if (tab_button_group->hscrollable != NULL) {
    if (tab_button_group->hscrollable->virtual_w != x - 1) {
      hscrollable_set_xoffset(tab_button_group->hscrollable, 0);
      hscrollable_set_virtual_w(tab_button_group->hscrollable, x - 1);
    }
    hscrollable_set_always_scrollable(tab_button_group->hscrollable, FALSE);
  }

  if (active != NULL && tab_button_group->is_active_in_viewport) {
    widget_ensure_visible_in_viewport(active);
  }

  return RET_OK;
}

static ret_t tab_button_group_set_active_in_viewport_on_layout(tab_button_group_t* tab_button_group, bool_t active_in_viewport) {
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);
  tab_button_group->is_active_in_viewport = active_in_viewport;
  return RET_OK;
}

static ret_t tab_button_group_on_layout_children(widget_t* widget) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  ENSURE(tab_button_group);
  if (widget->children && widget->children->size > 0) {
    if (tab_button_group->compact) {
      return tab_button_group_on_layout_children_compact(widget);
    } else {
      return tab_button_group_on_layout_children_non_compact(widget);
    }
  }

  return RET_OK;
}

static ret_t tab_button_group_get_prop(widget_t* widget, const char* name, value_t* v) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_COMPACT)) {
    value_set_bool(v, tab_button_group->compact);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_SCROLLABLE)) {
    value_set_bool(v, tab_button_group->scrollable);
    return RET_OK;
  } else if (tk_str_eq(name, TAB_BUTTON_GROUP_PROP_DRAG_CHILD)) {
    value_set_bool(v, tab_button_group->drag_child);
    return RET_OK;
  }

  if (tab_button_group->hscrollable != NULL) {
    return hscrollable_get_prop(tab_button_group->hscrollable, name, v);
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t tab_button_group_set_prop(widget_t* widget, const char* name, const value_t* v) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_COMPACT)) {
    tab_button_group_set_compact(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_SCROLLABLE)) {
    tab_button_group_set_scrollable(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(name, TAB_BUTTON_GROUP_PROP_DRAG_CHILD)) {
    tab_button_group_set_drag_child(widget, value_bool(v));
    return RET_OK;
  } else if (tk_str_eq(name, TAB_BUTTON_GROUP_PROP_SCROLLABLE_MODE)) {
    const char* str = value_str(v);
    if (str != NULL) {
      if (tk_stricmp(str, TAB_BUTTON_GROUP_SCROLLABLE_MODE_WHELL_STRING) == 0) {
        tab_button_group->scrollable_mode_flags = TAB_BUTTON_GROUP_SCROLLABLE_MODE_WHELL;
      } else if (tk_stricmp(str, TAB_BUTTON_GROUP_SCROLLABLE_MODE_DRAGGED_STRING) == 0) {
        tab_button_group->scrollable_mode_flags = TAB_BUTTON_GROUP_SCROLLABLE_MODE_DRAGGED;
      } else {
        tab_button_group->scrollable_mode_flags = TAB_BUTTON_GROUP_SCROLLABLE_MODE_ALL;
      }
      return RET_OK;
    }
    return RET_FAIL;
  }

  if (tab_button_group->hscrollable != NULL) {
    return hscrollable_set_prop(tab_button_group->hscrollable, name, v);
  } else {
    return RET_NOT_FOUND;
  }
}

static ret_t tab_button_group_on_paint_children(widget_t* widget, canvas_t* c) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  return hscrollable_on_paint_children(tab_button_group->hscrollable, c);
}

static ret_t tab_button_group_update_active(widget_t* widget) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
  if (widget_get_value(iter)) {
    tab_button_group->active = i;
    return RET_OK;
  }
  WIDGET_FOR_EACH_CHILD_END();

  tab_button_group->active = -1;
  return RET_OK;
}

static ret_t tab_button_group_ensure_active(widget_t* widget) {
  widget_t* first = NULL;
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  tab_button_group_update_active(widget);

  if (-1 != tab_button_group->active) {
    return RET_OK;
  }

  first = widget_get_child(widget, 0);
  if (first != NULL) {
    tab_button_group->active = 0;
    widget_set_value(first, TRUE);
  }

  return RET_OK;
}

static bool_t tab_button_group_active_is_valid(widget_t* widget, uint32_t active) {
  return (active < widget_count_children(widget));
}

static ret_t tab_button_group_last_iter_rect_destroy(tab_button_group_t* tab_button_group) {
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);
  if (tab_button_group->last_iter_rect != NULL) {
    rect_destroy(tab_button_group->last_iter_rect);
    tab_button_group->last_iter_rect = NULL;
  }
  return RET_OK;
}

static ret_t tab_button_group_on_pointer_down(widget_t* widget, pointer_event_t* e) {
  widget_t* target = NULL;
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL && e != NULL, RET_BAD_PARAMS);

  target = widget_find_target(widget, e->x, e->y);
  if (target != NULL) {
    if (tab_button_group->drag_child) {
      widget_grab(widget->parent, widget);
      tab_button_group->dragged = target;
    }
    widget_set_prop_bool(target, WIDGET_PROP_VALUE, TRUE);
    tab_button_group_last_iter_rect_destroy(tab_button_group);
  }
  return RET_OK;
}

static ret_t tab_button_group_on_pointer_move(widget_t* widget, pointer_event_t* e) {
  point_t p;
  ret_t ret = RET_OK;
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL && e != NULL, RET_BAD_PARAMS);
  p.x = e->x;
  p.y = e->y;
  ret = widget_to_local(widget->parent, &p);
  p.x -= widget->x;
  p.y -= widget->y;
  if (ret == RET_OK && p.y >= 0 && p.y <= widget->h) {
    if (p.x > 0 && p.x < widget->w) {
      p.x = e->x;
      p.y = e->y;
      if (widget_to_local(widget, &p) == RET_OK) {
        WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)
        xy_t xx = p.x - iter->x;
        xy_t yy = p.y - iter->y;
        if (widget_is_point_in(iter, xx, yy, TRUE)) {
          if (iter != tab_button_group->dragged) {
            if (!(tab_button_group->last_iter_rect != NULL && rect_contains(tab_button_group->last_iter_rect, p.x, p.y))) {
              tab_button_group_last_iter_rect_destroy(tab_button_group);
              tab_button_group->last_iter_rect = rect_create(iter->x, iter->y, iter->w, iter->h);
              tab_button_group_set_active_in_viewport_on_layout(tab_button_group, TRUE);
              tab_button_restack(tab_button_group->dragged, i);
              widget_layout(widget);
            }
          } else {
            tab_button_group_last_iter_rect_destroy(tab_button_group);
          }
          break;
        }
        WIDGET_FOR_EACH_CHILD_END();
      }
    } else if (p.x <= 0) {
      if (p.x <= -TK_TAB_BUTTON_DRAG_THRESHOLD && tab_button_group->last_pointer_x > e->x) {
        int32_t index = widget_index_of(tab_button_group->dragged);
        tab_button_group_set_active_in_viewport_on_layout(tab_button_group, TRUE);
        tab_button_restack(tab_button_group->dragged, tk_max(index - 1, 0));
        widget_layout(widget);
      }
    } else {
      if (p.x >= TK_TAB_BUTTON_DRAG_THRESHOLD + widget->w && tab_button_group->last_pointer_x < e->x) {
        int32_t index = widget_index_of(tab_button_group->dragged);
        int32_t count = widget_count_children(widget);
        tab_button_group_set_active_in_viewport_on_layout(tab_button_group, TRUE);
        tab_button_restack(tab_button_group->dragged, tk_min(index + 1, count));
        widget_layout(widget);
      }
    }
  }
  tab_button_group->last_pointer_x = e->x;
  return RET_OK;
}

static ret_t tab_button_group_on_event(widget_t* widget, event_t* e) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  switch (e->type) {
    case EVT_WINDOW_WILL_OPEN: {
      tab_button_group_ensure_active(widget);
      break;
    }
    case EVT_WIDGET_REMOVE_CHILD: {
      if (tab_button_group_active_is_valid(widget, tab_button_group->active)) {
        widget_set_value(widget_get_child(widget, tab_button_group->active), TRUE);
      }
      break;
    }
    case EVT_WHEEL: {
      if (tab_button_group->scrollable && tab_button_group->compact && 
          (tab_button_group->scrollable_mode_flags & TAB_BUTTON_GROUP_SCROLLABLE_MODE_WHELL)) {
        wheel_event_t* evt = (wheel_event_t*)e;
        int32_t xoffset = tab_button_group->hscrollable->xoffset;

        xoffset -= evt->dy;
        if ((xoffset + widget->w) > tab_button_group->hscrollable->virtual_w) {
          xoffset = tab_button_group->hscrollable->virtual_w - widget->w;
        }
        if (xoffset < 0) {
          xoffset = 0;
        }
        tab_button_group_set_active_in_viewport_on_layout(tab_button_group, FALSE);
        hscrollable_set_xoffset(tab_button_group->hscrollable, xoffset);
        widget_invalidate(widget, NULL);
      }
      break;
    }
    case EVT_POINTER_DOWN:
      tab_button_group_on_pointer_down(widget, (pointer_event_t*)e);
      break;
    case EVT_POINTER_UP: {
      if (tab_button_group->drag_child) {
        pointer_event_t* evt = (pointer_event_t*)e;
        tab_button_group->dragged = NULL;
        widget_ungrab(widget->parent, widget);
      }
      break;
    }
    case EVT_POINTER_MOVE: {
      if (tab_button_group->drag_child) {
        pointer_event_t* evt = (pointer_event_t*)e;
        if (!evt->pressed) {
          break;
        }
        if (tab_button_group->dragged != NULL) {
          tab_button_group_on_pointer_move(widget, (pointer_event_t*)e);
        }
      }
      break;
    }
    default:
      break;
  }

  if (tab_button_group->scrollable && tab_button_group->compact && !tab_button_group->drag_child && 
      (tab_button_group->scrollable_mode_flags & TAB_BUTTON_GROUP_SCROLLABLE_MODE_DRAGGED)) {
    int32_t xoffset = tab_button_group->hscrollable->xoffset;
    ret_t ret = hscrollable_on_event(tab_button_group->hscrollable, e);
    if (tab_button_group->is_active_in_viewport) {
      tab_button_group_set_active_in_viewport_on_layout(tab_button_group, xoffset == tab_button_group->hscrollable->xoffset);
    }
    return ret;
  }
  return RET_OK;
}

ret_t tab_button_group_set_compact(widget_t* widget, bool_t compact) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  tab_button_group->compact = compact;

  return RET_OK;
}

ret_t tab_button_group_set_scrollable(widget_t* widget, bool_t scrollable) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  tab_button_group->scrollable = scrollable;

  return RET_OK;
}

ret_t tab_button_group_set_drag_child(widget_t* widget, bool_t drag_child) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  tab_button_group->drag_child = drag_child;

  return RET_OK;
}


static ret_t tab_button_group_on_destroy(widget_t* widget) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(widget != NULL && tab_button_group != NULL, RET_BAD_PARAMS);

  hscrollable_destroy(tab_button_group->hscrollable);
  tab_button_group_last_iter_rect_destroy(tab_button_group);

  return RET_OK;
}

static ret_t tab_button_group_get_offset(widget_t* widget, xy_t* out_x, xy_t* out_y) {
  return_value_if_fail(widget != NULL && out_x != NULL && out_y != NULL, RET_BAD_PARAMS);
  *out_x = widget_get_prop_int(widget, WIDGET_PROP_XOFFSET, 0);
  *out_y = widget_get_prop_int(widget, WIDGET_PROP_YOFFSET, 0);
  return RET_OK;
}

static ret_t tab_button_group_on_remove_child(widget_t* widget, widget_t* child) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(
      widget != NULL && widget->children != NULL && tab_button_group != NULL && child != NULL,
      RET_BAD_PARAMS);

  if (!widget->destroying) {
    int32_t remove_index = widget_index_of(child);
    return_value_if_fail(remove_index >= 0, RET_BAD_PARAMS);

    tab_button_group_update_active(widget);

    if (-1 != tab_button_group->active) {
      uint32_t children_num = widget->children->size;
      if (remove_index < tab_button_group->active ||
          (remove_index == tab_button_group->active && remove_index == children_num - 1)) {
        tab_button_group->active--;
      }
    }
  }

  return RET_CONTINUE;
}

static ret_t tab_button_group_init(widget_t* widget) {
  tab_button_group_t* tab_button_group = TAB_BUTTON_GROUP(widget);
  return_value_if_fail(tab_button_group != NULL, RET_BAD_PARAMS);

  tab_button_group->hscrollable = hscrollable_create(widget);
  tab_button_group->is_active_in_viewport = TRUE;
  tab_button_group->scrollable_mode_flags = TAB_BUTTON_GROUP_SCROLLABLE_MODE_ALL;
  return RET_OK;
}

TK_DECL_VTABLE(tab_button_group) = {.size = sizeof(tab_button_group_t),
                                    .type = WIDGET_TYPE_TAB_BUTTON_GROUP,
                                    .scrollable = TRUE,
                                    .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
                                    .create = tab_button_group_create,
                                    .set_prop = tab_button_group_set_prop,
                                    .get_prop = tab_button_group_get_prop,
                                    .on_event = tab_button_group_on_event,
                                    .on_destroy = tab_button_group_on_destroy,
                                    .get_offset = tab_button_group_get_offset,
                                    .on_remove_child = tab_button_group_on_remove_child,
                                    .on_paint_children = tab_button_group_on_paint_children,
                                    .on_layout_children = tab_button_group_on_layout_children};

widget_t* tab_button_group_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(tab_button_group), x, y, w, h);
  return_value_if_fail(tab_button_group_init(widget) == RET_OK, NULL);

  return widget;
}

widget_t* tab_button_group_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, tab_button_group), NULL);

  return widget;
}
