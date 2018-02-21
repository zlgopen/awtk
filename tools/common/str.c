#include "str.h"
#include <ctype.h>

#define DEFAULT_CAPACITY 15

static str_t* str_init(str_t* s, const char* str, uint32_t size, uint32_t capacity) {
  return_value_if_fail(s != NULL, NULL);

  s->capacity = capacity;
  if (str != NULL) {
    uint32_t max_size = strlen(str);
    if (size > max_size) {
      size = max_size;
    }

    s->capacity = capacity > size ? capacity : size;
  }

  s->size = 0;
  if (s->capacity < DEFAULT_CAPACITY) {
    s->capacity = DEFAULT_CAPACITY;
  }

  s->str = (char*)malloc(s->capacity + 1);
  if (s->str && str) {
    str_append(s, str, size);
  }

  return s;
}

str_t* str_create(const char* str, uint32_t size, uint32_t capacity) {
  str_t* s = (str_t*)calloc(1, sizeof(str_t));
  return_value_if_fail(s != NULL, NULL);

  s->ref = 1;
  s->size = 0;

  return str_init(s, str, size, capacity);
}

bool_t str_copy(str_t* s, str_t* other) {
  return_value_if_fail(s != NULL && other != NULL, FALSE);

  s->size = 0;
  return str_append(s, other->str, other->size);
}

str_t* str_sub(str_t* s, uint32_t start, uint32_t size) {
  return_value_if_fail(s != NULL && s->str != NULL && start < s->size, NULL);

  if ((start + size) > s->size) {
    size = s->size - start;
  }

  return str_create(s->str + start, size, size);
}

str_t* str_dup(str_t* s) { return str_sub(s, 0, s->size); }

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

int str_cmp(str_t* s, const char* str) {
  return_value_if_fail(s != NULL && s->str != NULL && str != NULL, -1);

  return strcmp(s->str, str);
}

int str_ncmp(str_t* s, const char* str, uint32_t size) {
  return_value_if_fail(s != NULL && s->str != NULL && str != NULL, -1);

  return strncmp(s->str, str, size);
}

bool_t str_equal(str_t* s, const char* str) {
  return_value_if_fail(s != NULL && s->str != NULL && str != NULL, FALSE);

  return strcmp(s->str, str) == 0;
}

int32_t str_index_of_c(str_t* s, char c) {
  const char* p = NULL;
  return_value_if_fail(s != NULL && s->str != NULL, -1);

  p = strchr(s->str, c);

  return p != NULL ? (int32_t)(p - s->str) : -1;
}

int32_t str_last_index_of_c(str_t* s, char c) {
  const char* p = NULL;
  return_value_if_fail(s != NULL && s->str != NULL, -1);

  p = strrchr(s->str, c);

  return p != NULL ? (int32_t)(p - s->str) : -1;
}

int32_t str_index_of(str_t* s, const char* str) {
  const char* p = NULL;
  return_value_if_fail(s != NULL && s->str != NULL && str != NULL, -1);

  p = strstr(s->str, str);

  return p != NULL ? (int32_t)(p - s->str) : -1;
}

