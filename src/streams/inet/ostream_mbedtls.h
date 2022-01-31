/**
 * File:   ostream_mbedtls.h
 * Author: AWTK Develop Team
 * Brief:  output stream base on mbedtls 
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

#ifndef TK_OSTREAM_MBEDTLS_H
#define TK_OSTREAM_MBEDTLS_H

#include "tkc/ostream.h"
#include "streams/inet/mbedtls_helper.h"

BEGIN_C_DECLS

struct _tk_ostream_mbedtls_t;
typedef struct _tk_ostream_mbedtls_t tk_ostream_mbedtls_t;

/**
 * @class tk_ostream_mbedtls_t
 * @parent tk_ostream_t
 *
 * 基于MBEDTLS实现的输出流。
 *
 */
struct _tk_ostream_mbedtls_t {
  tk_ostream_t ostream;

  int sock;
  bool_t is_broken;
  mbedtls_ssl_context* ssl;
};

/**
 * @method tk_ostream_mbedtls_create
 *
 * 创建ostream对象。
 *
 * @param {mbedtls_ssl_context*} ssl ssl对象。
 *
 * @return {tk_ostream_t*} 返回ostream对象。
 *
 */
tk_ostream_t* tk_ostream_mbedtls_create(mbedtls_ssl_context* ssl);

tk_ostream_mbedtls_t* tk_ostream_mbedtls_cast(tk_ostream_t* s);
#define TK_OSTREAM_MBEDTLS(obj) tk_ostream_mbedtls_cast((tk_ostream_t*)(obj))

END_C_DECLS

#endif /*TK_OSTREAM_MBEDTLS_H*/
