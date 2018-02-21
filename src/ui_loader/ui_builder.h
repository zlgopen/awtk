/**
 * File:   ui_builder.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  ui_builder 
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef LFTK_UI_BUILDER_H
#define LFTK_UI_BUILDER_H

#include "base/widget.h"

BEGIN_C_DECLS

struct _ui_builder_t;
typedef struct _ui_builder_t ui_builder_t;

typedef ret_t (*ui_builder_on_widget_start_t)(ui_builder_t* b, uint16_t type, xy_t x, xy_t y, xy_t w, xy_t h);
typedef ret_t (*ui_builder_on_widget_prop_t)(ui_builder_t* b, const char* name, const char* value);
typedef ret_t (*ui_builder_on_widget_prop_end_t)(ui_builder_t* b);
typedef ret_t (*ui_builder_on_widget_end_t)(ui_builder_t* b);

struct _ui_builder_t {
  ui_builder_on_widget_start_t on_widget_start;
  ui_builder_on_widget_prop_t on_widget_prop;
  ui_builder_on_widget_prop_end_t on_widget_prop_end;
  ui_builder_on_widget_end_t on_widget_end;
  widget_t* root;
  widget_t* widget;
};

ret_t ui_builder_on_widget_start(ui_builder_t* b, uint16_t type, xy_t x, xy_t y, xy_t w, xy_t h);
ret_t ui_builder_on_widget_prop(ui_builder_t* b, const char* name, const char* value);
ret_t ui_builder_on_widget_prop_end(ui_builder_t* b);
ret_t ui_builder_on_widget_end(ui_builder_t* b);

#define UI_DATA_MAGIC 0x11221212

END_C_DECLS

#endif/*LFTK_UI_BUILDER_H*/

