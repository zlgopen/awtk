/**
 * File:   list_item.h
 * Author: AWTK Develop Team
 * Brief:  list_item
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
 * 2018-07-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/timer.h"
#include "scroll_view/list_item.h"

static ret_t list_item_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t list_item_on_timer(const timer_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);
  list_item_t* list_item = LIST_ITEM(widget);

  if (!list_item->dragged) {
    widget_set_state(widget, WIDGET_STATE_PRESSED);
  }
  list_item->timer_id = TK_INVALID_ID;

  return RET_REMOVE;
}

static ret_t list_item_remove_timer(widget_t* widget) {
  list_item_t* list_item = LIST_ITEM(widget);
  if (list_item->timer_id != TK_INVALID_ID) {
    timer_remove(list_item->timer_id);
    list_item->timer_id = TK_INVALID_ID;
  }

  return RET_OK;
}

static ret_t list_item_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  list_item_t* list_item = LIST_ITEM(widget);

  switch (type) {
    case EVT_POINTER_DOWN: {
      pointer_event_t* evt = (pointer_event_t*)e;
      list_item->down.x = evt->x;
      list_item->down.y = evt->y;
      list_item->timer_id = timer_add(list_item_on_timer, widget, 30);
      widget_invalidate_force(widget);
      break;
    }
    case EVT_POINTER_DOWN_ABORT: {
      list_item_remove_timer(widget);
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    }
    case EVT_POINTER_UP: {
      if (!list_item->dragged) {
        pointer_event_t evt = *(pointer_event_t*)e;
        evt.e = event_init(EVT_CLICK, widget);
        widget_dispatch(widget, (event_t*)&evt);
      }

      list_item->dragged = FALSE;
      list_item_remove_timer(widget);
      widget_invalidate_force(widget);
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    }
    case EVT_POINTER_MOVE: {
      pointer_event_t* evt = (pointer_event_t*)e;
      uint32_t dy = tk_abs(evt->y - list_item->down.y);

      if (dy > 6) {
        list_item->dragged = TRUE;
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

static const widget_vtable_t s_list_item_vtable = {.size = sizeof(list_item_t),
                                                   .type = WIDGET_TYPE_LIST_ITEM,
                                                   .create = list_item_create,
                                                   .on_event = list_item_on_event,
                                                   .on_paint_self = list_item_on_paint_self};

widget_t* list_item_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  list_item_t* list_item = TKMEM_ZALLOC(list_item_t);
  widget_t* widget = WIDGET(list_item);
  return_value_if_fail(list_item != NULL, NULL);

  widget_init(widget, parent, &s_list_item_vtable, x, y, w, h);
  list_item->timer_id = TK_INVALID_ID;

  return widget;
}

widget_t* list_item_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_list_item_vtable, NULL);

  return widget;
}
