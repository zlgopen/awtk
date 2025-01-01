/**
 * File:   date_time_format.c
 * Author: AWTK Develop Team
 * Brief:  date_time_format
 *
 * Copyright (c) 2020 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-01-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "base/locale_info.h"
#include "base/date_time_format.h"

static ret_t date_time_format_translate_callback(void* ctx, const void* data) {
  const char** dst = (const char**)(ctx);
  const char* src = (const char*)(data);
  return_value_if_fail(dst != NULL && src != NULL, RET_BAD_PARAMS);

  *dst = locale_info_tr(locale_info(), src);

  return RET_OK;
}

ret_t wstr_format_date_time(wstr_t* str, const char* format, const date_time_t* dt) {
  ret_t ret = RET_OK;
  str_t result;
  return_value_if_fail(str != NULL && format != NULL, RET_BAD_PARAMS);

  str_init(&result, tk_strlen(format));

  ret = tk_date_time_format_impl(dt, format, &result, date_time_format_translate_callback);
  if (RET_OK == ret) {
    ret = wstr_set_utf8_with_len(str, result.str, result.size);
  }

  str_reset(&result);

  return ret;
}

ret_t wstr_format_time(wstr_t* str, const char* format, uint64_t time) {
  date_time_t dt;
  return_value_if_fail(str != NULL && format != NULL, RET_BAD_PARAMS);

  memset(&dt, 0x00, sizeof(dt));
  date_time_from_time(&dt, time);

  return wstr_format_date_time(str, format, &dt);
}
