/**
 * File:   fscript_array.h
 * Author: AWTK Develop Team
 * Brief:  array functions for fscript
 *
 * Copyright (c) 2020 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-08 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#ifndef TK_FSCRIPT_ARRAY_H
#define TK_FSCRIPT_ARRAY_H

#include "tkc/str.h"
#include "tkc/object.h"
#include "tkc/fscript.h"
#include "tkc/object_array.h"

BEGIN_C_DECLS

typedef struct _fscript_array_vtable_t {
  const char* type;
  fscript_func_t array_dup;
  fscript_func_t array_push;
  fscript_func_t array_pop;
  fscript_func_t array_shift;
  fscript_func_t array_get;
  fscript_func_t array_set;
  fscript_func_t array_insert;
  fscript_func_t array_remove;
  fscript_func_t array_get_and_remove;
  fscript_func_t array_index_of;
  fscript_func_t array_last_index_of;
  fscript_func_t array_clear;
  fscript_func_t array_reverse;
  fscript_func_t array_join;
  fscript_func_t array_sort;
  fscript_func_t array_clone_and_sort;
  fscript_func_t array_min;
  fscript_func_t array_max;
  fscript_func_t array_avg;
  fscript_func_t array_sum;
  fscript_func_t array_is_empty;
  fscript_func_t array_size;
} fscript_array_vtable_t;

#define FSCRIPT_ARRAY_PROP_VTABEL_NAME "_vtable_"
#define FSCRIPT_ARRAY_PROP_VTABEL_TYPE_STRING "_fscript_array_type_"

/**
 * @method fscript_array_register
 * 注册typed array函数。
 *
 * @return {ret_t} 返回RET_OK表示成功，否则表示失败。
 */
ret_t fscript_array_register(void);

/**
 * @method fscript_array_get_fscript_array_vt
 * 获取 fscript array 的虚表。
 * @param {tk_object_t*} obj obj对象。
 *
 * @return {const fscript_array_vtable_t*} 成功返回虚表指针，失败返回 NULL。
 */
const fscript_array_vtable_t* fscript_array_get_fscript_array_vt(tk_object_t* obj);

END_C_DECLS

#endif /*TK_FSCRIPT_ARRAY_H*/
