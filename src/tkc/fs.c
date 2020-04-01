/**
 * File:   fs.h
 * Author: AWTK Develop Team
 * Brief:  simple fs api
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-04-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"

int32_t fs_file_read(fs_file_t* file, void* buffer, uint32_t size) {
  return_value_if_fail(file != NULL && buffer != NULL && file->read != NULL, -1);

  return file->read(file, buffer, size);
}

int32_t fs_file_write(fs_file_t* file, const void* buffer, uint32_t size) {
  return_value_if_fail(file != NULL && buffer != NULL && file->write != NULL, -1);

  return file->write(file, buffer, size);
}

int32_t fs_file_printf(fs_file_t* file, const char* const format_str, ...) {
  va_list v_l;
  int32_t ret = 0;
  return_value_if_fail(file != NULL && file->f_printf != NULL, -1);

  va_start(v_l, format_str);
  ret = file->f_printf(file, format_str, v_l);
  va_end(v_l);

  return ret;
}

ret_t fs_file_seek(fs_file_t* file, int32_t offset) {
  return_value_if_fail(file != NULL && file->seek != NULL, RET_BAD_PARAMS);

  return file->seek(file, offset);
}

ret_t fs_file_truncate(fs_file_t* file, int32_t offset) {
  return_value_if_fail(file != NULL && file->truncate != NULL, RET_BAD_PARAMS);

  return file->truncate(file, offset);
}

bool_t fs_file_eof(fs_file_t* file) {
  return_value_if_fail(file != NULL && file->eof != NULL, RET_BAD_PARAMS);

  return file->eof(file);
}

ret_t fs_file_close(fs_file_t* file) {
  return_value_if_fail(file != NULL && file->close != NULL, RET_BAD_PARAMS);

  return file->close(file);
}

ret_t fs_dir_rewind(fs_dir_t* dir) {
  return_value_if_fail(dir != NULL && dir->rewind != NULL, RET_BAD_PARAMS);

  return dir->rewind(dir);
}

ret_t fs_dir_read(fs_dir_t* dir, fs_item_t* item) {
  return_value_if_fail(dir != NULL && dir->read != NULL && item != NULL, RET_BAD_PARAMS);

  return dir->read(dir, item);
}

ret_t fs_dir_close(fs_dir_t* dir) {
  return_value_if_fail(dir != NULL && dir->close != NULL, RET_BAD_PARAMS);

  return dir->close(dir);
}

fs_file_t* fs_open_file(fs_t* fs, const char* name, const char* mode) {
  return_value_if_fail(fs != NULL && name != NULL && mode != NULL, NULL);

  return fs->open_file(fs, name, mode);
}

ret_t fs_remove_file(fs_t* fs, const char* name) {
  return_value_if_fail(fs != NULL && fs->remove_file != NULL && name != NULL, RET_BAD_PARAMS);

  return fs->remove_file(fs, name);
}

bool_t fs_file_exist(fs_t* fs, const char* name) {
  return_value_if_fail(fs != NULL && fs->file_exist != NULL && name != NULL, FALSE);

  return fs->file_exist(fs, name);
}

bool_t fs_file_rename(fs_t* fs, const char* name, const char* new_name) {
  return_value_if_fail(fs != NULL && fs->file_rename != NULL && name != NULL && new_name != NULL,
                       FALSE);

  return fs->file_rename(fs, name, new_name);
}

fs_dir_t* fs_open_dir(fs_t* fs, const char* name) {
  return_value_if_fail(fs != NULL && fs->open_dir != NULL && name != NULL, NULL);

  return fs->open_dir(fs, name);
}

ret_t fs_remove_dir(fs_t* fs, const char* name) {
  return_value_if_fail(fs != NULL && fs->remove_dir != NULL && name != NULL, RET_BAD_PARAMS);

  return fs->remove_dir(fs, name);
}

ret_t fs_create_dir(fs_t* fs, const char* name) {
  return_value_if_fail(fs != NULL && fs->create_dir != NULL && name != NULL, RET_BAD_PARAMS);

  return fs->create_dir(fs, name);
}

bool_t fs_dir_exist(fs_t* fs, const char* name) {
  return_value_if_fail(fs != NULL && fs->dir_exist != NULL && name != NULL, FALSE);

  return fs->dir_exist(fs, name);
}

bool_t fs_dir_rename(fs_t* fs, const char* name, const char* new_name) {
  return_value_if_fail(fs != NULL && fs->dir_rename != NULL && name != NULL && new_name, FALSE);

  return fs->dir_rename(fs, name, new_name);
}

int32_t fs_get_file_size(fs_t* fs, const char* name) {
  return_value_if_fail(fs != NULL && fs->get_file_size != NULL && name != NULL, -1);

  return fs->get_file_size(fs, name);
}

ret_t fs_get_disk_info(fs_t* fs, const char* volume, int32_t* free_kb, int32_t* total_kb) {
  return_value_if_fail(fs != NULL && free_kb != NULL && total_kb != NULL, RET_BAD_PARAMS);

  return fs->get_disk_info(fs, volume, free_kb, total_kb);
}

ret_t fs_get_exe(fs_t* fs, char path[MAX_PATH + 1]) {
  return_value_if_fail(fs != NULL && fs->get_exe != NULL && path != NULL, RET_BAD_PARAMS);

  return fs->get_exe(fs, path);
}

ret_t fs_get_user_storage_path(fs_t* fs, char path[MAX_PATH + 1]) {
  return_value_if_fail(fs != NULL && fs->get_user_storage_path != NULL && path != NULL,
                       RET_BAD_PARAMS);

  return fs->get_user_storage_path(fs, path);
}

ret_t fs_get_cwd(fs_t* fs, char path[MAX_PATH + 1]) {
  return_value_if_fail(fs != NULL && fs->get_cwd != NULL && path != NULL, RET_BAD_PARAMS);

  return fs->get_cwd(fs, path);
}

int32_t file_get_size(const char* name) {
  return fs_get_file_size(os_fs(), name);
}

ret_t fs_stat(fs_t* fs, const char* name, fs_stat_info_t* fst) {
  return_value_if_fail(fs != NULL && fs->stat != NULL && name != NULL, RET_BAD_PARAMS);

  return fs->stat(fs, name, fst);
}

int32_t file_read_part(const char* name, void* buff, uint32_t size, uint32_t offset) {
  int32_t ret = 0;
  fs_file_t* fp = NULL;
  return_value_if_fail(name != NULL && buff != NULL, -1);

  fp = fs_open_file(os_fs(), name, "rb");
  if (fp != NULL) {
    if (fs_file_seek(fp, offset) == RET_OK) {
      ret = fs_file_read(fp, buff, size);
    }
    fs_file_close(fp);
  }

  return ret;
}

void* file_read(const char* name, uint32_t* size) {
  uint8_t* buff = NULL;
  int32_t len = file_get_size(name);
  return_value_if_fail(name != NULL && len > 0, NULL);

  buff = (uint8_t*)TKMEM_ALLOC(len + 1);
  return_value_if_fail(buff != NULL, NULL);

  if (file_read_part(name, buff, len, 0) == len) {
    if (size != NULL) {
      *size = len;
    }
    buff[len] = '\0';

    return buff;
  } else {
    if (size != NULL) {
      *size = 0;
    }
    TKMEM_FREE(buff);

    return NULL;
  }
}

ret_t file_write(const char* name, const void* buff, uint32_t size) {
  ret_t ret = RET_OK;
  fs_file_t* fp = NULL;
  return_value_if_fail(name != NULL && buff != NULL, RET_BAD_PARAMS);

  fp = fs_open_file(os_fs(), name, "wb+");
  return_value_if_fail(fp != NULL, RET_FAIL);

  if (fs_file_write(fp, buff, size) != size) {
    ret = RET_FAIL;
  }

  fs_file_close(fp);

  return ret;
}

ret_t file_remove(const char* name) {
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);

  return fs_remove_file(os_fs(), name);
}

bool_t file_exist(const char* name) {
  return_value_if_fail(name != NULL, FALSE);

  return fs_file_exist(os_fs(), name);
}
