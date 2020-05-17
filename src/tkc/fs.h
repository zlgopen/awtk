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

#ifndef TK_FS_H
#define TK_FS_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

/**
 * @class fs_stat_info_t
 *
 * 文件状态信息。
 *
 */
typedef struct _fs_stat_info_t {
  uint32_t dev;
  uint16_t ino;
  uint16_t mode;
  int16_t nlink;
  int16_t uid;
  int16_t gid;
  uint32_t rdev;
  /**
   * @property {uint64_t} size
   * 文件大小。
   */
  uint64_t size;
  /**
   * @property {uint64_t} atime
   * 最后访问时间。
   */
  uint64_t atime;
  /**
   * @property {uint64_t} mtime
   * 最后修改时间。
   */
  uint64_t mtime;
  /**
   * @property {uint64_t} ctime
   * 创建时间。
   */
  uint64_t ctime;
  /**
   * @property {bool_t} is_dir
   * 是否为目录。
   */
  bool_t is_dir;
  /**
   * @property {bool_t} is_link
   * 是否为链接。
   */
  bool_t is_link;
  /**
   * @property {bool_t} is_reg_file
   * 是否普通文件。
   */
  bool_t is_reg_file;
} fs_stat_info_t;

struct _fs_file_t;
typedef struct _fs_file_t fs_file_t;

typedef int32_t (*fs_file_read_t)(fs_file_t* file, void* buffer, uint32_t size);
typedef int32_t (*fs_file_write_t)(fs_file_t* file, const void* buffer, uint32_t size);
typedef int32_t (*fs_printf_t)(fs_file_t* file, const char* const format_str, va_list);
typedef ret_t (*fs_file_seek_t)(fs_file_t* file, int32_t offset);
typedef ret_t (*fs_file_truncate_t)(fs_file_t* file, int32_t offset);
typedef bool_t (*fs_file_eof_t)(fs_file_t* file);
typedef int64_t (*fs_file_tell_t)(fs_file_t* file);
typedef int64_t (*fs_file_size_t)(fs_file_t* file);
typedef ret_t (*fs_file_stat_t)(fs_file_t* file, fs_stat_info_t* fst);
typedef ret_t (*fs_file_sync_t)(fs_file_t* file);
typedef ret_t (*fs_file_close_t)(fs_file_t* file);

/**
 * @class fs_file_t
 * @annotation ["fake"]
 * 文件接口。
 *
 * 示例：
 *
 * ```c
 * int32_t ret = 0;
 * const char* file_name = "test.txt";
 * int32_t len = file_get_size(file_name);
 * uint8_t* buff = (uint8_t*)TKMEM_ALLOC(len + 1);
 * return_value_if_fail(buff != NULL, NULL);

 * fs_file_t* fp = fs_open_file(os_fs(), file_name, "rb");
 * if (fp != NULL) {
 *   ret = fs_file_read(fp, buff, len);
 *   fs_file_close(fp);
 * }
 * ```
 *
 */
typedef struct _fs_file_vtable_t {
  fs_file_read_t read;
  fs_file_write_t write;
  fs_printf_t printf;
  fs_file_seek_t seek;
  fs_file_truncate_t truncate;
  fs_file_eof_t eof;
  fs_file_tell_t tell;
  fs_file_size_t size;
  fs_file_sync_t sync;
  fs_file_stat_t stat;
  fs_file_close_t close;
} fs_file_vtable_t;

struct _fs_file_t {
  const fs_file_vtable_t* vt;
  void* data;
};

/**
 * @method fs_file_read
 *
 * 读取文件。
 *
 * @param {fs_file_t*} file 文件对象。
 * @param {void*} buffer 用于返回数据的缓冲区。
 * @param {uint32_t} size 缓冲区大小。
 *
 * @return {int32_t} 返回实际读取的字节数。
 */
int32_t fs_file_read(fs_file_t* file, void* buffer, uint32_t size);

/**
 * @method fs_file_write
 *
 * 写入文件。
 *
 * @param {fs_file_t*} file 文件对象。
 * @param {const void*} buffer 数据缓冲区。
 * @param {uint32_t} size 数据长度。
 *
 * @return {int32_t} 返回实际写入的字节数。
 */
