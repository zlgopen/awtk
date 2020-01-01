/**
 * File:   ui_xml_writer.c
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

#include "tkc/utils.h"
#include "base/ui_builder.h"
#include "ui_loader/ui_xml_writer.h"

static ret_t str_write_n_char(str_t* str, char c, uint32_t nr) {
  uint32_t i = 0;

  for (i = 0; i < nr; i++) {
    return_value_if_fail(str_append_char(str, c) == RET_OK, RET_OOM);
  }

  return RET_OK;
}

static ret_t ui_xml_writer_write_prop(str_t* str, const char* name, const char* value) {
  return_value_if_fail(str_append(str, " ") == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, name) == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, "=\"") == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, value) == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, "\"") == RET_OK, RET_OOM);

  return RET_OK;
}

static ret_t ui_xml_writer_write_prop_int(str_t* str, const char* name, int32_t v) {
  char value[32];
  tk_snprintf(value, sizeof(value) - 1, "%d", v);

  return ui_xml_writer_write_prop(str, name, value);
}

static ret_t ui_xml_writer_on_widget_start(ui_builder_t* b, const widget_desc_t* desc) {
  ui_xml_writer_t* writer = (ui_xml_writer_t*)b;
  const char* tag = desc->type;
  str_t* str = writer->str;

  return_value_if_fail(str_write_n_char(str, ' ', writer->level) == RET_OK, RET_OOM);

  writer->level++;
  if (writer->level > 1) {
    return_value_if_fail(str_append_char(&(writer->tags), '\0') == RET_OK, RET_OOM);
  }
  return_value_if_fail(str_append(&(writer->tags), tag) == RET_OK, RET_OOM);

  return_value_if_fail(str_append(str, "<") == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, tag) == RET_OK, RET_OOM);
  return_value_if_fail(ui_xml_writer_write_prop_int(str, "x", desc->layout.x) == RET_OK, RET_OOM);
  return_value_if_fail(ui_xml_writer_write_prop_int(str, "y", desc->layout.y) == RET_OK, RET_OOM);
  return_value_if_fail(ui_xml_writer_write_prop_int(str, "w", desc->layout.w) == RET_OK, RET_OOM);
  return_value_if_fail(ui_xml_writer_write_prop_int(str, "h", desc->layout.h) == RET_OK, RET_OOM);

  return RET_OK;
}

static ret_t ui_xml_writer_on_widget_prop(ui_builder_t* b, const char* name, const char* value) {
  ui_xml_writer_t* writer = (ui_xml_writer_t*)b;

  return ui_xml_writer_write_prop(writer->str, name, value);
}

static ret_t ui_xml_writer_on_widget_prop_end(ui_builder_t* b) {
  ui_xml_writer_t* writer = (ui_xml_writer_t*)b;

  return str_append(writer->str, ">\n");
}

static const char* ui_xml_writer_get_tag(ui_xml_writer_t* writer) {
  uint32_t i = 0;
  const char* tag = writer->tags.str;

  for (i = 0; i < writer->level; i++) {
    tag += strlen(tag) + 1;
  }

  return tag;
}

static ret_t ui_xml_writer_on_widget_end(ui_builder_t* b) {
  ui_xml_writer_t* writer = (ui_xml_writer_t*)b;
  str_t* str = writer->str;
  const char* tag = NULL;

  assert(writer->level > 0);

  writer->level--;
  tag = ui_xml_writer_get_tag(writer);
  return_value_if_fail(str_write_n_char(str, ' ', writer->level) == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, "</") == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, tag) == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, ">\n") == RET_OK, RET_OOM);

  if (writer->level == 0) {
    str_reset(&(writer->tags));
    str_reset(&(writer->temp));
  } else {
    writer->tags.size = tag - writer->tags.str - 1;
  }

  return RET_OK;
}

ui_builder_t* ui_xml_writer_init(ui_xml_writer_t* writer, str_t* str) {
  return_value_if_fail(writer != NULL && str != NULL, NULL);
  memset(writer, 0x00, sizeof(ui_xml_writer_t));
  return_value_if_fail(str_init(&(writer->tags), 100) != NULL, NULL);
  return_value_if_fail(str_init(&(writer->temp), 100) != NULL, NULL);

  writer->str = str;
  writer->builder.on_widget_start = ui_xml_writer_on_widget_start;
  writer->builder.on_widget_prop = ui_xml_writer_on_widget_prop;
  writer->builder.on_widget_prop_end = ui_xml_writer_on_widget_prop_end;
  writer->builder.on_widget_end = ui_xml_writer_on_widget_end;

  return &(writer->builder);
}
