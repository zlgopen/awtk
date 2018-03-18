/**
 * File:   utils.c
 * Author: Li XianJing <xianjimli@hotmail.com>
 * Brief:  utils function
 *
 * Copyright (c) 2018 - 2018  Li XianJing <xianjimli@hotmail.com>
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
#include "base/mem.h"
#include "base/enums.h"
#include "base/resource_manager.h"

char* read_file(const char* file_name, uint32_t* length) {
  struct stat st = {0};
  if (stat(file_name, &st)) {
    return NULL;
  } else {
    char* buffer = (char*)MEM_ALLOC(st.st_size + 1);
    FILE* fp = fopen(file_name, "rb");
    fread(buffer, 1, st.st_size, fp);
    fclose(fp);
    buffer[st.st_size] = '\0';
    *length = st.st_size;

    return buffer;
  }
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

ret_t filename_to_name(const char* filename, char* str, uint32_t size) {
  char* p = NULL;
  const char* name = filename;
  return_value_if_fail(filename != NULL && str != NULL, RET_BAD_PARAMS);

  name = strrchr(filename, '/');
  if (name == NULL) {
    name = strrchr(filename, '\\');
  }

  if (name == NULL) {
    name = filename;
  } else {
    name += 1;
  }

  strncpy(str, name, size);
  str[size - 1] = '\0';
  p = strchr(str, '.');
  if (p != NULL) {
    *p = '\0';
  }

  return RET_OK;
}

ret_t output_c_source(const char* filename, const char* prefix, const char* name, uint8_t* buff,
                      uint32_t size) {
  uint32_t i = 0;
  FILE* fp = NULL;
  char str[NAME_LEN + 1];
  return_value_if_fail(filename != NULL && buff != NULL, RET_BAD_PARAMS);

  if (name == NULL) {
    filename_to_name(filename, str, sizeof(str));
    name = str;
  }

  fp = fopen(filename, "wb+");
  if (fp != NULL) {
    fprintf(fp, "const unsigned char %s_%s[] = {", prefix ? prefix : "", name);
    for (i = 0; i < size; i++) {
      if ((i % 20) == 0) {
        fprintf(fp, "\n");
      }
      fprintf(fp, "0x%02x,", (int)(buff[i]));
    }

    fprintf(fp, "};\n");
    fclose(fp);

    return RET_OK;
  }

  return RET_FAIL;
}

ret_t output_res_c_source(const char* filename, uint16_t type, uint16_t subtype, uint8_t* buff, uint32_t size) {
  resource_info_t* res = NULL;
  uint32_t total_size = sizeof(resource_info_t) + size;
  const key_type_value_t* kv = resouce_type_name_find_by_value(type);
  return_value_if_fail(kv != NULL, RET_BAD_PARAMS);
  return_value_if_fail(filename != NULL && buff != NULL, RET_BAD_PARAMS);

  res = (resource_info_t*)malloc(total_size);
  memset(res, 0x00, sizeof(resource_info_t));
  res->size = size;
  res->type = type;
  res->subtype = subtype;
  memcpy(res->data, buff, size);
  filename_to_name(filename, res->name, sizeof(res->name));
  output_c_source(filename, kv->name, res->name, (uint8_t*)res, total_size);
  free(res);

  return RET_OK;
}

const char* skip_to(const char* p, char c) {
  while(*p && *p != c) p++;

  return p;
}

const char* skip_char(const char* p, char c) {
  while(*p && *p == c) p++;

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
  return strncmp(p, str, strlen(str)) == 0;
}

const char* to_lower(char* str) {
  char* p = str;

  while(*p) {
    *p = tolower(*p);
    p++;
  }

  return str;
}

