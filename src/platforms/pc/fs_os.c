#include "tkc/types_def.h"

#ifdef ANDROID
#include "SDL.h"
#endif /*ANDROID*/

#if defined(__APPLE__) || defined(LINUX)
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#elif defined(WIN32)
#include <stdio.h>
#include <windows.h>
#include <io.h>
#include <direct.h>
#include <Shlobj.h>
#define unlink _unlink
#define rename MoveFileA
#define ftruncate _chsize
#include "./dirent.inc"
#include "tkc/utf8.h"
#include "tkc/mem.h"
#endif

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#endif /*__APPLE__*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/utils.h"

static int32_t fs_os_file_read(fs_file_t* file, void* buffer, uint32_t size) {
  FILE* fp = (FILE*)(file->data);

  return (int32_t)fread(buffer, 1, size, fp);
}

int32_t fs_os_file_write(fs_file_t* file, const void* buffer, uint32_t size) {
  FILE* fp = (FILE*)(file->data);

  return fwrite(buffer, 1, size, fp);
}

int32_t fs_os_file_printf(fs_file_t* file, const char* const format_str, va_list vl) {
  FILE* fp = (FILE*)(file->data);

  return vfprintf(fp, format_str, vl);
}

ret_t fs_os_file_seek(fs_file_t* file, int32_t offset) {
  FILE* fp = (FILE*)(file->data);

  return fseek(fp, offset, SEEK_SET) == 0 ? RET_OK : RET_FAIL;
}

ret_t fs_os_file_truncate(fs_file_t* file, int32_t size) {
  FILE* fp = (FILE*)(file->data);

  return ftruncate(fileno(fp), size) == 0 ? RET_OK : RET_FAIL;
}

bool_t fs_os_file_eof(fs_file_t* file) {
  FILE* fp = (FILE*)(file->data);

  return feof(fp) != 0;
}

ret_t fs_os_file_close(fs_file_t* file) {
  FILE* fp = (FILE*)(file->data);
  fclose(fp);
  TKMEM_FREE(file);

  return RET_OK;
}

ret_t fs_os_dir_rewind(fs_dir_t* dir) {
  DIR* d = (DIR*)(dir->data);

  rewinddir(d);

  return RET_OK;
}

