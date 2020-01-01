/**
 * File:   ostream_shdlc.c
 * Author: AWTK Develop Team
 * Brief:  output stream for shdlc
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

#include "tkc/mem.h"
#include "compressors/compressor_miniz.h"
#include "streams/shdlc/shdlc_helper.h"
#include "streams/shdlc/istream_shdlc.h"
#include "streams/shdlc/ostream_shdlc.h"

static int32_t tk_ostream_shdlc_write(tk_ostream_t* stream, const uint8_t* buff, uint32_t size) {
  ret_t ret = 0;
  tk_ostream_shdlc_t* ostream_shdlc = TK_OSTREAM_SHDLC(stream);
  uint8_t seqno = ostream_shdlc->seqno;
  uint32_t timeout = ostream_shdlc->timeout;
  uint32_t retry_times = 0;
  tk_istream_t* istream = ostream_shdlc->iostream->istream;
  tk_ostream_t* real_ostream = ostream_shdlc->iostream->real_ostream;

  wbuffer_t* wb = &(ostream_shdlc->wb);
  if (ostream_shdlc->compress_threshold <= size) {
    compressor_t* c = ostream_shdlc->compressor;
    wbuffer_t* wb_c = &(ostream_shdlc->wb_compress);
    return_value_if_fail(compressor_compress(c, buff, size, wb_c) == RET_OK, 0);
    return_value_if_fail(shdlc_write_data(wb, seqno, TRUE, wb_c->data, wb_c->cursor) == RET_OK, 0);
  } else {
    return_value_if_fail(shdlc_write_data(wb, seqno, FALSE, buff, size) == RET_OK, 0);
  }

  while (retry_times < ostream_shdlc->retry_times) {
    if (!object_get_prop_bool(OBJECT(real_ostream), TK_STREAM_PROP_IS_OK, TRUE)) {
      return RET_IO;
    }

    return_value_if_fail(
        tk_ostream_write_len(real_ostream, wb->data, wb->cursor, timeout) == wb->cursor, RET_IO);

    ret = tk_istream_shdlc_read_ack(istream, seqno);
    if (ret == RET_IO) {
      return 0;
    } else if (ret == RET_OK) {
      ostream_shdlc->seqno = shdlc_seqno_inc(ostream_shdlc->seqno);

      return size;
    }

    retry_times++;
    log_debug("retry_times=%u\n", retry_times);
  }

  log_debug("shdlc write failed\n");

  return 0;
}

static ret_t tk_ostream_shdlc_set_prop(object_t* obj, const char* name, const value_t* v) {
  tk_ostream_shdlc_t* ostream_shdlc = TK_OSTREAM_SHDLC(obj);
  tk_ostream_t* real_ostream = tk_iostream_get_ostream(ostream_shdlc->iostream->real_iostream);

  if (tk_str_eq(name, TK_STREAM_PROP_TIMEOUT)) {
    ostream_shdlc->timeout = value_uint32(v);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_RETRY_TIMES)) {
    ostream_shdlc->retry_times = value_uint32(v);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_COMPRESS_THRESHOLD)) {
    ostream_shdlc->compress_threshold = value_uint32(v);
    return RET_OK;
  }

  return object_set_prop(OBJECT(real_ostream), name, v);
}

static ret_t tk_ostream_shdlc_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_ostream_shdlc_t* ostream_shdlc = TK_OSTREAM_SHDLC(obj);
  tk_ostream_t* real_ostream = tk_iostream_get_ostream(ostream_shdlc->iostream->real_iostream);

  if (tk_str_eq(name, TK_STREAM_PROP_TIMEOUT)) {
    value_set_uint32(v, ostream_shdlc->timeout);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_RETRY_TIMES)) {
    value_set_uint32(v, ostream_shdlc->retry_times);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_COMPRESS_THRESHOLD)) {
    value_set_uint32(v, ostream_shdlc->compress_threshold);
    return RET_OK;
  }

  return object_get_prop(OBJECT(real_ostream), name, v);
}

static ret_t tk_ostream_shdlc_on_destroy(object_t* obj) {
  tk_ostream_shdlc_t* ostream_shdlc = TK_OSTREAM_SHDLC(obj);
  ENSURE(wbuffer_deinit(&(ostream_shdlc->wb)) == RET_OK);
  ENSURE(wbuffer_deinit(&(ostream_shdlc->wb_compress)) == RET_OK);
  ENSURE(compressor_destroy(ostream_shdlc->compressor) == RET_OK);

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
  ostream_shdlc->compress_threshold = 512;
  ostream_shdlc->compressor = compressor_miniz_create(COMPRESSOR_RATIO_FIRST);
  ENSURE(wbuffer_init_extendable(&(ostream_shdlc->wb)) != NULL);
  ENSURE(wbuffer_init_extendable(&(ostream_shdlc->wb_compress)) != NULL);
  TK_OSTREAM(obj)->write = tk_ostream_shdlc_write;

  return TK_OSTREAM(obj);
}
