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
#include "tkc/path.h"
#include "tkc/mem.h"
#include "tkc/crc.h"
#include "tkc/utils.h"
#include "conf_io/conf_ubjson.h"
#include "tkc/object_default.h"

#include "remote_ui/client/remote_ui.h"
#include "remote_ui/shared/remote_ui_types_def.h"

#ifdef WITH_FULL_REMOTE_UI
#include "base/keys.h"
#include "base/events.h"
#endif/*WITH_FULL_REMOTE_UI*/

#define REMOTE_UI_ISTREAM_TIMEOUT 100 * 1000

static ret_t remote_ui_on_notify(remote_ui_t* ui, tk_msg_header_t* header, wbuffer_t* wb) {
  conf_doc_t* doc = NULL;
  return_value_if_fail(ui != NULL && header != NULL && wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(header->data_type == MSG_DATA_TYPE_UBJSON, RET_BAD_PARAMS);

  doc = conf_doc_load_ubjson(wb->data, wb->cursor);
  return_value_if_fail(doc != NULL, RET_BAD_PARAMS);

  darray_push(&(ui->pending_events), doc);

  return RET_OK;
}

remote_ui_t* remote_ui_create(tk_iostream_t* io) {
  remote_ui_t* ui = NULL;
  return_value_if_fail(io != NULL, NULL);

  ui = (remote_ui_t*)TKMEM_ZALLOC(remote_ui_t);
  return_value_if_fail(ui != NULL, NULL);

  ui->event_handlers = object_default_create_ex(FALSE);
  tk_client_init(&(ui->client), io, (tk_client_on_notify_t)remote_ui_on_notify);
  darray_init(&(ui->pending_events), 100, (tk_destroy_t)tk_object_unref, NULL);
  darray_init(&(ui->dispatching_events), 100, (tk_destroy_t)conf_doc_destroy, NULL);

  return ui;
}

static ubjson_writer_t* remote_ui_client_get_writer(remote_ui_t* ui) {
  wbuffer_t* wb = &(ui->client.wb);
  ubjson_writer_t* writer = &(ui->writer);

  wb->cursor = 0;
  return ubjson_writer_init(writer, (ubjson_write_callback_t)wbuffer_write_binary, wb);
}

ret_t remote_ui_login(remote_ui_t* ui, const char* username, const char* password) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(username != NULL, RET_BAD_PARAMS);
  return_value_if_fail(password != NULL, RET_BAD_PARAMS);
  writer = remote_ui_client_get_writer(ui);

  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_USERNAME, username);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_PASSWORD, password);
  ubjson_writer_write_object_end(writer);

  return tk_client_request(&(ui->client), MSG_CODE_LOGIN, MSG_DATA_TYPE_UBJSON, &(ui->client.wb));
}

ret_t remote_ui_logout(remote_ui_t* ui) {
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->client.wb));
  return tk_client_request(&(ui->client), MSG_CODE_LOGOUT, MSG_DATA_TYPE_NONE, &(ui->client.wb));
}

ret_t remote_ui_get_dev_info(remote_ui_t* ui, remote_ui_dev_info_t* info) {
  ret_t ret = RET_FAIL;
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  memset(info, 0x00, sizeof(*info));
  ret = tk_client_request(&(ui->client), REMOTE_UI_GET_DEV_INFO, MSG_DATA_TYPE_NONE, wb);
  if (ret == RET_OK) {
    conf_doc_t* doc = conf_doc_load_ubjson(wb->data, wb->cursor);
    if (doc != NULL) {
      const char* p = NULL;
      p = conf_doc_get_str(doc, REMOTE_UI_KEY_DEV_NAME, NULL);
      if (p != NULL) {
        tk_strncpy(info->name, p, sizeof(info->name) - 1);
      }

      p = conf_doc_get_str(doc, REMOTE_UI_KEY_DEV_VERSION, NULL);
      if (p != NULL) {
        tk_strncpy(info->version, p, sizeof(info->version) - 1);
      }

      p = conf_doc_get_str(doc, REMOTE_UI_KEY_DEV_OS, NULL);
      if (p != NULL) {
        tk_strncpy(info->os, p, sizeof(info->os) - 1);
      }

      p = conf_doc_get_str(doc, REMOTE_UI_KEY_DEV_ARCH, NULL);
      if (p != NULL) {
        tk_strncpy(info->arch, p, sizeof(info->arch) - 1);
      }

      info->screen_width = conf_doc_get_int(doc, REMOTE_UI_KEY_DEV_SCREEN_WIDTH, 0);
      info->screen_height = conf_doc_get_int(doc, REMOTE_UI_KEY_DEV_SCREEN_HEIGHT, 0);
      info->dpi = conf_doc_get_int(doc, REMOTE_UI_KEY_DEV_DPI, 0);

      conf_doc_destroy(doc);
    }
  }

  return ret;
}

