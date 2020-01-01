/**
 * File:   dialog_title.c
 * Author: AWTK Develop Team
 * Brief:  dialog title
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "base/idle.h"
#include "widgets/label.h"
#include "base/enums.h"
#include "tkc/utils.h"
#include "widgets/dialog_title.h"

static ret_t dialog_title_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

TK_DECL_VTABLE(dialog_title) = {.size = sizeof(dialog_title_t),
                                .type = WIDGET_TYPE_DIALOG_TITLE,
                                .parent = TK_PARENT_VTABLE(widget),
                                .create = dialog_title_create,
                                .on_paint_self = dialog_title_on_paint_self};

widget_t* dialog_title_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(dialog_title), x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);

  widget_set_name(widget, "title");

  return widget;
}

widget_t* dialog_title_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, dialog_title), NULL);

  return widget;
}
