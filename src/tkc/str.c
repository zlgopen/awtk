/**
 * File:   str.c
 * Author: AWTK Develop Team
 * Brief:  string
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

str_t* str_create(uint32_t capacity) {
  str_t* str = TKMEM_ZALLOC(str_t);
  return_value_if_fail(str != NULL, NULL);

  return str_init(str, capacity);
}

ret_t str_destroy(str_t* str) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  str_reset(str);
  TKMEM_FREE(str);

  return RET_OK;
}

ret_t str_extend(str_t* str, uint32_t capacity) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  if (capacity <= str->capacity) {
    return RET_OK;
  }

  if (!str->extendable) {
    return RET_FAIL;
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

ret_t str_shrink(str_t* str, uint32_t size) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  if (size < str->size) {
    str->size = size;
    str->str[size] = '\0';
  }

  return RET_OK;
}

str_t* str_init(str_t* str, uint32_t capacity) {
  return_value_if_fail(str != NULL, NULL);

  memset(str, 0x00, sizeof(str_t));
  str->extendable = TRUE;

  return str_extend(str, capacity) == RET_OK ? str : NULL;
}

str_t* str_attach_with_size(str_t* str, char* buff, uint32_t size, uint32_t capacity) {
  return_value_if_fail(str != NULL && buff != NULL && capacity > 0, NULL);
  return_value_if_fail(size < capacity, NULL);

  memset(str, 0x00, sizeof(str_t));

  str->str = buff;
  str->size = size;
  str->capacity = capacity;
  str->extendable = FALSE;
  buff[size] = '\0';

  return str;
}

str_t* str_attach(str_t* str, char* buff, uint32_t capacity) {
  return_value_if_fail(str != NULL && buff != NULL && capacity > 0, NULL);

  memset(str, 0x00, sizeof(str_t));
  memset(buff, 0x00, capacity);

  return str_attach_with_size(str, buff, 0, capacity);
}

ret_t str_set(str_t* str, const char* text) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  return str_set_with_len(str, text, tk_strlen(text));
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
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  return_value_if_fail(str_extend(str, len + 1) == RET_OK, RET_BAD_PARAMS);

  tk_strncpy(str->str, text, len);
  str->size = len;
  str->str[str->size] = '\0';
  str->size = tk_strlen(str->str);

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

  return str_append_with_len(str, text, tk_strlen(text));
}

ret_t str_append_wchar_with_len(str_t* str, const wchar_t* text, uint32_t len) {
  uint32_t size = len * 6 + 1;
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_extend(str, str->size + size + 1) == RET_OK, RET_OOM);

  tk_utf8_from_utf16_ex(text, len, str->str + str->size, str->capacity - str->size - 1);
  str->size += tk_strlen(str->str + str->size);
  return RET_OK;
}

ret_t str_append_wchar(str_t* str, const wchar_t* text) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  return str_append_wchar_with_len(str, text, wcslen(text));
}

ret_t str_append_uppercase(str_t* str, const char* text) {
  ret_t ret = RET_OK;
  const char* p = text;
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  while (*p) {
    ret = str_append_char(str, toupper(*p));
    break_if_fail(ret == RET_OK);
    p++;
  }

  return ret;
}

ret_t str_append_lowercase(str_t* str, const char* text) {
  ret_t ret = RET_OK;
  const char* p = text;
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  while (*p) {
    ret = str_append_char(str, tolower(*p));
    break_if_fail(ret == RET_OK);
    p++;
  }

  return ret;
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
  char num[32] = {0};
  tk_snprintf(num, sizeof(num) - 1, "%d", value);

  return str_append(str, num);
}

ret_t str_append_uint32(str_t* str, uint32_t value) {
  char num[32] = {0};
  tk_snprintf(num, sizeof(num) - 1, "%u", value);

  return str_append(str, num);
}

ret_t str_append_int64(str_t* str, int64_t value) {
  char num[64] = {0};
  tk_snprintf(num, sizeof(num) - 1, "%" PRId64, value);

  return str_append(str, num);
}

ret_t str_append_uint64(str_t* str, uint64_t value) {
  char num[64] = {0};
  tk_snprintf(num, sizeof(num) - 1, "%" PRIu64, value);

  return str_append(str, num);
}

ret_t str_from_int64(str_t* str, int64_t value) {
  char num[64] = {0};
  tk_snprintf(num, sizeof(num) - 1, "%" PRId64, value);

  return str_set(str, num);
}

ret_t str_from_uint64(str_t* str, uint64_t value) {
  char num[64] = {0};
  tk_snprintf(num, sizeof(num) - 1, "%" PRIu64, value);

  return str_set(str, num);
}

ret_t str_append_char(str_t* str, char c) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_extend(str, str->size + 2) == RET_OK, RET_BAD_PARAMS);

  str->str[str->size++] = c;
  str->str[str->size] = '\0';

  return RET_OK;
}

ret_t str_append_n_chars(str_t* str, char c, uint32_t n) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_extend(str, str->size + n + 1) == RET_OK, RET_BAD_PARAMS);

  memset(str->str + str->size, c, n);
  str->size += n;
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
      if (tk_strncmp(s, "lt;", 3) == 0) {
        c = '<';
        s += 3;
      } else if (tk_strncmp(s, "gt;", 3) == 0) {
        c = '>';
        s += 3;
      } else if (tk_strncmp(s, "amp;", 4) == 0) {
        c = '&';
        s += 4;
      } else if (tk_strncmp(s, "quot;", 5) == 0 || tk_strncmp(s, "quota;", 6) == 0) {
        c = '\"';
        s += 5;
      } else if (tk_strncmp(s, "nbsp;", 5) == 0) {
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

  return str_decode_xml_entity_with_len(str, text, tk_strlen(text));
}

ret_t str_encode_xml_entity(str_t* str, const char* text) {
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);
  return str_encode_xml_entity_with_len(str, text, tk_strlen(text));
}

ret_t str_encode_xml_entity_with_len(str_t* str, const char* text, uint32_t len) {
  uint32_t i = 0;
  return_value_if_fail(str != NULL && text != NULL, RET_BAD_PARAMS);

  for (i = 0; i < len; i++) {
    char c = text[i];
    switch (c) {
      case '<': {
        return_value_if_fail(str_append(str, "&lt;") == RET_OK, RET_OOM);
        break;
      }
      case '>': {
        return_value_if_fail(str_append(str, "&gt;") == RET_OK, RET_OOM);
        break;
      }
      case '&': {
        return_value_if_fail(str_append(str, "&amp;") == RET_OK, RET_OOM);
        break;
      }
      case '"': {
        return_value_if_fail(str_append(str, "&quot;") == RET_OK, RET_OOM);
        break;
      }
      default: {
        return_value_if_fail(str_append_char(str, c) == RET_OK, RET_OOM);
        break;
      }
    }
  }

  return RET_OK;
}

/*https://en.wikipedia.org/wiki/Escape_sequences_in_C*/
typedef struct _str_char_escape_pair_t {
  char escape;
  char unescape;
} str_char_escape_pair_t;

