/**
 * File:   mutex.c
 * Author: AWTK Develop Team
 * Brief:  mutex
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
 * 2021-12-16 Wang LinFu <wanglinfu@zlg.cn> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/str_str.h"

#define FS_REG_MAX 6

static str_str_t s_items[FS_REG_MAX + 1];

static const char* str_str_value_with_len(const str_str_t* items, const char* name) {
  const str_str_t* iter = items;
  uint32_t len;
  return_value_if_fail(items != NULL && name != NULL, NULL);

  while (iter->name != NULL) {
    len = tk_strlen(iter->name);
    if (tk_str_eq_with_len(iter->name, name, len)) {
      if (name[len] == '/' || name[len] == '\0') {
        return iter->value;
      }
    }

    iter++;
  }

  return NULL;
}

/*----------------------------------------------------------------------------*/

static fs_file_t* fs_os_open_file(fs_t* fs, const char* name, const char* mode) {
  (void)fs;
  fs_t* rfs = NULL;
  rfs = (fs_t*)str_str_value_with_len(s_items, name);
  return_value_if_fail(rfs, NULL);
  return rfs->open_file(rfs, name, mode);
}

static ret_t fs_os_remove_file(fs_t* fs, const char* name) {
  (void)fs;
  fs_t* rfs = NULL;
  rfs = (fs_t*)str_str_value_with_len(s_items, name);
  return_value_if_fail(rfs, RET_BAD_PARAMS);
  return rfs->remove_file(rfs, name);
}

static bool_t fs_os_file_exist(fs_t* fs, const char* name) {
  (void)fs;
  fs_t* rfs = NULL;
  rfs = (fs_t*)str_str_value_with_len(s_items, name);
  return_value_if_fail(rfs, FALSE);
  return rfs->file_exist(rfs, name);
}

static ret_t fs_os_file_rename(fs_t* fs, const char* name, const char* new_name) {
  (void)fs;
  fs_t* rfs = NULL;

  rfs = (fs_t*)str_str_value_with_len(s_items, name);
  return_value_if_fail(rfs, RET_BAD_PARAMS);
  if ((fs_t*)str_str_value_with_len(s_items, new_name) != rfs) {
    return_value_if_fail(rfs, RET_BAD_PARAMS);
  }
  return rfs->file_rename(rfs, name, new_name);
}

static ret_t fs_os_stat(fs_t* fs, const char* name, fs_stat_info_t* fst) {
  fs_t* rfs = NULL;

  rfs = (fs_t*)str_str_value_with_len(s_items, name);
  return_value_if_fail(rfs, RET_BAD_PARAMS);
  return rfs->stat(rfs, name, fst);
}

/*----------------------------------------------------------------------------*/
static ret_t fs_os_create_dir(fs_t* fs, const char* name) {
  (void)fs;
  fs_t* rfs = NULL;
  rfs = (fs_t*)str_str_value_with_len(s_items, name);
  return_value_if_fail(rfs, RET_BAD_PARAMS);
  return rfs->create_dir(rfs, name);
}

static fs_dir_t* fs_os_open_dir(fs_t* fs, const char* name) {
  (void)fs;
  fs_t* rfs = NULL;
  rfs = (fs_t*)str_str_value_with_len(s_items, name);
  return_value_if_fail(rfs, NULL);
  return rfs->open_dir(rfs, name);
}

static ret_t fs_os_remove_dir(fs_t* fs, const char* name) {
  (void)fs;
  fs_t* rfs = NULL;
  rfs = (fs_t*)str_str_value_with_len(s_items, name);
  return_value_if_fail(rfs, RET_BAD_PARAMS);
  return rfs->remove_dir(rfs, name);
}

static bool_t fs_os_dir_exist(fs_t* fs, const char* name) {
  (void)fs;
  fs_t* rfs = NULL;
  rfs = (fs_t*)str_str_value_with_len(s_items, name);
  return_value_if_fail(rfs, FALSE);
  if (strrchr(name, '/') == name) {
    return TRUE;
  }
  return rfs->dir_exist(rfs, name);
}

