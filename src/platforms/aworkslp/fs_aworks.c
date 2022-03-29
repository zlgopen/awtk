/**
 * File:   mutex.c
 * Author: AWTK Develop Team
 * Brief:  mutex
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "aworks.h"
#include "aw_mount.h"
#include "aw_unistd.h"
#include "aw_fcntl.h"
#include "aw_fs_type.h"
#include "aw_dirent.h"
#include "aw_stat.h"
#include "aw_ioctl.h"
#include "aw_stropts.h"
#include "aw_fs_type.h"
#include "aw_autoconf.h"

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/path.h"
#include "tkc/utils.h"

#ifndef tk_log_info_lf
#define tk_log_info_lf(fmt, args...) \
  log_info("[%dL %s] " fmt " \r\n", __LINE__, __FUNCTION__, ##args)
#define tk_log_info_htlf(header, fmt, args...) tk_log_info_lf(fmt, ##args)
#endif

#if CONFIG_HC32F4A0_CATNET_BOARD || CONFIG_M105X_EV_BOARD_LP
#define TKC_FS_EXE_PATH "/lfs/bin/app/"
#define TKC_FS_TEMP_PATH "/lfs/tmp/"
#define TKC_FS_USER_STORAGE_PATH "/lfs/usr/"

#elif CONFIG_PLATFORM_UIS8910DM_AWORKS_LP
#define TKC_FS_EXE_PATH "/sd0/bin/app"
#define TKC_FS_TEMP_PATH "/sd0/tmp"
#define TKC_FS_USER_STORAGE_PATH "/sd0/usr"

#else
#define TKC_FS_EXE_PATH "/none"
#define TKC_FS_TEMP_PATH "/none"
#define TKC_FS_USER_STORAGE_PATH "/none"
#endif /* CONFIG_HC32F4A0_CATNET_BOARD */

/*----------------------------------------------------------------------------*/
/* 文件操作	                                                                  */
/*----------------------------------------------------------------------------*/

static int32_t fs_os_file_read(fs_file_t* file, void* buffer, uint32_t size) {
  return_value_if_fail(file, -1);
  int fd = (int)(file->data);

  return (int32_t)aw_read(fd, buffer, size);
}

static int32_t fs_os_file_write(fs_file_t* file, const void* buffer, uint32_t size) {
  return_value_if_fail(file, -1);
  int fd = (int)(file->data);

  return aw_write(fd, buffer, size);
}

static ret_t fs_os_file_seek(fs_file_t* file, int32_t offset) {
  return_value_if_fail(file, RET_BAD_PARAMS);
  int fd = (int)(file->data);

  return aw_lseek(fd, offset, AW_SEEK_SET) >= 0 ? RET_OK : RET_FAIL;
}

static ret_t fs_os_file_truncate(fs_file_t* file, int32_t size) {
  return_value_if_fail(file, RET_BAD_PARAMS);
  int fd = (int)(file->data);

  return aw_ftruncate(fd, size) == AW_OK ? RET_OK : RET_FAIL;
}

static ret_t fs_os_file_sync(fs_file_t* file) {
  return_value_if_fail(file, RET_BAD_PARAMS);
  int fd = (int)(file->data);
  return aw_fsync(fd) == AW_OK ? RET_OK : RET_FAIL;
}

static ret_t fs_os_file_close(fs_file_t* file) {
  return_value_if_fail(file, RET_BAD_PARAMS);
  int fd = (int)(file->data);

  aw_close(fd);
  TKMEM_FREE(file);

  return RET_OK;
}
static ret_t fs_os_file_stat(fs_file_t* file, fs_stat_info_t* fst) {
  return_value_if_fail(file && fst, RET_FAIL);
  struct aw_stat st;
  int fd = (int)(file->data);
  return_value_if_fail(aw_fstat(fd, &st) == AW_OK, -1);

  memset(fst, 0, sizeof(*fst));
  fst->mode = st.st_mode;
  fst->size = st.st_size;
  fst->dev = st.st_dev;
  fst->rdev = st.st_rdev;

  fst->dev = st.st_dev;
  fst->ino = st.st_ino;
  fst->mode = st.st_mode;
  fst->nlink = st.st_nlink;
  fst->uid = st.st_uid;
  fst->gid = st.st_gid;
  fst->rdev = st.st_rdev;
  fst->size = st.st_size;

  fst->is_dir = AW_S_ISDIR(st.st_mode);
  fst->is_reg_file = AW_S_ISREG(st.st_mode);
  return RET_OK;
}

