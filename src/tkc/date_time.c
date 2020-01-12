/**
 * File:   date_time.c
 * Author: AWTK Develop Team
 * Brief:  date time
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-11-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/date_time.h"

static date_time_get_now_t s_date_time_get_now;
static date_time_set_now_t s_date_time_set_now;
static date_time_from_time_t s_date_time_from_time;

ret_t date_time_global_init_ex(const date_time_vtable_t* vt) {
  return_value_if_fail(vt != NULL, RET_BAD_PARAMS);

  s_date_time_get_now = vt->get_now;
  s_date_time_set_now = vt->set_now;
  s_date_time_from_time = vt->from_time;

  return RET_OK;
}

ret_t date_time_global_init(date_time_get_now_t get, date_time_set_now_t set) {
  s_date_time_get_now = get;
  s_date_time_set_now = set;

  return RET_OK;
}

ret_t date_time_set_impl(date_time_get_now_t date_time_get_now) {
  s_date_time_get_now = date_time_get_now;

  return RET_OK;
}

date_time_t* date_time_create(void) {
  date_time_t* dt = TKMEM_ZALLOC(date_time_t);

  return date_time_init(dt);
}

ret_t date_time_from_time(date_time_t* dt, uint64_t time) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(s_date_time_from_time != NULL, RET_BAD_PARAMS);

  return s_date_time_from_time(dt, (time_t)time);
}

date_time_t* date_time_init(date_time_t* dt) {
  return_value_if_fail(dt != NULL, NULL);

  memset(dt, 0x00, sizeof(date_time_t));
  if (s_date_time_get_now != NULL) {
    s_date_time_get_now(dt);
    return dt;
  } else {
    return NULL;
  }
}

ret_t date_time_set(date_time_t* dt) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);
  return_value_if_fail(s_date_time_set_now != NULL, RET_NOT_IMPL);

  return s_date_time_set_now(dt);
}

ret_t date_time_destroy(date_time_t* dt) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(dt);

  return RET_OK;
}
