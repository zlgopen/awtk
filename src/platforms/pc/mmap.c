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

#ifdef WIN32
/* 缓冲区按字节数分配。合法 UTF-8 的 wchar 个数不会超过字节数，因此不会截断。 */
static wchar_t* mmap_filename_to_utf16(const char* filename) {
  uint32_t len = 0;
  wchar_t* wfilename = NULL;

  len = (uint32_t)strlen(filename);
  if (len == UINT32_MAX) {
    return NULL;
  }
  len += 1;

  wfilename = (wchar_t*)TKMEM_ALLOC(sizeof(wchar_t) * len);
  return_value_if_fail(wfilename != NULL, NULL);

  if (tk_utf8_to_utf16(filename, wfilename, len) == NULL) {
    TKMEM_FREE(wfilename);
    return NULL;
  }

  return wfilename;
}

static mmap_t* mmap_create_impl(const char* filename, bool_t writable, bool_t shared) {
  DWORD err = 0;
  uint32_t size = 0;
  mmap_t* map = NULL;
  wchar_t* wfilename = NULL;
  HANDLE file = INVALID_HANDLE_VALUE;
  HANDLE mapping = NULL;
  LARGE_INTEGER file_size;
  DWORD protect = PAGE_READONLY;
  DWORD file_access = GENERIC_READ;
  DWORD map_access = FILE_MAP_READ;

  file_size.QuadPart = 0;
  if (writable) {
    file_access = GENERIC_READ | GENERIC_WRITE;
    if (shared) {
      protect = PAGE_READWRITE;
      /* FILE_MAP_ALL_ACCESS 含 FILE_MAP_EXECUTE，PAGE_READWRITE 映射会失败。 */
      map_access = FILE_MAP_READ | FILE_MAP_WRITE;
    } else {
      protect = PAGE_WRITECOPY;
      map_access = FILE_MAP_COPY;
    }
  }

  wfilename = mmap_filename_to_utf16(filename);
  if (wfilename == NULL) {
    return NULL;
  }

  file = CreateFileW(wfilename, file_access, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  err = GetLastError();
  TKMEM_FREE(wfilename);
  if (file == INVALID_HANDLE_VALUE) {
    log_warn("mmap CreateFileW failed: %u\n", err);
    return NULL;
  }

  if (GetFileType(file) != FILE_TYPE_DISK) {
    CloseHandle(file);
    return NULL;
  }

  if (!GetFileSizeEx(file, &file_size)) {
    err = GetLastError();
    log_warn("mmap GetFileSizeEx failed: %u\n", err);
    CloseHandle(file);
    return NULL;
  }

  if (file_size.QuadPart <= 0 || (uint64_t)file_size.QuadPart > UINT32_MAX) {
    CloseHandle(file);
    return NULL;
  }
  size = (uint32_t)file_size.QuadPart;

  mapping = CreateFileMappingW(file, NULL, protect, 0, size, NULL);
  if (mapping == NULL) {
    err = GetLastError();
    log_warn("mmap CreateFileMappingW failed: %u\n", err);
    CloseHandle(file);
    return NULL;
  }

  map = TKMEM_ZALLOC(mmap_t);
  if (map == NULL) {
    CloseHandle(mapping);
    CloseHandle(file);
    return NULL;
  }

  map->size = size;
  map->writable = writable;
  map->shared = shared;
  map->fd = (void*)file;
  map->handle = (void*)mapping;
  map->data = MapViewOfFile(mapping, map_access, 0, 0, size);
  if (map->data == NULL) {
    err = GetLastError();
    log_warn("mmap MapViewOfFile failed: %u\n", err);
    CloseHandle(mapping);
    CloseHandle(file);
    TKMEM_FREE(map);
    return NULL;
  }

  return map;
}

static ret_t mmap_flush(mmap_t* map) {
  DWORD err = 0;
  return_value_if_fail(map != NULL && map->data != NULL, RET_BAD_PARAMS);

  if (!FlushViewOfFile(map->data, map->size)) {
    err = GetLastError();
    log_warn("FlushViewOfFile failed: %u\n", err);
    return RET_FAIL;
  }
  if (!FlushFileBuffers((HANDLE)map->fd)) {
    err = GetLastError();
    log_warn("FlushFileBuffers failed: %u\n", err);
    return RET_FAIL;
  }

  return RET_OK;
}

static ret_t mmap_close_handles(mmap_t* map) {
  ret_t ret = RET_OK;
  DWORD err = 0;
  HANDLE file = (HANDLE)map->fd;
  HANDLE mapping = (HANDLE)map->handle;

  if (map->data != NULL && !UnmapViewOfFile(map->data)) {
    err = GetLastError();
    log_warn("UnmapViewOfFile failed: %u\n", err);
    ret = RET_FAIL;
  }
  if (mapping != NULL && !CloseHandle(mapping)) {
    err = GetLastError();
    log_warn("CloseHandle mapping failed: %u\n", err);
    ret = RET_FAIL;
  }
  if (file != NULL && file != INVALID_HANDLE_VALUE && !CloseHandle(file)) {
    err = GetLastError();
    log_warn("CloseHandle file failed: %u\n", err);
    ret = RET_FAIL;
  }

  return ret;
}
#else
static mmap_t* mmap_create_impl(const char* filename, bool_t writable, bool_t shared) {
  int fd = -1;
  int saved_errno = 0;
  uint32_t size = 0;
  struct stat st;
  mmap_t* map = NULL;
  int protect = writable ? (PROT_READ | PROT_WRITE) : PROT_READ;
  int open_flags = (writable ? O_RDWR : O_RDONLY) | O_CLOEXEC;
  int map_flags = MAP_FILE | (shared ? MAP_SHARED : MAP_PRIVATE);

  memset(&st, 0x00, sizeof(st));

  fd = open(filename, open_flags);
  if (fd < 0) {
    saved_errno = errno;
    log_warn("mmap open failed: %d\n", saved_errno);
    return NULL;
  }

  if (fstat(fd, &st) != 0) {
    saved_errno = errno;
    log_warn("mmap fstat failed: %d\n", saved_errno);
    close(fd);
    return NULL;
  }

  if (!S_ISREG(st.st_mode) || st.st_size <= 0 || (uint64_t)st.st_size > UINT32_MAX) {
    close(fd);
    return NULL;
  }
  size = (uint32_t)st.st_size;

  map = TKMEM_ZALLOC(mmap_t);
  if (map == NULL) {
    close(fd);
    return NULL;
  }

  map->size = size;
  map->writable = writable;
  map->shared = shared;
  map->fd = tk_pointer_from_int(fd);
  map->data = mmap(NULL, size, protect, map_flags, fd, 0);
  if (map->data == MAP_FAILED) {
    saved_errno = errno;
    log_warn("mmap failed: %d\n", saved_errno);
    close(fd);
    TKMEM_FREE(map);
    return NULL;
  }

  return map;
}

static ret_t mmap_flush(mmap_t* map) {
  int saved_errno = 0;
  return_value_if_fail(map != NULL && map->data != NULL, RET_BAD_PARAMS);

  if (msync(map->data, map->size, MS_SYNC) != 0) {
    saved_errno = errno;
    log_warn("msync failed: %d\n", saved_errno);
    return RET_FAIL;
  }

  return RET_OK;
}

static ret_t mmap_close_handles(mmap_t* map) {
  ret_t ret = RET_OK;
  int saved_errno = 0;
  int fd = tk_pointer_to_int(map->fd);

  if (map->data != NULL && munmap(map->data, map->size) != 0) {
    saved_errno = errno;
    log_warn("munmap failed: %d\n", saved_errno);
    ret = RET_FAIL;
  }
  if (fd >= 0 && close(fd) != 0) {
    saved_errno = errno;
    log_warn("close mmap fd failed: %d\n", saved_errno);
    ret = RET_FAIL;
  }

  return ret;
}
#endif /*WIN32*/

mmap_t* mmap_create(const char* filename, bool_t writable, bool_t shared) {
  return_value_if_fail(filename != NULL, NULL);

  return mmap_create_impl(filename, writable, shared);
}

ret_t mmap_destroy(mmap_t* map) {
  ret_t ret = RET_OK;
  return_value_if_fail(map != NULL, RET_BAD_PARAMS);

  if (map->writable && map->shared && mmap_flush(map) != RET_OK) {
    ret = RET_FAIL;
  }
  if (mmap_close_handles(map) != RET_OK) {
    ret = RET_FAIL;
  }

  TKMEM_FREE(map);
  return ret;
}