static str_char_escape_pair_t s_str_char_escape_pairs[] = {
    {'n', '\n'}, {'t', '\t'}, {'r', '\r'}, {'b', '\b'},
    {'f', '\f'}, {'a', '\a'}, {'v', '\v'}, {'e', '\033'},
};

char str_escape_char(char c) {
  char ret = c;
  uint32_t i = 0;
  for (i = 0; i < ARRAY_SIZE(s_str_char_escape_pairs); i++) {
    if (s_str_char_escape_pairs[i].unescape == c) {
      ret = s_str_char_escape_pairs[i].escape;
      break;
    }
  }
  return ret;
}

char str_unescape_char(const char* s, uint32_t* nr) {
  char c = 0;
  const char* start = s;
  return_value_if_fail(s != NULL && nr != NULL, 0);

  switch (*s++) {
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
    case '?': {
      c = '\?';
      break;
    }
    case 'x': {
      int32_t v = 0;
      tk_sscanf(s, "%02x", &v);
      c = v;
      s += 2;
      break;
    }
    case '\0': {
      c = '\\';
      break;
    }
    default: {
      uint32_t i = 0;
      for (i = 0; i < ARRAY_SIZE(s_str_char_escape_pairs); i++) {
        if (s_str_char_escape_pairs[i].escape == *start) {
          c = s_str_char_escape_pairs[i].unescape;
          break;
        }
      }
      if (0 == c) {
        log_warn("not support char: [%c]\n", *start);
      }
      break;
    }
  }

  *nr = s - start;
  return c;
}

