/**
 * File:   ostream_shdlc.c
 * Author: AWTK Develop Team
 * Brief:  output stream for shdlc
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
#include "streams/ostream_shdlc.h"

static int32_t tk_ostream_shdlc_write(tk_ostream_t* stream, const uint8_t* buff, uint32_t size) {
  ret_t ret = 0;
  tk_ostream_shdlc_t* ostream_shdlc = TK_OSTREAM_SHDLC(stream);
  uint8_t seqno = ostream_shdlc->seqno;
  uint32_t timeout = ostream_shdlc->timeout;
  uint32_t retry_times = ostream_shdlc->retry_times;
  tk_istream_t* istream = ostream_shdlc->iostream->istream;
  tk_ostream_t* real_ostream = ostream_shdlc->iostream->real_ostream;

  wbuffer_t* wb = &(ostream_shdlc->wb);
  return_value_if_fail(shdlc_write_data(wb, seqno, buff, size) == RET_OK, 0);

  while (retry_times) {
    return_value_if_fail(
        tk_ostream_write_len(real_ostream, wb->data, wb->cursor, timeout) == wb->cursor, RET_IO);

    ret = tk_istream_shdlc_read_ack(istream, seqno);
    if (ret == RET_IO) {
      return 0;
    } else if (ret == RET_OK) {
      ostream_shdlc->seqno = shdlc_seqno_inc(ostream_shdlc->seqno);

      return size;
    }

    retry_times--;
  }

  log_debug("shdlc write failed\n");

  return 0;
}

static ret_t tk_ostream_shdlc_set_prop(object_t* obj, const char* name, const value_t* v) {
  tk_ostream_shdlc_t* ostream_shdlc = TK_OSTREAM_SHDLC(obj);
  tk_ostream_t* real_ostream = tk_iostream_get_ostream(ostream_shdlc->iostream->real_iostream);

  return object_set_prop(OBJECT(real_ostream), name, v);
}

static ret_t tk_ostream_shdlc_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_ostream_shdlc_t* ostream_shdlc = TK_OSTREAM_SHDLC(obj);
  tk_ostream_t* real_ostream = tk_iostream_get_ostream(ostream_shdlc->iostream->real_iostream);

  return object_get_prop(OBJECT(real_ostream), name, v);
}

static ret_t tk_ostream_shdlc_on_destroy(object_t* obj) {
  tk_ostream_shdlc_t* ostream_shdlc = TK_OSTREAM_SHDLC(obj);
  ENSURE(wbuffer_deinit(&(ostream_shdlc->wb)) == RET_OK);

  return RET_OK;
}

static const object_vtable_t s_tk_ostream_shdlc_vtable = {.type = "tk_ostream_shdlc",
                                                          .desc = "tk_ostream_shdlc",
                                                          .size = sizeof(tk_ostream_shdlc_t),
                                                          .on_destroy = tk_ostream_shdlc_on_destroy,
                                                          .get_prop = tk_ostream_shdlc_get_prop,
                                                          .set_prop = tk_ostream_shdlc_set_prop};

tk_ostream_t* tk_ostream_shdlc_create(tk_iostream_shdlc_t* iostream) {
  object_t* obj = NULL;
  tk_ostream_shdlc_t* ostream_shdlc = NULL;
  return_value_if_fail(iostream != NULL, NULL);

  obj = object_create(&s_tk_ostream_shdlc_vtable);
  ostream_shdlc = TK_OSTREAM_SHDLC(obj);
  return_value_if_fail(ostream_shdlc != NULL, NULL);

  ostream_shdlc->timeout = 3000;
  ostream_shdlc->retry_times = 10;
  ostream_shdlc->iostream = iostream;
  ENSURE(wbuffer_init_extendable(&(ostream_shdlc->wb)) != NULL);
  TK_OSTREAM(obj)->write = tk_ostream_shdlc_write;

  return TK_OSTREAM(obj);
}
