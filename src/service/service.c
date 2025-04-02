/**
 * File:   service.h
 * Author: AWTK Develop Team
 * Brief:  service interface
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "tkc/crc.h"
#include "tkc/utils.h"
#include "service/service.h"
#include "tkc/event_source_fd.h"
#include "streams/inet/iostream_tcp.h"
#include "streams/serial/iostream_serial.h"

#include "tkc/socket_helper.h"

ret_t tk_service_init(tk_service_t* service, tk_iostream_t* io) {
  return_value_if_fail(service != NULL, RET_BAD_PARAMS);

  service->io = io;
  service->mutex = tk_mutex_create();
  wbuffer_init_extendable(&(service->wb));
  wbuffer_extend_capacity(&(service->wb), 1024);

  return RET_OK;
}

ret_t tk_service_lock(tk_service_t* service) {
  return_value_if_fail(service != NULL && service->mutex != NULL, RET_BAD_PARAMS);

  return tk_mutex_lock(service->mutex);
}

ret_t tk_service_unlock(tk_service_t* service) {
  return_value_if_fail(service != NULL && service->mutex != NULL, RET_BAD_PARAMS);

  return tk_mutex_unlock(service->mutex);
}

ret_t tk_service_dispatch(tk_service_t* service) {
  return_value_if_fail(service != NULL && service->dispatch != NULL, RET_BAD_PARAMS);

  return service->dispatch(service);
}

ret_t tk_service_destroy(tk_service_t* service) {
  return_value_if_fail(service != NULL && service->destroy != NULL, RET_BAD_PARAMS);

  wbuffer_deinit(&(service->wb));
  TK_OBJECT_UNREF(service->io);

  tk_mutex_destroy(service->mutex);
  service->mutex = NULL;

  return service->destroy(service);
}

static ret_t tk_service_on_data(event_source_t* source) {
  event_source_fd_t* event_source_fd = EVENT_SOURCE_FD(source);
  tk_service_t* service = (tk_service_t*)(event_source_fd->ctx);

  if (tk_service_dispatch(service) != RET_OK) {
    tk_service_destroy(service);
    event_source_fd->ctx = NULL;
    log_debug("client disconnected\n");
    return RET_REMOVE;
  } else {
    return RET_OK;
  }
}

static ret_t tk_service_on_destory(void* ctx, event_t* e) {
  event_source_fd_t* event_source_fd = EVENT_SOURCE_FD(ctx);
  tk_service_t* service = (tk_service_t*)(event_source_fd->ctx);

  if (service != NULL) {
    tk_service_destroy(service);
    event_source_fd->ctx = NULL;
  }

  return RET_OK;
}

#ifdef WITH_SOCKET
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
        emitter_on(EMITTER(client_source), EVT_DESTROY, tk_service_on_destory, client_source);
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

static ret_t on_source_destroy(void* ctx, event_t* e) {
  event_source_fd_t* source = EVENT_SOURCE_FD(e->target);
  int listen_sock = source->fd;
  int port = tk_pointer_to_int(ctx);

  log_debug("stop service: socket=%d port=%d\n", listen_sock, port);
  socket_close(listen_sock);

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
  emitter_on(EMITTER(source), EVT_DESTROY, on_source_destroy, tk_pointer_from_int(port));

  OBJECT_UNREF(source);
  log_debug("service start: %s\n", url);

  return RET_OK;
}

#endif /*WITH_SOCKET*/

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
#ifdef WITH_SOCKET
  if (tk_str_start_with(url, STR_SCHEMA_TCP)) {
    return tk_service_start_tcp(esm, url, create, args);
  } else
#endif /*WITH_SOCKET*/
    if (tk_str_start_with(url, STR_SCHEMA_SERIAL)) {
      return tk_service_start_serial(esm, url, create, args);
    } else {
      log_debug("not supported: %s\n", url);
      return RET_NOT_IMPL;
    }
}

static ret_t tk_service_confirm_packet(tk_service_t* service, bool_t valid) {
  int32_t ret = 0;
  tk_msg_header_t header;

  header.size = 0;
  header.type = MSG_CODE_CONFIRM;
  header.data_type = MSG_DATA_TYPE_NONE;
  header.resp_code = valid ? RET_OK : RET_CRC;

  ret = tk_iostream_write_len(service->io, &header, sizeof(header), TK_OSTREAM_DEFAULT_TIMEOUT);

  return ret == sizeof(header) ? RET_OK : RET_FAIL;
}

