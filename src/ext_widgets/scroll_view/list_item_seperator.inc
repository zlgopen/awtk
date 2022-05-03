/**
 * File:   list_item_seperator.h
 * Author: AWTK Develop Team
 * Brief:  list_item_seperator
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-05-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "list_item_seperator.h"

static ret_t list_item_seperator_update(widget_t* widget, bool_t checked) {
  bool_t meet = FALSE;
  const char* type = widget_get_type(widget);

  WIDGET_FOR_EACH_CHILD_BEGIN(widget->parent, iter, i)
  if (!meet) {
    if (iter == widget) {
      meet = TRUE;
    }
    continue;
  }

  if (tk_str_eq(widget_get_type(iter), type)) {
    break;
  }

  widget_set_visible(iter, checked);
  widget_set_sensitive(iter, checked);

  WIDGET_FOR_EACH_CHILD_END()

  return RET_OK;
}

static ret_t list_item_seperator_on_value_changed(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  list_item_seperator_update(widget, widget_get_value_int(widget));
  return RET_OK;
}

static ret_t list_item_seperator_on_will_open(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(ctx);
  list_item_seperator_update(widget, widget_get_value_int(widget));
  return RET_REMOVE;
}

widget_t* list_item_seperator_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget =
      check_button_create_ex(parent, x, y, w, h, WIDGET_TYPE_LIST_ITEM_SEPERATOR, FALSE);
  return_value_if_fail(widget != NULL, NULL);

  widget_on(widget, EVT_VALUE_CHANGED, list_item_seperator_on_value_changed, widget);
  widget_on(widget, EVT_WINDOW_WILL_OPEN, list_item_seperator_on_value_changed, widget);

  return widget;
}

widget_t* list_item_seperator_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL, NULL);
  if (!tk_str_eq(widget_get_type(widget), WIDGET_TYPE_LIST_ITEM_SEPERATOR)) {
    return NULL;
  }

  return widget;
}