int32_t fs_file_write(fs_file_t* file, const void* buffer, uint32_t size);

/**
 * @method fs_file_printf
 *
 * 写入文件。
 *
 * @param {fs_file_t*} file 文件对象。
 * @param {const char* const} format_str 格式化字符串。
 *
 * @return {int32_t} 返回实际写入的字节数。
 */
int32_t fs_file_printf(fs_file_t* file, const char* const format_str, ...);

/**
 * @method fs_file_seek
 *
 * 定位读写指针到指定的位置。
 *
 * @param {fs_file_t*} file 文件对象。
 * @param {uint32_t} offset 数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_file_seek(fs_file_t* file, int32_t offset);

/**
 * @method fs_file_truncate
 *
 * 清除文件内容。
 *
 * @param {fs_file_t*} file 文件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_file_truncate(fs_file_t* file, int32_t offset);

/**
 * @method fs_file_close
 *
 * 关闭文件。
 *
 * @param {fs_file_t*} file 文件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_file_close(fs_file_t* file);

/**
 * @method fs_file_sync
 *
 * 同步文件到磁盘。
 *
 * @param {fs_file_t*} file 文件对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_file_sync(fs_file_t* file);

/**
 * @method fs_file_stat
 *
 * 获取文件信息。
 *
 * @param {fs_file_t*} file 文件对象。
 * @param {fs_stat_info_t*} fst 文件状态信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_file_stat(fs_file_t* file, fs_stat_info_t* fst);

/**
 * @method fs_file_eof
 *
 * 判断文件是否结束。
 *
 * @param {fs_file_t*} file 文件对象。
 *
 * @return {bool_t} 返回TRUE表示结束，否则表示没结束。
 */
bool_t fs_file_eof(fs_file_t* file);

/**
 * @method fs_file_tell
 *
 * 获取文件当前读写位置。
 *
 * @param {fs_file_t*} file 文件对象。
 *
 * @return {int64_t} 返回文件当前读写位置。
 */
int64_t fs_file_tell(fs_file_t* file);

/**
 * @method fs_file_size
 *
 * 获取文件大小。
 *
 * @param {fs_file_t*} file 文件对象。
 *
 * @return {int64_t} 返回文件大小。
 */
int64_t fs_file_size(fs_file_t* file);

/**
 * @class fs_item_t
 * 代表目录或文件。
 */
typedef struct _fs_item_t {
  /**
   * @property {bool_t} is_dir
   * @annotation ["readable"]
   * 是否是目录。
   */
  bool_t is_dir;
  /**
   * @property {bool_t} is_link
   * @annotation ["readable"]
   * 是否是符号链接。
   */
  bool_t is_link;
  /**
   * @property {bool_t} is_reg_file
   * @annotation ["readable"]
   * 是否是普通文件。
   */
  bool_t is_reg_file;
  /**
   * @property {char*} name
   * @annotation ["readable"]
   * 名称。
   */
  char name[MAX_PATH + 1];
} fs_item_t;

struct _fs_dir_t;
typedef struct _fs_dir_t fs_dir_t;

typedef ret_t (*fs_dir_rewind_t)(fs_dir_t* dir);
typedef ret_t (*fs_dir_read_t)(fs_dir_t* dir, fs_item_t* item);
typedef ret_t (*fs_dir_close_t)(fs_dir_t* dir);

/**
 * @class fs_dir_t
 *
 * 文件夹接口。
 *
 */
typedef struct _fs_dir_vtable_t {
  fs_dir_rewind_t rewind;
  fs_dir_read_t read;
  fs_dir_close_t close;
  void* data;
} fs_dir_vtable_t;

struct _fs_dir_t {
  const fs_dir_vtable_t* vt;
  void* data;
};

