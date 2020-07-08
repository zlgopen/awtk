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
#include "tkc/buffer.h"
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
    str_t str;
    const char* name = xml_builder_get_attr(attrs, "name");
    str_init(&str, 100);
    str_decode_xml_entity(&str, name);
    b->str = str.str;
    str_reset(&(str));

    assert(strcmp(tag, "string") == 0);
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

bool xml_to_str_gen(const char* input_file, const char* output_dir, const char* theme, bool bin) {
  StrGen sg;
  uint8_t* p = NULL;
  char path[MAX_PATH + 1];

  return_value_if_fail(input_file != NULL && output_dir != NULL, false);
  return_value_if_fail(xml_to_str_gen(input_file, &sg) == true, false);

  memset(path, 0x00, sizeof(path));

  vector<string> languages = sg.GetLanguages();
  for (size_t i = 0; i < languages.size(); i++) {
    wbuffer_t wbuffer;
    wbuffer_init_extendable(&wbuffer);
    string iter = languages[i];
    uint32_t size = sg.Output(iter, &wbuffer);
    if (size > 0) {
      if (bin) {
        snprintf(path, MAX_PATH, "%s/%s.bin", output_dir, iter.c_str());
        file_write(path, wbuffer.data, size);
      } else {
        snprintf(path, MAX_PATH, "%s/%s.data", output_dir, iter.c_str());
        output_res_c_source(path, theme, ASSET_TYPE_STRINGS, 0, wbuffer.data, size);
      }
      log_debug("write %s\n", path);
    } else {
      log_debug("language  %s Sentences is NULL.\n", iter.c_str());
    }
    wbuffer_deinit(&wbuffer);
  }

  return true;
}
