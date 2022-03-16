/**
 * File:   utils.h
 * Author: AWTK Develop Team
 * Brief:  utils struct and utils functions.
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/object.h"
#include "tkc/named_value.h"
#include "tkc/data_reader_factory.h"
#include "tkc/data_writer_factory.h"

#define IS_ADDRESS_ALIGN_4(addr) !((((size_t)(addr)) & 0x3) | 0x0)

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

int32_t tk_strtoi(const char* str, const char** end, int base) {
  long ret = tk_strtol(str, end, base);
  if (ret > INT32_MAX) {
    ret = INT32_MAX;
  } else if (ret < INT32_MIN) {
    ret = INT32_MIN;
  }
  return (int32_t)ret;
}

long tk_strtol(const char* str, const char** end, int base) {
  return_value_if_fail(str != NULL, 0);

  return strtol(str, (char**)end, base);
}

int64_t tk_strtoll(const char* str, const char** end, int base) {
  return strtoll(str, (char**)end, base);
}

uint64_t tk_strtoull(const char* str, const char** end, int base) {
  return strtoull(str, (char**)end, base);
}

const char* tk_itoa(char* str, int len, int n) {
  return_value_if_fail(str != NULL, NULL);

  tk_snprintf(str, len, "%d", n);

  return str;
}
#endif /*HAS_NO_LIBC*/

#define IS_HEX_NUM(s) (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
#define IS_BIN_NUM(s) (s[0] == '0' && (s[1] == 'b' || s[1] == 'B'))

int32_t tk_atoi(const char* str) {
  return_value_if_fail(str != NULL, 0);
  if (IS_HEX_NUM(str)) {
    return tk_strtoi(str + 2, NULL, 16);
  } else if (IS_BIN_NUM(str)) {
    return tk_strtoi(str + 2, NULL, 2);
  } else {
    return tk_strtoi(str, NULL, 10);
  }
}

int64_t tk_atol(const char* str) {
  return_value_if_fail(str != NULL, 0);
  if (IS_HEX_NUM(str)) {
    return tk_strtoll(str + 2, NULL, 16);
  } else if (IS_BIN_NUM(str)) {
    return tk_strtoll(str + 2, NULL, 2);
  } else {
    return tk_strtoll(str, NULL, 10);
  }
}

uint64_t tk_atoul(const char* str) {
  return_value_if_fail(str != NULL, 0);
  if (IS_HEX_NUM(str)) {
    return tk_strtoull(str + 2, NULL, 16);
  } else if (IS_BIN_NUM(str)) {
    return tk_strtoull(str + 2, NULL, 2);
  } else {
    return tk_strtoull(str, NULL, 10);
  }
}

const char* tk_ftoa(char* str, int len, double value) {
  tk_snprintf(str, len, "%lf", value);

  return str;
}

char* tk_strcpy(char* dst, const char* src) {
  return_value_if_fail(dst != NULL && src != NULL, NULL);
  if (dst != src) {
    return strcpy(dst, src);
  } else {
    return dst;
  }
}

char* tk_strncpy(char* dst, const char* src, size_t len) {
  return_value_if_fail(dst != NULL && src != NULL, NULL);

  if (dst != src) {
    strncpy(dst, src, len);
    dst[len] = '\0';
  }

  return dst;
}

char* tk_strncpy_s(char* dst, size_t dst_len, const char* src, size_t src_len) {
  size_t len = 0;
  return_value_if_fail(dst != NULL && src != NULL && dst_len > 0, NULL);

  len = tk_min(dst_len - 1, src_len);

  return tk_strncpy(dst, src, len);
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
  return_value_if_fail(str != NULL && format != NULL, 0);

  va_start(va, format);
  ret = tk_vsnprintf(str, size, format, va);
  va_end(va);

  return ret;
}

