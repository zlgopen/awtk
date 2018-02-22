/**
 * File:   ui_loader_xml.h
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  default ui_loader
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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

#include "ui_loader/ui_loader_xml.h"
#include "base/enums.h"
#include "base/mem.h"
#include "xml/xml_parser.h"

typedef struct _xml_builder_t {
  XmlBuilder builder;
  ui_builder_t* ui_builder;
} xml_builder_t;

static void xml_loader_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  xy_t x = 0;
  xy_t y = 0;
  wh_t w = 0;
  wh_t h = 0;
  char c = '\0';
  uint32_t i = 0;
  const char* key = NULL;
  const char* value = NULL;
  xml_builder_t* b = (xml_builder_t*)thiz;
  const key_type_value_t* widget_item = widget_name_find(tag);
  return_if_fail(widget_item != NULL);

  while (attrs[i] != NULL) {
    key = attrs[i];
    value = attrs[i + 1];

    c = key[0];
    if (key[1] == '\0') {
      if (c == 'x') {
        x = atoi(value);
      } else if (c == 'y') {
        y = atoi(value);
      } else if (c == 'w') {
        w = atoi(value);
      } else if (c == 'h') {
        h = atoi(value);
      }
    }

    i += 2;
  }

  ui_builder_on_widget_start(b->ui_builder, widget_item->value, x, y, w, h);

  i = 0;
  while (attrs[i] != NULL) {
    char c = key[0];
    key = attrs[i];
    value = attrs[i + 1];

    if (key[1] == '\0') {
      if (c == 'x' || c == 'y' || c == 'w' || c == 'h') {
        i += 2;
        continue;
      }
    }

    ui_builder_on_widget_prop(b->ui_builder, key, value);

    i += 2;
  }
  ui_builder_on_widget_prop_end(b->ui_builder);

  return;
}

static void xml_loader_on_end(XmlBuilder* thiz, const char* tag) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  (void)thiz;
  (void)tag;
  ui_builder_on_widget_end(b->ui_builder);

  return;
}

static void xml_loader_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  (void)thiz;
  (void)text;
  (void)length;
  return;
}

static void xml_loader_on_comment(XmlBuilder* thiz, const char* text, size_t length) {
  (void)thiz;
  (void)text;
  (void)length;

  return;
}

static void xml_loader_on_pi(XmlBuilder* thiz, const char* tag, const char** attrs) {
  (void)thiz;
  (void)tag;
  (void)attrs;
  return;
}

static void xml_loader_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  (void)thiz;
  printf("parse error: %d:%d %s\n", line, row, message);
  return;
}

static void xml_loader_destroy(XmlBuilder* thiz) {
  (void)thiz;
  return;
}

static XmlBuilder* builder_init(xml_builder_t* b, ui_builder_t* ui_builder) {
  b->builder.on_start = xml_loader_on_start;
  b->builder.on_end = xml_loader_on_end;
  b->builder.on_text = xml_loader_on_text;
  b->builder.on_error = xml_loader_on_error;
  b->builder.on_comment = xml_loader_on_comment;
  b->builder.on_pi = xml_loader_on_pi;
  b->builder.destroy = xml_loader_destroy;
  b->ui_builder = ui_builder;

  return &(b->builder);
}

ret_t ui_loader_load_xml(ui_loader_t* loader, const uint8_t* data, uint32_t size,
                         ui_builder_t* ui_builder) {
  xml_builder_t b;
  XmlParser* parser = NULL;
  return_value_if_fail(loader != NULL && data != NULL && ui_builder != NULL, RET_BAD_PARAMS);

  parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(&b, ui_builder));
  xml_parser_parse(parser, (const char*)data, size);
  xml_parser_destroy(parser);

  return RET_OK;
}

static const ui_loader_t s_xml_ui_loader = {.load = ui_loader_load_xml};

ui_loader_t* xml_ui_loader() { return (ui_loader_t*)&s_xml_ui_loader; }
