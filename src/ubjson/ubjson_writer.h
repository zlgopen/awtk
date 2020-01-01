/**
 * File:   ubjson_writer.h
 * Author: AWTK Develop Team
 * Brief:  ubjson writer
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

#ifndef TK_UBJSON_WRITER_H
#define TK_UBJSON_WRITER_H

#include "tkc/types_def.h"
#include "ubjson/ubjson_const.h"

BEGIN_C_DECLS

/**
 * @class ubjson_writer_t
 * ubjson writer
 *
 */
struct _ubjson_writer_t;
typedef struct _ubjson_writer_t ubjson_writer_t;

typedef ret_t (*ubjson_write_callback_t)(void* ctx, const void* data, uint32_t size);

struct _ubjson_writer_t {
  void* ctx;
  ubjson_write_callback_t write;
};

/**
 * @method ubjson_writer_init
 *
 * 初始化。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {ubjson_write_callback_t} write 真正写入数据的函数。
 * @param {void*} ctx write函数的上下文。
 *
 * @return {ubjson_writer_t*} 返回writer对象。
 */
ubjson_writer_t* ubjson_writer_init(ubjson_writer_t* writer, ubjson_write_callback_t write,
                                    void* ctx);

/**
 * @method ubjson_writer_write_key
 *
 * 写入key。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} value key的名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_key(ubjson_writer_t* writer, const char* value);

/**
 * @method ubjson_writer_write_null
 *
 * 写入null。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_null(ubjson_writer_t* writer);

/**
 * @method ubjson_writer_write_noop
 *
 * 写入noop。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_noop(ubjson_writer_t* writer);

/**
 * @method ubjson_writer_write_true
 *
 * 写入true。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_true(ubjson_writer_t* writer);

/**
 * @method ubjson_writer_write_false
 *
 * 写入false。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_false(ubjson_writer_t* writer);

/**
 * @method ubjson_writer_write_int
 *
 * 写入int。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {int32_t} value 要写入的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_int(ubjson_writer_t* writer, int32_t value);

/**
 * @method ubjson_writer_write_int8
 *
 * 写入int8。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {int8_t} value 要写入的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_int8(ubjson_writer_t* writer, int8_t value);

/**
 * @method ubjson_writer_write_uint8
 *
 * 写入uint8。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {uint8_t} value 要写入的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_uint8(ubjson_writer_t* writer, uint8_t value);

/**
 * @method ubjson_writer_write_int16
 *
 * 写入int16。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {int16_t} value 要写入的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_int16(ubjson_writer_t* writer, int16_t value);

/**
 * @method ubjson_writer_write_int32
 *
 * 写入int32。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {int32_t} value 要写入的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_int32(ubjson_writer_t* writer, int32_t value);

/**
 * @method ubjson_writer_write_int64
 *
 * 写入int64。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {int64_t} value 要写入的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_int64(ubjson_writer_t* writer, int64_t value);

/**
 * @method ubjson_writer_write_float32
 *
 * 写入float。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {float} value 要写入的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_float32(ubjson_writer_t* writer, float value);

/**
 * @method ubjson_writer_write_float64
 *
 * 写入double。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {double} value 要写入的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_float64(ubjson_writer_t* writer, double value);

/**
 * @method ubjson_writer_write_char
 *
 * 写入char。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {char} value 要写入的值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_char(ubjson_writer_t* writer, char value);

/**
 * @method ubjson_writer_write_str
 *
 * 写入str。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} value 要写入的字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_str(ubjson_writer_t* writer, const char* value);

/**
 * @method ubjson_writer_write_str_len
 *
 * 写入str。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} value 要写入的字符串。
 * @param {uint32_t} len 字符串的长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_str_len(ubjson_writer_t* writer, const char* value, uint32_t len);

/**
 * @method ubjson_writer_write_object
 *
 * 写入obj。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {object_t*} obj 要写入的对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_object(ubjson_writer_t* writer, object_t* obj);

/**
 * @method ubjson_writer_write_array_begin
 *
 * 写入数组开始。
 * > 即： [
 *
 * @param {ubjson_writer_t*} writer writer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_array_begin(ubjson_writer_t* writer);

/**
 * @method ubjson_writer_write_array_end
 *
 * 写入数组结束。
 * > 即： ]
 *
 * @param {ubjson_writer_t*} writer writer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_array_end(ubjson_writer_t* writer);

/**
 * @method ubjson_writer_write_object_begin
 *
 * 写入对象开始。
 * > 即： {
 *
 * @param {ubjson_writer_t*} writer writer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_object_begin(ubjson_writer_t* writer);

/**
 * @method ubjson_writer_write_object_end
 *
 * 写入对象结束。
 * > 即：}
 * @param {ubjson_writer_t*} writer writer对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_object_end(ubjson_writer_t* writer);

/**
 * @method ubjson_writer_write_data
 *
 * 写入二进制数据。
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const void*} data 数据。
 * @param {uint32_t} size 数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_data(ubjson_writer_t* writer, const void* data, uint32_t size);

/**
 * @method ubjson_writer_write_kv_object_begin
 *
 * 写key和对象的开始。
 *
 * > 即："key": {
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} key 键值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_kv_object_begin(ubjson_writer_t* writer, const char* key);

/**
 * @method ubjson_writer_write_kv_object
 *
 * 写key和对象。
 *
 * > 即："key": { object }
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} key 键名。
 * @param {object_t*} value 对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_kv_object(ubjson_writer_t* writer, const char* key, object_t* value);

/**
 * @method ubjson_writer_write_kv_int
 *
 * 写入key和int的值。
 *
 * > 即："key": value
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} key 键名。
 * @param {int32_t} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_kv_int(ubjson_writer_t* writer, const char* key, int32_t value);

/**
 * @method ubjson_writer_write_kv_int64
 *
 * 写入key和int64_t的值。
 *
 * > 即："key": value
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} key 键名。
 * @param {int64_t} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_kv_int64(ubjson_writer_t* writer, const char* key, int64_t value);

/**
 * @method ubjson_writer_write_kv_float
 *
 * 写入key和float的值。
 *
 * > 即："key": value
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} key 键名。
 * @param {float} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_kv_float(ubjson_writer_t* writer, const char* key, float value);

/**
 * @method ubjson_writer_write_kv_double
 *
 * 写入key和double的值。
 *
 * > 即："key": value
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} key 键名。
 * @param {double} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_kv_double(ubjson_writer_t* writer, const char* key, double value);

/**
 * @method ubjson_writer_write_kv_bool
 *
 * 写入key和bool的值。
 *
 * > 即："key": value
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} key 键名。
 * @param {bool_t} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_kv_bool(ubjson_writer_t* writer, const char* key, bool_t value);

/**
 * @method ubjson_writer_write_kv_str
 *
 * 写入key和str的值。
 *
 * > 即："key": value
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} key 键名。
 * @param {const char*} value 字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_kv_str(ubjson_writer_t* writer, const char* key, const char* value);

/**
 * @method ubjson_writer_write_kv_str_len
 *
 * 写入key和str的值。
 *
 * > 即："key": value
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} key 键名。
 * @param {const char*} value 字符串。
 * @param {uint32_t} len 字符串的长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_kv_str_len(ubjson_writer_t* writer, const char* key, const char* value,
                                     uint32_t len);

/**
 * @method ubjson_writer_write_kv_value
 *
 * 写入key和value_t的值。
 *
 * > 即："key": value
 *
 * @param {ubjson_writer_t*} writer writer对象。
 * @param {const char*} key 键名。
 * @param {value_t*} value 值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t ubjson_writer_write_kv_value(ubjson_writer_t* writer, const char* key, value_t* value);

END_C_DECLS

#endif /*TK_UBJSON_WRITER_H*/