ret_t remote_ui_reboot(remote_ui_t* ui, remote_ui_reboot_type_t reboot_type) {
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->client.wb));
  wbuffer_write_int32(&(ui->client.wb), reboot_type);
  return tk_client_request(&(ui->client), REMOTE_UI_REBOOT, MSG_DATA_TYPE_NONE, &(ui->client.wb));
}

ret_t remote_ui_download_file(remote_ui_t* ui, const char* remote_file, const char* local_file) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);

  return tk_client_download_file(&(ui->client), remote_file, local_file);
}

ret_t remote_ui_upload_file(remote_ui_t* ui, const char* remote_file, const char* local_file) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);

  return tk_client_upload_file(&(ui->client), remote_file, local_file);
}

ret_t remote_ui_create_dir(remote_ui_t* ui, const char* remote_dir) {
  ret_t ret = RET_OK;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_dir != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->client.wb));
  wbuffer_write_string(&(ui->client.wb), remote_dir);
  ret = tk_client_request(&(ui->client), REMOTE_UI_CREATE_DIR, MSG_DATA_TYPE_STRING,
                          &(ui->client.wb));

  return ret;
}

ret_t remote_ui_remove_dir(remote_ui_t* ui, const char* remote_dir) {
  ret_t ret = RET_OK;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_dir != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->client.wb));
  wbuffer_write_string(&(ui->client.wb), remote_dir);
  ret = tk_client_request(&(ui->client), REMOTE_UI_REMOVE_DIR, MSG_DATA_TYPE_STRING,
                          &(ui->client.wb));

  return ret;
}

ret_t remote_ui_remove_file(remote_ui_t* ui, const char* remote_file) {
  ret_t ret = RET_OK;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_file != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->client.wb));
  wbuffer_write_string(&(ui->client.wb), remote_file);
  ret = tk_client_request(&(ui->client), REMOTE_UI_REMOVE_FILE, MSG_DATA_TYPE_STRING,
                          &(ui->client.wb));

  return ret;
}

ret_t remote_ui_take_snapshot(remote_ui_t* ui, const char* target, const char* file) {
  char remote_file[MAX_PATH + 1] = {0};
  path_build(remote_file, sizeof(remote_file) - 1, REMOTE_UI_FILE_SNAPSHOT, target, NULL);

  return remote_ui_download_file(ui, remote_file, file);
}

ret_t remote_ui_get_manifest(remote_ui_t* ui, const char* file) {
  return remote_ui_download_file(ui, REMOTE_UI_FILE_MANIFEST, file);
}

ret_t remote_ui_get_xml_source(remote_ui_t* ui, const char* target, const char* file) {
  char remote_file[MAX_PATH + 1] = {0};
  path_build(remote_file, sizeof(remote_file) - 1, REMOTE_UI_FILE_XML_SOURCE, target, NULL);

  return remote_ui_download_file(ui, remote_file, file);
}

static ret_t remote_ui_on_event_local(remote_ui_t* ui, const char* target, uint32_t event,
                                      event_func_t func, void* ctx) {
  emitter_t* emitter = NULL;
  emitter = tk_object_get_prop_pointer(ui->event_handlers, target);
  if (emitter == NULL) {
    emitter = emitter_create();
    return_value_if_fail(emitter != NULL, RET_OOM);
    tk_object_set_prop_pointer_ex(ui->event_handlers, target, emitter,
                                  (tk_destroy_t)emitter_destroy);
  }
  emitter_on(emitter, event, func, ctx);
	
  return RET_OK;
}

