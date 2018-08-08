/**
 * File:   dialog_client.c
 * Author: AWTK Develop Team
 * Brief:  dialog client
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/utils.h"
#include "base/dialog_client.h"

static ret_t dialog_client_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static const widget_vtable_t s_dialog_client_vtable = {
    .size = sizeof(dialog_client_t),
    .type = WIDGET_TYPE_DIALOG_CLIENT,
    .create = dialog_client_create,
    .on_paint_self = dialog_client_on_paint_self};

widget_t* dialog_client_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  dialog_client_t* dialog_client = TKMEM_ZALLOC(dialog_client_t);
  widget_t* widget = WIDGET(dialog_client);
  return_value_if_fail(dialog_client != NULL, NULL);

  widget_init(widget, parent, &s_dialog_client_vtable, x, y, w, h);

  widget_set_name(widget, "client");
  widget_set_state(widget, WIDGET_STATE_NORMAL);

  return widget;
}

widget_t* dialog_client_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_dialog_client_vtable, NULL);

  return widget;
}