static ret_t fs_os_dir_rename(fs_t* fs, const char* name, const char* new_name) {
  (void)fs;
  fs_t* rfs = NULL;
  rfs = (fs_t*)str_str_value_with_len(s_items, name);
  return_value_if_fail(rfs, FALSE);
  if ((fs_t*)str_str_value_with_len(s_items, new_name) != rfs) {
    return_value_if_fail(rfs, FALSE);
  }
  return rfs->dir_rename(rfs, name, new_name);
}

/*----------------------------------------------------------------------------*/

static int32_t fs_os_get_file_size(fs_t* fs, const char* name) {
  fs_t* rfs = NULL;
  rfs = (fs_t*)str_str_value_with_len(s_items, name);
  return_value_if_fail(rfs, RET_BAD_PARAMS);
  return rfs->get_file_size(rfs, name);
}

static ret_t fs_os_get_disk_info(fs_t* fs, const char* volume, int32_t* free_kb,
                                 int32_t* total_kb) {
  (void)fs;
  fs_t* rfs = NULL;
  rfs = (fs_t*)str_str_value_with_len(s_items, volume);
  return_value_if_fail(rfs, RET_BAD_PARAMS);
  return rfs->get_disk_info(rfs, volume, free_kb, total_kb);
}

static ret_t fs_os_get_exe(fs_t* fs, char path[MAX_PATH + 1]) {
  fs_t* rfs = s_items[0].value;
  return_value_if_fail(rfs, RET_BAD_PARAMS);
  return rfs->get_exe(rfs, path);
}

static ret_t fs_os_get_cwd(fs_t* fs, char path[MAX_PATH + 1]) {
  (void)fs;
  return_value_if_fail(path, RET_BAD_PARAMS);

  *path = '\0';
  return RET_NOT_IMPL;
}

static ret_t fs_os_get_temp_path(fs_t* fs, char path[MAX_PATH + 1]) {
  fs_t* rfs = s_items[0].value;
  return_value_if_fail(rfs, RET_BAD_PARAMS);
  return rfs->get_temp_path(rfs, path);
}
/*----------------------------------------------------------------------------*/
typedef fs_t* (*fs_create_t)(const char* mnt);

ret_t fs_os_register(const char* mnt_name, fs_t* (*create)(const char*)) {
  return_value_if_fail(mnt_name && create, RET_BAD_PARAMS);
  return_value_if_fail(mnt_name[0] == '/', RET_BAD_PARAMS);
  int i;
  fs_t* fs = create(mnt_name);
  return_value_if_fail(fs, RET_BAD_PARAMS);

  for (i = 0; i < FS_REG_MAX; i++) {
    if (s_items[i].name == NULL) {
      s_items[i].name = tk_strdup(mnt_name);
      s_items[i].value = (char*)fs;
      return RET_OK;
    }
  }
  return_value_if_fail(!"fs register num over FS_REG_MAX!", RET_OOM);
  return RET_OOM;
}

/*----------------------------------------------------------------------------*/

static const fs_t s_os_fs = {
    .open_file = fs_os_open_file,
    .remove_file = fs_os_remove_file,
    .file_exist = fs_os_file_exist,
    .file_rename = fs_os_file_rename,
    .stat = fs_os_stat,

    .create_dir = fs_os_create_dir,
    .open_dir = fs_os_open_dir,
    .remove_dir = fs_os_remove_dir,
    .dir_exist = fs_os_dir_exist,
    .dir_rename = fs_os_dir_rename,

    .get_file_size = fs_os_get_file_size,
    .get_disk_info = fs_os_get_disk_info,
    .get_cwd = fs_os_get_cwd,
    .get_exe = fs_os_get_exe,
    .get_temp_path = fs_os_get_temp_path,
};

fs_t* os_fs(void) {
  return (fs_t*)&s_os_fs;
}
