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

#include "base/utils.h"

int ftk_str2bool(const char* str) {
  if (str == NULL || str[0] == '0' || strcmp(str, "false") == 0 || strcmp(str, "no") == 0) {
    return 0;
  }

  return 1;
}

static long ftk_strtol_internal(const char* str, const char** end, int base) {
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

long ftk_strtol(const char* str, const char** end, int base) {
  long n = 0;
  int neg = 0;
  return_value_if_fail(str != NULL, 0);

  while (*str == ' ' || *str == '\t') str++;

  if (*str == '+' || *str == '-') {
    neg = *str == '-';
    str++;
  }

  n = ftk_strtol_internal(str, end, base);

  return neg ? -n : n;
}

int ftk_atoi(const char* str) { return ftk_strtol(str, NULL, 10); }

float_t ftk_atof(const char* str) {
  int n = 0;
  unsigned int f = 0;
  int neg = 0;
  float_t result = 0;
  const char* p = NULL;
  return_value_if_fail(str != NULL, 0);

  if (str[0] == '+' || str[0] == '-') {
    neg = str[0] == '-';
    str++;
  }

  n = ftk_strtol_internal(str, &p, 10);

  if (p != NULL && *p == '.') {
    f = ftk_strtol_internal(p + 1, NULL, 10);
  }

  result = f;
  while (result >= 1) {
    result = result / 10;
  }

  result = n + result;

  return neg ? -result : result;
}

static const char* ftk_itoa_simple(char* str, int len, int n, const char** end) {
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

const char* ftk_itoa(char* str, int len, int n) { return ftk_itoa_simple(str, len, n, NULL); }

const char* ftk_ftoa(char* str, int len, float_t value) {
  int i = 0;
  char str_n[32] = {0};
  char str_f[32] = {0};
  int n = (int)value;
  int f = (int)((value - n) * 1000000000);

  ftk_itoa(str_n, sizeof(str_n), n);
  ftk_itoa(str_f, sizeof(str_f), f > 0 ? f : -f);

  if (f == 0) {
    strncpy(str, str_n, len);

    return str;
  }

  i = strlen(str_f) - 1;
  i = i > 6 ? 6 : i;
  str_f[i] = '\0';

  while (i > 0) {
    if (str_f[i] == '0') {
      str_f[i] = '\0';
    }
    i--;
  }
  return_value_if_fail(len > (strlen(str_n) + 1 + i), NULL);

  if (len > (strlen(str_n) + strlen(str_f) + 2)) {
    strcpy(str, str_n);
    strcpy(str + strlen(str), ".");
    strcpy(str + strlen(str), str_f);

    return str;
  }

  return NULL;
}

char* ftk_strcpy(char* dst, const char* src) { return strcpy(dst, src); }
