/**
 * File:   ostream_buffered.c
 * Author: AWTK Develop Team
 * Brief:  buffered ostream: do write when call flush.
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
 * 2019-10-06 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/buffer.h"
#include "streams/buffered/ostream_buffered.h"

static int32_t tk_ostream_buffered_write(tk_ostream_t* stream, const uint8_t* buff,
                                         uint32_t max_size) {
  tk_ostream_buffered_t* ostream_buffered = TK_OSTREAM_BUFFERED(stream);
  wbuffer_t* wb = &(ostream_buffered->wb);
  uint32_t old_size = wb->cursor;

  ENSURE(wbuffer_write_binary(wb, buff, max_size) == RET_OK);

  return wb->cursor - old_size;
}

static ret_t tk_ostream_buffered_flush(tk_ostream_t* stream) {
  tk_ostream_buffered_t* ostream_buffered = TK_OSTREAM_BUFFERED(stream);

  if (ostream_buffered->wb.cursor > 0) {
    wbuffer_t* wb = &(ostream_buffered->wb);
    tk_ostream_t* ostream = ostream_buffered->real_ostream;
    int32_t size = tk_ostream_write(ostream, wb->data, wb->cursor);

    if (size < 0) {
      return RET_FAIL;
    } else if (size == wb->cursor) {
      wb->cursor = 0;
    } else {
      void* d = wb->data;
      void* s = wb->data + size;
      size = wb->cursor - size;
      memmove(d, s, size);
      wb->cursor = size;
    }
  }

  return RET_OK;
}

static ret_t tk_ostream_buffered_set_prop(object_t* obj, const char* name, const value_t* v) {
  tk_ostream_buffered_t* ostream_buffered = TK_OSTREAM_BUFFERED(obj);
  tk_ostream_t* real_ostream = ostream_buffered->real_ostream;

  return object_set_prop(OBJECT(real_ostream), name, v);
}

static ret_t tk_ostream_buffered_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_ostream_buffered_t* ostream_buffered = TK_OSTREAM_BUFFERED(obj);
  tk_ostream_t* real_ostream = ostream_buffered->real_ostream;

  return object_get_prop(OBJECT(real_ostream), name, v);
}

static ret_t tk_ostream_buffered_on_destroy(object_t* obj) {
  tk_ostream_buffered_t* ostream_buffered = TK_OSTREAM_BUFFERED(obj);

  wbuffer_deinit(&(ostream_buffered->wb));
  OBJECT_UNREF(ostream_buffered->real_ostream);

  return RET_OK;
}

static const object_vtable_t s_tk_ostream_buffered_vtable = {
    .type = "tk_ostream_buffered",
    .desc = "tk_ostream_buffered",
    .size = sizeof(tk_ostream_buffered_t),
    .on_destroy = tk_ostream_buffered_on_destroy,
    .get_prop = tk_ostream_buffered_get_prop,
    .set_prop = tk_ostream_buffered_set_prop};

tk_ostream_t* tk_ostream_buffered_create(tk_ostream_t* real_ostream) {
  object_t* obj = NULL;
  tk_ostream_buffered_t* ostream_buffered = NULL;
  return_value_if_fail(real_ostream != NULL, NULL);

  obj = object_create(&s_tk_ostream_buffered_vtable);
  ostream_buffered = TK_OSTREAM_BUFFERED(obj);
  return_value_if_fail(ostream_buffered != NULL, NULL);

  OBJECT_REF(real_ostream);
  ostream_buffered->real_ostream = real_ostream;
  wbuffer_init_extendable(&(ostream_buffered->wb));

  TK_OSTREAM(obj)->write = tk_ostream_buffered_write;
  TK_OSTREAM(obj)->flush = tk_ostream_buffered_flush;

  return TK_OSTREAM(obj);
}

uint32_t tk_ostream_buffered_get_size(tk_ostream_t* ostream) {
  tk_ostream_buffered_t* ostream_buffered = TK_OSTREAM_BUFFERED(ostream);
  return_value_if_fail(ostream_buffered != NULL, 0);

  return ostream_buffered->wb.cursor;
}
