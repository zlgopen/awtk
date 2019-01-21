/**
 * File:   fs.h
 * Author: AWTK Develop Team
 * Brief:  simple fs api
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#ifndef TK_FS_H
#define TK_FS_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

struct _fs_file_t;
typedef struct _fs_file_t fs_file_t;

typedef int32_t (*fs_file_read_t)(fs_file_t* file, void* buffer, uint32_t size);
typedef int32_t (*fs_file_write_t)(fs_file_t* file, const void* buffer, uint32_t size);
typedef ret_t (*fs_file_seek_t)(fs_file_t* file, int32_t offset);
typedef ret_t (*fs_file_truncate_t)(fs_file_t* file, int32_t offset);
typedef ret_t (*fs_file_close_t)(fs_file_t* file);

struct _fs_file_t {
  fs_file_read_t read;
  fs_file_write_t write;
  fs_file_seek_t seek;
  fs_file_truncate_t truncate;
  fs_file_close_t close;
  void* data;
};

int32_t fs_file_read(fs_file_t* file, void* buffer, uint32_t size);
int32_t fs_file_write(fs_file_t* file, const void* buffer, uint32_t size);
ret_t fs_file_seek(fs_file_t* file, int32_t offset);
ret_t fs_file_truncate(fs_file_t* file, int32_t offset);
ret_t fs_file_close(fs_file_t* file);

typedef struct _fs_item_t {
  uint32_t is_dir : 1;
  uint32_t is_file : 1;
  char name[MAX_PATH + 1];
} fs_item_t;

struct _fs_dir_t;
typedef struct _fs_dir_t fs_dir_t;

typedef ret_t (*fs_dir_rewind_t)(fs_dir_t* dir);
typedef ret_t (*fs_dir_read_t)(fs_dir_t* dir, fs_item_t* item);
typedef ret_t (*fs_dir_close_t)(fs_dir_t* dir);

struct _fs_dir_t {
  fs_dir_rewind_t rewind;
  fs_dir_read_t read;
  fs_dir_close_t close;
  void* data;
};

ret_t fs_dir_rewind(fs_dir_t* dir);
ret_t fs_dir_read(fs_dir_t* dir, fs_item_t* item);
ret_t fs_dir_close(fs_dir_t* dir);

struct _fs_t;
typedef struct _fs_t fs_t;

typedef fs_file_t* (*fs_open_file_t)(fs_t* fs, const char* name, const char* mode);
typedef ret_t (*fs_remove_file_t)(fs_t* fs, const char* name);
typedef bool_t (*fs_file_exist_t)(fs_t* fs, const char* name);
typedef bool_t (*fs_file_rename_t)(fs_t* fs, const char* name, const char* new_name);

typedef fs_dir_t* (*fs_open_dir_t)(fs_t* fs, const char* name);
typedef ret_t (*fs_remove_dir_t)(fs_t* fs, const char* name);
typedef bool_t (*fs_dir_exist_t)(fs_t* fs, const char* name);
typedef bool_t (*fs_dir_rename_t)(fs_t* fs, const char* name, const char* new_name);

typedef int32_t (*fs_get_file_size_t)(fs_t* fs, const char* name);
typedef ret_t (*fs_get_disk_info_t)(fs_t* fs, const char* volume, int32_t* free_kb,
                                    int32_t* total_kb);
typedef ret_t (*fs_get_exe_t)(fs_t* fs, char path[MAX_PATH + 1]);
typedef ret_t (*fs_get_cwd_t)(fs_t* fs, char path[MAX_PATH + 1]);

struct _fs_t {
  fs_open_file_t open_file;
  fs_remove_file_t remove_file;
  fs_file_exist_t file_exist;
  fs_file_rename_t file_rename;

  fs_open_dir_t open_dir;
  fs_remove_dir_t remove_dir;
  fs_dir_exist_t dir_exist;
  fs_dir_rename_t dir_rename;

  fs_get_file_size_t get_file_size;
  fs_get_disk_info_t get_disk_info;
  fs_get_cwd_t get_cwd;
  fs_get_exe_t get_exe;
};

fs_file_t* fs_open_file(fs_t* fs, const char* name, const char* mode);
ret_t fs_remove_file(fs_t* fs, const char* name);
bool_t fs_file_exist(fs_t* fs, const char* name);
bool_t fs_file_rename(fs_t* fs, const char* name, const char* new_name);

fs_dir_t* fs_open_dir(fs_t* fs, const char* name);
ret_t fs_remove_dir(fs_t* fs, const char* name);
bool_t fs_dir_exist(fs_t* fs, const char* name);
bool_t fs_dir_rename(fs_t* fs, const char* name, const char* new_name);

int32_t fs_get_file_size(fs_t* fs, const char* name);
ret_t fs_get_disk_info(fs_t* fs, const char* volume, int32_t* free_kb, int32_t* total_kb);
ret_t fs_get_exe(fs_t* fs, char path[MAX_PATH + 1]);
ret_t fs_get_cwd(fs_t* fs, char path[MAX_PATH + 1]);

fs_t* os_fs(void);

/*wrapper*/
bool_t file_exist(const char* name);
ret_t file_remove(const char* name);
int32_t file_get_size(const char* name);
void* file_read(const char* name, uint32_t* size);
int32_t file_read_part(const char* name, void* buff, uint32_t size, uint32_t offset);
ret_t file_write(const char* name, const void* buff, uint32_t size);

END_C_DECLS

#endif /*TK_FS_H*/
