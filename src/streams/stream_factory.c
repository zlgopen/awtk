/**
 * File:   stream_factory.h
 * Author: AWTK Develop Team
 * Brief:  stream factory
 *
 * Copyright (c) 2019 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-10-02 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/url.h"
#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/serial_helper.h"
#include "streams/inet/iostream_tcp.h"
#include "streams/inet/iostream_udp.h"
#include "streams/serial/iostream_serial.h"

#include "streams/stream_factory.h"

tk_iostream_t* tk_stream_factory_create_iostream(const char* url) {
  url_t* aurl = NULL;
  tk_iostream_t* io = NULL;
  return_value_if_fail(url != NULL, NULL);
  aurl = url_create(url);
  return_value_if_fail(aurl != NULL, NULL);

  if (tk_str_start_with(url, STR_SCHEMA_TCP)) {
#ifdef WITH_SOCKET	  
    io = tk_iostream_tcp_create_client(aurl->host, aurl->port);
#endif/*WITH_SOCKET*/    
  } else if (tk_str_start_with(url, STR_SCHEMA_UDP)) {
#ifdef WITH_SOCKET	  
    io = tk_iostream_udp_create_client(aurl->host, aurl->port);
#endif/*WITH_SOCKET*/    
  } else if (tk_str_eq(aurl->schema, "serial")) {
    char filename[MAX_PATH + 1] = {0};

    path_build(filename, MAX_PATH, aurl->host, aurl->path, NULL);
    io = tk_iostream_serial_create(filename);

    if (io != NULL) {
      int baudrate = url_get_param_int32(aurl, "baudrate", 115200);
      stopbits_t stopbits = serial_stopbits_from_str(url_get_param(aurl, "stopbits"));
      parity_t parity = serial_parity_from_str(url_get_param(aurl, "parity"));
      flowcontrol_t flowcontrol = serial_flowcontrol_from_str(url_get_param(aurl, "flowcontrol"));
      bytesize_t bytesize = serial_bytesize_from_str(url_get_param(aurl, "bytesize"));

      tk_iostream_serial_config(io, baudrate, bytesize, parity, stopbits, flowcontrol);
    }
  } else {
    log_warn("not support:%s\n", url);
  }
  url_destroy(aurl);

  return io;
}