ret_t remote_ui_on_event(remote_ui_t* ui, const char* target, uint32_t event, event_func_t func,
                         void* ctx) {
  ret_t ret = RET_FAIL;
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->event_handlers != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(func != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);

  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_EVENT, event);
  ubjson_writer_write_object_end(writer);

  ret =
      tk_client_request(&(ui->client), REMOTE_UI_ON_EVENT, MSG_DATA_TYPE_UBJSON, &(ui->client.wb));
  if (ret == RET_OK) {
    target = (char*)(ui->client.wb.data);
    remote_ui_on_event_local(ui, target, event, func, ctx);
  }

  return ret;
}

static ret_t remote_ui_off_event_local(remote_ui_t* ui, const char* target, uint32_t event,
                                       event_func_t func, void* ctx) {
  emitter_t* emitter = NULL;
  return_value_if_fail(ui != NULL && ui->event_handlers != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  emitter = tk_object_get_prop_pointer(ui->event_handlers, target);
  if (emitter != NULL) {
    emitter_off_by_func(emitter, event, func, ctx);

    if (emitter->items == NULL) {
      tk_object_remove_prop(ui->event_handlers, target);
    }
  }

  return RET_OK;
}

ret_t remote_ui_off_event(remote_ui_t* ui, const char* target, uint32_t event,
                          event_func_t func, void* ctx) {
  ret_t ret = RET_FAIL;
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->event_handlers != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_EVENT, event);
  ubjson_writer_write_object_end(writer);

  ret =
      tk_client_request(&(ui->client), REMOTE_UI_OFF_EVENT, MSG_DATA_TYPE_UBJSON, &(ui->client.wb));
  if (ret == RET_OK) {
    target = (char*)(ui->client.wb.data);
    remote_ui_off_event_local(ui, target, event, func, ctx);
  }

  return ret;
}

#ifdef WITH_FULL_REMOTE_UI	
ret_t remote_ui_click(remote_ui_t* ui, const char* target) {
  pointer_event_t e;
  pointer_event_init(&e, EVT_CLICK, NULL, 0, 0);

  return remote_ui_send_event(ui, target, (event_t*)&e);
}

ret_t remote_ui_key(remote_ui_t* ui, const char* target, int32_t key_code) {
  key_event_t e;
  key_event_init(&e, EVT_KEY_DOWN, NULL, key_code);
  if (remote_ui_send_event(ui, target, (event_t*)&e) == RET_OK) {
    key_event_init(&e, EVT_KEY_UP, NULL, key_code);
    return remote_ui_send_event(ui, target, (event_t*)&e);
  } else {
    return RET_FAIL;
  }
}

ret_t remote_ui_send_event(remote_ui_t* ui, const char* target, event_t* event) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
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

  return tk_client_request(&(ui->client), REMOTE_UI_SEND_EVENT, MSG_DATA_TYPE_UBJSON,
                           &(ui->client.wb));
}

ret_t remote_ui_open_window(remote_ui_t* ui, const char* name, const char* xml,
                            const char* init_json) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
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

  return tk_client_request(&(ui->client), REMOTE_UI_OPEN_WINDOW, MSG_DATA_TYPE_UBJSON,
                           &(ui->client.wb));
}

#endif/*WITH_FULL_REMOTE_UI*/
static ret_t remote_ui_show_dialog(remote_ui_t* ui, const char* type, const char* title,
                                   const char* content, uint32_t duration) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(type != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TYPE, type);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TITLE, title);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_CONTENT, content);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_DURATION, duration);
  ubjson_writer_write_object_end(writer);

  return tk_client_request(&(ui->client), REMOTE_UI_OPEN_DIALOG, MSG_DATA_TYPE_UBJSON,
                           &(ui->client.wb));
}

ret_t remote_ui_show_confirm(remote_ui_t* ui, const char* title, const char* content) {
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(title != NULL, RET_BAD_PARAMS);
  return_value_if_fail(content != NULL, RET_BAD_PARAMS);

  return remote_ui_show_dialog(ui, REMOTE_UI_DIALOG_TYPE_CONFIRM, title, content, 0);
}

