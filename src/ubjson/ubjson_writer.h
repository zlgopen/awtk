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
#include "ubjson/ubjson_const.h"

BEGIN_C_DECLS

struct _ubjson_writer_t;
typedef struct _ubjson_writer_t ubjson_writer_t;

typedef ret_t (*ubjson_write_callback_t)(void* ctx, const void* data, uint32_t size);

struct _ubjson_writer_t {
  void* ctx;
  ubjson_write_callback_t write;
};

ubjson_writer_t* ubjson_writer_init(ubjson_writer_t* writer, ubjson_write_callback_t write,
                                    void* ctx);

ret_t ubjson_writer_write_key(ubjson_writer_t* writer, const char* value);

ret_t ubjson_writer_write_null(ubjson_writer_t* writer);
ret_t ubjson_writer_write_noop(ubjson_writer_t* writer);
ret_t ubjson_writer_write_true(ubjson_writer_t* writer);
ret_t ubjson_writer_write_false(ubjson_writer_t* writer);
ret_t ubjson_writer_write_int(ubjson_writer_t* writer, int32_t value);
ret_t ubjson_writer_write_int8(ubjson_writer_t* writer, int8_t value);
ret_t ubjson_writer_write_uint8(ubjson_writer_t* writer, uint8_t value);
ret_t ubjson_writer_write_int16(ubjson_writer_t* writer, int16_t value);
ret_t ubjson_writer_write_int32(ubjson_writer_t* writer, int32_t value);
ret_t ubjson_writer_write_int64(ubjson_writer_t* writer, int64_t value);
ret_t ubjson_writer_write_float32(ubjson_writer_t* writer, float value);
ret_t ubjson_writer_write_float64(ubjson_writer_t* writer, double value);
ret_t ubjson_writer_write_char(ubjson_writer_t* writer, char value);
ret_t ubjson_writer_write_str(ubjson_writer_t* writer, const char* value);
ret_t ubjson_writer_write_str_len(ubjson_writer_t* writer, const char* value, uint32_t len);
ret_t ubjson_writer_write_object(ubjson_writer_t* writer, object_t* obj);
ret_t ubjson_writer_write_array_begin(ubjson_writer_t* writer);
ret_t ubjson_writer_write_array_end(ubjson_writer_t* writer);
ret_t ubjson_writer_write_object_begin(ubjson_writer_t* writer);
ret_t ubjson_writer_write_object_end(ubjson_writer_t* writer);

ret_t ubjson_writer_write_data(ubjson_writer_t* writer, const void* data, uint32_t size);
ret_t ubjson_writer_write_kv_object_begin(ubjson_writer_t* writer, const char* key);
ret_t ubjson_writer_write_kv_object(ubjson_writer_t* writer, const char* key, object_t* value);
ret_t ubjson_writer_write_kv_int(ubjson_writer_t* writer, const char* key, int32_t value);
ret_t ubjson_writer_write_kv_int64(ubjson_writer_t* writer, const char* key, int64_t value);
ret_t ubjson_writer_write_kv_float(ubjson_writer_t* writer, const char* key, float value);
ret_t ubjson_writer_write_kv_double(ubjson_writer_t* writer, const char* key, double value);
ret_t ubjson_writer_write_kv_bool(ubjson_writer_t* writer, const char* key, bool_t value);
ret_t ubjson_writer_write_kv_str(ubjson_writer_t* writer, const char* key, const char* value);
ret_t ubjson_writer_write_kv_str_len(ubjson_writer_t* writer, const char* key, const char* value,
                                     uint32_t len);
ret_t ubjson_writer_write_kv_value(ubjson_writer_t* writer, const char* key, value_t* value);

END_C_DECLS

#endif /*TK_UBJSON_WRITER_H*/
