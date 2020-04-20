/**
 * File:   xml_str_gen.c
 * Author: AWTK Develop Team
 * Brief:  xml str gen
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
 * 2018-05-03 Li XianJing <xianjimli@hotmail.com> adapted from ftk.
 *
 */

#include "str_gen.h"
#include "tkc/fs.h"
#include "tkc/str.h"
#include "base/enums.h"
#include "base/theme.h"
#include "common/utils.h"
#include "xml_str_gen.h"
#include "xml/xml_parser.h"
#include "base/assets_manager.h"

typedef struct _xml_str_builder_t {
  XmlBuilder builder;
  StrGen* sg;

  string tag;
  string str;
  uint32_t level;
  string language;
} xml_str_builder_t;

static void xml_str_gen_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  xml_str_builder_t* b = (xml_str_builder_t*)thiz;

  if (b->level == 0) {
    assert(strcmp(tag, "string") == 0);
    b->str = xml_builder_get_attr(attrs, "name");
  } else if (b->level == 1) {
    assert(strcmp(tag, "language") == 0);
    b->language = xml_builder_get_attr(attrs, "name");
  }

  b->level++;

  return;
}

static void xml_str_gen_on_end(XmlBuilder* thiz, const char* tag) {
  xml_str_builder_t* b = (xml_str_builder_t*)thiz;
  b->level--;
  (void)tag;

  return;
}

static void xml_str_gen_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  xml_str_builder_t* b = (xml_str_builder_t*)thiz;

  if (b->level == 2) {
    str_t str;
    str_t tr_str;
    str_init(&(str), length);
    str_init(&(tr_str), length);
    str_set_with_len(&tr_str, text, length);
    str_decode_xml_entity(&str, tr_str.str);

    b->sg->Add(b->language, Sentence(b->str, str.str));
    str_reset(&(str));
    str_reset(&(tr_str));
  }

  return;
}

static void xml_gen_on_comment(XmlBuilder* thiz, const char* text, size_t length) {
  (void)thiz;
  (void)text;
  (void)length;

  return;
}

static XmlBuilder* builder_init(xml_str_builder_t& b, StrGen* sg) {
  b.sg = sg;
  b.level = 0;
  b.builder.on_start = xml_str_gen_on_start;
  b.builder.on_end = xml_str_gen_on_end;
  b.builder.on_comment = xml_gen_on_comment;
  b.builder.on_text = xml_str_gen_on_text;

  return &(b.builder);
}

bool xml_to_str_gen(const char* input_file, StrGen* sg) {
  return xml_buff_to_str_gen((char*)file_read(input_file, NULL), sg);
}

bool xml_buff_to_str_gen(const char* buff, StrGen* sg) {
  xml_str_builder_t b;
  return_value_if_fail(buff != NULL && sg != NULL, false);

  XmlParser* parser = xml_parser_create();
  xml_parser_set_builder(parser, builder_init(b, sg));
  xml_parser_parse(parser, buff, strlen(buff));

  xml_parser_destroy(parser);

  return true;
}

static uint8_t output_buff[1024 * 1024];

bool xml_to_str_gen(const char* input_file, const char* output_dir, const char* theme, bool bin) {
  StrGen sg;
  uint8_t* p = NULL;
  char path[MAX_PATH + 1];

  return_value_if_fail(input_file != NULL && output_dir != NULL, false);
  return_value_if_fail(xml_to_str_gen(input_file, &sg) == true, false);

  memset(path, 0x00, sizeof(path));
  vector<string> languages = sg.GetLanguages();
  for (size_t i = 0; i < languages.size(); i++) {
    string iter = languages[i];
    p = sg.Output(iter, output_buff, sizeof(output_buff));
    uint32_t size = p - output_buff;

    if (bin) {
      snprintf(path, MAX_PATH, "%s/%s.bin", output_dir, iter.c_str());
      file_write(path, output_buff, size);
    } else {
      snprintf(path, MAX_PATH, "%s/%s.data", output_dir, iter.c_str());
      output_res_c_source(path, theme, ASSET_TYPE_STRINGS, 0, output_buff, size);
    }
    log_debug("write %s\n", path);
  }

  return true;
}
