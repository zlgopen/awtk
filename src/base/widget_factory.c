/**
 * File:   widget_factory.c
 * Author: AWTK Develop Team
 * Brief:  widget factory
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
 * 2018-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/widget_factory.h"

static widget_factory_t* s_widget_factory = NULL;

widget_factory_t* widget_factory(void) {
  return s_widget_factory;
}

widget_factory_t* widget_factory_create(void) {
  return object_default_create();
}

ret_t widget_factory_register(widget_factory_t* factory, const char* type, widget_create_t create) {
  return_value_if_fail(factory != NULL && type != NULL && create != NULL, RET_BAD_PARAMS);

  return object_set_prop_pointer(factory, type, create);
}

widget_t* widget_factory_create_widget(widget_factory_t* factory, const char* type,
                                       widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = NULL;
  widget_create_t create = NULL;
  return_value_if_fail(factory != NULL && type != NULL, NULL);

  create = (widget_create_t)object_get_prop_pointer(factory, type);
  return_value_if_fail(create != NULL, NULL);

  widget = create(parent, x, y, w, h);
  if (widget != NULL) {
    event_t e = event_init(EVT_WIDGET_CREATED, widget);
    emitter_dispatch(EMITTER(factory), &e);
  }

  return widget;
}

ret_t widget_factory_set(widget_factory_t* factory) {
  s_widget_factory = factory;

  return RET_OK;
}

ret_t widget_factory_destroy(widget_factory_t* factory) {
  OBJECT_UNREF(factory);
  return RET_OK;
}
