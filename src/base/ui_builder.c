/**
 * File:   ui_builder.c
 * Author: AWTK Develop Team
 * Brief:  ui_builder
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/ui_builder.h"

ret_t ui_builder_on_widget_start(ui_builder_t* b, const widget_desc_t* desc) {
  return_value_if_fail(b != NULL && b->on_widget_start != NULL && desc != NULL, RET_BAD_PARAMS);

  return b->on_widget_start(b, desc);
}

ret_t ui_builder_on_widget_prop(ui_builder_t* b, const char* name, const char* value) {
  return_value_if_fail(b != NULL && b->on_widget_prop != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && value != NULL, RET_BAD_PARAMS);

  return b->on_widget_prop(b, name, value);
}

ret_t ui_builder_on_widget_prop_end(ui_builder_t* b) {
  return_value_if_fail(b != NULL && b->on_widget_prop_end != NULL, RET_BAD_PARAMS);

  return b->on_widget_prop_end(b);
}

ret_t ui_builder_on_widget_end(ui_builder_t* b) {
  return_value_if_fail(b != NULL && b->on_widget_end != NULL, RET_BAD_PARAMS);

  return b->on_widget_end(b);
}

ret_t ui_builder_on_start(ui_builder_t* b) {
  return_value_if_fail(b != NULL, RET_BAD_PARAMS);

  if (b->on_start) {
    return b->on_start(b);
  } else {
    return RET_OK;
  }
}

ret_t ui_builder_on_end(ui_builder_t* b) {
  return_value_if_fail(b != NULL, RET_BAD_PARAMS);

  if (b->on_end) {
    return b->on_end(b);
  } else {
    return RET_OK;
  }
}
