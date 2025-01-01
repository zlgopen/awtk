/**
 * File:   client.c
 * Author: AWTK Develop Team
 * Brief:  client
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
#include "service/client.h"
#ifdef WITH_SOCKET
#include "streams/inet/iostream_tcp.h"
#endif /*WITH_SOCKET*/
#include "streams/serial/iostream_serial.h"

ret_t tk_client_init(tk_client_t* client, tk_iostream_t* io, tk_client_on_notify_t on_notify) {
  return_value_if_fail(client != NULL && io != NULL, RET_BAD_PARAMS);

  client->io = io;
  client->on_notify = on_notify;
  client->rtimeout_ms = TK_ISTREAM_DEFAULT_TIMEOUT;
  client->wtimeout_ms = TK_OSTREAM_DEFAULT_TIMEOUT;
  wbuffer_init_extendable(&(client->wb));
  wbuffer_extend_capacity(&(client->wb), 1024);

  return RET_OK;
}

ret_t tk_client_deinit(tk_client_t* client) {
  return_value_if_fail(client != NULL, RET_BAD_PARAMS);

  TK_OBJECT_UNREF(client->io);
  wbuffer_deinit(&(client->wb));

  return RET_OK;
}

static ret_t tk_client_send_req_impl(tk_client_t* client, uint32_t type, uint32_t data_type,
                                     wbuffer_t* wb) {
  int32_t ret = 0;
  uint32_t size = 0;
  const void* data = NULL;
  tk_msg_header_t header;
  tk_iostream_t* io = NULL;
  uint16_t crc_value = PPPINITFCS16;
  uint32_t timeout = client->wtimeout_ms;

  memset(&header, 0x00, sizeof(header));
  return_value_if_fail(wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(client != NULL && client->io != NULL, RET_BAD_PARAMS);

  io = client->io;
  data = wb->data;
  size = wb->cursor;
  if (size > 0) {
    return_value_if_fail(data != NULL, RET_BAD_PARAMS);
  }

  header.type = type;
  header.size = size;
  header.data_type = data_type;

  crc_value = tk_crc16(crc_value, &header, sizeof(header));
  if (data != NULL && size > 0) {
    crc_value = tk_crc16(crc_value, data, size);
  }
  ret = tk_iostream_write_len(io, &header, sizeof(header), timeout);
  return_value_if_fail(ret == sizeof(header), RET_IO);

  if (size > 0) {
    timeout = client->wtimeout_ms * (size / 10240) + client->wtimeout_ms;
    ret = tk_iostream_write_len(io, data, size, timeout);
    return_value_if_fail(ret == size, RET_IO);
  }

  ret = tk_iostream_write_len(io, &crc_value, sizeof(crc_value), client->wtimeout_ms);
  return_value_if_fail(ret == sizeof(crc_value), RET_IO);

  return RET_OK;
}

ret_t tk_client_send_req(tk_client_t* client, uint32_t type, uint32_t data_type, wbuffer_t* wb) {
  int32_t len = 0;
  int32_t retry_times = 0;
  tk_msg_header_t header;
  return_value_if_fail(client != NULL && client->io != NULL, RET_BAD_PARAMS);

  if (client->retry_times < 1) {
    return tk_client_send_req_impl(client, type, data_type, wb);
  }

  while (retry_times < client->retry_times) {
    ret_t ret = tk_client_send_req_impl(client, type, data_type, wb);
    break_if_fail(ret == RET_OK);

    memset(&header, 0x00, sizeof(header));
    len = tk_iostream_read_len(client->io, &header, sizeof(header), client->rtimeout_ms);
    break_if_fail(len == sizeof(header));

    if (header.resp_code == RET_OK) {
      return RET_OK;
    }

    if (header.resp_code == RET_CRC) {
      retry_times++;
      log_debug("crc error, retry times: %d", retry_times);
      continue;
    } else {
      break;
    }
  }

  return RET_FAIL;
}

static ret_t tk_client_confirm_packet(tk_client_t* client, bool_t valid) {
  int32_t ret = 0;
  tk_msg_header_t header;

  header.size = 0;
  header.type = MSG_CODE_CONFIRM;
  header.data_type = MSG_DATA_TYPE_NONE;
  header.resp_code = valid ? RET_OK : RET_CRC;

  ret = tk_iostream_write_len(client->io, &header, sizeof(header), client->wtimeout_ms);

  return ret == sizeof(header) ? RET_OK : RET_FAIL;
}

static ret_t tk_client_read_resp_impl(tk_client_t* client, tk_msg_header_t* header, wbuffer_t* wb) {
  int32_t ret = 0;
  uint16_t crc_value = 0;
  tk_iostream_t* io = NULL;
  uint16_t real_crc_value = PPPINITFCS16;
  return_value_if_fail(client != NULL && client->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(header != NULL && wb != NULL, RET_BAD_PARAMS);

  io = client->io;
  wbuffer_rewind(wb);
  ret = tk_iostream_read_len(io, header, sizeof(*header), client->rtimeout_ms);
  return_value_if_fail(ret == sizeof(*header), RET_IO);

  real_crc_value = tk_crc16(real_crc_value, header, sizeof(*header));
  if (header->size > 0) {
    return_value_if_fail(wbuffer_extend_capacity(wb, header->size) == RET_OK, RET_OOM);
    ret = tk_iostream_read_len(io, wb->data, header->size, client->rtimeout_ms);
    return_value_if_fail(ret == header->size, RET_IO);
    real_crc_value = tk_crc16(real_crc_value, wb->data, header->size);
  }

  ret = tk_iostream_read_len(io, &crc_value, sizeof(crc_value), client->rtimeout_ms);
  return_value_if_fail(ret == sizeof(crc_value), RET_IO);
  return_value_if_fail(real_crc_value == crc_value, RET_CRC);

  wb->cursor = header->size;

  return RET_OK;
}

static ret_t tk_client_read_resp_ex(tk_client_t* client, bool_t read_notify,
                                    tk_msg_header_t* header, wbuffer_t* wb) {
  ret_t ret = RET_OK;
  int32_t retry_times = 0;
  return_value_if_fail(client != NULL && client->io != NULL, RET_BAD_PARAMS);

  if (client->retry_times < 1) {
    while (TRUE) {
      ret = tk_client_read_resp_impl(client, header, wb);
      if (header->type == MSG_CODE_NOTIFY) {
        if (client->on_notify != NULL) {
          client->on_notify(client, header, wb);
        }

        if (read_notify) {
          return ret;
        } else {
          continue;
        }
      } else {
        return ret;
      }
    }
  }

  while (retry_times < client->retry_times) {
    ret = tk_client_read_resp_impl(client, header, wb);
    if (ret != RET_IO) {
      tk_client_confirm_packet(client, ret != RET_CRC);
    }

    if (ret == RET_CRC) {
      retry_times++;
      log_debug("crc error, retry times: %d", retry_times);
      continue;
    } else {
      if (header->type == MSG_CODE_NOTIFY) {
        if (client->on_notify != NULL) {
          client->on_notify(client, header, wb);
        }

        if (read_notify) {
          return ret;
        } else {
          continue;
        }
      }
      return ret;
    }
  }

  return RET_FAIL;
}

ret_t tk_client_read_notify(tk_client_t* client, uint32_t timeout_ms) {
  ret_t ret = RET_OK;
  wbuffer_t* wb = NULL;
  tk_msg_header_t header;
  return_value_if_fail(client != NULL && client->io != NULL, RET_BAD_PARAMS);

  ret = tk_istream_wait_for_data(tk_iostream_get_istream(client->io), timeout_ms);
  if (ret == RET_OK) {
    wb = &(client->wb);
    wbuffer_rewind(wb);
    memset(&header, 0x00, sizeof(header));
    ret = tk_client_read_resp_ex(client, TRUE, &header, wb);
  }

  return ret;
}

ret_t tk_client_read_resp(tk_client_t* client, tk_msg_header_t* header, wbuffer_t* wb) {
  return tk_client_read_resp_ex(client, FALSE, header, wb);
}

ret_t tk_client_request(tk_client_t* client, uint32_t type, uint32_t data_type, wbuffer_t* wb) {
  ret_t ret = tk_client_send_req(client, type, data_type, wb);
  if (ret == RET_OK) {
    tk_msg_header_t header;
    memset(&header, 0x00, sizeof(header));
    ret = tk_client_read_resp(client, &header, wb);
    if (ret == RET_OK) {
      ret = (ret_t)header.resp_code;
    }
  }

  return ret;
}

ret_t tk_client_upload_file(tk_client_t* client, const char* remote_file, const char* local_file) {
  wbuffer_t wb;
  int32_t len = 0;
  ret_t ret = RET_OK;
  fs_file_t* file = NULL;
  uint8_t buff[4096] = {0};
  return_value_if_fail(client != NULL && client->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_file != NULL, RET_BAD_PARAMS);
  return_value_if_fail(local_file != NULL, RET_BAD_PARAMS);

  file = fs_open_file(os_fs(), local_file, "rb");
  return_value_if_fail(file != NULL, RET_BAD_PARAMS);

  wbuffer_init(&wb, (void*)remote_file, strlen(remote_file) + 1);
  wb.cursor = wb.capacity;
  ret = tk_client_request(client, MSG_CODE_UPLOAD_FILE_BEGIN, MSG_DATA_TYPE_STRING, &wb);
  goto_error_if_fail(ret == RET_OK);

  while ((len = fs_file_read(file, buff, sizeof(buff))) > 0) {
    wbuffer_init(&wb, buff, len);
    wb.cursor = len;
    ret = tk_client_request(client, MSG_CODE_UPLOAD_FILE_DATA, MSG_DATA_TYPE_BINARY, &wb);
    break_if_fail(ret == RET_OK);
  }

  wbuffer_rewind(&wb);
  ret = tk_client_request(client, MSG_CODE_UPLOAD_FILE_END, MSG_DATA_TYPE_NONE, &wb);

  fs_file_close(file);

  return ret;
error:
  fs_file_close(file);

  return RET_FAIL;
}

ret_t tk_client_download_file(tk_client_t* client, const char* remote_file,
                              const char* local_file) {
  int32_t len = 0;
  ret_t ret = RET_OK;
  fs_file_t* file = NULL;
  wbuffer_t* wb = NULL;
  tk_msg_header_t header;
  return_value_if_fail(client != NULL && client->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_file != NULL, RET_BAD_PARAMS);
  return_value_if_fail(local_file != NULL, RET_BAD_PARAMS);

  file = fs_open_file(os_fs(), local_file, "wb+");
  return_value_if_fail(file != NULL, RET_BAD_PARAMS);

  wb = &(client->wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, remote_file);
  ret = tk_client_request(client, MSG_CODE_DOWNLOAD_FILE_BEGIN, MSG_DATA_TYPE_STRING, wb);
  goto_error_if_fail(ret == RET_OK);

  memset(&header, 0x00, sizeof(header));

  while ((ret = tk_client_read_resp(client, &header, wb)) == RET_OK) {
    if (header.type == MSG_CODE_DOWNLOAD_FILE_DATA) {
      len = fs_file_write(file, wb->data, wb->cursor);
      break_if_fail(len == wb->cursor);
    } else if (header.type == MSG_CODE_DOWNLOAD_FILE_END) {
      ret = RET_OK;
      break;
    } else {
      assert(!"impossible");
      ret = RET_FAIL;
      break;
    }
  }
  fs_file_close(file);

  return ret;
error:
  fs_file_close(file);

  return ret;
}

ret_t tk_client_set_retry_times(tk_client_t* client, uint32_t retry_times) {
  return_value_if_fail(client != NULL, RET_BAD_PARAMS);

  client->retry_times = retry_times;

  return RET_OK;
}

ret_t tk_client_set_timeout_ms(tk_client_t* client, uint32_t wtimeout_ms, uint32_t rtimeout_ms) {
  return_value_if_fail(client != NULL, RET_BAD_PARAMS);

  client->wtimeout_ms = wtimeout_ms;
  client->rtimeout_ms = rtimeout_ms;

  return RET_OK;
}
