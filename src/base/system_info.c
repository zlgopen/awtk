/**
 * File:   system_info.h
 * Author: AWTK Develop Team
 * Brief:  system info
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-04-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/system_info.h"

static system_info_t info;

system_info_t* system_info() {
  return &info;
}

ret_t system_info_init(app_type_t app_type, const char* app_name, const char* app_root) {
  memset(&info, 0x00, sizeof(info));

  info.app_type = app_type;
  info.app_root = app_root ? app_root : "./";
  info.app_name = app_name ? app_name : "AWTK Simulator";

  return RET_OK;
}
