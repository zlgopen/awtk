/**
 * File:   xml_builder.c
 * Author: AWTK Develop Team
 * Brief:  xml builder interface
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

#include "xml/xml_builder.h"

void xml_builder_on_start(XmlBuilder* thiz, const char* tag, const char** attrs) {
  return_if_fail(thiz != NULL && thiz->on_start != NULL);

  thiz->on_start(thiz, tag, attrs);

  return;
}

void xml_builder_on_end(XmlBuilder* thiz, const char* tag) {
  return_if_fail(thiz != NULL);
  if (thiz->on_end != NULL) {
    thiz->on_end(thiz, tag);
  }

  return;
}

void xml_builder_on_text(XmlBuilder* thiz, const char* text, size_t length) {
  return_if_fail(thiz != NULL);
  if (thiz->on_text != NULL) {
    thiz->on_text(thiz, text, length);
  }

  return;
}

void xml_builder_on_comment(XmlBuilder* thiz, const char* text, size_t length) {
  return_if_fail(thiz != NULL);
  if (thiz->on_comment != NULL) {
    thiz->on_comment(thiz, text, length);
  }

  return;
}

void xml_builder_on_pi(XmlBuilder* thiz, const char* tag, const char** attrs) {
  return_if_fail(thiz != NULL);
  if (thiz->on_pi != NULL) {
    thiz->on_pi(thiz, tag, attrs);
  }

  return;
}

void xml_builder_on_error(XmlBuilder* thiz, int line, int row, const char* message) {
  return_if_fail(thiz != NULL);
  if (thiz->on_error != NULL) {
    thiz->on_error(thiz, line, row, message);
  } else {
    log_debug("%d:%d %s\n", line, row, message);
  }

  return;
}

void xml_builder_destroy(XmlBuilder* thiz) {
  if (thiz != NULL && thiz->destroy != NULL) {
    thiz->destroy(thiz);
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
