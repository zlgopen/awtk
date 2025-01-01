/**
 * File:   tk_ostream.h
 * Author: AWTK Develop Team
 * Brief:  output stream interface
 *
 * Copyright (c) 2019 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-08-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OSTREAM_H
#define TK_OSTREAM_H

#include "tkc/object.h"
#include "tkc/stream_const.h"

BEGIN_C_DECLS

struct _tk_ostream_t;
typedef struct _tk_ostream_t tk_ostream_t;

typedef int32_t (*tk_ostream_write_t)(tk_ostream_t* stream, const uint8_t* buff, uint32_t max_size);
typedef ret_t (*tk_ostream_flush_t)(tk_ostream_t* stream);
typedef ret_t (*tk_ostream_seek_t)(tk_ostream_t* stream, uint32_t offset);
typedef int32_t (*tk_ostream_tell_t)(tk_ostream_t* stream);

/**
 * @class tk_ostream_t
 * @parent tk_object_t
 *
 * 输出流的接口。
 *
 */
struct _tk_ostream_t {
  tk_object_t object;

  tk_ostream_flush_t flush;
  tk_ostream_write_t write;
  tk_ostream_seek_t seek;
  tk_ostream_tell_t tell;
};

/**
 * @method tk_ostream_write
 *
 * 写入数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {const void*} buff 返回数据的缓冲区。
 * @param {uint32_t} max_size 缓冲区的大小。
 *
 * @return {int32_t} 返回负数表示写入失败，否则返回实际写入数据的长度。
 *
 */
int32_t tk_ostream_write(tk_ostream_t* stream, const void* buff, uint32_t max_size);

/**
 * @method tk_ostream_seek
 *
 * 设置偏移量。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {uint32_t} offset 偏移量。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_seek(tk_ostream_t* stream, uint32_t offset);

/**
 * @method tk_ostream_tell
 *
 * 获取当前读取位置。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 *
 * @return {int32_t} 返回负数表示失败，否则返回当前读取位置。
 *
 */
int32_t tk_ostream_tell(tk_ostream_t* stream);

/**
 * @method tk_ostream_write_len
 *
 * 写入指定长度的数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {const void*} buff 数据的缓冲区。
 * @param {uint32_t} max_size 缓冲区的大小。
 * @param {uint32_t} timeout_ms timeout.
 *
 * @return {int32_t} 返回负数表示写入失败，否则返回实际写入数据的长度。
 *
 */
int32_t tk_ostream_write_len(tk_ostream_t* stream, const void* buff, uint32_t max_size,
                             uint32_t timeout_ms);

/**
 * @method tk_ostream_write_byte
 *
 * 写入一个字节的数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {uint8_t} value 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_byte(tk_ostream_t* stream, uint8_t value);

/**
 * @method tk_ostream_write_uint8
 *
 * 写入uint8数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {uint8_t} value 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_uint8(tk_ostream_t* stream, uint8_t value);

/**
 * @method tk_ostream_write_uint16
 *
 * 写入uint16数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {uint16_t} value 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_uint16(tk_ostream_t* stream, uint16_t value);

/**
 * @method tk_ostream_write_uint32
 *
 * 写入uint32数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {uint32_t} value 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_uint32(tk_ostream_t* stream, uint32_t value);

/**
 * @method tk_ostream_write_uint64
 *
 * 写入uint64数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {uint64_t} value 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_uint64(tk_ostream_t* stream, uint64_t value);

/**
 * @method tk_ostream_write_int8
 *
 * 写入int8数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {int8_t} value 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_int8(tk_ostream_t* stream, int8_t value);

/**
 * @method tk_ostream_write_int16
 *
 * 写入int16数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {int16_t} value 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_int16(tk_ostream_t* stream, int16_t value);

/**
 * @method tk_ostream_write_int32
 *
 * 写入int32数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {int32_t} value 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_int32(tk_ostream_t* stream, int32_t value);

/**
 * @method tk_ostream_write_int64
 *
 * 写入int64数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {int64_t} value 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_int64(tk_ostream_t* stream, int64_t value);

/**
 * @method tk_ostream_write_float
 *
 * 写入float数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {float} value 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_float(tk_ostream_t* stream, float value);

/**
 * @method tk_ostream_write_double
 *
 * 写入double数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {double} value 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_double(tk_ostream_t* stream, double value);

/**
 * @method tk_ostream_flush
 *
 * 刷新数据。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_flush(tk_ostream_t* stream);

/**
 * @method tk_ostream_write_str
 *
 * 写入字符串。
 *
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {const char*} str 字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_ostream_write_str(tk_ostream_t* stream, const char* str);

/**
 * @method tk_ostream_printf
 *
 * 写入格式化字符串。
 * > 长度不超过1024。
 * @param {tk_ostream_t*} stream ostream对象。
 * @param {const char*} format 格式化字符串。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_ostream_printf(tk_ostream_t* stream, const char* format, ...);

#define TK_OSTREAM(obj) ((tk_ostream_t*)(obj))
#define TK_OSTREAM_SEEKABLE(obj) (TK_OSTREAM(obj)->seek != NULL)
#define TK_OSTREAM_TELLABLE(obj) (TK_OSTREAM(obj)->tell != NULL)

/**
 * @method tk_ostream_unref
 *
 * 引用计数减1。引用计数为0时，销毁对象。
 * @param {tk_ostream_t*} stream ostream对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t tk_ostream_unref(tk_ostream_t* stream);

END_C_DECLS

#endif /*TK_OSTREAM_H*/
