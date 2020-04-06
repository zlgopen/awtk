/**
 * File:   view.h
 * Author: AWTK Develop Team
 * Brief:  view
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
 * 2018-04-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "widgets/view.h"

static ret_t view_get_prop(widget_t* widget, const char* name, value_t* v) {
  view_t* view = VIEW(widget);
  return_value_if_fail(view != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_DEFAULT_FOCUSED_CHILD)) {
    value_set_str(v, view->default_focused_child);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t view_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_DEFAULT_FOCUSED_CHILD)) {
    return view_set_default_focused_child(widget, value_str(v));
  }

  return RET_NOT_FOUND;
}

static ret_t view_on_destroy(widget_t* widget) {
  view_t* view = VIEW(widget);
  TKMEM_FREE(view->default_focused_child);

  return RET_OK;
}

TK_DECL_VTABLE(view) = {.size = sizeof(view_t),
                        .type = WIDGET_TYPE_VIEW,
                        .parent = TK_PARENT_VTABLE(widget),
                        .set_prop = view_set_prop,
                        .get_prop = view_get_prop,
                        .on_destroy = view_on_destroy,
                        .create = view_create};

widget_t* view_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(view), x, y, w, h);
}

widget_t* view_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, view), NULL);

  return widget;
}

ret_t view_set_default_focused_child(widget_t* widget, const char* default_focused_child) {
  view_t* view = VIEW(widget);
  return_value_if_fail(view != NULL, RET_BAD_PARAMS);
  return_value_if_fail(default_focused_child != NULL, RET_BAD_PARAMS);

  view->default_focused_child = tk_str_copy(view->default_focused_child, default_focused_child);

  return RET_OK;
}
