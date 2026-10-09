/**
 * File:   mmap.c
 * Author: AWTK Develop Team
 * Brief:  mmap
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-12-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/mmap.h"

#include <string.h>

#ifdef WIN32
#include "tkc/utf8.h"
#else
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#endif /*WIN32*/

/**
 * @enum mmap_mode_t
 * @prefix MMAP_MODE_
 * mmap 模式。
 */
typedef enum _mmap_mode_t {
  /**
   * @const MMAP_MODE_FILE
   * 文件映射。
   */
  MMAP_MODE_FILE = 0,
} mmap_mode_t;

/**
 * @struct mmap_opt_t
 * mmap 选项。
 *
 * writable 为 TRUE 且 shared 为 TRUE：写入对其它共享映射可见，mmap_destroy 时刷回文件。
 * writable 为 TRUE 且 shared 为 FALSE：写时复制，只影响当前映射，不回写文件。
 * writable 为 FALSE：只读。shared 只决定是否与其它映射共享物理页。
 */
typedef struct _mmap_opt_t {
  /**
   * @property {bool_t} writable
   * 是否可写。
   */
  bool_t writable;
  /**
   * @property {bool_t} shared
   * 是否共享。(TRUE: 写回写；FALSE: COW 私有写)
   */
  bool_t shared;
} mmap_opt_t;

typedef struct _mmap_impl_t {
  mmap_t map;
  mmap_mode_t mode;
  mmap_opt_t opt;
  void* handle;
  void* fd;
} mmap_impl_t;

inline static bool_t mmap_size_valid(int64_t size) {
  return 0 < size && size <= UINT32_MAX;
}

inline static mmap_impl_t* mmap_alloc(uint32_t size, mmap_mode_t mode, const mmap_opt_t* opt) {
  mmap_impl_t* map = TKMEM_ZALLOC(mmap_impl_t);
  return_value_if_fail(map != NULL, NULL);

  map->map.size = size;
  map->mode = mode;
  if (opt != NULL) {
    map->opt = *opt;
  }

  return map;
}

#ifdef WIN32
/* 缓冲区按字节数分配。合法 UTF-8 的 wchar 个数不会超过字节数，因此不会截断。 */
static wchar_t* mmap_filename_to_utf16(const char* filename) {
  wchar_t* wfilename = NULL;
  uint32_t len = tk_strlen(filename);
  return_value_if_fail(len < UINT32_MAX, NULL);

  len += 1;

  wfilename = (wchar_t*)TKMEM_ALLOC(sizeof(wchar_t) * len);
  return_value_if_fail(wfilename != NULL, NULL);

  goto_error_if_fail(NULL != tk_utf8_to_utf16(filename, wfilename, len));

  return wfilename;
error:
  TKMEM_FREE(wfilename);
  return NULL;
}

