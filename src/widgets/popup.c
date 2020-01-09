/**
 * File:   popup.c
 * Author: AWTK Develop Team
 * Brief:  popup
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
  }

  return window_base_set_prop(widget, name, v);
}

static ret_t popup_idle_check_if_need_set_background_state(const idle_info_t* idle) {
  xy_t x = 0;
  xy_t y = 0;
  wh_t right = 0;
  wh_t bottom = 0;
  bool_t is_background = FALSE;
  widget_t* widget = WIDGET(idle->ctx);
  widget_t* win = widget->parent;
  event_t e = event_init(EVT_WINDOW_TO_BACKGROUND, widget);

  if (win != NULL) {
    WIDGET_FOR_EACH_CHILD_BEGIN_R(win, iter, i)
    if (iter == widget) {
      break;
    }
    x = tk_min(x, iter->x);
    y = tk_min(y, iter->y);
    right = tk_max(right, (iter->x + iter->w));
    bottom = tk_max(bottom, (iter->y + iter->h));

    WIDGET_FOR_EACH_CHILD_END();

    if (x < widget->x && y < widget->y && right > (widget->x + widget->w) &&
        bottom > (widget->y + widget->h)) {
      is_background = TRUE;
    }
  }

  if (is_background) {
    widget_dispatch(widget, &e);
  }
  return RET_REMOVE;
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
        } else if (!popup->close_when_click) {
          idle_add(popup_idle_check_if_need_set_background_state, widget);
        }

        if (close_window) {
          idle_add(popup_idle_window_close, widget);
        }
      }

      break;
    }
    default:
      break;
  }

  return window_base_on_event(widget, e);
}

static const char* const s_popup_properties[] = {WIDGET_PROP_ANIM_HINT,
                                                 WIDGET_PROP_OPEN_ANIM_HINT,
                                                 WIDGET_PROP_CLOSE_ANIM_HINT,
                                                 WIDGET_PROP_THEME,
                                                 WIDGET_PROP_CLOSE_WHEN_CLICK,
                                                 WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE,
                                                 WIDGET_PROP_MOVE_FOCUS_PREV_KEY,
                                                 WIDGET_PROP_MOVE_FOCUS_NEXT_KEY,
                                                 NULL};

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
