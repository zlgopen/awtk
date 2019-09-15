/**
 * File:   ubjson_writer.h
 * Author: AWTK Develop Team
 * Brief:  ubjson writer
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
 * 2019-09-14 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_UBJSON_WRITER_H
#define TK_UBJSON_WRITER_H

#include "tkc/types_def.h"
#include "ubjson/ubjson_common.h"

BEGIN_C_DECLS

struct _ubjson_writer_t;
typedef struct _ubjson_writer_t ubjson_writer_t;

typedef int32_t (*ubjson_write_callback_t)(void* ctx, const void* data, uint32_t size);

struct _ubjson_writer_t {
  void* ctx;
  ubjson_write_callback_t write;
};

ubjson_writer_t* ubjson_writer_init(ubjson_writer_t* writer, ubjson_write_callback_t write,
                                    void* ctx);

ret_t ubjson_writer_write_null(ubjson_writer_t* writer);
ret_t ubjson_writer_write_noop(ubjson_writer_t* writer);
ret_t ubjson_writer_write_true(ubjson_writer_t* writer);
ret_t ubjson_writer_write_false(ubjson_writer_t* writer);
ret_t ubjson_writer_write_int8(ubjson_writer_t* writer, int8_t value);
ret_t ubjson_writer_write_uint8(ubjson_writer_t* writer, uint8_t value);
ret_t ubjson_writer_write_int16(ubjson_writer_t* writer, int16_t value);
ret_t ubjson_writer_write_int32(ubjson_writer_t* writer, int32_t value);
ret_t ubjson_writer_write_int64(ubjson_writer_t* writer, int64_t value);
ret_t ubjson_writer_write_float32(ubjson_writer_t* writer, float value);
ret_t ubjson_writer_write_float64(ubjson_writer_t* writer, double value);
ret_t ubjson_writer_write_char(ubjson_writer_t* writer, char value);
ret_t ubjson_writer_write_string(ubjson_writer_t* writer, const char* value);
ret_t ubjson_writer_write_array_begin(ubjson_writer_t* writer);
ret_t ubjson_writer_write_array_end(ubjson_writer_t* writer);
ret_t ubjson_writer_write_object_begin(ubjson_writer_t* writer);
ret_t ubjson_writer_write_object_end(ubjson_writer_t* writer);

END_C_DECLS

#endif /*TK_UBJSON_WRITER_H*/
