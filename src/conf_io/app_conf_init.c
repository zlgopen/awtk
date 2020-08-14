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
#include "tkc/mem.h"
#include "tkc/path.h"
#include "tkc/utils.h"
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
    data_reader_t* reader = data_reader_factory_create_reader(data_reader_factory(), default_url);
    if (reader != NULL) {
      uint32_t size = data_reader_get_size(reader);
      if (size > 0) {
        data_writer_t* writer = data_writer_factory_create_writer(data_writer_factory(), url);
        if (writer != NULL) {
          void* buff = TKMEM_CALLOC(1, size + 1);
          if (buff != NULL) {
            int32_t rsize = data_reader_read(reader, 0, buff, size);
            assert(rsize == size);
            rsize = data_writer_write(writer, 0, buff, rsize);
            assert(rsize == size);
            TKMEM_FREE(buff);
            log_debug("load default conf %s ok\n", default_url);
          } else {
            ret = RET_FAIL;
          }
          data_writer_destroy(writer);
        } else {
          ret = RET_FAIL;
        }
      } else {
        log_debug("no default conf %s \n", default_url);
      }
      data_reader_destroy(reader);
    }
  }

  return ret;
}

ret_t app_conf_init(conf_load_t load, const char* app_name, const char* extname) {
  object_t* obj = NULL;
#ifdef APP_CONF_URL
  char path[MAX_PATH + 1];
  const char* app_conf_name = APP_CONF_URL;
#else
  char path[MAX_PATH + 1];
  char app_dir[MAX_PATH + 1];
  char app_conf_name[MAX_PATH + 1];

  fs_t* fs = os_fs();

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

  tk_snprintf(path, MAX_PATH, "asset://%s.%s", app_name, extname);
  app_conf_prepare_default(app_conf_name, path);

  obj = load(app_conf_name, TRUE);
  return_value_if_fail(obj != NULL, RET_FAIL);

  app_conf_set_instance(obj);
  OBJECT_UNREF(obj);

  return RET_OK;
}
