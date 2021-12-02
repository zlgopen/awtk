/**
 * File:   object_widget.c
 * Author: AWTK Develop Team
 * Brief:  wrap widget to an object.
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY { without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-09-22 Liu YuXin <liuyuxin@zlg.cn> created
 *
 */

#include "object_widget.h"

static ret_t object_widget_on_destroy(tk_object_t* obj) {
  object_widget_t* o = OBJECT_WIDGET(obj);
  return_value_if_fail(o != NULL && o->widget != NULL, RET_BAD_PARAMS);

  o->widget = NULL;

  return RET_OK;
}

static int32_t object_widget_compare(tk_object_t* obj, tk_object_t* other) {
  object_widget_t* o1 = OBJECT_WIDGET(obj);
  object_widget_t* o2 = OBJECT_WIDGET(other);
  return_value_if_fail(o1 != NULL, -1);
  return_value_if_fail(o2 != NULL, 1);

  return o1->widget == o2->widget ? 0 : o1->widget > o2->widget ? 2 : -2;
}

static ret_t object_widget_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_widget_t* o = OBJECT_WIDGET(obj);
  return_value_if_fail(o != NULL && o->widget != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, OBJECT_WIDGET_PROP_NATIVE_WIDGET)) {
    o->widget = (widget_t*)value_pointer(v);
    ret = RET_OK;
  } else {
    ret = widget_set_prop(o->widget, name, v);
  }

  return ret;
}

static ret_t object_widget_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  ret_t ret = RET_NOT_FOUND;
  object_widget_t* o = OBJECT_WIDGET(obj);
  return_value_if_fail(o != NULL && o->widget != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(name, OBJECT_WIDGET_PROP_NATIVE_WIDGET)) {
    value_set_pointer(v, (void*)o->widget);
    ret = RET_OK;
  } else {
    ret = widget_get_prop(o->widget, name, v);
  }
  return ret;
}

static const object_vtable_t s_object_widget_vtable = {.type = "object_widget",
                                                       .desc = "object_widget",
                                                       .size = sizeof(object_widget_t),
                                                       .is_collection = FALSE,
                                                       .on_destroy = object_widget_on_destroy,
                                                       .compare = object_widget_compare,
                                                       .get_prop = object_widget_get_prop,
                                                       .set_prop = object_widget_set_prop};

tk_object_t* object_widget_create(widget_t* widget) {
  tk_object_t* o = NULL;
  object_widget_t* wrapper = NULL;
  return_value_if_fail(widget != NULL, NULL);

  o = tk_object_create(&s_object_widget_vtable);
  return_value_if_fail(o != NULL, NULL);

  wrapper = OBJECT_WIDGET(o);
  return_value_if_fail(wrapper != NULL, NULL);

  wrapper->widget = widget;

  return o;
}

object_widget_t* object_widget_cast(tk_object_t* obj) {
  return_value_if_fail(obj != NULL && obj->vt == &s_object_widget_vtable, NULL);

  return (object_widget_t*)(obj);
}

