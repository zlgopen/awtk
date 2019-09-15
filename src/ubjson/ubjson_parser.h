/**
 * File:   ubjson_parser.h
 * Author: AWTK Develop Team
 * Brief:  ubjson parser
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UBJSON_PARSER_H
#define TK_UBJSON_PARSER_H

#include "tkc/object_default.h"
#include "ubjson/ubjson_reader.h"

BEGIN_C_DECLS

typedef ret_t (*ubjson_on_key_value_t)(void* ctx, const char* key, value_t* value);

ret_t ubjson_parse(void* data, uint32_t size, ubjson_on_key_value_t on_key_value, void* ctx);

object_t* object_from_ubjson(void* data, uint32_t size);
ret_t ubjson_dump(void* data, uint32_t size);

END_C_DECLS

#endif /*TK_UBJSON_PARSER_H*/
