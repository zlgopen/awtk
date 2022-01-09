/**
 * File:   fscript_date_time.c
 * Author: AWTK Develop Team
 * Brief:  date_time functions for fscript
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
#include "tkc/time_now.h"
#include "tkc/object_date_time.h"

static ret_t func_date_time_create(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_object_t* obj = object_date_time_create();

  value_set_object(result, obj);
  result->free_handle = TRUE;

  return RET_OK;
}

static date_time_t* get_date_time(fscript_t* fscript, fscript_args_t* args) {
  tk_object_t* obj = value_object(args->args);
  return_value_if_fail(obj != NULL, NULL);

  return OBJECT_DATE_TIME(obj)->dt;
}

static ret_t func_date_time_to_time(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  date_time_t* dt = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  dt = get_date_time(fscript, args);
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);

  value_set_uint64(result, date_time_to_time(dt));

  return RET_OK;
}

static ret_t func_date_time_from_time(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  date_time_t* dt = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  dt = get_date_time(fscript, args);
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);

  date_time_from_time(dt, value_uint64(args->args + 1));
  value_set_bool(result, TRUE);

  return RET_OK;
}

static ret_t func_date_time_set(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  date_time_t* dt = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  dt = get_date_time(fscript, args);
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);

  value_set_bool(result, date_time_set(dt) == RET_OK);

  return RET_OK;
}

static ret_t func_time_now_us(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_set_uint64(result, time_now_us());
  return RET_OK;
}

static ret_t func_time_now_ms(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_set_uint64(result, time_now_ms());
  return RET_OK;
}

static ret_t func_time_now_s(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  value_set_uint64(result, time_now_s());
  return RET_OK;
}

static ret_t func_year_is_leap(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  FSCRIPT_FUNC_CHECK(args->size == 1, RET_BAD_PARAMS);
  value_set_bool(result, date_time_is_leap(value_int32(args->args)));
  return RET_OK;
}

static ret_t func_get_days_of_month(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  int32_t year = 0;
  int32_t month = 0;
  FSCRIPT_FUNC_CHECK(args->size == 2, RET_BAD_PARAMS);
  year = value_int32(args->args);
  month = value_int32(args->args + 1);
  value_set_uint32(result, date_time_get_days(year, month));

  return RET_OK;
}

static ret_t func_date_time_get_prop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size >= 2, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  if (tk_object_get_prop(obj, value_str(args->args + 1), result) != RET_OK) {
    if (args->size > 2) {
      value_copy(result, args->args + 2);
    } else {
      result->type = VALUE_TYPE_INVALID;
    }
  }

  return RET_OK;
}

static ret_t func_date_time_set_prop(fscript_t* fscript, fscript_args_t* args, value_t* result) {
  tk_object_t* obj = NULL;
  FSCRIPT_FUNC_CHECK(args->size == 3, RET_BAD_PARAMS);
  obj = value_object(args->args);
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  value_set_bool(result,
                 tk_object_set_prop(obj, value_str(args->args + 1), args->args + 2) == RET_OK);

  return RET_OK;
}

FACTORY_TABLE_BEGIN(s_ext_date_time)
FACTORY_TABLE_ENTRY("date_time_create", func_date_time_create)
FACTORY_TABLE_ENTRY("date_time_to_time", func_date_time_to_time)
FACTORY_TABLE_ENTRY("date_time_from_time", func_date_time_from_time)
FACTORY_TABLE_ENTRY("date_time_set", func_date_time_set)
FACTORY_TABLE_ENTRY("time_now_us", func_time_now_us)
FACTORY_TABLE_ENTRY("time_now_ms", func_time_now_ms)
FACTORY_TABLE_ENTRY("time_now_s", func_time_now_s)
FACTORY_TABLE_ENTRY("time_now", func_time_now_s)
FACTORY_TABLE_ENTRY("is_leap_year", func_year_is_leap)
FACTORY_TABLE_ENTRY("get_days_of_month", func_get_days_of_month)

/*主要给AWBLOCK使用*/
FACTORY_TABLE_ENTRY("date_time_set_prop", func_date_time_set_prop)
FACTORY_TABLE_ENTRY("date_time_get_prop", func_date_time_get_prop)
FACTORY_TABLE_END()

ret_t fscript_date_time_register(void) {
  return fscript_register_funcs(s_ext_date_time);
}
