/**
 * File:   utils.h
 * Author: AWTK Develop Team
 * Brief:  utils struct and utils functions.
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
 * 2018-02-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/fs.h"
#include "base/mem.h"
#include "base/path.h"
#include "base/utils.h"

const char* tk_skip_to_num(const char* str) {
  const char* p = str;
  return_value_if_fail(str != NULL, NULL);

  while (*p && (*p < '0' || *p > '9')) p++;

  return p;
}

int tk_str2bool(const char* str) {
  if (str == NULL || str[0] == '0' || strcmp(str, "false") == 0 || strcmp(str, "no") == 0) {
    return 0;
  }

  return 1;
}

static long tk_strtol_internal(const char* str, const char** end, int base) {
  int i = 0;
  long n = 0;
  char c = 0;
  return_value_if_fail(str != NULL && (base == 10 || base == 8 || base == 16), 0);

  if (base == 10) {
    for (i = 0; str[i] && i < 10; i++) {
      c = str[i];

      if (c < '0' || c > '9') {
        break;
      }

      n = n * base + c - '0';
    }
  } else if (base == 8) {
    for (i = 0; str[i] && i < 10; i++) {
      c = str[i];

      if (c < '0' || c > '7') {
        break;
      }

      n = n * base + c - '0';
    }
  } else if (base == 16) {
    for (i = 0; str[i] && i < 10; i++) {
      c = str[i];

      if ((c >= '0' && c <= '9')) {
        c -= '0';
      } else if (c >= 'a' && c <= 'f') {
        c = c - 'a' + 10;
      } else if (c >= 'A' && c <= 'F') {
        c = c - 'A' + 10;
      } else {
        break;
      }

      n = n * base + c;
    }
  }

  if (end != NULL) {
    *end = str + i;
  }

  return n;
}

long tk_strtol(const char* str, const char** end, int base) {
  long n = 0;
  int neg = 0;
  return_value_if_fail(str != NULL, 0);

  while (*str == ' ' || *str == '\t') str++;

  if (*str == '+' || *str == '-') {
    neg = *str == '-';
    str++;
  }

  n = tk_strtol_internal(str, end, base);

  return neg ? -n : n;
}

int tk_atoi(const char* str) {
  return tk_strtol(str, NULL, 10);
}

double tk_atof(const char* str) {
  int n = 0;
  uint32_t zero = 0;
  unsigned int f = 0;
  int neg = 0;
  double result = 0;
  const char* p = NULL;
  return_value_if_fail(str != NULL, 0);

  if (str[0] == '+' || str[0] == '-') {
    neg = str[0] == '-';
    str++;
  }

  n = tk_strtol_internal(str, &p, 10);

  if (p != NULL && *p == '.') {
    p++;
    while (*p && *p == '0') {
      p++;
      zero++;
    }
    f = tk_strtol_internal(p, NULL, 10);
  }

  result = f;
  while (result >= 1) {
    result = result / 10;
  }
  while (zero > 0) {
    result = result / 10;
    zero--;
  }

  result = n + result;

  return neg ? -result : result;
}

static const char* tk_itoa_simple(char* str, int len, int n, const char** end) {
  int i = 0;
  int value = n;
  int need_len = 0;

  return_value_if_fail(str != NULL && len > 2, NULL);

  if (n == 0) {
    str[0] = '0';
    str[1] = '\0';

    if (end != NULL) {
      *end = str + 1;
    }

    return str;
  }

  if (n < 0) {
    n = -n;
    str[0] = '-';
    need_len++;
  }

  value = n;
  while (value > 0) {
    value = value / 10;
    need_len++;
  }

  need_len++; /*for null char*/
  return_value_if_fail(len > (need_len), NULL);

  i = need_len - 2;
  while (n > 0) {
    str[i--] = (n % 10) + '0';
    n = n / 10;
  }
  str[need_len - 1] = '\0';

  if (end != NULL) {
    *end = str + need_len - 1;
  }

  return str;
}

const char* tk_itoa(char* str, int len, int n) {
  return tk_itoa_simple(str, len, n, NULL);
}

const char* tk_ftoa(char* str, int len, double value) {
  tk_snprintf(str, len, "%lf", value);

  return str;
}

char* tk_strcpy(char* dst, const char* src) {
  return strcpy(dst, src);
}

char* tk_strncpy(char* dst, const char* src, size_t len) {
  return_value_if_fail(dst != NULL && src != NULL, dst);

  strncpy(dst, src, len);
  dst[len] = '\0';

  return dst;
}

char* tk_strndup(const char* str, uint32_t len) {
  char* s = NULL;
  return_value_if_fail(str != NULL, NULL);

  s = (char*)TKMEM_ALLOC(len + 1);
  if (s != NULL) {
    memcpy(s, str, len);
    s[len] = '\0';
  }

  return s;
}

char* tk_strdup(const char* str) {
  return_value_if_fail(str != NULL, NULL);

  return tk_strndup(str, strlen(str));
}

