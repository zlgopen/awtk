/**
 * File:   shdlc_helper.h
 * Author: AWTK Develop Team
 * Brief:  iostream for shdlc
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
 * 2019-10-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/crc.h"
#include "streams/shdlc/shdlc_helper.h"

static inline uint8_t shdlc_escape(uint8_t c) {
  return c ^ 0x20;
}

static inline uint8_t shdlc_unescape(uint8_t c) {
  return c ^ 0x20;
}

static ret_t shdlc_write_uint8(wbuffer_t* wb, uint8_t c) {
  if (c == SHDLC_FLAG || c == SHDLC_ESCAPE) {
    return_value_if_fail(wbuffer_write_uint8(wb, SHDLC_ESCAPE) == RET_OK, RET_FAIL);
    c = shdlc_escape(c);
  }

  return wbuffer_write_uint8(wb, c);
}

static ret_t shdlc_write_uint16(wbuffer_t* wb, uint16_t v) {
  uint8_t c = v & 0xff;
  return_value_if_fail(shdlc_write_uint8(wb, c) == RET_OK, RET_OOM);

  c = v >> 8;
  return shdlc_write_uint8(wb, c);
}

static ret_t shdlc_write(wbuffer_t* wb, shdlc_header_t header, const uint8_t* data, uint32_t len) {
  uint32_t i = 0;
  uint16_t fcs = 0;
  return_value_if_fail(wb != NULL && header.s.type != SHDLC_INVALID, RET_BAD_PARAMS);

  wb->cursor = 0;
  return_value_if_fail(wbuffer_write_uint8(wb, SHDLC_FLAG) == RET_OK, RET_OOM);
  return_value_if_fail(shdlc_write_uint8(wb, header.data) == RET_OK, RET_OOM);

  if (data != NULL && len > 0) {
    for (i = 0; i < len; i++) {
      return_value_if_fail(shdlc_write_uint8(wb, data[i]) == RET_OK, RET_OOM);
    }
  }

  fcs = tk_crc16(PPPINITFCS16, wb->data + 1, wb->cursor - 1);
  return_value_if_fail(shdlc_write_uint16(wb, fcs) == RET_OK, RET_OOM);
  return_value_if_fail(wbuffer_write_uint8(wb, SHDLC_FLAG) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t shdlc_write_ack(wbuffer_t* wb, uint8_t seqno) {
  shdlc_header_t header;
  header.s.reserve = 0;
  header.s.seqno = seqno;
  header.s.type = SHDLC_ACK;

  return shdlc_write(wb, header, NULL, 0);
}

ret_t shdlc_write_nack(wbuffer_t* wb, uint8_t seqno) {
  shdlc_header_t header;
  header.s.reserve = 0;
  header.s.seqno = seqno;
  header.s.type = SHDLC_NACK;

  return shdlc_write(wb, header, NULL, 0);
}

ret_t shdlc_write_data(wbuffer_t* wb, uint8_t seqno, const void* data, uint32_t len) {
  shdlc_header_t header;
  return_value_if_fail(data != NULL && len > 0, RET_BAD_PARAMS);

  header.s.reserve = 0;
  header.s.seqno = seqno;
  header.s.type = SHDLC_DATA;

  return shdlc_write(wb, header, (const uint8_t*)data, len);
}

ret_t shdlc_read_data(tk_istream_t* istream, wbuffer_t* wb, uint32_t timeout) {
  uint8_t c = 0;
  uint16_t fcs = 0;
  uint8_t last_c = 0;
  uint16_t fcs_real = 0;
  return_value_if_fail(istream != NULL && wb != NULL, RET_BAD_PARAMS);

  do {
    return_value_if_fail(tk_istream_read_len(istream, &c, 1, timeout) == 1, RET_IO);
    if (c == SHDLC_FLAG) {
      break;
    }
  } while (1);

  do {
    return_value_if_fail(tk_istream_read_len(istream, &c, 1, timeout) == 1, RET_IO);
    if (c != SHDLC_FLAG) {
      break;
    }
  } while (1);

  /*c is type*/
  wb->cursor = 0;
  return_value_if_fail(wbuffer_write_uint8(wb, c) == RET_OK, RET_OOM);

  do {
    return_value_if_fail(tk_istream_read_len(istream, &c, 1, timeout) == 1, RET_IO);

    if (c == SHDLC_FLAG) {
      uint8_t fcs_low = 0;
      uint8_t fcs_high = 0;
      return_value_if_fail(wb->cursor >= 2, RET_FAIL);
      wb->cursor -= 2;
      fcs_low = wb->data[wb->cursor];
      fcs_high = wb->data[wb->cursor + 1];
      fcs = fcs_low | (fcs_high << 8);
      fcs_real = tk_crc16(PPPINITFCS16, wb->data, wb->cursor);
      return_value_if_fail(fcs == fcs_real, RET_CRC);
      break;
    } else if (c == SHDLC_ESCAPE) {
    } else {
      if (last_c == SHDLC_ESCAPE) {
        c = shdlc_unescape(c);
      }
      return_value_if_fail(wbuffer_write_uint8(wb, c) == RET_OK, RET_OOM);
    }

    last_c = c;
  } while (1);

  return RET_OK;
}

uint8_t shdlc_seqno_inc(uint8_t seqno) {
  return (seqno + 1) & 0x07;
}
