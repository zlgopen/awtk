/**
 * File:   str.c
 * Author: AWTK Develop Team
 * Brief:  string
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
 * 2018-04-30 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "base/mem.h"
#include "base/utf8.h"
#include "base/str.h"
#include "base/utils.h"
#include "base/value.h"
#include "base/tokenizer.h"

static ret_t str_extend(str_t* str, uint32_t capacity) {
  if (capacity <= str->capacity) {
    return RET_OK;
  }

  if (capacity > 0) {
    char* s = TKMEM_REALLOC(char, str->str, capacity + 1);
    return_value_if_fail(s != NULL, RET_FAIL);

    s[capacity] = '\0';
    str->str = s;
    str->capacity = capacity;
    memset(s + str->size, 0x00, capacity + 1 - str->size);
  }

  return RET_OK;
}

str_t* str_init(str_t* str, uint32_t capacity) {
  return_value_if_fail(str != NULL, NULL);

  memset(str, 0x00, sizeof(str_t));

  return str_extend(str, capacity) == RET_OK ? str : NULL;
}

ret_t str_set(str_t* str, const char* text) {
  return str_set_with_len(str, text, 0xffff);
}

ret_t str_set_with_len(str_t* str, const char* text, uint32_t len) {
  uint32_t size = 0;
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  size = strlen(text);
  if (len <= size) {
    size = len;
  }
  return_value_if_fail(str_extend(str, size + 1) == RET_OK, RET_BAD_PARAMS);

  tk_strncpy(str->str, text, size);
  str->size = size;

  return RET_OK;
}

ret_t str_append_with_len(str_t* str, const char* text, uint32_t size) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_extend(str, str->size + size + 1) == RET_OK, RET_BAD_PARAMS);

  memcpy(str->str + str->size, text, size);
  str->size += size;
  str->str[str->size] = '\0';

  return RET_OK;
}

ret_t str_append(str_t* str, const char* text) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  return str_append_with_len(str, text, strlen(text));
}

ret_t str_append_char(str_t* str, char c) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_extend(str, str->size + 2) == RET_OK, RET_BAD_PARAMS);

  str->str[str->size++] = c;
  str->str[str->size] = '\0';

  return RET_OK;
}

ret_t str_decode_xml_entity_with_len(str_t* str, const char* text, uint32_t len) {
  char* d = NULL;
  const char* s = text;
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_extend(str, len + 1) == RET_OK, RET_OOM);

  d = str->str;
  str->size = 0;

  while (*s && (s - text) < len) {
    char c = *s++;
    if (c == '&') {
      if (strncmp(s, "lt;", 3) == 0) {
        c = '<';
        s += 3;
      } else if (strncmp(s, "gt;", 3) == 0) {
        c = '>';
        s += 3;
      } else if (strncmp(s, "amp;", 4) == 0) {
        c = '&';
        s += 4;
      } else if (strncmp(s, "quota;", 6) == 0) {
        c = '\"';
        s += 6;
      } else if (strncmp(s, "nbsp;", 5) == 0) {
        c = ' ';
        s += 5;
      }
    }
    *d++ = c;
  }
  *d = '\0';
  str->size = d - str->str;

  return RET_OK;
}

ret_t str_decode_xml_entity(str_t* str, const char* text) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  return str_decode_xml_entity_with_len(str, text, strlen(text));
}

bool_t str_eq(str_t* str, const char* text) {
  if ((str == NULL && text == NULL) || (str != NULL && str->str == NULL && text == NULL)) {
    return TRUE;
  }

  if (str == NULL || text == NULL || str->str == NULL) {
    return FALSE;
  }

  if (str->str[0] != text[0]) {
    return FALSE;
  }

  return strcmp(str->str, text) == 0;
}

ret_t str_from_int(str_t* str, int32_t v) {
  char buff[TK_NUM_MAX_LEN + 1];
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  return str_set(str, tk_itoa(buff, sizeof(buff), v));
}

ret_t str_from_float(str_t* str, float v) {
  char buff[TK_NUM_MAX_LEN + 1];
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  return str_set(str, tk_ftoa(buff, sizeof(buff), v));
}

ret_t str_from_value(str_t* str, const value_t* v) {
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);

  if (v->type == VALUE_TYPE_STRING) {
    return str_set(str, value_str(v));
  } else if (v->type == VALUE_TYPE_WSTRING) {
    return str_from_wstr(str, value_wstr(v));
  } else if (v->type == VALUE_TYPE_FLOAT) {
    return str_from_float(str, value_float(v));
  } else if (v->type == VALUE_TYPE_BOOL) {
    return str_set(str, value_bool(v) ? "true" : "false");
  } else {
    return str_from_int(str, value_int(v));
  }
}

ret_t str_from_wstr(str_t* str, const wchar_t* wstr) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  str->size = 0;
  if (str->str != NULL) {
    str->str[0] = '\0';
  }

  if (wstr != NULL) {
    uint32_t size = wcslen(wstr) * 3;
    return_value_if_fail(str_extend(str, size + 1) == RET_OK, RET_OOM);

    utf8_from_utf16(wstr, str->str, size);
    str->size = strlen(str->str);
  }

  return RET_OK;
}

ret_t str_to_int(str_t* str, int32_t* v) {
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);
  *v = tk_atoi(str->str);

  return RET_OK;
}

ret_t str_to_float(str_t* str, float* v) {
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);
  *v = tk_atof(str->str);

  return RET_OK;
}

ret_t str_reset(str_t* str) {
  return_value_if_fail(str != NULL, RET_OK);
  TKMEM_FREE(str->str);
  memset(str, 0x00, sizeof(str_t));

  return RET_OK;
}

bool_t str_end_with(str_t* s, const char* str) {
  size_t len = 0;
  return_value_if_fail(s != NULL && s->str != NULL && str != NULL, FALSE);

  len = strlen(str);
  if (len > s->size) {
    return FALSE;
  }

  return strncmp(s->str + s->size - len, str, len) == 0;
}

bool_t str_start_with(str_t* s, const char* str) {
  return_value_if_fail(s != NULL && s->str != NULL && str != NULL, FALSE);

  return strncmp(s->str, str, strlen(str)) == 0;
}

ret_t str_trim_left(str_t* s, const char* str) {
  uint32_t i = 0;
  uint32_t k = 0;
  char* p = s->str;
  uint32_t n = s->size;
  return_value_if_fail(s != NULL && s->str != NULL, RET_BAD_PARAMS);

  if (!s->size) {
    return RET_OK;
  }

  if (str == NULL) {
    str = " ";
  }

  for (i = 0; i < n; i++) {
    char c = p[i];
    if (strchr(str, c) == NULL) {
      break;
    }
  }

  s->size = n - i;
  for (k = 0; i < n; i++, k++) {
    p[k] = p[i];
  }
  s->str[s->size] = '\0';

  return RET_OK;
}

ret_t str_trim_right(str_t* s, const char* str) {
  int32_t i = 0;
  char* p = s->str;
  uint32_t n = s->size;
  return_value_if_fail(s != NULL && s->str != NULL, RET_BAD_PARAMS);

  if (!s->size) {
    return RET_OK;
  }

  if (str == NULL) {
    str = " ";
  }

  for (i = n - 1; i >= 0; i--) {
    char c = p[i];
    if (strchr(str, c) == NULL) {
      break;
    }
  }

  p[i + 1] = '\0';
  s->size = i + 1;

  return RET_OK;
}

ret_t str_trim(str_t* s, const char* str) {
  if (!s->size) {
    return RET_OK;
  }

  str_trim_left(s, str);

  return str_trim_right(s, str);
}

static uint32_t str_count_sub_str(str_t* s, const char* str) {
  char* p = s->str;
  uint32_t count = 0;
  uint32_t size = strlen(str);

  do {
    p = strstr(p, str);
    if (p != NULL) {
      count++;
      p += size;
    }
  } while (p != NULL);

  return count;
}

ret_t str_replace(str_t* s, const char* str, const char* new_str) {
  uint32_t count = 0;

  return_value_if_fail(s != NULL && s->str != NULL && str != NULL && new_str != NULL,
                       RET_BAD_PARAMS);

  count = str_count_sub_str(s, str);

  if (count > 0) {
    char* p = s->str;
    char* src = s->str;
    uint32_t str_len = strlen(str);
    uint32_t new_str_len = strlen(new_str);
    uint32_t capacity = s->size + count * (strlen(new_str) - strlen(str)) + 1;

    char* temp_str = (char*)TKMEM_ALLOC(capacity);
    char* dst = temp_str;
    return_value_if_fail(temp_str != NULL, RET_OOM);
    do {
      uint32_t size = 0;
      p = strstr(src, str);
      if (p != NULL) {
        size = (uint32_t)(p - src);
      } else {
        size = (uint32_t)strlen(src);
      }
      memcpy(dst, src, size);
      src += size;
      dst += size;
      if (p != NULL) {
        if (new_str_len > 0) {
          memcpy(dst, new_str, new_str_len);
          dst += new_str_len;
        }
        src += str_len;
      }
      *dst = '\0';
    } while (p != NULL);

    TKMEM_FREE(s->str);
    s->str = temp_str;
    s->size = strlen(s->str);
    s->capacity = capacity;
  }

  return RET_OK;
}

ret_t str_to_lower(str_t* s) {
  uint32_t i = 0;
  char* p = s->str;
  uint32_t n = s->size;

  return_value_if_fail(s != NULL && s->str != NULL, RET_BAD_PARAMS);

  for (i = 0; i < n; i++) {
    p[i] = tolower(p[i]);
  }

  return RET_OK;
}

ret_t str_to_upper(str_t* s) {
  uint32_t i = 0;
  char* p = s->str;
  uint32_t n = s->size;

  return_value_if_fail(s != NULL && s->str != NULL, RET_BAD_PARAMS);

  for (i = 0; i < n; i++) {
    p[i] = toupper(p[i]);
  }

  return RET_OK;
}

ret_t str_insert_with_len(str_t* s, uint32_t offset, const char* text, uint32_t size) {
  return_value_if_fail(s != NULL && offset <= s->size && text != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_extend(s, s->size + size + 1) == RET_OK, RET_OOM);

  memmove(s->str + offset + size, s->str + offset, strlen(s->str + offset));
  memcpy(s->str + offset, text, size);
  s->size += size;
  s->str[s->size] = '\0';

  return RET_OK;
}

ret_t str_insert(str_t* s, uint32_t offset, const char* text) {
  return_value_if_fail(s != NULL && offset <= s->size && text != NULL, RET_BAD_PARAMS);

  return str_insert_with_len(s, offset, text, strlen(text));
}

ret_t str_remove(str_t* s, uint32_t offset, uint32_t size) {
  return_value_if_fail(s != NULL && (offset + size) <= s->size && size > 0, RET_BAD_PARAMS);

  memmove(s->str + offset, s->str + offset + size, strlen(s->str + offset + size));
  s->size -= size;
  s->str[s->size] = '\0';

  return RET_OK;
}
