/**
 * File:   input_engine.
 * Author: AWTK Develop Team
 * Brief:  input method engine
 *
 * Copyright (c) 2018 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/keys.h"
#include "base/input_method.h"
#include "base/input_engine.h"

ret_t input_engine_init(input_engine_t* engine) {
  ret_t ret = RET_OK;
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  wbuffer_init_extendable(&(engine->candidates));
  if (TK_IM_MAX_CANDIDATE_CHARS != 0) {
    wbuffer_extend_capacity(&(engine->candidates), TK_IM_MAX_CANDIDATE_CHARS);
  }

  if (engine->init) {
    ret = engine->init(engine);
  }

  return ret;
}

ret_t input_engine_deinit(input_engine_t* engine) {
  ret_t ret = RET_OK;
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  wbuffer_deinit(&(engine->candidates));
  if (engine->deinit) {
    ret = engine->deinit(engine);
  }

  return ret;
}

ret_t input_engine_reset_candidates(input_engine_t* engine) {
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  engine->candidates_nr = 0;
  return wbuffer_rewind(&(engine->candidates));
}

ret_t input_engine_add_candidate(input_engine_t* engine, const char* str) {
  return_value_if_fail(engine != NULL && str != NULL, RET_BAD_PARAMS);
  if (TK_IM_MAX_CANDIDATE_CHARS != 0 &&
      engine->candidates.cursor + strlen(str) + 2 > engine->candidates.capacity) {
    return RET_FAIL;
  }
  engine->candidates_nr++;
  return wbuffer_write_string(&(engine->candidates), str);
}

ret_t input_engine_add_candidates_from_char(input_engine_t* engine, const wchar_t** table, char c) {
#if defined(WITH_IME_T9) || defined(WITH_IME_T9EXT) || defined(WITH_IME_SPINYIN)
  return_value_if_fail(engine != NULL && table != NULL, RET_BAD_PARAMS);
  engine->candidates_nr +=
      ime_utils_add_chars(&(engine->candidates), table, c, TK_IM_MAX_CANDIDATE_CHARS == 0);
  return RET_OK;
#else
  return RET_NOT_IMPL;
#endif
}

ret_t input_engine_add_candidates_from_string(input_engine_t* engine, const table_entry_t* items,
                                              uint32_t items_nr, const char* key, bool_t exact) {
#if defined(WITH_IME_T9) || defined(WITH_IME_T9EXT) || defined(WITH_IME_SPINYIN)
  return_value_if_fail(engine != NULL && items != NULL && items_nr > 0 && key != NULL,
                       RET_BAD_PARAMS);
  engine->candidates_nr += ime_utils_table_search(items, items_nr, key, &(engine->candidates),
                                                  exact, TK_IM_MAX_CANDIDATE_CHARS == 0);
  return RET_OK;
#else
  return RET_NOT_IMPL;
#endif
}

ret_t input_engine_dispatch_candidates(input_engine_t* engine, int32_t selected) {
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  return input_method_dispatch_candidates(engine->im, (const char*)(engine->candidates.data),
                                          engine->candidates_nr, selected);
}

ret_t input_engine_reset_input(input_engine_t* engine) {
  ret_t ret = RET_OK;
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);

  engine->keys.size = 0;
  engine->keys.str[0] = '\0';
  engine->candidates_nr = 0;

  if (engine->reset_input) {
    ret = engine->reset_input(engine);
  }

  return ret;
}

ret_t input_engine_set_lang(input_engine_t* engine, const char* lang) {
  ret_t ret = RET_OK;
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  input_engine_reset_input(engine);
  if (engine->set_lang) {
    ret = engine->set_lang(engine, lang);
  }

  return ret;
}

const char* input_engine_get_lang(input_engine_t* engine) {
  return_value_if_fail(engine != NULL, NULL);

  if (engine->get_lang) {
    return engine->get_lang(engine);
  }

  return NULL;
}

ret_t input_engine_search(input_engine_t* engine, const char* keys) {
  return_value_if_fail(engine != NULL && engine->search != NULL && keys != NULL, RET_BAD_PARAMS);

  return engine->search(engine, keys);
}

static ret_t input_engine_append_char(input_engine_t* engine, int key) {
  if (key == TK_KEY_TAB || key == TK_KEY_LEFT || key == TK_KEY_RIGHT || key == TK_KEY_UP ||
      key == TK_KEY_DOWN || key == TK_KEY_PAGEUP || key == TK_KEY_PAGEDOWN ||
      key_code_is_enter(key) || key == TK_KEY_CANCEL) {
    return RET_OK;
  }

  if (tk_isprint(key)) {
    str_append_char(&(engine->keys), (char)key);
  }

  return RET_OK;
}

ret_t input_engine_input(input_engine_t* engine, int key) {
  ret_t ret = RET_OK;
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);

  if (key == TK_KEY_BACKSPACE || key == TK_KEY_DELETE) {
    if (engine->keys.size > 0) {
      engine->keys.size--;
      engine->keys.str[engine->keys.size] = '\0';
    } else {
      input_engine_reset_input(engine);
      return RET_FAIL;
    }
  } else {
    if (engine->keys.size >= TK_IM_MAX_INPUT_CHARS) {
      return RET_BAD_PARAMS;
    }

    if (engine->input != NULL) {
      if (engine->input(engine, key) == RET_OK) {
        input_engine_append_char(engine, key);
      } else {
        return RET_FAIL;
      }
    } else {
      input_engine_append_char(engine, key);
    }
  }

  if (engine->search) {
    ret = engine->search(engine, engine->keys.str);
  }

  return ret;
}
