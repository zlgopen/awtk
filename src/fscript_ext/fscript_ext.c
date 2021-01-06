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

#include "fscript_ext/fscript_istream.h"
#include "fscript_ext/fscript_ostream.h"
#include "fscript_ext/fscript_iostream.h"
#include "fscript_ext/fscript_iostream_file.h"
#include "fscript_ext/fscript_iostream_inet.h"
#include "fscript_ext/fscript_iostream_serial.h"

static ret_t func_value_is_valid(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_bool(result, args->args->type != VALUE_TYPE_INVALID);
  return RET_OK;
}

ret_t fscript_ext_init(void) {
  ENSURE(fscript_register_func("value_is_valid", func_value_is_valid) == RET_OK);

  fscript_object_register();

#ifdef FSCRIPT_WITH_STREAM
  fscript_istream_register();
  fscript_ostream_register();
  fscript_iostream_register();
  #ifdef FSCRIPT_WITH_STREAM_FILE
  fscript_iostream_file_register();
  #endif/*FSCRIPT_WITH_STREAM_FILE*/
  #ifdef FSCRIPT_WITH_STREAM_INET
  fscript_iostream_inet_register();
  #endif/*FSCRIPT_WITH_STREAM_INET*/
  #ifdef FSCRIPT_WITH_STREAM_SERIAL
  fscript_iostream_serial_register();
  #endif/*FSCRIPT_WITH_STREAM_SERIAL*/
#endif /*FSCRIPT_WITH_STREAM*/

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