int tk_vsnprintf(char* str, size_t size, const char* format, va_list ap) {
  return_value_if_fail(str != NULL && format != NULL, 0);

  return vsnprintf(str, size, format, ap);
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
#define TAG_PROPERTY "property"
#define CHAR_DOUDLE_QUOTE '\"'
#define CHAR_SINGLE_QUOTE '\''

typedef enum _xml_property_close_state_t {
  XML_PROPERTY_CLOSE_STATE_CLOSE = 0x0,
  XML_PROPERTY_CLOSE_STATE_OPEN_PROPERTY,
  XML_PROPERTY_CLOSE_STATE_OPEN_DOUDLE_QUOTE,
  XML_PROPERTY_CLOSE_STATE_OPEN_SINGLE_QUOTE,
} xml_property_close_state_t;

static xml_property_close_state_t xml_property_get_close_state(const char* start, const char* end) {
  const char* tmp = start;
  xml_property_close_state_t close_state = XML_PROPERTY_CLOSE_STATE_CLOSE;

  while (tmp != end) {
    if (*tmp == CHAR_DOUDLE_QUOTE) {
      if (close_state == XML_PROPERTY_CLOSE_STATE_OPEN_DOUDLE_QUOTE) {
        close_state = XML_PROPERTY_CLOSE_STATE_CLOSE;
      } else if (close_state == XML_PROPERTY_CLOSE_STATE_CLOSE) {
        close_state = XML_PROPERTY_CLOSE_STATE_OPEN_DOUDLE_QUOTE;
      }
    } else if (*tmp == CHAR_SINGLE_QUOTE) {
      if (close_state == XML_PROPERTY_CLOSE_STATE_OPEN_SINGLE_QUOTE) {
        close_state = XML_PROPERTY_CLOSE_STATE_CLOSE;
      } else if (close_state == XML_PROPERTY_CLOSE_STATE_CLOSE) {
        close_state = XML_PROPERTY_CLOSE_STATE_OPEN_SINGLE_QUOTE;
      }
    } else if (strstr(tmp, TAG_PROPERTY) == tmp) {
      if (close_state == XML_PROPERTY_CLOSE_STATE_OPEN_PROPERTY) {
        close_state = XML_PROPERTY_CLOSE_STATE_CLOSE;
      } else if (close_state == XML_PROPERTY_CLOSE_STATE_CLOSE) {
        close_state = XML_PROPERTY_CLOSE_STATE_OPEN_PROPERTY;
      }
    }
    tmp++;
  }
  return close_state;
}

ret_t xml_file_expand(const char* filename, str_t* s, const char* data) {
  str_t ss;
  char subfilename[MAX_PATH + 1];

  const char* start = data;
  const char* p = strstr(start, INCLUDE_XML);

  str_init(&ss, 1024);
  while (p != NULL) {
    /* 过滤在属性中的 INCLUDE_XML */
    xml_property_close_state_t close_state = xml_property_get_close_state(start, p);
    if (close_state == XML_PROPERTY_CLOSE_STATE_CLOSE) {
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
    } else {
      int size = 0;
      char* str_end = NULL;
      char* include_string_end = strstr(p, "?>");
      if (close_state == XML_PROPERTY_CLOSE_STATE_OPEN_PROPERTY) {
        str_end = TAG_PROPERTY;
        size = tk_strlen(TAG_PROPERTY);
      } else if (close_state == XML_PROPERTY_CLOSE_STATE_OPEN_SINGLE_QUOTE) {
        size = 1;
        str_end = "\'";
      } else if (close_state == XML_PROPERTY_CLOSE_STATE_OPEN_DOUDLE_QUOTE) {
        size = 1;
        str_end = "\"";
      }
      if (str_end == NULL) {
        log_error("do not find close property string !");
      } else {
        p = strstr(include_string_end, str_end) + size;
        str_append_with_len(s, start, p - start);
      }
    }

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

bool_t tk_str_end_with(const char* str, const char* appendix) {
  uint32_t len_str = 0;
  uint32_t len_appendix = 0;
  return_value_if_fail(str != NULL && appendix != NULL, FALSE);

  len_str = strlen(str);
  len_appendix = strlen(appendix);

  if (len_str < len_appendix) {
    return FALSE;
  } else {
    return strncmp(str + len_str - len_appendix, appendix, len_appendix) == 0;
  }
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

char* tk_str_totitle(char* str) {
  char* p = str;
  char* prev = str;
  return_value_if_fail(str != NULL, NULL);

  while (*p) {
    if (tk_isalpha(*p)) {
      if (p == str || (!tk_isalpha(*prev) && !tk_isdigit(*prev))) {
        *p = toupper(*p);
      }
    }
    prev = p;
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

ret_t image_region_parse(uint32_t img_w, uint32_t img_h, const char* region, rect_t* r) {
  return_value_if_fail(r != NULL && region != NULL, RET_BAD_PARAMS);
  if (*region == '#') {
    region++;
  }

  if (strncmp(region, "xywh(", 5) == 0) {
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    ENSURE(tk_sscanf(region, "xywh(%d,%d,%d,%d)", &x, &y, &w, &h) == 4);

    *r = rect_init(x, y, w, h);
    return RET_OK;
  } else if (strncmp(region, "grid(", 5) == 0) {
    int rows = 0;
    int cols = 0;
    int row = 0;
    int col = 0;
    int tile_w = 0;
    int tile_h = 0;
    ENSURE(tk_sscanf(region, "grid(%d,%d,%d,%d)", &rows, &cols, &row, &col) == 4);
    return_value_if_fail(rows > 0 && cols > 0 && row >= 0 && col >= 0, RET_FAIL);
    return_value_if_fail(rows > row && cols > col, RET_FAIL);

    tile_w = img_w / cols;
    tile_h = img_h / rows;

    *r = rect_init(col * tile_w, row * tile_h, tile_w, tile_h);
    return RET_OK;
  }

  return RET_FAIL;
}

typedef struct _to_json_ctx_t {
  tk_object_t* obj;
  str_t* str;
  uint32_t index;
} to_json_ctx_t;

static ret_t escape_json_str(str_t* str, const char* p) {
  str_append_char(str, '\"');
  if (p != NULL) {
    while (*p) {
      if (*p == '\"' || *p == '\\') {
        str_append_char(str, '\\');
      }
      str_append_char(str, *p);
      p++;
    }
  }
  str_append_char(str, '\"');

  return RET_OK;
}

static ret_t to_json_on_prop(void* ctx, const void* data) {
  named_value_t* nv = (named_value_t*)data;
  to_json_ctx_t* info = (to_json_ctx_t*)ctx;

  if (info->index > 0) {
    str_append_char(info->str, ',');
  }

  if (!tk_object_is_collection(info->obj)) {
    str_append_more(info->str, "\"", nv->name, "\":", NULL);
  }

  switch (nv->value.type) {
    case VALUE_TYPE_OBJECT: {
      str_t str;
      str_init(&str, 100);
      object_to_json(value_object(&(nv->value)), &str);
      str_append(info->str, str.str);
      str_reset(&str);
      break;
    }
    case VALUE_TYPE_STRING: {
      escape_json_str(info->str, value_str(&(nv->value)));
      break;
    }
    case VALUE_TYPE_WSTRING: {
      str_t str;
      str_init(&str, 100);
      str_from_wstr(&str, value_wstr(&(nv->value)));
      escape_json_str(info->str, str.str);
      str_reset(&str);
      break;
    }
    default: {
      char buff[32];
      str_append(info->str, value_str_ex(&(nv->value), buff, sizeof(buff) - 1));
      break;
    }
  }

  info->index++;
  return RET_OK;
}

ret_t object_to_json(tk_object_t* obj, str_t* str) {
  to_json_ctx_t ctx = {obj, str, 0};
  return_value_if_fail(obj != NULL && str != NULL, RET_BAD_PARAMS);

  if (tk_object_is_collection(obj)) {
    str_set(str, "[");
    tk_object_foreach_prop(obj, to_json_on_prop, &ctx);
    str_append_char(str, ']');
  } else {
    str_set(str, "{");
    tk_object_foreach_prop(obj, to_json_on_prop, &ctx);
    str_append_char(str, '}');
  }

  return RET_OK;
}

#ifdef WITH_DATA_READER_WRITER
ret_t data_url_copy(const char* dst_url, const char* src_url) {
  ret_t ret = RET_OK;
  return_value_if_fail(dst_url != NULL && src_url != NULL, RET_BAD_PARAMS);

  data_reader_t* reader = data_reader_factory_create_reader(data_reader_factory(), src_url);
  if (reader != NULL) {
    uint32_t size = data_reader_get_size(reader);
    if (size > 0) {
      data_writer_t* writer = data_writer_factory_create_writer(data_writer_factory(), dst_url);
      if (writer != NULL) {
        void* buff = TKMEM_CALLOC(1, size + 1);
        if (buff != NULL) {
          int32_t rsize = data_reader_read(reader, 0, buff, size);
          assert(rsize == size);
          rsize = data_writer_write(writer, 0, buff, rsize);
          assert(rsize == size);
          TKMEM_FREE(buff);
          log_debug("copy: %s=>%s\n", src_url, dst_url);
        } else {
          ret = RET_FAIL;
        }
        data_writer_destroy(writer);
      } else {
        ret = RET_FAIL;
        log_debug("open dst(%s) failed\n", dst_url);
      }
    } else {
      log_debug("open src(%s) failed\n", src_url);
    }
    data_reader_destroy(reader);
  }

  return ret;
}
#endif /*WITH_DATA_READER_WRITER*/

static void tk_quick_sort_impl(void** array, size_t left, size_t right, tk_compare_t cmp) {
  size_t save_left = left;
  size_t save_right = right;
  void* x = array[left];

  while (left < right) {
    while (cmp(array[right], x) >= 0 && left < right) right--;
    if (left != right) {
      array[left] = array[right];
      left++;
    }

    while (cmp(array[left], x) <= 0 && left < right) left++;
    if (left != right) {
      array[right] = array[left];
      right--;
    }
  }
  array[left] = x;

  if (save_left < left) {
    tk_quick_sort_impl(array, save_left, left - 1, cmp);
  }

  if (save_right > left) {
    tk_quick_sort_impl(array, left + 1, save_right, cmp);
  }

  return;
}

ret_t tk_qsort(void** array, size_t nr, tk_compare_t cmp) {
  ret_t ret = RET_OK;

  return_value_if_fail(array != NULL && cmp != NULL, RET_BAD_PARAMS);

  if (nr > 1) {
    tk_quick_sort_impl(array, 0, nr - 1, cmp);
  }

  return ret;
}

const char* tk_strrstr(const char* str, const char* substr) {
  char c = 0;
  uint32_t len = 0;
  const char* p = NULL;
  const char* end = NULL;
  return_value_if_fail(str != NULL && substr != NULL, NULL);

  c = *substr;
  len = strlen(substr);
  end = str + strlen(str) - 1;

  for (p = end; p >= str; p--) {
    if (*p == c) {
      if (strncmp(p, substr, len) == 0) {
        return p;
      }
    }
  }

  return NULL;
}

bool_t tk_str_is_in_array(const char* str, const char** str_array, uint32_t array_size) {
  uint32_t i = 0;
  return_value_if_fail(str != NULL && str_array != NULL && array_size > 0, FALSE);

  for (i = 0; i < array_size; i++) {
    if (tk_str_eq(str, str_array[i])) {
      return TRUE;
    }
  }

  return FALSE;
}

void* tk_memcpy_by_align_4(void* dst_align_4, const void* src_align_4, uint32_t len) {
#define MEMCPY_TINY_MAX_LENGTH 64
  if (dst_align_4 != NULL && src_align_4 != NULL && len > 0) {
    if (len < MEMCPY_TINY_MAX_LENGTH) {
      register unsigned char* dd = (unsigned char*)dst_align_4 + len;
      register const unsigned char* ss = (const unsigned char*)src_align_4 + len;

      assert(IS_ADDRESS_ALIGN_4(dst_align_4));
      assert(IS_ADDRESS_ALIGN_4(src_align_4));

      switch (len) {
        case 64:
          *((int*)(dd - 64)) = *((int*)(ss - 64));
        case 60:
          *((int*)(dd - 60)) = *((int*)(ss - 60));
        case 56:
          *((int*)(dd - 56)) = *((int*)(ss - 56));
        case 52:
          *((int*)(dd - 52)) = *((int*)(ss - 52));
        case 48:
          *((int*)(dd - 48)) = *((int*)(ss - 48));
        case 44:
          *((int*)(dd - 44)) = *((int*)(ss - 44));
        case 40:
          *((int*)(dd - 40)) = *((int*)(ss - 40));
        case 36:
          *((int*)(dd - 36)) = *((int*)(ss - 36));
        case 32:
          *((int*)(dd - 32)) = *((int*)(ss - 32));
        case 28:
          *((int*)(dd - 28)) = *((int*)(ss - 28));
        case 24:
          *((int*)(dd - 24)) = *((int*)(ss - 24));
        case 20:
          *((int*)(dd - 20)) = *((int*)(ss - 20));
        case 16:
          *((int*)(dd - 16)) = *((int*)(ss - 16));
        case 12:
          *((int*)(dd - 12)) = *((int*)(ss - 12));
        case 8:
          *((int*)(dd - 8)) = *((int*)(ss - 8));
        case 4:
          *((int*)(dd - 4)) = *((int*)(ss - 4));
          break;
        case 63:
          *((int*)(dd - 63)) = *((int*)(ss - 63));
        case 59:
          *((int*)(dd - 59)) = *((int*)(ss - 59));
        case 55:
          *((int*)(dd - 55)) = *((int*)(ss - 55));
        case 51:
          *((int*)(dd - 51)) = *((int*)(ss - 51));
        case 47:
          *((int*)(dd - 47)) = *((int*)(ss - 47));
        case 43:
          *((int*)(dd - 43)) = *((int*)(ss - 43));
        case 39:
          *((int*)(dd - 39)) = *((int*)(ss - 39));
        case 35:
          *((int*)(dd - 35)) = *((int*)(ss - 35));
        case 31:
          *((int*)(dd - 31)) = *((int*)(ss - 31));
        case 27:
          *((int*)(dd - 27)) = *((int*)(ss - 27));
        case 23:
          *((int*)(dd - 23)) = *((int*)(ss - 23));
        case 19:
          *((int*)(dd - 19)) = *((int*)(ss - 19));
        case 15:
          *((int*)(dd - 15)) = *((int*)(ss - 15));
        case 11:
          *((int*)(dd - 11)) = *((int*)(ss - 11));
        case 7:
          *((int*)(dd - 7)) = *((int*)(ss - 7));
          *((int*)(dd - 4)) = *((int*)(ss - 4));
          break;
        case 3:
          *((short*)(dd - 3)) = *((short*)(ss - 3));
          dd[-1] = ss[-1];
          break;
        case 62:
          *((int*)(dd - 62)) = *((int*)(ss - 62));
        case 58:
          *((int*)(dd - 58)) = *((int*)(ss - 58));
        case 54:
          *((int*)(dd - 54)) = *((int*)(ss - 54));
        case 50:
          *((int*)(dd - 50)) = *((int*)(ss - 50));
        case 46:
          *((int*)(dd - 46)) = *((int*)(ss - 46));
        case 42:
          *((int*)(dd - 42)) = *((int*)(ss - 42));
        case 38:
          *((int*)(dd - 38)) = *((int*)(ss - 38));
        case 34:
          *((int*)(dd - 34)) = *((int*)(ss - 34));
        case 30:
          *((int*)(dd - 30)) = *((int*)(ss - 30));
        case 26:
          *((int*)(dd - 26)) = *((int*)(ss - 26));
        case 22:
          *((int*)(dd - 22)) = *((int*)(ss - 22));
        case 18:
          *((int*)(dd - 18)) = *((int*)(ss - 18));
        case 14:
          *((int*)(dd - 14)) = *((int*)(ss - 14));
        case 10:
          *((int*)(dd - 10)) = *((int*)(ss - 10));
        case 6:
          *((int*)(dd - 6)) = *((int*)(ss - 6));
        case 2:
          *((short*)(dd - 2)) = *((short*)(ss - 2));
          break;
        case 61:
          *((int*)(dd - 61)) = *((int*)(ss - 61));
        case 57:
          *((int*)(dd - 57)) = *((int*)(ss - 57));
        case 53:
          *((int*)(dd - 53)) = *((int*)(ss - 53));
        case 49:
          *((int*)(dd - 49)) = *((int*)(ss - 49));
        case 45:
          *((int*)(dd - 45)) = *((int*)(ss - 45));
        case 41:
          *((int*)(dd - 41)) = *((int*)(ss - 41));
        case 37:
          *((int*)(dd - 37)) = *((int*)(ss - 37));
        case 33:
          *((int*)(dd - 33)) = *((int*)(ss - 33));
        case 29:
          *((int*)(dd - 29)) = *((int*)(ss - 29));
        case 25:
          *((int*)(dd - 25)) = *((int*)(ss - 25));
        case 21:
          *((int*)(dd - 21)) = *((int*)(ss - 21));
        case 17:
          *((int*)(dd - 17)) = *((int*)(ss - 17));
        case 13:
          *((int*)(dd - 13)) = *((int*)(ss - 13));
        case 9:
          *((int*)(dd - 9)) = *((int*)(ss - 9));
        case 5:
          *((int*)(dd - 5)) = *((int*)(ss - 5));
        case 1:
          dd[-1] = ss[-1];
          break;
        case 0:
        default:
          break;
      }
      return dd;
    }
  }
  return memcpy(dst_align_4, src_align_4, len);
}

void* tk_memcpy(void* dst, const void* src, uint32_t len) {
  size_t offset_src_address = ((size_t)src) & 0x3;
  size_t offset_dst_address = ((size_t)dst) & 0x3;
  return_value_if_fail(dst != NULL && src != NULL, NULL);
  if (offset_src_address == offset_dst_address) {
    uint32_t i = 0;
    uint8_t* d = dst;
    uint8_t* s = (uint8_t*)src;
    int32_t curr_len = len;
    uint32_t offset = 0x4 - offset_src_address;
    if (offset_src_address != 0) {
      for (i = 0; i < offset && i < len; i++) {
        d[i] = s[i];
      }
      d += offset;
      s += offset;
      curr_len -= offset;
    }
    if (curr_len > 0) {
      return tk_memcpy_by_align_4((void*)d, (const void*)s, curr_len);
    } else {
      return dst;
    }
  } else {
    return memcpy(dst, src, len);
  }
}

bool_t tk_wild_card_match(const char* pattern, const char* str) {
  const char* p_str = str;
  const char* p_pattern = pattern;
  return_value_if_fail(pattern != NULL && str != NULL, FALSE);

  while (*p_pattern && *p_str) {
    char c = *p_pattern;
    if (c == '*') {
      c = p_pattern[1];
      if (c == '\0') {
        break;
      }
      while (*p_str) {
        if (c == *p_str) {
          break;
        } else {
          p_str++;
        }
      }
    } else if (c == '?') {
      p_str++;
    } else {
      if (c != *p_str) {
        return FALSE;
      }
      p_str++;
    }
    p_pattern++;
  }

  return *p_pattern == '*' || *p_str == *p_pattern;
}
