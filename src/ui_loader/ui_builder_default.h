/**
 * File:   ui_builder_default.h
 * Author: AWTK Develop Team
 * Brief:  ui_builder default
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

#ifndef TK_UI_BUILDER_DEFAULT_H
#define TK_UI_BUILDER_DEFAULT_H

#include "base/ui_builder.h"

BEGIN_C_DECLS

ui_builder_t* ui_builder_default(const char* name);

widget_t* window_open(const char* name);
widget_t* dialog_open(const char* name);
widget_t* window_open_and_close(const char* name, widget_t* to_close);

END_C_DECLS

#endif /*TK_UI_BUILDER_DEFAULT_H*/