static const fs_file_vtable_t s_fs_file_vtable_t = {.read = fs_os_file_read,
                                                    .write = fs_os_file_write,
                                                    .seek = fs_os_file_seek,
                                                    .truncate = fs_os_file_truncate,
                                                    .stat = fs_os_file_stat,
                                                    .sync = fs_os_file_sync,
                                                    .close = fs_os_file_close};

static fs_file_t* fs_file_create(int fd) {
  fs_file_t* f = NULL;
  return_value_if_fail(fd >= 0, NULL);

  f = TKMEM_ZALLOC(fs_file_t);
  if (f != NULL) {
    f->vt = &s_fs_file_vtable_t;
    f->data = (void*)fd;
  } else {
    aw_close(fd);
  }

  return f;
}

#if TKC_SAVE_FIRMWARE_BY_BL

static int32_t fs_bl_firmware_read(fs_file_t* file, void* buffer, uint32_t size) {
  return_value_if_fail(!"not support!", -1);
  return -1;
}

static int32_t fs_bl_firmware_write(fs_file_t* file, const void* buffer, uint32_t size) {
  return BL_AppUpdateProcess(buffer, size);
}

static ret_t fs_bl_firmware_seek(fs_file_t* file, int32_t offset) {
  return_value_if_fail(!"not support!", RET_NOT_IMPL);
  return RET_OK;
}

static ret_t fs_bl_firmware_truncate(fs_file_t* file, int32_t size) {
  return_value_if_fail(!"not support!", -1);
  return -1;
}

static ret_t fs_bl_firmware_sync(fs_file_t* file) {
  return_value_if_fail(!"not support!", -1);
  return -1;
}

static ret_t fs_bl_firmware_close(fs_file_t* file) {
  return_value_if_fail(BL_AppUpdateEnd() == 0, RET_FAIL);
  TKMEM_FREE(file);
  return RET_OK;
}

static ret_t fs_bl_firmware_stat(fs_file_t* file, fs_stat_info_t* fst) {
  return_value_if_fail(!"not support!", -1);
  return -1;
}

static const fs_file_vtable_t s_fs_bl_firmware_vtable_t = {
    .read = fs_bl_firmware_read,
    .write = fs_bl_firmware_write,
    .seek = fs_bl_firmware_seek,
    .truncate = fs_bl_firmware_truncate,
    .stat = fs_bl_firmware_stat,
    .sync = fs_bl_firmware_sync,
    .close = fs_bl_firmware_close,
};

static fs_file_t* fs_bl_firmware_create(void) {
  fs_file_t* f = NULL;
  int ret;

  f = TKMEM_ZALLOC(fs_file_t);
  if (f != NULL) {
    f->vt = &s_fs_bl_firmware_vtable_t;
    f->data = NULL;
    ret = BL_AppUpdateStart(TKC_FIRMWARE_PATH);
    if (ret != 0) {
      TKMEM_FREE(f);
      return_value_if_fail(!"BL_AppUpdateStart() failed", NULL);
    }
  }
  return f;
}
#endif /* TKC_SAVE_FIRMWARE_BY_BL */