/**
 * @method fs_dir_rewind
 *
 * 重置文件夹读取位置到开始。
 *
 * @param {fs_dir_t*} dir 文件夹对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_dir_rewind(fs_dir_t* dir);

/**
 * @method fs_dir_read
 *
 * 读取文件夹对象。
 *
 * @param {fs_dir_t*} dir 文件夹对象。
 * @param {fs_item_t*} item 返回一项。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_dir_read(fs_dir_t* dir, fs_item_t* item);

/**
 * @method fs_dir_close
 *
 * 关闭文件夹对象。
 *
 * @param {fs_dir_t*} dir 文件夹对象。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_dir_close(fs_dir_t* dir);

struct _fs_t;
typedef struct _fs_t fs_t;

typedef fs_file_t* (*fs_open_file_t)(fs_t* fs, const char* name, const char* mode);
typedef ret_t (*fs_remove_file_t)(fs_t* fs, const char* name);
typedef bool_t (*fs_file_exist_t)(fs_t* fs, const char* name);
typedef ret_t (*fs_file_rename_t)(fs_t* fs, const char* name, const char* new_name);

typedef fs_dir_t* (*fs_open_dir_t)(fs_t* fs, const char* name);
typedef ret_t (*fs_remove_dir_t)(fs_t* fs, const char* name);
typedef ret_t (*fs_create_dir_t)(fs_t* fs, const char* name);
typedef bool_t (*fs_dir_exist_t)(fs_t* fs, const char* name);
typedef ret_t (*fs_dir_rename_t)(fs_t* fs, const char* name, const char* new_name);

typedef int32_t (*fs_get_file_size_t)(fs_t* fs, const char* name);
typedef ret_t (*fs_get_disk_info_t)(fs_t* fs, const char* volume, int32_t* free_kb,
                                    int32_t* total_kb);
typedef ret_t (*fs_get_exe_t)(fs_t* fs, char path[MAX_PATH + 1]);
typedef ret_t (*fs_get_cwd_t)(fs_t* fs, char path[MAX_PATH + 1]);
typedef ret_t (*fs_stat_t)(fs_t* fs, const char* name, fs_stat_info_t* fst);
typedef ret_t (*fs_get_user_storage_path_t)(fs_t* fs, char path[MAX_PATH + 1]);

/**
 * @class fs_t
 *
 * 文件系统接口。
 *
 */
struct _fs_t {
  fs_open_file_t open_file;
  fs_remove_file_t remove_file;
  fs_file_exist_t file_exist;
  fs_file_rename_t file_rename;

  fs_open_dir_t open_dir;
  fs_create_dir_t create_dir;
  fs_remove_dir_t remove_dir;
  fs_dir_exist_t dir_exist;
  fs_dir_rename_t dir_rename;

  fs_get_file_size_t get_file_size;
  fs_get_disk_info_t get_disk_info;
  fs_get_cwd_t get_cwd;
  fs_get_exe_t get_exe;
  fs_get_user_storage_path_t get_user_storage_path;
  fs_stat_t stat;
};

/**
 * @method fs_open_file
 *
 * 打开文件。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} name 文件名。
 * @param {const char*} mode 打开方式，取值请参考POSIX的[fopen函数](https://www.runoob.com/cprogramming/c-function-fopen.html)相应的参数。
 *
 * @return {ret_t} 返回非NULL表示成功，否则表示失败。
 */
fs_file_t* fs_open_file(fs_t* fs, const char* name, const char* mode);

/**
 * @method fs_remove_file
 *
 * 刪除文件。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} name 文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_remove_file(fs_t* fs, const char* name);

/**
 * @method fs_file_exist
 *
 * 判断文件是否存在。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} name 文件名。
 *
 * @return {bool_t} 返回TRUE表示存在，否则表示不存在。
 */
bool_t fs_file_exist(fs_t* fs, const char* name);

/**
 * @method fs_file_rename
 *
 * 文件重命名。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} name 旧文件名。
 * @param {const char*} new_name 新文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_file_rename(fs_t* fs, const char* name, const char* new_name);

/**
 * @method fs_open_dir
 *
 * 打开目录。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} name 目录名称。
 *
 * @return {fs_dir_t} 返回非NULL表示成功，否则表示失败。
 */
fs_dir_t* fs_open_dir(fs_t* fs, const char* name);

/**
 * @method fs_create_dir
 *
 * 创建目录。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} name 目录名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_create_dir(fs_t* fs, const char* name);

/**
 * @method fs_remove_dir
 *
 * 刪除目录。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} name 目录名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_remove_dir(fs_t* fs, const char* name);

/**
 * @method fs_dir_exist
 *
 * 判断目录是否存在。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} name 目录名称。
 *
 * @return {bool_t} 返回TRUE表示存在，否则表示不存在。
 */
