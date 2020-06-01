/**
 * File:   list_item.h
 * Author: AWTK Develop Team
 * Brief:  list_item
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
 * 2018-07-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/timer.h"
#include "scroll_view/list_item.h"

static ret_t list_item_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t list_item_on_parent_pointer_up(void* ctx, event_t* e) {
  list_item_t* list_item = LIST_ITEM(ctx);

  list_item->dragged = FALSE;
  list_item->pressed = FALSE;
  list_item->downed = FALSE;

  return RET_REMOVE;
}

static ret_t list_item_on_detach_parent(widget_t* widget, widget_t* parent) {
  return_value_if_fail(widget != NULL && parent != NULL, RET_BAD_PARAMS);

  widget_off_by_func(parent, EVT_POINTER_UP, list_item_on_parent_pointer_up, widget);
  return RET_OK;
}

static ret_t list_item_on_timer(const timer_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);
  list_item_t* list_item = LIST_ITEM(widget);
  return_value_if_fail(widget != NULL && list_item != NULL, RET_BAD_PARAMS);

  if (!list_item->dragged) {
    widget_set_state(widget, WIDGET_STATE_PRESSED);
  }

  list_item->timer_id = TK_INVALID_ID;

  return RET_REMOVE;
}

static ret_t list_item_remove_timer(widget_t* widget) {
  list_item_t* list_item = LIST_ITEM(widget);
  return_value_if_fail(list_item != NULL, RET_BAD_PARAMS);

  if (list_item->timer_id != TK_INVALID_ID) {
    timer_remove(list_item->timer_id);
    list_item->timer_id = TK_INVALID_ID;
  }

  return RET_OK;
}

static ret_t list_item_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  list_item_t* list_item = LIST_ITEM(widget);
  return_value_if_fail(list_item != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t* evt = (pointer_event_t*)e;
      list_item->downed = TRUE;
      list_item->down.x = evt->x;
      list_item->down.y = evt->y;
      list_item->pressed = TRUE;
      widget_grab(widget->parent, widget);
      list_item->timer_id = timer_add(list_item_on_timer, widget, 30);
      widget_invalidate_force(widget, NULL);
      widget_off_by_func(widget->parent, EVT_POINTER_UP, list_item_on_parent_pointer_up, widget);
      widget_on(widget->parent, EVT_POINTER_UP, list_item_on_parent_pointer_up, widget);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      list_item->dragged = FALSE;
      list_item->pressed = FALSE;
      list_item->downed = FALSE;
      widget_ungrab(widget->parent, widget);
      list_item_remove_timer(widget);
      widget_invalidate_force(widget, NULL);
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    }
    case EVT_POINTER_UP: {
      list_item_remove_timer(widget);
      widget_invalidate_force(widget, NULL);
      widget_set_state(widget, WIDGET_STATE_NORMAL);

      if (!list_item->dragged && list_item->pressed) {
        pointer_event_t evt = *(pointer_event_t*)e;
        evt.e = event_init(EVT_CLICK, widget);
        widget_dispatch(widget, (event_t*)&evt);
      }
      list_item->dragged = FALSE;
      list_item->pressed = FALSE;
      list_item->downed = FALSE;
      widget_ungrab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      uint32_t dy = tk_abs(evt->y - list_item->down.y);

      if (list_item->downed && evt->pressed && dy > TK_DRAG_THRESHOLD) {
        list_item->dragged = TRUE;
        list_item->pressed = FALSE;
        widget_ungrab(widget->parent, widget);
        list_item_remove_timer(widget);
        widget_set_state(widget, WIDGET_STATE_NORMAL);
      }

      break;
    }
    case EVT_POINTER_LEAVE:
      list_item_remove_timer(widget);
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    case EVT_POINTER_ENTER:
      if (list_item->dragged) {
        widget_set_state(widget, WIDGET_STATE_NORMAL);
      } else {
        widget_set_state(widget, WIDGET_STATE_OVER);
      }
      break;
    default:
      break;
  }

  return RET_OK;
}

static ret_t list_item_on_destroy(widget_t* widget) {
  list_item_t* list_item = LIST_ITEM(widget);
  return_value_if_fail(list_item != NULL, RET_BAD_PARAMS);

  if (list_item->timer_id != TK_INVALID_ID) {
    timer_remove(list_item->timer_id);
    list_item->timer_id = TK_INVALID_ID;
  }

  return RET_OK;
}

TK_DECL_VTABLE(list_item) = {.size = sizeof(list_item_t),
                             .type = WIDGET_TYPE_LIST_ITEM,
                             .space_key_to_activate = TRUE,
                             .return_key_to_activate = TRUE,
                             .parent = TK_PARENT_VTABLE(widget),
                             .create = list_item_create,
                             .on_event = list_item_on_event,
                             .on_paint_self = list_item_on_paint_self,
                             .on_detach_parent = list_item_on_detach_parent,
                             .on_destroy = list_item_on_destroy};

widget_t* list_item_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(list_item), x, y, w, h);
  list_item_t* list_item = LIST_ITEM(widget);
  return_value_if_fail(list_item != NULL, NULL);

  list_item->timer_id = TK_INVALID_ID;

  return widget;
}

widget_t* list_item_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, list_item), NULL);

  return widget;
}
