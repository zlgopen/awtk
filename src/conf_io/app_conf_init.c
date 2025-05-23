﻿/**
 * File:   app_conf_init.h
 * Author: AWTK Develop Team
 * Brief:  init config for app
 *
 * Copyright (c) 2020 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/mem.h"
#include "tkc/path.h"
#include "tkc/utils.h"

#include "conf_io/conf_obj.h"
#include "conf_io/object_app_conf.inc"
#include "conf_io/app_conf_init.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"

static ret_t app_conf_get_url(char url[MAX_PATH + 1], const char* app_name, const char* extname) {
  char path[MAX_PATH + 1];
  char app_dir[MAX_PATH + 1];
  fs_t* fs = os_fs();

  return_value_if_fail(fs != NULL, RET_NOT_FOUND);
  return_value_if_fail(app_name != NULL && extname != NULL, RET_BAD_PARAMS);
  return_value_if_fail(fs_get_user_storage_path(fs, path) == RET_OK, RET_BAD_PARAMS);

  if (!path_exist(path)) {
    return_value_if_fail(fs_create_dir(fs, path) == RET_OK, RET_FAIL);
  }

  path_build(app_dir, MAX_PATH, path, app_name, NULL);
  if (!path_exist(app_dir)) {
    return_value_if_fail(fs_create_dir(fs, app_dir) == RET_OK, RET_FAIL);
  }

  tk_snprintf(url, MAX_PATH, "app_conf.%s", extname);
  path_build(path, MAX_PATH, app_dir, url, NULL);
  tk_snprintf(url, MAX_PATH, "file://%s", path);

  return RET_OK;
}

ret_t app_conf_init(conf_load_t load, const char* app_name, const char* extname) {
  ret_t ret = RET_FAIL;
  tk_object_t* obj = NULL;
  tk_object_t* default_obj = NULL;
  tk_object_t* user_obj = NULL;
  char path[MAX_PATH + 1];
#ifdef APP_CONF_URL
  const char* app_conf_name = APP_CONF_URL;
#else
  char app_conf_name[MAX_PATH + 1];
  return_value_if_fail(app_conf_get_url(app_conf_name, app_name, extname) == RET_OK, RET_FAIL);
#endif /*APP_CONF_URL*/

  log_info("app conf: %s\n", app_conf_name);
  tk_snprintf(path, MAX_PATH, "asset://data/%s.%s", app_name, extname);

  log_info("load default conf: %s\n", path);
  default_obj = load(path, TRUE);
  goto_error_if_fail(default_obj != NULL);

  log_info("load user conf: %s\n", app_conf_name);
  user_obj = load(app_conf_name, TRUE);
  goto_error_if_fail(user_obj != NULL);

  obj = object_app_conf_create(user_obj, default_obj);
  goto_error_if_fail(obj != NULL);

  app_conf_set_instance(obj);
  app_conf_set_str(CONF_OBJ_PROP_DEFAULT_URL, path);
  assert(obj->ref_count > 0);
  assert(user_obj->ref_count > 0);
  assert(default_obj->ref_count > 0);
  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(user_obj);
  TK_OBJECT_UNREF(default_obj);

  return RET_OK;
error:
  TK_OBJECT_UNREF(obj);
  TK_OBJECT_UNREF(user_obj);
  TK_OBJECT_UNREF(default_obj);

  return ret;
}

ret_t app_conf_reset(void) {
  const char* url = app_conf_get_str(CONF_OBJ_PROP_URL, NULL);

  return_value_if_fail(url != NULL, RET_BAD_PARAMS);
  data_writer_write_all(url, "\0", 1);
  return_value_if_fail(app_conf_reload() == RET_OK, RET_BAD_PARAMS);

  return RET_OK;
}