static mmap_t* mmap_create_file(const char* filename, const mmap_opt_t* opt) {
  uint32_t size = 0;
  mmap_impl_t* map = NULL;
  wchar_t* wfilename = NULL;
  HANDLE file = INVALID_HANDLE_VALUE;
  HANDLE mapping = NULL;
  LARGE_INTEGER file_size;
  DWORD protect = PAGE_READONLY;
  DWORD file_access = GENERIC_READ;
  DWORD map_access = FILE_MAP_READ;
  return_value_if_fail(filename != NULL && opt != NULL, NULL);

  file_size.QuadPart = 0;
  if (opt->writable) {
    file_access = GENERIC_READ | GENERIC_WRITE;
    if (opt->shared) {
      protect = PAGE_READWRITE;
      /* FILE_MAP_ALL_ACCESS 含 FILE_MAP_EXECUTE，PAGE_READWRITE 映射会失败。 */
      map_access = FILE_MAP_READ | FILE_MAP_WRITE;
    } else {
      protect = PAGE_WRITECOPY;
      map_access = FILE_MAP_COPY;
    }
  }

  wfilename = mmap_filename_to_utf16(filename);
  return_value_if_fail(wfilename != NULL, NULL);

  file = CreateFileW(wfilename, file_access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  TKMEM_FREE(wfilename);

  goto_error_if_fail(INVALID_HANDLE_VALUE != file);
  goto_error_if_fail(FILE_TYPE_DISK == GetFileType(file));
  goto_error_if_fail(GetFileSizeEx(file, &file_size));
  goto_error_if_fail(mmap_size_valid((int64_t)file_size.QuadPart));

  size = (uint32_t)file_size.QuadPart;

  mapping = CreateFileMappingW(file, NULL, protect, 0, size, NULL);
  goto_error_if_fail(mapping != NULL);

  map = mmap_alloc(size, MMAP_MODE_FILE, opt);
  goto_error_if_fail(map != NULL);

  map->fd = (void*)file;
  map->handle = (void*)mapping;

  map->map.data = MapViewOfFile(mapping, map_access, 0, 0, size);
  goto_error_if_fail(map->map.data != NULL);

  return &map->map;
error:
  log_warn("%s failed: err code = %u\n", __FUNCTION__, GetLastError());
  if (mapping != NULL) {
    CloseHandle(mapping);
  }
  if (file != NULL && file != INVALID_HANDLE_VALUE) {
    CloseHandle(file);
  }
  TKMEM_FREE(map);
  return NULL;
}

inline static ret_t mmap_flush(mmap_t* map) {
  mmap_impl_t* impl = (mmap_impl_t*)map;
  return_value_if_fail(map != NULL && map->data != NULL, RET_BAD_PARAMS);

  goto_error_if_fail(FlushViewOfFile(map->data, map->size));
  goto_error_if_fail(FlushFileBuffers((HANDLE)impl->fd));

  return RET_OK;
error:
  log_warn("%s failed: err code = %u\n", __FUNCTION__, GetLastError());
  return RET_FAIL;
}

inline static ret_t mmap_close_handles(mmap_t* map) {
  ret_t ret = RET_OK;
  mmap_impl_t* impl = (mmap_impl_t*)map;
  HANDLE file = (HANDLE)impl->fd;
  HANDLE mapping = (HANDLE)impl->handle;

  if (map->data != NULL) {
    if (!UnmapViewOfFile(map->data)) {
      ret = RET_FAIL;
    }
  }
  if (mapping != NULL) {
    if (!CloseHandle(mapping)) {
      ret = RET_FAIL;
    }
  }
  if (file != NULL && file != INVALID_HANDLE_VALUE) {
    if (!CloseHandle(file)) {
      ret = RET_FAIL;
    }
  }

  if (RET_OK != ret) {
    log_warn("%s failed: err code = %u\n", __FUNCTION__, GetLastError());
  }
  return ret;
}
#else
static mmap_t* mmap_create_file(const char* filename, const mmap_opt_t* opt) {
  int fd = -1;
  uint32_t size = 0;
  struct stat st;
  mmap_impl_t* map = NULL;
  int protect, open_flags, map_flags;
  return_value_if_fail(filename != NULL && opt != NULL, NULL);

  protect = opt->writable ? (PROT_READ | PROT_WRITE) : PROT_READ;
  open_flags = (opt->writable ? O_RDWR : O_RDONLY) | O_CLOEXEC;
  map_flags = MAP_FILE | (opt->shared ? MAP_SHARED : MAP_PRIVATE);

  memset(&st, 0x00, sizeof(st));

  fd = open(filename, open_flags);
  goto_error_if_fail(fd >= 0);
  goto_error_if_fail(0 == fstat(fd, &st));
  goto_error_if_fail(S_ISREG(st.st_mode));
  goto_error_if_fail(mmap_size_valid((int64_t)st.st_size));

  size = (uint32_t)st.st_size;

  map = mmap_alloc(size, MMAP_MODE_FILE, opt);
  goto_error_if_fail(map != NULL);

  map->fd = tk_pointer_from_int(fd);

  map->map.data = mmap(NULL, size, protect, map_flags, fd, 0);
  goto_error_if_fail(map->map.data != MAP_FAILED);

  return &map->map;
error:
  log_warn("%s failed: err code = %d\n", __FUNCTION__, errno);
  if (fd >= 0) {
    close(fd);
  }
  TKMEM_FREE(map);
  return NULL;
}

inline static ret_t mmap_flush(mmap_t* map) {
  return_value_if_fail(map != NULL && map->data != NULL, RET_BAD_PARAMS);

  goto_error_if_fail(0 == msync(map->data, map->size, MS_SYNC));

  return RET_OK;
error:
  log_warn("%s failed: err code = %d\n", __FUNCTION__, errno);
  return RET_FAIL;
}

inline static ret_t mmap_close_handles(mmap_t* map) {
  ret_t ret = RET_OK;
  mmap_impl_t* impl = (mmap_impl_t*)map;
  int fd = tk_pointer_to_int(impl->fd);

  if (map->data != NULL) {
    if (0 != munmap(map->data, map->size)) {
      ret = RET_FAIL;
    }
  }
  if (fd >= 0) {
    if (0 != close(fd)) {
      ret = RET_FAIL;
    }
  }

  if (RET_OK != ret) {
    log_warn("%s failed: err code = %d\n", __FUNCTION__, errno);
  }
  return ret;
}
#endif /*WIN32*/

static mmap_t* mmap_create_ex(const char* name, mmap_mode_t mode, const mmap_opt_t* opt) {
  mmap_opt_t _opt;
  return_value_if_fail(name != NULL, NULL);

  if (NULL == opt) {
    memset(&_opt, 0, sizeof(_opt));
    opt = &_opt;
  }

  switch (mode) {
    case MMAP_MODE_FILE:
      return mmap_create_file(name, opt);
    default:
      log_warn("%s: unsupported mmap mode %d\n", __FUNCTION__, mode);
      return NULL;
  }
}

mmap_t* mmap_create(const char* filename, bool_t writable, bool_t shared) {
  mmap_opt_t opt = {.writable = writable, .shared = shared};
  return mmap_create_ex(filename, MMAP_MODE_FILE, &opt);
}

ret_t mmap_destroy(mmap_t* map) {
  ret_t ret = RET_OK;
  mmap_impl_t* impl = (mmap_impl_t*)map;
  return_value_if_fail(map != NULL, RET_BAD_PARAMS);

  if (MMAP_MODE_FILE == impl->mode && impl->opt.writable && impl->opt.shared) {
    if (RET_OK != mmap_flush(map)) {
      ret = RET_FAIL;
    }
  }
  if (RET_OK != mmap_close_handles(map)) {
    ret = RET_FAIL;
  }

  TKMEM_FREE(map);
  return ret;
}
