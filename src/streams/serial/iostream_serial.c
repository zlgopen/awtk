/**
 * File:   iostream_serial.c
 * Author: AWTK Develop Team
 * Brief:  input stream base on serial port
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
#include "streams/serial/istream_serial.h"
#include "streams/serial/ostream_serial.h"
#include "streams/serial/iostream_serial.h"

static ret_t tk_iostream_serial_get_prop(object_t* obj, const char* name, value_t* v) {
  tk_iostream_serial_t* iostream_serial = TK_IOSTREAM_SERIAL(obj);

  if (tk_str_eq(name, TK_STREAM_PROP_FD)) {
    value_set_int(v, (int32_t)(serial_handle_get_fd(iostream_serial->fd)));
    return RET_OK;
  } else if (tk_str_eq(name, TK_IOSTREAM_SERIAL_PROP_PARITY)) {
    value_set_uint8(v, iostream_serial->parity);
    return RET_OK;
  } else if (tk_str_eq(name, TK_IOSTREAM_SERIAL_PROP_STOPBITS)) {
    value_set_uint8(v, iostream_serial->stopbits);
    return RET_OK;
  } else if (tk_str_eq(name, TK_IOSTREAM_SERIAL_PROP_BYTESIZE)) {
    value_set_uint8(v, iostream_serial->bytesize);
    return RET_OK;
  } else if (tk_str_eq(name, TK_IOSTREAM_SERIAL_PROP_BAUDRATE)) {
    value_set_int(v, iostream_serial->baudrate);
    return RET_OK;
  } else if (tk_str_eq(name, TK_IOSTREAM_SERIAL_PROP_FLOWCONTROL)) {
    value_set_uint8(v, iostream_serial->flowcontrol);
    return RET_OK;
  } else if (tk_str_eq(name, TK_STREAM_PROP_IS_OK)) {
    bool_t is_ok1 =
        object_get_prop_bool(OBJECT(iostream_serial->istream), TK_STREAM_PROP_IS_OK, TRUE);
    bool_t is_ok2 =
        object_get_prop_bool(OBJECT(iostream_serial->ostream), TK_STREAM_PROP_IS_OK, TRUE);

    value_set_bool(v, is_ok1 && is_ok2);

    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tk_iostream_serial_set_prop(object_t* obj, const char* name, const value_t* v) {
  tk_iostream_serial_t* iostream_serial = TK_IOSTREAM_SERIAL(obj);

  if (tk_str_eq(name, TK_IOSTREAM_SERIAL_PROP_PARITY)) {
    iostream_serial->parity = (parity_t)value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, TK_IOSTREAM_SERIAL_PROP_STOPBITS)) {
    iostream_serial->stopbits = (stopbits_t)value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, TK_IOSTREAM_SERIAL_PROP_BYTESIZE)) {
    iostream_serial->bytesize = (bytesize_t)value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, TK_IOSTREAM_SERIAL_PROP_BAUDRATE)) {
    iostream_serial->baudrate = value_int(v);
    return RET_OK;
  } else if (tk_str_eq(name, TK_IOSTREAM_SERIAL_PROP_FLOWCONTROL)) {
    iostream_serial->flowcontrol = (flowcontrol_t)value_int(v);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t tk_iostream_serial_exec(object_t* obj, const char* name, const char* args) {
  tk_iostream_serial_t* s = TK_IOSTREAM_SERIAL(obj);
  if (tk_str_eq(name, TK_STREAM_CMD_IFLUSH)) {
    return object_exec(OBJECT(s->istream), TK_STREAM_CMD_IFLUSH, args);
  } else if (tk_str_eq(name, TK_STREAM_CMD_OFLUSH)) {
    return object_exec(OBJECT(s->ostream), TK_STREAM_CMD_IFLUSH, args);
  } else if (tk_str_eq(name, TK_IOSTREAM_SERIAL_CMD_CONFIG)) {
    int ret =
        serial_config(s->fd, s->baudrate, s->bytesize, s->stopbits, s->flowcontrol, s->parity);
    log_debug("baudrate=%d bytesize=%d stopbits=%d flowcontrol=%d parity=%d\n", s->baudrate,
              s->bytesize, s->stopbits, s->flowcontrol, s->parity);
    serial_oflush(s->fd);

    return ret == 0 ? RET_OK : RET_FAIL;
  }

  return RET_NOT_IMPL;
}

static ret_t tk_iostream_serial_on_destroy(object_t* obj) {
  tk_iostream_serial_t* iostream_serial = TK_IOSTREAM_SERIAL(obj);

  serial_close(iostream_serial->fd);
  object_unref(OBJECT(iostream_serial->istream));
  object_unref(OBJECT(iostream_serial->ostream));

  return RET_OK;
}

static const object_vtable_t s_tk_iostream_serial_vtable = {
    .type = "tk_iostream_serial",
    .desc = "tk_iostream_serial",
    .size = sizeof(tk_iostream_serial_t),
    .get_prop = tk_iostream_serial_get_prop,
    .set_prop = tk_iostream_serial_set_prop,
    .exec = tk_iostream_serial_exec,
    .on_destroy = tk_iostream_serial_on_destroy};

static tk_istream_t* tk_iostream_serial_get_istream(tk_iostream_t* stream) {
  tk_iostream_serial_t* iostream_serial = TK_IOSTREAM_SERIAL(stream);

  return iostream_serial->istream;
}

static tk_ostream_t* tk_iostream_serial_get_ostream(tk_iostream_t* stream) {
  tk_iostream_serial_t* iostream_serial = TK_IOSTREAM_SERIAL(stream);

  return iostream_serial->ostream;
}

tk_iostream_t* tk_iostream_serial_create(const char* port) {
  object_t* obj = NULL;
  serial_handle_t fd = 0;
  tk_iostream_serial_t* iostream_serial = NULL;
  return_value_if_fail(port != NULL, NULL);

  fd = serial_open(port);
  return_value_if_fail(fd > 0, NULL);

  obj = object_create(&s_tk_iostream_serial_vtable);
  iostream_serial = TK_IOSTREAM_SERIAL(obj);
  if (iostream_serial == NULL) {
    serial_close(fd);
    return_value_if_fail(iostream_serial != NULL, NULL);
  }

  iostream_serial->fd = fd;
  iostream_serial->baudrate = 115200;
  iostream_serial->parity = parity_none;
  iostream_serial->bytesize = eightbits;
  iostream_serial->stopbits = stopbits_one;
  iostream_serial->flowcontrol = flowcontrol_none;

  iostream_serial->istream = tk_istream_serial_create(fd);
  iostream_serial->ostream = tk_ostream_serial_create(fd);
  TK_IOSTREAM(obj)->get_istream = tk_iostream_serial_get_istream;
  TK_IOSTREAM(obj)->get_ostream = tk_iostream_serial_get_ostream;

  object_exec(obj, TK_IOSTREAM_SERIAL_CMD_CONFIG, NULL);

  return TK_IOSTREAM(obj);
}

ret_t tk_iostream_serial_config(tk_iostream_t* iostream, int32_t baudrate, bytesize_t bytesize,
                                parity_t parity, stopbits_t stopbits, flowcontrol_t flowcontrol) {
  return_value_if_fail(iostream != NULL, RET_BAD_PARAMS);

  object_set_prop_int(OBJECT(iostream), TK_IOSTREAM_SERIAL_PROP_STOPBITS, stopbits);
  object_set_prop_int(OBJECT(iostream), TK_IOSTREAM_SERIAL_PROP_BAUDRATE, baudrate);
  object_set_prop_int(OBJECT(iostream), TK_IOSTREAM_SERIAL_PROP_PARITY, parity);
  object_set_prop_int(OBJECT(iostream), TK_IOSTREAM_SERIAL_PROP_BYTESIZE, bytesize);
  object_set_prop_int(OBJECT(iostream), TK_IOSTREAM_SERIAL_PROP_FLOWCONTROL, flowcontrol);

  object_exec(OBJECT(iostream), TK_IOSTREAM_SERIAL_CMD_CONFIG, NULL);

  return RET_OK;
}
