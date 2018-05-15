/**
 * File:   ui_builder_writer.c
 * Author: AWTK Develop Team
 * Brief:  ui_builder write widget info into buffer.
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
 * 2018-02-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/buffer.h"
#include "base/enums.h"
#include "base/utf8.h"
#include "base/value.h"
#include "ui_loader/ui_builder.h"
#include "ui_loader/ui_loader_default.h"

typedef struct _ui_builder_writer_t {
  ui_builder_t builder;
  wbuffer_t* wbuffer;
} ui_builder_writer_t;

static ret_t ui_builder_writer_on_widget_start(ui_builder_t* b, const widget_desc_t* desc) {
  ui_builder_writer_t* writer = (ui_builder_writer_t*)b;
  return wbuffer_write_binary(writer->wbuffer, desc, sizeof(*desc));
}

static ret_t ui_builder_writer_on_widget_prop(ui_builder_t* b, const char* name,
                                              const char* value) {
  ui_builder_writer_t* writer = (ui_builder_writer_t*)b;

  wbuffer_write_string(writer->wbuffer, name);
  return wbuffer_write_string(writer->wbuffer, value);
}

static ret_t ui_builder_writer_on_widget_prop_end(ui_builder_t* b) {
  ui_builder_writer_t* writer = (ui_builder_writer_t*)b;

  return wbuffer_write_uint8(writer->wbuffer, 0);
}

static ret_t ui_builder_writer_on_widget_end(ui_builder_t* b) {
  ui_builder_writer_t* writer = (ui_builder_writer_t*)b;

  return wbuffer_write_uint8(writer->wbuffer, 0);
}

static ui_builder_writer_t s_ui_builder_writer;

ui_builder_t* ui_builder_writer(wbuffer_t* wbuffer) {
  return_value_if_fail(wbuffer != NULL, NULL);

  memset(&s_ui_builder_writer, 0x00, sizeof(s_ui_builder_writer));

  s_ui_builder_writer.wbuffer = wbuffer;
  s_ui_builder_writer.builder.on_widget_start = ui_builder_writer_on_widget_start;
  s_ui_builder_writer.builder.on_widget_prop = ui_builder_writer_on_widget_prop;
  s_ui_builder_writer.builder.on_widget_prop_end = ui_builder_writer_on_widget_prop_end;
  s_ui_builder_writer.builder.on_widget_end = ui_builder_writer_on_widget_end;

  wbuffer_write_uint32(wbuffer, UI_DATA_MAGIC);

  return &(s_ui_builder_writer.builder);
}
