/**
 * File:   event_source.c
 * Author: AWTK Develop Team
 * Brief:  event source interface.
 *
 * Copyright (c) 2019 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2019-09-29 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/event_source.h"

int32_t event_source_get_fd(event_source_t* source) {
  return_value_if_fail(source != NULL, -1);

  return source->get_fd != NULL ? source->get_fd(source) : -1;
}

ret_t event_source_dispatch(event_source_t* source) {
  return_value_if_fail(source != NULL && source->dispatch != NULL, RET_BAD_PARAMS);

  return source->dispatch(source);
}

ret_t event_source_check(event_source_t* source) {
  return_value_if_fail(source != NULL, RET_BAD_PARAMS);

  return source->check != NULL ? source->check(source) : RET_FAIL;
}

uint32_t event_source_get_wakeup_time(event_source_t* source) {
  return_value_if_fail(source != NULL, 0);

  return source->get_wakeup_time != NULL ? source->get_wakeup_time(source) : 0;
}

ret_t event_source_set_tag(event_source_t* source, void* tag) {
  return_value_if_fail(source != NULL, RET_BAD_PARAMS);

  source->tag = tag;

  return RET_OK;
}
