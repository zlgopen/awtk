/**
 * File:   istream_mbedtls.h
 * Author: AWTK Develop Team
 * Brief:  input stream base on mbedtls 
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
#ifndef TK_ISTREAM_MBEDTLS_H
#define TK_ISTREAM_MBEDTLS_H

#include "tkc/istream.h"
#include "streams/inet/mbedtls_helper.h"

BEGIN_C_DECLS

struct _tk_istream_mbedtls_t;
typedef struct _tk_istream_mbedtls_t tk_istream_mbedtls_t;

/**
 * @class tk_istream_mbedtls_t
 * @parent tk_istream_t
 *
 * 基于MBEDTLS实现的输入流。
 *
 */
struct _tk_istream_mbedtls_t {
  tk_istream_t istream;

  int sock;
  bool_t is_broken;
  mbedtls_ssl_context* ssl;
};

/**
 * @method tk_istream_mbedtls_create
 *
 * 创建istream对象。
 *
 * @param {mbedtls_ssl_context*} ssl ssl对象。
 *
 * @return {tk_istream_t*} 返回istream对象。
 *
 */
tk_istream_t* tk_istream_mbedtls_create(mbedtls_ssl_context* ssl);

tk_istream_mbedtls_t* tk_istream_mbedtls_cast(tk_istream_t* s);
#define TK_ISTREAM_MBEDTLS(obj) tk_istream_mbedtls_cast((tk_istream_t*)(obj))

END_C_DECLS

#endif /*TK_ISTREAM_MBEDTLS_H*/
