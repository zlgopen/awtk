/**
 * File:   xml_gen.c
 * Author: AWTK Develop Team
 * Brief:  generate theme date from xml
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/color_parser.h"
#include "base/assets_manager.h"

typedef struct _xml_builder_t {
  XmlBuilder builder;
  ThemeGen gen;
  Style widget_style;
  Style share_style;

  uint16_t level;
  string style_name;
  string widget_type;
} xml_builder_t;

static color_t parse_color(const char* name) {
  color_t c = color_parse(name);

  return c;
}

static uint32_t to_border(const char* value) {
  uint32_t border = 0;
  if (strstr(value, "left")) {
    border |= BORDER_LEFT;
  }
  if (strstr(value, "right")) {
    border |= BORDER_RIGHT;
  }
  if (strstr(value, "top")) {
    border |= BORDER_TOP;
  }
  if (strstr(value, "bottom")) {
    border |= BORDER_BOTTOM;
  }
  if (strstr(value, "all")) {
    border |= BORDER_ALL;
  }

  return border;
}

static uint32_t to_icon_at(const char* value) {
  uint32_t icon_at = ICON_AT_AUTO;

  if (strstr(value, "left")) {
    icon_at = ICON_AT_LEFT;
  }
  if (strstr(value, "right")) {
    icon_at = ICON_AT_RIGHT;
  }
  if (strstr(value, "top")) {
    icon_at = ICON_AT_TOP;
  }
  if (strstr(value, "bottom")) {
    icon_at = ICON_AT_BOTTOM;
  }

  return icon_at;
}

static void xml_gen_style(xml_builder_t* b, Style& s, const char** attrs) {
  uint32_t i = 0;

  while (attrs[i]) {
    const char* name = attrs[i];
    const char* value = attrs[i + 1];

    if (strcmp(name, "name") == 0) {
    } else if (strcmp(name, "bg_image_draw_type") == 0 || strcmp(name, "fg_image_draw_type") == 0) {
      const key_type_value_t* dt = image_draw_type_find(value);
      s.AddInt(name, dt->value);
    } else if (strcmp(name, "text_align_h") == 0) {
      const key_type_value_t* dt = align_h_type_find(value);
      s.AddInt(name, dt->value);
    } else if (strcmp(name, "text_align_v") == 0) {
      const key_type_value_t* dt = align_v_type_find(value);
      s.AddInt(name, dt->value);
    } else if (strcmp(name, "border") == 0) {
      uint32_t border = to_border(value);
      s.AddInt(name, border);
    } else if (strcmp(name, "icon_at") == 0) {
      uint32_t icon_at = to_icon_at(value);
      s.AddInt(name, icon_at);
    } else if (strstr(name, "color") != NULL) {
      s.AddInt(name, parse_color(value).color);
    } else if (strstr(name, "image") != NULL || strstr(name, "name") != NULL ||
               strstr(name, "icon") != NULL) {
      s.AddString(name, value);
    } else {
      s.AddInt(name, tk_atoi(value));
    }

    i += 2;
  }
}

static void xml_gen_on_widget(xml_builder_t* b, const char* tag, const char** attrs) {
  b->widget_style.Reset();
  xml_gen_style(b, b->widget_style, attrs);

  b->widget_type = tag;
  b->style_name = TK_DEFAULT_STYLE;
}

static void xml_gen_on_style(xml_builder_t* b, const char* tag, const char** attrs) {
  uint32_t i = 0;
  b->style_name = TK_DEFAULT_STYLE;

  while (attrs[i]) {
    const char* name = attrs[i];
    const char* value = attrs[i + 1];

    if (strcmp(name, "name") == 0) {
      b->style_name = value;
    }

    i += 2;
  }

  b->share_style.Reset();
  xml_gen_style(b, b->share_style, attrs);
}

static void xml_gen_on_state(xml_builder_t* b, const char* tag, const char** attrs) {
  const char* state = tag;
  Style s(b->widget_type, b->style_name, state);

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
  b.style_name = TK_DEFAULT_STYLE;
  b.widget_type = "";

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
  uint8_t buff[500 * 1024];

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
    output_res_c_source(output_file, ASSET_TYPE_STYLE, 0, buff, size);
  }

  xml_parser_destroy(parser);

  return true;
}
