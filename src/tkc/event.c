/**
 * File:   event.c
 * Author: AWTK Develop Team
 * Brief:  event structs
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
 * 2018-03-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/event.h"
#include "tkc/time_now.h"

event_t event_init(uint32_t type, void* target) {
  event_t e;

  e.type = type;
  e.target = target;
  e.time = time_now_ms();

  return e;
}

event_t* event_cast(event_t* event) {
  return event;
}
