/**
 * File:   ui_loader_default.h
 * Author: AWTK Develop Team
 * Brief:  default ui_loader
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

#include "tkc/mem.h"
#include "tkc/buffer.h"
#include "ui_loader/ui_loader_default.h"

ret_t ui_loader_load_default(ui_loader_t* loader, const uint8_t* data, uint32_t size,
                             ui_builder_t* b) {
  rbuffer_t rbuffer;
  widget_desc_t desc;
  uint32_t magic = 0;
  uint8_t widget_end_mark = 0;

  return_value_if_fail(loader != NULL && data != NULL && b != NULL, RET_BAD_PARAMS);
  return_value_if_fail(rbuffer_init(&rbuffer, data, size) != NULL, RET_BAD_PARAMS);
  return_value_if_fail(rbuffer_read_uint32(&rbuffer, &magic) == RET_OK, RET_BAD_PARAMS);
  return_value_if_fail(magic == UI_DATA_MAGIC, RET_BAD_PARAMS);

  ui_builder_on_start(b);
  while ((rbuffer.cursor + sizeof(desc)) <= rbuffer.capacity) {
    const char* key = NULL;
    const char* value = NULL;
    return_value_if_fail(rbuffer_read_binary(&rbuffer, &desc, sizeof(desc)) == RET_OK,
                         RET_BAD_PARAMS);
    ui_builder_on_widget_start(b, &desc);

    return_value_if_fail(rbuffer_read_string(&rbuffer, &key) == RET_OK, RET_BAD_PARAMS);
    while (*key) {
      return_value_if_fail(rbuffer_read_string(&rbuffer, &value) == RET_OK, RET_BAD_PARAMS);
      ui_builder_on_widget_prop(b, key, value);
      return_value_if_fail(rbuffer_read_string(&rbuffer, &key) == RET_OK, RET_BAD_PARAMS);
    }
    ui_builder_on_widget_prop_end(b);

    if (rbuffer_has_more(&rbuffer)) {
      return_value_if_fail(rbuffer_peek_uint8(&rbuffer, &widget_end_mark) == RET_OK,
                           RET_BAD_PARAMS);
      while (widget_end_mark == 0) {
        rbuffer_read_uint8(&rbuffer, &widget_end_mark);
        ui_builder_on_widget_end(b);
        if ((rbuffer.cursor + 1) >= rbuffer.capacity ||
            rbuffer_peek_uint8(&rbuffer, &widget_end_mark) != RET_OK) {
          break;
        }
      }
    }
    if (b->widget == NULL) break;
  }
  ui_builder_on_end(b);

  return RET_OK;
}

static const ui_loader_t s_default_ui_loader = {.load = ui_loader_load_default};

ui_loader_t* default_ui_loader() {
  return (ui_loader_t*)&s_default_ui_loader;
}
