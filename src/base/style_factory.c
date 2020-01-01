/**
 * File:   style_factory.h
 * Author: AWTK Develop Team
 * Brief:  style factory
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
 * 2018-10-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/style_const.h"
#include "base/style_factory.h"

style_t* style_factory_create_style(style_factory_t* factory, widget_t* widget) {
  if (factory != NULL && factory->create_style != NULL) {
    return factory->create_style(factory, widget);
  } else {
    return style_const_create(widget);
  }
}

static style_factory_t* s_style_factory;
style_factory_t* style_factory(void) {
  return s_style_factory;
}

ret_t style_factory_set(style_factory_t* factory) {
  s_style_factory = factory;

  return RET_OK;
}
