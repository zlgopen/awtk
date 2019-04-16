/**
 * File:   tab_button.c
 * Author: AWTK Develop Team
 * Brief:  tab_button
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
 * 2018-07-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "widgets/tab_button.h"
#include "base/image_manager.h"
#include "base/widget_vtable.h"

static ret_t tab_button_on_event(widget_t* widget, event_t* e) {
  uint16_t type = e->type;
  switch (type) {
    case EVT_POINTER_DOWN: {
      widget_set_state(widget, WIDGET_STATE_PRESSED);
      break;
    }
    case EVT_POINTER_UP: {
      tab_button_set_value(widget, TRUE);
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    }
    case EVT_POINTER_LEAVE:
      widget_set_state(widget, WIDGET_STATE_NORMAL);
      break;
    case EVT_POINTER_ENTER:
      widget_set_state(widget, WIDGET_STATE_OVER);
      break;
    default:
      break;
  }

  return RET_OK;
}

static ret_t tab_button_on_paint_self(widget_t* widget, canvas_t* c) {
  const char* icon = NULL;
  tab_button_t* tab_button = TAB_BUTTON(widget);
  return_value_if_fail(tab_button != NULL, RET_BAD_PARAMS);

  icon = tab_button->value ? tab_button->active_icon : tab_button->icon;

  return widget_paint_helper(widget, c, icon, NULL);
}

static ret_t tab_button_set_value_only(widget_t* widget, bool_t value) {
  tab_button_t* tab_button = TAB_BUTTON(widget);
  return_value_if_fail(tab_button != NULL, RET_BAD_PARAMS);

  if (tab_button->value != value) {
    event_t e = event_init(EVT_VALUE_WILL_CHANGE, widget);
    widget_dispatch(widget, &e);
    tab_button->value = value;
    e = event_init(EVT_VALUE_CHANGED, widget);
    widget_dispatch(widget, &e);
    widget_update_style(widget);
  }

  return RET_OK;
}

static widget_t* tab_button_get_pages(widget_t* widget) {
  widget_t* pages = NULL;
  return_value_if_fail(widget && widget->parent && widget->parent->parent, NULL);

  pages = widget_lookup_by_type(widget->parent->parent, WIDGET_TYPE_PAGES, TRUE);
  if (pages == NULL) {
    pages = widget_lookup_by_type(widget->parent->parent->parent, WIDGET_TYPE_PAGES, TRUE);
  }

  return pages;
}

ret_t tab_button_set_value(widget_t* widget, bool_t value) {
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  if (widget->parent != NULL) {
    int32_t index = 0;
    widget_t* pages = NULL;
    widget_t* parent = widget->parent;

    WIDGET_FOR_EACH_CHILD_BEGIN(parent, iter, i)
    if (iter != widget) {
      tab_button_set_value_only(iter, !value);
    } else {
      tab_button_set_value_only(iter, value);
    }
    WIDGET_FOR_EACH_CHILD_END();

    pages = tab_button_get_pages(widget);
    if (pages != NULL) {
      index = widget_index_of(widget);
      widget_set_value(pages, index);
    }
  }

  return RET_OK;
}

static int32_t tab_button_get_min_w(widget_t* widget) {
  tab_button_t* tab_button = TAB_BUTTON(widget);
  int32_t text_w = widget_measure_text(widget, widget->text.str) + widget->h / 2;

  if (tab_button->icon != NULL || tab_button->active_icon != NULL) {
    text_w += widget->h;
  }

  return text_w;
}

static ret_t tab_button_get_prop(widget_t* widget, const char* name, value_t* v) {
  tab_button_t* tab_button = TAB_BUTTON(widget);
  return_value_if_fail(tab_button != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_bool(v, tab_button->value);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_STATE_FOR_STYLE)) {
    value_set_str(v, widget_get_state_for_style(widget, tab_button->value, FALSE));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_MIN_W)) {
    value_set_int(v, tab_button_get_min_w(widget));
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ICON)) {
    value_set_str(v, tab_button->icon);
    return RET_OK;
  } else if (tk_str_eq(name, WIDGET_PROP_ACTIVE_ICON)) {
    value_set_str(v, tab_button->active_icon);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tab_button_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    return tab_button_set_value(widget, value_bool(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ICON)) {
    return tab_button_set_icon(widget, value_str(v));
  } else if (tk_str_eq(name, WIDGET_PROP_ACTIVE_ICON)) {
    return tab_button_set_active_icon(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t tab_button_on_destroy(widget_t* widget) {
  tab_button_t* tab_button = TAB_BUTTON(widget);
  return_value_if_fail(widget != NULL && tab_button != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(tab_button->icon);
  TKMEM_FREE(tab_button->active_icon);

  return RET_OK;
}

static const char* s_tab_button_clone_properties[] = {WIDGET_PROP_VALUE, NULL};
TK_DECL_VTABLE(tab_button) = {.size = sizeof(tab_button_t),
                              .type = WIDGET_TYPE_TAB_BUTTON,
                              .clone_properties = s_tab_button_clone_properties,
                              .parent = TK_PARENT_VTABLE(widget),
                              .create = tab_button_create,
                              .on_event = tab_button_on_event,
                              .on_paint_self = tab_button_on_paint_self,
                              .get_prop = tab_button_get_prop,
                              .set_prop = tab_button_set_prop,
                              .on_destroy = tab_button_on_destroy};

ret_t tab_button_set_icon(widget_t* widget, const char* name) {
  tab_button_t* tab_button = TAB_BUTTON(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(tab_button->icon);
  tab_button->icon = tk_strdup(name);

  return RET_OK;
}

ret_t tab_button_set_active_icon(widget_t* widget, const char* name) {
  tab_button_t* tab_button = TAB_BUTTON(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(tab_button->active_icon);
  tab_button->active_icon = tk_strdup(name);

  return RET_OK;
}

widget_t* tab_button_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(tab_button), x, y, w, h);
  tab_button_t* tab_button = TAB_BUTTON(widget);
  return_value_if_fail(tab_button != NULL, NULL);

  tab_button_set_value_only(widget, FALSE);

  return widget;
}

widget_t* tab_button_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, tab_button), NULL);

  return widget;
}
