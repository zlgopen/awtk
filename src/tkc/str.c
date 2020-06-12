/**
 * File:   str.c
 * Author: AWTK Develop Team
 * Brief:  string
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
 * 2018-04-30 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/str.h"
#include "tkc/utils.h"
#include "tkc/value.h"
#include "tkc/object.h"
#include "tkc/tokenizer.h"

ret_t str_extend(str_t* str, uint32_t capacity) {
  if (capacity <= str->capacity) {
    return RET_OK;
  }

  if (capacity > 0) {
    char* s = TKMEM_REALLOCT(char, str->str, capacity + 1);
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

ret_t str_clear(str_t* str) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  str->size = 0;
  if (str->str != NULL) {
    str->str[0] = '\0';
  }

  return RET_OK;
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
  str->str[str->size] = '\0';

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

ret_t str_append_more(str_t* str, const char* text, ...) {
  va_list va;
  const char* p = NULL;
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_append(str, text) == RET_OK, RET_OOM);

  va_start(va, text);
  do {
    p = va_arg(va, char*);
    if (p != NULL) {
      return_value_if_fail(str_append(str, p) == RET_OK, RET_OOM);
    } else {
      break;
    }
  } while (p != NULL);

  va_end(va);

  return RET_OK;
}

ret_t str_append_int(str_t* str, int32_t value) {
  char num[32];
  tk_snprintf(num, sizeof(num), "%d", value);

  return str_append(str, num);
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
      } else if (strncmp(s, "quot;", 5) == 0) {
        c = '\"';
        s += 5;
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

ret_t str_unescape(str_t* str) {
  char* s = NULL;
  char* d = NULL;
  return_value_if_fail(str != NULL && str->str != NULL, RET_BAD_PARAMS);
  s = str->str;
  d = str->str;

  while ((s - str->str) < str->size) {
    char c = *s++;

    if (c == '\\') {
      switch (*s++) {
        case 'n': {
          c = '\n';
          break;
        }
        case 'r': {
          c = '\r';
          break;
        }
        case 't': {
          c = '\t';
          break;
        }
        case '\'': {
          c = '\'';
          break;
        }
        case '\"': {
          c = '\"';
          break;
        }
        case '\\': {
          c = '\\';
          break;
        }
        case '\0': {
          break;
        }
        default: {
          log_warn("not support char: %s [%c]\n", str->str, *s);
          break;
        }
      }
    }
    *d++ = c;
  }
  *d = '\0';
  str->size = d - str->str;

  return RET_OK;
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

ret_t str_from_float(str_t* str, double v) {
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
  } else if (v->type == VALUE_TYPE_FLOAT || v->type == VALUE_TYPE_FLOAT32 ||
             v->type == VALUE_TYPE_DOUBLE) {
    return str_from_float(str, value_float(v));
  } else if (v->type == VALUE_TYPE_BOOL) {
    return str_set(str, value_bool(v) ? "true" : "false");
  } else {
    return str_from_int(str, value_int(v));
  }
}

ret_t str_from_wstr_with_len(str_t* str, const wchar_t* wstr, uint32_t len) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  str->size = 0;
  if (str->str != NULL) {
    str->str[0] = '\0';
  }

  if (wstr != NULL) {
    uint32_t size = len * 4 + 1;
    return_value_if_fail(str_extend(str, size + 1) == RET_OK, RET_OOM);

    if (size > 0) {
      tk_utf8_from_utf16_ex(wstr, len, str->str, size);
      str->size = strlen(str->str);
    } else {
      str_set(str, "");
    }
  }

  return RET_OK;
}

ret_t str_from_wstr(str_t* str, const wchar_t* wstr) {
  return_value_if_fail(str != NULL && wstr != NULL, RET_BAD_PARAMS);

  return str_from_wstr_with_len(str, wstr, wcslen(wstr));
}

ret_t str_to_int(str_t* str, int32_t* v) {
  return_value_if_fail(str != NULL && v != NULL, RET_BAD_PARAMS);
  *v = tk_atoi(str->str);

  return RET_OK;
}

ret_t str_to_float(str_t* str, double* v) {
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

bool_t str_end_with(str_t* str, const char* text) {
  size_t len = 0;
  return_value_if_fail(str != NULL && str->str != NULL && text != NULL, FALSE);

  len = strlen(text);
  if (len > str->size) {
    return FALSE;
  }

  return strncmp(str->str + str->size - len, text, len) == 0;
}

bool_t str_start_with(str_t* str, const char* text) {
  return_value_if_fail(str != NULL && str->str != NULL && text != NULL, FALSE);

  return strncmp(str->str, text, strlen(text)) == 0;
}

ret_t str_trim_left(str_t* str, const char* text) {
  uint32_t i = 0;
  uint32_t k = 0;
  char* p = str->str;
  uint32_t n = str->size;
  return_value_if_fail(str != NULL && str->str != NULL, RET_BAD_PARAMS);

  if (!str->size) {
    return RET_OK;
  }

  if (text == NULL) {
    text = " ";
  }

  for (i = 0; i < n; i++) {
    char c = p[i];
    if (strchr(text, c) == NULL) {
      break;
    }
  }

  str->size = n - i;
  for (k = 0; i < n; i++, k++) {
    p[k] = p[i];
  }
  str->str[str->size] = '\0';

  return RET_OK;
}

ret_t str_trim_right(str_t* str, const char* text) {
  int32_t i = 0;
  char* p = str->str;
  uint32_t n = str->size;
  return_value_if_fail(str != NULL && str->str != NULL, RET_BAD_PARAMS);

  if (!str->size) {
    return RET_OK;
  }

  if (text == NULL) {
    text = " ";
  }

  for (i = n - 1; i >= 0; i--) {
    char c = p[i];
    if (strchr(text, c) == NULL) {
      break;
    }
  }

  p[i + 1] = '\0';
  str->size = i + 1;

  return RET_OK;
}

ret_t str_trim(str_t* str, const char* text) {
  if (!str->size) {
    return RET_OK;
  }

  str_trim_left(str, text);

  return str_trim_right(str, text);
}

static uint32_t str_count_sub_str(str_t* s, const char* str) {
  char* p = s->str;
  uint32_t count = 0;
  uint32_t size = strlen(str);

  if (size == 0) {
    return 0;
  }

  do {
    p = strstr(p, str);
    if (p != NULL) {
      count++;
      p += size;
    }
  } while (p != NULL);

  return count;
}

ret_t str_replace(str_t* str, const char* text, const char* new_text) {
  uint32_t count = 0;

  return_value_if_fail(str != NULL && str->str != NULL && text != NULL && new_text != NULL,
                       RET_BAD_PARAMS);

  count = str_count_sub_str(str, text);

  if (count > 0) {
    char* p = str->str;
    char* src = str->str;
    uint32_t str_len = strlen(text);
    uint32_t new_text_len = strlen(new_text);
    uint32_t capacity = str->size + count * (strlen(new_text) - strlen(text)) + 1;

    char* temp_str = (char*)TKMEM_ALLOC(capacity);
    char* dst = temp_str;
    return_value_if_fail(temp_str != NULL, RET_OOM);
    do {
      uint32_t size = 0;
      p = strstr(src, text);
      if (p != NULL) {
        size = (uint32_t)(p - src);
      } else {
        size = (uint32_t)strlen(src);
      }
      memcpy(dst, src, size);
      src += size;
      dst += size;
      if (p != NULL) {
        if (new_text_len > 0) {
          memcpy(dst, new_text, new_text_len);
          dst += new_text_len;
        }
        src += str_len;
      }
      *dst = '\0';
    } while (p != NULL);

    TKMEM_FREE(str->str);
    str->str = temp_str;
    str->size = strlen(str->str);
    str->capacity = capacity;
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

static const char* expand_var(str_t* str, const char* p, const object_t* obj) {
  value_t v;
  uint32_t len = 0;
  char name[TK_NAME_LEN + 1];
  const char* end = strchr(p, '}');
  return_value_if_fail(end != NULL, p);

  len = end - p;
  return_value_if_fail(len <= TK_NAME_LEN, end + 1);

  tk_strncpy(name, p, len);
  if (object_eval((object_t*)obj, name, &v) != RET_OK) {
    value_reset(&v);

    return end + 1;
  }

  if (v.type == VALUE_TYPE_STRING) {
    str_append(str, value_str(&v));
  } else {
    char num[TK_NUM_MAX_LEN + 1];
    tk_snprintf(num, TK_NUM_MAX_LEN, "%d", value_int(&v));
    str_append(str, num);
  }
  value_reset(&v);

  return end + 1;
}

ret_t str_expand_vars(str_t* str, const char* src, const object_t* obj) {
  const char* p = src;
  return_value_if_fail(str != NULL && src != NULL && obj != NULL, RET_BAD_PARAMS);

  while (*p) {
    char c = *p;

    if (c == '$') {
      if (p[1] && p[2]) {
        p = expand_var(str, p + 2, obj);
      } else {
        return RET_BAD_PARAMS;
      }
    } else {
      str_append_char(str, c);
      p++;
    }
  }

  return RET_OK;
}

ret_t str_pop(str_t* str) {
  return_value_if_fail(str != NULL && str->size > 0, RET_BAD_PARAMS);

  str->size--;
  str->str[str->size] = '\0';

  return RET_OK;
}

ret_t str_append_double(str_t* str, const char* format, double value) {
  char buff[64];
  const char* fmt = format != NULL ? format : "%.4lf";
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  tk_snprintf(buff, sizeof(buff), fmt, value);

  return str_append(str, buff);
}

ret_t str_append_json_str(str_t* str, const char* json_str) {
  const char* p = json_str;
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_append_char(str, '\"') == RET_OK, RET_OOM);
  if (p != NULL) {
    while (*p) {
      if (*p == '\"') {
        return_value_if_fail(str_append_char(str, '\\') == RET_OK, RET_OOM);
      }
      return_value_if_fail(str_append_char(str, *p) == RET_OK, RET_OOM);
      p++;
    }
  }
  return_value_if_fail(str_append_char(str, '\"') == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t str_append_json_int_pair(str_t* str, const char* key, int32_t value) {
  return_value_if_fail(str != NULL && key != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_append_json_str(str, key) == RET_OK, RET_OOM);
  return_value_if_fail(str_append_char(str, ':') == RET_OK, RET_OOM);
  return_value_if_fail(str_append_int(str, value) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t str_append_json_str_pair(str_t* str, const char* key, const char* value) {
  return_value_if_fail(str != NULL && key != NULL && value != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_append_json_str(str, key) == RET_OK, RET_OOM);
  return_value_if_fail(str_append_char(str, ':') == RET_OK, RET_OOM);
  return_value_if_fail(str_append_json_str(str, value) == RET_OK, RET_OOM);

  return RET_OK;
}

ret_t str_append_json_double_pair(str_t* str, const char* key, double value) {
  return_value_if_fail(str != NULL && key != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_append_json_str(str, key) == RET_OK, RET_OOM);
  return_value_if_fail(str_append_char(str, ':') == RET_OK, RET_OOM);
  return_value_if_fail(str_append_double(str, NULL, value) == RET_OK, RET_OOM);

  return RET_OK;
}
ret_t str_append_json_bool_pair(str_t* str, const char* key, bool_t value) {
  return_value_if_fail(str != NULL && key != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_append_json_str(str, key) == RET_OK, RET_OOM);
  return_value_if_fail(str_append_char(str, ':') == RET_OK, RET_OOM);
  return_value_if_fail(str_append(str, value ? "true" : "false") == RET_OK, RET_OOM);

  return RET_OK;
}
