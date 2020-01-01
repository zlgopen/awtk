/**
 * File:   iostream_noisy.c
 * Author: AWTK Develop Team
 * Brief:  inject error into stream
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
 * 2019-10-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "streams/noisy/iostream_noisy.h"

static ret_t tk_iostream_noisy_on_destroy(object_t* obj) {
  tk_iostream_noisy_t* iostream_noisy = TK_IOSTREAM_NOISY(obj);

  OBJECT_UNREF(iostream_noisy->istream);
  OBJECT_UNREF(iostream_noisy->real_iostream);

  return RET_OK;
}

static const object_vtable_t s_tk_iostream_noisy_vtable = {
    .type = "tk_iostream_noisy",
    .desc = "tk_iostream_noisy",
    .size = sizeof(tk_iostream_noisy_t),
    .on_destroy = tk_iostream_noisy_on_destroy};

static tk_istream_t* tk_iostream_noisy_get_istream(tk_iostream_t* stream) {
  tk_iostream_noisy_t* iostream_noisy = TK_IOSTREAM_NOISY(stream);

  return iostream_noisy->istream;
}

static tk_ostream_t* tk_iostream_noisy_get_ostream(tk_iostream_t* stream) {
  tk_iostream_noisy_t* iostream_noisy = TK_IOSTREAM_NOISY(stream);

  return iostream_noisy->ostream;
}

tk_iostream_t* tk_iostream_noisy_create(tk_iostream_t* real_iostream) {
  object_t* obj = NULL;
  tk_ostream_t* real_ostream = NULL;
  tk_iostream_noisy_t* iostream_noisy = NULL;
  return_value_if_fail(real_iostream != NULL, NULL);
  real_ostream = tk_iostream_get_ostream(real_iostream);
  return_value_if_fail(real_ostream != NULL, NULL);

  obj = object_create(&s_tk_iostream_noisy_vtable);
  iostream_noisy = TK_IOSTREAM_NOISY(obj);
  return_value_if_fail(iostream_noisy != NULL, NULL);

  iostream_noisy->real_iostream = TK_IOSTREAM(object_ref(OBJECT(real_iostream)));

  iostream_noisy->istream = tk_iostream_get_istream(real_iostream);
  iostream_noisy->ostream = tk_ostream_noisy_create(real_ostream);

  TK_IOSTREAM(obj)->get_istream = tk_iostream_noisy_get_istream;
  TK_IOSTREAM(obj)->get_ostream = tk_iostream_noisy_get_ostream;

  return TK_IOSTREAM(obj);
}
