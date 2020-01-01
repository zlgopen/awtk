/**
 * File:   iostream_noisy.h
 * Author: AWTK Develop Team
 * Brief:  inject error into stream
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
 * 2019-10-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IOSTREAM_NOISY_H
#define TK_IOSTREAM_NOISY_H

#include "tkc/fs.h"
#include "tkc/iostream.h"
#include "streams/noisy/ostream_noisy.h"

BEGIN_C_DECLS

struct _tk_iostream_noisy_t;
typedef struct _tk_iostream_noisy_t tk_iostream_noisy_t;

/**
 * @class tk_iostream_noisy_t
 * @parent tk_iostream_t
 *
 * 在输出流中注入错误，方便进行可靠性测试。
 *
 */
struct _tk_iostream_noisy_t {
  tk_iostream_t iostream;

  tk_ostream_t* ostream;
  tk_istream_t* istream;

  tk_iostream_t* real_iostream;
};

/**
 * @method tk_iostream_noisy_create
 *
 * 创建iostream对象。
 *
 * 本函数自动增加real_iostream的引用计数。
 *
 * @param {tk_iostream_t*} real_iostream 底层的iostream。
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_iostream_noisy_create(tk_iostream_t* real_iostream);

#define TK_IOSTREAM_NOISY(obj) ((tk_iostream_noisy_t*)(obj))

END_C_DECLS

#endif /*TK_IOSTREAM_NOISY_H*/
