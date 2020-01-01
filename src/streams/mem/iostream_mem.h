/**
 * File:   iostream_mem.h
 * Author: AWTK Develop Team
 * Brief:  iostream base on mem
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
 * 2019-09-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IOSTREAM_MEM_H
#define TK_IOSTREAM_MEM_H

#include "tkc/fs.h"
#include "tkc/iostream.h"

BEGIN_C_DECLS

struct _tk_iostream_mem_t;
typedef struct _tk_iostream_mem_t tk_iostream_mem_t;

/**
 * @class tk_iostream_mem_t
 * @parent tk_iostream_t
 *
 * 基于内存实现的输入输出流。
 *
 */
struct _tk_iostream_mem_t {
  tk_iostream_t iostream;

  tk_istream_t* istream;
  tk_ostream_t* ostream;
};

/**
 * @method tk_iostream_mem_create
 *
 * 创建iostream对象。
 *
 * @param {void*} buff_in 输入流缓冲区。
 * @param {uint32_t} size_in 输入流缓冲区大小。
 * @param {void*} buff_out 输入流缓冲区。
 * @param {uoutt32_t} size_out 输入流缓冲区大小。
 * @param {bool_t} own_the_buff 是否让istream对象拥有buff。
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_iostream_mem_create(void* buff_in, uint32_t size_in, void* buff_out,
                                      uint32_t size_out, bool_t own_the_buff);

#define TK_IOSTREAM_MEM(obj) ((tk_iostream_mem_t*)(obj))

END_C_DECLS

#endif /*TK_IOSTREAM_MEM_H*/