bool_t fs_dir_exist(fs_t* fs, const char* name);

/**
 * @method fs_dir_rename
 *
 * 目录重命名。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} name 旧目录名称。
 * @param {const char*} new_name 新目录名称。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_dir_rename(fs_t* fs, const char* name, const char* new_name);

/**
 * @method fs_get_file_size
 *
 * 获取文件大小。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} name 文件名。
 *
 * @return {ret_t} 返回不是-1表示成功，否则表示失败。
 */
int32_t fs_get_file_size(fs_t* fs, const char* name);

/**
 * @method fs_get_disk_info
 *
 * 获取文件系统信息。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} value 卷名。
 * @param {int32_t*} free_kb 用于返回空闲空间大小(KB)
 * @param {int32_t*} total_kb 用于返回总共空间大小(KB)
 *
 * @return {ret_t} 返回不是-1表示成功，否则表示失败。
 */
ret_t fs_get_disk_info(fs_t* fs, const char* volume, int32_t* free_kb, int32_t* total_kb);

/**
 * @method fs_stat
 *
 * 获取文件信息。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {const char*} name 文件名。
 * @param {fs_stat_info_t*} fst 文件状态信息。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_stat(fs_t* fs, const char* name, fs_stat_info_t* fst);

/**
 * @method fs_get_exe
 *
 * 获取可执行文件所在目录。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {char*} path 保存可执行文件的路径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_get_exe(fs_t* fs, char path[MAX_PATH + 1]);

/**
 * @method fs_get_user_storage_path
 *
 * 获取home目录或者应用程序可以写入数据的目录。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {char*} path 保存路径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_get_user_storage_path(fs_t* fs, char path[MAX_PATH + 1]);

/**
 * @method fs_get_cwd
 *
 * 获取当前所在目录。
 *
 * @param {fs_t*} fs 文件系统对象，一般赋值为os_fs()。
 * @param {char*} path 保存当前所在目录的路径。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fs_get_cwd(fs_t* fs, char path[MAX_PATH + 1]);

/**
 * @method os_fs
 * 获取缺省的文件系统对象。
 * @annotation ["constructor"]
 * @return {fs_t*} 返回文件系统对象。
 */
fs_t* os_fs(void);

/*wrapper*/

/**
 * @method file_exist
 *
 * 判断文件是否存在。
 *
 * @param {const char*} name 文件名。
 *
 * @return {bool_t} 返回TRUE表示成功，否则表示失败。
 */
bool_t file_exist(const char* name);

/**
 * @method file_remove
 *
 * 刪除文件。
 *
 * @param {const char*} name 文件名。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_remove(const char* name);

/**
 * @method file_get_size
 *
 * 获取文件大小。
 *
 * @param {const char*} name 文件名。
 *
 * @return {int32_t} 返回非负表示文件大小，否则表示失败。
 */
int32_t file_get_size(const char* name);

/**
 * @method file_read
 *
 * 读取文件的全部内容。
 *
 * @param {const char*} name 文件名。
 * @param {uint32_t*} size 返回实际读取的长度。
 *
 * @return {void*} 返回读取的数据，需要调用TKMEM_FREE释放。
 */
void* file_read(const char* name, uint32_t* size);

/**
 * @method file_read_part
 *
 * 从某个位置读取文件。
 *
 * @param {const char*} name 文件名。
 * @param {const void*} buffer 数据缓冲区。
 * @param {uint32_t} size 数据长度。
 * @param {uint32_t} offset 偏移量。
 *
 * @return {int32_t} 返回实际读取的字节数。
 */
int32_t file_read_part(const char* name, void* buff, uint32_t size, uint32_t offset);

/**
 * @method file_write
 *
 * 写入文件。
 *
 * @param {const char*} name 文件名。
 * @param {const void*} buffer 数据缓冲区。
 * @param {uint32_t} size 数据长度。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t file_write(const char* name, const void* buff, uint32_t size);

/*basic function test*/
ret_t fs_test(fs_t* fs);
ret_t fs_test_file(fs_t* fs);
ret_t fs_test_dir(fs_t* fs);

END_C_DECLS

#endif /*TK_FS_H*/
