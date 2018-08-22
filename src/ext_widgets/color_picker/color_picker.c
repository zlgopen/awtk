/**
 * File:   color_picker.
 * Author: AWTK Develop Team
 * Brief:  color_picker
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
 * 2018-08-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/mem.h"
#include "color_picker/color_picker.h"

static const widget_vtable_t s_color_picker_vtable = {
    .size = sizeof(color_picker_t), .type = WIDGET_TYPE_COLOR_PICKER, .create = color_picker_create};

widget_t* color_picker_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  color_picker_t* color_picker = TKMEM_ZALLOC(color_picker_t);
  widget_t* widget = WIDGET(color_picker);
  return_value_if_fail(color_picker != NULL, NULL);

  widget_init(widget, parent, &s_color_picker_vtable, x, y, w, h);

  return widget;
}

ret_t color_picker_set_color(widget_t* widget, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  color_picker_t* color_picker = COLOR_PICKER(widget);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  color_picker->color = color_init(r, g, b, a);

  return RET_OK;
}

widget_t* color_picker_cast(widget_t* widget) {
  return_value_if_fail(widget != NULL && widget->vt == &s_color_picker_vtable, NULL);

  return widget;
}
