/**
 * File:   dialog_client.c
 * Author: AWTK Develop Team
 * Brief:  dialog client
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
#include "tkc/utils.h"
#include "widgets/dialog_client.h"

static ret_t dialog_client_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

TK_DECL_VTABLE(dialog_client) = {.size = sizeof(dialog_client_t),
                                 .type = WIDGET_TYPE_DIALOG_CLIENT,
                                 .parent = TK_PARENT_VTABLE(widget),
                                 .create = dialog_client_create,
                                 .on_paint_self = dialog_client_on_paint_self};

widget_t* dialog_client_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(dialog_client), x, y, w, h);
  return_value_if_fail(widget != NULL, NULL);

  widget_set_name(widget, "client");
  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return widget;
}

widget_t* dialog_client_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, dialog_client), NULL);

  return widget;
}
