/**
 * File:   iostream_mbedtls.h
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

#ifndef TK_IOSTREAM_MBEDTLS_H
#define TK_IOSTREAM_MBEDTLS_H

#include "tkc/iostream.h"
#include "streams/inet/mbedtls_helper.h"

BEGIN_C_DECLS

struct _tk_iostream_mbedtls_t;
typedef struct _tk_iostream_mbedtls_t tk_iostream_mbedtls_t;

/**
 * @class tk_iostream_mbedtls_t
 * @parent tk_iostream_t
 *
 * 基于MBEDTLS实现的输入输出流。
 *
 */
struct _tk_iostream_mbedtls_t {
  tk_iostream_t iostream;

  mbedtls_conn_t* conn;
  tk_istream_t* istream;
  tk_ostream_t* ostream;
};

/**
 * @method tk_iostream_mbedtls_create
 *
 * 创建iostream对象。
 *
 * @param {mbedtls_conn_t*} conn ssl连接。
 *
 * @return {tk_iostream_t*} 返回iostream对象。
 *
 */
tk_iostream_t* tk_iostream_mbedtls_create(mbedtls_conn_t* conn);

#define TK_IOSTREAM_MBEDTLS(obj) ((tk_iostream_mbedtls_t*)(obj))

END_C_DECLS

#endif /*TK_IOSTREAM_MBEDTLS_H*/
