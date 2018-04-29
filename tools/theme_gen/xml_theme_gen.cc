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
  Style widget_style;
  Style share_style;

  uint16_t level;
  uint16_t widget_type;
  uint16_t style_name;
} xml_builder_t;

static color_t parse_color(const char* name) {
  color_t c;
  c.color = 0;
  color_parse(name, &(c.rgba.r), &(c.rgba.g), &(c.rgba.b), &(c.rgba.a));

  return c;
}

static void xml_gen_style(xml_builder_t* b, Style& s, const char** attrs) {
  uint32_t i = 0;

  while (attrs[i]) {
    const char* name = attrs[i];
    const char* value = attrs[i + 1];

    const key_type_value_t* item = style_id_find(name);
    if (item != NULL) {
      if (strcmp(name, "bg_image_draw_type") == 0 || strcmp(name, "fg_image_draw_type") == 0) {
        const key_type_value_t* dt = image_draw_type_find(value);
        s.AddInt(item->value, dt->value);
      } else if (strcmp(name, "text_align_h") == 0) {
        const key_type_value_t* dt = align_h_type_find(value);
        s.AddInt(item->value, dt->value);
      } else if (strcmp(name, "text_align_v") == 0) {
        const key_type_value_t* dt = align_v_type_find(value);
        s.AddInt(item->value, dt->value);
      } else if (item->type == TYPE_INT) {
        s.AddInt(item->value, atoi(value));
      } else if (item->type == TYPE_COLOR) {
        s.AddInt(item->value, parse_color(value).color);
      } else {
        s.AddString(item->value, value);
      }
    } else if (strcmp(name, "name") != 0) {
      printf("Not supported style: %s\n", name);
    }

    i += 2;
  }
}

static void xml_gen_on_widget(xml_builder_t* b, const char* tag, const char** attrs) {
  const key_type_value_t* item = widget_type_find(tag);
  assert(item != NULL);

  b->widget_style.Reset();
  xml_gen_style(b, b->widget_style, attrs);

  b->widget_type = item->value;
  b->style_name = 0;
}

static void xml_gen_on_style(xml_builder_t* b, const char* tag, const char** attrs) {
  uint32_t i = 0;
  b->style_name = 0;

  while (attrs[i]) {
    const char* name = attrs[i];
    const char* value = attrs[i + 1];

    if (strcmp(name, "name") == 0) {
      if (strcmp(value, "default") == 0 || strcmp(value, "0:default") == 0) {
        b->style_name = 0;
      } else {
        b->style_name = atoi(value);
      }
    }

    i += 2;
  }

  b->share_style.Reset();
  xml_gen_style(b, b->share_style, attrs);
}

static void xml_gen_on_state(xml_builder_t* b, const char* tag, const char** attrs) {
  const key_type_value_t* state_item = widget_state_find(tag);
  assert(state_item != NULL);
  Style s(b->widget_type, b->style_name, state_item->value);

  s.Merge(b->widget_style);
  s.Merge(b->share_style);
  xml_gen_style(b, s, attrs);

  b->gen.AddStyle(s);
}

static void xml_gen_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (b->level == 0) {
    xml_gen_on_widget(b, tag, attrs);
  } else if (b->level == 1) {
    xml_gen_on_style(b, tag, attrs);
  } else {
    xml_gen_on_state(b, tag, attrs);
  }

  b->level++;

  return;
}

static void xml_gen_on_end(XmlBuilder* thiz, const char* tag) {
  (void)thiz;
  (void)tag;
  xml_builder_t* b = (xml_builder_t*)thiz;
  b->level--;

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
  b.level = 0;
  b.style_name = 0;
  b.widget_type = 0;

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


bool xml_gen(const char* input_file, const char* output_file, bool_t output_bin) {
  xml_builder_t b;
  uint8_t buff[100 * 1024];

  return_value_if_fail(input_file != NULL && output_file != NULL, false);

  XmlParser* parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(b));
  xml_parser_parse_file(parser, input_file);

  uint8_t* end = b.gen.Output(buff, sizeof(buff));
  return_value_if_fail(end != NULL, false);

  uint32_t size = end - buff;

  if (output_bin) {
    write_file(output_file, buff, size);
  } else {
    output_res_c_source(output_file, RESOURCE_TYPE_THEME, 0, buff, size);
  }

  xml_parser_destroy(parser);

  return true;
}
