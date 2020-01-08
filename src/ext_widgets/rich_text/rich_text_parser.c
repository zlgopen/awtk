/**
 * File:   rich_text_parser.c
 * Author: AWTK Develop Team
 * Brief:  rich_text_parser
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is dirich_text_parseributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-07-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/str.h"
#include "tkc/utils.h"
#include "base/enums.h"
#include "base/bitmap.h"
#include "xml/xml_parser.h"
#include "tkc/color_parser.h"
#include "rich_text/rich_text_node.h"
#include "rich_text/rich_text_parser.h"

#define MAX_FONT_LEVEL 8

typedef struct _xml_builder_t {
  XmlBuilder builder;
  int32_t font_level;
  rich_text_font_t fonts[MAX_FONT_LEVEL];
  rich_text_font_t* font;
  rich_text_node_t* node;
  str_t temp;
} xml_builder_t;

static void xml_rich_text_push_font(xml_builder_t* b) {
  b->font_level++;
  b->fonts[b->font_level] = b->fonts[b->font_level - 1];

  b->font = b->fonts + b->font_level;
  if (b->font->name != NULL) {
    b->font->name = tk_strdup(b->font->name);
  }
}

static void xml_rich_text_pop_font(xml_builder_t* b) {
  rich_text_font_t* font = b->fonts + b->font_level;

  if (font->name != NULL) {
    TKMEM_FREE(font->name);
    font->name = NULL;
  }

  b->font_level--;
  b->font = b->fonts + b->font_level;
}

static void xml_rich_text_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  int32_t i = 0;
  xml_builder_t* b = (xml_builder_t*)thiz;

  if (tk_str_eq(tag, "font")) {
    xml_rich_text_push_font(b);
    b->font->align_v = ALIGN_V_BOTTOM;

    while (attrs[i]) {
      const char* key = attrs[i];
      const char* value = attrs[i + 1];
      if (tk_str_eq(key, "size")) {
        b->font->size = tk_atoi(value);
      } else if (tk_str_eq(key, "color")) {
        b->font->color = color_parse(value);
      } else if (tk_str_eq(key, "align_v")) {
        const key_type_value_t* kv = align_v_type_find(value);
        if (kv != NULL) {
          b->font->align_v = (align_v_t)(kv->value);
        }
      } else if (tk_str_eq(key, "name")) {
        TKMEM_FREE(b->font->name);
        b->font->name = tk_strdup(value);
      }
      i += 2;
    }
  } else if (tk_str_eq(tag, "b")) {
    xml_rich_text_push_font(b);
    b->font->bold = TRUE;
  } else if (tk_str_eq(tag, "i")) {
    xml_rich_text_push_font(b);
    b->font->italic = TRUE;
  } else if (tk_str_eq(tag, "u")) {
    xml_rich_text_push_font(b);
    b->font->underline = TRUE;
  } else if (tk_str_eq(tag, "image")) {
    int32_t w = 0;
    int32_t h = 0;
    int32_t draw_type = IMAGE_DRAW_CENTER;
    const char* image_name = NULL;

    while (attrs[i]) {
      const char* key = attrs[i];
      const char* value = attrs[i + 1];
      if (tk_str_eq(key, "name")) {
        image_name = value;
      } else if (tk_str_eq(key, "w")) {
        w = tk_atoi(value);
      } else if (tk_str_eq(key, "h")) {
        h = tk_atoi(value);
      } else if (tk_str_eq(key, "draw_type")) {
        const key_type_value_t* kv = image_draw_type_find(value);
        if (kv != NULL) {
          draw_type = kv->value;
        }
      }
      i += 2;
    }

    if (image_name != NULL) {
      b->node = rich_text_node_append(b->node, rich_text_image_create(image_name, w, h, draw_type));
    }
  }

  return;
}

static void xml_rich_text_on_end(XmlBuilder* thiz, const char* tag) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  if (tk_str_eq(tag, "font") || tk_str_eq(tag, "b") || tk_str_eq(tag, "i") || tk_str_eq(tag, "u")) {
    xml_rich_text_pop_font(b);
  }

  return;
}

static void xml_rich_text_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  rich_text_font_t* font = b->font;

  str_decode_xml_entity_with_len(&(b->temp), text, length);
  b->node = rich_text_node_append(b->node, rich_text_text_create(font, b->temp.str));

  return;
}

static void xml_rich_text_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  (void)thiz;
  log_debug("parse error: %d:%d %s\n", line, row, message);
  return;
}

static void xml_rich_text_destroy(XmlBuilder* thiz) {
  xml_builder_t* b = (xml_builder_t*)thiz;
  str_reset(&(b->temp));
  return;
}

static XmlBuilder* builder_init(xml_builder_t* b) {
  int32_t i = 0;
  memset(b, 0x00, sizeof(xml_builder_t));

  b->builder.on_start = xml_rich_text_on_start;
  b->builder.on_end = xml_rich_text_on_end;
  b->builder.on_text = xml_rich_text_on_text;
  b->builder.on_error = xml_rich_text_on_error;
  b->builder.destroy = xml_rich_text_destroy;
  b->font = b->fonts;

  for (i = 0; i < MAX_FONT_LEVEL; i++) {
    rich_text_font_t* iter = b->fonts + i;

    iter->size = TK_DEFAULT_FONT_SIZE;
    iter->color = color_init(0, 0, 0, 0xff);
    iter->align_v = ALIGN_V_BOTTOM;
  }
  str_init(&(b->temp), 100);

  return &(b->builder);
}

rich_text_node_t* rich_text_parse(const char* str, uint32_t size) {
  xml_builder_t b;
  XmlParser* parser = NULL;
  rich_text_node_t* node = NULL;
  return_value_if_fail(str != NULL, NULL);

  parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(&b));
  xml_parser_set_trim_text(parser, FALSE);
  xml_parser_parse(parser, str, size);

  node = b.node;
  xml_parser_destroy(parser);
  xml_builder_destroy(&(b.builder));

  return node;
}