ret_t remote_ui_show_warn(remote_ui_t* ui, const char* title, const char* content) {
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(title != NULL, RET_BAD_PARAMS);
  return_value_if_fail(content != NULL, RET_BAD_PARAMS);

  return remote_ui_show_dialog(ui, REMOTE_UI_DIALOG_TYPE_WARN, title, content, 0);
}

ret_t remote_ui_show_info(remote_ui_t* ui, const char* title, const char* content) {
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(title != NULL, RET_BAD_PARAMS);
  return_value_if_fail(content != NULL, RET_BAD_PARAMS);

  return remote_ui_show_dialog(ui, NULL, title, content, 0);
}

ret_t remote_ui_show_toast(remote_ui_t* ui, uint32_t duration, const char* content) {
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(content != NULL, RET_BAD_PARAMS);

  return remote_ui_show_dialog(ui, REMOTE_UI_DIALOG_TYPE_TOAST, "", content, duration);
}

ret_t remote_ui_close_window(remote_ui_t* ui, const char* name) {
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->client.wb));
  wbuffer_write_string(&(ui->client.wb), name);
  return tk_client_request(&(ui->client), REMOTE_UI_CLOSE_WINDOW, MSG_DATA_TYPE_STRING,
                           &(ui->client.wb));
}

ret_t remote_ui_back_to_prev(remote_ui_t* ui) {
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->client.wb));
  return tk_client_request(&(ui->client), REMOTE_UI_BACK_TO_PREV, MSG_DATA_TYPE_NONE,
                           &(ui->client.wb));
}

ret_t remote_ui_back_to_home(remote_ui_t* ui) {
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->client.wb));
  return tk_client_request(&(ui->client), REMOTE_UI_BACK_TO_HOME, MSG_DATA_TYPE_NONE,
                           &(ui->client.wb));
}

ret_t remote_ui_set_prop(remote_ui_t* ui, const char* target, const char* name,
                         const value_t* value) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(value != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_NAME, name);
  ubjson_writer_write_kv_value(writer, REMOTE_UI_KEY_VALUE, value);
  ubjson_writer_write_object_end(writer);

  return tk_client_request(&(ui->client), REMOTE_UI_SET_PROP, MSG_DATA_TYPE_UBJSON,
                           &(ui->client.wb));
}

ret_t remote_ui_get_prop(remote_ui_t* ui, const char* target, const char* name, value_t* value) {
  ret_t ret = RET_OK;
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(value != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_NAME, name);
  ubjson_writer_write_object_end(writer);

  ret =
      tk_client_request(&(ui->client), REMOTE_UI_GET_PROP, MSG_DATA_TYPE_UBJSON, &(ui->client.wb));
  if (ret == RET_OK) {
    conf_doc_t* doc = conf_doc_load_ubjson(ui->client.wb.data, ui->client.wb.cursor);
    if (doc != NULL) {
      value_t v;
      value_set_int(&v, 0);
      ret = conf_doc_get(doc, REMOTE_UI_KEY_VALUE, &v);
      if (ret == RET_OK) {
        value_deep_copy(value, &v);
      }
      conf_doc_destroy(doc);
    } else {
      ret = RET_OOM;
    }
  }

  return ret;
}

ret_t remote_ui_set_theme(remote_ui_t* ui, const char* theme) {
  value_t v;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(theme != NULL, RET_BAD_PARAMS);

  return remote_ui_set_prop(ui, REMOTE_UI_TARGET_GLOBAL, REMOTE_UI_PROP_THEME,
                            value_set_str(&v, theme));
}

ret_t remote_ui_set_language(remote_ui_t* ui, const char* language) {
  value_t v;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(language != NULL, RET_BAD_PARAMS);

  return remote_ui_set_prop(ui, REMOTE_UI_TARGET_GLOBAL, REMOTE_UI_PROP_LANGUAGE,
                            value_set_str(&v, language));
}

