/**
 * File:   encoding.c
 * Author: AWTK Develop Team
 * Brief:  encoding conversion
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; withto even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2020-08-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "encoding.h"

#ifdef WIN32
#include "win_iconv.inc"
#define WITH_ICONV 1
#endif /*WIN32*/

#if defined(LINUX) || defined(MACOS)
#include <iconv.h>
#define WITH_ICONV 1
#endif

#if defined(ANDROID)
#undef WITH_ICONV
#endif /*ANDROID*/

#ifdef WITH_ICONV
static const char* get_native_encoding_name(encoding_name_t name) {
  switch (name) {
    case ENCODING_UTF8: {
      return "UTF-8";
    }
    case ENCODING_UTF16: {
      return "UTF-16";
    }
    case ENCODING_UTF32: {
      return "UTF-32";
    }
    case ENCODING_GBK: {
      return "GBK";
    }
    case ENCODING_BIG: {
      return "BIG5-2003";
    }
    case ENCODING_GB2312: {
      return "GB_2312-80";
    }
    case ENCODING_GB18030: {
      return "GB18030";
    }
    default: {
      return NULL;
    }
  }
}

static ret_t encoding_convert_impl(encoding_name_t from, const char* from_str, uint32_t from_size,
                                   encoding_name_t to, char* to_str, uint32_t to_size) {
  iconv_t icv = 0;
  ret_t ret = RET_OK;
  char** pout = (char**)(&to_str);
  char** pin = (char**)(&from_str);
  size_t outlen = to_size;
  size_t inlen = from_size;
  const char* from_charset = get_native_encoding_name(from);
  const char* to_charset = get_native_encoding_name(to);
  return_value_if_fail(from_charset != NULL && to_charset != NULL, RET_NOT_IMPL);

  icv = iconv_open(to_charset, from_charset);
  return_value_if_fail(icv != 0, RET_FAIL);

  memset(to_str, 0, to_size);

  if (iconv(icv, pin, &inlen, pout, &outlen) < 0) {
    ret = RET_FAIL;
  }

  iconv_close(icv);
  return ret;
}
#else
/*for embedded system*/
#include "impl/encoding_conv.inc"
#endif /*WITH_ICONV*/

ret_t encoding_convert(encoding_name_t from, const char* from_str, uint32_t from_size,
                       encoding_name_t to, char* to_str, uint32_t to_size) {
  return_value_if_fail(from_str != NULL && to_str != NULL && to_size > 0, RET_BAD_PARAMS);

  return encoding_convert_impl(from, from_str, from_size, to, to_str, to_size);
}

ret_t encoding_utf8_to_gbk(const char* from_str, uint32_t from_size, char* to_str,
                           uint32_t to_size) {
  return encoding_convert(ENCODING_UTF8, from_str, from_size, ENCODING_GBK, to_str, to_size);
}

ret_t encoding_gbk_to_utf8(const char* from_str, uint32_t from_size, char* to_str,
                           uint32_t to_size) {
  return encoding_convert(ENCODING_GBK, from_str, from_size, ENCODING_UTF8, to_str, to_size);
}
