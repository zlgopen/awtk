/**
 * File:   list_view.h
 * Author: AWTK Develop Team
 * Brief:  list_view
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
 * 2018-07-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/time_now.h"
#include "base/layout.h"
#include "scroll_view/list_view.h"
#include "scroll_view/scroll_bar.h"
#include "scroll_view/scroll_view.h"

static ret_t list_view_on_add_child(widget_t* widget, widget_t* child);
static ret_t list_view_on_remove_child(widget_t* widget, widget_t* child);

static ret_t list_view_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t list_view_get_prop(widget_t* widget, const char* name, value_t* v) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(list_view != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ITEM_HEIGHT)) {
    value_set_int(v, list_view->item_height);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DEFAULT_ITEM_HEIGHT)) {
    value_set_int(v, list_view->default_item_height);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_HIDE_SCROLL_BAR)) {
    value_set_bool(v, list_view->auto_hide_scroll_bar);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t list_view_on_pointer_up(list_view_t* list_view, pointer_event_t* e) {
  scroll_bar_t* scroll_bar = (scroll_bar_t*)list_view->scroll_bar;
  if (scroll_bar != NULL && scroll_bar->wa_opactiy == NULL && list_view->scroll_bar->visible) {
    scroll_bar_hide_by_opacity_animation(list_view->scroll_bar, 500);
  }
  return RET_OK;
}
static ret_t list_view_set_prop(widget_t* widget, const char* name, const value_t* v) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(list_view != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ITEM_HEIGHT)) {
    list_view->item_height = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_DEFAULT_ITEM_HEIGHT)) {
    list_view->default_item_height = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_AUTO_HIDE_SCROLL_BAR)) {
    list_view->auto_hide_scroll_bar = value_bool(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t list_view_on_event(widget_t* widget, event_t* e) {
  ret_t ret = RET_OK;
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(list_view != NULL, RET_BAD_PARAMS);

  switch (e->type) {
    case EVT_WHEEL: {
      wheel_event_t* evt = (wheel_event_t*)e;
      int32_t delta = -evt->dy;
      if (list_view->scroll_bar != NULL) {
        scroll_bar_add_delta(list_view->scroll_bar, delta);
        log_debug("wheel: %d\n", delta);
      }

      ret = RET_STOP;
      break;
    }
    case EVT_KEY_DOWN: {
      key_event_t* evt = (key_event_t*)e;
      if (evt->key == TK_KEY_PAGEDOWN) {
        scroll_view_scroll_delta_to(list_view->scroll_view, 0, widget->h, TK_ANIMATING_TIME);
        ret = RET_STOP;
      } else if (evt->key == TK_KEY_PAGEUP) {
        scroll_view_scroll_delta_to(list_view->scroll_view, 0, -widget->h, TK_ANIMATING_TIME);
        ret = RET_STOP;
      } else if (evt->key == TK_KEY_UP) {
        uint32_t item_height = tk_max(list_view->item_height, 20);
        scroll_view_scroll_delta_to(list_view->scroll_view, 0, -item_height, TK_ANIMATING_TIME);
        ret = RET_STOP;
      } else if (evt->key == TK_KEY_DOWN) {
        uint32_t item_height = tk_max(list_view->item_height, 20);
        scroll_view_scroll_delta_to(list_view->scroll_view, 0, item_height, TK_ANIMATING_TIME);
        ret = RET_STOP;
      }
      break;
    }
    case EVT_POINTER_UP: {
      pointer_event_t* evt = (pointer_event_t*)e;
      list_view_on_pointer_up(list_view, evt);
    } break;
    default:
      break;
  }
  return ret;
}

TK_DECL_VTABLE(list_view) = {.type = WIDGET_TYPE_LIST_VIEW,
                             .size = sizeof(list_view_t),
                             .parent = TK_PARENT_VTABLE(widget),
                             .create = list_view_create,
                             .set_prop = list_view_set_prop,
                             .get_prop = list_view_get_prop,
                             .on_event = list_view_on_event,
                             .on_add_child = list_view_on_add_child,
                             .on_remove_child = list_view_on_remove_child,
                             .on_paint_self = list_view_on_paint_self};

static int32_t scroll_bar_to_scroll_view(list_view_t* list_view, int32_t v) {
  int32_t range = 0;
  float_t percent = 0;
  scroll_view_t* scroll_view = NULL;
  scroll_bar_t* scroll_bar = NULL;
  return_value_if_fail(list_view != NULL, 0);

  scroll_view = SCROLL_VIEW(list_view->scroll_view);
  scroll_bar = SCROLL_BAR(list_view->scroll_bar);
  return_value_if_fail(scroll_bar != NULL && scroll_view != NULL, 0);

  range = scroll_bar->virtual_size;
  percent = range > 0 ? (float_t)v / (float_t)(range) : 0;

  return percent * (scroll_view->virtual_h - list_view->scroll_view->h);
}

static ret_t list_view_on_scroll_bar_value_changed(void* ctx, event_t* e) {
  int32_t offset = 0;
  scroll_bar_t* scroll_bar = NULL;
  list_view_t* list_view = LIST_VIEW(ctx);
  return_value_if_fail(list_view != NULL, RET_REMOVE);

  scroll_bar = SCROLL_BAR(list_view->scroll_bar);
  offset = scroll_bar_to_scroll_view(list_view, scroll_bar->value);
  scroll_view_set_offset(list_view->scroll_view, 0, offset);

  return RET_OK;
}

static int32_t scroll_view_to_scroll_bar(list_view_t* list_view, int32_t v) {
  int32_t range = 0;
  float_t percent = 0;
  scroll_view_t* scroll_view = NULL;
  scroll_bar_t* scroll_bar = NULL;
  return_value_if_fail(list_view != NULL, 0);

  scroll_view = SCROLL_VIEW(list_view->scroll_view);
  scroll_bar = SCROLL_BAR(list_view->scroll_bar);
  return_value_if_fail(scroll_bar != NULL && scroll_view != NULL, 0);

  range = scroll_view->virtual_h - list_view->scroll_view->h;
  percent = range > 0 ? (float_t)v / (float_t)range : 0;

  return percent * scroll_bar->virtual_size;
}

static ret_t list_view_on_scroll_view_scroll(widget_t* widget, int32_t xoffset, int32_t yoffset) {
  list_view_t* list_view = LIST_VIEW(widget->parent);
  return_value_if_fail(list_view != NULL, RET_BAD_PARAMS);

  if (list_view->scroll_bar != NULL) {
    int32_t value = scroll_view_to_scroll_bar(list_view, yoffset);
    scroll_bar_set_value_only(list_view->scroll_bar, value);

    if (scroll_bar_is_mobile(list_view->scroll_bar)) {
      widget_set_opacity(list_view->scroll_bar, 0xff);
      widget_set_visible_only(list_view->scroll_bar, TRUE);
    }
  }

  return RET_OK;
}

static ret_t list_view_on_scroll_view_scroll_to(widget_t* widget, int32_t xoffset_end,
                                                int32_t yoffset_end, int32_t duration) {
  list_view_t* list_view = LIST_VIEW(widget->parent);
  return_value_if_fail(widget != NULL && list_view != NULL, RET_BAD_PARAMS);

  if (scroll_bar_is_mobile(list_view->scroll_bar)) {
    int32_t value = scroll_view_to_scroll_bar(list_view, yoffset_end);

    emitter_disable(list_view->scroll_bar->emitter);
    scroll_bar_scroll_to(list_view->scroll_bar, value, duration);
    emitter_enable(list_view->scroll_bar->emitter);

    (void)xoffset_end;
  }

  return RET_OK;
}

static ret_t list_view_on_scroll_view_layout_children(widget_t* widget) {
  if (widget->children_layout == NULL) {
    widget_set_children_layout(widget, "list_view(m=0,s=0)");
  }
  return_value_if_fail(widget->children_layout != NULL, RET_BAD_PARAMS);

  children_layouter_layout(widget->children_layout, widget);
  return RET_OK;
}

static ret_t list_view_on_scroll_view_paint_children(widget_t* widget, canvas_t* c) {
  int32_t left = 0;
  int32_t top = 0;
  int32_t bottom = 0;
  int32_t right = 0;
  int32_t max_w = canvas_get_width(c);
  int32_t max_h = canvas_get_height(c);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget, iter, i)

  if (!iter->visible) {
    iter->dirty = FALSE;
    continue;
  }

  left = c->ox + iter->x;
  top = c->oy + iter->y;
  bottom = top + iter->h;
  right = left + iter->w;

  if (top > max_h || left > max_w) {
    break;
  }

  if (bottom < 0 || right < 0) {
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

static ret_t list_view_on_add_child(widget_t* widget, widget_t* child) {
  list_view_t* list_view = LIST_VIEW(widget);
  const char* type = widget_get_type(child);
  return_value_if_fail(list_view != NULL && widget != NULL && child != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(type, WIDGET_TYPE_SCROLL_VIEW)) {
    scroll_view_t* scroll_view = (scroll_view_t*)list_view->scroll_view;

    if (scroll_view != NULL) {
      scroll_view->on_scroll = NULL;
      scroll_view->on_scroll_to = NULL;
      scroll_view->on_layout_children = NULL;
    }

    list_view->scroll_view = child;
    scroll_view = SCROLL_VIEW(child);
    scroll_view->on_scroll = list_view_on_scroll_view_scroll;
    scroll_view->on_scroll_to = list_view_on_scroll_view_scroll_to;
    scroll_view->on_layout_children = list_view_on_scroll_view_layout_children;
    scroll_view->on_paint_children = list_view_on_scroll_view_paint_children;

  } else if (tk_str_eq(type, WIDGET_TYPE_SCROLL_BAR) ||
             tk_str_eq(type, WIDGET_TYPE_SCROLL_BAR_DESKTOP) ||
             tk_str_eq(type, WIDGET_TYPE_SCROLL_BAR_MOBILE)) {
    if (list_view->scroll_bar != NULL) {
      widget_off_by_func(list_view->scroll_bar, EVT_VALUE_CHANGED,
                         list_view_on_scroll_bar_value_changed, widget);
    }
    list_view->scroll_bar = child;
    widget_on(child, EVT_VALUE_CHANGED, list_view_on_scroll_bar_value_changed, widget);
  }

  return RET_CONTINUE;
}

static ret_t list_view_on_remove_child(widget_t* widget, widget_t* child) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(list_view != NULL && widget != NULL && child != NULL, RET_BAD_PARAMS);
  if (list_view->scroll_view == child) {
    scroll_view_t* scroll_view = SCROLL_VIEW(child);
    scroll_view->on_scroll = NULL;
    scroll_view->on_scroll_to = NULL;
    scroll_view->on_layout_children = NULL;

    list_view->scroll_view = NULL;
    WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
    if (iter && iter != child && tk_str_eq(iter->vt->type, WIDGET_TYPE_SCROLL_VIEW)) {
      list_view->scroll_view = iter;
      scroll_view->on_scroll = list_view_on_scroll_view_scroll;
      scroll_view->on_scroll_to = list_view_on_scroll_view_scroll_to;
      scroll_view->on_layout_children = list_view_on_scroll_view_layout_children;
      break;
    }
    WIDGET_FOR_EACH_CHILD_END();
  } else if (list_view->scroll_bar == child) {
    widget_off_by_func(child, EVT_VALUE_CHANGED, list_view_on_scroll_bar_value_changed, widget);
    list_view->scroll_bar = NULL;
    WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
    if (iter && iter != child &&
        (tk_str_eq(iter->vt->type, WIDGET_TYPE_SCROLL_BAR) ||
         tk_str_eq(iter->vt->type, WIDGET_TYPE_SCROLL_BAR_DESKTOP) ||
         tk_str_eq(iter->vt->type, WIDGET_TYPE_SCROLL_BAR_MOBILE))) {
      list_view->scroll_bar = iter;
      widget_on(iter, EVT_VALUE_CHANGED, list_view_on_scroll_bar_value_changed, widget);
      break;
    }
    WIDGET_FOR_EACH_CHILD_END();
  }

  return RET_FAIL;
}

ret_t list_view_reinit(widget_t* widget) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(list_view != NULL && widget != NULL, RET_BAD_PARAMS);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter && tk_str_eq(iter->vt->type, WIDGET_TYPE_SCROLL_VIEW)) {
    if (iter == list_view->scroll_view) break;
    scroll_view_t* scroll_view = SCROLL_VIEW(list_view->scroll_view);
    if (scroll_view != NULL) {
      scroll_view->on_scroll = NULL;
      scroll_view->on_scroll_to = NULL;
      scroll_view->on_layout_children = NULL;
    }
    list_view->scroll_view = iter;
    scroll_view->on_scroll = list_view_on_scroll_view_scroll;
    scroll_view->on_scroll_to = list_view_on_scroll_view_scroll_to;
    scroll_view->on_layout_children = list_view_on_scroll_view_layout_children;
    break;
  }
  WIDGET_FOR_EACH_CHILD_END();

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter && (tk_str_eq(iter->vt->type, WIDGET_TYPE_SCROLL_BAR) ||
               tk_str_eq(iter->vt->type, WIDGET_TYPE_SCROLL_BAR_DESKTOP) ||
               tk_str_eq(iter->vt->type, WIDGET_TYPE_SCROLL_BAR_MOBILE))) {
    if (iter == list_view->scroll_bar) break;
    if (list_view->scroll_bar != NULL) {
      widget_off_by_func(list_view->scroll_bar, EVT_VALUE_CHANGED,
                         list_view_on_scroll_bar_value_changed, widget);
    }
    list_view->scroll_bar = iter;
    widget_on(iter, EVT_VALUE_CHANGED, list_view_on_scroll_bar_value_changed, widget);
    break;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return RET_OK;
}

widget_t* list_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(list_view), x, y, w, h);
}

ret_t list_view_set_item_height(widget_t* widget, int32_t item_height) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(list_view != NULL, RET_BAD_PARAMS);

  list_view->item_height = item_height;

  return RET_OK;
}

ret_t list_view_set_default_item_height(widget_t* widget, int32_t default_item_height) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(list_view != NULL, RET_BAD_PARAMS);

  list_view->default_item_height = default_item_height;

  return RET_OK;
}

ret_t list_view_set_auto_hide_scroll_bar(widget_t* widget, bool_t auto_hide_scroll_bar) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(list_view != NULL, RET_BAD_PARAMS);

  list_view->auto_hide_scroll_bar = auto_hide_scroll_bar;

  return RET_OK;
}

widget_t* list_view_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, list_view), NULL);

  return widget;
}
