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

#include "fscript_ext/fscript_math.h"

#if defined(LINUX) || defined(WINDOWS) || defined(MACOS) || defined(ANDROID) || defined(IOS)
#define FSCRIPT_WITH_MATH 1
#endif/*PC*/

ret_t fscript_ext_init(void) {
#ifdef FSCRIPT_WITH_MATH
  fscript_math_register();
#endif/*FSCRIPT_WITH_MATH*/

  return RET_OK;
}

