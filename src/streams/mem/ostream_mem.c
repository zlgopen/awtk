/**
 * File:   ostream_mem.c
 * Author: AWTK Develop Team
 * Brief:  output stream base on memory
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
#include "streams/mem/ostream_mem.h"

static int32_t tk_ostream_mem_write(tk_ostream_t* stream, const uint8_t* buff, uint32_t max_size) {
  int32_t size = max_size;
  tk_ostream_mem_t* ostream_mem = TK_OSTREAM_MEM(stream);

  if (ostream_mem->packet_size && ostream_mem->packet_size < max_size) {
    size = ostream_mem->packet_size;
  }

  if (size > (ostream_mem->size - ostream_mem->cursor)) {
    size = ostream_mem->size - ostream_mem->cursor;
  }

  if (size > 0) {
    memcpy(ostream_mem->buff + ostream_mem->cursor, buff, size);
    ostream_mem->cursor += size;
  }

  return size;
}

static ret_t tk_ostream_mem_seek(tk_ostream_t* stream, uint32_t offset) {
  tk_ostream_mem_t* ostream_mem = TK_OSTREAM_MEM(stream);
  return_value_if_fail(offset <= ostream_mem->size, RET_BAD_PARAMS);

  ostream_mem->cursor = offset;

  return RET_OK;
}

static ret_t tk_ostream_mem_set_prop(object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t tk_ostream_mem_get_prop(object_t* obj, const char* name, value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t tk_ostream_mem_on_destroy(object_t* obj) {
  tk_ostream_mem_t* ostream_mem = TK_OSTREAM_MEM(obj);

  if (ostream_mem->own_the_buff) {
    TKMEM_FREE(ostream_mem->buff);
  }

  return RET_OK;
}

static const object_vtable_t s_tk_ostream_mem_vtable = {.type = "tk_ostream_mem",
                                                        .desc = "tk_ostream_mem",
                                                        .size = sizeof(tk_ostream_mem_t),
                                                        .on_destroy = tk_ostream_mem_on_destroy,
                                                        .get_prop = tk_ostream_mem_get_prop,
                                                        .set_prop = tk_ostream_mem_set_prop};

tk_ostream_t* tk_ostream_mem_create(uint8_t* buff, uint32_t size, uint32_t packet_size,
                                    bool_t own_the_buff) {
  object_t* obj = NULL;
  tk_ostream_mem_t* ostream_mem = NULL;
  return_value_if_fail(buff != NULL && packet_size < size, NULL);

  obj = object_create(&s_tk_ostream_mem_vtable);
  ostream_mem = TK_OSTREAM_MEM(obj);
  return_value_if_fail(ostream_mem != NULL, NULL);

  ostream_mem->buff = buff;
  ostream_mem->size = size;
  ostream_mem->cursor = 0;
  ostream_mem->own_the_buff = own_the_buff;
  ostream_mem->packet_size = packet_size;
  TK_OSTREAM(obj)->write = tk_ostream_mem_write;
  TK_OSTREAM(obj)->seek = tk_ostream_mem_seek;

  return TK_OSTREAM(obj);
}
