/**
 * File:   color_parser.h
 * Author: AWTK Develop Team
 * Brief:  color parser
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/color.h"
#include "base/types_def.h"

#ifndef COLOR_PARSER_H
#define COLOR_PARSER_H

BEGIN_C_DECLS

bool_t color_parse(const char* color, uint8_t* r, uint8_t* g, uint8_t* b, uint8_t* a);

color_t color_parse_simple(const char* color);

END_C_DECLS

#endif /*COLOR_PARSER_H*/
