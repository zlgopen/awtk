/**
 * File:   ext_widgets.c
 * Author: AWTK Develop Team
 * Brief:  ext widgets
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
 * 2018-08-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ext_widgets.h"
#include "base/widget_factory.h"
#include "rich_text/rich_text.h"
#include "color_picker/color_picker.h"
#include "color_picker/color_component.h"

ret_t tk_ext_widgets_init() {
  widget_factory_register(widget_factory(), "rich_text", rich_text_create);
  widget_factory_register(widget_factory(), "color_picker", color_picker_create);
  widget_factory_register(widget_factory(), "color_component", color_component_create);

  return RET_OK;
}
