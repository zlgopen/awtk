/**
 * File:   xml_parse.h
 * Author: AWTK Develop Team
 * Brief:  simple xml parser
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

#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "xml/xml_builder.h"

BEGIN_C_DECLS

#define MAX_ATTR_NR 24

struct _XmlParser;
typedef struct _XmlParser XmlParser;

XmlParser* xml_parser_create(void);
void xml_parser_set_builder(XmlParser* thiz, XmlBuilder* builder);
void xml_parser_parse(XmlParser* thiz, const char* xml, int length);
void xml_parser_parse_file(XmlParser* thiz, const char* filename);
void xml_parser_destroy(XmlParser* thiz);

END_C_DECLS

#endif /*XML_PARSER_H*/
