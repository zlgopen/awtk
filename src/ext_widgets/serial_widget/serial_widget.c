/**
 * File:   serial_widget.c
 * Author: AWTK Develop Team
 * Brief:  用于串口通信的控件
 *
 * Copyright (c) 2022 - 2022 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2022-07-08 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utils.h"
#include "tkc/event_source_fd.h"

#include "base/main_loop.h"

#include "serial_widget.h"
#include "streams/serial/serial_helper.h"
#include "streams/serial/iostream_serial.h"

static ret_t serial_widget_apply_props(widget_t* widget);

ret_t serial_widget_set_baudrate(widget_t* widget, uint32_t baudrate) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);
  return_value_if_fail(serial_widget != NULL, RET_BAD_PARAMS);

  serial_widget->baudrate = baudrate;
  serial_widget_apply_props(widget);

  return RET_OK;
}

ret_t serial_widget_set_device(widget_t* widget, const char* device) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);
  return_value_if_fail(serial_widget != NULL, RET_BAD_PARAMS);

  serial_widget->device = tk_str_copy(serial_widget->device, device);
  serial_widget_apply_props(widget);

  return RET_OK;
}

ret_t serial_widget_set_bytesize(widget_t* widget, uint32_t bytesize) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);
  return_value_if_fail(serial_widget != NULL, RET_BAD_PARAMS);

  serial_widget->bytesize = bytesize;
  serial_widget_apply_props(widget);

  return RET_OK;
}

ret_t serial_widget_set_parity(widget_t* widget, uint32_t parity) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);
  return_value_if_fail(serial_widget != NULL, RET_BAD_PARAMS);

  serial_widget->parity = parity;
  serial_widget_apply_props(widget);

  return RET_OK;
}

ret_t serial_widget_set_stopbits(widget_t* widget, uint32_t stopbits) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);
  return_value_if_fail(serial_widget != NULL, RET_BAD_PARAMS);

  serial_widget->stopbits = stopbits;
  serial_widget_apply_props(widget);

  return RET_OK;
}

ret_t serial_widget_set_flowcontrol(widget_t* widget, uint32_t flowcontrol) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);
  return_value_if_fail(serial_widget != NULL, RET_BAD_PARAMS);

  serial_widget->flowcontrol = flowcontrol;
  serial_widget_apply_props(widget);

  return RET_OK;
}

ret_t serial_widget_set_check_interval(widget_t* widget, uint32_t check_interval) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);
  return_value_if_fail(serial_widget != NULL, RET_BAD_PARAMS);

  serial_widget->check_interval = check_interval;
  serial_widget_apply_props(widget);

  return RET_OK;
}

static ret_t serial_widget_get_prop(widget_t* widget, const char* name, value_t* v) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);
  return_value_if_fail(serial_widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(SERIAL_WIDGET_PROP_BAUDRATE, name)) {
    value_set_uint32(v, serial_widget->baudrate);
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_DEVICE, name)) {
    value_set_str(v, serial_widget->device);
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_BYTESIZE, name)) {
    value_set_uint32(v, serial_widget->bytesize);
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_PARITY, name)) {
    value_set_uint32(v, serial_widget->parity);
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_STOPBITS, name)) {
    value_set_uint32(v, serial_widget->stopbits);
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_FLOWCONTROL, name)) {
    value_set_uint32(v, serial_widget->flowcontrol);
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_ISTREAM, name)) {
    value_set_object(v, TK_OBJECT(serial_widget->istream));
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_OSTREAM, name)) {
    value_set_object(v, TK_OBJECT(serial_widget->ostream));
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_IOSTREAM, name)) {
    value_set_object(v, TK_OBJECT(serial_widget->iostream));
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_CHECK_INTERVAL, name)) {
    value_set_uint32(v, serial_widget->check_interval);
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t serial_widget_on_data(widget_t* widget) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);

  widget_dispatch_simple_event(widget, EVT_DATA);
  if (!tk_object_get_prop_bool(TK_OBJECT(serial_widget->iostream), TK_STREAM_PROP_IS_OK, FALSE)) {
    log_debug("stream broken\n");
    return RET_FAIL;
  }

  return RET_OK;
}

static ret_t serial_widget_close_device(widget_t* widget) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);

  TK_OBJECT_UNREF(serial_widget->iostream);
  serial_widget->istream = NULL;
  serial_widget->ostream = NULL;

  if (serial_widget->idle_id != TK_INVALID_ID) {
    idle_remove(serial_widget->idle_id);
    serial_widget->idle_id = TK_INVALID_ID;
  }

  if (serial_widget->timer_id != TK_INVALID_ID) {
    timer_remove(serial_widget->timer_id);
    serial_widget->timer_id = TK_INVALID_ID;
  }

  if (serial_widget->event_source != NULL) {
    main_loop_remove_event_source(main_loop(), serial_widget->event_source);
    serial_widget->event_source = NULL;
  }

  return RET_OK;
}

static ret_t serial_widget_on_event_source_event(event_source_t* source) {
  event_source_fd_t* fd = EVENT_SOURCE_FD(source);
  serial_widget_t* serial_widget = SERIAL_WIDGET(fd->ctx);
  widget_t* widget = WIDGET(serial_widget);

  if (serial_widget_on_data(widget) == RET_OK) {
    return RET_OK;
  } else {
    serial_widget_close_device(widget);
    return RET_REMOVE;
  }
}

static ret_t serial_widget_check_if_data_available(const timer_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);
  serial_widget_t* serial_widget = SERIAL_WIDGET(info->ctx);

  if (serial_widget->iostream != NULL) {
    ret_t ret = tk_iostream_serial_wait_for_data(serial_widget->iostream, 10);
    if (ret == RET_OK) {
      if (serial_widget_on_data(WIDGET(serial_widget)) == RET_OK) {
        return RET_REPEAT;
      }
    } else if (ret == RET_TIMEOUT) {
      return RET_REPEAT;
    }
  }

  serial_widget_close_device(widget);
  serial_widget->timer_id = TK_INVALID_ID;

  return RET_REMOVE;
}

static ret_t serial_widget_apply_props_async(const idle_info_t* info) {
  int fd = -1;
  widget_t* widget = WIDGET(info->ctx);
  serial_widget_t* serial_widget = SERIAL_WIDGET(info->ctx);

  serial_widget_close_device(widget);
  return_value_if_fail(serial_widget->device != NULL, RET_REMOVE);
  serial_widget->iostream = tk_iostream_serial_create(serial_widget->device);
  return_value_if_fail(serial_widget->iostream != NULL, RET_REMOVE);

  tk_iostream_serial_config(serial_widget->iostream, serial_widget->baudrate,
                            serial_widget->bytesize, serial_widget->parity, serial_widget->stopbits,
                            serial_widget->flowcontrol);
  serial_widget->istream = tk_iostream_get_istream(serial_widget->iostream);
  serial_widget->ostream = tk_iostream_get_ostream(serial_widget->iostream);

  fd = tk_object_get_prop_int(TK_OBJECT(serial_widget->iostream), TK_STREAM_PROP_FD, -1);
  if (fd >= 0) {
    serial_widget->event_source =
        event_source_fd_create(fd, serial_widget_on_event_source_event, serial_widget);
    main_loop_add_event_source(main_loop(), serial_widget->event_source);
  } else {
    serial_widget->timer_id = timer_add(serial_widget_check_if_data_available, serial_widget,
                                        serial_widget->check_interval);
  }

  log_debug("open serial %s ok\n", serial_widget->device);

  return RET_REMOVE;
}

static ret_t serial_widget_apply_props(widget_t* widget) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);
  return_value_if_fail(serial_widget != NULL, RET_BAD_PARAMS);

  if (serial_widget->idle_id == TK_INVALID_ID && serial_widget->device != NULL) {
    serial_widget->idle_id = idle_add(serial_widget_apply_props_async, widget);
  }

  return RET_OK;
}

static ret_t serial_widget_set_prop(widget_t* widget, const char* name, const value_t* v) {
  return_value_if_fail(widget != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(SERIAL_WIDGET_PROP_BAUDRATE, name)) {
    serial_widget_set_baudrate(widget, value_uint32(v));
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_DEVICE, name)) {
    serial_widget_set_device(widget, value_str(v));
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_BYTESIZE, name)) {
    if (v->type == VALUE_TYPE_STRING) {
      serial_widget_set_bytesize(widget, serial_bytesize_from_str(value_str(v)));
    } else {
      serial_widget_set_bytesize(widget, value_uint32(v));
    }
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_PARITY, name)) {
    if (v->type == VALUE_TYPE_STRING) {
      serial_widget_set_parity(widget, serial_parity_from_str(value_str(v)));
    } else {
      serial_widget_set_parity(widget, value_uint32(v));
    }
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_STOPBITS, name)) {
    if (v->type == VALUE_TYPE_STRING) {
      serial_widget_set_stopbits(widget, serial_stopbits_from_str(value_str(v)));
    } else {
      serial_widget_set_stopbits(widget, value_uint32(v));
    }
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_FLOWCONTROL, name)) {
    if (v->type == VALUE_TYPE_STRING) {
      serial_widget_set_flowcontrol(widget, serial_flowcontrol_from_str(value_str(v)));
    } else {
      serial_widget_set_flowcontrol(widget, value_uint32(v));
    }
    return RET_OK;
  } else if (tk_str_eq(SERIAL_WIDGET_PROP_CHECK_INTERVAL, name)) {
    serial_widget_set_check_interval(widget, value_uint32(v));
    return RET_OK;
  }

  return RET_NOT_FOUND;
}

static ret_t serial_widget_on_destroy(widget_t* widget) {
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);
  return_value_if_fail(widget != NULL && serial_widget != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(serial_widget->device);
  serial_widget_close_device(widget);

  return RET_OK;
}

static ret_t serial_widget_on_paint_self(widget_t* widget, canvas_t* c) {
  return RET_OK;
}

static ret_t serial_widget_on_event(widget_t* widget, event_t* e) {
  return RET_OK;
}

const char* s_serial_widget_properties[] = {SERIAL_WIDGET_PROP_BAUDRATE,
                                            SERIAL_WIDGET_PROP_BYTESIZE,
                                            SERIAL_WIDGET_PROP_PARITY,
                                            SERIAL_WIDGET_PROP_STOPBITS,
                                            SERIAL_WIDGET_PROP_FLOWCONTROL,
                                            SERIAL_WIDGET_PROP_DEVICE,
                                            NULL};

TK_DECL_VTABLE(serial_widget) = {.size = sizeof(serial_widget_t),
                                 .type = WIDGET_TYPE_SERIAL_WIDGET,
                                 .clone_properties = s_serial_widget_properties,
                                 .persistent_properties = s_serial_widget_properties,
                                 .get_parent_vt = TK_GET_PARENT_VTABLE(widget),
                                 .create = serial_widget_create,
                                 .on_paint_self = serial_widget_on_paint_self,
                                 .set_prop = serial_widget_set_prop,
                                 .get_prop = serial_widget_get_prop,
                                 .on_event = serial_widget_on_event,
                                 .on_destroy = serial_widget_on_destroy};

widget_t* serial_widget_create(widget_t* parent, xy_t x, xy_t y, wh_t w, wh_t h) {
  widget_t* widget = widget_create(parent, TK_REF_VTABLE(serial_widget), x, y, w, h);
  serial_widget_t* serial_widget = SERIAL_WIDGET(widget);
  return_value_if_fail(serial_widget != NULL, NULL);

  serial_widget->baudrate = 115200;
  serial_widget->bytesize = eightbits;
  serial_widget->stopbits = stopbits_one;
  serial_widget->flowcontrol = flowcontrol_none;
  serial_widget->check_interval = 100;

  return widget;
}

widget_t* serial_widget_cast(widget_t* widget) {
  return_value_if_fail(WIDGET_IS_INSTANCE_OF(widget, serial_widget), NULL);

  return widget;
}
