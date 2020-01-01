/**
 * File:   color_parser.h
 * Author: AWTK Develop Team
 * Brief:  color parser
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
 * 2018-01-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/color.h"
#include "tkc/types_def.h"

#ifndef COLOR_PARSER_H
#define COLOR_PARSER_H

BEGIN_C_DECLS

/**
 * @class color_parser_t
 * @annotation ["fake"]
 * 颜色解析相关函数。
 *
 * 示例：
 *
 * ```c
 * color_t c;
 * c = color_parse("#112233");
 * c = color_parse("white");
 * c = color_parse("rgb(11,22,33)");
 * c = color_parse("rgba(11,22,33,0.5)");
 * ```
 *
 */

/**
 * @method color_parse
 * 把字符串格式的颜色转换成color\_t对象。
 *
 * 目前支持下列格式：
 *
 * * 16进制格式。如："#112233"
 * * 颜色名称格式。如："green"
 * * rgb格式。如："rgb(11,22,33)"
 * * rgba格式。如："rgba(11,22,33,0.5)"
 *
 * @annotation ["static"]
 * @param {const char*} color 字符串格式的颜色。
 *
 * @return {void*} 返回color_t对象。
 */
color_t color_parse(const char* color);

END_C_DECLS

#endif /*COLOR_PARSER_H*/
