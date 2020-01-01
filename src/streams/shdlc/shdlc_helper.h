/**
 * File:   shdlc_helper.h
 * Author: AWTK Develop Team
 * Brief:  iostream for shdlc
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

#ifndef TK_SHDLC_HELPER_H
#define TK_SHDLC_HELPER_H

#include "tkc/istream.h"
#include "tkc/ostream.h"
#include "tkc/buffer.h"

BEGIN_C_DECLS

#define SHDLC_FLAG 0x7E
#define SHDLC_ESCAPE 0x7D

typedef enum { SHDLC_INVALID = 0, SHDLC_DATA, SHDLC_ACK, SHDLC_NACK } shdlc_frame_type_t;

#pragma pack(push, 1)
typedef union _shdlc_header_t {
  uint8_t data;
  struct {
    uint8_t type : 3;
    uint8_t seqno : 3;
    uint8_t compressed : 1;
    uint8_t reserve : 1;
  } s;
} shdlc_header_t;
#pragma pack(pop)

/**
 * normal:
 * | SHDLC_FLAG(1B) | SHDLC_DATA(1B) | ...data... | fcs16(2B) | SHDLC_FLAG(1B) |
 * ack:
 * | SHDLC_FLAG(1B) | SHDLC_ACK(1B) | fcs16(2B) | SHDLC_FLAG(1B) |
 * nack:
 * | SHDLC_FLAG(1B) | SHDLC_NACK(1B) | fcs16(2B) | SHDLC_FLAG(1B) |
 */

ret_t shdlc_write_ack(wbuffer_t* wb, uint8_t seqno);
ret_t shdlc_write_nack(wbuffer_t* wb, uint8_t seqno);
ret_t shdlc_read_data(tk_istream_t* istream, wbuffer_t* wb, uint32_t timeout);
ret_t shdlc_write_data(wbuffer_t* wb, uint8_t seqno, bool_t compressed, const void* data,
                       uint32_t len);
uint8_t shdlc_seqno_inc(uint8_t seqno);

END_C_DECLS

#endif /*TK_SHDLC_HELPER_H*/
