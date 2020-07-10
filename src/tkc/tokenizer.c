/**
 * File:   tokenizer.c
 * Author: AWTK Develop Team
 * Brief:  tokenizer
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is ditokenizeributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-06-17 Li XianJing <xianjimli@hotmail.com> adapt from uclib
 *
 */

#include "tkc/utils.h"
#include "tkc/tokenizer.h"

tokenizer_t* tokenizer_init_ex(tokenizer_t* tokenizer, const char* str, uint32_t size,
                               const char* separtor, const char* single_char_token) {
  uint32_t str_size = 0;
  return_value_if_fail(tokenizer != NULL && str != NULL && separtor != NULL, NULL);

  str_size = strlen(str);
  tokenizer->str = str;
  tokenizer->cursor = 0;
  tokenizer->separtor = separtor;
  tokenizer->size = tk_min(str_size, size);
  tokenizer->single_char_token = single_char_token;
  str_init(&(tokenizer->token), 20);

  return tokenizer;
}

tokenizer_t* tokenizer_init(tokenizer_t* tokenizer, const char* str, uint32_t size,
                            const char* separtor) {
  return tokenizer_init_ex(tokenizer, str, size, separtor, NULL);
}

static ret_t tokenizer_skip_separator(tokenizer_t* tokenizer) {
  return_value_if_fail(tokenizer != NULL, RET_BAD_PARAMS);

  while (tokenizer->cursor < tokenizer->size) {
    char c = tokenizer->str[tokenizer->cursor];
    if (strchr(tokenizer->separtor, c) != NULL) {
      tokenizer->cursor++;
    } else {
      break;
    }
  }

  return RET_OK;
}

static ret_t tokenizer_skip_non_separator(tokenizer_t* tokenizer) {
  const char* separtor = NULL;
  const char* single_char_token = NULL;
  return_value_if_fail(tokenizer != NULL, RET_BAD_PARAMS);

  separtor = tokenizer->separtor;
  single_char_token = tokenizer->single_char_token != NULL ? tokenizer->single_char_token : "";

  while (tokenizer->cursor < tokenizer->size) {
    char c = tokenizer->str[tokenizer->cursor];
    if (strchr(separtor, c) == NULL && strchr(single_char_token, c) == NULL) {
      tokenizer->cursor++;
    } else {
      break;
    }
  }

  return RET_OK;
}

bool_t tokenizer_has_more(tokenizer_t* tokenizer) {
  return_value_if_fail(tokenizer != NULL, FALSE);

  return tokenizer->cursor < tokenizer->size;
}

static ret_t tokenizer_skip_quoted_str(tokenizer_t* tokenizer) {
  bool_t escaped = FALSE;
  char c = tokenizer->str[tokenizer->cursor];
  return_value_if_fail(c == '\"', RET_BAD_PARAMS);

  tokenizer->cursor++;
  while (tokenizer->str[tokenizer->cursor]) {
    c = tokenizer->str[tokenizer->cursor++];

    if (c == '\"') {
      if (!escaped) {
        break;
      }
    }

    if (c == '\\') {
      escaped = !escaped;
    } else {
      escaped = FALSE;
    }
  }

  return RET_OK;
}

const char* tokenizer_next_expr_until(tokenizer_t* tokenizer, const char* str) {
  return_value_if_fail(tokenizer_skip_separator(tokenizer) == RET_OK && str != NULL, NULL);

  if (tokenizer_has_more(tokenizer)) {
    uint32_t len = 0;
    str_t* s = &(tokenizer->token);
    uint32_t start = tokenizer->cursor;

    while (tokenizer->str[tokenizer->cursor]) {
      char c = tokenizer->str[tokenizer->cursor];
      if (c == '\"') {
        tokenizer_skip_quoted_str(tokenizer);
        continue;
      } else if (c == '(') {
        int32_t lparent = 1;
        tokenizer->cursor++;
        while (tokenizer->str[tokenizer->cursor]) {
          c = tokenizer->str[tokenizer->cursor];
          if (c == '\"') {
            tokenizer_skip_quoted_str(tokenizer);
            continue;
          } else if (c == '(') {
            lparent++;
          } else if (c == ')') {
            lparent--;
            if (lparent <= 0) {
              tokenizer->cursor++;
              break;
            }
          }
          tokenizer->cursor++;
        }
        continue;
      }

      if (strchr(str, c) != NULL) {
        break;
      }

      tokenizer->cursor++;
    }

    len = tokenizer->cursor - start;
    str_set_with_len(s, tokenizer->str + start, len);
    tokenizer_skip_separator(tokenizer);

    return s->str;
  }

  return NULL;
}

const char* tokenizer_next_until(tokenizer_t* tokenizer, const char* str) {
  return_value_if_fail(tokenizer_skip_separator(tokenizer) == RET_OK && str != NULL, NULL);

  if (tokenizer_has_more(tokenizer)) {
    uint32_t len = 0;
    str_t* s = &(tokenizer->token);
    uint32_t start = tokenizer->cursor;

    while (tokenizer->str[tokenizer->cursor]) {
      char c = tokenizer->str[tokenizer->cursor];
      if (strchr(str, c) != NULL) {
        break;
      }
      tokenizer->cursor++;
    }

    len = tokenizer->cursor - start;
    str_set_with_len(s, tokenizer->str + start, len);
    tokenizer_skip_separator(tokenizer);

    return s->str;
  }

  return NULL;
}

const char* tokenizer_next(tokenizer_t* tokenizer) {
  return_value_if_fail(tokenizer_skip_separator(tokenizer) == RET_OK, NULL);

  if (tokenizer_has_more(tokenizer)) {
    uint32_t len = 0;
    str_t* s = &(tokenizer->token);
    uint32_t start = tokenizer->cursor;

    if (tokenizer->single_char_token != NULL) {
      if (strchr(tokenizer->single_char_token, tokenizer->str[start]) != NULL) {
        str_set_with_len(s, tokenizer->str + start, 1);
        tokenizer->cursor++;

        return s->str;
      }
    }

    tokenizer_skip_non_separator(tokenizer);

    len = tokenizer->cursor - start;
    str_set_with_len(s, tokenizer->str + start, len);
    tokenizer_skip_separator(tokenizer);

    return s->str;
  }

  return NULL;
}

int tokenizer_next_int(tokenizer_t* tokenizer, int defval) {
  return_value_if_fail(tokenizer != NULL, defval);
  if (tokenizer_has_more(tokenizer)) {
    return tk_atoi(tokenizer_next(tokenizer));
  } else {
    return defval;
  }
}

float tokenizer_next_float(tokenizer_t* tokenizer, float defval) {
  return_value_if_fail(tokenizer != NULL, defval);
  if (tokenizer_has_more(tokenizer)) {
    return tk_atof(tokenizer_next(tokenizer));
  } else {
    return defval;
  }
}

ret_t tokenizer_deinit(tokenizer_t* tokenizer) {
  return_value_if_fail(tokenizer != NULL, RET_BAD_PARAMS);
  str_reset(&(tokenizer->token));
  memset(tokenizer, 0x00, sizeof(tokenizer_t));

  return RET_OK;
}
