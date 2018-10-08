/**
 * File:   ui_loader_xml.h
 * Author: AWTK Develop Team
 * Brief:  default ui_loader
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
 * 2018-02-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/enums.h"
#include "base/mem.h"
#include "base/str.h"
#include "base/layout.h"
#include "xml/xml_parser.h"
#include "ui_loader/ui_loader_xml.h"

typedef struct _xml_builder_t {
  XmlBuilder builder;
  ui_builder_t* ui_builder;
  str_t str;
} xml_builder_t;

/*FIXME: it is not a good solution to hardcode*/
static bool_t is_precedence_prop(const char* tag, const char* prop) {
  if ((tk_str_eq(tag, WIDGET_TYPE_EDIT) || tk_str_eq(tag, WIDGET_TYPE_SPIN_BOX)) &&
      tk_str_eq(prop, WIDGET_PROP_INPUT_TYPE)) {
    return TRUE;
  } else if (tk_str_eq(prop, WIDGET_PROP_OPTIONS)) {
    return TRUE;
  } else if (tk_str_eq(prop, "visible_nr")) {
    return TRUE;
  }

  return FALSE;
}

static void xml_loader_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  char c = '\0';
  uint32_t i = 0;
  const char* x = "0";
  const char* y = "0";
  const char* w = "0";
  const char* h = "0";
  widget_desc_t desc;
  const char* key = NULL;
  const char* value = NULL;
  xml_builder_t* b = (xml_builder_t*)thiz;

  memset(&desc, 0x00, sizeof(desc));

  while (attrs[i] != NULL) {
    key = attrs[i];
    value = attrs[i + 1];

    c = key[0];
    if (key[1] == '\0') {
      if (c == 'x') {
        x = (value);
      } else if (c == 'y') {
        y = (value);
      } else if (c == 'w') {
        w = (value);
      } else if (c == 'h') {
        h = (value);
      }
    }

    i += 2;
  }

  strncpy(desc.type, tag, NAME_LEN);
  widget_layout_parse(&(desc.layout), x, y, w, h);
  ui_builder_on_widget_start(b->ui_builder, &desc);

  /*set highest priority props*/
  i = 0;
  while (attrs[i] != NULL) {
    key = attrs[i];
    value = attrs[i + 1];
    c = key[0];

    if (key[1] == '\0') {
      if (c == 'x' || c == 'y' || c == 'w' || c == 'h') {
        i += 2;
        continue;
      }
    }

    if (is_precedence_prop(tag, key)) {
      ENSURE(str_decode_xml_entity(&(b->str), value) == RET_OK);
      ui_builder_on_widget_prop(b->ui_builder, key, b->str.str);
    }

    i += 2;
  }

  /*set normal priority props*/
  i = 0;
  while (attrs[i] != NULL) {
    key = attrs[i];
    value = attrs[i + 1];
    c = key[0];

    if (key[1] == '\0') {
      if (c == 'x' || c == 'y' || c == 'w' || c == 'h') {
        i += 2;
        continue;
      }
    }

    if (!is_precedence_prop(tag, key)) {
      ENSURE(str_decode_xml_entity(&(b->str), value) == RET_OK);
      ui_builder_on_widget_prop(b->ui_builder, key, b->str.str);
    }

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
  log_debug("parse error: %d:%d %s\n", line, row, message);
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
  str_init(&(b->str), 100);

  return &(b->builder);
}

ret_t ui_loader_load_xml(ui_loader_t* loader, const uint8_t* data, uint32_t size,
                         ui_builder_t* ui_builder) {
  xml_builder_t b;
  XmlParser* parser = NULL;
  return_value_if_fail(loader != NULL && data != NULL && ui_builder != NULL, RET_BAD_PARAMS);

  parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(&b, ui_builder));
  ui_builder_on_start(ui_builder);
  xml_parser_parse(parser, (const char*)data, size);
  ui_builder_on_end(ui_builder);
  xml_parser_destroy(parser);
  str_reset(&(b.str));

  return RET_OK;
}

static const ui_loader_t s_xml_ui_loader = {.load = ui_loader_load_xml};

ui_loader_t* xml_ui_loader() {
  return (ui_loader_t*)&s_xml_ui_loader;
}
