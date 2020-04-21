/**
 * File:   suggest_words.h
 * Author: AWTK Develop Team
 * Brief:  suggest_words
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
 * 2018-06-27 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/buffer.h"
#include "base/suggest_words.h"

/*ref:tools/word_gen/gen.js*/

typedef struct _suggest_words_index_t {
  uint32_t code;
  uint32_t offset;
} suggest_words_index_t;

typedef struct _suggest_words_header_t {
  uint32_t version;
  uint32_t nr;
} suggest_words_header_t;

static const uint8_t* suggest_words_find_data(const asset_info_t* res, wchar_t c) {
  int low = 0;
  int mid = 0;
  int high = 0;
  int result = 0;
  uint32_t header_size = sizeof(suggest_words_header_t);
  const suggest_words_header_t* header = (suggest_words_header_t*)(res->data);
  const suggest_words_index_t* index = (suggest_words_index_t*)(res->data + header_size);
  uint32_t data_offset = header_size + header->nr * sizeof(suggest_words_index_t);

  high = header->nr - 1;
  while (low <= high) {
    mid = low + ((high - low) >> 1);
    result = index[mid].code - c;

    if (result == 0) {
      return res->data + data_offset + index[mid].offset;
    } else if (result < 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return NULL;
}

static const uint16_t* get_str(const uint16_t* p16, wchar_t* str, uint32_t max_chars) {
  uint32_t i = 0;
  wchar_t* pc = str;

  while (*p16) {
    if (i < max_chars) {
      *pc++ = *p16++;
    } else {
      p16++;
    }
    i++;
  }

  return p16 + 1;
}

static ret_t suggest_words_update(suggest_words_t* suggest_words, const uint8_t* data) {
  wbuffer_t wb;
  uint32_t i = 0;
  uint32_t nr = 0;
  char sword[64];
  wchar_t word[16];

  suggest_words->words_nr = 0;
  wbuffer_init(&wb, (uint8_t*)(suggest_words->words), sizeof(suggest_words->words));

  if (data != NULL) {
    const uint8_t* p = data;
    const uint16_t* p16 = (const uint16_t*)(data + 4);

    load_uint32(p, nr);
    for (i = 0; i < nr; i++) {
      memset(word, 0x00, sizeof(word));
      p16 = get_str(p16, word, ARRAY_SIZE(word) - 1);

      tk_utf8_from_utf16(word, sword, sizeof(sword));
      if (wbuffer_write_string(&wb, sword) != RET_OK) {
        break;
      }
      suggest_words->words_nr++;
    }
  }

  return RET_OK;
}

ret_t suggest_words_find(suggest_words_t* suggest_words, wchar_t c) {
  const uint8_t* data = NULL;
  return_value_if_fail(suggest_words != NULL && c != 0, RET_BAD_PARAMS);

  suggest_words->words_nr = 0;
  data = suggest_words_find_data(suggest_words->res, c);

  return suggest_words_update(suggest_words, data);
}

suggest_words_t* suggest_words_create(const asset_info_t* res) {
  suggest_words_t* suggest_words = NULL;
  return_value_if_fail(res != NULL && res->data != NULL, NULL);

  suggest_words = TKMEM_ZALLOC(suggest_words_t);
  return_value_if_fail(suggest_words != NULL, NULL);

  suggest_words->res = res;

  return suggest_words;
}

ret_t suggest_words_destroy(suggest_words_t* suggest_words) {
  return_value_if_fail(suggest_words != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(suggest_words);

  return RET_OK;
}
