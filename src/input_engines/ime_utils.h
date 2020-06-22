
/**
 * File:   t9.h
 * Author: AWTK Develop Team
 * Brief:  t9 input method helper functions
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
 * 2020-04-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/buffer.h"

#ifndef TK_IME_UTILS
#define TK_IME_UTILS

BEGIN_C_DECLS

typedef struct _table_entry_t {
  const char* key;
  const char* memo;
  const char** words;
} table_entry_t;

uint32_t table_search(const table_entry_t* items, uint32_t items_nr, const char* keys,
                      wbuffer_t* result, bool_t exact);

ret_t input_engine_add_chars(wbuffer_t* wb, const wchar_t** table, char c);

END_C_DECLS

#endif /*TK_IME_UTILS*/
