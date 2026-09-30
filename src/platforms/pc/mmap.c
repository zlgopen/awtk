/**
 * File:   mmap.c
 * Author: AWTK Develop Team
 * Brief:  mmap
 *
 * Copyright (c) 2018 - 2026 Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is dimmapibuted in the hope that it will be useful,
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

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/mmap.h"
#include "tkc/utf8.h"

#ifndef WIN32
#include <errno.h>
#include <sys/types.h>
#include <sys/mman.h>
#endif /*WIN32*/

mmap_t* mmap_create(const char* filename, bool_t writable, bool_t shared) {
  uint32_t size = 0;
  fs_stat_info_t st;
  mmap_t* map = NULL;
  return_value_if_fail(filename != NULL, NULL);

  return_value_if_fail(fs_stat(os_fs(), filename, &st) == RET_OK, NULL);
  return_value_if_fail(st.is_reg_file, NULL);
  return_value_if_fail(0 < st.size && st.size <= UINT32_MAX, NULL);
  size = (uint32_t)st.size;

  map = TKMEM_ZALLOC(mmap_t);
  return_value_if_fail(map != NULL, NULL);

  map->size = size;
  map->writable = writable;
  map->shared = shared;

#ifdef WIN32
  DWORD err = 0;
  HANDLE hFile = INVALID_HANDLE_VALUE;
  HANDLE handle = INVALID_HANDLE_VALUE;
  /* CreateFileW 用 GENERIC_*，CreateFileMapping 用 PAGE_*，MapViewOfFile 用 FILE_MAP_* */
  DWORD flProtect = writable ? PAGE_READWRITE : PAGE_READONLY;
  DWORD flFileAccess = writable ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_READ;
  DWORD flMapAccess = shared ? (writable ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ) : FILE_MAP_COPY;
  wchar_t wfilename[MAX_PATH + 1];

  tk_utf8_to_utf16(filename, wfilename, MAX_PATH);
  hFile = CreateFileW(wfilename, flFileAccess, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  goto_error_if_fail(hFile != INVALID_HANDLE_VALUE);

  handle = CreateFileMapping(hFile, NULL, flProtect, 0, size, NULL);
  goto_error_if_fail(handle != NULL);

  map->fd = (void*)hFile;
  map->handle = (void*)handle;

  map->data = MapViewOfFile(handle, flMapAccess, 0, 0, size);
  goto_error_if_fail(map->data != NULL);

  return map;
error:
  err = GetLastError();
  log_debug("err=%u\n", err);
  if (handle != INVALID_HANDLE_VALUE) {
    CloseHandle(handle);
  }
  if (hFile != INVALID_HANDLE_VALUE) {
    CloseHandle(hFile);
  }
  TKMEM_FREE(map);
  return NULL;
#else
  int protect = writable ? PROT_WRITE | PROT_READ : PROT_READ;
  int flags = writable ? O_RDWR : O_RDONLY;
  int fd = open(filename, flags);
  goto_error_if_fail(fd >= 0);

  map->fd = tk_pointer_from_int(fd);

  flags = MAP_FILE | (shared ? MAP_SHARED : MAP_PRIVATE);
  map->data = mmap(NULL, size, protect, flags, fd, 0);
  goto_error_if_fail(map->data != MAP_FAILED);

  return map;
error:
  if (fd >= 0) {
    close(fd);
  }
  TKMEM_FREE(map);
  return NULL;
#endif /*WIN32*/
}

/* 把映射区的改动刷到存储上 */
inline static ret_t mmap_flush(mmap_t* map) {
  return_value_if_fail(map != NULL && map->data != NULL, RET_BAD_PARAMS);

#ifdef WIN32
  return FlushViewOfFile(map->data, map->size) ? RET_OK : RET_FAIL;
#else
  return msync(map->data, map->size, MS_SYNC) == 0 ? RET_OK : RET_FAIL;
#endif /*WIN32*/
}

ret_t mmap_destroy(mmap_t* map) {
  return_value_if_fail(map != NULL, RET_BAD_PARAMS);

  if (map->writable && map->shared) {
    if (mmap_flush(map) != RET_OK) {
      log_debug("mmap_flush failed\n");
    }
  }

#ifdef WIN32
  HANDLE fd = (HANDLE)(map->fd);
  HANDLE handle = (HANDLE)(map->handle);

  if (map->data != NULL) {
    if (!UnmapViewOfFile(map->data)) {
      log_debug("UnmapViewOfFile failed: %u\n", GetLastError());
    }
  }
  if (fd != INVALID_HANDLE_VALUE) {
    CloseHandle(fd);
  }
  if (handle != INVALID_HANDLE_VALUE) {
    CloseHandle(handle);
  }
#else
  int fd = tk_pointer_to_int(map->fd);
  if (map->data != NULL) {
    if (munmap(map->data, map->size) != 0) {
      log_debug("munmap failed: %d\n", errno);
    }
  }
  if (fd >= 0) {
    close(fd);
  }
#endif /*WIN32*/

  TKMEM_FREE(map);
  return RET_OK;
}
