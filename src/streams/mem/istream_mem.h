/**
 * File:   istream_mem.h
 * Author: AWTK Develop Team
 * Brief:  input stream base on memory
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

#ifndef TK_ISTREAM_MEM_H
#define TK_ISTREAM_MEM_H

#include "tkc/istream.h"

BEGIN_C_DECLS

struct _tk_istream_mem_t;
typedef struct _tk_istream_mem_t tk_istream_mem_t;

/**
 * @class tk_istream_mem_t
 * @parent tk_istream_t
 *
 * 基于内存实现的输入流。
 *
 */
struct _tk_istream_mem_t {
  tk_istream_t istream;

  bool_t own_the_buff;
  uint8_t* buff;
  uint32_t size;
  uint32_t cursor;
  uint32_t packet_size;
};

/**
 * @method tk_istream_mem_create
 *
 * 创建istream对象。
 *
 * @param {uint8_t*} buff 返回数据的缓冲区。
 * @param {uint32_t} size 缓冲区的大小。
 * @param {uint32_t} packet_size 缺省一次读取的数据的长度。
 * @param {bool_t} own_the_buff 是否让istream对象拥有buff。
 *
 * @return {tk_istream_t*} 返回istream对象。
 *
 */
tk_istream_t* tk_istream_mem_create(uint8_t* buff, uint32_t size, uint32_t packet_size,
                                    bool_t own_the_buff);

#define TK_ISTREAM_MEM(obj) ((tk_istream_mem_t*)(obj))

END_C_DECLS

#endif /*TK_ISTREAM_MEM_H*/