ret_t tk_service_send_resp_impl(tk_service_t* service, uint32_t type, uint32_t data_type,
                                uint32_t resp_code, wbuffer_t* wb) {
  int32_t ret = 0;
  uint32_t size = 0;
  const void* data = NULL;
  tk_msg_header_t header;
  tk_iostream_t* io = NULL;
  uint16_t crc_value = PPPINITFCS16;
  uint32_t timeout = TK_OSTREAM_DEFAULT_TIMEOUT;

  memset(&header, 0x00, sizeof(header));
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(service != NULL && service->io != NULL, RET_BAD_PARAMS);

  io = service->io;
  data = wb->data;
  size = wb->cursor;
  if (size > 0) {
    return_value_if_fail(data != NULL, RET_BAD_PARAMS);
  }

  header.type = type;
  header.size = size;
  header.data_type = data_type;
  header.resp_code = resp_code;

  crc_value = tk_crc16(crc_value, &header, sizeof(header));
  if (data != NULL && size > 0) {
    crc_value = tk_crc16(crc_value, data, size);
  }

  ret = tk_iostream_write_len(io, &header, sizeof(header), timeout);
  return_value_if_fail(ret == sizeof(header), RET_IO);

  if (size > 0) {
    timeout = TK_OSTREAM_DEFAULT_TIMEOUT * (size / 10240) + TK_OSTREAM_DEFAULT_TIMEOUT;
    ret = tk_iostream_write_len(io, data, size, timeout);
    return_value_if_fail(ret == size, RET_IO);
  }

  ret = tk_iostream_write_len(io, &crc_value, sizeof(crc_value), TK_OSTREAM_DEFAULT_TIMEOUT);
  return_value_if_fail(ret == sizeof(crc_value), RET_IO);

  return RET_OK;
}

ret_t tk_service_send_resp(tk_service_t* service, uint32_t type, uint32_t data_type,
                           uint32_t resp_code, wbuffer_t* wb) {
  int32_t len = 0;
  int32_t retry_times = 0;
  tk_msg_header_t header;
  ret_t ret = RET_OK;
  return_value_if_fail(service != NULL && wb != NULL, RET_BAD_PARAMS);

  tk_service_lock(service);
  if (service->retry_times < 1) {
    ret = tk_service_send_resp_impl(service, type, data_type, resp_code, wb);
    goto end;
  }

  while (retry_times < service->retry_times) {
    ret_t ret = tk_service_send_resp_impl(service, type, data_type, resp_code, wb);
    break_if_fail(ret == RET_OK);

    /*读取确认包*/
    memset(&header, 0x00, sizeof(header));
    len = tk_iostream_read_len(service->io, &header, sizeof(header), TK_ISTREAM_DEFAULT_TIMEOUT);
    break_if_fail(len == sizeof(header));

    if (header.resp_code == RET_OK) {
      ret = RET_OK;
      goto end;
    }

    if (header.resp_code == RET_CRC) {
      retry_times++;
      log_debug("crc error, retry times: %d", retry_times);
      continue;
    } else {
      log_debug("unknown error\n.");
      break;
    }
  }
  ret = RET_FAIL;

end:
  tk_service_unlock(service);

  return ret;
}

