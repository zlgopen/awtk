/**
 * File:   utils.h
 * Author: AWTK Develop Team
 * Brief:  utils struct and utils functions.
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
 * 2018-02-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/path.h"
#include "tkc/utils.h"

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

bool_t tk_atob(const char* str) {
  if (str == NULL || *str == 'f' || *str == 'F') {
    return FALSE;
  }

  return TRUE;
}

#ifdef HAS_NO_LIBC
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

#else
double tk_atof(const char* str) {
  return_value_if_fail(str != NULL, 0);

  return atof(str);
}

long tk_strtol(const char* str, const char** end, int base) {
  return_value_if_fail(str != NULL, 0);

  return strtol(str, (char**)end, base);
}

const char* tk_itoa(char* str, int len, int n) {
  return_value_if_fail(str != NULL, NULL);

  tk_snprintf(str, len, "%d", n);

  return str;
}
#endif /*HAS_NO_LIBC*/

int tk_atoi(const char* str) {
  return tk_strtol(str, NULL, 10);
}

const char* tk_ftoa(char* str, int len, double value) {
  tk_snprintf(str, len, "%lf", value);

  return str;
}

char* tk_strcpy(char* dst, const char* src) {
  return_value_if_fail(dst != NULL && src != NULL, NULL);

  return strcpy(dst, src);
}

