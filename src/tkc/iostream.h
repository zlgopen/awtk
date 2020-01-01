/**
 * File:   tk_iostream.h
 * Author: AWTK Develop Team
 * Brief:  input/ouput stream interface
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
 * 2019-08-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_TK_IOSTREAM_H
#define TK_TK_IOSTREAM_H

#include "tkc/istream.h"
#include "tkc/ostream.h"
#include "tkc/stream_const.h"

BEGIN_C_DECLS

struct _tk_iostream_t;
typedef struct _tk_iostream_t tk_iostream_t;

typedef tk_istream_t* (*tk_iostream_get_istream_t)(tk_iostream_t* stream);
typedef tk_ostream_t* (*tk_iostream_get_ostream_t)(tk_iostream_t* stream);

/**
 * @class tk_iostream_t
 * @parent object_t
 *
 * 输入输出流的接口。
 *
 */
struct _tk_iostream_t {
  object_t object;

  tk_iostream_get_istream_t get_istream;
  tk_iostream_get_ostream_t get_ostream;
};

/**
 * @method tk_iostream_get_istream
 *
 * 获取输入流对象。
 *
 * @param {tk_iostream_t*} stream iostream对象。
 *
 * @return {tk_istream_t*} 返回输入流对象。
 *
 */
tk_istream_t* tk_iostream_get_istream(tk_iostream_t* stream);

/**
 * @method tk_iostream_get_ostream
 *
 * 获取输出流对象。
 *
 * @param {tk_iostream_t*} stream iostream对象。
 *
 * @return {tk_ostream_t*} 返回输出流对象。
 *
 */
tk_ostream_t* tk_iostream_get_ostream(tk_iostream_t* stream);

/**
 * @method tk_iostream_read
 *
 * 读取数据。
 *
 * @param {tk_iostream_t*} stream iostream对象。
 * @param {void*} buff 返回数据的缓冲区。
 * @param {uint32_t} max_size 缓冲区的大小。
 *
 * @return {int32_t} 返回负数表示读取失败，否则返回实际读取数据的长度。
 *
 */
int32_t tk_iostream_read(tk_iostream_t* stream, void* buff, uint32_t max_size);

/**
 * @method tk_iostream_read_len
 *
 * 读取指定长度的数据。
 *
 * @param {tk_iostream_t*} stream iostream对象。
 * @param {void*} buff 返回数据的缓冲区。
 * @param {uint32_t} max_size 缓冲区的大小。
 * @param {uint32_t} timeout_ms timeout.
 *
 * @return {int32_t} 返回负数表示读取失败，否则返回实际读取数据的长度。
 *
 */
int32_t tk_iostream_read_len(tk_iostream_t* stream, void* buff, uint32_t max_size,
                             uint32_t timeout_ms);

/**
 * @method tk_iostream_write
 *
 * 写入数据。
 *
 * @param {tk_iostream_t*} stream iostream对象。
 * @param {const void*} buff 返回数据的缓冲区。
 * @param {uint32_t} max_size 缓冲区的大小。
 *
 * @return {int32_t} 返回负数表示写入失败，否则返回实际写入数据的长度。
 *
 */
int32_t tk_iostream_write(tk_iostream_t* stream, const void* buff, uint32_t max_size);

/**
 * @method tk_iostream_write_len
 *
 * 写入指定长度的数据。
 *
 * @param {tk_iostream_t*} stream iostream对象。
 * @param {const void*} buff 数据的缓冲区。
 * @param {uint32_t} max_size 缓冲区的大小。
 * @param {uint32_t} timeout_ms timeout.
 *
 * @return {int32_t} 返回负数表示写入失败，否则返回实际写入数据的长度。
 *
 */
int32_t tk_iostream_write_len(tk_iostream_t* stream, const void* buff, uint32_t max_size,
                              uint32_t timeout_ms);

#define TK_IOSTREAM(obj) ((tk_iostream_t*)(obj))

END_C_DECLS

#endif /*TK_TK_IOSTREAM_H*/
