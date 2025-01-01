/**
 * File:   group_box.h
 * Author: AWTK Develop Team
 * Brief:  group_box
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "widgets/group_box.h"
#include "tkc/mem.h"

static ret_t group_box_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static ret_t group_box_set_prop(widget_t* widget, const char* name, const value_t* v) {
  group_box_t* group_box = GROUP_BOX(widget);
  return_value_if_fail(group_box != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    if (!widget_is_window_opened(widget)) {
      group_box->value = value_uint32(v);
    }
    return group_box_set_value(widget, value_uint32(v));
  }

  return RET_NOT_FOUND;
}

static ret_t group_box_get_prop(widget_t* widget, const char* name, value_t* v) {
  group_box_t* group_box = GROUP_BOX(widget);
  return_value_if_fail(group_box != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, WIDGET_PROP_VALUE)) {
    value_set_uint32(v, group_box->value);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t group_box_on_child_value_change(void* ctx, event_t* e) {
  value_change_event_t evt;
  widget_t* widget = WIDGET(ctx);
  widget_t* child = WIDGET(e->target);
  uint32_t i = widget_index_of(child);
  group_box_t* group_box = GROUP_BOX(widget);
  return_value_if_fail(group_box != NULL, RET_BAD_PARAMS);

  if (i == group_box->value) {
    return RET_OK;
  }

  e = value_change_event_init(&evt, e->type, widget);
  value_set_uint32(&(evt.old_value), group_box->value);
  value_set_uint32(&(evt.new_value), i);

  if (e->type == EVT_VALUE_CHANGED) {
    group_box->value = i;
  }

  return widget_dispatch(widget, e);
}

static ret_t group_box_on_add_child(widget_t* widget, widget_t* child) {
  return_value_if_fail(widget != NULL && child != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(widget_get_type(child), WIDGET_TYPE_RADIO_BUTTON)) {
    widget_on(child, EVT_VALUE_WILL_CHANGE, group_box_on_child_value_change, widget);
    widget_on(child, EVT_VALUE_CHANGED, group_box_on_child_value_change, widget);
  }

  /*让调用者继续处理*/
  return RET_FAIL;
}

static ret_t group_box_on_event(widget_t* widget, event_t* e) {
  group_box_t* group_box = GROUP_BOX(widget);
  return_value_if_fail(group_box != NULL && e != NULL, RET_BAD_PARAMS);

  switch (e->type) {
    case EVT_WINDOW_WILL_OPEN: {
      group_box_set_value(widget, group_box->value);
      break;
    }
    case EVT_WIDGET_REMOVE_CHILD: {
      widget_t* child = WIDGET(e->target);
      if (tk_str_eq(widget_get_type(child), WIDGET_TYPE_RADIO_BUTTON)) {
        widget_off_by_ctx(child, widget);
      }
      break;
    }
  }

  return RET_OK;
}

ret_t group_box_set_value(widget_t* widget, uint32_t value) {
  uint32_t i = value;
  widget_t* child = widget_get_child(widget, i);
  group_box_t* group_box = GROUP_BOX(widget);
  return_value_if_fail(group_box != NULL, RET_BAD_PARAMS);

  if (child != NULL) {
    group_box->value = value;
    return widget_set_prop_bool(child, WIDGET_PROP_VALUE, TRUE);
  }

  return RET_NOT_FOUND;
}

TK_DECL_VTABLE(group_box) = {.size = sizeof(group_box_t),
                             .type = WIDGET_TYPE_GROUP_BOX,
                             .inputable = TRUE,
                             .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
                             .create = group_box_create,
                             .get_prop = group_box_get_prop,
                             .set_prop = group_box_set_prop,
                             .on_event = group_box_on_event,
                             .on_add_child = group_box_on_add_child,
                             .on_paint_self = group_box_on_paint_self};

widget_t* group_box_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  return widget_create(parent, TK_REF_VTABLE(group_box), x, y, w, h);
}

widget_t* group_box_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, group_box), NULL);

  return widget;
}
