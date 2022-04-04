/**
 * File:   debugger_message.c
 * Author: AWTK Develop Team
 * Brief:  debugger message
 *
 * Copyright (c) 2022 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-01-15 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "debugger/debugger_message.h"

event_t* debugger_log_event_init(debugger_log_event_t* event, uint32_t line, const char* message) {
  return_value_if_fail(event != NULL, NULL);

  memset(event, 0x00, sizeof(*event));
  event->e = event_init(DEBUGGER_RESP_MSG_LOG, NULL);
  event->e.size = sizeof(*event);
  event->line = line;
  event->message = message;

  return (event_t*)event;
}

debugger_log_event_t* debugger_log_event_cast(event_t* event) {
  return_value_if_fail(event != NULL && event->type == DEBUGGER_RESP_MSG_LOG, NULL);
  return_value_if_fail(event->size == sizeof(debugger_log_event_t), NULL);

  return (debugger_log_event_t*)event;
}

event_t* debugger_error_event_init(debugger_error_event_t* event, uint32_t line,
                                   const char* message) {
  return_value_if_fail(event != NULL, NULL);

  memset(event, 0x00, sizeof(*event));
  event->e = event_init(DEBUGGER_RESP_MSG_ERROR, NULL);
  event->e.size = sizeof(*event);
  event->line = line;
  event->message = message;

  return (event_t*)event;
}

debugger_error_event_t* debugger_error_event_cast(event_t* event) {
  return_value_if_fail(event != NULL && event->type == DEBUGGER_RESP_MSG_ERROR, NULL);
  return_value_if_fail(event->size == sizeof(debugger_error_event_t), NULL);

  return (debugger_error_event_t*)event;
}

event_t* debugger_breaked_event_init(debugger_breaked_event_t* event, uint32_t line) {
  return_value_if_fail(event != NULL, NULL);

  memset(event, 0x00, sizeof(*event));
  event->e = event_init(DEBUGGER_RESP_MSG_BREAKED, NULL);
  event->e.size = sizeof(*event);
  event->line = line;

  return (event_t*)event;
}

debugger_breaked_event_t* debugger_breaked_event_cast(event_t* event) {
  return_value_if_fail(event != NULL && event->type == DEBUGGER_RESP_MSG_BREAKED, NULL);
  return_value_if_fail(event->size == sizeof(debugger_breaked_event_t), NULL);

  return (debugger_breaked_event_t*)event;
}
