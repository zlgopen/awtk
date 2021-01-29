/**
 * File:   xml_gen.c
 * Author: AWTK Develop Team
 * Brief:  generate theme date from xml
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
 * 2018-01-19 Li XianJing <xianjimli@hotmail.com> adapted from ftk.
 *
 */

#include "theme_gen.h"
#include "base/enums.h"
#include "base/theme.h"
#include "base/style.h"
#include "base/widget.h"
#include "common/utils.h"
#include "xml_theme_gen.h"
#include "xml/xml_parser.h"
#include "tkc/color_parser.h"
#include "tkc/buffer.h"
#include "base/assets_manager.h"

#define TAG_PROPERTY "property"

typedef struct _xml_builder_t {
  XmlBuilder builder;
  ThemeGen gen;
  Style widget_style;
  Style share_style;
  Style state_style;

  uint16_t level;
  string style_name;
  string widget_type;

  bool_t is_property;
  char property_name[TK_NAME_LEN * 2 + 2];
} xml_builder_t;

static void xml_gen_style(xml_builder_t* b, Style& s, const char** attrs) {
  value_t v;
  uint32_t i = 0;

  value_set_int(&v, 0);
  while (attrs[i]) {
    const char* name = attrs[i];
    const char* value = attrs[i + 1];
    ENSURE(style_normalize_value(name, value, &v) == RET_OK);

    if (strcmp(name, "name") != 0) {
      s.AddValue(name, v);
    }
    value_reset(&v);

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
  b->state_style.Reset();

  b->state_style.widget_type = b->widget_type;
  b->state_style.name = b->style_name;
  b->state_style.state = tag;

  xml_gen_style(b, b->state_style, attrs);
}

static void xml_gen_on_start_property(XmlBuilder* thiz, const char* tag, const char** attrs) {
  uint32_t i = 0;
  xml_builder_t* b = (xml_builder_t*)thiz;

  while (attrs[i] != NULL) {
    const char* key = attrs[i];
    const char* value = attrs[i + 1];
    if (tk_str_eq(key, "name")) {
      tk_strncpy(b->property_name, value, TK_NAME_LEN * 2 + 1);
      break;
    }
  }
}

static void xml_gen_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (tk_str_eq(tag, TAG_PROPERTY)) {
    b->is_property = TRUE;
    xml_gen_on_start_property(thiz, tag, attrs);
  } else {
    b->is_property = FALSE;

    if (b->level == 0) {
      xml_gen_on_widget(b, tag, attrs);
    } else if (b->level == 1) {
      xml_gen_on_style(b, tag, attrs);
    } else {
      xml_gen_on_state(b, tag, attrs);
    }
    b->level++;
  }

  return;
}

static void xml_gen_on_widget_end(XmlBuilder* thiz) {
  xml_builder_t* b = (xml_builder_t*)thiz;

  for (uint32_t i = 0; i < b->widget_style.datas.size(); i++) {
    Style& style = b->widget_style.datas[i];
    for (uint32_t j = 0; j < style.datas.size(); j++) {
      Style state;
      state.Merge(b->widget_style);
      state.Merge(style);
      state.Merge(style.datas[j]);
      state.widget_type = style.datas[j].widget_type;
      state.name = style.datas[j].name;
      state.state = style.datas[j].state;

      b->gen.AddStyle(state);
    }
  }
}

static void xml_gen_on_style_end(XmlBuilder* thiz) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  b->widget_style.datas.push_back(b->share_style);
  b->share_style.Reset();
}

static void xml_gen_on_state_end(XmlBuilder* thiz) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  b->share_style.datas.push_back(b->state_style);
  b->state_style.Reset();
}

static void xml_gen_on_end(XmlBuilder* thiz, const char* tag) {
  (void)tag;
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (tk_str_eq(tag, TAG_PROPERTY)) {
    b->is_property = FALSE;
  } else {
    if (b->level == 1) {
      xml_gen_on_widget_end(thiz);
    } else if (b->level == 2) {
      xml_gen_on_style_end(thiz);
    } else if (b->level == 3) {
      xml_gen_on_state_end(thiz);
    }
    b->level--;
  }

  return;
}

static void xml_gen_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (b->is_property) {
    Style* s = NULL;

    if (b->level == 1) {
      s = &(b->widget_style);
    } else if (b->level == 2) {
      s = &(b->share_style);
    } else if (b->level == 3) {
      s = &(b->state_style);
    }

    if (s != NULL) {
      value_t v;
      ENSURE(style_normalize_value(b->property_name, text, &v) == RET_OK);
      s->AddValue(b->property_name, v);
      value_reset(&v);
    }
  }

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
  b.is_property = FALSE;

  return &(b.builder);
}

uint32_t xml_gen_buff(const char* xml, uint8_t* output, uint32_t max_size) {
  xml_builder_t b;
  return_value_if_fail(xml != NULL && output != NULL, 0);

  XmlParser* parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(b));
  xml_parser_parse(parser, xml, strlen(xml));
  wbuffer_t wbuffer;
  wbuffer_init(&wbuffer, output, max_size);

  b.gen.Output(&wbuffer);

  wbuffer_deinit(&wbuffer);

  xml_parser_destroy(parser);

  return 0;
}

bool xml_gen(const char* input_file, const char* output_file, const char* theme,
             bool_t output_bin) {
  xml_builder_t b;
  wbuffer_t wbuffer;
  return_value_if_fail(input_file != NULL && output_file != NULL, false);

  wbuffer_init_extendable(&wbuffer);
  XmlParser* parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(b));
  xml_parser_parse_file(parser, input_file);

  if (b.gen.Output(&wbuffer) == RET_OK) {
    if (output_bin) {
      write_file(output_file, wbuffer.data, wbuffer.cursor);
    } else {
      output_res_c_source(output_file, theme, ASSET_TYPE_STYLE, 0, wbuffer.data, wbuffer.cursor);
    }
  }

  wbuffer_deinit(&wbuffer);
  xml_parser_destroy(parser);

  return true;
}
