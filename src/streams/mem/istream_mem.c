/**
 * File:   istream_mem.c
 * Author: AWTK Develop Team
 * Brief:  input stream base on memory
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
 * 2019-08-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "streams/mem/istream_mem.h"

static int32_t tk_istream_mem_read(tk_istream_t* stream, uint8_t* buff, uint32_t max_size) {
  int32_t size = max_size;
  tk_istream_mem_t* istream_mem = TK_ISTREAM_MEM(stream);

  if (istream_mem->packet_size && istream_mem->packet_size < max_size) {
    size = istream_mem->packet_size;
  }

  if (size > (istream_mem->size - istream_mem->cursor)) {
    size = istream_mem->size - istream_mem->cursor;
  }

  if (size > 0) {
    memcpy(buff, istream_mem->buff + istream_mem->cursor, size);
    istream_mem->cursor += size;
  } else {
    errno = EIO;
  }

  return size;
}

static ret_t tk_istream_mem_wait_for_data(tk_istream_t* stream, uint32_t timeout_ms) {
  tk_istream_mem_t* istream_mem = TK_ISTREAM_MEM(stream);
  if (istream_mem->cursor < istream_mem->size) {
    return RET_OK;
  } else {
    return RET_EOS;
  }
}

static ret_t tk_istream_mem_seek(tk_istream_t* stream, uint32_t offset) {
  tk_istream_mem_t* istream_mem = TK_ISTREAM_MEM(stream);
  return_value_if_fail(offset <= istream_mem->size, RET_BAD_PARAMS);

  istream_mem->cursor = offset;

  return RET_OK;
}

static ret_t tk_istream_mem_set_prop(object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t tk_istream_mem_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_istream_mem_t* istream_mem = TK_ISTREAM_MEM(obj);

  if (tk_str_eq(name, TK_STREAM_PROP_IS_EOS)) {
    value_set_bool(v, istream_mem->cursor == istream_mem->size);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tk_istream_mem_on_destroy(object_t* obj) {
  tk_istream_mem_t* istream_mem = TK_ISTREAM_MEM(obj);

  if (istream_mem->own_the_buff) {
    TKMEM_FREE(istream_mem->buff);
  }

  return RET_OK;
}

static const object_vtable_t s_tk_istream_mem_vtable = {.type = "tk_istream_mem",
                                                        .desc = "tk_istream_mem",
                                                        .size = sizeof(tk_istream_mem_t),
                                                        .on_destroy = tk_istream_mem_on_destroy,
                                                        .get_prop = tk_istream_mem_get_prop,
                                                        .set_prop = tk_istream_mem_set_prop};

tk_istream_t* tk_istream_mem_create(uint8_t* buff, uint32_t size, uint32_t packet_size,
                                    bool_t own_the_buff) {
  object_t* obj = NULL;
  tk_istream_mem_t* istream_mem = NULL;
  return_value_if_fail(buff != NULL && packet_size < size, NULL);

  obj = object_create(&s_tk_istream_mem_vtable);
  istream_mem = TK_ISTREAM_MEM(obj);
  return_value_if_fail(istream_mem != NULL, NULL);

  istream_mem->buff = buff;
  istream_mem->size = size;
  istream_mem->cursor = 0;
  istream_mem->own_the_buff = own_the_buff;
  istream_mem->packet_size = packet_size;
  TK_ISTREAM(obj)->read = tk_istream_mem_read;
  TK_ISTREAM(obj)->seek = tk_istream_mem_seek;
  TK_ISTREAM(obj)->wait_for_data = tk_istream_mem_wait_for_data;

  return TK_ISTREAM(obj);
}
