/**
 * File:   list_view.h
 * Author: AWTK Develop Team
 * Brief:  list_view
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
 * 2018-07-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/layout.h"
#include "base/list_view.h"
#include "base/scroll_bar.h"
#include "base/scroll_view.h"

static ret_t list_view_on_add_child(widget_t* widget, widget_t* child);

static ret_t list_view_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t list_view_get_prop(widget_t* widget, const char* name, value_t* v) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ITEM_HEIGHT)) {
    value_set_uint8(v, list_view->item_height);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t list_view_set_prop(widget_t* widget, const char* name, const value_t* v) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ITEM_HEIGHT)) {
    list_view->item_height = value_int(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const widget_vtable_t s_list_view_vtable = {.type_name = WIDGET_TYPE_LIST_VIEW,
                                                   .set_prop = list_view_set_prop,
                                                   .get_prop = list_view_get_prop,
                                                   .on_add_child = list_view_on_add_child,
                                                   .on_paint_self = list_view_on_paint_self};

static ret_t list_view_on_scroll_view_scroll(widget_t* widget, int32_t xoffset, int32_t yoffset) {
  list_view_t* list_view = LIST_VIEW(widget->parent);
 
  scroll_bar_set_value_only(list_view->scroll_bar, yoffset+widget->h);

  return RET_OK;
}

static ret_t list_view_on_scroll_view_scroll_to(widget_t* widget, int32_t xoffset_end, int32_t yoffset_end, int32_t duration) {
  list_view_t* list_view = LIST_VIEW(widget->parent);

 // scroll_bar_scroll_to(list_view->scroll_bar, yoffset_end, duration);

  return RET_OK;
}

static ret_t list_view_on_scroll_view_layout_children(widget_t* widget) {
  list_view_t* list_view = LIST_VIEW(widget->parent);
  int32_t virtual_h = widget->h;
  int32_t item_height = list_view->item_height;

  if (widget->children != NULL) {
    int32_t i = 0;
    int32_t n = 0;
    int32_t x = 0;
    int32_t y = 0;
    int32_t w = widget->w;
    int32_t h = item_height;

    for (i = 0, n = widget->children->size; i < n; i++) {
      widget_t* iter = (widget_t*)(widget->children->elms[i]);

      if (item_height <= 0) {
        h = iter->h;
      }

      widget_move_resize(iter, x, y, w, h);
      widget_layout(iter);

      y = iter->y + iter->h;

      if (y > virtual_h) {
        virtual_h = y;
      }
    }
  }

  scroll_view_set_virtual_h(list_view->scroll_view, virtual_h);
  scroll_bar_set_params(list_view->scroll_bar, virtual_h, list_view->item_height);

  return RET_OK;
}

static ret_t list_view_on_scroll_bar_value_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  list_view_t* list_view = LIST_VIEW(widget);
  scroll_view_t* scroll_view = SCROLL_VIEW(list_view->scroll_view);
  scroll_bar_t* scroll_bar = SCROLL_BAR(list_view->scroll_bar);

  int32_t value =
      (scroll_view->virtual_h - list_view->scroll_view->h) * scroll_bar->value / scroll_bar->max;
  scroll_view_set_offset(list_view->scroll_view, 0, value);

  return RET_OK;
}

static ret_t list_view_on_add_child(widget_t* widget, widget_t* child) {
  list_view_t* list_view = LIST_VIEW(widget);
  if (child->type == WIDGET_SCROLL_VIEW) {
    scroll_view_t* scroll_view = SCROLL_VIEW(child);

    list_view->scroll_view = child;
    scroll_view->on_scroll = list_view_on_scroll_view_scroll;
    scroll_view->on_scroll_to = list_view_on_scroll_view_scroll_to;
    scroll_view->on_layout_children = list_view_on_scroll_view_layout_children;
  } else if (child->type == WIDGET_SCROLL_BAR) {
    list_view->scroll_bar = child;
    widget_on(list_view->scroll_bar, EVT_VALUE_CHANGED, list_view_on_scroll_bar_value_changed,
              widget);
  }

  return RET_FAIL;
}

widget_t* list_view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  list_view_t* list_view = TKMEM_ZALLOC(list_view_t);
  return_value_if_fail(list_view != NULL, NULL);

  widget = WIDGET(list_view);
  widget->vt = &s_list_view_vtable;
  widget_init(widget, parent, WIDGET_LIST_VIEW);
  widget_move_resize(widget, x, y, w, h);

  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return widget;
}

ret_t list_view_set_item_height(widget_t* widget, int32_t item_height) {
  list_view_t* list_view = LIST_VIEW(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  list_view->item_height = item_height;

  return RET_OK;
}
