/**
 * File:   tk_iostream.h
 * Author: AWTK Develop Team
 * Brief:  input/ouput stream interface
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
 * input/output stream interface
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

#define TK_IOSTREAM(obj) ((tk_iostream_t*)(obj))

END_C_DECLS

#endif /*TK_TK_IOSTREAM_H*/
