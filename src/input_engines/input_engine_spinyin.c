/**
 * File:   input_engine_spinyin.c
 * Author: AWTK Develop Team
 * Brief:  simple pinyin input engine
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
 * 2020-04-18 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/buffer.h"
#include "base/input_engine.h"
#include "base/input_method.h"

#ifdef WITH_IME_SPINYIN

#include "ime_utils.h"
#include "pinyin_table.inc"

static const char* input_engine_spinyin_get_lang(input_engine_t* engine) {
  return IM_LANG_ZH_CN;
}

static ret_t input_engine_spinyin_reset_input(input_engine_t* engine) {
  input_method_dispatch_candidates(engine->im, engine->candidates, 0, 0);

  return RET_OK;
}

static ret_t input_engine_spinyin_search(input_engine_t* engine, const char* keys) {
  wbuffer_t wb;
  uint32_t keys_size = strlen(keys);
  const table_entry_t* items = s_pinyin_chinese_items;
  uint32_t items_nr = ARRAY_SIZE(s_pinyin_chinese_items);

  if (keys_size < 1) {
    input_engine_reset_input(engine);
    return RET_OK;
  }

  wbuffer_init(&wb, (uint8_t*)(engine->candidates), sizeof(engine->candidates));

  engine->candidates_nr = 1;
  wbuffer_write_string(&wb, keys);
  engine->candidates_nr += table_search(items, items_nr, keys, &wb, FALSE);
  input_method_dispatch_candidates(engine->im, engine->candidates, engine->candidates_nr, 0);

  return RET_OK;
}

static ret_t input_engine_spinyin_set_lang(input_engine_t* engine, const char* lang) {
  return RET_OK;
}

input_engine_t* input_engine_create(input_method_t* im) {
  input_engine_t* engine = TKMEM_ZALLOC(input_engine_t);

  return_value_if_fail(engine != NULL, NULL);

  str_init(&(engine->keys), TK_IM_MAX_INPUT_CHARS + 1);
  engine->reset_input = input_engine_spinyin_reset_input;
  engine->search = input_engine_spinyin_search;
  engine->set_lang = input_engine_spinyin_set_lang;
  engine->get_lang = input_engine_spinyin_get_lang;
  engine->im = im;

  return engine;
}

ret_t input_engine_destroy(input_engine_t* engine) {
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  str_reset(&(engine->keys));
  TKMEM_FREE(engine);

  return RET_OK;
}

#endif /*WITH_IME_SPINYIN*/
