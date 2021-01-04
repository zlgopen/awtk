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

#include "fscript_ext/fscript_ext.h"
#include "fscript_ext/fscript_crc.h"
#include "fscript_ext/fscript_math.h"
#include "fscript_ext/fscript_endian.h"
#include "fscript_ext/fscript_object.h"
#include "fscript_ext/fscript_rbuffer.h"
#include "fscript_ext/fscript_wbuffer.h"
#include "fscript_ext/fscript_typed_array.h"

static ret_t func_value_reset(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_reset(args->args);
  return RET_OK;
}

ret_t fscript_ext_init(void) {
  ENSURE(fscript_register_func("value_reset", func_value_reset) == RET_OK);

  fscript_object_register();

#ifdef FSCRIPT_WITH_CRC
  fscript_crc_register();
#endif /*FSCRIPT_WITH_CRC*/

#ifdef FSCRIPT_WITH_BUFFER
  fscript_rbuffer_register();
  fscript_wbuffer_register();
#endif /*FSCRIPT_WITH_BUFFER*/

#ifdef FSCRIPT_WITH_MATH
  fscript_math_register();
#endif /*FSCRIPT_WITH_MATH*/

#ifdef FSCRIPT_WITH_ENDIAN
  fscript_endian_register();
#endif /*FSCRIPT_WITH_MATH*/

#ifdef FSCRIPT_WITH_TYPED_ARRAY
  fscript_typed_array_register();
#endif /*FSCRIPT_WITH_TYPED_ARRAY*/

  return RET_OK;
}
