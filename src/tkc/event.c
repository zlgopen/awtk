/**
 * File:   event.c
 * Author: AWTK Develop Team
 * Brief:  event structs
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
 * 2018-03-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/event.h"
#include "tkc/time_now.h"

event_t event_init(uint32_t type, void* target) {
  event_t e;

  memset(&e, 0x00, sizeof(e));

  e.type = type;
  e.target = target;
  e.time = time_now_ms();

  return e;
}

event_t* event_cast(event_t* event) {
  return event;
}

event_t* event_create(uint32_t type) {
  event_t* e = TKMEM_ZALLOC(event_t);

  return_value_if_fail(e != NULL, NULL);
  *e = event_init(type, NULL);

  return e;
}

ret_t event_destroy(event_t* event) {
  return_value_if_fail(event != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(event);

  return RET_OK;
}

prop_change_event_t* prop_change_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_PROP_CHANGED || event->type == EVT_PROP_WILL_CHANGE,
                       NULL);

  return (prop_change_event_t*)event;
}

event_t* prop_change_event_init(prop_change_event_t* event, uint32_t type, const char* name,
                                const value_t* value) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(*event));

  event->e.type = type;
  event->name = name;
  event->value = value;

  return (event_t*)(event);
}

progress_event_t* progress_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_PROGRESS, NULL);

  return (progress_event_t*)event;
}

event_t* progress_event_init(progress_event_t* event, uint32_t percent) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(*event));

  event->e.type = EVT_PROGRESS;
  event->percent = percent;

  return (event_t*)(event);
}

done_event_t* done_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_PROGRESS, NULL);

  return (done_event_t*)event;
}

event_t* done_event_init(done_event_t* event, ret_t result) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(*event));

  event->e.type = EVT_DONE;
  event->result = result;

  return (event_t*)(event);
}

error_event_t* error_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_PROGRESS, NULL);

  return (error_event_t*)event;
}

event_t* error_event_init(error_event_t* event, int32_t code, const char* message) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(*event));

  event->e.type = EVT_ERROR;
  event->code = code;
  event->message = message;

  return (event_t*)(event);
}

cmd_exec_event_t* cmd_exec_event_cast(event_t* event) {
  return_value_if_fail(event != NULL, NULL);
  return_value_if_fail(event->type == EVT_CMD_WILL_EXEC || event->type == EVT_CMD_EXECED ||
                           event->type == EVT_CMD_CAN_EXEC,
                       NULL);

  return (cmd_exec_event_t*)event;
}

event_t* cmd_exec_event_init(cmd_exec_event_t* event, uint32_t type, const char* name,
                             const char* value) {
  return_value_if_fail(event != NULL, NULL);
  memset(event, 0x00, sizeof(*event));

  event->e.type = type;
  event->name = name;
  event->args = value;

  return (event_t*)(event);
}
