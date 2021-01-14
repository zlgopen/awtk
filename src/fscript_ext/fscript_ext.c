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
#include "fscript_ext/fscript_fs.h"
#include "fscript_ext/fscript_crc.h"
#include "fscript_ext/fscript_bits.h"
#include "fscript_ext/fscript_math.h"
#include "fscript_ext/fscript_json.h"
#include "fscript_ext/fscript_array.h"
#include "fscript_ext/fscript_endian.h"
#include "fscript_ext/fscript_object.h"
#include "fscript_ext/fscript_rbuffer.h"
#include "fscript_ext/fscript_wbuffer.h"
#include "fscript_ext/fscript_app_conf.h"
#include "fscript_ext/fscript_date_time.h"
#include "fscript_ext/fscript_typed_array.h"

#include "fscript_ext/fscript_istream.h"
#include "fscript_ext/fscript_ostream.h"
#include "fscript_ext/fscript_iostream.h"
#include "fscript_ext/fscript_iostream_file.h"
#include "fscript_ext/fscript_iostream_inet.h"
#include "fscript_ext/fscript_iostream_serial.h"

#ifdef FSCRIPT_WITH_WIDGET
#include "fscript_ext/fscript_widget.h"
#endif /*FSCRIPT_WITH_WIDGET*/

static ret_t func_value_is_valid(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_bool(result, args->args->type != VALUE_TYPE_INVALID);
  return RET_OK;
}

static ret_t func_value_is_null(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_bool(result,
                 value_pointer(args->args) == NULL || args->args->type == VALUE_TYPE_INVALID);
  return RET_OK;
}

static ret_t func_value_get_binary_size(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);

  if (args->args->type == VALUE_TYPE_BINARY) {
    binary_data_t* bin = value_binary_data(args->args);
    FSCRIPT_FUNC_CHECK(bin != NULL, RET_BAD_PARAMS);
    value_set_uint32(result, bin->size);
    return RET_OK;
  }

  return RET_FAIL;
}

static ret_t func_value_get_binary_data(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);

  if (args->args->type == VALUE_TYPE_BINARY) {
    binary_data_t* bin = value_binary_data(args->args);
    FSCRIPT_FUNC_CHECK(bin != NULL, RET_BAD_PARAMS);
    value_set_pointer(result, bin->data);
    return RET_OK;
  }

  return RET_FAIL;
}

ret_t fscript_ext_init(void) {
  ENSURE(fscript_register_func("value_is_valid", func_value_is_valid) == RET_OK);
  ENSURE(fscript_register_func("value_is_null", func_value_is_null) == RET_OK);
  ENSURE(fscript_register_func("value_get_binary_data", func_value_get_binary_data) == RET_OK);
  ENSURE(fscript_register_func("value_get_binary_size", func_value_get_binary_size) == RET_OK);

  fscript_object_register();

#ifdef FSCRIPT_WITH_STREAM
  fscript_istream_register();
  fscript_ostream_register();
  fscript_iostream_register();
#ifdef FSCRIPT_WITH_STREAM_FILE
  fscript_iostream_file_register();
#endif /*FSCRIPT_WITH_STREAM_FILE*/
#ifdef FSCRIPT_WITH_STREAM_INET
  fscript_iostream_inet_register();
#endif /*FSCRIPT_WITH_STREAM_INET*/
#ifdef FSCRIPT_WITH_STREAM_SERIAL
  fscript_iostream_serial_register();
#endif /*FSCRIPT_WITH_STREAM_SERIAL*/
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

#ifdef FSCRIPT_WITH_BITS
  fscript_bits_register();
#endif /*FSCRIPT_WITH_BITS*/

#ifdef FSCRIPT_WITH_FS
  fscript_fs_register();
#endif /*FSCRIPT_WITH_FS*/

#ifdef FSCRIPT_WITH_JSON
  fscript_json_register();
#endif /*FSCRIPT_WITH_JSON*/

#ifdef FSCRIPT_WITH_ENDIAN
  fscript_endian_register();
#endif /*FSCRIPT_WITH_MATH*/

#ifdef FSCRIPT_WITH_ARRAY
  fscript_array_register();
#endif /*FSCRIPT_WITH_ARRAY*/

#ifdef FSCRIPT_WITH_TYPED_ARRAY
  fscript_typed_array_register();
#endif /*FSCRIPT_WITH_TYPED_ARRAY*/

#ifdef FSCRIPT_WITH_APP_CONF
  fscript_app_conf_register();
#endif /*FSCRIPT_WITH_APP_CONF*/

#ifdef FSCRIPT_WITH_DATE_TIME
  fscript_date_time_register();
#endif /*FSCRIPT_WITH_DATE_TIME*/

#ifdef FSCRIPT_WITH_WIDGET
  fscript_widget_register();
#endif /*FSCRIPT_WITH_WIDGET*/

  return RET_OK;
}
