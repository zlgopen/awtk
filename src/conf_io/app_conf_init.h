/**
 * File:   app_conf_init.h
 * Author: AWTK Develop Team
 * Brief:  init config for app
 *
 * Copyright (c) 2020 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-06-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_APP_CONF_INIT_H
#define TK_APP_CONF_INIT_H

#include "tkc/object.h"
#include "conf_io/app_conf.h"

BEGIN_C_DECLS

typedef object_t* (*conf_load_t)(const char* url, bool_t create_if_not_exist);

/**
 * @method app_conf_init
 *
 * 初始化。
 *
 * @annotation ["global"]
 * 
 * @param {conf_loader_t} load 配置加载函数。
 * @param {const char*} app_name 应用程序名称。
 * @param {const char*} extname 文件扩展名。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_init(conf_load_t load, const char* app_name, const char* extname);

END_C_DECLS

#endif /*TK_APP_CONF_INIT_H*/
