/**
 * File:   xml_gen.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  generate theme date from xml
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> adapted from ftk.
 *
 */

#include "theme_gen.h"
#include "base/enums.h"
#include "base/theme.h"
#include "base/widget.h"
#include "common/utils.h"
#include "xml_theme_gen.h"
#include "xml/xml_parser.h"
#include "common/color_parser.h"
#include "base/resource_manager.h"

typedef struct _xml_builder_t {
  XmlBuilder builder;
  ThemeGen gen;
} xml_builder_t;

static color_t parse_color(const char* name) {
  color_t c;
  c.color = 0;
  color_parse(name, &(c.rgba.r), &(c.rgba.g), &(c.rgba.b), &(c.rgba.a));

  return c;
}

static void xml_gen_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  uint32_t i = 0;
  xml_builder_t* b = (xml_builder_t*)thiz;
  const key_type_value_t* widget_item = widget_name_find(tag);

  return_if_fail(widget_item != NULL);

  uint8_t subtype = 0;
  uint16_t type = widget_item->value;
  uint8_t state = WIDGET_STATE_NORMAL;

  Style s(type, subtype, state);

  while (attrs[i]) {
    const char* name = attrs[i];
    const char* value = attrs[i + 1];

    if (strcmp(name, "state") == 0) {
      const key_type_value_t* state_item = state_name_find(value);
      if (state_item != NULL) {
        s.state = state_item->value;
      } else {
        printf("Not supported state: %s\n", value);
      }
    } else if (strcmp(name, "subtype") == 0) {
      s.subtype = atoi(value);
    } else {
      const key_type_value_t* item = style_name_find(name);
      if (item != NULL) {
        if (item->type == TYPE_INT) {
          s.AddInt(item->value, atoi(value));
        } else if (item->type == TYPE_COLOR) {
          s.AddInt(item->value, parse_color(value).color);
        } else {
          s.AddString(item->value, value);
        }
      } else {
        printf("Not supported style: %s\n", name);
      }
    }

    i += 2;
  }

  b->gen.AddStyle(s);

  return;
}

static void xml_gen_on_end(XmlBuilder* thiz, const char* tag) {
  (void)thiz;
  (void)tag;
  return;
}

static void xml_gen_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  (void)thiz;
  (void)text;
  (void)length;
  return;
}

static void xml_gen_on_comment(XmlBuilder* thiz, const char* text, size_t length) {
  (void)thiz;
  (void)text;
  (void)length;

  return;
}

static void xml_gen_on_pi(XmlBuilder* thiz, const char* tag, const char** attrs) {
  (void)thiz;
  (void)tag;
  (void)attrs;
  return;
}

static void xml_gen_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  (void)thiz;
  printf("parse error: %d:%d %s\n", line, row, message);
  return;
}

static void xml_gen_destroy(XmlBuilder* thiz) {
  (void)thiz;
  return;
}

static XmlBuilder* builder_init(xml_builder_t& b) {
  b.builder.on_start = xml_gen_on_start;
  b.builder.on_end = xml_gen_on_end;
  b.builder.on_text = xml_gen_on_text;
  b.builder.on_error = xml_gen_on_error;
  b.builder.on_comment = xml_gen_on_comment;
  b.builder.on_pi = xml_gen_on_pi;
  b.builder.destroy = xml_gen_destroy;

  return &(b.builder);
}

uint32_t xml_gen_buff(const char* xml, uint8_t* output, uint32_t max_size) {
  xml_builder_t b;
  return_value_if_fail(xml != NULL && output != NULL, 0);

  XmlParser* parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(b));
  xml_parser_parse(parser, xml, strlen(xml));

  uint8_t* end = b.gen.Output(output, max_size);
  return_value_if_fail(end != NULL, 0);

  uint32_t size = end - output;

  xml_parser_destroy(parser);

  return size;
}

bool xml_gen(const char* input_file, const char* output_file) {
  xml_builder_t b;
  uint8_t buff[1024 * 1024];
  return_value_if_fail(input_file != NULL && output_file != NULL, false);

  XmlParser* parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(b));
  xml_parser_parse_file(parser, input_file);

  uint8_t* end = b.gen.Output(buff, sizeof(buff));
  return_value_if_fail(end != NULL, false);

  uint32_t size = end - buff;
  output_res_c_source(output_file, RESOURCE_TYPE_THEME, 0, buff, size);

  xml_parser_destroy(parser);

  return true;
}
