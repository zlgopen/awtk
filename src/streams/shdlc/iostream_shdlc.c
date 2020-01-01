/**
 * File:   iostream_shdlc.c
 * Author: AWTK Develop Team
 * Brief:  input stream base on shdlc
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
#include "streams/shdlc/istream_shdlc.h"
#include "streams/shdlc/ostream_shdlc.h"
#include "streams/shdlc/iostream_shdlc.h"
#include "streams/buffered/istream_buffered.h"

static ret_t tk_iostream_shdlc_on_destroy(object_t* obj) {
  tk_iostream_shdlc_t* iostream_shdlc = TK_IOSTREAM_SHDLC(obj);

  OBJECT_UNREF(iostream_shdlc->istream);
  OBJECT_UNREF(iostream_shdlc->ostream);
  OBJECT_UNREF(iostream_shdlc->real_istream);
  OBJECT_UNREF(iostream_shdlc->real_iostream);

  return RET_OK;
}

static const object_vtable_t s_tk_iostream_shdlc_vtable = {
    .type = "tk_iostream_shdlc",
    .desc = "tk_iostream_shdlc",
    .size = sizeof(tk_iostream_shdlc_t),
    .on_destroy = tk_iostream_shdlc_on_destroy};

static tk_istream_t* tk_iostream_shdlc_get_istream(tk_iostream_t* stream) {
  tk_iostream_shdlc_t* iostream_shdlc = TK_IOSTREAM_SHDLC(stream);

  return iostream_shdlc->istream;
}

static tk_ostream_t* tk_iostream_shdlc_get_ostream(tk_iostream_t* stream) {
  tk_iostream_shdlc_t* iostream_shdlc = TK_IOSTREAM_SHDLC(stream);

  return iostream_shdlc->ostream;
}

tk_iostream_t* tk_iostream_shdlc_create(tk_iostream_t* real_iostream) {
  object_t* obj = NULL;
  tk_iostream_shdlc_t* iostream_shdlc = NULL;
  return_value_if_fail(real_iostream != NULL, NULL);

  obj = object_create(&s_tk_iostream_shdlc_vtable);
  iostream_shdlc = TK_IOSTREAM_SHDLC(obj);
  if (iostream_shdlc == NULL) {
    return_value_if_fail(iostream_shdlc != NULL, NULL);
  }

  iostream_shdlc->real_iostream = TK_IOSTREAM(object_ref(OBJECT(real_iostream)));
  do {
    iostream_shdlc->istream = tk_istream_shdlc_create(iostream_shdlc);
    break_if_fail(iostream_shdlc->istream != NULL);

    iostream_shdlc->ostream = tk_ostream_shdlc_create(iostream_shdlc);
    break_if_fail(iostream_shdlc->ostream != NULL);

    iostream_shdlc->real_istream =
        tk_istream_buffered_create(tk_iostream_get_istream(real_iostream), 4096);
    break_if_fail(iostream_shdlc->real_istream != NULL);
    iostream_shdlc->real_ostream = tk_iostream_get_ostream(real_iostream);

    TK_IOSTREAM(obj)->get_istream = tk_iostream_shdlc_get_istream;
    TK_IOSTREAM(obj)->get_ostream = tk_iostream_shdlc_get_ostream;

    return TK_IOSTREAM(obj);
  } while (1);

  OBJECT_UNREF(obj);

  return NULL;
}
