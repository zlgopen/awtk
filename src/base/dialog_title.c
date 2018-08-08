/**
 * File:   dialog_title.c
 * Author: AWTK Develop Team
 * Brief:  dialog title
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
#include "base/idle.h"
#include "base/label.h"
#include "base/enums.h"
#include "base/utils.h"
#include "base/dialog_title.h"

static ret_t dialog_title_on_paint_self(widget_t* widget, canvas_t* c) {
  return widget_paint_helper(widget, c, NULL, NULL);
}

static const widget_vtable_t s_dialog_title_vtable = {.size = sizeof(dialog_title_t),
                                                      .type = WIDGET_TYPE_DIALOG_TITLE,
                                                      .create = dialog_title_create,
                                                      .on_paint_self = dialog_title_on_paint_self};

widget_t* dialog_title_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  dialog_title_t* dialog_title = TKMEM_ZALLOC(dialog_title_t);
  widget_t* widget = WIDGET(dialog_title);
  return_value_if_fail(dialog_title != NULL, NULL);

  widget_init(widget, parent, &s_dialog_title_vtable, x, y, w, h);
  widget_set_name(widget, "title");

  return widget;
}

widget_t* dialog_title_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_dialog_title_vtable, NULL);

  return widget;
}
