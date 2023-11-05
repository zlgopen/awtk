/**
 * File:   remote_ui.c
 * Author: AWTK Develop Team
 * Brief:  remote ui client
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
 * 2023-11-04 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/crc.h"
#include "tkc/utils.h"
#include "base/keys.h"
#include "base/events.h"
#include "conf_io/conf_ubjson.h"
#include "tkc/object_default.h"

#include "remote_ui/client/remote_ui.h"
#include "remote_ui/shared/remote_ui_types_def.h"

#define REMOTE_UI_ISTREAM_TIMEOUT 100 * 1000

remote_ui_t* remote_ui_create(tk_iostream_t* io) {
  remote_ui_t* ui = NULL;
  return_value_if_fail(io != NULL, NULL);

  ui = (remote_ui_t*)TKMEM_ZALLOC(remote_ui_t);
  return_value_if_fail(ui != NULL, NULL);

  ui->io = io;
  ui->event_handlers = object_default_create_ex(FALSE);
  wbuffer_init_extendable(&(ui->wb));

  return ui;
}

static ret_t remote_ui_send_req(tk_iostream_t* io, uint32_t type, uint32_t data_type,
                                wbuffer_t* wb) {
  int32_t ret = 0;
  const void* data = NULL;
  uint32_t size = 0;
  uint32_t timeout = TK_OSTREAM_DEFAULT_TIMEOUT;
  uint16_t crc_value = PPPINITFCS16;
  remote_ui_msg_header_t header;
  memset(&header, 0x00, sizeof(header));
  return_value_if_fail(io != NULL && wb != NULL, RET_BAD_PARAMS);

  size = wb->cursor;
  data = wb->data;
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
    timeout = TK_OSTREAM_DEFAULT_TIMEOUT * (size / 10240) + TK_OSTREAM_DEFAULT_TIMEOUT;
    ret = tk_iostream_write_len(io, data, size, timeout);
    return_value_if_fail(ret == size, RET_IO);
  }

  ret = tk_iostream_write_len(io, &crc_value, sizeof(crc_value), TK_OSTREAM_DEFAULT_TIMEOUT);
  return_value_if_fail(ret == sizeof(crc_value), RET_IO);

  return RET_OK;
}

static ret_t remote_ui_read_resp(tk_iostream_t* io, remote_ui_msg_header_t* header, wbuffer_t* wb) {
  int32_t ret = 0;
  uint16_t crc_value = 0;
  uint16_t real_crc_value = PPPINITFCS16;
  return_value_if_fail(io != NULL && header != NULL && wb != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(wb);
  ret = tk_iostream_read_len(io, header, sizeof(*header), REMOTE_UI_ISTREAM_TIMEOUT);
  return_value_if_fail(ret == sizeof(*header), RET_IO);

  real_crc_value = tk_crc16(real_crc_value, header, sizeof(*header));
  if (header->size > 0) {
    return_value_if_fail(wbuffer_extend_capacity(wb, header->size) == RET_OK, RET_OOM);
    ret = tk_iostream_read_len(io, wb->data, header->size, REMOTE_UI_ISTREAM_TIMEOUT);
    return_value_if_fail(ret == header->size, RET_IO);
    real_crc_value = tk_crc16(real_crc_value, wb->data, header->size);
  }

  ret = tk_iostream_read_len(io, &crc_value, sizeof(crc_value), REMOTE_UI_ISTREAM_TIMEOUT);
  return_value_if_fail(ret == sizeof(crc_value), RET_IO);
  return_value_if_fail(real_crc_value == crc_value, RET_CRC);

  wb->cursor = header->size;

  return header->resp_code;
}

static ret_t remote_ui_request(tk_iostream_t* io, uint32_t type, uint32_t data_type,
                               wbuffer_t* wb) {
  ret_t ret = remote_ui_send_req(io, type, data_type, wb);
  if (ret == RET_OK) {
    remote_ui_msg_header_t header;
    memset(&header, 0x00, sizeof(header));
    ret = remote_ui_read_resp(io, &header, wb);
  }

  return ret;
}

static ubjson_writer_t* remote_ui_client_get_writer(remote_ui_t* ui) {
  wbuffer_t* wb = &(ui->wb);
  ubjson_writer_t* writer = &(ui->writer);

  wb->cursor = 0;
  return ubjson_writer_init(writer, (ubjson_write_callback_t)wbuffer_write_binary, wb);
}

ret_t remote_ui_login(remote_ui_t* ui, const char* username, const char* password) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(username != NULL, RET_BAD_PARAMS);
  return_value_if_fail(password != NULL, RET_BAD_PARAMS);
  writer = remote_ui_client_get_writer(ui);

  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_USERNAME, username);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_PASSWORD, password);
  ubjson_writer_write_object_end(writer);

  return remote_ui_request(ui->io, REMOTE_UI_REQ_LOGIN, REMOTE_UI_DATA_TYPE_UBJSON, &(ui->wb));
}

ret_t remote_ui_logout(remote_ui_t* ui) {
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->wb));
  return remote_ui_request(ui->io, REMOTE_UI_REQ_LOGOUT, REMOTE_UI_DATA_TYPE_NONE, &(ui->wb));
}

ret_t remote_ui_get_dev_info(remote_ui_t* ui, remote_ui_dev_info_t* info) {
  ret_t ret = RET_FAIL;
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  wb = &(ui->wb);
  wbuffer_rewind(wb);
  memset(info, 0x00, sizeof(*info));
  ret = remote_ui_request(ui->io, REMOTE_UI_REQ_GET_DEV_INFO, REMOTE_UI_DATA_TYPE_UBJSON, wb);
  if (ret == RET_OK) {
    tk_object_t* obj = conf_ubjson_load_from_buff(wb->data, wb->cursor, FALSE);
    if (obj != NULL) {
      const char* p = NULL;
      p = tk_object_get_prop_str(obj, REMOTE_UI_KEY_DEV_NAME);
      if (p != NULL) {
        tk_strncpy(info->name, p, sizeof(info->name) - 1);
      }

      p = tk_object_get_prop_str(obj, REMOTE_UI_KEY_DEV_VERSION);
      if (p != NULL) {
        tk_strncpy(info->version, p, sizeof(info->version) - 1);
      }

      p = tk_object_get_prop_str(obj, REMOTE_UI_KEY_DEV_OS);
      if (p != NULL) {
        tk_strncpy(info->os, p, sizeof(info->os) - 1);
      }

      p = tk_object_get_prop_str(obj, REMOTE_UI_KEY_DEV_ARCH);
      if (p != NULL) {
        tk_strncpy(info->arch, p, sizeof(info->arch) - 1);
      }

      info->screen_width = tk_object_get_prop_int(obj, REMOTE_UI_KEY_DEV_SCREEN_WIDTH, 0);
      info->screen_height = tk_object_get_prop_int(obj, REMOTE_UI_KEY_DEV_SCREEN_HEIGHT, 0);
      info->dpi = tk_object_get_prop_int(obj, REMOTE_UI_KEY_DEV_DPI, 0);

      TK_OBJECT_UNREF(obj);
    }
  }

  return ret;
}

ret_t remote_ui_reboot(remote_ui_t* ui, remote_ui_reboot_type_t reboot_type) {
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->wb));
  wbuffer_write_int32(&(ui->wb), reboot_type);
  return remote_ui_request(ui->io, REMOTE_UI_REQ_REBOOT, REMOTE_UI_DATA_TYPE_NONE, &(ui->wb));
}

ret_t remote_ui_upload_file(remote_ui_t* ui, const char* remote_file, const char* local_file) {
  wbuffer_t wb;
  int32_t len = 0;
  ret_t ret = RET_OK;
  fs_file_t* file = NULL;
  uint8_t buff[4096] = {0};
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_file != NULL, RET_BAD_PARAMS);
  return_value_if_fail(local_file != NULL, RET_BAD_PARAMS);

  file = fs_open_file(os_fs(), local_file, "rb");
  return_value_if_fail(file != NULL, RET_BAD_PARAMS);

  wbuffer_init(&wb, (void*)remote_file, strlen(remote_file) + 1);
  wb.cursor = wb.capacity;
  ret = remote_ui_request(ui->io, REMOTE_UI_REQ_UPLOAD_FILE_BEGIN, REMOTE_UI_DATA_TYPE_STRING, &wb);
  goto_error_if_fail(ret == RET_OK);

  while ((len = fs_file_read(file, buff, sizeof(buff))) > 0) {
    wbuffer_init(&wb, buff, len);
    wb.cursor = len;
    ret =
        remote_ui_request(ui->io, REMOTE_UI_REQ_UPLOAD_FILE_DATA, REMOTE_UI_DATA_TYPE_BINARY, &wb);
    break_if_fail(ret == RET_OK);
  }

  wbuffer_rewind(&wb);
  ret = remote_ui_request(ui->io, REMOTE_UI_REQ_UPLOAD_FILE_END, REMOTE_UI_DATA_TYPE_NONE, &wb);

  fs_file_close(file);

  return ret;
error:
  fs_file_close(file);

  return RET_FAIL;
}

ret_t remote_ui_download_file(remote_ui_t* ui, const char* remote_file, const char* local_file) {
  int32_t len = 0;
  ret_t ret = RET_OK;
  fs_file_t* file = NULL;
  wbuffer_t* wb = NULL;
  remote_ui_msg_header_t header;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_file != NULL, RET_BAD_PARAMS);
  return_value_if_fail(local_file != NULL, RET_BAD_PARAMS);

  file = fs_open_file(os_fs(), local_file, "wb+");
  return_value_if_fail(file != NULL, RET_BAD_PARAMS);

  wb = &(ui->wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, remote_file);
  ret =
      remote_ui_request(ui->io, REMOTE_UI_REQ_DOWNLOAD_FILE_BEGIN, REMOTE_UI_DATA_TYPE_STRING, wb);
  goto_error_if_fail(ret == RET_OK);

  memset(&header, 0x00, sizeof(header));

  while ((ret = remote_ui_read_resp(ui->io, &header, wb)) == RET_OK) {
    if (header.type == REMOTE_UI_RESP_DOWNLOAD_FILE_DATA) {
      len = fs_file_write(file, wb->data, wb->cursor);
      break_if_fail(len == wb->cursor);
    } else if (header.type == REMOTE_UI_RESP_DOWNLOAD_FILE_END) {
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

ret_t remote_ui_create_dir(remote_ui_t* ui, const char* remote_dir) {
  ret_t ret = RET_OK;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_dir != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->wb));
  wbuffer_write_string(&(ui->wb), remote_dir);
  ret = remote_ui_request(ui->io, REMOTE_UI_REQ_CREATE_DIR, REMOTE_UI_DATA_TYPE_STRING, &(ui->wb));

  return ret;
}

ret_t remote_ui_remove_dir(remote_ui_t* ui, const char* remote_dir) {
  ret_t ret = RET_OK;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_dir != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->wb));
  wbuffer_write_string(&(ui->wb), remote_dir);
  ret = remote_ui_request(ui->io, REMOTE_UI_REQ_REMOVE_DIR, REMOTE_UI_DATA_TYPE_STRING, &(ui->wb));

  return ret;
}

ret_t remote_ui_remove_file(remote_ui_t* ui, const char* remote_file) {
  ret_t ret = RET_OK;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_file != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->wb));
  wbuffer_write_string(&(ui->wb), remote_file);
  ret = remote_ui_request(ui->io, REMOTE_UI_REQ_REMOVE_FILE, REMOTE_UI_DATA_TYPE_STRING, &(ui->wb));

  return ret;
}

ret_t remote_ui_take_screen_shot(remote_ui_t* ui, const char* file) {
  return remote_ui_download_file(ui, REMOTE_UI_FILE_SCREEN_SHOT, file);
}

ret_t remote_ui_get_manifest(remote_ui_t* ui, const char* file) {
  return remote_ui_download_file(ui, REMOTE_UI_FILE_MANIFEST, file);
}

ret_t remote_ui_get_xml_source(remote_ui_t* ui, const char* file) {
  return remote_ui_download_file(ui, REMOTE_UI_FILE_XML_SOURCE, file);
}

ret_t remote_ui_on_event(remote_ui_t* ui, const char* target, uint32_t event, event_func_t func,
                         void* ctx) {
  emitter_t* emitter = NULL;
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->event_handlers != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(func != NULL, RET_BAD_PARAMS);

  emitter = tk_object_get_prop_pointer(ui->event_handlers, target);
  if (emitter == NULL) {
    emitter = emitter_create();
    return_value_if_fail(emitter != NULL, RET_OOM);
    tk_object_set_prop_pointer_ex(ui->event_handlers, target, emitter,
                                  (tk_destroy_t)emitter_destroy);
  }
  emitter_on(emitter, event, func, ctx);

  writer = remote_ui_client_get_writer(ui);

  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_EVENT, event);
  ubjson_writer_write_object_end(writer);

  return remote_ui_request(ui->io, REMOTE_UI_REQ_ON_EVENT, REMOTE_UI_DATA_TYPE_UBJSON, &(ui->wb));
}

ret_t remote_ui_off_event(remote_ui_t* ui, const char* target, uint32_t event) {
  emitter_t* emitter = NULL;
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->event_handlers != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  emitter = tk_object_get_prop_pointer(ui->event_handlers, target);
  if (emitter != NULL) {
    emitter_off(emitter, event);
  }

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_EVENT, event);
  ubjson_writer_write_object_end(writer);

  return remote_ui_request(ui->io, REMOTE_UI_REQ_OFF_EVENT, REMOTE_UI_DATA_TYPE_UBJSON, &(ui->wb));
}

ret_t remote_ui_send_event(remote_ui_t* ui, const char* target, event_t* event) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(event != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_EVENT, event->type);
  switch (event->type) {
    case EVT_CLICK:
    case EVT_POINTER_DOWN:
    case EVT_POINTER_UP:
    case EVT_POINTER_MOVE: {
      pointer_event_t* e = (pointer_event_t*)event;
      ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_X, e->x);
      ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_Y, e->y);
      break;
    }
    case EVT_KEY_DOWN:
    case EVT_KEY_UP: {
      key_event_t* e = (key_event_t*)event;
      ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_CODE, e->key);
      break;
    }
    default: {
      log_debug("not supported event type:%d\n", event->type);
      return RET_NOT_IMPL;
    }
  }
  ubjson_writer_write_object_end(writer);

  return remote_ui_request(ui->io, REMOTE_UI_REQ_SEND_EVENT, REMOTE_UI_DATA_TYPE_UBJSON, &(ui->wb));
}

ret_t remote_ui_open_window(remote_ui_t* ui, const char* name, const char* xml,
                            const char* init_json) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_NAME, name);
  if (xml != NULL) {
    ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_XML, xml);
  }
  if (init_json != NULL) {
    ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_INIT, init_json);
  }
  ubjson_writer_write_object_end(writer);

  return remote_ui_request(ui->io, REMOTE_UI_REQ_OPEN_WINDOW, REMOTE_UI_DATA_TYPE_UBJSON,
                           &(ui->wb));
}

static ret_t remote_ui_show_dialog(remote_ui_t* ui, const char* type, const char* title,
                                   const char* content, uint32_t duration) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(type != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TYPE, type);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TITLE, title);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_CONTENT, content);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_DURATION, duration);
  ubjson_writer_write_object_end(writer);

  return remote_ui_request(ui->io, REMOTE_UI_REQ_OPEN_DIALOG, REMOTE_UI_DATA_TYPE_UBJSON,
                           &(ui->wb));
}

ret_t remote_ui_show_confirm(remote_ui_t* ui, const char* title, const char* content) {
    return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
    return_value_if_fail(title != NULL, RET_BAD_PARAMS);
    return_value_if_fail(content != NULL, RET_BAD_PARAMS);

    return remote_ui_show_dialog(ui, REMOTE_UI_DIALOG_TYPE_CONFIRM, title, content, 0);
}

ret_t remote_ui_show_warn(remote_ui_t* ui, const char* title, const char* content) {
    return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
    return_value_if_fail(title != NULL, RET_BAD_PARAMS);
    return_value_if_fail(content != NULL, RET_BAD_PARAMS);

    return remote_ui_show_dialog(ui, REMOTE_UI_DIALOG_TYPE_WARN, title, content, 0);    
}

ret_t remote_ui_show_info(remote_ui_t* ui, const char* title, const char* content) {
    return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
    return_value_if_fail(title != NULL, RET_BAD_PARAMS);
    return_value_if_fail(content != NULL, RET_BAD_PARAMS);

    return remote_ui_show_dialog(ui, NULL, title, content, 0);    
}

ret_t remote_ui_show_toast(remote_ui_t* ui, uint32_t duration, const char* content) {
    return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
    return_value_if_fail(content != NULL, RET_BAD_PARAMS);

    return remote_ui_show_dialog(ui, REMOTE_UI_DIALOG_TYPE_TOAST, "", content, duration);     
}

ret_t remote_ui_close_window(remote_ui_t* ui, const char* name) {
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->wb));
  wbuffer_write_string(&(ui->wb), name);
  return remote_ui_request(ui->io, REMOTE_UI_REQ_CLOSE_WINDOW, REMOTE_UI_DATA_TYPE_STRING,
                           &(ui->wb));
}

ret_t remote_ui_back_to_prev(remote_ui_t* ui) {
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->wb));
  return remote_ui_request(ui->io, REMOTE_UI_REQ_BACK_TO_PREV, REMOTE_UI_DATA_TYPE_NONE, &(ui->wb));
}

ret_t remote_ui_back_to_home(remote_ui_t* ui) {
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->wb));
  return remote_ui_request(ui->io, REMOTE_UI_REQ_BACK_TO_HOME, REMOTE_UI_DATA_TYPE_NONE, &(ui->wb));
}

ret_t remote_ui_set_prop(remote_ui_t* ui, const char* target, const char* name,
                         const value_t* value) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(value != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_NAME, name);
  ubjson_writer_write_kv_value(writer, REMOTE_UI_KEY_VALUE, value);
  ubjson_writer_write_object_end(writer);

  return remote_ui_request(ui->io, REMOTE_UI_REQ_SET_PROP, REMOTE_UI_DATA_TYPE_UBJSON, &(ui->wb));
}

ret_t remote_ui_get_prop(remote_ui_t* ui, const char* target, const char* name, value_t* value) {
  ret_t ret = RET_OK;
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(value != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_NAME, name);
  ubjson_writer_write_kv_value(writer, REMOTE_UI_KEY_VALUE, value);
  ubjson_writer_write_object_end(writer);

  ret = remote_ui_request(ui->io, REMOTE_UI_REQ_GET_PROP, REMOTE_UI_DATA_TYPE_UBJSON, &(ui->wb));
  return_value_if_fail(ret == RET_OK, ret);

  value_dup_str_with_len(value, (char*)(ui->wb.data), ui->wb.cursor);

  return RET_OK;
}

ret_t remote_ui_set_theme(remote_ui_t* ui, const char* theme) {
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(theme != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->wb));
  wbuffer_write_string(&(ui->wb), theme);
  return remote_ui_request(ui->io, REMOTE_UI_REQ_SET_THEME, REMOTE_UI_DATA_TYPE_STRING, &(ui->wb));
}

ret_t remote_ui_set_language(remote_ui_t* ui, const char* language) {
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(language != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->wb));
  wbuffer_write_string(&(ui->wb), language);
  return remote_ui_request(ui->io, REMOTE_UI_REQ_SET_LANGUAGE, REMOTE_UI_DATA_TYPE_STRING,
                           &(ui->wb));
}

ret_t remote_ui_exec_fscript(remote_ui_t* ui, const char* script, str_t* str) {
  ret_t ret = RET_OK;
  return_value_if_fail(ui != NULL && ui->io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(script != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->wb));
  wbuffer_write_string(&(ui->wb), script);
  ret =
      remote_ui_request(ui->io, REMOTE_UI_REQ_EXEC_FSCRIPT, REMOTE_UI_DATA_TYPE_STRING, &(ui->wb));
  if (ret == RET_OK) {
    str_set_with_len(str, (char*)(ui->wb.data), ui->wb.cursor);
  }

  return ret;
}

ret_t remote_ui_dispatch(remote_ui_t* ui) {
  /*TODO*/
  return RET_OK;
}

ret_t remote_ui_destroy(remote_ui_t* ui) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);

  TK_OBJECT_UNREF(ui->io);
  TK_OBJECT_UNREF(ui->event_handlers);
  wbuffer_deinit(&(ui->wb));
  TKMEM_FREE(ui);

  return RET_OK;
}
