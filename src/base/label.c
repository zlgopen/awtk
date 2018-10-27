/**
 * File:   label.h
 * Author: AWTK Develop Team
 * Brief:  label
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
 * 2018-02-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "base/label.h"
#include "base/widget_vtable.h"

static const widget_vtable_t s_label_vtable = {.size = sizeof(label_t),
                                               .type = WIDGET_TYPE_LABEL,
                                               .create = label_create,
                                               .on_paint_self = widget_on_paint_self_default};

widget_t* label_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  label_t* label = TKMEM_ZALLOC(label_t);
  widget_t* widget = WIDGET(label);
  return_value_if_fail(label != NULL, NULL);

  return widget_init(widget, parent, &s_label_vtable, x, y, w, h);
}
