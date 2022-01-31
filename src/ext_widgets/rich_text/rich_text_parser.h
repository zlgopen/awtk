/**
 * File:   rich_text_parser.h
 * Author: AWTK Develop Team
 * Brief:  rich_text_parser
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is dirich_text_parseributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-07-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_RICH_TEXT_PARSER_H
#define TK_RICH_TEXT_PARSER_H

#include "rich_text/rich_text_node.h"

BEGIN_C_DECLS

rich_text_node_t* rich_text_parse(const char* str, uint32_t size, const char* font_name,
                                  uint16_t font_size, color_t color, align_v_t align_v);

END_C_DECLS

#endif /*TK_RICH_TEXT_PARSER_H*/
