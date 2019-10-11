/**
 * File:   istream_shdlc.c
 * Author: AWTK Develop Team
 * Brief:  input stream for shdlc
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

#include "tkc/mem.h"
#include "streams/shdlc_helper.h"
#include "streams/istream_shdlc.h"

static ret_t tk_istream_shdlc_send_ack(tk_istream_t* stream, bool_t ok, uint8_t seqno) {
  wbuffer_t wb;
  uint8_t buff[32];
  tk_istream_shdlc_t* istream_shdlc = TK_ISTREAM_SHDLC(stream);

  uint32_t timeout = istream_shdlc->timeout;
  tk_ostream_t* real_ostream = istream_shdlc->iostream->real_ostream;

  wbuffer_init(&wb, buff, sizeof(buff));
  if (ok) {
    shdlc_write_ack(&wb, seqno);
  } else {
    shdlc_write_nack(&wb, seqno);
  }

  return tk_ostream_write_len(real_ostream, wb.data, wb.cursor, timeout) == wb.cursor ? RET_OK
                                                                                      : RET_IO;
}

ret_t tk_istream_shdlc_read_frame(tk_istream_t* stream, wbuffer_t* wb) {
  shdlc_header_t header = {0};
  tk_istream_shdlc_t* istream_shdlc = TK_ISTREAM_SHDLC(stream);
  uint32_t timeout = istream_shdlc->timeout;
  tk_istream_t* real_istream = istream_shdlc->iostream->real_istream;
  ret_t ret = shdlc_read_data(real_istream, wb, timeout);
  return_value_if_fail(ret != RET_IO, RET_IO);

  header.data = wb->data[0];
  if (header.s.type == SHDLC_DATA) {
    tk_istream_shdlc_send_ack(stream, ret == RET_OK, header.s.seqno);
  }

  return ret;
}

static ret_t tk_istream_shdlc_save_data_frame(tk_istream_t* stream, wbuffer_t* wb) {
  shdlc_header_t header = {0};
  tk_istream_shdlc_t* istream_shdlc = TK_ISTREAM_SHDLC(stream);
  ring_buffer_t* rb = (istream_shdlc->rb);

  header.data = wb->data[0];
  ENSURE(header.s.type == SHDLC_DATA);
  if (istream_shdlc->last_seqno != header.s.seqno) {
    return_value_if_fail(ring_buffer_write_len(rb, wb->data + 1, wb->cursor - 1) == RET_OK,
                         RET_OOM);
    istream_shdlc->last_seqno = header.s.seqno;
  } else {
    log_debug("dicard duplicated packet: %d\n", (int)(header.s.seqno));
  }

  return RET_OK;
}

ret_t tk_istream_shdlc_read_ack(tk_istream_t* stream, uint8_t seqno) {
  ret_t ret = RET_OK;
  shdlc_header_t header = {0};
  tk_istream_shdlc_t* istream_shdlc = TK_ISTREAM_SHDLC(stream);
  wbuffer_t* wb = &(istream_shdlc->wb);

  do {
    ret = tk_istream_shdlc_read_frame(stream, wb);
    return_value_if_fail(ret == RET_OK, ret);

    header.data = wb->data[0];
    if (header.s.type != SHDLC_DATA) {
      ENSURE(header.s.seqno == seqno);
      break;
    }

    return_value_if_fail(tk_istream_shdlc_save_data_frame(stream, wb) == RET_OK, RET_OOM);
  } while (TRUE);

  return header.s.type == SHDLC_ACK ? RET_OK : RET_FAIL;
}

static int32_t tk_istream_shdlc_read(tk_istream_t* stream, uint8_t* buff, uint32_t max_size) {
  tk_istream_shdlc_t* istream_shdlc = TK_ISTREAM_SHDLC(stream);
  wbuffer_t* wb = &(istream_shdlc->wb);
  ring_buffer_t* rb = (istream_shdlc->rb);

  if (ring_buffer_size(rb) > 0) {
    return ring_buffer_read(rb, buff, max_size);
  }

  return_value_if_fail(tk_istream_shdlc_read_frame(stream, wb) == RET_OK, 0);
  return_value_if_fail(tk_istream_shdlc_save_data_frame(stream, wb) == RET_OK, 0);

  return ring_buffer_read(rb, buff, max_size);
}

static ret_t tk_istream_shdlc_set_prop(object_t* obj, const char* name, const value_t* v) {
  tk_istream_shdlc_t* istream_shdlc = TK_ISTREAM_SHDLC(obj);
  tk_istream_t* real_istream = tk_iostream_get_istream(istream_shdlc->iostream->real_iostream);

  return object_set_prop(OBJECT(real_istream), name, v);
}

static ret_t tk_istream_shdlc_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_istream_shdlc_t* istream_shdlc = TK_ISTREAM_SHDLC(obj);
  tk_istream_t* real_istream = tk_iostream_get_istream(istream_shdlc->iostream->real_iostream);

  return object_get_prop(OBJECT(real_istream), name, v);
}

static ret_t tk_istream_shdlc_on_destroy(object_t* obj) {
  tk_istream_shdlc_t* istream_shdlc = TK_ISTREAM_SHDLC(obj);

  ENSURE(ring_buffer_destroy(istream_shdlc->rb) == RET_OK);
  ENSURE(wbuffer_deinit(&(istream_shdlc->wb)) == RET_OK);

  return RET_OK;
}

static const object_vtable_t s_tk_istream_shdlc_vtable = {.type = "tk_istream_shdlc",
                                                          .desc = "tk_istream_shdlc",
                                                          .size = sizeof(tk_istream_shdlc_t),
                                                          .on_destroy = tk_istream_shdlc_on_destroy,
                                                          .get_prop = tk_istream_shdlc_get_prop,
                                                          .set_prop = tk_istream_shdlc_set_prop};

tk_istream_t* tk_istream_shdlc_create(tk_iostream_shdlc_t* iostream) {
  object_t* obj = NULL;
  tk_istream_shdlc_t* istream_shdlc = NULL;
  return_value_if_fail(iostream != NULL, NULL);

  obj = object_create(&s_tk_istream_shdlc_vtable);
  istream_shdlc = TK_ISTREAM_SHDLC(obj);
  return_value_if_fail(istream_shdlc != NULL, NULL);

  istream_shdlc->rb = ring_buffer_create(1024, 32 * 1024);
  ENSURE(wbuffer_init_extendable(&(istream_shdlc->wb)) != NULL);

  istream_shdlc->timeout = 3000;
  istream_shdlc->last_seqno = 0xff;
  istream_shdlc->iostream = iostream;
  TK_ISTREAM(obj)->read = tk_istream_shdlc_read;

  return TK_ISTREAM(obj);
}
