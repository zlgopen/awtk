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

#include "tkc/utf8.h"
#include "tkc/utils.h"
#include "tkc/wstr.h"
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

static ret_t func_index_of_ex(fscript_t* fscript, fscript_args_t* args, value_t* result,
                              bool_t last) {
  const char* p = NULL;
  const char* str = NULL;
  const char* substr = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  str = value_str(args->args);
  substr = value_str(args->args + 1);
  return_value_if_fail(str != NULL && substr != NULL, RET_BAD_PARAMS);

  p = last ? tk_strrstr(str, substr) : strstr(str, substr);
  if (p != NULL) {
    value_set_int32(result, p - str);
  } else {
    value_set_int32(result, -1);
  }

  return RET_OK;
}

static ret_t func_index_of(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_index_of_ex(fscript, args, result, FALSE);
}

static ret_t func_last_index_of(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_index_of_ex(fscript, args, result, TRUE);
}

static ret_t func_totitle(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  str_set(str, value_str(args->args));
  tk_str_totitle(str->str);
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_text_reverse(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);

  str_set(str, value_str(args->args));
  str_reverse(str);
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_text_count(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  str_set(str, value_str(args->args));
  value_set_int(result, str_count(str, value_str(args->args + 1)));

  return RET_OK;
}

static ret_t func_usubstr(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  wstr_t wstr;
  int32_t start = 0;
  int32_t end = 0;
  ret_t ret = RET_OK;
  const char* str = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  str = value_str(args->args);
  return_value_if_fail(str != NULL && *str, RET_BAD_PARAMS);

  wstr_init(&wstr, 0);
  return_value_if_fail(wstr_set_utf8(&wstr, str) == RET_OK, RET_OOM);

  if (args->size >= 2) {
    start = value_int(args->args + 1);
  }

  if (args->size >= 3) {
    end = value_int(args->args + 2);
  } else {
    end = wstr.size;
  }

  if (start < 0) {
    start += wstr.size;
  }

  if (end < 0) {
    end += wstr.size;
  }

  if (start >= 0 && start < wstr.size && start < end && end <= wstr.size) {
    str_t* str = &(fscript->str);
    str_from_wstr_with_len(str, wstr.str + start, end - start);
    value_set_str(result, str->str);
  } else {
    value_set_str(result, "");
    ret = RET_FAIL;
  }
  wstr_reset(&wstr);

  return ret;
}

static ret_t func_char_at(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  wstr_t wstr;
  int32_t index = 0;
  ret_t ret = RET_OK;
  const char* str = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  str = value_str(args->args);
  return_value_if_fail(str != NULL && *str, RET_BAD_PARAMS);
  index = value_int(args->args + 1);

  wstr_init(&wstr, 0);
  return_value_if_fail(wstr_set_utf8(&wstr, str) == RET_OK, RET_OOM);

  if (index < 0) {
    index += wstr.size;
  }

  if (index >= 0 && index < wstr.size) {
    str_t* str = &(fscript->str);
    str_from_wstr_with_len(str, wstr.str + index, 1);
    value_set_str(result, str->str);
  } else {
    ret = RET_FAIL;
  }
  wstr_reset(&wstr);

  return ret;
}

static ret_t func_trim_left(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  str_set(str, value_str(args->args));
  str_trim_left(str, " \t\r\n");
  value_set_str(result, str->str);

  return RET_OK;
}

#ifdef HAS_STDIO
static ret_t func_prompt(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char text[128];
  str_t* str = &(fscript->str);
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);

  memset(text, 0x00, sizeof(text));
  fprintf(stdout, "%s", value_str(args->args));
  fgets(text, sizeof(text) - 1, stdin);

  str_set(str, text);
  value_set_str(result, str->str);

  return RET_OK;
}
#endif /*HAS_STDIO*/
static ret_t func_trim_right(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  str_set(str, value_str(args->args));
  str_trim_right(str, " \t\r\n");
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_ulen(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  wstr_t wstr;
  const char* str = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  str = value_str(args->args);

  wstr_init(&wstr, 0);
  return_value_if_fail(wstr_set_utf8(&wstr, str) == RET_OK, RET_OOM);
  value_set_int32(result, wstr.size);
  wstr_reset(&wstr);

  return RET_OK;
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

FACTORY_TABLE_BEGIN(s_ext_basic)
FACTORY_TABLE_ENTRY("index_of", func_index_of)
FACTORY_TABLE_ENTRY("last_index_of", func_last_index_of)
FACTORY_TABLE_ENTRY("ulen", func_ulen)
FACTORY_TABLE_ENTRY("trim_left", func_trim_left)
FACTORY_TABLE_ENTRY("trim_right", func_trim_right)
FACTORY_TABLE_ENTRY("totitle", func_totitle)
FACTORY_TABLE_ENTRY("char_at", func_char_at)
FACTORY_TABLE_ENTRY("text_count", func_text_count)
FACTORY_TABLE_ENTRY("text_reverse", func_text_reverse)
FACTORY_TABLE_ENTRY("usubstr", func_usubstr)
#ifdef HAS_STDIO
FACTORY_TABLE_ENTRY("prompt", func_prompt)
#endif /*HAS_STDIO*/
FACTORY_TABLE_ENTRY("value_is_valid", func_value_is_valid)
FACTORY_TABLE_ENTRY("value_is_null", func_value_is_null)
FACTORY_TABLE_ENTRY("value_get_binary_data", func_value_get_binary_data)
FACTORY_TABLE_ENTRY("value_get_binary_size", func_value_get_binary_size)
FACTORY_TABLE_END()

ret_t fscript_ext_init(void) {
  fscript_register_funcs(s_ext_basic);

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

  return RET_OK;
}
