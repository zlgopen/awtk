/**
 * File:   ostream_shdlc.h
 * Author: AWTK Develop Team
 * Brief:  reliable ostream base on simple HDLC
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
 * 2019-10-10 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_OSTREAM_SHDLC_H
#define TK_OSTREAM_SHDLC_H

#include "tkc/buffer.h"
#include "streams/shdlc/iostream_shdlc.h"

BEGIN_C_DECLS

struct _tk_ostream_shdlc_t;
typedef struct _tk_ostream_shdlc_t tk_ostream_shdlc_t;

/**
 * @class tk_ostream_shdlc_t
 * @parent tk_ostream_t
 *
 * reliable ostream base on simple HDLC
 *
 */
struct _tk_ostream_shdlc_t {
  tk_ostream_t ostream;

  wbuffer_t wb;
  uint8_t seqno;
  uint32_t timeout;
  uint8_t retry_times;
  tk_iostream_shdlc_t* iostream;
};

/**
 * @method tk_ostream_shdlc_create
 *
 * 创建ostream对象。
 *
 *> 只能由iostream_shdlc调用。
 *
 * @param {tk_iostream_shdlc_t*} iostream iostream对象。
 *
 * @return {tk_ostream_t*} 返回ostream对象。
 *
 */
tk_ostream_t* tk_ostream_shdlc_create(tk_iostream_shdlc_t* iostream);

#define TK_OSTREAM_SHDLC(obj) ((tk_ostream_shdlc_t*)(obj))

END_C_DECLS

#endif /*TK_OSTREAM_SHDLC_H*/
