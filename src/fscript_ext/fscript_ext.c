/**
 * File:   fscript_ext.h
 * Author: AWTK Develop Team
 * Brief:  ext functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-03 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/fscript.h"
#include "fscript_ext/fscript_math.h"
#include "fscript_ext/fscript_object.h"
#include "fscript_ext/fscript_typed_array.h"

#if defined(LINUX) || defined(WINDOWS) || defined(MACOS) || defined(ANDROID) || defined(IOS)
#define FSCRIPT_WITH_MATH 1
#define FSCRIPT_WITH_TYPED_ARRAY 1
#endif /*PC*/

ret_t fscript_ext_init(void) {
  fscript_object_register();
#ifdef FSCRIPT_WITH_MATH
  fscript_math_register();
#endif /*FSCRIPT_WITH_MATH*/

#ifdef FSCRIPT_WITH_TYPED_ARRAY
  fscript_typed_array_register();
#endif /*FSCRIPT_WITH_TYPED_ARRAY*/

  return RET_OK;
}
