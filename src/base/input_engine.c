/**
 * File:   input_engine.
 * Author: AWTK Develop Team
 * Brief:  input method engine
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "base/keys.h"
#include "base/input_engine.h"

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

ret_t input_engine_input(input_engine_t* engine, int key) {
  ret_t ret = RET_OK;
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  return_value_if_fail(engine->keys.size < TK_IM_MAX_INPUT_CHARS, RET_BAD_PARAMS);

  if (key == FKEY_BACKSPACE) {
    if (engine->keys.size > 0) {
      engine->keys.size--;
      engine->keys.str[engine->keys.size] = '\0';
    } else {
      return RET_FAIL;
    }
  } else {
    str_append_char(&(engine->keys), (char)key);
  }

  if (engine->input) {
    ret = engine->input(engine, key);
  }

  return ret;
}