char* tk_strncpy(char* dst, const char* src, size_t len) {
  return_value_if_fail(dst != NULL && src != NULL, NULL);

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

wchar_t* tk_wstrdup(const wchar_t* str) {
  uint32_t size = 0;
  wchar_t* new_str = NULL;
  return_value_if_fail(str != NULL, NULL);
  size = wcslen(str) + 1;

  new_str = TKMEM_ALLOC(size * sizeof(wchar_t));
  return_value_if_fail(new_str != NULL, NULL);
  memcpy(new_str, str, size * sizeof(wchar_t));

  return new_str;
}

uint16_t* tk_memset16(uint16_t* buff, uint16_t val, uint32_t size) {
  uint32_t n = 0;
  uint16_t* p = buff;
  uint8_t* pb = (uint8_t*)buff;

  return_value_if_fail(buff != NULL, NULL);

  while ((size_t)pb % 4 != 0 && size > 0) {
    *p = val;

    p++;
    size--;
    pb += 2;
  }

  n = size / 8; /*16bytes*/
  if (n > 0) {
    uint32_t* p32 = NULL;
    uint32_t data = val | (val << 16);

    while (n > 0) {
      p32 = (uint32_t*)pb;

      p32[0] = data;
      p32[1] = data;
      p32[2] = data;
      p32[3] = data;

      n--;
      pb += 16;
    }
  }

  n = size % 8;
  if (n > 0) {
    p = (uint16_t*)pb;
    while (n > 0) {
      *p = val;
      p++;
      n--;
    }
  }

  return buff;
}

uint32_t* tk_memset24(uint32_t* buff, void* val, uint32_t size) {
  uint32_t n = 0;
  uint32_t bytes = size * 3;
  uint8_t* pb = (uint8_t*)buff;
  uint8_t* src = (uint8_t*)val;

  while ((size_t)pb % 4 != 0 && size > 0) {
    pb[0] = src[0];
    pb[1] = src[1];
    pb[2] = src[2];

    pb += 3;
    size--;
  }

  bytes = size * 3;
  n = bytes / 12;

  if (n > 0) {
    uint32_t* p = NULL;
    uint32_t data0 = src[0] | src[1] << 8 | src[2] << 16 | src[0] << 24;
    uint32_t data1 = src[1] | src[2] << 8 | src[0] << 16 | src[1] << 24;
    uint32_t data2 = src[2] | src[0] << 8 | src[1] << 16 | src[2] << 24;

    while (n > 0) {
      p = (uint32_t*)pb;
      p[0] = data0;
      p[1] = data1;
      p[2] = data2;
      pb += 12;
      n--;
    }
  }

  bytes = bytes % 12;
  while (bytes > 0) {
    pb[0] = src[0];
    pb[1] = src[1];
    pb[2] = src[2];
    pb += 3;
    bytes -= 3;
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
    memcpy(dst, src, size * bpp);
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

ret_t filename_to_name_ex(const char* filename, char* str, uint32_t size, bool_t remove_extname) {
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

  if (remove_extname) {
    p = strrchr(str, '.');
    if (p != NULL) {
      *p = '\0';
    }
  }

  return RET_OK;
}

ret_t filename_to_name(const char* filename, char* str, uint32_t size) {
  return filename_to_name_ex(filename, str, size, TRUE);
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

int32_t tk_str_cmp(const char* a, const char* b) {
  if (a == b) {
    return 0;
  }

  if (a == NULL) {
    return -1;
  }

  if (b == NULL) {
    return 1;
  }

  return strcmp(a, b);
}

int32_t tk_str_icmp(const char* a, const char* b) {
  if (a == b) {
    return 0;
  }

  if (a == NULL) {
    return -1;
  }

  if (b == NULL) {
    return 1;
  }

  return strcasecmp(a, b);
}

char* tk_str_copy(char* dst, const char* src) {
  if (src != NULL) {
    uint32_t size = strlen(src) + 1;
    if (dst != NULL) {
      char* str = TKMEM_REALLOCT(char, dst, size);
      return_value_if_fail(str != NULL, dst);
      memcpy(str, src, size);
      dst = str;
    } else {
      char* str = (char*)TKMEM_ALLOC(size);
      return_value_if_fail(str != NULL, dst);
      memcpy(str, src, size);
      dst = str;
    }
  } else {
    if (dst != NULL) {
      *dst = '\0';
    }
  }

  return dst;
}

int tk_watoi_n(const wchar_t* str, uint32_t len) {
  char num[TK_NUM_MAX_LEN + 1] = {0};
  return_value_if_fail(str != NULL, 0);

  memset(num, 0x00, sizeof(num));
  tk_utf8_from_utf16_ex(str, len, num, TK_NUM_MAX_LEN);

  return tk_atoi(num);
}

int tk_watoi(const wchar_t* str) {
  return_value_if_fail(str != NULL, 0);

  return tk_watoi_n(str, wcslen(str));
}

bool_t tk_watob(const wchar_t* str) {
  if (str == NULL || *str == 'f' || *str == 'F') {
    return FALSE;
  }

  return TRUE;
}

double tk_watof(const wchar_t* str) {
  char num[TK_NUM_MAX_LEN + 1] = {0};
  return_value_if_fail(str != NULL, 0);

  tk_utf8_from_utf16(str, num, TK_NUM_MAX_LEN);

  return tk_atof(num);
}

ret_t default_destroy(void* data) {
  TKMEM_FREE(data);

  return RET_OK;
}

ret_t dummy_destroy(void* data) {
  return RET_OK;
}

int pointer_compare(const void* a, const void* b) {
  return ((const char*)a - (const char*)b);
}

int compare_always_equal(const void* a, const void* b) {
  return 0;
}

ret_t tk_replace_locale(const char* name, char out[TK_NAME_LEN + 1], const char* locale) {
  char* d = NULL;
  char* p = NULL;
  int32_t len = 0;
  const char* s = NULL;
  return_value_if_fail(strlen(name) < TK_NAME_LEN, RET_BAD_PARAMS);
  return_value_if_fail(strlen(locale) <= strlen(TK_LOCALE_MAGIC), RET_BAD_PARAMS);
  return_value_if_fail(name != NULL && out != NULL && locale != NULL, RET_BAD_PARAMS);

  d = out;
  s = name;
  p = strstr(name, TK_LOCALE_MAGIC);
  return_value_if_fail(p != NULL, RET_BAD_PARAMS);

  len = p - s;
  memcpy(d, s, len);
  d += len;

  len = strlen(locale);
  memcpy(d, locale, len);

  d += len;
  strcpy(d, p + strlen(TK_LOCALE_MAGIC));

  return RET_OK;
}

bool_t tk_is_valid_name(const char* name) {
  const char* p = name;
  while (*p) {
    if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') ||
        *p == '_') {
      p++;
    } else {
      return FALSE;
    }
  }

  return TRUE;
}

bool_t tk_str_start_with(const char* str, const char* prefix) {
  return_value_if_fail(str != NULL && prefix != NULL, FALSE);

  return strncmp(str, prefix, strlen(prefix)) == 0;
}

const char* tk_under_score_to_camel(const char* name, char* out, uint32_t max_out_size) {
  uint32_t i = 0;
  const char* s = name;
  return_value_if_fail(name != NULL && out != NULL && max_out_size > 0, NULL);

  while (*s && i < max_out_size) {
    if (*s == '_') {
      s++;
      if (*s != '\0') {
        out[i++] = toupper(*s);
      } else {
        break;
      }
    } else {
      out[i++] = *s;
    }
    s++;
  }
  out[i] = '\0';

  return out;
}

int32_t tk_pointer_to_int(const void* p) {
  return (char*)p - (char*)(NULL);
}

void* tk_pointer_from_int(int32_t v) {
  return ((char*)NULL) + v;
}

char* tk_str_toupper(char* str) {
  char* p = str;
  return_value_if_fail(str != NULL, NULL);

  while (*p) {
    *p = toupper(*p);
    p++;
  }

  return str;
}

char* tk_str_tolower(char* str) {
  char* p = str;
  return_value_if_fail(str != NULL, NULL);

  while (*p) {
    *p = tolower(*p);
    p++;
  }

  return str;
}

const char* tk_normalize_key_name(const char* name, char fixed_name[TK_NAME_LEN + 1]) {
  uint32_t len = 0;
  return_value_if_fail(name != NULL && fixed_name != NULL, NULL);

  len = strlen(name);
  tk_strncpy(fixed_name, name, TK_NAME_LEN);

  if (len > 1) {
    tk_str_toupper(fixed_name);
  }

  return fixed_name;
}

uint32_t tk_strlen(const char* str) {
  if (str == NULL || *str == '\0') {
    return 0;
  }

  return strlen(str);
}

wchar_t* tk_wstr_dup_utf8(const char* str) {
  int32_t len = 0;
  int32_t size = 0;
  wchar_t* wstr = NULL;
  return_value_if_fail(str != NULL, NULL);

  len = strlen(str) + 1;
  size = len * sizeof(wchar_t);
  wstr = TKMEM_ALLOC(size);
  return_value_if_fail(wstr != NULL, NULL);
  memset(wstr, 0x00, size);

  tk_utf8_to_utf16(str, wstr, len);

  return wstr;
}

uint32_t tk_wstr_count_c(const wchar_t* str, wchar_t c) {
  uint32_t nr = 0;
  const wchar_t* p = str;
  return_value_if_fail(p != NULL, nr);

  while (*p) {
    if (*p == c) {
      nr++;
    }
    p++;
  }

  return nr;
}