ret_t remote_ui_exec_fscript(remote_ui_t* ui, const char* script, str_t* str) {
  ret_t ret = RET_OK;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(script != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->client.wb));
  wbuffer_write_string(&(ui->client.wb), script);
  ret = tk_client_request(&(ui->client), REMOTE_UI_EXEC_FSCRIPT, MSG_DATA_TYPE_STRING,
                          &(ui->client.wb));
  if (ret == RET_OK) {
    str_set_with_len(str, (char*)(ui->client.wb.data), ui->client.wb.cursor);
  }

  return ret;
}

ret_t remote_ui_move_widget(remote_ui_t* ui, const char* target, int32_t x, int32_t y) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_X, x);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_Y, y);
  ubjson_writer_write_object_end(writer);

  return tk_client_request(&(ui->client), REMOTE_UI_MOVE_WIDGET, MSG_DATA_TYPE_UBJSON,
                           &(ui->client.wb));
}

ret_t remote_ui_resize_widget(remote_ui_t* ui, const char* target, uint32_t w, uint32_t h) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_W, w);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_H, h);
  ubjson_writer_write_object_end(writer);

  return tk_client_request(&(ui->client), REMOTE_UI_RESIZE_WIDGET, MSG_DATA_TYPE_UBJSON,
                           &(ui->client.wb));
}

ret_t remote_ui_destroy_widget(remote_ui_t* ui, const char* target) {
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  wbuffer_rewind(&(ui->client.wb));
  wbuffer_write_string(&(ui->client.wb), target);
  return tk_client_request(&(ui->client), REMOTE_UI_DESTROY_WIDGET, MSG_DATA_TYPE_STRING,
                           &(ui->client.wb));
}

ret_t remote_ui_create_widget(remote_ui_t* ui, const char* target, const char* xml) {
  ubjson_writer_t* writer = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(xml != NULL, RET_BAD_PARAMS);

  writer = remote_ui_client_get_writer(ui);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_XML, xml);
  ubjson_writer_write_object_end(writer);

  return tk_client_request(&(ui->client), REMOTE_UI_CREATE_WIDGET, MSG_DATA_TYPE_UBJSON,
                           &(ui->client.wb));
}

ret_t remote_ui_get_loaded_images_info(remote_ui_t* ui, const char* file) {
  return remote_ui_download_file(ui, REMOTE_UI_FILE_LOADED_IMAGES_INFO, file);
}

ret_t remote_ui_get_loaded_assets_info(remote_ui_t* ui, const char* file) {
  return remote_ui_download_file(ui, REMOTE_UI_FILE_LOADED_ASSETS_INFO, file);
}

#ifdef WITH_FULL_REMOTE_UI
static ret_t value_from_str(value_t* v, int32_t value_type, const char* str) {
  switch (value_type) {
    case VALUE_TYPE_STRING: {
      value_set_str(v, str);
      break;
    }
    case VALUE_TYPE_INT8: {
      value_set_int8(v, tk_atoi(str));
      break;
    }
    case VALUE_TYPE_INT16: {
      value_set_int16(v, tk_atoi(str));
      break;
    }
    case VALUE_TYPE_INT32: {
      value_set_int32(v, tk_atoi(str));
      break;
    }
    case VALUE_TYPE_INT64: {
      value_set_int64(v, tk_atol(str));
      break;
    }
    case VALUE_TYPE_UINT8: {
      value_set_uint8(v, tk_atoi(str));
      break;
    }
    case VALUE_TYPE_UINT16: {
      value_set_uint16(v, tk_atoi(str));
      break;
    }
    case VALUE_TYPE_UINT32: {
      value_set_uint32(v, tk_atoi(str));
      break;
    }
    case VALUE_TYPE_UINT64: {
      value_set_uint64(v, tk_atol(str));
      break;
    }
    case VALUE_TYPE_FLOAT: {
      value_set_float(v, tk_atof(str));
      break;
    }
    case VALUE_TYPE_DOUBLE: {
      value_set_double(v, tk_atof(str));
      break;
    }
    case VALUE_TYPE_WSTRING: {
      value_set_wstr(v, tk_wstr_dup_utf8(str));
      break;
    }
    default: {
      value_set_int(v, 0);
      log_debug("not supported value type:%d\n", value_type);
      break;
    }
  }
  return RET_OK;
}
#endif/*WITH_FULL_REMOTE_UI*/

