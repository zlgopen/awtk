/**
 * File:   log.c
 * Author: AWTK Develop Team
 * Brief:  log functions
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */
#include "tkc/types_def.h"
#include "tkc/log.h"

static log_level_t s_log_level = LOG_LEVEL_DEBUG;

ret_t log_set_log_level(log_level_t log_level) {
  s_log_level = log_level;

  return RET_OK;
}

log_level_t log_get_log_level(void) {
  return s_log_level;
}
