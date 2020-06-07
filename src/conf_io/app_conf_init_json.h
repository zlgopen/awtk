/**
 * File:   app_conf_init_json.h
 * Author: AWTK Develop Team
 * Brief:  ini config for app
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
 * 2020-06-07 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_APP_CONF_INIT_JSON_H
#define TK_APP_CONF_INIT_JSON_H

#include "conf_io/app_conf_init.h"

BEGIN_C_DECLS

/**
 * @method app_conf_init_json
 *
 * 初始化应用程序的配置信息。
 *
 * @annotation ["global"]
 * 
 * @param {const char*} app_name 应用程序名称。
 * 
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t app_conf_init_json(const char* app_name);

END_C_DECLS

#endif /*TK_APP_CONF_INIT_JSON_H*/
