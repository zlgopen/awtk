/**
 * File:   ui_binary_writer.c
 * Author: AWTK Develop Team
 * Brief:  ui_builder write widget info into buffer.
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
 * 2018-02-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/buffer.h"
#include "base/enums.h"
#include "tkc/utf8.h"
#include "tkc/value.h"
#include "base/ui_builder.h"
#include "ui_loader/ui_loader_default.h"
#include "ui_loader/ui_binary_writer.h"

static ret_t ui_binary_writer_on_widget_start(ui_builder_t* b, const widget_desc_t* desc) {
  ui_binary_writer_t* writer = (ui_binary_writer_t*)b;
  return wbuffer_write_binary(writer->wbuffer, desc, sizeof(*desc));
}

static ret_t ui_binary_writer_on_widget_prop(ui_builder_t* b, const char* name, const char* value) {
  ui_binary_writer_t* writer = (ui_binary_writer_t*)b;

  wbuffer_write_string(writer->wbuffer, name);
  return wbuffer_write_string(writer->wbuffer, value);
}

static ret_t ui_binary_writer_on_widget_prop_end(ui_builder_t* b) {
  ui_binary_writer_t* writer = (ui_binary_writer_t*)b;

  return wbuffer_write_uint8(writer->wbuffer, 0);
}

static ret_t ui_binary_writer_on_widget_end(ui_builder_t* b) {
  ui_binary_writer_t* writer = (ui_binary_writer_t*)b;

  return wbuffer_write_uint8(writer->wbuffer, 0);
}

ui_builder_t* ui_binary_writer_init(ui_binary_writer_t* writer, wbuffer_t* wbuffer) {
  return_value_if_fail(writer != NULL && wbuffer != NULL, NULL);

  memset(writer, 0x00, sizeof(ui_binary_writer_t));

  writer->wbuffer = wbuffer;
  writer->builder.on_widget_start = ui_binary_writer_on_widget_start;
  writer->builder.on_widget_prop = ui_binary_writer_on_widget_prop;
  writer->builder.on_widget_prop_end = ui_binary_writer_on_widget_prop_end;
  writer->builder.on_widget_end = ui_binary_writer_on_widget_end;

  wbuffer_write_uint32(wbuffer, UI_DATA_MAGIC);

  return &(writer->builder);
}
