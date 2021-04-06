/**
 * File:   ostream_mbedtls.c
 * Author: AWTK Develop Team
 * Brief:  output stream base on mbedtls 
 *
 * Copyright (c) 2021 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "streams/inet/ostream_mbedtls.h"

static int32_t tk_ostream_mbedtls_write(tk_ostream_t* stream, const uint8_t* buff,
                                        uint32_t max_size) {
  int32_t ret = 0;
  tk_ostream_mbedtls_t* ostream_mbedtls = TK_OSTREAM_MBEDTLS(stream);

  ret = mbedtls_ssl_write(ostream_mbedtls->ssl, buff, max_size);
  if (ret <= 0) {
    if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) {
      perror("send");
      ostream_mbedtls->is_broken = TRUE;
    }
  }

  return ret;
}

static ret_t tk_ostream_mbedtls_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_ostream_mbedtls_t* ostream_mbedtls = TK_OSTREAM_MBEDTLS(obj);
  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, ostream_mbedtls->sock);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok = ostream_mbedtls->sock >= 0 && ostream_mbedtls->is_broken == FALSE;
    value_set_bool(v, is_ok);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static const object_vtable_t s_tk_ostream_mbedtls_vtable = {
    .type = "tk_ostream_mbedtls",
    .desc = "tk_ostream_mbedtls",
    .size = sizeof(tk_ostream_mbedtls_t),
    .get_prop = tk_ostream_mbedtls_get_prop};

tk_ostream_t* tk_ostream_mbedtls_create(mbedtls_ssl_context* ssl) {
  object_t* obj = NULL;
  mbedtls_net_context* bio = NULL;
  tk_ostream_mbedtls_t* ostream_mbedtls = NULL;
  return_value_if_fail(ssl != NULL, NULL);
  bio = (mbedtls_net_context*)(ssl->p_bio);

  obj = object_create(&s_tk_ostream_mbedtls_vtable);
  ostream_mbedtls = TK_OSTREAM_MBEDTLS(obj);
  return_value_if_fail(ostream_mbedtls != NULL, NULL);

  ostream_mbedtls->ssl = ssl;
  ostream_mbedtls->sock = bio->fd;
  TK_OSTREAM(obj)->write = tk_ostream_mbedtls_write;

  return TK_OSTREAM(obj);
}

tk_ostream_mbedtls_t* tk_ostream_mbedtls_cast(tk_ostream_t* s) {
  return_value_if_fail(s != NULL && OBJECT(s)->vt == &s_tk_ostream_mbedtls_vtable, NULL);

  return (tk_ostream_mbedtls_t*)s;
}

#endif /*WITH_MBEDTLS*/
