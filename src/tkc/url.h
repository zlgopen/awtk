/**
 * File:   url.h
 * Author: AWTK Develop Team
 * Brief:  URL parser 
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-12-25 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_URL_H
#define TK_URL_H

#include "tkc/object.h"

BEGIN_C_DECLS

/**
 * @class url_t
 * URL
 */
typedef struct _url_t {
  /**
   * @property {char*} schema
   * @annotation ["readable"]
   * schema。
   */
  char* schema;

  /**
   * @property {char*} user_name
   * @annotation ["readable"]
   * 用户名。
   */
  char* user_name;

  /**
   * @property {char*} password
   * @annotation ["readable"]
   * 密码。
   */
  char* password;

  /**
   * @property {char*} host
   * @annotation ["readable"]
   * 主机名。
   */
  char* host;

  /**
   * @property {char*} path
   * @annotation ["readable"]
   * 路径。
   */
  char* path;

  /**
   * @property {int32_t} port
   * @annotation ["readable"]
   * 端口。
   */
  int32_t port;

  /**
   * @property {object_t*} params
   * @annotation ["readable"]
   * 参数集合。
   */
  object_t* params;
} url_t;

/**
 * @method url_create
 * 创建URL对象。 
 *
 * @param {const char*}  surl 字符串格式的URL。
 *
 * @return {url_t*} 返回URL对象。 
 */
url_t* url_create(const char* surl);

/**
 * @method url_set_schema
 * 设置schema。
 *
 * @param {url_t*} url url对象。
 * @param {const char*} schema schema。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t url_set_schema(url_t* url, const char* schema);

/**
 * @method url_set_user_name
 * 设置用户名。
 *
 * @param {url_t*} url url对象。
 * @param {const char*} user_name 用户名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t url_set_user_name(url_t* url, const char* user_name);

/**
 * @method url_set_password
 * 设置密码。
 *
 * @param {url_t*} url url对象。
 * @param {const char*} password 密码。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t url_set_password(url_t* url, const char* password);

/**
 * @method url_set_host
 * 设置主机名。
 *
 * @param {url_t*} url url对象。
 * @param {const char*} host 主机名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t url_set_host(url_t* url, const char* host);

/**
 * @method url_set_port
 * 设置端口。
 *
 * @param {url_t*} url url对象。
 * @param {int32_t} port 端口。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t url_set_port(url_t* url, int32_t port);

/**
 * @method url_set_path
 * 设置路径。
 *
 * @param {url_t*} url url对象。
 * @param {const char*} path 路径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t url_set_path(url_t* url, const char* path);

/**
 * @method url_set_param
 * 设置参数。
 *
 * @param {url_t*} url url对象。
 * @param {const char*} name 参数名。
 * @param {const char*} value 参数值。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t url_set_param(url_t* url, const char* name, const char* value);

/**
 * @method url_get_param
 * 获取参数。
 *
 * @param {url_t*} url url对象。
 * @param {const char*} name 参数名。
 *
 * @return {const char*} 返回指定参数名的参数值。
 */
const char* url_get_param(url_t* url, const char* name);

/**
 * @method url_destroy
 * 销毁url对象。
 * @param {url_t*} url url对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t url_destroy(url_t* url);

END_C_DECLS

#endif /*TK_URL_H*/
