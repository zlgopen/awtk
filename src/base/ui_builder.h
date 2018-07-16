/**
 * File:   ui_builder.h
 * Author: AWTK Develop Team
 * Brief:  ui_builder
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UI_BUILDER_H
#define TK_UI_BUILDER_H

#include "base/widget.h"
#include "base/layout.h"

BEGIN_C_DECLS

struct _ui_builder_t;
typedef struct _ui_builder_t ui_builder_t;

typedef struct _widget_desc_t {
  char type[NAME_LEN + 1];
  widget_layout_t layout;
} widget_desc_t;

typedef ret_t (*ui_builder_on_start_t)(ui_builder_t* b);
typedef ret_t (*ui_builder_on_widget_start_t)(ui_builder_t* b, const widget_desc_t* desc);
typedef ret_t (*ui_builder_on_widget_prop_t)(ui_builder_t* b, const char* name, const char* value);
typedef ret_t (*ui_builder_on_widget_prop_end_t)(ui_builder_t* b);
typedef ret_t (*ui_builder_on_widget_end_t)(ui_builder_t* b);
typedef ret_t (*ui_builder_on_end_t)(ui_builder_t* b);

struct _ui_builder_t {
  ui_builder_on_start_t on_start;
  ui_builder_on_widget_start_t on_widget_start;
  ui_builder_on_widget_prop_t on_widget_prop;
  ui_builder_on_widget_prop_end_t on_widget_prop_end;
  ui_builder_on_widget_end_t on_widget_end;
  ui_builder_on_end_t on_end;
  widget_t* root;
  widget_t* widget;
  const char* name;
};

ret_t ui_builder_on_widget_start(ui_builder_t* b, const widget_desc_t* desc);
ret_t ui_builder_on_widget_prop(ui_builder_t* b, const char* name, const char* value);
ret_t ui_builder_on_widget_prop_end(ui_builder_t* b);
ret_t ui_builder_on_widget_end(ui_builder_t* b);
ret_t ui_builder_on_start(ui_builder_t* b);
ret_t ui_builder_on_end(ui_builder_t* b);

#define UI_DATA_MAGIC 0x11221212

END_C_DECLS

#endif /*TK_UI_BUILDER_H*/
