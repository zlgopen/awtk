/**
 * File:   service.h
 * Author: AWTK Develop Team
 * Brief:  service interface
 *
 * Copyright (c) 2018 - 2023  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2023-11-05 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "tkc/url.h"
#include "tkc/utils.h"
#include "service/service.h"
#include "tkc/event_source_fd.h"
#include "streams/inet/iostream_tcp.h"
#include "streams/serial/iostream_serial.h"

#include "tkc/socket_helper.h"

ret_t tk_service_dispatch(tk_service_t* service) {
  return_value_if_fail(service != NULL && service->dispatch != NULL, RET_BAD_PARAMS);

  return service->dispatch(service);
}

ret_t tk_service_destroy(tk_service_t* service) {
  return_value_if_fail(service != NULL && service->destroy != NULL, RET_BAD_PARAMS);

  return service->destroy(service);
}

static ret_t tk_service_on_data(event_source_t* source) {
  event_source_fd_t* event_source_fd = (event_source_fd_t*)source;
  tk_service_t* service = (tk_service_t*)(event_source_fd->ctx);

  if (tk_service_dispatch(service) != RET_OK) {
    tk_service_destroy(service);
    log_debug("client disconnected\n");
    return RET_REMOVE;
  } else {
    return RET_OK;
  }
}

static ret_t tk_service_tcp_on_client(event_source_t* source) {
  event_source_fd_t* event_source_fd = (event_source_fd_t*)source;
  tk_service_create_t create = (tk_service_create_t)(event_source_fd->ctx);

  int listen_sock = event_source_get_fd(source);
  int sock = tcp_accept(listen_sock);

  if (sock >= 0) {
    log_debug("client connected:%d\n", sock);
    tk_iostream_t* io = tk_iostream_tcp_create(sock);
    if (io != NULL) {
      tk_service_t* service = create(io, event_source_fd->ctx2);
      if (service != NULL) {
        event_source_manager_t* esm = source->manager;
        event_source_t* client_source = event_source_fd_create(sock, tk_service_on_data, service);
        event_source_manager_add(esm, client_source);
        TK_OBJECT_UNREF(client_source);
      } else {
        TK_OBJECT_UNREF(io);
      }
    } else {
      log_debug("oom! disconnected:%d\n", sock);
      tk_socket_close(sock);
    }
  } else {
    log_debug("error disconnected:%d\n", sock);
    tk_socket_close(sock);
  }

  return RET_OK;
}

static ret_t tk_service_start_tcp(event_source_manager_t* esm, const char* url,
                                  tk_service_create_t create, void* args) {
  int port = 0;
  int listen_sock = -1;
  event_source_t* source = NULL;
  url_t* aurl = url_create(url);
  return_value_if_fail(esm != NULL && aurl != NULL && create != NULL, RET_BAD_PARAMS);

  port = aurl->port;
  listen_sock = tcp_listen(port);
  url_destroy(aurl);
  return_value_if_fail(listen_sock >= 0, RET_BAD_PARAMS);

  log_debug("listen on %d listen_sock=%d\n", port, listen_sock);

  source = event_source_fd_create(listen_sock, tk_service_tcp_on_client, create);
  return_value_if_fail(source != NULL, RET_OOM);
  EVENT_SOURCE_FD(source)->ctx2 = args;
  event_source_manager_add(esm, source);
  OBJECT_UNREF(source);
  log_debug("service start: %s\n", url);

  return RET_OK;
}

static ret_t tk_service_start_serial(event_source_manager_t* esm, const char* url,
                                     tk_service_create_t create, void* args) {
  tk_iostream_t* io = NULL;
  tk_service_t* service = NULL;
  return_value_if_fail(esm != NULL && create != NULL, RET_BAD_PARAMS);

  io = tk_iostream_serial_create_ex(url);
  return_value_if_fail(io != NULL, RET_BAD_PARAMS);

  service = create(io, args);
  if (service != NULL) {
    int fd = tk_object_get_prop_int(TK_OBJECT(io), TK_STREAM_PROP_FD, -1);
    event_source_t* client_source = event_source_fd_create(fd, tk_service_on_data, service);
    event_source_manager_add(esm, client_source);
    TK_OBJECT_UNREF(client_source);
    log_debug("service start: %s\n", url);
  } else {
    TK_OBJECT_UNREF(io);
  }

  return RET_OK;
}

ret_t tk_service_start(event_source_manager_t* esm, const char* url, tk_service_create_t create,
                       void* args) {
  return_value_if_fail(esm != NULL && create != NULL, RET_BAD_PARAMS);

  if (tk_str_start_with(url, STR_SCHEMA_TCP)) {
    return tk_service_start_tcp(esm, url, create, args);
  } else if (tk_str_start_with(url, STR_SCHEMA_SERIAL)) {
    return tk_service_start_serial(esm, url, create, args);
  } else {
    log_debug("not supported: %s\n", url);
    return RET_NOT_IMPL;
  }
}
