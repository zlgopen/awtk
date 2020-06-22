/**
 * File:   path.c
 * Author: AWTK Develop Team
 * Brief:  path
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
 * 2018-08-26 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <stdarg.h>
#include "tkc/fs.h"
#include "tkc/path.h"
#include "tkc/utils.h"

#define IS_PATH_SEP(c) ((c) == '/' || (c) == '\\')

ret_t path_basename(const char* path, char* result, int32_t size) {
  const char* p = NULL;
  int32_t real_size = 0;
  return_value_if_fail(path != NULL && result != NULL, RET_BAD_PARAMS);

  memset(result, 0x00, size);
  p = strrchr(path, TK_PATH_SEP);
  if (p == NULL) {
    p = strrchr(path, TK_PATH_SEP == '/' ? '\\' : '/');
  }
  if (p == NULL) {
    p = path;
  } else {
    p++;
  }

  real_size = strlen(p);
  return_value_if_fail(real_size < size, RET_BAD_PARAMS);

  tk_strncpy(result, p, size - 1);

  return RET_OK;
}

ret_t path_extname(const char* path, char* result, int32_t size) {
  const char* p = NULL;
  int32_t real_size = 0;
  return_value_if_fail(path != NULL && result != NULL, RET_BAD_PARAMS);

  memset(result, 0x00, size);
  p = strrchr(path, '.');
  if (p != NULL) {
    real_size = strlen(p);
    return_value_if_fail(real_size < size, RET_BAD_PARAMS);
    tk_strncpy(result, p, size - 1);

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t path_dirname(const char* path, char* result, int32_t size) {
  const char* p = NULL;
  int32_t real_size = 0;
  return_value_if_fail(path != NULL && result != NULL, RET_BAD_PARAMS);

  memset(result, 0x00, size);
  p = strrchr(path, TK_PATH_SEP);
  if (p == NULL) {
    p = strrchr(path, TK_PATH_SEP == '/' ? '\\' : '/');
  }

  if (p != NULL) {
    real_size = p - path + 1;
    return_value_if_fail(real_size < size, RET_BAD_PARAMS);
    tk_strncpy(result, path, real_size);

    return RET_OK;
  }

  return RET_FAIL;
}

bool_t path_exist(const char* path) {
  return fs_dir_exist(os_fs(), path);
}

ret_t path_cwd(char path[MAX_PATH + 1]) {
  return fs_get_cwd(os_fs(), path);
}

ret_t path_exe(char path[MAX_PATH + 1]) {
  return fs_get_exe(os_fs(), path);
}

ret_t path_app_root(char path[MAX_PATH + 1]) {
  char* p = NULL;
  char exe_path[MAX_PATH + 1];
  memset(exe_path, 0x00, sizeof(exe_path));

  if (fs_get_exe(os_fs(), exe_path) == RET_OK) {
    path_normalize(exe_path, path, MAX_PATH + 1);
    p = strrchr(path, TK_PATH_SEP);
    if (p != NULL) {
      *p = '\0';
      p = strrchr(path, TK_PATH_SEP);
      if (p != NULL) {
        if (strcmp(p + 1, "bin") == 0) {
          *p = '\0';
        }
      }

      return RET_OK;
    }
  }

  return RET_FAIL;
}

static char* path_up(char* path) {
  uint32_t len = strlen(path);
  if (len > 0) {
    char* p = path + strlen(path) - 1;
    if (IS_PATH_SEP(*p)) {
      *p = '\0';
      p--;
    }

    while (p > path && *p != '/' && *p != '\\') {
      p--;
    }

    if (path == p) {
      if (*p != '/' && *p != '\\') {
        p[0] = '\0';
        return p;
      } else {
        p[0] = TK_PATH_SEP;
      }
    } else {
      p[0] = TK_PATH_SEP;
    }
    p[1] = '\0';

    return p + 1;
  } else {
    return path;
  }
}

ret_t path_normalize(const char* path, char* result, int32_t size) {
  const char* s = path;
  char* d = result;
  return_value_if_fail(path != NULL && result != NULL, RET_BAD_PARAMS);

  memset(result, 0x00, size);
  while ((d - result) < size && *s) {
    switch (*s) {
      case '/':
      case '\\': {
        if (d == result || !IS_PATH_SEP(d[-1])) {
          *d++ = TK_PATH_SEP;
        }
        while (IS_PATH_SEP(*s)) {
          s++;
        }
        break;
      }
      case '.': {
        if (IS_PATH_SEP(s[1])) {
          s += 2;
        } else if (s[1] == '.') {
          return_value_if_fail(IS_PATH_SEP(s[2]) || s[2] == '\0', RET_FAIL);

          d = path_up(result);
          s += 2;
          if (IS_PATH_SEP(s[0])) {
            s++;
          }
        } else {
          *d++ = *s++;
        }
        break;
      }
      default: { *d++ = *s++; }
    }
  }

  return RET_OK;
}

bool_t path_is_abs(const char* path) {
  return_value_if_fail(path != NULL && *path, FALSE);

  return path[0] == '/' || path[1] == ':';
}

ret_t path_abs(const char* path, char* result, int32_t size) {
  char cwd[MAX_PATH + 1];
  return_value_if_fail(path != NULL && result != NULL && strlen(path) < size, RET_BAD_PARAMS);

  if (path_is_abs(path)) {
    tk_strncpy(result, path, size);

    return RET_OK;
  }

  if (path_cwd(cwd) == RET_OK) {
    return path_build(result, size, cwd, path, NULL);
  }

  return RET_FAIL;
}

ret_t path_build(char* result, int32_t size, ...) {
  va_list va;
  char* d = result;
  const char* p = NULL;
  int32_t avail_size = size;
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);

  memset(result, 0x00, size);
  va_start(va, size);
  do {
    p = va_arg(va, char*);
    if (p != NULL) {
      int32_t new_size = strlen(p);
      return_value_if_fail(((new_size + 1) < avail_size), RET_FAIL);

      if (d != result) {
        if (d[-1] != TK_PATH_SEP) {
          *d++ = TK_PATH_SEP;
        }
      }

      memcpy(d, p, new_size);
      avail_size -= new_size;
      d += new_size;
    } else {
      break;
    }
  } while (p != NULL);

  va_end(va);

  return RET_OK;
}

ret_t path_replace_basename(char* result, int32_t size, const char* filename,
                            const char* basename) {
  char dirname[MAX_PATH + 1];
  return_value_if_fail(result != NULL && filename != NULL && basename != NULL, RET_BAD_PARAMS);

  path_normalize(filename, result, size);
  path_dirname(result, dirname, MAX_PATH);

  dirname[MAX_PATH] = '\0';
  return path_build(result, size, dirname, basename, NULL);
}

ret_t path_replace_extname(char* result, int32_t size, const char* filename, const char* extname) {
  char* p = NULL;
  return_value_if_fail(result != NULL && filename != NULL && extname != NULL, RET_BAD_PARAMS);

  memset(result, 0x00, size);
  path_normalize(filename, result, size);
  p = strrchr(result, '.');
  if (p != NULL) {
    uint32_t n = 0;
    p++;
    *p = '\0';
    n = size - strlen(result) - 1;
    strncpy(p, extname, n);
  }

  return RET_OK;
}
