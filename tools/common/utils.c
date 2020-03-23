/**
 * File:   utils.c
 * Author: AWTK Develop Team
 * Brief:  utils function
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
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/wstr.h"
#include "base/enums.h"
#include "tkc/path.h"
#include "base/assets_manager.h"

bool_t exit_if_need_not_update(const char* in, const char* out) {
  if (in == NULL || out == NULL) {
    log_debug("invalid params: %s %s\n", in, out);
    return TRUE;
  }

  if (!fs_file_exist(os_fs(), in)) {
    log_debug("%s not exist\n", in);
    return TRUE;
  }

  if (!fs_file_exist(os_fs(), out)) {
    return FALSE;
  }

  ret_t rt = RET_OK;
  fs_stat_info_t fst_in;
  fs_stat_info_t fst_out;

  rt = fs_stat(os_fs(), in, &fst_in);
  if (rt != RET_OK) {
    log_debug("get \"%s\" filetime failed\n", in);
    return TRUE;
  }
  rt = fs_stat(os_fs(), out, &fst_out);
  if (rt != RET_OK) {
    log_debug("get \"%s\" filetime failed\n", out);
    return TRUE;
  }

  if (fst_in.mtime < fst_out.mtime) {
    log_debug("Skip because: %s is newer than %s\n", out, in);
    return TRUE;
  }
  return FALSE;
}

void exit_if_need_not_update_for_infiles(const char* out, int infiles_number, ...) {
  int i = 0;
  va_list va;
  ret_t rt = RET_OK;
  fs_stat_info_t st_in;
  fs_stat_info_t st_out;
  bool_t is_not_need_update = TRUE;

  if (out == NULL) {
    log_debug("invalid params: %s \n", out);
    exit(-1);
  }

  if (!fs_file_exist(os_fs(), out)) {
    return;
  }

  rt = fs_stat(os_fs(), out, &st_out);
  if (rt != RET_OK) {
    log_debug("get \"%s\" filetime failed\n", out);
    exit(-1);
  }

  va_start(va, infiles_number);

  for (i = 0; i < infiles_number; i++) {
    char* in = va_arg(va, char*);

    if (in == NULL) {
      continue;
    }

    if (!fs_file_exist(os_fs(), in)) {
      log_debug("%s not exist\n", in);
      is_not_need_update = TRUE;
      break;
    }

    rt = fs_stat(os_fs(), in, &st_in);
    if (rt != RET_OK) {
      log_debug("get \"%s\" filetime failed\n", in);
      exit(-1);
    }

    if (st_in.mtime > st_out.mtime) {
      is_not_need_update = FALSE;
      break;
    }
  }

  va_end(va);

  if (is_not_need_update) {
    log_debug("Skip because: %s is new \n", out);
    exit(0);
  }
}

char* read_file(const char* file_name, uint32_t* length) {
  return (char*)file_read(file_name, length);
}

ret_t write_file(const char* file_name, const void* buff, uint32_t length) {
  return file_write(file_name, buff, length);
}

int unique(wchar_t* str, int size) {
  int i = 0;
  wchar_t* d = str + 1;

  for (i = 1; i < size; i++) {
    wchar_t c = str[i];
    if (c != d[-1]) {
      d[0] = c;
      d++;
    }
  }

  return d - str;
}

static const char* to_var_name(char var_name[2 * TK_NAME_LEN + 1], const char* theme,
                               const char* prefix, const char* name) {
  if (tk_str_eq(theme, "default") || theme == NULL) {
    tk_snprintf(var_name, 2 * TK_NAME_LEN, "%s_%s", prefix ? prefix : "", name);
  } else {
    tk_snprintf(var_name, 2 * TK_NAME_LEN, "%s_%s_%s", prefix ? prefix : "", name, theme);
  }

  return filter_name(var_name);
}

ret_t output_c_source(const char* filename, const char* theme, const char* prefix, const char* name,
                      uint8_t* buff, uint32_t size) {
  uint32_t i = 0;
  fs_file_t* ft = NULL;
  char str[TK_NAME_LEN + 1];
  char var_name[2 * TK_NAME_LEN + 1];
  return_value_if_fail(filename != NULL && buff != NULL, RET_BAD_PARAMS);

  if (name == NULL) {
    filename_to_name(filename, str, sizeof(str));
    name = str;
  }

  log_debug("filename=%s prefix=%s name=%s size=%u\n", filename, prefix, name, size);

  ft = fs_open_file(os_fs(), filename, "wb+");
  if (ft != NULL) {
    fs_file_printf(ft, "TK_CONST_DATA_ALIGN(const unsigned char %s[]) = {",
                   to_var_name(var_name, theme, prefix, name));
    for (i = 0; i < size; i++) {
      if ((i % 20) == 0) {
        fs_file_printf(ft, "\n");
      }
      fs_file_printf(ft, "0x%02x,", (int)(buff[i]));
    }

    fs_file_printf(ft, "};/*%u*/\n", size);
    fs_file_close(ft);

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t output_res_c_source_ex(const char* filename, const char* theme, uint16_t type,
                             uint16_t subtype, uint8_t* buff, uint32_t size, const char* name) {
  asset_info_t* res = NULL;
  uint32_t total_size = sizeof(asset_info_t) + size;
  const key_type_value_t* kv = asset_type_find_by_value(type);
  return_value_if_fail(kv != NULL, RET_BAD_PARAMS);
  return_value_if_fail(filename != NULL && buff != NULL, RET_BAD_PARAMS);

  res = (asset_info_t*)malloc(total_size);
  memset(res, 0x00, total_size);
  res->size = size;
  res->type = type;
  res->is_in_rom = TRUE;
  res->refcount = 0;
  res->subtype = subtype;
  memcpy(res->data, buff, size);
  if (name != NULL) {
    tk_strncpy(res->name, name, sizeof(res->name) - 1);
  } else {
    if (type == ASSET_TYPE_DATA) {
      filename_to_name_ex(filename, res->name, sizeof(res->name), FALSE);
    } else {
      filename_to_name(filename, res->name, sizeof(res->name));
    }
  }
  output_c_source(filename, theme, kv->name, res->name, (uint8_t*)res, total_size);
  free(res);

  return RET_OK;
}

