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

#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "conf_io/app_conf_init.h"

ret_t app_conf_init(conf_load_t load, const char* app_name, const char* extname) {
#ifdef APP_CONF_URL
  const char* app_conf_name = APP_CONF_URL;
#else
  char path[MAX_PATH + 1];
  char app_dir[MAX_PATH + 1];
  char app_conf_name[MAX_PATH + 1];

  fs_t* fs = os_fs();
  object_t* obj = NULL;

  return_value_if_fail(app_name != NULL && load != NULL, RET_BAD_PARAMS);
  return_value_if_fail(fs != NULL, RET_NOT_FOUND);
  return_value_if_fail(fs_get_user_storage_path(fs, path) == RET_OK, RET_BAD_PARAMS);

  if (!path_exist(path)) {
    return_value_if_fail(fs_create_dir(fs, path) == RET_OK, RET_FAIL);
  }

  path_build(app_dir, MAX_PATH, path, app_name, NULL);
  if (!path_exist(app_dir)) {
    return_value_if_fail(fs_create_dir(fs, app_dir) == RET_OK, RET_FAIL);
  }

  tk_snprintf(app_conf_name, sizeof(app_conf_name) - 1, "app_conf.%s", extname);
  path_build(path, MAX_PATH, app_dir, app_conf_name, NULL);

  tk_snprintf(app_conf_name, MAX_PATH, "file://%s", path);
#endif /*APP_CONF_URL*/
  log_info("app conf: %s\n", app_conf_name);

  obj = load(app_conf_name, TRUE);
  return_value_if_fail(obj != NULL, RET_FAIL);

  app_conf_set_instance(obj);
  OBJECT_UNREF(obj);

  return RET_OK;
}
