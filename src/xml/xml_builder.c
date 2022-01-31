/**
 * File:   xml_builder.c
 * Author: AWTK Develop Team
 * Brief:  xml builder interface
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "xml/xml_builder.h"

void xml_builder_on_start(XmlBuilder* builder, const char* tag, const char** attrs) {
  return_if_fail(builder != NULL && builder->on_start != NULL);

  builder->on_start(builder, tag, attrs);

  return;
}

void xml_builder_on_end(XmlBuilder* builder, const char* tag) {
  return_if_fail(builder != NULL);
  if (builder->on_end != NULL) {
    builder->on_end(builder, tag);
  }

  return;
}

void xml_builder_on_text(XmlBuilder* builder, const char* text, size_t length) {
  return_if_fail(builder != NULL);
  if (builder->on_text != NULL) {
    builder->on_text(builder, text, length);
  }

  return;
}

void xml_builder_on_comment(XmlBuilder* builder, const char* text, size_t length) {
  return_if_fail(builder != NULL);
  if (builder->on_comment != NULL) {
    builder->on_comment(builder, text, length);
  }

  return;
}

void xml_builder_on_pi(XmlBuilder* builder, const char* tag, const char** attrs) {
  return_if_fail(builder != NULL);
  if (builder->on_pi != NULL) {
    builder->on_pi(builder, tag, attrs);
  }

  return;
}

void xml_builder_on_error(XmlBuilder* builder, int line, int col, const char* message) {
  return_if_fail(builder != NULL);
  if (builder->on_error != NULL) {
    builder->on_error(builder, line, col, message);
  } else {
    log_debug("%d:%d %s\n", line, col, message);
  }

  return;
}

void xml_builder_destroy(XmlBuilder* builder) {
  if (builder != NULL && builder->destroy != NULL) {
    builder->destroy(builder);
  }

  return;
}

const char* xml_builder_get_attr(const char** attrs, const char* attr) {
  uint32_t i = 0;
  return_value_if_fail(attrs != NULL && attr != NULL, NULL);

  while (attrs[i]) {
    const char* name = attrs[i];
    const char* value = attrs[i + 1];

    if (strcmp(name, attr) == 0) {
      return value;
    }

    i += 2;
  }

  return NULL;
}
