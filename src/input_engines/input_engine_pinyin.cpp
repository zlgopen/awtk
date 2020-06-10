/**
 * File:   input_engine_pinyin.c
 * Author: AWTK Develop Team
 * Brief:  pinyin input method engine
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
 * 2018-06-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/buffer.h"
#include "base/input_engine.h"
#include "base/input_method.h"

#ifdef WITH_IME_PINYIN

#include "pinyinime.h"

#define MAX_WORD_LEN 32

using namespace ime_pinyin;

static const char* input_engine_pinyin_get_lang(input_engine_t* engine) {
  return IM_LANG_ZH_CN;
}

static ret_t input_engine_pinyin_reset_input(input_engine_t* engine) {
  (void)engine;
  im_reset_search();

  return RET_OK;
}

static ret_t input_engine_pinyin_add_candidate(input_engine_t* engine, wbuffer_t* wb,
                                               uint32_t index) {
  uint32_t i = 0;
  char str[MAX_WORD_LEN * 2 + 1];
  char16 str16[MAX_WORD_LEN + 1];
  wchar_t wstr[MAX_WORD_LEN + 1];
  im_get_candidate(index, str16, MAX_WORD_LEN);

  str16[MAX_WORD_LEN] = 0;
  while (str16[i] && i < MAX_WORD_LEN) {
    wstr[i] = str16[i];
    i++;
  }
  wstr[i] = 0;

  tk_utf8_from_utf16(wstr, str, sizeof(str) - 1);

  if ((wb->cursor + strlen(str) + 1) >= wb->capacity) {
    return RET_FAIL;
  }

  return wbuffer_write_string(wb, str);
}

static ret_t input_engine_ensure_data(input_engine_t* engine) {
  if (engine->data != NULL) {
    return RET_OK;
  } else {
    const asset_info_t* data = assets_manager_ref(assets_manager(), ASSET_TYPE_DATA, "gpinyin.dat");
    return_value_if_fail(data != NULL, RET_FAIL);
    engine->data = (void*)data;
  }
  im_open_decoder_rom((const char*)engine->data);
  im_set_max_lens(32, 16);

  return RET_OK;
}

static ret_t input_engine_pinyin_search(input_engine_t* engine, const char* keys) {
  wbuffer_t wb;
  uint32_t i = 0;
  uint32_t keys_size = strlen(keys);
  uint32_t nr = im_search(keys, tk_min(14, keys_size));

  wbuffer_init(&wb, (uint8_t*)(engine->candidates), sizeof(engine->candidates));
  return_value_if_fail(input_engine_ensure_data(engine) == RET_OK, RET_FAIL);

  if (keys_size == 0) {
    input_engine_reset_input(engine);
    input_method_dispatch_candidates(engine->im, engine->candidates, 0, 0);

    return RET_OK;
  }

  wbuffer_write_string(&wb, keys);
  engine->candidates_nr = 1;

  for (i = 0; i < nr; i++) {
    if (input_engine_pinyin_add_candidate(engine, &wb, i) == RET_OK) {
      engine->candidates_nr++;
    } else {
      break;
    }
  }

  input_method_dispatch_candidates(engine->im, engine->candidates, engine->candidates_nr, 0);

  return RET_OK;
}

input_engine_t* input_engine_create(input_method_t* im) {
  input_engine_t* engine = TKMEM_ZALLOC(input_engine_t);
  return_value_if_fail(engine != NULL, NULL);

  str_init(&(engine->keys), TK_IM_MAX_INPUT_CHARS + 1);
  engine->reset_input = input_engine_pinyin_reset_input;

  engine->im = im;
  engine->search = input_engine_pinyin_search;
  engine->get_lang = input_engine_pinyin_get_lang;

  return engine;
}

ret_t input_engine_destroy(input_engine_t* engine) {
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  assets_manager_unref(assets_manager(), (const asset_info_t*)(engine->data));
  str_reset(&(engine->keys));
  im_close_decoder();
  TKMEM_FREE(engine);

  return RET_OK;
}

#endif /*WITH_IME_PINYIN*/