uint16_t* tk_memset16(uint16_t* buff, uint16_t val, uint32_t size) {
  return_value_if_fail(buff != NULL, NULL);

  if (size < 8) {
    uint32_t i = 0;

    for (i = 0; i < size; i++) {
      buff[i] = val;
    }

    return buff;
  } else {
    uint32_t* p = NULL;
    uint32_t* end = NULL;
    uint32_t val32 = (val << 16) | val;
    uint8_t pad1 = ((size_t)buff) & 0x03;
    uint8_t pad2 = pad1 ? (size & 0x01) == 0 : (size & 0x01) == 1;

    if (pad1) {
      buff[0] = val;
      p = (uint32_t*)(buff + 1);
    } else {
      p = (uint32_t*)(buff);
    }

    if (pad2) {
      buff[size - 1] = val;
      end = (uint32_t*)(buff + size - 1);
    } else {
      end = (uint32_t*)(buff + size);
    }

    while (p < end) {
      *p++ = val32;
    }
  }

  return buff;
}

uint32_t* tk_memset32(uint32_t* buff, uint32_t val, uint32_t size) {
  uint32_t* p = buff;
  return_value_if_fail(buff != NULL, NULL);

  while (size-- > 0) {
    *p++ = val;
  }

  return buff;
}

uint16_t* tk_memcpy16(uint16_t* dst, uint16_t* src, uint32_t size) {
  uint16_t* d = dst;
  uint16_t* s = src;
  return_value_if_fail(dst != NULL && src != NULL, NULL);

  while (size-- > 0) {
    *d++ = *s++;
  }

  return dst;
}

uint32_t* tk_memcpy32(uint32_t* dst, uint32_t* src, uint32_t size) {
  uint32_t* d = dst;
  uint32_t* s = src;
  return_value_if_fail(dst != NULL && src != NULL, NULL);

  while (size-- > 0) {
    *d++ = *s++;
  }

  return dst;
}

void* tk_pixel_copy(void* dst, const void* src, uint32_t size, uint8_t bpp) {
  return_value_if_fail(dst != NULL && src != NULL, NULL);

#ifdef HAS_FAST_MEMCPY
  memcpy(dst, src, size * bpp);
#else
  if (bpp == 2) {
    tk_memcpy16((uint16_t*)dst, (uint16_t*)src, size);
  } else if (bpp == 4) {
    tk_memcpy32((uint32_t*)dst, (uint32_t*)src, size);
  } else {
    assert(!"not supported yet.");
  }
#endif /*HAS_FAST_MEMCPY*/

  return dst;
}

#if defined(LINUX) || defined(APPLE) || defined(HAS_STDIO) || defined(WINDOWS)
#include <stdio.h>
#else
extern int vsscanf(const char* s, const char* format, va_list arg);
extern int vsnprintf(char* str, size_t size, const char* format, va_list ap);
#endif /*LINUX || APPLE || HAS_STDIO || WINDOWS*/

int tk_snprintf(char* str, size_t size, const char* format, ...) {
  int ret = 0;
  va_list va;
  va_start(va, format);
  ret = vsnprintf(str, size, format, va);
  va_end(va);

  return ret;
}

int tk_sscanf(const char* str, const char* format, ...) {
  int ret = 0;
  va_list va;
  va_start(va, format);
  ret = vsscanf(str, format, va);
  va_end(va);

  return ret;
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

  tk_strncpy(str, name, size - 1);
  p = strchr(str, '.');
  if (p != NULL) {
    *p = '\0';
  }

  return RET_OK;
}

#define INCLUDE_XML "<?include"

ret_t xml_file_expand(const char* filename, str_t* s, const char* data) {
  str_t ss;
  char subfilename[MAX_PATH + 1];

  const char* start = data;
  const char* p = strstr(start, INCLUDE_XML);

  str_init(&ss, 1024);
  while (p != NULL) {
    str_set(&ss, "");
    str_append_with_len(s, start, p - start);

    /*<include filename="subfilename">*/
    while (*p != '\"' && *p != '\0') {
      p++;
    }
    return_value_if_fail(*p == '\"', RET_FAIL);
    p++;
    while (*p != '\"' && *p != '\0') {
      str_append_char(&ss, *p++);
    }
    return_value_if_fail(*p == '\"', RET_FAIL);
    while (*p != '>' && *p != '\0') {
      p++;
    }
    return_value_if_fail(*p == '>', RET_FAIL);
    p++;

    path_replace_basename(subfilename, MAX_PATH, filename, ss.str);
    xml_file_expand_read(subfilename, &ss);

    str_append(s, ss.str);

    start = p;
    p = strstr(start, INCLUDE_XML);
  }

  str_append(s, start);
  str_reset(&ss);

  return RET_OK;
}

ret_t xml_file_expand_read(const char* filename, str_t* s) {
  uint32_t size = 0;
  char* buff = NULL;
  return_value_if_fail(filename != NULL && s != NULL, RET_BAD_PARAMS);

  str_set(s, "");
  buff = (char*)file_read(filename, &size);
  return_value_if_fail(buff != NULL, RET_FAIL);

  if (strstr(buff, INCLUDE_XML) != NULL) {
    xml_file_expand(filename, s, buff);
  } else {
    str_set_with_len(s, (const char*)buff, size);
  }
  TKMEM_FREE(buff);

  return RET_OK;
}

ret_t tk_str_append(char* str, uint32_t max_len, const char* s) {
  uint32_t len = 0;
  uint32_t org_len = 0;
  return_value_if_fail(str != NULL && s != NULL, RET_BAD_PARAMS);

  len = strlen(s);
  org_len = strlen(str);

  return_value_if_fail(max_len > (len + org_len), RET_FAIL);
  memcpy(str + org_len, s, len);
  str[org_len + len] = '\0';

  return RET_OK;
}
