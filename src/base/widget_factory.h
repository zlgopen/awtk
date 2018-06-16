/**
 * File:   widget_factory.h
 * Author: AWTK Develop Team
 * Brief:  widget factory
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
 * 2018-06-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_FACTORY_H
#define TK_WIDGET_FACTORY_H

#include "base/array.h"
#include "base/widget.h"

BEGIN_C_DECLS

typedef struct _widget_factory_t {
  array_t creators;
} widget_factory_t;

widget_factory_t* widget_factory(void);
widget_factory_t* widget_factory_create(void);
widget_factory_t* widget_factory_init(widget_factory_t* factory);

ret_t widget_factory_register(widget_factory_t* factory, const char* type, widget_create_t create);
widget_t* widget_factory_create_widget(widget_factory_t* factory, const char* type,
                                       widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h);

ret_t widget_factory_set(widget_factory_t* factory);
ret_t widget_factory_deinit(widget_factory_t* factory);
ret_t widget_factory_destroy(widget_factory_t* factory);

END_C_DECLS

#endif /*TK_WIDGET_FACTORY_H*/