ret_t tk_service_read_req_impl(tk_service_t* service, tk_msg_header_t* header, wbuffer_t* wb) {
  int32_t ret = 0;
  uint16_t crc_value = 0;
  tk_iostream_t* io = NULL;
  uint16_t real_crc_value = PPPINITFCS16;
  return_value_if_fail(service != NULL && service->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(header != NULL && wb != NULL, RET_BAD_PARAMS);

  io = service->io;
  wbuffer_rewind(wb);
  ret = tk_iostream_read_len(io, header, sizeof(*header), TK_ISTREAM_DEFAULT_TIMEOUT);
  if (ret == 0) {
    return RET_IO;
  }
  return_value_if_fail(ret == sizeof(*header), RET_IO);

  real_crc_value = tk_crc16(real_crc_value, header, sizeof(*header));
  if (header->size > 0) {
    return_value_if_fail(wbuffer_extend_capacity(wb, header->size) == RET_OK, RET_OOM);
    ret = tk_iostream_read_len(io, wb->data, header->size, TK_ISTREAM_DEFAULT_TIMEOUT);
    return_value_if_fail(ret == header->size, RET_IO);
    real_crc_value = tk_crc16(real_crc_value, wb->data, header->size);
  }

  ret = tk_iostream_read_len(io, &crc_value, sizeof(crc_value), TK_ISTREAM_DEFAULT_TIMEOUT);
  return_value_if_fail(ret == sizeof(crc_value), RET_IO);
  return_value_if_fail(crc_value == real_crc_value, RET_CRC);

  wb->cursor = header->size;

  return RET_OK;
}

ret_t tk_service_read_req(tk_service_t* service, tk_msg_header_t* header, wbuffer_t* wb) {
  int32_t retry_times = 0;
  return_value_if_fail(service != NULL && header != NULL && wb != NULL, RET_BAD_PARAMS);

  if (service->retry_times < 1) {
    return tk_service_read_req_impl(service, header, wb);
  }

  while (retry_times < service->retry_times) {
    ret_t ret = tk_service_read_req_impl(service, header, wb);
    if (ret != RET_IO) {
      tk_service_confirm_packet(service, ret != RET_CRC);
    }

    if (ret == RET_CRC) {
      retry_times++;
      log_debug("crc error, retry times: %d", retry_times);
      continue;
    } else {
      return ret;
    }
  }

  return RET_FAIL;
}

ret_t tk_service_upload_file(tk_service_t* service, const char* filename) {
  int32_t len = 0;
  ret_t ret = RET_OK;
  fs_file_t* file = NULL;
  wbuffer_t* wb = NULL;
  tk_msg_header_t header;
  return_value_if_fail(service != NULL && service->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(filename != NULL, RET_BAD_PARAMS);

  wb = &(service->wb);
  wbuffer_rewind(wb);
  file = fs_open_file(os_fs(), filename, "wb+");
  if (file != NULL) {
    tk_service_send_resp(service, MSG_CODE_UPLOAD_FILE_BEGIN, MSG_DATA_TYPE_NONE, RET_OK, wb);
  } else {
    tk_service_send_resp(service, MSG_CODE_UPLOAD_FILE_BEGIN, MSG_DATA_TYPE_NONE, RET_FAIL, wb);
  }
  return_value_if_fail(file != NULL, RET_BAD_PARAMS);

  memset(&header, 0x00, sizeof(header));
  while ((ret = tk_service_read_req(service, &header, wb)) == RET_OK) {
    if (header.type == MSG_CODE_UPLOAD_FILE_DATA) {
      len = fs_file_write(file, wb->data, wb->cursor);
      ret = (len == wb->cursor) ? RET_OK : RET_FAIL;
      tk_service_send_resp(service, MSG_CODE_UPLOAD_FILE_DATA, MSG_DATA_TYPE_NONE, ret, wb);
      break_if_fail(ret == RET_OK);
    } else if (header.type == MSG_CODE_UPLOAD_FILE_END) {
      ret = RET_OK;
      ret = tk_service_send_resp(service, MSG_CODE_UPLOAD_FILE_END, MSG_DATA_TYPE_NONE, ret, wb);
      break_if_fail(ret == RET_OK);
      break;
    } else {
      assert(!"impossible");
      ret = RET_FAIL;
      tk_service_send_resp(service, MSG_CODE_UPLOAD_FILE_END, MSG_DATA_TYPE_NONE, ret, wb);
      break;
    }
  }
  fs_file_close(file);

  return RET_OK;
}

ret_t tk_service_download_file(tk_service_t* service, const char* filename) {
  wbuffer_t wb;
  int32_t len = 0;
  ret_t ret = RET_OK;
  fs_file_t* file = NULL;
  uint8_t buff[4096] = {0};
  return_value_if_fail(service != NULL && service->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(filename != NULL, RET_BAD_PARAMS);

  wbuffer_init(&wb, buff, sizeof(buff));
  file = fs_open_file(os_fs(), filename, "rb");
  if (file != NULL) {
    tk_service_send_resp(service, MSG_CODE_DOWNLOAD_FILE_BEGIN, MSG_DATA_TYPE_NONE, RET_OK, &wb);
  } else {
    tk_service_send_resp(service, MSG_CODE_DOWNLOAD_FILE_BEGIN, MSG_DATA_TYPE_NONE, RET_FAIL, &wb);
  }
  return_value_if_fail(file != NULL, RET_BAD_PARAMS);

  while ((len = fs_file_read(file, buff, sizeof(buff))) > 0) {
    wbuffer_init(&wb, buff, len);
    wb.cursor = len;
    ret = tk_service_send_resp(service, MSG_CODE_DOWNLOAD_FILE_DATA, MSG_DATA_TYPE_BINARY, RET_OK,
                               &wb);
    break_if_fail(ret == RET_OK);
  }

  wbuffer_rewind(&wb);
  ret = tk_service_send_resp(service, MSG_CODE_DOWNLOAD_FILE_END, MSG_DATA_TYPE_NONE, ret, &wb);

  fs_file_close(file);

  return RET_OK;
}

ret_t tk_service_set_retry_times(tk_service_t* service, uint32_t retry_times) {
  return_value_if_fail(service != NULL, RET_BAD_PARAMS);

  service->retry_times = retry_times;

  return RET_OK;
}
