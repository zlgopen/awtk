
/**
 * File:   t9.c
 * Author: AWTK Develop Team
 * Brief:  t9 input method helper functions
 *
 * Copyright (c) 2018 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-04-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "ime_utils.h"

int32_t ime_utils_table_search_index(const table_entry_t* items, uint32_t items_nr, const char* key,
                           uint32_t key_len, bool_t exact) {
  int r = 0;
  int32_t low = 0;
  int32_t high = items_nr - 1;

  while (low <= high) {
    uint32_t mid = (low + high) / 2;
    const table_entry_t* iter = items + mid;

    if (exact) {
      r = strcmp(iter->key, key);
    } else {
      r = strncmp(iter->key, key, key_len);
    }

    if (r == 0) {
      return mid;
    } else if (r < 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return -1;
}

static uint32_t ime_utils_count_words(const char** words) {
  uint32_t n = 0;

  while (words[n] != NULL) n++;

  return n;
}

uint32_t ime_utils_table_search(const table_entry_t* items, uint32_t items_nr, const char* key,
                      wbuffer_t* result, bool_t exact) {
  uint32_t i = 0;
  uint32_t nr = 0;
  int32_t found = 0;
  uint32_t key_len = 0;
  const table_entry_t* iter = NULL;
  return_value_if_fail(items != NULL && key != NULL && result != NULL, 0);

  key_len = strlen(key);
  if (key_len == 0) {
    return 0;
  }

  /*match exact*/
  found = ime_utils_table_search_index(items, items_nr, key, key_len, TRUE);
  if (found >= 0) {
    iter = items + found;

    if (key_len > 1 && iter->memo != NULL) {
      if (wbuffer_write_string(result, iter->memo) == RET_OK) {
        nr++;
      }
    }

    while (iter->words[i] != NULL) {
      if (!tk_str_eq(iter->memo, iter->words[i]) && wbuffer_write_string(result, iter->words[i]) != RET_OK) {
        break;
      }
      i++;
      nr++;
    }

    return nr;
  }

  if (exact) {
    return nr;
  }

  /*match prefix*/
  found = ime_utils_table_search_index(items, items_nr, key, key_len, FALSE);
  if (found >= 0) {
    uint32_t k = 0;
    uint32_t key_len = strlen(key);

    iter = items + found;
    if (key_len > 1 && iter->memo != NULL) {
      if (wbuffer_write_string(result, iter->memo) == RET_OK) {
        nr++;
      }
    }

    while (found > 0) {
      iter = items + found - 1;
      if (strncmp(iter->key, key, key_len) == 0) {
        found--;
      } else {
        break;
      }
    }

    for (i = 0; i < 5; i++) {
      for (k = found; k < items_nr; k++) {
        iter = items + k;
        if (strncmp(iter->key, key, key_len) == 0) {
          if (ime_utils_count_words(iter->words) > i) {
            if (wbuffer_write_string(result, iter->words[i]) != RET_OK) {
              return nr;
            }
            nr++;
          }
        } else {
          break;
        }
      }
    }
  }

  return nr;
}

uint32_t ime_utils_add_chars(wbuffer_t* wb, const wchar_t** table, char c) {
  char str[8];
  uint32_t n = 0;
  const wchar_t* p = NULL;
  uint32_t index = c - '0';
  return_value_if_fail(index >= 0 && index <= 9, RET_BAD_PARAMS);

  p = table[index];
  while (*p) {
    memset(str, 0x00, sizeof(str));
    tk_utf8_from_utf16_ex(p, 1, str, sizeof(str));

    if (wbuffer_write_string(wb, str) != RET_OK) {
      break;
    }
    n++;
    p++;
  }

  return n;
}
