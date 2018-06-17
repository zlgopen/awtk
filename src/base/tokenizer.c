/**
 * File:   tokenizer.c
 * Author: AWTK Develop Team
 * Brief:  tokenizer
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/utils.h"
#include "base/tokenizer.h"

tokenizer_t* tokenizer_init(tokenizer_t* tokenizer, const char* str, uint32_t size,
                            const char* separtor) {
  uint32_t str_size = 0;
  return_value_if_fail(tokenizer != NULL && str != NULL && separtor != NULL, NULL);

  str_size = strlen(str);
  tokenizer->str = str;
  tokenizer->cursor = 0;
  tokenizer->separtor = separtor;
  tokenizer->size = tk_min(str_size, size);
  str_init(&(tokenizer->token), 20);

  return tokenizer;
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
  return_value_if_fail(tokenizer != NULL, RET_BAD_PARAMS);

  while (tokenizer->cursor < tokenizer->size) {
    char c = tokenizer->str[tokenizer->cursor];
    if (strchr(tokenizer->separtor, c) == NULL) {
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

const char* tokenizer_next(tokenizer_t* tokenizer) {
  return_value_if_fail(tokenizer_skip_separator(tokenizer) == RET_OK, NULL);

  if (tokenizer_has_more(tokenizer)) {
    uint32_t len = 0;
    str_t* s = &(tokenizer->token);
    uint32_t start = tokenizer->cursor;

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
