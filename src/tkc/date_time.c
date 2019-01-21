/**
 * File:   date_time.c
 * Author: AWTK Develop Team
 * Brief:  date time
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
ret_t date_time_set_impl(date_time_get_now_t date_time_get_now) {
  s_date_time_get_now = date_time_get_now;

  return RET_OK;
}

date_time_t* date_time_create(void) {
  date_time_t* dt = TKMEM_ZALLOC(date_time_t);

  return date_time_init(dt);
}

date_time_t* date_time_init(date_time_t* dt) {
  return_value_if_fail(dt != NULL, NULL);
  memset(dt, 0x00, sizeof(date_time_t));
  return_value_if_fail(s_date_time_get_now != NULL, dt);

  s_date_time_get_now(dt);

  return dt;
}

ret_t date_time_destroy(date_time_t* dt) {
  return_value_if_fail(dt != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(dt);

  return RET_OK;
}
