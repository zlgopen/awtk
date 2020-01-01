/**
 * File:   iostream_shdlc.h
 * Author: AWTK Develop Team
 * Brief:  reliable iostream base on simple HDLC
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
 * 2019-10-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_IOSTREAM_SHDLC_H
#define TK_IOSTREAM_SHDLC_H

#include "tkc/fs.h"
#include "tkc/iostream.h"

BEGIN_C_DECLS

struct _tk_iostream_shdlc_t;
typedef struct _tk_iostream_shdlc_t tk_iostream_shdlc_t;

/**
 * @class tk_iostream_shdlc_t
 * @parent tk_iostream_t
 *
 * 基于简化版的HDLC协议实现可靠的iostream。
 *
 * 一般用于串口通信。
 *
 */
struct _tk_iostream_shdlc_t {
  tk_iostream_t iostream;

  tk_istream_t* istream;
  tk_ostream_t* ostream;

  tk_istream_t* real_istream;
  tk_ostream_t* real_ostream;
  tk_iostream_t* real_iostream;
};

/**
 * @method tk_iostream_shdlc_create
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
tk_iostream_t* tk_iostream_shdlc_create(tk_iostream_t* real_iostream);

#define TK_IOSTREAM_SHDLC(obj) ((tk_iostream_shdlc_t*)(obj))

END_C_DECLS

#endif /*TK_IOSTREAM_SHDLC_H*/