ret_t str_append_unescape(str_t* str, const char* s, uint32_t size) {
  uint32_t i = 0;
  return_value_if_fail(str != NULL && s != NULL, RET_BAD_PARAMS);

  size = tk_min_int(tk_strlen(s), size);
  for (i = 0; i < size; i++) {
    char c = *s++;
    if (c == '\\') {
      uint32_t nr = 0;
      c = str_unescape_char(s, &nr);
      i += nr;
      s += nr;
    }

    str_append_char(str, c);
  }

  return RET_OK;
}

ret_t str_append_escape(str_t* str, const char* s, uint32_t size) {
  uint32_t i = 0;
  return_value_if_fail(str != NULL && s != NULL, RET_BAD_PARAMS);

  size = tk_min_int(tk_strlen(s), size);
  for (i = 0; i < size; i++) {
    char c = str_escape_char(s[i]);
    if (c != s[i] || c == '\\' || c == '\'' || c == '\"') {
      str_append_char(str, '\\');
    }
    str_append_char(str, c);
  }

  return RET_OK;
}

ret_t str_escape(str_t* str) {
  uint32_t i = 0;
  return_value_if_fail(str != NULL && str->str != NULL, RET_BAD_PARAMS);

  for (i = 0; i < str->size; i++) {
    char c = str_escape_char(str->str[i]);
    if (c != str->str[i] || c == '\\' || c == '\'' || c == '\"') {
      str_insert_with_len(str, i, "\\", 1);
      i++;
    }
    str->str[i] = c;
  }

  return RET_OK;
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
      uint32_t nr = 0;
      c = str_unescape_char(s, &nr);
      s += nr;
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

  return tk_strcmp(str->str, text) == 0;
}

bool_t str_equal(str_t* str, str_t* other) {
  if (str == other) {
    return TRUE;
  }

  if (str == NULL || other == NULL) {
    return FALSE;
  }

  if (str->str == other->str) {
    return TRUE;
  }

  if (str->str == NULL || other->str == NULL) {
    return FALSE;
  }

  return tk_strcmp(str->str, other->str) == 0;
}

ret_t str_from_int(str_t* str, int32_t value) {
  char buff[TK_NUM_MAX_LEN + 1];
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  return str_set(str, tk_itoa(buff, sizeof(buff), value));
}

ret_t str_from_uint32(str_t* str, uint32_t value) {
  char num[32] = {0};
  tk_snprintf(num, sizeof(num) - 1, "%u", value);

  return str_set(str, num);
}

ret_t str_from_float(str_t* str, double value) {
  char buff[TK_NUM_MAX_LEN + 1];
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  return str_set(str, tk_ftoa(buff, sizeof(buff), value));
}

ret_t str_from_value(str_t* str, const value_t* value) {
  return_value_if_fail(str != NULL && value != NULL, RET_BAD_PARAMS);

  if (value->type == VALUE_TYPE_STRING) {
    return str_set(str, value_str(value));
  } else if (value->type == VALUE_TYPE_WSTRING) {
    return str_from_wstr(str, value_wstr(value));
  } else if (value->type == VALUE_TYPE_FLOAT || value->type == VALUE_TYPE_FLOAT32 ||
             value->type == VALUE_TYPE_DOUBLE) {
    return str_from_float(str, value_float(value));
  } else if (value->type == VALUE_TYPE_BOOL) {
    return str_set(str, value_bool(value) ? "true" : "false");
  } else {
    return str_from_int(str, value_int(value));
  }
}