bool_t str_replace_c(str_t* s, char c, char new_char) {
  uint32_t i = 0;
  uint32_t n = 0;
  char* p = NULL;
  return_value_if_fail(s != NULL && s->str != NULL, FALSE);

  for (p = s->str, n = s->size; i < n; i++) {
    if (p[i] == c) {
      p[i] = new_char;
    }
  }

  return TRUE;
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

bool_t str_extend(str_t* s, uint32_t capacity) {
  if (s->capacity > capacity) {
    return TRUE;
  } else {
    char* str = (char*)realloc(s->str, capacity + 1);
    if (str != NULL) {
      s->str = str;
    }

    return str != NULL;
  }
}

bool_t str_replace(str_t* s, const char* str, const char* new_str) {
  uint32_t count = 0;

  return_value_if_fail(s != NULL && s->str != NULL && str != NULL && new_str != NULL, FALSE);

  count = str_count_sub_str(s, str);

  if (count > 0) {
    char* p = s->str;
    char* src = s->str;
    uint32_t str_len = strlen(str);
    uint32_t new_str_len = strlen(new_str);
    uint32_t capacity = s->size + count * (strlen(new_str) - strlen(str)) + 1;

    char* temp_str = (char*)malloc(capacity);
    char* dst = temp_str;
    return_value_if_fail(temp_str != NULL, FALSE);
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

    free(s->str);
    s->str = temp_str;
    s->size = strlen(s->str);
    s->capacity = capacity;
  }

  return TRUE;
}

bool_t str_append_char(str_t* s, char c) {
  return_value_if_fail(s != NULL && s->str != NULL, FALSE);
  return_value_if_fail(str_extend(s, s->size + 1), FALSE);
  s->str[s->size++] = c;
  s->str[s->size] = '\0';

  return TRUE;
}

bool_t str_append(str_t* s, const char* str, uint32_t size) {
  uint32_t max_size = 0;
  return_value_if_fail(s != NULL && s->str != NULL && str != NULL, FALSE);

  max_size = strlen(str);
  if (size > max_size) {
    size = max_size;
  }

  memcpy(s->str + s->size, str, size);
  s->size += size;
  s->str[s->size] = '\0';

  return TRUE;
}

bool_t str_set(str_t* s, const char* str, uint32_t size) {
  return_value_if_fail(s != NULL && s->str != NULL && str != NULL, FALSE);

  s->size = 0;

  return str_append(s, str, size);
}

int32_t str_to_int(str_t* s) {
  return_value_if_fail(s != NULL && s->str != NULL, 0);

  return (int32_t)atoi(s->str);
}

float str_to_float(str_t* s) {
  return_value_if_fail(s != NULL && s->str != NULL, 0);

  return (float)atof(s->str);
}

double str_to_double(str_t* s) {
  return_value_if_fail(s != NULL && s->str != NULL, 0);

  return (double)atof(s->str);
}

bool_t str_from_int(str_t* s, int32_t value) {
  char buff[32];
  return_value_if_fail(s != NULL, FALSE);

  s->size = 0;
  snprintf(buff, sizeof(buff), "%d", value);

  return str_append(s, buff, 0xffff);
}

bool_t str_from_float(str_t* s, float value) {
  char buff[32];
  return_value_if_fail(s != NULL, FALSE);

  s->size = 0;
  snprintf(buff, sizeof(buff), "%f", value);

  return str_append(s, buff, 0xffff);
}

bool_t str_from_double(str_t* s, double value) {
  char buff[32];
  return_value_if_fail(s != NULL, FALSE);

  s->size = 0;
  snprintf(buff, sizeof(buff), "%lf", value);

  return str_append(s, buff, 0xffff);
}

static int32_t str_last_slash(const char* str) {
  const char* p = str;
  int32_t i = strlen(str);

  for (; i >= 0; i--) {
    char c = p[i];
    if (c == '/' || c == '\\') {
      return i;
    }
  }

  return -1;
}

str_t* str_basename(const char* filename) {
  uint32_t len = 0;
  int32_t offset = 0;
  const char* p = NULL;
  return_value_if_fail(filename != NULL && *filename, NULL);

  offset = str_last_slash(filename);
  if (offset < 0) {
    offset = 0;
  }

  p = filename + offset + 1;
  len = strlen(p);

  return str_create(p, len, len);
}

str_t* str_dirname(const char* filename) {
  int32_t offset = 0;
  return_value_if_fail(filename != NULL && *filename, NULL);

  offset = str_last_slash(filename);
  if (offset < 0) {
    offset = 0;

    return str_create("/", 1, 1);
  } else {
    offset += 1;
  }

  return str_create(filename, offset, 0);
}

bool_t str_to_lower(str_t* s) {
  uint32_t i = 0;
  char* p = s->str;
  uint32_t n = s->size;

  return_value_if_fail(s != NULL && s->str != NULL, FALSE);

  for (i = 0; i < n; i++) {
    p[i] = tolower(p[i]);
  }

  return TRUE;
}

bool_t str_to_upper(str_t* s) {
  uint32_t i = 0;
  char* p = s->str;
  uint32_t n = s->size;

  return_value_if_fail(s != NULL && s->str != NULL, FALSE);

  for (i = 0; i < n; i++) {
    p[i] = toupper(p[i]);
  }

  return TRUE;
}

str_t* str_ref(str_t* s) {
  return_value_if_fail(s != NULL && s->ref > 0, NULL);

  s->ref++;

  return s;
}

str_t* str_unref(str_t* s) {
  return_value_if_fail(s != NULL && s->ref > 0, NULL);

  s->ref--;
  if (s->ref == 0) {
    free(s->str);
    memset(s, 0x00, sizeof(str_t));
    free(s);
    s = NULL;
  }

  return s;
}

str_t* str_init_static(str_t* s, const char* str) {
  return_value_if_fail(s != NULL && str != NULL, NULL);

  memset(s, 0x00, sizeof(str_t));
  s->size = strlen(str);
  s->str = (char*)str;
  s->capacity = s->size;

  return s;
}

bool_t str_trim_left(str_t* s, const char* str) {
  uint32_t i = 0;
  uint32_t k = 0;
  char* p = s->str;
  uint32_t n = s->size;
  return_value_if_fail(s != NULL && s->str != NULL, FALSE);

  if (!s->size) {
    return TRUE;
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

  return TRUE;
}

bool_t str_trim_right(str_t* s, const char* str) {
  uint32_t i = 0;
  char* p = s->str;
  uint32_t n = s->size;
  return_value_if_fail(s != NULL && s->str != NULL, FALSE);

  if (!s->size) {
    return TRUE;
  }

  if (str == NULL) {
    str = " ";
  }

  for (i = n - 1; i; i--) {
    char c = p[i];
    if (strchr(str, c) == NULL) {
      p[i + 1] = '\0';
      break;
    }
  }

  return TRUE;
}

bool_t str_trim(str_t* s, const char* str) {
  if (!s->size) {
    return TRUE;
  }

  return str_trim_left(s, str) && str_trim_right(s, str);
}

bool_t str_tokenizer(str_t* s, str_on_token_t on_token, void* ctx, const char* delim,
                     const char* ctokens) {
  enum _state_t { STATE_IN, STATE_OUT } state = STATE_OUT;

  uint32_t i = 0;
  uint32_t start = 0;
  uint32_t n = s->size;
  const char* p = s->str;
  str_t* token = NULL;
  return_value_if_fail(s != NULL && s->str != NULL && on_token != NULL, FALSE);

  token = str_create(NULL, 0, 100);
  return_value_if_fail(token != NULL, FALSE);

  if (delim == NULL) {
    delim = " ";
  }

  if (ctokens == NULL) {
    ctokens = "";
  }

  for (i = 0; i < n; i++) {
    char c = p[i];
    if (strchr(ctokens, c) != NULL) {
      if (state == STATE_IN) {
        str_set(token, p + start, i - start);
        on_token(ctx, token);
      }

      str_set(token, &c, 1);
      on_token(ctx, token);

      state = STATE_OUT;
    } else if (strchr(delim, c) != NULL) {
      if (state == STATE_IN) {
        str_set(token, p + start, i - start);
        on_token(ctx, token);
        state = STATE_OUT;
      }
    } else {
      if (state == STATE_OUT) {
        start = i;
        state = STATE_IN;
      }
    }
  }

  if (state == STATE_IN) {
    str_set(token, p + start, i - start);
    on_token(ctx, token);
  }

  str_unref(token);

  return TRUE;
}
