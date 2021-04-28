/**
 * File:   app_conf_init.h
 * Author: AWTK Develop Team
 * Brief:  init config for app
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
 * 2020-06-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "conf_io/conf_obj.h"
#include "conf_io/app_conf_init.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"

static bool_t app_conf_file_exist(const char* url) {
  bool_t exist = FALSE;
  data_reader_t* reader = data_reader_factory_create_reader(data_reader_factory(), url);

  if (reader != NULL) {
    if (data_reader_get_size(reader) > 0) {
      exist = TRUE;
    }
    data_reader_destroy(reader);
  }

  return exist;
}

static ret_t app_conf_prepare_default(const char* url, const char* default_url) {
  ret_t ret = RET_OK;
  if (!app_conf_file_exist(url)) {
    ret = data_url_copy(url, default_url);
  }

  return ret;
}

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
  object_t* obj = NULL;
  char path[MAX_PATH + 1];
#ifdef APP_CONF_URL
  const char* app_conf_name = APP_CONF_URL;
#else
  char app_conf_name[MAX_PATH + 1];
  return_value_if_fail(app_conf_get_url(app_conf_name, app_name, extname) == RET_OK, RET_FAIL);
#endif /*APP_CONF_URL*/

  log_info("app conf: %s\n", app_conf_name);
  tk_snprintf(path, MAX_PATH, "asset://data/%s.%s", app_name, extname);
  app_conf_prepare_default(app_conf_name, path);

  obj = load(app_conf_name, TRUE);
  return_value_if_fail(obj != NULL, RET_FAIL);
  app_conf_set_instance(obj);
  app_conf_set_str(CONF_OBJ_PROP_DEFAULT_URL, path);

  OBJECT_UNREF(obj);

  return RET_OK;
}

ret_t app_conf_reset(void) {
  const char* url = app_conf_get_str(CONF_OBJ_PROP_URL, NULL);
  const char* default_url = app_conf_get_str(CONF_OBJ_PROP_DEFAULT_URL, NULL);

  return_value_if_fail(url != NULL && default_url != NULL, RET_BAD_PARAMS);
  return_value_if_fail(data_url_copy(url, default_url) == RET_OK, RET_BAD_PARAMS);
  return_value_if_fail(app_conf_reload() == RET_OK, RET_BAD_PARAMS);

  return RET_OK;
}
