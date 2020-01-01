/**
 * File:   iostream_mem.c
 * Author: AWTK Develop Team
 * Brief:  iostream base on mem
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
 * 2019-09-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "streams/mem/istream_mem.h"
#include "streams/mem/ostream_mem.h"
#include "streams/mem/iostream_mem.h"

static ret_t tk_iostream_mem_on_destroy(object_t* obj) {
  tk_iostream_mem_t* iostream_mem = TK_IOSTREAM_MEM(obj);

  object_unref(OBJECT(iostream_mem->istream));
  object_unref(OBJECT(iostream_mem->ostream));

  return RET_OK;
}

static const object_vtable_t s_tk_iostream_mem_vtable = {.type = "tk_iostream_mem",
                                                         .desc = "tk_iostream_mem",
                                                         .size = sizeof(tk_iostream_mem_t),
                                                         .on_destroy = tk_iostream_mem_on_destroy};

static tk_istream_t* tk_iostream_mem_get_istream(tk_iostream_t* stream) {
  tk_iostream_mem_t* iostream_mem = TK_IOSTREAM_MEM(stream);

  return iostream_mem->istream;
}

static tk_ostream_t* tk_iostream_mem_get_ostream(tk_iostream_t* stream) {
  tk_iostream_mem_t* iostream_mem = TK_IOSTREAM_MEM(stream);

  return iostream_mem->ostream;
}

tk_iostream_t* tk_iostream_mem_create(void* buff_in, uint32_t size_in, void* buff_out,
                                      uint32_t size_out, bool_t own_buff) {
  object_t* obj = NULL;
  tk_iostream_mem_t* iostream_mem = NULL;
  return_value_if_fail(buff_in != NULL && size_in > 0, NULL);
  return_value_if_fail(buff_out != NULL && size_out > 0, NULL);

  obj = object_create(&s_tk_iostream_mem_vtable);
  iostream_mem = TK_IOSTREAM_MEM(obj);
  if (iostream_mem == NULL) {
    return_value_if_fail(iostream_mem != NULL, NULL);
  }

  iostream_mem->istream = tk_istream_mem_create(buff_in, size_in, 0, own_buff);
  iostream_mem->ostream = tk_ostream_mem_create(buff_out, size_out, 0, own_buff);

  TK_IOSTREAM(obj)->get_istream = tk_iostream_mem_get_istream;
  TK_IOSTREAM(obj)->get_ostream = tk_iostream_mem_get_ostream;

  return TK_IOSTREAM(obj);
}