ret_t str_from_wstr_with_len(str_t* str, const wchar_t* wstr, uint32_t len) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  str->size = 0;
  if (str->str != NULL) {
    str->str[0] = '\0';
  }

  if (wstr != NULL) {
    uint32_t size = len * 6 + 1;
    return_value_if_fail(str_extend(str, size + 1) == RET_OK, RET_OOM);

    if (size > 0) {
      tk_utf8_from_utf16_ex(wstr, len, str->str, size);
      str->size = tk_strlen(str->str);
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
  if (str->extendable) {
    TKMEM_FREE(str->str);
  }
  str->str = NULL;
  str->size = 0;
  str->capacity = 0;

  return RET_OK;
}

bool_t str_end_with(str_t* str, const char* text) {
  size_t len = 0;
  return_value_if_fail(str != NULL && str->str != NULL && text != NULL, FALSE);

  len = tk_strlen(text);
  if (len > str->size) {
    return FALSE;
  }

  return tk_strncmp(str->str + str->size - len, text, len) == 0;
}

bool_t str_start_with(str_t* str, const char* text) {
  return_value_if_fail(str != NULL && str->str != NULL && text != NULL, FALSE);

  return tk_strncmp(str->str, text, tk_strlen(text)) == 0;
}

ret_t str_trim_left(str_t* str, const char* text) {
  return_value_if_fail(text != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str != NULL && str->str != NULL, RET_BAD_PARAMS);

  if (!str->size) {
    return RET_OK;
  }

  tk_str_trim_left(str->str, text);
  str->size = tk_strlen(str->str);

  return RET_OK;
}

ret_t str_trim_right(str_t* str, const char* text) {
  return_value_if_fail(text != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str != NULL && str->str != NULL, RET_BAD_PARAMS);

  if (!str->size) {
    return RET_OK;
  }

  tk_str_trim_right(str->str, text);
  str->size = tk_strlen(str->str);

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
  uint32_t size = tk_strlen(str);

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

static uint32_t str_replace_impl(char* dst, char* src, const char* text, const char* new_text) {
  char* d = dst;
  char* s = src;
  uint32_t text_len = tk_strlen(text);
  uint32_t new_text_len = tk_strlen(new_text);

  while (*s) {
    if (memcmp(s, text, text_len) == 0) {
      memcpy(d, new_text, new_text_len);
      s += text_len;
      d += new_text_len;
    } else {
      *d++ = *s++;
    }
  }
  *d = '\0';

  return d - dst;
}

ret_t str_replace(str_t* str, const char* text, const char* new_text) {
  uint32_t count = 0;
  uint32_t text_len = 0;
  uint32_t new_text_len = 0;
  return_value_if_fail(str != NULL && str->str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(text != NULL && new_text != NULL, RET_BAD_PARAMS);
  return_value_if_fail(*text, RET_BAD_PARAMS);

  text_len = tk_strlen(text);
  new_text_len = tk_strlen(new_text);
  count = str_count_sub_str(str, text);
  if (count > 0) {
    int32_t delta_len = new_text_len - text_len;
    uint32_t capacity = str->size + count * delta_len + 1;

    if (delta_len <= 0) {
      uint32_t size = str_replace_impl(str->str, str->str, text, new_text);
      str->size = size;
    } else if (str->extendable) {
      char* temp_str = (char*)TKMEM_ALLOC(capacity);
      uint32_t size = str_replace_impl(temp_str, str->str, text, new_text);
      TKMEM_FREE(str->str);
      str->str = temp_str;
      str->size = size;
      str->capacity = capacity;
    } else {
      return_value_if_fail(str->extendable, RET_FAIL);
    }
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

  memmove(s->str + offset + size, s->str + offset, tk_strlen(s->str + offset));
  memcpy(s->str + offset, text, size);
  s->size += size;
  s->str[s->size] = '\0';

  return RET_OK;
}

ret_t str_insert(str_t* s, uint32_t offset, const char* text) {
  return_value_if_fail(s != NULL && offset <= s->size && text != NULL, RET_BAD_PARAMS);

  return str_insert_with_len(s, offset, text, tk_strlen(text));
}

ret_t str_remove(str_t* s, uint32_t offset, uint32_t size) {
  return_value_if_fail(s != NULL && (offset + size) <= s->size && size > 0, RET_BAD_PARAMS);

  memmove(s->str + offset, s->str + offset + size, tk_strlen(s->str + offset + size));
  s->size -= size;
  s->str[s->size] = '\0';

  return RET_OK;
}

static const char* expand_var(str_t* str, const char* p, const tk_object_t* obj) {
  value_t v;
  uint32_t len = 0;
  char name[TK_NAME_LEN + 1];
  const char* end = strchr(p, '}');
  return_value_if_fail(end != NULL, p);

  len = end - p;
  return_value_if_fail(len <= TK_NAME_LEN, end + 1);

  tk_strncpy(name, p, len);
  if (tk_object_eval((tk_object_t*)obj, name, &v) != RET_OK) {
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

ret_t str_expand_vars(str_t* str, const char* src, const tk_object_t* obj) {
  const char* p = src;
  return_value_if_fail(str != NULL && src != NULL && obj != NULL, RET_BAD_PARAMS);

  while (*p) {
    char c = *p;

    if (c == '$') {
      if (tk_strncmp(p, "${}", 3) == 0) {
        p += 3;
      } else if (p[1] && p[2]) {
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

ret_t str_append_c_str(str_t* str, const char* c_str) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_append_char(str, '\"') == RET_OK, RET_OOM);
  if (c_str != NULL) {
    str_append_escape(str, c_str, tk_strlen(c_str));
  }
  return_value_if_fail(str_append_char(str, '\"') == RET_OK, RET_OOM);
  return RET_OK;
}

ret_t str_append_json_str(str_t* str, const char* json_str) {
  const char* p = json_str;
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_append_char(str, '\"') == RET_OK, RET_OOM);
  if (p != NULL) {
    while (*p) {
      if (*p == '\"') {
        return_value_if_fail(str_append(str, "\\\"") == RET_OK, RET_OOM);
      } else if (*p == '\n') {
        return_value_if_fail(str_append(str, "\\n") == RET_OK, RET_OOM);
      } else if (*p == '\r') {
        return_value_if_fail(str_append(str, "\\r") == RET_OK, RET_OOM);
      } else if (*p == '\t') {
        return_value_if_fail(str_append(str, "\\t") == RET_OK, RET_OOM);
      } else if (*p == '\b') {
        return_value_if_fail(str_append(str, "\\b") == RET_OK, RET_OOM);
      } else if (*p == '\f') {
        return_value_if_fail(str_append(str, "\\f") == RET_OK, RET_OOM);
      } else if (*p == '\\') {
        return_value_if_fail(str_append(str, "\\\\") == RET_OK, RET_OOM);
      } else {
        return_value_if_fail(str_append_char(str, *p) == RET_OK, RET_OOM);
      }
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

ret_t str_encode_hex(str_t* str, const void* data, uint32_t size, const char* format) {
  char tstr[64];
  uint32_t i = 0;
  const uint8_t* p = data;
  return_value_if_fail(str != NULL && data != NULL, RET_BAD_PARAMS);

  if (format == NULL) {
    format = "%02x";
  }

  for (i = 0; i < size; i++) {
    tk_snprintf(tstr, sizeof(tstr) - 1, format, p[i]);
    return_value_if_fail(str_append(str, tstr) == RET_OK, RET_OOM);
  }

  return RET_OK;
}

ret_t str_decode_hex(str_t* str, void* data, uint32_t size) {
  char* p = NULL;
  char v[3] = {0, 0, 0};
  uint8_t* d = data;
  uint8_t* dend = d + size;
  return_value_if_fail(str != NULL && data != NULL, RET_BAD_PARAMS);

  for (p = str->str; p < str->str + str->size && d < dend; p += 2) {
    while (p[0] == ' ') {
      p++;
    }
    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) {
      p += 2;
    }
    tk_strncpy(v, p, 2);
    *d = tk_strtol(v, 0, 16);
    d++;
  }

  return RET_OK;
}

ret_t str_common_prefix(str_t* str, const char* other) {
  uint32_t i = 0;
  return_value_if_fail(str != NULL && other != NULL, RET_BAD_PARAMS);

  for (i = 0; i < str->size && other[i] != '\0'; i++) {
    if (str->str[i] != other[i]) {
      break;
    }
  }
  str->str[i] = '\0';
  str->size = i;

  return RET_OK;
}

ret_t str_reverse(str_t* str) {
  return_value_if_fail(str != NULL, RET_BAD_PARAMS);

  if (str->size > 1) {
    char* start = str->str;
    char* end = str->str + str->size - 1;

    while (start < end) {
      char c = *start;
      *start = *end;
      *end = c;
      start++;
      end--;
    }
  }

  return RET_OK;
}

uint32_t str_count(str_t* str, const char* substr) {
  return_value_if_fail(str != NULL && substr != NULL, 0);

  return str_count_sub_str(str, substr);
}

ret_t str_format(str_t* str, uint32_t size, const char* format, ...) {
  va_list va;
  int32_t ret = 0;
  return_value_if_fail(str != NULL && format != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_extend(str, size + 1) == RET_OK, RET_OOM);

  va_start(va, format);
  ret = tk_vsnprintf(str->str, size, format, va);
  va_end(va);
  return_value_if_fail(ret >= 0, RET_BAD_PARAMS);
  str->size = ret;

  return RET_OK;
}

ret_t str_append_format(str_t* str, uint32_t size, const char* format, ...) {
  va_list va;
  ret_t ret = RET_OK;
  return_value_if_fail(str != NULL && format != NULL, RET_BAD_PARAMS);
  va_start(va, format);
  ret = str_append_vformat(str, size, format, va);
  va_end(va);
  return ret;
}

ret_t str_append_vformat(str_t* str, uint32_t size, const char* format, va_list ap) {
  va_list va;
  int32_t ret = 0;
  return_value_if_fail(str != NULL && format != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_extend(str, str->size + size + 1) == RET_OK, RET_OOM);

  ret = tk_vsnprintf(str->str + str->size, size, format, ap);
  return_value_if_fail(ret >= 0, RET_BAD_PARAMS);
  str->size += ret;

  return RET_OK;
}

ret_t str_append_format_padding(str_t* str, uint32_t size, const char* format, ...) {
  va_list va;
  int32_t ret = 0;
  uint32_t old_size = 0;
  return_value_if_fail(str != NULL && format != NULL, RET_BAD_PARAMS);
  return_value_if_fail(str_extend(str, str->size + size + 1) == RET_OK, RET_OOM);

  old_size = str->size;
  va_start(va, format);
  ret = tk_vsnprintf(str->str + str->size, size, format, va);
  va_end(va);

  return_value_if_fail(ret >= 0, RET_BAD_PARAMS);
  str->size += ret;

  if (str->size < (old_size + size)) {
    uint32_t n = (old_size + size) - str->size;
    str_append_n_chars(str, ' ', n);
  }

  return RET_OK;
}

ret_t str_append_json_pair(str_t* str, const char* key, const value_t* value) {
  ret_t ret;
  return_value_if_fail(str != NULL && key != NULL && value, RET_BAD_PARAMS);

  return_value_if_fail(str_append_json_str(str, key) == RET_OK, RET_OOM);
  return_value_if_fail(str_append_char(str, ':') == RET_OK, RET_OOM);

  if (value->type == VALUE_TYPE_BOOL) {
    return_value_if_fail(str_append(str, value_bool(value) ? "true" : "false") == RET_OK, RET_OOM);

  } else if (value->type == VALUE_TYPE_INT8 || value->type == VALUE_TYPE_INT16 ||
             value->type == VALUE_TYPE_INT32) {
    return_value_if_fail(str_append_int(str, value_int32(value)) == RET_OK, RET_OOM);

  } else if (value->type == VALUE_TYPE_UINT8 || value->type == VALUE_TYPE_UINT16 ||
             value->type == VALUE_TYPE_UINT32) {
    return_value_if_fail(str_append_uint32(str, value_uint32(value)) == RET_OK, RET_OOM);

  } else if (value->type == VALUE_TYPE_UINT64) {
    return_value_if_fail(str_append_uint64(str, value_uint64(value)) == RET_OK, RET_OOM);

  } else if (value->type == VALUE_TYPE_INT64) {
    return_value_if_fail(str_append_int64(str, value_int64(value)) == RET_OK, RET_OOM);

  } else if (value->type == VALUE_TYPE_DOUBLE || value->type == VALUE_TYPE_FLOAT32 ||
             value->type == VALUE_TYPE_FLOAT) {
    return_value_if_fail(str_append_double(str, NULL, value_double(value)) == RET_OK, RET_OOM);

  } else if (value->type == VALUE_TYPE_STRING) {
    return_value_if_fail(str_append_json_str(str, value_str(value)) == RET_OK, RET_OOM);

  } else if (value->type == VALUE_TYPE_SIZED_STRING) {
    sized_str_t* ss = value_sized_str(value);
    return_value_if_fail(ss, RET_BAD_PARAMS);
    return_value_if_fail(str_append_json_str(str, ss->str) == RET_OK, RET_OOM);

  } else if (value->type == VALUE_TYPE_BINARY) {
    str_t hexstr;
    binary_data_t* bd = value_binary_data(value);

    return_value_if_fail(bd, RET_BAD_PARAMS);
    return_value_if_fail(str_init(&hexstr, bd->size * 2 + 1) != NULL, RET_OOM);

    ret = str_encode_hex(&hexstr, bd->data, bd->size, NULL);
    if (ret != RET_OK) {
      str_reset(&hexstr);
      return ret;
    }

    ret = str_append_json_str(str, hexstr.str);
    if (ret != RET_OK) {
      str_reset(&hexstr);
      return ret;
    }
    str_reset(&hexstr);
  }

  return RET_OK;
}
