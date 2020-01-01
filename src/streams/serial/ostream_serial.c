/**
 * File:   ostream_serial.c
 * Author: AWTK Develop Team
 * Brief:  input stream base on serial
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
 * 2019-09-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "streams/serial/serial_helper.h"
#include "streams/serial/ostream_serial.h"

static int32_t tk_ostream_serial_write(tk_ostream_t* stream, const uint8_t* buff,
                                       uint32_t max_size) {
  tk_ostream_serial_t* ostream_serial = TK_OSTREAM_SERIAL(stream);
  int32_t ret = serial_write(ostream_serial->fd, buff, max_size);

  if (ret < 0) {
    ostream_serial->is_broken = TRUE;
  }

  return ret;
}

static ret_t tk_ostream_serial_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_ostream_serial_t* ostream_serial = TK_OSTREAM_SERIAL(obj);
  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, serial_handle_get_fd(ostream_serial->fd));
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok = ostream_serial->fd >= 0 && ostream_serial->is_broken == FALSE;
    value_set_bool(v, is_ok);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tk_ostream_serial_flush(tk_ostream_t* stream) {
  tk_ostream_serial_t* ostream_serial = TK_OSTREAM_SERIAL(stream);

  return serial_oflush(ostream_serial->fd);
}

static ret_t tk_ostream_serial_exec(object_t* obj, const char* name, const char* args) {
  if (tk_str_eq(name, TK_STREAM_CMD_OFLUSH)) {
    return tk_ostream_serial_flush(TK_OSTREAM(obj));
  }

  return RET_NOT_IMPL;
}

static const object_vtable_t s_tk_ostream_serial_vtable = {.type = "tk_ostream_serial",
                                                           .desc = "tk_ostream_serial",
                                                           .size = sizeof(tk_ostream_serial_t),
                                                           .exec = tk_ostream_serial_exec,
                                                           .get_prop = tk_ostream_serial_get_prop};

tk_ostream_t* tk_ostream_serial_create(serial_handle_t fd) {
  object_t* obj = NULL;
  tk_ostream_serial_t* ostream_serial = NULL;
  return_value_if_fail(fd >= 0, NULL);

  obj = object_create(&s_tk_ostream_serial_vtable);
  ostream_serial = TK_OSTREAM_SERIAL(obj);
  return_value_if_fail(ostream_serial != NULL, NULL);

  ostream_serial->fd = fd;
  TK_OSTREAM(obj)->write = tk_ostream_serial_write;
  TK_OSTREAM(obj)->flush = tk_ostream_serial_flush;

  return TK_OSTREAM(obj);
}