ret_t fs_os_dir_read(fs_dir_t* dir, fs_item_t* item) {
  DIR* d = (DIR*)(dir->data);
  struct dirent* ent = readdir(d);

  memset(item, 0x00, sizeof(fs_item_t));
  if (ent != NULL) {
    uint8_t type = ent->d_type;
    item->is_dir = (type & DT_DIR) != 0;
    item->is_link = (type & DT_LNK) != 0;
    item->is_reg_file = (type & DT_REG) != 0;
#ifdef WIN32
    str_t str;
    str_init(&str, wcslen(ent->d_name) * 4 + 1);
    str_from_wstr_with_len(&str, ent->d_name, wcslen(ent->d_name));
    tk_strncpy(item->name, str.str, MAX_PATH);
    str_reset(&str);
#else
    tk_strncpy(item->name, ent->d_name, MAX_PATH);
#endif
    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

ret_t fs_os_dir_close(fs_dir_t* dir) {
  DIR* d = (DIR*)dir->data;
  closedir(d);
  TKMEM_FREE(dir);

  return RET_OK;
}

static fs_file_t* fs_file_create(FILE* fp) {
  fs_file_t* f = NULL;
  return_value_if_fail(fp != NULL, NULL);

  f = TKMEM_ZALLOC(fs_file_t);
  if (f != NULL) {
    f->read = fs_os_file_read;
    f->write = fs_os_file_write;
    f->f_printf = fs_os_file_printf;
    f->seek = fs_os_file_seek;
    f->truncate = fs_os_file_truncate;
    f->eof = fs_os_file_eof;
    f->close = fs_os_file_close;
    f->data = fp;
  } else {
    fclose(fp);
  }

  return f;
}

fs_file_t* fs_os_open_file(fs_t* fs, const char* name, const char* mode) {
  (void)fs;
  return_value_if_fail(name != NULL && mode != NULL, NULL);
#ifdef WIN32
  int16_t len = 0;
  fs_file_t* file = NULL;
  wchar_t* w_name = NULL;
  wchar_t* w_mode = NULL;

  len = strlen(name) + 1;
  w_name = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(name, w_name, len);

  len = strlen(mode) + 1;
  w_mode = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(mode, w_mode, len);

  file = fs_file_create(_wfopen(w_name, w_mode));
  TKMEM_FREE(w_name);
  TKMEM_FREE(w_mode);
  return file;
#else
  return fs_file_create(fopen(name, mode));
#endif
}

ret_t fs_os_remove_file(fs_t* fs, const char* name) {
  (void)fs;
  return_value_if_fail(name != NULL, RET_FAIL);

#ifdef WIN32
  int16_t len = 0;
  wchar_t* w_name = NULL;

  len = strlen(name) + 1;
  w_name = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(name, w_name, len);

  _wunlink(w_name);
  TKMEM_FREE(w_name);
#else
  unlink(name);
#endif
  return RET_OK;
}

bool_t fs_os_file_exist(fs_t* fs, const char* name) {
  (void)fs;
  return_value_if_fail(name != NULL, FALSE);

#ifdef WIN32
  bool_t rtn = FALSE;
  int16_t len = 0;
  wchar_t* w_name = NULL;
  struct _stat64i32 st;

  len = strlen(name) + 1;
  w_name = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(name, w_name, len);

  rtn = (_wstat(w_name, &st) == 0 && st.st_mode & S_IFREG);
  TKMEM_FREE(w_name);
  return rtn;
#else
  struct stat st;
  return (stat(name, &st) == 0 && st.st_mode & S_IFREG);
#endif
}

bool_t fs_os_file_rename(fs_t* fs, const char* name, const char* new_name) {
  (void)fs;
  return_value_if_fail(name != NULL && new_name != NULL, FALSE);

#ifdef WIN32
  bool_t rtn = FALSE;
  int16_t len = 0;
  wchar_t* w_name = NULL;
  wchar_t* w_new_name = NULL;

  len = strlen(name) + 1;
  w_name = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(name, w_name, len);

  len = strlen(new_name) + 1;
  w_new_name = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(new_name, w_new_name, len);

  rtn = _wrename(w_name, w_new_name) == 0;
  TKMEM_FREE(w_name);
  TKMEM_FREE(w_new_name);
  return rtn;
#else
  return rename(name, new_name) == 0;
#endif
}

fs_dir_t* fs_dir_create(DIR* dir) {
  fs_dir_t* d = NULL;
  return_value_if_fail(dir != NULL, NULL);

  d = TKMEM_ZALLOC(fs_dir_t);
  if (d != NULL) {
    d->read = fs_os_dir_read;
    d->rewind = fs_os_dir_rewind;
    d->close = fs_os_dir_close;
    d->data = dir;
  } else {
    closedir(dir);
  }

  return d;
}

fs_dir_t* fs_os_open_dir(fs_t* fs, const char* name) {
  (void)fs;
  return_value_if_fail(name != NULL, NULL);
#ifdef WIN32
  int16_t len = 0;
  wchar_t* w_name = NULL;
  fs_dir_t* pDir = NULL;

  len = strlen(name) + 1;
  w_name = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(name, w_name, len);

  pDir = fs_dir_create(opendir(w_name));
  TKMEM_FREE(w_name);
  return pDir;
#else
  return fs_dir_create(opendir(name));
#endif
}

ret_t fs_os_remove_dir(fs_t* fs, const char* name) {
  (void)fs;
  return_value_if_fail(name != NULL, RET_FAIL);
#if defined(WIN32)
  wchar_t* w_name = NULL;
  int16_t len = strlen(name) + 1;
  w_name = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(name, w_name, len);
  int8_t ret = _wrmdir(w_name);
  TKMEM_FREE(w_name);
  if (ret == 0) {
#else
  if (rmdir(name) == 0) {
#endif
    return RET_OK;
  } else {
    perror(name);
    return RET_FAIL;
  }
}

ret_t fs_os_create_dir(fs_t* fs, const char* name) {
  (void)fs;
  return_value_if_fail(name != NULL, RET_FAIL);
#if defined(WIN32)
  wchar_t* w_name = NULL;
  int16_t len = strlen(name) + 1;
  w_name = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(name, w_name, len);
  int8_t ret = _wmkdir(w_name);
  TKMEM_FREE(w_name);
  if (ret == 0) {
#else
  if (mkdir(name, 0755) == 0) {
#endif
    return RET_OK;
  } else {
    perror(name);
    return RET_FAIL;
  }
}

bool_t fs_os_dir_exist(fs_t* fs, const char* name) {
  (void)fs;
  return_value_if_fail(name != NULL, FALSE);
#ifdef WIN32
  struct _stat64i32 st;
  bool_t rtn = FALSE;
  int16_t len = 0;
  wchar_t* w_name = NULL;

  len = strlen(name) + 1;
  w_name = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(name, w_name, len);

  rtn = (_wstat(w_name, &st) == 0 && st.st_mode & S_IFDIR);
  TKMEM_FREE(w_name);
  return rtn;
#else
  struct stat st;
  return (stat(name, &st) == 0 && st.st_mode & S_IFDIR);
#endif
}

bool_t fs_os_dir_rename(fs_t* fs, const char* name, const char* new_name) {
  (void)fs;
  (void)name;
  (void)new_name;
  assert(!"fs_os_dir_rename not supported yet");

  return FALSE;
}

int32_t fs_os_get_file_size(fs_t* fs, const char* name) {
  (void)fs;
  return_value_if_fail(name != NULL, -1);
#ifdef WIN32
  struct _stat64i32 st;
  bool_t rtn = FALSE;
  int16_t len = 0;
  wchar_t* w_name = NULL;

  len = strlen(name) + 1;
  w_name = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(name, w_name, len);

  int n = _wstat(w_name, &st);
  rtn = n == 0;
  TKMEM_FREE(w_name);
  if (!rtn) {
    return -1;
  }
  return st.st_size;
#else
  struct stat st;
  return_value_if_fail(stat(name, &st) == 0, -1);
  return st.st_size;
#endif
}

ret_t fs_os_get_disk_info(fs_t* fs, const char* volume, int32_t* free_kb, int32_t* total_kb) {
  /*TODO*/
  *free_kb = 0;
  *total_kb = 0;
  (void)fs;
  assert(!"fs_os_get_disk_info not supported yet");

  return RET_FAIL;
}

ret_t fs_os_get_exe(fs_t* fs, char path[MAX_PATH + 1]) {
  uint32_t size = MAX_PATH;
  (void)fs;

#if defined(LINUX)
  size = readlink("/proc/self/exe", path, MAX_PATH);
  if (size >= 0) {
    path[size] = '\0';
    return RET_OK;
  } else {
    *path = '\0';
    return RET_FAIL;
  }
#elif defined(WIN32)
  (void)size;
  GetModuleFileNameA(GetModuleHandle(NULL), path, MAX_PATH);
#elif defined(__APPLE__)
  _NSGetExecutablePath(path, &size);
  assert(size <= MAX_PATH);
  path[size] = '\0';
#endif

  return RET_OK;
}

ret_t fs_os_get_user_storage_path(fs_t* fs, char path[MAX_PATH + 1]) {
#if defined(ANDROID)
  const char* homedir = SDL_AndroidGetInternalStoragePath();
  memset(path, 0x00, MAX_PATH + 1);

  return_value_if_fail(homedir != NULL, RET_FAIL);
  tk_strncpy(path, homedir, MAX_PATH);

  return RET_OK;
#elif defined(LINUX) || defined(__APPLE__) || defined(IOS)
  const char* homedir = NULL;
  memset(path, 0x00, MAX_PATH + 1);

  if ((homedir = getenv("HOME")) == NULL) {
    homedir = getpwuid(getuid())->pw_dir;
  }

  return_value_if_fail(homedir != NULL, RET_FAIL);
  tk_strncpy(path, homedir, MAX_PATH);

  return RET_OK;
#elif defined(WIN32)
  WCHAR homedir[MAX_PATH];

  if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, homedir))) {
    str_t str;
    str_init(&str, MAX_PATH);
    str_from_wstr(&str, homedir);
    tk_strncpy(path, str.str, MAX_PATH);
    str_reset(&str);

    return RET_OK;
  }
#endif

  return RET_FAIL;
}

static ret_t fs_os_get_cwd(fs_t* fs, char path[MAX_PATH + 1]) {
  return_value_if_fail(fs != NULL && path != NULL, RET_BAD_PARAMS);

  memset(path, 0x00, MAX_PATH + 1);
#if defined(WIN32)
  GetCurrentDirectory(MAX_PATH, path);
#else
  getcwd(path, MAX_PATH);
#endif /*WIN32*/

  return RET_OK;
}

ret_t fs_os_stat(fs_t* fs, const char* name, fs_stat_info_t* fst) {
  (void)fs;
  return_value_if_fail(name != NULL && fst != NULL, RET_BAD_PARAMS);

  int stat_ret = 0;

#ifdef WIN32
  struct _stat64i32 st;
  int16_t len = 0;
  wchar_t* w_name = NULL;

  len = strlen(name) + 1;
  w_name = (wchar_t*)TKMEM_ALLOC(len * 2);
  tk_utf8_to_utf16(name, w_name, len);

  stat_ret = _wstat(w_name, &st);
  TKMEM_FREE(w_name);
#else
  struct stat st;
  stat_ret = stat(name, &st);
#endif

  if (stat_ret == -1) {
    return RET_FAIL;
  } else {
    fst->dev = st.st_dev;
    fst->ino = st.st_ino;
    fst->mode = st.st_mode;
    fst->nlink = st.st_nlink;
    fst->uid = st.st_uid;
    fst->gid = st.st_gid;
    fst->rdev = st.st_rdev;
    fst->size = st.st_size;
    fst->atime = st.st_atime;
    fst->mtime = st.st_mtime;
    fst->ctime = st.st_ctime;
    fst->is_dir = (st.st_mode & S_IFDIR) != 0;
#ifdef S_IFLNK
    fst->is_link = (st.st_mode & S_IFLNK) != 0;
#else
    fst->is_link = FALSE;
#endif /*S_IFLNK*/
    fst->is_reg_file = (st.st_mode & S_IFREG) != 0;
  }

  return RET_OK;
}

static const fs_t s_os_fs = {.open_file = fs_os_open_file,
                             .remove_file = fs_os_remove_file,
                             .file_exist = fs_os_file_exist,
                             .file_rename = fs_os_file_rename,

                             .open_dir = fs_os_open_dir,
                             .remove_dir = fs_os_remove_dir,
                             .create_dir = fs_os_create_dir,
                             .dir_exist = fs_os_dir_exist,
                             .dir_rename = fs_os_dir_rename,

                             .get_file_size = fs_os_get_file_size,
                             .get_disk_info = fs_os_get_disk_info,
                             .get_cwd = fs_os_get_cwd,
                             .get_exe = fs_os_get_exe,
                             .get_user_storage_path = fs_os_get_user_storage_path,
                             .stat = fs_os_stat};

fs_t* os_fs(void) {
  return (fs_t*)&s_os_fs;
}
