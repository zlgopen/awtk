/**
 * File:   app_conf_init_json.c
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

#include "conf_io/conf_json.h"
#include "conf_io/app_conf_init_json.h"

ret_t app_conf_init_json(const char* app_name) {
  return app_conf_init(conf_json_load, app_name, "json");
}
