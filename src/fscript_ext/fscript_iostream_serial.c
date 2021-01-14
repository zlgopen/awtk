/**
 * File:   fscript_iostream_serial.c
 * Author: AWTK Develop Team
 * Brief:  iostream_serial functions for fscript
 *
 * Copyright (c) 2020 - 2021  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 */

/**
 * History:
 * ================================================================
 * 2021-01-06 Li XianJing <lixianjing@zlg.cn> created
 *
 */

#include "tkc/buffer.h"
#include "tkc/fscript.h"
#include "tkc/socket_helper.h"
#include "streams/serial/iostream_serial.h"

static ret_t func_iostream_serial_create(fscript_t* fscript, fscript_args_t* args,
                                         value_t* result) {
  int baudrate = 0;
  object_t* obj = NULL;
  const char* devname = NULL;

  result->type = VALUE_TYPE_INVALID;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);
  devname = value_str(args->args);
  baudrate = args->size > 1 ? value_int(args->args + 1) : 0;
  return_value_if_fail(devname != NULL, RET_BAD_PARAMS);
  obj = OBJECT(tk_iostream_serial_create(devname));
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  value_set_object(result, obj);
  result->free_handle = TRUE;

  if (baudrate > 0) {
    object_set_prop_int(OBJECT(obj), TK_IOSTREAM_SERIAL_PROP_BAUDRATE, baudrate);
    object_exec(OBJECT(obj), TK_IOSTREAM_SERIAL_CMD_CONFIG, NULL);
  }

  return RET_OK;
}

ret_t fscript_iostream_serial_register(void) {
  ENSURE(fscript_register_func("iostream_serial_create", func_iostream_serial_create) == RET_OK);
  return RET_OK;
}
