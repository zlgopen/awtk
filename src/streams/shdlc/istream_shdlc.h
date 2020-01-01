/**
 * File:   istream_shdlc.h
 * Author: AWTK Develop Team
 * Brief:  reliable istream base on simple HDLC
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
 * 2019-10-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_ISTREAM_SHDLC_H
#define TK_ISTREAM_SHDLC_H

#include "tkc/buffer.h"
#include "tkc/compressor.h"
#include "tkc/ring_buffer.h"
#include "streams/shdlc/iostream_shdlc.h"

BEGIN_C_DECLS

struct _tk_istream_shdlc_t;
typedef struct _tk_istream_shdlc_t tk_istream_shdlc_t;

/**
 * @class tk_istream_shdlc_t
 * @parent tk_istream_t
 *
 * reliable istream base on simple HDLC
 *
 */
struct _tk_istream_shdlc_t {
  tk_istream_t istream;

  /**
   * @property {uint32_t} timeout
   * 读写超时时间(ms)
   */
  uint32_t timeout;
  /**
   * @property {uint32_t} retry_times
   * 失败重传次数。
   */
  uint8_t retry_times;

  wbuffer_t wb;
  ring_buffer_t* rb;
  uint8_t last_seqno;
  wbuffer_t wb_compress;
  compressor_t* compressor;
  tk_iostream_shdlc_t* iostream;
};

/**
 * @method tk_istream_shdlc_create
 *
 * 创建istream对象。
 *
 *> 只能由iostream_shdlc调用。
 *
 * @param {tk_iostream_shdlc_t*} iostream iostream对象。
 *
 * @return {tk_istream_t*} 返回istream对象。
 *
 */
tk_istream_t* tk_istream_shdlc_create(tk_iostream_shdlc_t* iostream);

ret_t tk_istream_shdlc_read_ack(tk_istream_t* istream, uint8_t seqno);

#define TK_ISTREAM_SHDLC(obj) ((tk_istream_shdlc_t*)(obj))

END_C_DECLS

#endif /*TK_ISTREAM_SHDLC_H*/
