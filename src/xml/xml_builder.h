/**
 * File:   xml_builder.h
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

#include "tkc/types_def.h"

#ifndef XML_BUILDER_H
#define XML_BUILDER_H

BEGIN_C_DECLS

struct _XmlBuilder;
typedef struct _XmlBuilder XmlBuilder;

typedef void (*XmlBuilderOnStartFunc)(XmlBuilder* thiz, const char* tag, const char** attrs);
typedef void (*XmlBuilderOnEndFunc)(XmlBuilder* thiz, const char* tag);
typedef void (*XmlBuilderOnTextFunc)(XmlBuilder* thiz, const char* text, size_t length);
typedef void (*XmlBuilderOnCommentFunc)(XmlBuilder* thiz, const char* text, size_t length);
typedef void (*XmlBuilderOnPiFunc)(XmlBuilder* thiz, const char* tag, const char** attrs);
typedef void (*XmlBuilderOnErrorFunc)(XmlBuilder* thiz, int line, int row, const char* message);
typedef void (*XmlBuilderDestroyFunc)(XmlBuilder* thiz);

struct _XmlBuilder {
  XmlBuilderOnStartFunc on_start;
  XmlBuilderOnEndFunc on_end;
  XmlBuilderOnTextFunc on_text;
  XmlBuilderOnCommentFunc on_comment;
  XmlBuilderOnPiFunc on_pi;
  XmlBuilderOnErrorFunc on_error;
  XmlBuilderDestroyFunc destroy;
};

void xml_builder_on_start(XmlBuilder* thiz, const char* tag, const char** attrs);
void xml_builder_on_end(XmlBuilder* thiz, const char* tag);
void xml_builder_on_text(XmlBuilder* thiz, const char* text, size_t length);
void xml_builder_on_comment(XmlBuilder* thiz, const char* text, size_t length);
void xml_builder_on_pi(XmlBuilder* thiz, const char* tag, const char** attrs);
void xml_builder_on_error(XmlBuilder* thiz, int line, int row, const char* message);
void xml_builder_destroy(XmlBuilder* thiz);

const char* xml_builder_get_attr(const char** attrs, const char* attr);

END_C_DECLS

#endif /*XML_BUILDER_H*/
