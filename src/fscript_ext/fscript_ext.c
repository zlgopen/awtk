/**
 * File:   fscript_ext.h
 * Author: AWTK Develop Team
 * Brief:  ext functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "fscript_ext/fscript_module.h"
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

static ret_t func_char_at_impl(fscript_t* fscript, fscript_args_t* args, value_t* result,
                               int32_t index) {
  wstr_t wstr;
  ret_t ret = RET_OK;
  const char* str = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  str = value_str(args->args);
  return_value_if_fail(str != NULL && *str, RET_BAD_PARAMS);

  if (args->size > 1) {
    index = value_int(args->args + 1);
  }

  wstr_init(&wstr, 0);
  return_value_if_fail(wstr_set_utf8(&wstr, str) == RET_OK, RET_OOM);

  if (index < 0) {
    index += wstr.size;
  }

  if (index > wstr.size) {
    index = index % wstr.size;
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

static ret_t func_char_at(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_char_at_impl(fscript, args, result, 0);
}

static ret_t func_char_at_first(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_char_at_impl(fscript, args, result, 0);
}

static ret_t func_char_at_last(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_char_at_impl(fscript, args, result, -1);
}

static ret_t func_char_at_random(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  return func_char_at_impl(fscript, args, result, random());
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

static ret_t func_global_var(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_bool(result, TRUE);
  return RET_OK;
}

static ret_t func_member_var(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_bool(result, TRUE);
  return RET_OK;
}

static ret_t func_str_is_empty(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_bool(result, tk_strlen(value_str(args->args)) == 0);
  return RET_OK;
}

static ret_t func_str_len(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_uint32(result, tk_strlen(value_str(args->args)));
  return RET_OK;
}

static ret_t func_str_append(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  str_t* str = &(fscript->str);
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);

  str_set(str, value_str(args->args));
  str_append(str, value_str(args->args + 1));
  value_set_str(result, str->str);

  return RET_OK;
}

static ret_t func_flow_get(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char name[64];
  const char* type = NULL;
  const char* subname = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 2, RET_BAD_PARAMS);
  type = value_str(args->args);
  subname = value_str(args->args + 1);
  FSCRIPT_FUNC_CHECK(name != NULL && subname != NULL, RET_BAD_PARAMS);

  tk_snprintf(name, sizeof(name) - 1, "%s.%s", type, subname);
  if (tk_object_get_prop(fscript->obj, name, result) != RET_OK) {
    if (args->size > 2) {
      value_copy(result, args->args + 2);
    } else {
      result->type = VALUE_TYPE_INVALID;
    }
  }

  return RET_OK;
}

static ret_t func_flow_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  char name[64];
  const char* type = NULL;
  const char* subname = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  type = value_str(args->args);
  subname = value_str(args->args + 1);
  FSCRIPT_FUNC_CHECK(name != NULL && subname != NULL, RET_BAD_PARAMS);

  tk_snprintf(name, sizeof(name) - 1, "%s.%s", type, subname);
  value_set_bool(result, tk_object_set_prop(fscript->obj, name, args->args + 2) == RET_OK);

  return RET_OK;
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
/*用于反向解析保留信息*/
FACTORY_TABLE_ENTRY("member_var", func_member_var)
FACTORY_TABLE_ENTRY("global_var", func_global_var)
#ifdef HAS_STDIO
FACTORY_TABLE_ENTRY("prompt", func_prompt)
#endif /*HAS_STDIO*/
FACTORY_TABLE_ENTRY("value_is_valid", func_value_is_valid)
FACTORY_TABLE_ENTRY("value_is_null", func_value_is_null)
FACTORY_TABLE_ENTRY("value_get_binary_data", func_value_get_binary_data)
FACTORY_TABLE_ENTRY("value_get_binary_size", func_value_get_binary_size)

/*主要给AWBLOCK使用*/
FACTORY_TABLE_ENTRY("str_len", func_str_len)
FACTORY_TABLE_ENTRY("str_is_empty", func_str_is_empty)
FACTORY_TABLE_ENTRY("str_append", func_str_append)
FACTORY_TABLE_ENTRY("char_at_first", func_char_at_first)
FACTORY_TABLE_ENTRY("char_at_last", func_char_at_last)
FACTORY_TABLE_ENTRY("char_at_random", func_char_at_random)
FACTORY_TABLE_ENTRY("flow_get", func_flow_get)
FACTORY_TABLE_ENTRY("flow_set", func_flow_set)

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

#ifdef FSCRIPT_WITH_MODULE
  fscript_module_register();
#endif /*FSCRIPT_WITH_MODULE*/

  return RET_OK;
}