static fs_file_t* fs_os_open_file(fs_t* fs, const char* name, const char* mode) {
  (void)fs;
  return_value_if_fail(name && mode, NULL);
  int oflag = 0;
  fs_file_t* file;

  if (strchr(mode, '+')) {
    if (strchr(mode, 'r')) {
      oflag |= AW_O_RDWR;
    } else if (strchr(mode, 'w')) {
      oflag |= AW_O_RDWR | AW_O_CREAT | AW_O_TRUNC;
    } else if (strchr(mode, 'a')) {
#if 0
      oflag |= AW_O_RDWR | AW_O_CREAT | AW_O_APPEND;
#else
      /* lfs bug：带有 APPEND 标志，则每次写都从文件尾开始写，seek 就无效了，因此这里不带 append 标志，创建完后再 seek 一次 */
      oflag |= AW_O_RDWR | AW_O_CREAT;
#endif
    }
  } else {
    if (strchr(mode, 'r')) {
      oflag |= AW_O_RDONLY;
    } else if (strchr(mode, 'w')) {
      oflag |= AW_O_WRONLY | AW_O_CREAT | AW_O_TRUNC;
    } else if (strchr(mode, 'a')) {
#if 0
      oflag |= AW_O_RDWR | AW_O_CREAT | AW_O_APPEND;
#else
      /* lfs bug：带有 APPEND 标志，则每次写都从文件尾开始写，seek 就无效了，因此这里不带 append 标志，创建完后再 seek 一次 */
      oflag |= AW_O_RDWR | AW_O_CREAT;
#endif
    }
  }

#if TKC_SAVE_FIRMWARE_BY_BL
  if (tk_str_eq(TKC_FIRMWARE_PATH, name)) {
    file = fs_bl_firmware_create();
  }
  return file;
#endif /* TKC_SAVE_FIRMWARE_BY_BL */

  file = fs_file_create(aw_open(name, oflag, 0777));
  if (file) {
    if (strchr(mode, 'a')) {
      int fd = (int)(file->data);
      aw_lseek(fd, 0, AW_SEEK_END);
    }
  }
  return file;
}

static ret_t fs_os_remove_file(fs_t* fs, const char* name) {
  (void)fs;
  return_value_if_fail(name, RET_BAD_PARAMS);

  return aw_unlink(name) == AW_OK ? RET_OK : RET_FAIL;
}

static bool_t fs_os_file_exist(fs_t* fs, const char* name) {
  (void)fs;
  return_value_if_fail(name, FALSE);

  struct aw_stat st;
  return (aw_stat(name, &st) == 0 && AW_S_ISREG(st.st_mode));
}

static bool_t fs_os_file_rename(fs_t* fs, const char* name, const char* new_name) {
  (void)fs;
  return_value_if_fail(name && new_name, FALSE);

  return (aw_rename(name, new_name) == AW_OK);
}

static ret_t fs_os_stat(fs_t* fs, const char* name, fs_stat_info_t* fst) {
  struct aw_stat st;
  return_value_if_fail(fs && fst && name, RET_FAIL);
  return_value_if_fail(aw_stat(name, &st) == AW_OK, -1);

  memset(fst, 0, sizeof(*fst));
  fst->mode = st.st_mode;
  fst->size = st.st_size;
  fst->dev = st.st_dev;
  fst->rdev = st.st_rdev;

  fst->dev = st.st_dev;
  fst->ino = st.st_ino;
  fst->mode = st.st_mode;
  fst->nlink = st.st_nlink;
  fst->uid = st.st_uid;
  fst->gid = st.st_gid;
  fst->rdev = st.st_rdev;
  fst->size = st.st_size;

  fst->is_dir = AW_S_ISDIR(st.st_mode);
  fst->is_reg_file = AW_S_ISREG(st.st_mode);
  return RET_OK;
}

/*----------------------------------------------------------------------------*/
/* 文件夹操作                                                                 */
/*----------------------------------------------------------------------------*/

struct fs_dir_ex {
  fs_dir_t dir;
  char dir_name[MAX_PATH + 1];
};

