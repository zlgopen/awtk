/**
 * File:   xml_builder.h
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

#include "tkc/types_def.h"

#ifndef XML_BUILDER_H
#define XML_BUILDER_H

BEGIN_C_DECLS

struct _XmlBuilder;
typedef struct _XmlBuilder XmlBuilder;

typedef void (*XmlBuilderOnStartFunc)(XmlBuilder* builder, const char* tag, const char** attrs);
typedef void (*XmlBuilderOnEndFunc)(XmlBuilder* builder, const char* tag);
typedef void (*XmlBuilderOnTextFunc)(XmlBuilder* builder, const char* text, size_t length);
typedef void (*XmlBuilderOnCommentFunc)(XmlBuilder* builder, const char* text, size_t length);
typedef void (*XmlBuilderOnPiFunc)(XmlBuilder* builder, const char* tag, const char** attrs);
typedef void (*XmlBuilderOnErrorFunc)(XmlBuilder* builder, int line, int col, const char* message);
typedef void (*XmlBuilderDestroyFunc)(XmlBuilder* builder);

/**
 * @class xml_builder_t
 * xml builder interface
 *
 */

struct _XmlBuilder {
  XmlBuilderOnStartFunc on_start;
  XmlBuilderOnEndFunc on_end;
  XmlBuilderOnTextFunc on_text;
  XmlBuilderOnCommentFunc on_comment;
  XmlBuilderOnPiFunc on_pi;
  XmlBuilderOnErrorFunc on_error;
  XmlBuilderDestroyFunc destroy;
};

/**
 * @method xml_builder_on_start
 * 解析到tag开始时调用本函数。
 *
 * @param {XmlBuilder*} builder builder对象。 
 * @param {const char*} tag  tag名称。
 * @param {const char**} attrs 属性列表。
 *
 * @return {void} 返回无。
 *
 */
void xml_builder_on_start(XmlBuilder* builder, const char* tag, const char** attrs);

/**
 * @method xml_builder_on_end
 * 解析到tag结束时调用本函数。
 *
 * @param {XmlBuilder*} builder builder对象。 
 * @param {const char*} tag  tag名称。
 *
 * @return {void} 返回无。
 *
 */
void xml_builder_on_end(XmlBuilder* builder, const char* tag);

/**
 * @method xml_builder_on_text
 * 解析到文本时调用本函数。
 *
 * @param {XmlBuilder*} builder builder对象。 
 * @param {const char*} text 文本。
 * @param {size_t} length 文本长度。
 *
 * @return {void} 返回无。
 *
 */
void xml_builder_on_text(XmlBuilder* builder, const char* text, size_t length);

/**
 * @method xml_builder_on_comment
 * 解析到注释时调用本函数。
 *
 * @param {XmlBuilder*} builder builder对象。 
 * @param {const char*} text 注释。
 * @param {size_t} length 注释长度。
 *
 * @return {void} 返回无。
 *
 */
void xml_builder_on_comment(XmlBuilder* builder, const char* text, size_t length);

/**
 * @method xml_builder_on_pi
 * 解析到处理指令时调用本函数。
 *
 * @param {XmlBuilder*} builder builder对象。 
 * @param {const char*} tag  tag名称。
 * @param {const char**} attrs 属性列表。
 *
 * @return {void} 返回无。
 *
 */
void xml_builder_on_pi(XmlBuilder* builder, const char* tag, const char** attrs);

/**
 * @method xml_builder_on_error
 * 解析遇到错误时调用本函数。
 *
 * @param {XmlBuilder*} builder builder对象。 
 * @param {int} line 出现错误的行。
 * @param {int} col 出现错误的列。
 * @param {const char*} message 错误信息。
 *
 * @return {void} 返回无。
 *
 */
void xml_builder_on_error(XmlBuilder* builder, int line, int col, const char* message);

/**
 * @method xml_builder_destroy
 * 销毁builder对象。
 *
 * @param {XmlBuilder*} builder builder对象。 
 *
 * @return {void} 返回无。
 *
 */
void xml_builder_destroy(XmlBuilder* builder);

/**
 * @method xml_builder_get_attr
 * 获取属性。
 * @annotation ["static"]
 * @param {const char**} attrs 属性列表。
 * @param {const char*} attr 属性名。
 *
 * @return {const char*} 返回属性的值。
 *
 */
const char* xml_builder_get_attr(const char** attrs, const char* attr);

END_C_DECLS

#endif /*XML_BUILDER_H*/
