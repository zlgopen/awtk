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
  return_value_if_fail(file != NULL && file->vt != NULL && buffer != NULL && file->vt->read != NULL,
                       -1);

  return file->vt->read(file, buffer, size);
}

int32_t fs_file_write(fs_file_t* file, const void* buffer, uint32_t size) {
  return_value_if_fail(
      file != NULL && file->vt != NULL && buffer != NULL && file->vt->write != NULL, -1);

  return file->vt->write(file, buffer, size);
}

int32_t fs_file_printf(fs_file_t* file, const char* const format_str, ...) {
  va_list v_l;
  int32_t ret = 0;
  return_value_if_fail(file != NULL && file->vt != NULL && file->vt->printf != NULL, -1);

  va_start(v_l, format_str);
  ret = file->vt->printf(file, format_str, v_l);
  va_end(v_l);

  return ret;
}

ret_t fs_file_seek(fs_file_t* file, int32_t offset) {
  return_value_if_fail(file != NULL && file->vt != NULL && file->vt->seek != NULL, RET_BAD_PARAMS);

  return file->vt->seek(file, offset);
}

ret_t fs_file_truncate(fs_file_t* file, int32_t offset) {
  return_value_if_fail(file != NULL && file->vt != NULL && file->vt->truncate != NULL,
                       RET_BAD_PARAMS);
  fs_file_seek(file, 0);
  return file->vt->truncate(file, offset);
}

bool_t fs_file_eof(fs_file_t* file) {
  return_value_if_fail(file != NULL && file->vt != NULL && file->vt->eof != NULL, TRUE);

  return file->vt->eof(file);
}

int64_t fs_file_tell(fs_file_t* file) {
  return_value_if_fail(file != NULL && file->vt != NULL && file->vt->tell != NULL, -1);

  return file->vt->tell(file);
}

int64_t fs_file_size(fs_file_t* file) {
  return_value_if_fail(file != NULL && file->vt != NULL && file->vt->size != NULL, -1);

  return file->vt->size(file);
}

ret_t fs_file_close(fs_file_t* file) {
  return_value_if_fail(file != NULL && file->vt != NULL && file->vt->close != NULL, RET_BAD_PARAMS);

  return file->vt->close(file);
}

ret_t fs_file_sync(fs_file_t* file) {
  return_value_if_fail(file != NULL && file->vt != NULL && file->vt->sync != NULL, RET_BAD_PARAMS);

  return file->vt->sync(file);
}

ret_t fs_file_stat(fs_file_t* file, fs_stat_info_t* fst) {
  return_value_if_fail(file != NULL && file->vt != NULL && file->vt->stat != NULL && fst != NULL,
                       RET_BAD_PARAMS);

  return file->vt->stat(file, fst);
}

ret_t fs_dir_rewind(fs_dir_t* dir) {
  return_value_if_fail(dir != NULL && dir->vt != NULL && dir->vt->rewind != NULL, RET_BAD_PARAMS);

  return dir->vt->rewind(dir);
}

ret_t fs_dir_read(fs_dir_t* dir, fs_item_t* item) {
  return_value_if_fail(dir != NULL && dir->vt != NULL && dir->vt->read != NULL && item != NULL,
                       RET_BAD_PARAMS);

  return dir->vt->read(dir, item);
}

