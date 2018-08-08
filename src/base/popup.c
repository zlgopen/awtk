/**
 * File:   popup.c
 * Author: AWTK Develop Team
 * Brief:  popup
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/idle.h"
#include "base/label.h"
#include "base/enums.h"
#include "base/utils.h"
#include "base/layout.h"
#include "base/popup.h"
#include "base/window.h"
#include "base/image_manager.h"
#include "base/window_manager.h"

static ret_t popup_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t popup_get_prop(widget_t* widget, const char* name, value_t* v) {
  popup_t* popup = POPUP(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    value_set_str(v, popup->anim_hint);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME)) {
    value_set_str(v, popup->theme);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_WHEN_CLICK)) {
    value_set_bool(v, popup->close_when_click);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE)) {
    value_set_bool(v, popup->close_when_click_outside);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t popup_set_prop(widget_t* widget, const char* name, const value_t* v) {
  popup_t* popup = POPUP(widget);
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_ANIM_HINT)) {
    TKMEM_FREE(popup->anim_hint);
    popup->anim_hint = tk_strdup(value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_THEME)) {
    TKMEM_FREE(popup->theme);
    popup->theme = tk_strdup(value_str(v));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_WHEN_CLICK)) {
    popup->close_when_click = value_bool(v);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE)) {
    popup->close_when_click_outside = value_bool(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t popup_destroy(widget_t* widget) {
  popup_t* popup = POPUP(widget);

  TKMEM_FREE(popup->theme);
  TKMEM_FREE(popup->anim_hint);

  return RET_OK;
}

static ret_t popup_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  popup_t* popup = POPUP(widget);

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

  return RET_OK;
}

static const char* s_popup_clone_properties[] = {WIDGET_PROP_ANIM_HINT,
                                                 WIDGET_PROP_THEME,
                                                 WIDGET_PROP_SCRIPT,
                                                 WIDGET_PROP_CLOSE_WHEN_CLICK,
                                                 WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE,
                                                 NULL};
static const widget_vtable_t s_popup_vtable = {.size = sizeof(popup_t),
                                               .type = WIDGET_TYPE_POPUP,
                                               .clone_properties = s_popup_clone_properties,
                                               .create = popup_create,
                                               .get_prop = popup_get_prop,
                                               .set_prop = popup_set_prop,
                                               .on_event = popup_on_event,
                                               .destroy = popup_destroy,
                                               .on_paint_self = popup_on_paint_self};

widget_t* popup_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  popup_t* popup = TKMEM_ZALLOC(popup_t);
  widget_t* widget = WIDGET(popup);
  return_value_if_fail(popup != NULL, NULL);

  widget_init(widget, NULL, &s_popup_vtable, x, y, w, h);
  if (parent == NULL) {
    parent = window_manager();
  }

  return_value_if_fail(window_manager_open_window(parent, widget) == RET_OK, NULL);
  widget_update_style(widget);

  return widget;
}

widget_t* popup_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_popup_vtable, NULL);

  return widget;
}
