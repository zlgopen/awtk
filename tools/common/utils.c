/**
 * File:   utils.c
 * Author: AWTK Develop Team
 * Brief:  utils function
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/enums.h"
#include "tkc/path.h"
#include "base/assets_manager.h"

void exit_if_need_not_update(const char* in, const char* out) {
  struct stat st_in;
  struct stat st_out;

  memset(&st_in, 0x00, sizeof(st_in));
  memset(&st_out, 0x00, sizeof(st_out));

  if (in == NULL || out == NULL) {
    log_debug("invalid params: %s %s\n", in, out);
    exit(-1);
  }

  if (stat(in, &st_in) < 0) {
    log_debug("%s not exist\n", in);
    exit(-1);
  }

  if (stat(out, &st_out) < 0) {
    return;
  }

  if (st_in.st_mtime < st_out.st_mtime) {
    log_debug("Skip because: %s is newer than %s\n", out, in);
    exit(0);
  }
}

char* read_file(const char* file_name, uint32_t* length) {
  return file_read(file_name, length);
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

static const char* to_var_name(char var_name[2 * TK_NAME_LEN + 1], const char* prefix,
                               const char* name) {
  tk_snprintf(var_name, 2 * TK_NAME_LEN, "%s_%s", prefix ? prefix : "", name);

  char* p = var_name;
  while (*p) {
    if (!(isdigit(*p) || isalpha(*p) || *p == '_')) {
      *p = '_';
    }
    p++;
  }

  return var_name;
}

ret_t output_c_source(const char* filename, const char* prefix, const char* name, uint8_t* buff,
                      uint32_t size) {
  uint32_t i = 0;
  FILE* fp = NULL;
  char str[TK_NAME_LEN + 1];
  char var_name[2 * TK_NAME_LEN + 1];
  return_value_if_fail(filename != NULL && buff != NULL, RET_BAD_PARAMS);

  if (name == NULL) {
    filename_to_name(filename, str, sizeof(str));
    name = str;
  }

  log_debug("filename=%s prefix=%s name=%s size=%u\n", filename, prefix, name, size);

  fp = fopen(filename, "wb+");
  if (fp != NULL) {
    fprintf(fp, "TK_CONST_DATA_ALIGN(const unsigned char %s[]) = {",
            to_var_name(var_name, prefix, name));
    for (i = 0; i < size; i++) {
      if ((i % 20) == 0) {
        fprintf(fp, "\n");
      }
      fprintf(fp, "0x%02x,", (int)(buff[i]));
    }

    fprintf(fp, "};/*%u*/\n", size);
    fclose(fp);

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t output_res_c_source_ex(const char* filename, uint16_t type, uint16_t subtype, uint8_t* buff,
                             uint32_t size, const char* name) {
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
  output_c_source(filename, kv->name, res->name, (uint8_t*)res, total_size);
  free(res);

  return RET_OK;
}

ret_t output_res_c_source(const char* filename, uint16_t type, uint16_t subtype, uint8_t* buff,
                          uint32_t size) {
  return output_res_c_source_ex(filename, type, subtype, buff, size, NULL);
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
