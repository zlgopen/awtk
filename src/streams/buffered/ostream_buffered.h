/**
 * File:   ostream_buffered.h
 * Author: AWTK Develop Team
 * Brief:  buffered ostream: do write when call flush.
 *
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

#ifndef TK_OSTREAM_BUFFERED_H
#define TK_OSTREAM_BUFFERED_H

#include "tkc/buffer.h"
#include "tkc/ostream.h"

BEGIN_C_DECLS

struct _tk_ostream_buffered_t;
typedef struct _tk_ostream_buffered_t tk_ostream_buffered_t;

/**
 * @class tk_ostream_buffered_t
 * @parent tk_ostream_t
 *
 * 先将数据写入一个缓存区，调用flush时才真正写入底层的ostream。
 *
 */
struct _tk_ostream_buffered_t {
  tk_ostream_t ostream;

  wbuffer_t wb;
  tk_ostream_t* real_ostream;
};

/**
 * @method tk_ostream_buffered_create
 *
 * 创建ostream对象。
 *
 * 本函数自动增加real_ostream的引用计数。
 *
 * @param {tk_ostream_t*} real_ostream 实际的ostream对象。
 *
 * @return {tk_ostream_t*} 返回ostream对象。
 *
 */
tk_ostream_t* tk_ostream_buffered_create(tk_ostream_t* real_ostream);

/**
 * @method tk_ostream_buffered_get_size
 *
 * 获取当前缓存数据的长度。
 *
 * @param {tk_ostream_t*} ostream ostream对象。
 *
 * @return {uint32_t} 返回缓存数据的长度。
 *
 */
uint32_t tk_ostream_buffered_get_size(tk_ostream_t* ostream);

#define TK_OSTREAM_BUFFERED(obj) ((tk_ostream_buffered_t*)(obj))

END_C_DECLS

#endif /*TK_OSTREAM_BUFFERED_H*/
