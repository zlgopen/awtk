/**
 * File:   foo_client.h
 * Author: AWTK Develop Team
 * Brief:  foo client
 *
 * Copyright (c) 2025 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-04-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_FOO_CLIENT_H
#define TK_FOO_CLIENT_H

#include "tkc/buffer.h"
#include "tkc/darray.h"
#include "tkc/iostream.h"
#include "ubjson/ubjson_writer.h"
#include "foo_comm_types_def.h"
#include "service/client.h"

struct _foo_client_t;
typedef struct _foo_client_t foo_client_t;

BEGIN_C_DECLS

/**
 * @class foo_client_t
 * foo客户端。 
*/
typedef struct _foo_client_t {
  tk_client_t client;
} foo_client_t;

/**
 * @method foo_client_create
 * 创建foo客户端。
 *
 * @param {tk_iostream_t*} io IO流。
 *
 * @return {foo_client_t*} 返回foo客户端对象。
 */
foo_client_t* foo_client_create(tk_iostream_t* io);

/**
 * @method foo_client_login
 * 登录。
 *
 * @param {foo_client_t*} foo foo客户端对象。
 * @param {const char*} username 用户名。
 * @param {const char*} password 密码。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t foo_client_login(foo_client_t* foo, const char* username, const char* password);

/**
 * @method foo_client_logout
 * 登出。
 *
 * @param {foo_client_t*} foo foo客户端对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t foo_client_logout(foo_client_t* foo);

/**
 * @method foo_client_set_int
 * 设置整数值。
 * @param {foo_client_t*} foo foo客户端对象。
 * @param {int} value 整数值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t foo_client_set_int(foo_client_t* foo, int value);

/**
 * @method foo_client_set_string
 * 设置字符串值。
 * @param {foo_client_t*} foo foo客户端对象。
 * @param {const char*} value 字符串值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t foo_client_set_string(foo_client_t* foo, const char* value);

/**
 * @method foo_client_get_int
 * 获取整数值。
 * @param {foo_client_t*} foo foo客户端对象。
 * @param {int*} value 返回整数值。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t foo_client_get_int(foo_client_t* foo, int* value);

/**
 * @method foo_client_get_string
 * 获取字符串值。
 * @param {foo_client_t*} foo foo客户端对象。
 * @param {str_t*} str 返回字符串值(调用者初始化和释放)。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t foo_client_get_string(foo_client_t* foo, str_t* str);

/**
 * @method foo_client_get_version
 * 获取服务器版本。
 * @param {foo_client_t*} foo foo客户端对象。
 * @param {uint32_t*} version 返回服务器版本。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t foo_client_get_version(foo_client_t* foo, uint32_t* version);

/**
 * @method foo_client_get_retry_times
 * 获取重试次数。
 * @param {foo_client_t*} foo foo客户端对象。
 * @param {uint32_t*} retry_times 返回重试次数。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t foo_client_get_retry_times(foo_client_t* foo, uint32_t* retry_times);

/**
 * @method foo_client_quit
 * 退出。
 * @param {foo_client_t*} foo foo客户端对象。
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t foo_client_quit(foo_client_t* foo);

/**
 * @method foo_client_destroy
 * 销毁foo客户端。
 *
 * @param {foo_client_t*} foo foo客户端对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t foo_client_destroy(foo_client_t* foo);

END_C_DECLS

#endif /*TK_FOO_CLIENT_H*/
