/**
 * File:   istream_buffered.h
 * Author: AWTK Develop Team
 * Brief:  buffered input stream
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
 * 2019-10-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ISTREAM_BUFFERED_H
#define TK_ISTREAM_BUFFERED_H

#include "tkc/istream.h"
#include "tkc/ring_buffer.h"

BEGIN_C_DECLS

struct _tk_istream_buffered_t;
typedef struct _tk_istream_buffered_t tk_istream_buffered_t;

/**
 * @class tk_istream_buffered_t
 * @parent tk_istream_t
 *
 * 缓冲输入流。
 *
 * 如果调用者每次只读取少量数据，可以使用本类对底层istream进行装饰，以提高读取效率。
 *
 */
struct _tk_istream_buffered_t {
  tk_istream_t istream;

  ring_buffer_t* rb;
  tk_istream_t* real_istream;
};

/**
 * @method tk_istream_buffered_create
 *
 * 创建istream对象。
 *
 * 本函数自动增加real_ostream的引用计数。
 *
 * @param {tk_istream_t*} real_istream 实际的istream。
 * @param {uint32_t} buffer_size 缓冲区的大小。
 *
 * @return {tk_istream_t*} 返回istream对象。
 *
 */
tk_istream_t* tk_istream_buffered_create(tk_istream_t* real_istream, uint32_t buffer_size);

#define TK_ISTREAM_BUFFERED(obj) ((tk_istream_buffered_t*)(obj))

END_C_DECLS

#endif /*TK_ISTREAM_BUFFERED_H*/