ret_t fs_dir_close(fs_dir_t* dir) {
  return_value_if_fail(dir != NULL && dir->vt != NULL && dir->vt->close != NULL, RET_BAD_PARAMS);

  return dir->vt->close(dir);
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

ret_t fs_file_rename(fs_t* fs, const char* name, const char* new_name) {
  return_value_if_fail(fs != NULL && fs->file_rename != NULL && name != NULL && new_name != NULL,
                       RET_BAD_PARAMS);

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

ret_t fs_dir_rename(fs_t* fs, const char* name, const char* new_name) {
  return_value_if_fail(fs != NULL && fs->dir_rename != NULL && name != NULL && new_name,
                       RET_BAD_PARAMS);

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

ret_t fs_test_file(fs_t* fs) {
  char buff[32];
  fs_file_t* fp = NULL;
  const char* filename = "./test.txt";

  memset(buff, 0x00, sizeof(buff));
  fp = fs_open_file(fs, filename, "w+");
  assert(fs_file_write(fp, "hello", 5) == 5);
  assert(fs_file_tell(fp) == 5);
  assert(fs_file_sync(fp) == RET_OK);
  assert(fs_file_size(fp) == 5);
  assert(fs_file_truncate(fp, 0) == RET_OK);
  assert(fs_file_write(fp, "world", 5) == 5);
  assert(fs_file_seek(fp, 0) == RET_OK);
  assert(fs_file_write(fp, "WORLD", 5) == 5);
  assert(fs_file_close(fp) == RET_OK);
  assert(fs_file_exist(fs, filename) == TRUE);

  fp = fs_open_file(fs, filename, "a");
  assert(fs_file_write(fp, "world", 5) == 5);
  assert(fs_file_close(fp) == RET_OK);
  assert(fs_get_file_size(fs, filename) == 10);

  fp = fs_open_file(fs, filename, "r");
  assert(fs_file_read(fp, buff, 10) == 10);
  assert(strcmp(buff, "WORLDworld") == 0);
  assert(fs_file_close(fp) == RET_OK);
  assert(fs_get_file_size(fs, filename) == 10);

  fp = fs_open_file(fs, filename, "w+");
  assert(fs_file_printf(fp, "%s:%d", "hello", 10) == 8);
  assert(fs_file_close(fp) == RET_OK);

  assert(fs_file_rename(fs, filename, "./test.bin") == RET_OK);
  assert(!fs_file_exist(fs, filename));
  assert(fs_file_exist(fs, "./test.bin"));
  assert(fs_remove_file(fs, "./test.bin") == RET_OK);
  assert(!fs_file_exist(fs, "./test.bin"));

  return RET_OK;
}

ret_t fs_test_dir(fs_t* fs) {
  fs_item_t item;
  fs_dir_t* dir = NULL;

  assert(!fs_dir_exist(fs, "./a"));
  assert(fs_create_dir(fs, "./a") == RET_OK);
  assert(fs_dir_exist(fs, "./a"));

  assert(fs_create_dir(fs, "./a/b") == RET_OK);
  assert(fs_dir_exist(fs, "./a/b"));

  assert(fs_create_dir(fs, "./a/b/c1") == RET_OK);
  assert(fs_dir_exist(fs, "./a/b/c1"));

  assert(fs_create_dir(fs, "./a/b/c2") == RET_OK);
  assert(fs_dir_exist(fs, "./a/b/c2"));

  dir = fs_open_dir(fs, "./a/b");
  assert(dir != NULL);

  do {
    assert(fs_dir_read(dir, &item) == RET_OK);
    if (item.name[0] != '.') {
      break;
    } else {
      assert(!item.is_reg_file);
      assert(item.is_dir);
    }
  } while (TRUE);

  assert(strcmp(item.name, "c1") == 0 || strcmp(item.name, "c2") == 0);
  assert(!item.is_reg_file);
  assert(item.is_dir);

  assert(fs_dir_read(dir, &item) == RET_OK);
  assert(strcmp(item.name, "c1") == 0 || strcmp(item.name, "c2") == 0);
  assert(!item.is_reg_file);
  assert(item.is_dir);

  assert(fs_dir_close(dir) == RET_OK);

  if (fs_dir_rename(fs, "./a/b/c2", "./a/b/c3") == RET_OK) {
    assert(!fs_dir_exist(fs, "./a/b/c2"));
    assert(fs_dir_exist(fs, "./a/b/c3"));
    assert(fs_dir_rename(fs, "./a/b/c3", "./a/b/c2") == RET_OK);
  }
  assert(fs_remove_dir(fs, "./a/b/c1") == RET_OK);
  assert(fs_remove_dir(fs, "./a/b/c2") == RET_OK);
  assert(fs_remove_dir(fs, "./a/b") == RET_OK);
  assert(fs_remove_dir(fs, "./a") == RET_OK);

  return RET_OK;
}

ret_t fs_test(fs_t* fs) {
  fs_test_file(fs);
  return fs_test_dir(fs);
}
