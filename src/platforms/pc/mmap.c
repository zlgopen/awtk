/**
 * File:   mmap.c
 * Author: AWTK Develop Team
 * Brief:  mmap
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include <sys/types.h>
#include <sys/mman.h>
#endif /*WIN32*/

mmap_t* mmap_create(const char* filename, bool_t writable, bool_t shared) {
#ifdef WIN32
  DWORD err = 0;
  mmap_t* map = NULL;
  wchar_t wfilename[MAX_PATH + 1];
  HANDLE hFile = INVALID_HANDLE_VALUE;
  HANDLE handle = INVALID_HANDLE_VALUE;
  DWORD flProtect = writable ? PAGE_READWRITE : PAGE_READONLY;
  DWORD dwDesiredAccess = writable ? GENERIC_WRITE | GENERIC_READ : GENERIC_READ;
  uint32_t size = file_get_size(filename);
  return_value_if_fail(filename != NULL && size > 0, NULL);

  map = TKMEM_ZALLOC(mmap_t);
  goto_error_if_fail(map != NULL);
  tk_utf8_to_utf16(filename, wfilename, MAX_PATH);
  hFile =
      CreateFileW(wfilename, dwDesiredAccess, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  goto_error_if_fail(hFile != INVALID_HANDLE_VALUE);
  handle = CreateFileMapping(hFile, NULL, flProtect, 0, size, NULL);
  goto_error_if_fail(handle != NULL);
  dwDesiredAccess = writable ? FILE_MAP_ALL_ACCESS : FILE_MAP_READ;

  map->size = size;
  map->fd = (void*)hFile;
  map->handle = (void*)handle;
  map->data = MapViewOfFile(handle, dwDesiredAccess, 0, 0, size);

  return map;
error:
  err = GetLastError();
  log_debug("err=%u\n", err);
  if (hFile != INVALID_HANDLE_VALUE) {
    CloseHandle(hFile);
  }
  if (map != NULL) {
    TKMEM_FREE(map);
  }
  return NULL;
#else
  int fd = 0;
  int size = 0;
  int flags = 0;
  int protect = 0;
  mmap_t* map = NULL;
  return_value_if_fail(filename != NULL, NULL);
  size = file_get_size(filename);
  return_value_if_fail(size > 0, NULL);

  flags = writable ? O_RDWR : O_RDONLY;
  protect = writable ? PROT_WRITE | PROT_READ : PROT_READ;
  fd = open(filename, flags);
  return_value_if_fail(fd >= 0, NULL);

  map = TKMEM_ZALLOC(mmap_t);
  goto_error_if_fail(map != NULL);

  flags = MAP_FILE | (shared ? MAP_SHARED : MAP_PRIVATE);
  map->data = mmap(NULL, size, protect, flags, fd, 0);
  goto_error_if_fail(map->handle != MAP_FAILED);

  map->size = size;
  map->fd = tk_pointer_from_int(fd);
  return map;
error:
  if (fd > 0) {
    close(fd);
  }
  if (map != NULL) {
    TKMEM_FREE(map);
  }
  return NULL;
#endif /*WIN32*/
}

ret_t mmap_destroy(mmap_t* map) {
#ifdef WIN32
  HANDLE fd = INVALID_HANDLE_VALUE;
  HANDLE handle = INVALID_HANDLE_VALUE;
  return_value_if_fail(map != NULL, RET_BAD_PARAMS);

  fd = (HANDLE)(map->fd);
  handle = (HANDLE)(map->handle);

  if (map->data != NULL) {
    UnmapViewOfFile(map->data);
  }

  if (fd != INVALID_HANDLE_VALUE) {
    CloseHandle(fd);
  }

  if (handle != INVALID_HANDLE_VALUE) {
    CloseHandle(handle);
  }

  memset(map, 0x00, sizeof(mmap_t));
  TKMEM_FREE(map);

  return RET_OK;
#else
  int fd = 0;
  return_value_if_fail(map != NULL, RET_BAD_PARAMS);
  fd = tk_pointer_to_int(map->fd);
  if (map->data != NULL) {
    munmap(map->data, map->size);
  }
  if (fd >= 0) {
    close(fd);
  }
  memset(map, 0x00, sizeof(mmap_t));
  TKMEM_FREE(map);
#endif /*WIN32*/
  return RET_OK;
}