static ret_t fs_os_dir_read(fs_dir_t* dir, fs_item_t* item) {
  return_value_if_fail(dir && dir->data && item, RET_BAD_PARAMS);
  char item_path[MAX_PATH];
  struct aw_dirent* entry;
  struct aw_dir* d = (struct aw_dir*)(dir->data);
  return_value_if_fail(aw_readdir(d, &entry) == AW_OK, RET_FAIL);

  memset(item, 0x00, sizeof(fs_item_t));
  if (entry != NULL) {
    item_path[0] = '\0';
    strncat(item_path, ((struct fs_dir_ex*)dir)->dir_name, MAX_PATH);
    strncat(item_path, "/", MAX_PATH);
    strncat(item_path, entry->d_name, MAX_PATH);

    struct aw_stat st;
    return_value_if_fail(aw_stat(item_path, &st) == AW_OK, RET_FAIL);

    item->is_dir = AW_S_ISDIR(st.st_mode);
    item->is_reg_file = AW_S_ISREG(st.st_mode);
    tk_strncpy(item->name, entry->d_name, MAX_PATH);
    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

static ret_t fs_os_dir_rewind(fs_dir_t* dir) {
  return_value_if_fail(!"fs_os_dir_rewind not supported yet", RET_BAD_PARAMS);
  return RET_NOT_IMPL;
}

static ret_t fs_os_dir_close(fs_dir_t* dir) {
  return_value_if_fail(dir && dir->data, RET_BAD_PARAMS);
  struct aw_dir* d = (struct aw_dir*)dir->data;

  aw_closedir(d);
  TKMEM_FREE(d);
  TKMEM_FREE(dir);

  return RET_OK;
}

static const fs_dir_vtable_t s_fs_dir_vtable_t = {
    .read = fs_os_dir_read, .rewind = fs_os_dir_rewind, .close = fs_os_dir_close};

static fs_dir_t* fs_dir_create(struct aw_dir* dir, const char* dir_name) {
  fs_dir_t* d = NULL;
  return_value_if_fail(dir != NULL, NULL);

  struct fs_dir_ex* d_ex = TKMEM_ZALLOC(struct fs_dir_ex);
  if (d_ex != NULL) {
    d = &d_ex->dir;
    d->vt = &s_fs_dir_vtable_t;
    d->data = dir;
    tk_strncpy(d_ex->dir_name, dir_name, MAX_PATH);
  } else {
    aw_closedir(dir);
    TKMEM_FREE(dir);
  }

  return d;
}

static ret_t fs_os_create_dir(fs_t* fs, const char* name) {
  return aw_mkdir(name, AW_S_IRWXU | AW_S_IRWXG | AW_S_IRWXO) == 0 ? RET_OK : RET_FAIL;
}

static fs_dir_t* fs_os_open_dir(fs_t* fs, const char* name) {
  struct aw_dir* dir = TKMEM_ZALLOC(struct aw_dir);
  return_value_if_fail(aw_opendir(dir, name) == AW_OK, NULL);
  return fs_dir_create(dir, name);
}

static ret_t fs_os_remove_dir(fs_t* fs, const char* name) {
  return aw_rmdir(name) == AW_OK ? RET_OK : RET_FAIL;
}

static bool_t fs_os_dir_exist(fs_t* fs, const char* name) {
  struct aw_stat st;
  return (aw_stat(name, &st) == AW_OK && AW_S_ISDIR(st.st_mode));
}

static ret_t fs_os_dir_rename(fs_t* fs, const char* name, const char* new_name) {
  return (aw_rename(name, new_name) == AW_OK) ? RET_OK : RET_FAIL;
}

/*----------------------------------------------------------------------------*/
/* 文件系统操作                                                               */
/*----------------------------------------------------------------------------*/

static int32_t fs_os_get_file_size(fs_t* fs, const char* name) {
  struct aw_stat st;

  return_value_if_fail(aw_stat(name, &st) == AW_OK, -1);
  return st.st_size;
}

static ret_t fs_os_get_disk_info(fs_t* fs, const char* volume, int32_t* free_kb,
                                 int32_t* total_kb) {
  (void)fs;
  return_value_if_fail(free_kb && total_kb, RET_BAD_PARAMS);
  return_value_if_fail(!"fs_os_get_disk_info not supported yet", RET_BAD_PARAMS);

  *free_kb = 0;
  *total_kb = 0;
  return RET_NOT_IMPL;
}

static ret_t fs_os_get_exe(fs_t* fs, char path[MAX_PATH + 1]) {
  (void)fs;

  tk_strcpy(path, TKC_FS_EXE_PATH);
  if (!path_exist(path)) {
    fs_create_dir_r(os_fs(), path);
    tk_log_info_lf("create dir '%s'", path);
  }
  return_value_if_fail(path_exist(path), RET_FAIL);
  path[tk_strlen(path)] = '/';
  path[tk_strlen(path) + 1] = '\0';
  return RET_OK;
}

static ret_t fs_os_get_cwd(fs_t* fs, char path[MAX_PATH + 1]) {
  (void)fs;
  return_value_if_fail(path, RET_BAD_PARAMS);

  *path = '\0';
  // return aw_getcwd(path, MAX_PATH) != NULL ? RET_OK : RET_FAIL;
  return RET_OK;
}

static ret_t fs_os_get_temp_path(fs_t* fs, char path[MAX_PATH + 1]) {
  tk_strcpy(path, TKC_FS_TEMP_PATH);
  if (!path_exist(path)) {
    fs_create_dir_r(os_fs(), path);
    tk_log_info_lf("create dir '%s'", path);
  }
  return_value_if_fail(path_exist(path), RET_FAIL);
  path[tk_strlen(path)] = '/';
  path[tk_strlen(path) + 1] = '\0';
  return RET_OK;
}

static ret_t fs_os_get_user_storage_path(fs_t* fs, char path[MAX_PATH + 1]) {
  tk_strcpy(path, TKC_FS_USER_STORAGE_PATH);
  if (!path_exist(path)) {
    fs_create_dir_r(os_fs(), path);
    tk_log_info_lf("create dir '%s'", path);
  }
  return_value_if_fail(path_exist(path), RET_FAIL);
  path[tk_strlen(path)] = '/';
  path[tk_strlen(path) + 1] = '\0';
  return RET_OK;
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
    .get_user_storage_path = fs_os_get_user_storage_path,
};
/*----------------------------------------------------------------------------*/
#define __MAKE_SYSFILE_NAME "~systemmkiffd.reserved"

#if CONFIG_HC32F4A0_CATNET_BOARD
#define __LFS_DEVNAME "/dev/is25lpx_partition_0"
#define __LFFS_DEVNAME "/dev/is25lpx_partition_1"

fs_t* fs_aworks_create(const char* mnt) {
  int ret;
  struct aw_fs_format_arg fmt = {"awdisk", 0, 1};
  struct aw_stat file_stat;
  char stmp[64];
  memset(stmp, 0, sizeof(stmp));

  if (tk_str_eq("/lffs", mnt)) {
    aw_umount("/lffs", 0);
    ret = aw_mount("/lffs", __LFFS_DEVNAME, AW_LFFS_TYPE_NAME, 0, NULL);
    if (ret == 0) {
      tk_str_append(stmp, sizeof(stmp), mnt);
      tk_str_append(stmp, sizeof(stmp), "/" __MAKE_SYSFILE_NAME);
      ret = aw_stat(stmp, &file_stat);
    }
    if (ret != 0) {
      aw_umount("/lffs", 0);
      aw_make_fs(__LFFS_DEVNAME, AW_LFFS_TYPE_NAME, &fmt);
      ret = aw_mount("/lffs", __LFFS_DEVNAME, AW_LFFS_TYPE_NAME, 0, NULL);
      goto_error_if_fail(ret == 0);
      ret = aw_create("/lffs/" __MAKE_SYSFILE_NAME, AW_S_IRWXU | AW_S_IRWXG | AW_S_IRWXO);
      goto_error_if_fail(ret == 0);
    }
    tk_log_info_lf("mount %s", mnt);
  } else if (tk_str_eq("/lfs", mnt)) {
    ret = aw_mount(mnt, __LFS_DEVNAME, AW_LITTLEFS_TYPE_NAME, 0, NULL);
    if (ret != 0) {
      aw_umount(mnt, 0);
      ret = aw_make_fs(__LFS_DEVNAME, AW_LITTLEFS_TYPE_NAME, &fmt);
      if (ret != AW_OK) {
        ret = aw_make_fs(__LFS_DEVNAME, AW_LITTLEFS_TYPE_NAME, NULL);
        goto_error_if_fail(ret == 0);
      }
      ret = aw_mount(mnt, __LFS_DEVNAME, AW_LITTLEFS_TYPE_NAME, 0, NULL);
      goto_error_if_fail(ret == 0);
    }
    tk_log_info_lf("mount %s", mnt);
  } else if (tk_str_eq("/dev", mnt)) {
    tk_log_info_lf("mount %s", mnt);
    return &s_os_fs;
  } else {
    return_value_if_fail(!"Not support the mnt!", NULL);
  }
  return &s_os_fs;

error:
  aw_umount(mnt, 0);
  return NULL;
}

ret_t fs_aworks_init(void) {
  return fs_os_register("/lfs", fs_aworks_create); /* littlefs */
}

#elif CONFIG_M105X_EV_BOARD_LP
#define __LFS_DEVNAME "/dev/spiflash"

fs_t* fs_aworks_create(const char* mnt) {
  int ret;
  struct aw_fs_format_arg fmt = {"awdisk", 0, 1};
  struct aw_stat file_stat;
  char stmp[64];
  memset(stmp, 0, sizeof(stmp));

  if (tk_str_eq("/lfs", mnt)) {
    ret = aw_mount(mnt, __LFS_DEVNAME, AW_LITTLEFS_TYPE_NAME, 0, NULL);
    if (ret != 0) {
      aw_umount(mnt, 0);
      ret = aw_make_fs(__LFS_DEVNAME, AW_LITTLEFS_TYPE_NAME, &fmt);
      if (ret != AW_OK) {
        ret = aw_make_fs(__LFS_DEVNAME, AW_LITTLEFS_TYPE_NAME, NULL);
        goto_error_if_fail(ret == 0);
      }
      ret = aw_mount(mnt, __LFS_DEVNAME, AW_LITTLEFS_TYPE_NAME, 0, NULL);
      goto_error_if_fail(ret == 0);
    }
    tk_log_info_lf("mount %s", mnt);
  } else if (tk_str_eq("/dev", mnt)) {
    tk_log_info_lf("mount %s", mnt);
    return &s_os_fs;
  } else {
    return_value_if_fail(!"Not support the mnt!", NULL);
  }
  return &s_os_fs;

error:
  aw_umount(mnt, 0);
  return NULL;
}

ret_t fs_aworks_init(void) {
  return fs_os_register("/lfs", fs_aworks_create); /* littlefs */
}

#elif CONFIG_PLATFORM_UIS8910DM_AWORKS_LP

fs_t* fs_aworks_create(const char* mnt) {
  int ret;
  struct aw_fs_format_arg fmt = {"awdisk", 0, 1};
  struct aw_stat file_stat;
  char stmp[64];
  memset(stmp, 0, sizeof(stmp));

  if (tk_str_eq("/sd0", mnt)) {
    /* ZC1上插入了SD卡，板子启动后默认的会进行fatfs文件系统进行挂载，且挂载点为"/sd0" */
    tk_log_info_lf("mount %s", mnt);
    return &s_os_fs;
  } else if (tk_str_eq("/dev", mnt)) {
    tk_log_info_lf("mount %s", mnt);
    return &s_os_fs;
  } else {
    return_value_if_fail(!"Not support the mnt!", NULL);
  }
  return &s_os_fs;

error:
  return NULL;
}

#if defined(UTEST_ENABLE)
void fs_aworks_utest(void) {
  char* p;
  uint32_t sz;
  if (!dir_exist("/lfs/test_dir")) {
    fs_create_dir(os_fs(), "/lfs/test_dir");
  }
  ENSURE(file_write("/lfs/test_dir/tkc_test", "abc123456", tk_strlen("abc123456") + 1) == RET_OK);
  p = file_read("/lfs/test_dir/tkc_test", &sz);
  ENSURE(p && sz >= tk_strlen("abc123456") + 1 &&
         tk_str_eq_with_len(p, "abc123456", tk_strlen("abc123456") + 1));
  TKMEM_FREE(p);

  tk_log_info_htlf("[UTEST]", "test R/W \"/lfs\" pass.");
}
#else
void fs_aworks_utest(void) {
}
#endif /* defined(UTEST_ENABLE) */

ret_t fs_aworks_init(void) {
  return_value_if_fail(fs_os_register("/sd0", fs_aworks_create) == RET_OK, RET_FAIL);
  fs_aworks_utest();
  return RET_OK;
}

#else
fs_t* fs_aworks_create(const char* mnt) {
  return NULL;
}

ret_t fs_aworks_init(void) {
  return RET_FAIL;
}
#endif
