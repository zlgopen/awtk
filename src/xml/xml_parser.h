/**
 * File:   xml_parse.h
 * Author: AWTK Develop Team
 * Brief:  simple xml parser
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "xml/xml_builder.h"

BEGIN_C_DECLS

#ifndef TK_XML_MAX_ATTRS
#define TK_XML_MAX_ATTRS 24
#endif /*TK_XML_MAX_ATTRS*/

#define MAX_ATTR_KEY_VALUE_NR (TK_XML_MAX_ATTRS * 2)

/**
 * @class XmlParser
 * xml解析器
 */

struct _XmlParser;
typedef struct _XmlParser XmlParser;

/**
 * @method xml_parser_create
 *
 * 创建解析器。
 *
 * @return {XmlParser*} 返回parser对象。
 */
XmlParser* xml_parser_create(void);

/**
 * @method xml_parser_set_builder
 *
 * 设置builder。
 *
 * @param {XmlParser*} parser parser对象。
 * @param {XmlBuilder*} builder builder对象。
 *
 * @return {void} 返回无。
 */
void xml_parser_set_builder(XmlParser* parser, XmlBuilder* builder);

/**
 * @method xml_parser_set_trim_text
 *
 * 设置是否去掉文本两端的空白字符。
 *
 * > 默认值为去掉文本两端的空白字符，如果不希望去掉，调用本函数设置为FALSE。
 *
 * @param {XmlParser*} parser parser对象。
 * @param {bool_t} trim_text 是否去掉文本两端的空白字符。
 *
 * @return {void} 返回无。
 */
void xml_parser_set_trim_text(XmlParser* parser, bool_t trim_text);

/**
 * @method xml_parser_parse
 *
 * 解析数据。
 *
 * @param {XmlParser*} parser parser对象。
 * @param {const char*} xml 数据。
 * @param {int} length 数据长度。
 *
 * @return {void} 返回无。
 */
void xml_parser_parse(XmlParser* parser, const char* xml, int length);

/**
 * @method xml_parser_parse_file
 *
 * 解析文件。
 *
 * @param {XmlParser*} parser parser对象。
 * @param {const char*} filename 文件名。
 *
 * @return {void} 返回无。
 */
void xml_parser_parse_file(XmlParser* parser, const char* filename);

/**
 * @method xml_parser_destroy
 *
 * 销毁parser对象。
 *
 * @param {XmlParser*} parser parser对象。
 *
 * @return {void} 返回无。
 */
void xml_parser_destroy(XmlParser* parser);

END_C_DECLS

#endif /*XML_PARSER_H*/
