/**
 * File:   ubjson_reader.h
 * Author: AWTK Develop Team
 * Brief:  ubjson reader
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UBJSON_READER_H
#define TK_UBJSON_READER_H

#include "tkc/str.h"
#include "ubjson/ubjson_const.h"

BEGIN_C_DECLS

struct _ubjson_reader_t;
typedef struct _ubjson_reader_t ubjson_reader_t;

typedef ret_t (*ubjson_read_callback_t)(void* ctx, void* data, uint32_t size);

struct _ubjson_reader_t {
  void* ctx;
  str_t str;
  ubjson_read_callback_t read;
};

ubjson_reader_t* ubjson_reader_init(ubjson_reader_t* reader, ubjson_read_callback_t read,
                                    void* ctx);

ret_t ubjson_reader_read(ubjson_reader_t* reader, value_t* v);
ret_t ubjson_reader_read_data(ubjson_reader_t* reader, void* data, uint32_t size);

ret_t ubjson_reader_reset(ubjson_reader_t* reader);

END_C_DECLS

#endif /*TK_UBJSON_READER_H*/
