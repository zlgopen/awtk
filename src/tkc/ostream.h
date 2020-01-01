/**
 * File:   tk_ostream.h
 * Author: AWTK Develop Team
 * Brief:  output stream interface
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

/**
 * @class tk_ostream_t
 * @parent object_t
 *
 * 输出流的接口。
 *
 */
struct _tk_ostream_t {
  object_t object;

  tk_ostream_flush_t flush;
  tk_ostream_write_t write;
  tk_ostream_seek_t seek;
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
 * @param {uint8_t} byte 数据。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 *
 */
ret_t tk_ostream_write_byte(tk_ostream_t* stream, uint8_t byte);

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

#define TK_OSTREAM(obj) ((tk_ostream_t*)(obj))

END_C_DECLS

#endif /*TK_OSTREAM_H*/
