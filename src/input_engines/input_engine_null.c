/**
 * File:   input_engine_null.c
 * Author: AWTK Develop Team
 * Brief:  null input method engine
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
#include "tkc/buffer.h"
#include "base/input_engine.h"
#include "base/input_method.h"

#ifdef WITH_IME_NULL

static ret_t input_engine_null_reset_input(input_engine_t* engine) {
  (void)engine;

  return RET_OK;
}

static ret_t input_engine_null_search(input_engine_t* engine, const char* keys) {
  wbuffer_t wb;
  uint32_t keys_size = strlen(keys);
  wbuffer_init(&wb, (uint8_t*)(engine->candidates), sizeof(engine->candidates));

  if (keys_size > 0) {
    wbuffer_write_string(&wb, keys);
    wbuffer_write_string(&wb, "广");
    wbuffer_write_string(&wb, "州");
    wbuffer_write_string(&wb, "致");
    wbuffer_write_string(&wb, "远");
    wbuffer_write_string(&wb, "电子");
    wbuffer_write_string(&wb, "有限公司");
    engine->candidates_nr = 7;
    input_method_dispatch_candidates(engine->im, engine->candidates, engine->candidates_nr, 0);
  } else {
    engine->candidates_nr = 0;
  }

  return RET_OK;
}

input_engine_t* input_engine_create(input_method_t* im) {
  input_engine_t* engine = TKMEM_ZALLOC(input_engine_t);
  return_value_if_fail(engine != NULL, NULL);

  str_init(&(engine->keys), TK_IM_MAX_INPUT_CHARS + 1);
  engine->reset_input = input_engine_null_reset_input;
  engine->search = input_engine_null_search;
  engine->im = im;

  return engine;
}

ret_t input_engine_destroy(input_engine_t* engine) {
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  str_reset(&(engine->keys));
  TKMEM_FREE(engine);

  return RET_OK;
}
#endif /*WITH_IME_NULL*/
