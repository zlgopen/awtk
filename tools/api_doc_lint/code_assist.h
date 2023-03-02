/**
 * File:   code_assist.h
 * Author: AWTK Develop Team
 * Brief:  代码辅助模块接口定义。
 *
 * Copyright (c) 2022 - 2023 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-03-01 Wang JunSheng <wangjunsheng@zlg.cn> created
 *
 */

#ifndef TK_CODE_ASSIST_H
#define TK_CODE_ASSIST_H

#include "tkc/types_def.h"

BEGIN_C_DECLS

typedef enum _ca_symbol_type_t {
  CA_LOCAL,
  CA_GENERAL,
  CA_FUNC_DEF,
  CA_FUNC_PROTOTYPE,
  CA_TYPE,
  CA_DEF,
  CA_INCLUDE,
  CA_KEYWORD,
  CA_ENUM,
} ca_symbol_type_t;

typedef struct _ca_symbol_t {
  ca_symbol_type_t type;
  const char* name;
  int32_t pos;
  int32_t line;
  const char* line_content;
  const char* file;
  const char* param;
  const char* scope;
  const char* typeref;
  const char* comment;
} ca_symbol_t;

struct _ca_symbols_t;
typedef struct _ca_symbols_t ca_symbols_t;
typedef void (*ca_symbols_destroy_t)(ca_symbols_t* symbols);

typedef struct _ca_symbols_t {
  uint32_t size;
  ca_symbols_destroy_t destroy;
  ca_symbol_t nodes[1];
} ca_symbols_t;

typedef struct _code_assist_t {
  /*private*/
  void* impl;
} code_assist_t;

code_assist_t* code_assist_create();
ret_t code_assist_destroy(code_assist_t* ca);
ret_t code_assist_visit_file2(code_assist_t* ca, const char* full_path);
ca_symbols_t* code_assist_symbols_from_file(code_assist_t* ca, const char* full_path, bool_t sort);

END_C_DECLS

#endif /*TK_CODE_ASSIST_H*/
