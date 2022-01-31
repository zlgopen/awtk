/**
 * File:   iostream_mbedtls.c
 * Author: AWTK Develop Team
 * Brief:  iostream base on mbedtls 
 *
 * Copyright (c) 2021 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2021-03-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifdef WITH_MBEDTLS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif /*WIN32_LEAN_AND_MEAN*/

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/socket_helper.h"
#include "streams/inet/istream_mbedtls.h"
#include "streams/inet/ostream_mbedtls.h"
#include "streams/inet/iostream_mbedtls.h"

static ret_t tk_iostream_mbedtls_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  tk_iostream_mbedtls_t* iostream_mbedtls = TK_IOSTREAM_MBEDTLS(obj);

  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, iostream_mbedtls->conn->sock);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok1 =
        tk_object_get_prop_bool(TK_OBJECT(iostream_mbedtls->istream), TK_STREAM_PROP_IS_OK, TRUE);
    bool_t is_ok2 =
        tk_object_get_prop_bool(TK_OBJECT(iostream_mbedtls->ostream), TK_STREAM_PROP_IS_OK, TRUE);

    value_set_bool(v, is_ok1 && is_ok2);

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tk_iostream_mbedtls_on_destroy(tk_object_t* obj) {
  tk_iostream_mbedtls_t* iostream_mbedtls = TK_IOSTREAM_MBEDTLS(obj);

  if (iostream_mbedtls->conn != NULL) {
    mbedtls_conn_destroy(iostream_mbedtls->conn);
  }

  TK_OBJECT_UNREF(iostream_mbedtls->istream);
  TK_OBJECT_UNREF(iostream_mbedtls->ostream);

  return RET_OK;
}

static const object_vtable_t s_tk_iostream_mbedtls_vtable = {
    .type = "tk_iostream_mbedtls",
    .desc = "tk_iostream_mbedtls",
    .size = sizeof(tk_iostream_mbedtls_t),
    .get_prop = tk_iostream_mbedtls_get_prop,
    .on_destroy = tk_iostream_mbedtls_on_destroy};

static tk_istream_t* tk_iostream_mbedtls_get_istream(tk_iostream_t* stream) {
  tk_iostream_mbedtls_t* iostream_mbedtls = TK_IOSTREAM_MBEDTLS(stream);

  return iostream_mbedtls->istream;
}

static tk_ostream_t* tk_iostream_mbedtls_get_ostream(tk_iostream_t* stream) {
  tk_iostream_mbedtls_t* iostream_mbedtls = TK_IOSTREAM_MBEDTLS(stream);

  return iostream_mbedtls->ostream;
}

tk_iostream_t* tk_iostream_mbedtls_create(mbedtls_conn_t* conn) {
  tk_object_t* obj = NULL;
  tk_iostream_mbedtls_t* iostream_mbedtls = NULL;
  return_value_if_fail(conn != NULL, NULL);
  obj = tk_object_create(&s_tk_iostream_mbedtls_vtable);
  iostream_mbedtls = TK_IOSTREAM_MBEDTLS(obj);
  return_value_if_fail(iostream_mbedtls != NULL, NULL);

  iostream_mbedtls->conn = conn;
  iostream_mbedtls->istream = tk_istream_mbedtls_create(&(conn->ssl));
  iostream_mbedtls->ostream = tk_ostream_mbedtls_create(&(conn->ssl));
  TK_IOSTREAM(obj)->get_istream = tk_iostream_mbedtls_get_istream;
  TK_IOSTREAM(obj)->get_ostream = tk_iostream_mbedtls_get_ostream;

  return TK_IOSTREAM(obj);
}
#endif /*WITH_MBEDTLS*/
