/**
 * File:   fs.h
 * Author: AWTK Develop Team
 * Brief:  simple fs api
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#if defined(HAS_POSIX_FS) || defined(__APPLE__) || defined(LINUX) || defined(WIN32)

#if defined(__APPLE__) || defined(LINUX)
#include <unistd.h>
#elif defined(WIN32)
#include <windows.h>
#define unlink _unlink
#endif

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "base/fs.h"
#include "base/mem.h"

int32_t fs_file_size(const char* name) {
  struct stat st = {0};
  return_value_if_fail(name != NULL, -1);

  if (stat(name, &st) == 0 && (st.st_mode & S_IFMT) == S_IFREG) {
    return st.st_size;
  } else {
    return -1;
  }
}

int32_t fs_read_file_part(const char* name, void* buff, uint32_t size, uint32_t offset) {
  int32_t ret = 0;
  FILE* fp = NULL;
  return_value_if_fail(name != NULL && buff != NULL && size > 0, -1);

  fp = fopen(name, "rb");

  if (fp != NULL) {
    ret = fseek(fp, offset, SEEK_SET);
    if (ret == 0) {
      ret = fread(buff, 1, size, fp);
    }
    fclose(fp);
  }

  return ret;
}

void* fs_read_file(const char* name, uint32_t* size) {
  uint8_t* buff = NULL;
  int32_t len = fs_file_size(name);
  return_value_if_fail(name != NULL && len > 0, NULL);

  buff = (uint8_t*)TKMEM_ALLOC(len + 1);
  return_value_if_fail(buff != NULL, NULL);

  if (fs_read_file_part(name, buff, len, 0) == len) {
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

ret_t fs_write_file(const char* name, const void* buff, uint32_t size) {
  FILE* fp = NULL;
  return_value_if_fail(name != NULL && buff != NULL, RET_BAD_PARAMS);

  fp = fopen(name, "wb+");
  return_value_if_fail(fp != NULL, RET_FAIL);

  fwrite(buff, size, 1, fp);
  fclose(fp);

  return RET_OK;
}

ret_t fs_unlink(const char* name) {
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);

  unlink(name);

  return RET_OK;
}

#else
#include "base/fs.h"
#include "base/mem.h"

int32_t fs_file_size(const char* name) {
  (void)name;
  return FALSE;
}

int32_t fs_read_file_part(const char* name, void* buff, uint32_t size, uint32_t offset) {
  (void)name;
  (void)buff;
  (void)size;
  (void)offset;
  return RET_FAIL;
}

void* fs_read_file(const char* name, uint32_t* size) {
  (void)name;
  (void)size;

  return NULL;
}

ret_t fs_write_file(const char* name, const void* buff, uint32_t size) {
  (void)name;
  (void)buff;
  (void)size;

  return RET_FAIL;
}

ret_t fs_unlink(const char* name) {
  (void)name;

  return RET_FAIL;
}

#endif