ret_t output_res_c_source(const char* filename, const char* theme, uint16_t type, uint16_t subtype,
                          uint8_t* buff, uint32_t size) {
  return output_res_c_source_ex(filename, theme, type, subtype, buff, size, NULL);
}

const char* skip_to(const char* p, char c) {
  while (*p && *p != c) p++;

  return p;
}

const char* skip_char(const char* p, char c) {
  while (*p && *p == c) p++;

  return p;
}

const char* skip_to_next(const char* p, char c) {
  return skip_char(skip_to(skip_char(p, c), c), c);
}

const char* get_next_token(const char* p, char* token, char c) {
  const char* start = skip_char(p, c);
  const char* end = skip_to(start, c);
  uint32_t size = end - start;
  strncpy(token, start, size);
  token[size] = '\0';

  return token;
}

bool_t start_with(const char* p, const char* str) {
  return_value_if_fail(p != NULL && str != NULL && strlen(p) >= strlen(str), FALSE);

  return strncmp(p, str, strlen(str)) == 0;
}

bool_t end_with(const char* p, const char* str) {
  return_value_if_fail(p != NULL && str != NULL && strlen(p) >= strlen(str), FALSE);

  return strncmp(p + strlen(p) - strlen(str), str, strlen(str)) == 0;
}

const char* to_lower(char* str) {
  char* p = str;

  while (*p) {
    *p = tolower(*p);
    p++;
  }

  return str;
}

const char* filter_name(char* name) {
  char* p = name;
  while (*p) {
    if (!(isdigit(*p) || isalpha(*p) || *p == '_')) {
      *p = '_';
    }
    p++;
  }

  return name;
}

wchar_t** argvw_create(int argc, char* argv[]) {
  int i = 0;
  wstr_t str;
  wchar_t** argvw = TKMEM_ALLOC((argc + 1) * sizeof(wchar_t*));
  return_value_if_fail(argvw != NULL, NULL);
  wstr_init(&str, 100);

  for (i = 0; i < argc; i++) {
    wstr_set_utf8(&str, argv[i]);
    argvw[i] = wcsdup(str.str);
  }
  argvw[i] = NULL;

  return argvw;
}

ret_t argvw_destroy(wchar_t** argvw) {
  uint32_t i = 0;
  return_value_if_fail(argvw != NULL, RET_BAD_PARAMS);

  for (i = 0; argvw[i] != NULL; i++) {
    TKMEM_FREE(argvw[i]);
  }
  TKMEM_FREE(argvw);

  return RET_OK;
}
