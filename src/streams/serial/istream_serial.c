/**
 * File:   istream_serial.c
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
#include "streams/serial/istream_serial.h"

static int32_t tk_istream_serial_read(tk_istream_t* stream, uint8_t* buff, uint32_t max_size) {
  tk_istream_serial_t* istream_serial = TK_ISTREAM_SERIAL(stream);
  int32_t ret = serial_read(istream_serial->fd, buff, max_size);

  if (ret < 0) {
    istream_serial->is_broken = TRUE;
  }

  return ret;
}

static ret_t tk_istream_serial_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_istream_serial_t* istream_serial = TK_ISTREAM_SERIAL(obj);
  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, (int)(serial_handle_get_fd(istream_serial->fd)));
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok = istream_serial->fd >= 0 && istream_serial->is_broken == FALSE;
    value_set_bool(v, is_ok);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tk_istream_serial_flush(tk_istream_t* stream) {
  tk_istream_serial_t* s = TK_ISTREAM_SERIAL(stream);

  return serial_iflush(s->fd);
}

static ret_t tk_istream_serial_exec(object_t* obj, const char* name, const char* args) {
  if (tk_str_eq(name, TK_STREAM_CMD_IFLUSH)) {
    return tk_istream_serial_flush(TK_ISTREAM(obj));
  }

  return RET_NOT_IMPL;
}

static const object_vtable_t s_tk_istream_serial_vtable = {.type = "tk_istream_serial",
                                                           .desc = "tk_istream_serial",
                                                           .size = sizeof(tk_istream_serial_t),
                                                           .exec = tk_istream_serial_exec,
                                                           .get_prop = tk_istream_serial_get_prop};

ret_t tk_istream_serial_wait_for_data(tk_istream_t* stream, uint32_t timeout_ms) {
  tk_istream_serial_t* s = TK_ISTREAM_SERIAL(stream);

  return serial_wait_for_data(s->fd, timeout_ms);
}

tk_istream_t* tk_istream_serial_create(serial_handle_t fd) {
  object_t* obj = NULL;
  tk_istream_serial_t* istream_serial = NULL;
  return_value_if_fail(fd >= 0, NULL);

  obj = object_create(&s_tk_istream_serial_vtable);
  istream_serial = TK_ISTREAM_SERIAL(obj);
  return_value_if_fail(istream_serial != NULL, NULL);

  istream_serial->fd = fd;
  TK_ISTREAM(obj)->read = tk_istream_serial_read;
  TK_ISTREAM(obj)->flush = tk_istream_serial_flush;
  TK_ISTREAM(obj)->wait_for_data = tk_istream_serial_wait_for_data;

  return TK_ISTREAM(obj);
}
