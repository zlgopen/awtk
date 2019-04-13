/**
 * File:   popup.c
 * Author: AWTK Develop Team
 * Brief:  popup
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/idle.h"
#include "widgets/label.h"
#include "base/enums.h"
#include "tkc/utils.h"
#include "base/layout.h"
#include "widgets/popup.h"
#include "widgets/window.h"
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
      break;
    }
    case EVT_POINTER_UP: {
      bool_t close_window = FALSE;
      pointer_event_t* evt = (pointer_event_t*)e;

      if (popup->close_when_click) {
        close_window = TRUE;
      } else if (popup->close_when_click_outside) {
        rect_t r = rect_init(widget->x, widget->y, widget->w, widget->h);
        if (!rect_contains(&r, evt->x, evt->y)) {
          close_window = TRUE;
        }
      }

      if (close_window) {
        widget_ungrab(widget->parent, widget);
        window_close(widget);
      }

      break;
    }
    default:
      break;
  }

  return window_base_on_event(widget, e);
}

static const char* s_popup_properties[] = {WIDGET_PROP_ANIM_HINT,
                                           WIDGET_PROP_OPEN_ANIM_HINT,
                                           WIDGET_PROP_CLOSE_ANIM_HINT,
                                           WIDGET_PROP_THEME,
                                           WIDGET_PROP_CLOSE_WHEN_CLICK,
                                           WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE,
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
