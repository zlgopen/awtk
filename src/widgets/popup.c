/**
 * File:   popup.c
 * Author: AWTK Develop Team
 * Brief:  popup
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/idle.h"
#include "base/window.h"
#include "widgets/label.h"
#include "base/enums.h"
#include "tkc/utils.h"
#include "base/widget.h"
#include "base/layout.h"
#include "widgets/popup.h"
#include "base/image_manager.h"
#include "base/window_manager.h"

static ret_t popup_get_prop(widget_t* widget, const char* name, value_t* v) {
  popup_t* popup = POPUP(widget);
  return_value_if_fail(popup != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_CLOSE_WHEN_CLICK)) {
    value_set_bool(v, popup->close_when_click);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE)) {
    value_set_bool(v, popup->close_when_click_outside);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_WHEN_TIMEOUT)) {
    value_set_uint32(v, popup->close_when_timeout);
    return RET_OK;
  }

  return window_base_get_prop(widget, name, v);
}

static ret_t popup_set_prop(widget_t* widget, const char* name, const value_t* v) {
  popup_t* popup = POPUP(widget);
  return_value_if_fail(popup != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_CLOSE_WHEN_CLICK)) {
    popup->close_when_click = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE)) {
    popup->close_when_click_outside = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_WHEN_TIMEOUT)) {
    popup_set_close_when_timeout(widget, value_uint32(v));
    return RET_OK;
  }

  return window_base_set_prop(widget, name, v);
}

static ret_t popup_idle_window_close(const idle_info_t* idle) {
  widget_t* widget = WIDGET(idle->ctx);
  widget_t* win = widget->parent;
  return_value_if_fail(win && widget != NULL, RET_REMOVE);

  widget_ungrab(win, widget);

  if (window_manager_is_animating(win)) {
    window_close_force(widget);
  } else {
    window_close(widget);
  }
  return RET_REMOVE;
}

static ret_t popup_update_close_timer(widget_t* widget) {
  popup_t* popup = POPUP(widget);
  return_value_if_fail(popup && widget != NULL, RET_BAD_PARAMS);

  if (popup->timer_id != TK_INVALID_ID && popup->close_when_timeout > 0) {
    timer_modify(popup->timer_id, popup->close_when_timeout);
  }

  return RET_OK;
}

static ret_t popup_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  popup_t* popup = POPUP(widget);
  return_value_if_fail(popup && widget != NULL, RET_BAD_PARAMS);

  switch (type) {
    case EVT_WINDOW_OPEN: {
      widget_grab(widget->parent, widget);
      break;
    }
    case EVT_POINTER_DOWN: {
      popup->is_outside = FALSE;
      if (popup->close_when_click_outside) {
        pointer_event_t* evt = (pointer_event_t*)e;
        rect_t r = rect_init(widget->x, widget->y, widget->w, widget->h);
        popup->is_outside = !rect_contains(&r, evt->x, evt->y);
      }
      break;
    }
    case EVT_POINTER_UP: {
      window_base_t* window_base = WINDOW_BASE(popup);
      ENSURE(window_base != NULL);

      if (window_base->stage != WINDOW_STAGE_CLOSED) {
        bool_t close_window = FALSE;
        pointer_event_t* evt = (pointer_event_t*)e;

        if (popup->close_when_click) {
          close_window = TRUE;
        } else if (popup->close_when_click_outside && popup->is_outside) {
          rect_t r = rect_init(widget->x, widget->y, widget->w, widget->h);
          if (!rect_contains(&r, evt->x, evt->y)) {
            close_window = TRUE;
          }
        }

        if (close_window) {
          widget_add_idle(widget, popup_idle_window_close);
        }
      }

      break;
    }
    default:
      break;
  }

  switch (type) {
    case EVT_POINTER_DOWN:
    case EVT_POINTER_MOVE:
    case EVT_POINTER_UP:
    case EVT_KEY_DOWN:
    case EVT_KEY_UP: {
      popup_update_close_timer(widget);
      break;
    }
    default:
      break;
  }

  return window_base_on_event(widget, e);
}

static const char* const s_popup_properties[] = {WIDGET_PROP_CLOSE_WHEN_CLICK,
                                                 WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE,
                                                 WIDGET_PROP_CLOSE_WHEN_TIMEOUT, NULL};

TK_DECL_VTABLE(popup) = {.size = sizeof(popup_t),
                         .type = WIDGET_TYPE_POPUP,
                         .is_window = TRUE,
                         .clone_properties = s_popup_properties,
                         .persistent_properties = s_popup_properties,
                         .parent = TK_PARENT_VTABLE(window_base),
                         .create = popup_create,
                         .get_prop = popup_get_prop,
                         .set_prop = popup_set_prop,
                         .on_event = popup_on_event,
                         .on_paint_self = window_base_on_paint_self,
                         .on_paint_begin = window_base_on_paint_begin,
                         .on_paint_end = window_base_on_paint_end,
                         .on_destroy = window_base_on_destroy};

widget_t* popup_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return window_base_create(parent, TK_REF_VTABLE(popup), x, y, w, h);
}

widget_t* popup_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, popup), NULL);

  return widget;
}

ret_t popup_set_close_when_click(widget_t* widget, bool_t close_when_click) {
  popup_t* popup = POPUP(widget);
  return_value_if_fail(popup != NULL, RET_FAIL);

  popup->close_when_click = close_when_click;

  return RET_OK;
}

ret_t popup_set_close_when_click_outside(widget_t* widget, bool_t close_when_click_outside) {
  popup_t* popup = POPUP(widget);
  return_value_if_fail(popup != NULL, RET_FAIL);

  popup->close_when_click_outside = close_when_click_outside;

  return RET_OK;
}

static ret_t popup_on_timeout(const timer_info_t* info) {
  window_close(WIDGET(info->ctx));

  return RET_REMOVE;
}

ret_t popup_set_close_when_timeout(widget_t* widget, uint32_t close_when_timeout) {
  popup_t* popup = POPUP(widget);
  return_value_if_fail(popup != NULL, RET_FAIL);

  popup->close_when_timeout = close_when_timeout;
  if (popup->timer_id != TK_INVALID_ID) {
    timer_remove(popup->timer_id);
    popup->timer_id = TK_INVALID_ID;
  }

  if (close_when_timeout > 0) {
    popup->timer_id = widget_add_timer(widget, popup_on_timeout, close_when_timeout);
  }

  return RET_OK;
}
