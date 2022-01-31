/**
 * File:   fscript_iostream_serial.c
 * Author: AWTK Develop Team
 * Brief:  iostream_serial functions for fscript
 *
 * Copyright (c) 2020 - 2022  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
  int baudrate = 115200;
  tk_object_t* obj = NULL;
  const char* devname = NULL;
  bytesize_t bytesize = eightbits;
  parity_t parity = parity_none;
  stopbits_t stopbits = stopbits_one;
  flowcontrol_t flowcontrol = flowcontrol_none;
  FSCRIPT_FUNC_CHECK(args->size >= 1, RET_BAD_PARAMS);

  devname = value_str(args->args);
  if (args->size > 1) {
    baudrate = value_int(args->args + 1);
  }
  if (args->size > 2) {
    bytesize = value_int(args->args + 2);
  }
  if (args->size > 3) {
    const char* str = value_str(args->args + 3);
    if (str != NULL) {
      if (strstr(str, "odd") != NULL) {
        parity = parity_odd;
      } else if (strstr(str, "even") != NULL) {
        parity = parity_even;
      }
    }
  }
  if (args->size > 4) {
    stopbits = value_int(args->args + 4);
  }
  if (args->size > 5) {
    const char* str = value_str(args->args + 5);
    if (str != NULL) {
      if (strstr(str, "hard") != NULL) {
        flowcontrol = flowcontrol_hardware;
        parity = parity_odd;
      } else if (strstr(str, "soft") != NULL) {
        flowcontrol = flowcontrol_software;
      }
    }
  }

  return_value_if_fail(devname != NULL, RET_BAD_PARAMS);
  obj = TK_OBJECT(tk_iostream_serial_create(devname));
  return_value_if_fail(obj != NULL, RET_BAD_PARAMS);

  value_set_object(result, obj);
  result->free_handle = TRUE;

  tk_iostream_serial_config(TK_IOSTREAM(obj), baudrate, bytesize, parity, stopbits, flowcontrol);

  return RET_OK;
}

FACTORY_TABLE_BEGIN(s_ext_iostream_serial)
FACTORY_TABLE_ENTRY("iostream_serial_create", func_iostream_serial_create)
FACTORY_TABLE_END()

ret_t fscript_iostream_serial_register(void) {
  return fscript_register_funcs(s_ext_iostream_serial);
}