ret_t remote_ui_dispatch_one(remote_ui_t* ui, conf_doc_t* doc) {
  const char* target = NULL;
  uint32_t type = 0;
  return_value_if_fail(ui != NULL && doc != NULL, RET_BAD_PARAMS);

  target = conf_doc_get_str(doc, REMOTE_UI_KEY_TARGET, NULL);
  type = conf_doc_get_int(doc, REMOTE_UI_KEY_EVENT, 0);
  if (target != NULL && type != 0) {
    emitter_t* emitter = tk_object_get_prop_pointer(ui->event_handlers, target);
    if (emitter != NULL) {
      switch (type) {
#ifdef WITH_FULL_REMOTE_UI				
        case EVT_KEY_DOWN:
        case EVT_KEY_UP: {
          key_event_t e;
          int key = conf_doc_get_int(doc, REMOTE_UI_KEY_CODE, 0);
          log_debug("key_event:: type:%d key:%d\n", type, key);
          emitter_dispatch(emitter, key_event_init(&e, type, emitter, key));
          break;
        }
        case EVT_POINTER_DOWN:
        case EVT_POINTER_UP:
        case EVT_POINTER_MOVE:
        case EVT_CLICK: {
          pointer_event_t e;
          int x = conf_doc_get_int(doc, REMOTE_UI_KEY_X, 0);
          int y = conf_doc_get_int(doc, REMOTE_UI_KEY_Y, 0);
          log_debug("pointer_event:: type:%d x:%d y:%d\n", type, x, y);
          emitter_dispatch(emitter, pointer_event_init(&e, type, emitter, x, y));
          break;
        }
        case EVT_VALUE_CHANGED: {
          value_change_event_t e;
          value_change_event_init(&e, type, emitter);
          conf_doc_get(doc, REMOTE_UI_KEY_VALUE, &(e.new_value));
          emitter_dispatch(emitter, (event_t*)&e);
          value_reset(&(e.new_value));
          break;
        }
#endif/*WITH_FULL_REMOTE_UI*/								
        case EVT_PROP_CHANGED: {
          value_t value;
          prop_change_event_t e;
          const char* name = conf_doc_get_str(doc, REMOTE_UI_KEY_NAME, NULL);
          value_set_int(&value, 0);
          conf_doc_get(doc, REMOTE_UI_KEY_VALUE, &value);
          prop_change_event_init(&e, type, name, &value);
          emitter_dispatch(emitter, (event_t*)&e);
          value_reset(&(value));
          break;
        }
        default: {
          log_debug("not supported event type:%d\n", type);
          break;
        }
      }
    }
  }

  return RET_OK;
}

ret_t remote_ui_dispatch(remote_ui_t* ui) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);
  if (ui->pending_events.size > 0) {
    uint32_t i = 0;
    uint32_t n = ui->pending_events.size;

    darray_t* src = &(ui->pending_events);
    darray_t* dest = &(ui->dispatching_events);
    darray_clear(dest);

    /*防止在分发事件时触发新事件导致崩溃，拷贝一份再分发*/
    for (i = 0; i < n; i++) {
      void* iter = darray_get(src, i);
      darray_push(dest, iter);
    }
    src->size = 0;

    for (i = 0; i < n; i++) {
      conf_doc_t* iter = (conf_doc_t*)darray_get(dest, i);
      remote_ui_dispatch_one(ui, iter);
    }
    darray_clear(dest);
  }

  return RET_OK;
}

ret_t remote_ui_off_all_events(remote_ui_t* ui) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

ret_t remote_ui_destroy(remote_ui_t* ui) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);

  remote_ui_off_all_events(ui);
  TK_OBJECT_UNREF(ui->event_handlers);
  darray_deinit(&(ui->pending_events));
  darray_deinit(&(ui->dispatching_events));
  tk_client_deinit(&(ui->client));
  TKMEM_FREE(ui);

  return RET_OK;
}
