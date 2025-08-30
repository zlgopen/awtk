/**
 * File:   remote_ui.c
 * Author: AWTK Develop Team
 * Brief:  remote ui client
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
#endif /*WITH_FULL_REMOTE_UI*/

#define REMOTE_UI_ISTREAM_TIMEOUT 100 * 1000

static ret_t my_rbuffer_destroy(rbuffer_t* rb) {
  return_value_if_fail(rb != NULL, RET_BAD_PARAMS);
  TKMEM_FREE(rb->data);
  TKMEM_FREE(rb);

  return RET_OK;
}

static rbuffer_t* my_rbuffer_create(wbuffer_t* wb) {
  rbuffer_t* rb = NULL;
  return_value_if_fail(wb != NULL, NULL);

  rb = (rbuffer_t*)TKMEM_ZALLOC(rbuffer_t);
  return_value_if_fail(rb != NULL, NULL);

  rb->capacity = wb->cursor;
  rb->data = TKMEM_ALLOC(rb->capacity);

  if (rb->data != NULL) {
    memcpy((void*)(rb->data), wb->data, wb->cursor);
  } else {
    my_rbuffer_destroy(rb);
    rb = NULL;
  }

  return rb;
}

static ret_t remote_ui_request_simple(remote_ui_t* ui, uint32_t code) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  return tk_client_request(&(ui->client), code, MSG_DATA_TYPE_NONE, wb);
}

static ret_t remote_ui_on_notify(remote_ui_t* ui, tk_msg_header_t* header, wbuffer_t* wb) {
  rbuffer_t* rb = NULL;
  return_value_if_fail(ui != NULL && header != NULL && wb != NULL, RET_BAD_PARAMS);
  return_value_if_fail(header->data_type == MSG_DATA_TYPE_BINARY, RET_BAD_PARAMS);

  rb = my_rbuffer_create(wb);
  return_value_if_fail(rb != NULL, RET_OOM);

  if (darray_push(&(ui->pending_events), rb) != RET_OK) {
    my_rbuffer_destroy(rb);
  }

  if (ui->on_notify != NULL) {
    ui->on_notify(ui, ui->on_notify_ctx);
  }

  return RET_OK;
}

remote_ui_t* remote_ui_create(tk_iostream_t* io) {
  remote_ui_t* ui = NULL;
  return_value_if_fail(io != NULL, NULL);

  ui = (remote_ui_t*)TKMEM_ZALLOC(remote_ui_t);
  return_value_if_fail(ui != NULL, NULL);

  ui->event_handlers = object_default_create_ex(FALSE);
  tk_client_init(&(ui->client), io, (tk_client_on_notify_t)remote_ui_on_notify);
  darray_init(&(ui->pending_events), 100, (tk_destroy_t)my_rbuffer_destroy, NULL);
  darray_init(&(ui->dispatching_events), 100, (tk_destroy_t)my_rbuffer_destroy, NULL);

  return ui;
}

ret_t remote_ui_login(remote_ui_t* ui, const char* username, const char* password) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(username != NULL, RET_BAD_PARAMS);
  return_value_if_fail(password != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, username);
  wbuffer_write_string(wb, password);

  return tk_client_request(&(ui->client), MSG_CODE_LOGIN, MSG_DATA_TYPE_BINARY, wb);
}

ret_t remote_ui_logout(remote_ui_t* ui) {
  return remote_ui_request_simple(ui, MSG_CODE_LOGOUT);
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
    rbuffer_t rb;
    if (rbuffer_init(&rb, wb->data, wb->cursor) != NULL) {
      const char* p = NULL;

      if (rbuffer_read_string(&rb, &p) == RET_OK) {
        tk_strncpy(info->name, p, sizeof(info->name) - 1);
      }

      if (rbuffer_read_string(&rb, &p) == RET_OK) {
        tk_strncpy(info->version, p, sizeof(info->version) - 1);
      }

      if (rbuffer_read_string(&rb, &p) == RET_OK) {
        tk_strncpy(info->os, p, sizeof(info->os) - 1);
      }

      if (rbuffer_read_string(&rb, &p) == RET_OK) {
        tk_strncpy(info->arch, p, sizeof(info->arch) - 1);
      }

      rbuffer_read_uint32(&rb, &info->screen_width);
      rbuffer_read_uint32(&rb, &info->screen_height);
      rbuffer_read_uint32(&rb, &info->dpi);
    }
  }

  return ret;
}

ret_t remote_ui_reboot(remote_ui_t* ui, remote_ui_reboot_type_t reboot_type) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_int32(wb, reboot_type);
  return tk_client_request(&(ui->client), REMOTE_UI_REBOOT, MSG_DATA_TYPE_BINARY, wb);
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
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_dir != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, remote_dir);
  ret = tk_client_request(&(ui->client), REMOTE_UI_CREATE_DIR, MSG_DATA_TYPE_STRING, wb);

  return ret;
}

ret_t remote_ui_remove_dir(remote_ui_t* ui, const char* remote_dir) {
  ret_t ret = RET_OK;
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_dir != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, remote_dir);
  ret = tk_client_request(&(ui->client), REMOTE_UI_REMOVE_DIR, MSG_DATA_TYPE_STRING, wb);

  return ret;
}

ret_t remote_ui_remove_file(remote_ui_t* ui, const char* remote_file) {
  ret_t ret = RET_OK;
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(remote_file != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, remote_file);
  ret = tk_client_request(&(ui->client), REMOTE_UI_REMOVE_FILE, MSG_DATA_TYPE_STRING, wb);

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
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->event_handlers != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(func != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, target);
  wbuffer_write_int32(wb, event);

  ret = tk_client_request(&(ui->client), REMOTE_UI_ON_EVENT, MSG_DATA_TYPE_BINARY, wb);
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

ret_t remote_ui_off_event(remote_ui_t* ui, const char* target, uint32_t event, event_func_t func,
                          void* ctx) {
  ret_t ret = RET_FAIL;
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->event_handlers != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, target);
  wbuffer_write_int32(wb, event);

  ret = tk_client_request(&(ui->client), REMOTE_UI_OFF_EVENT, MSG_DATA_TYPE_BINARY, wb);
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
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(event != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, target);
  wbuffer_write_int32(wb, event->type);

  switch (event->type) {
    case EVT_CLICK:
    case EVT_POINTER_DOWN:
    case EVT_POINTER_UP:
    case EVT_POINTER_MOVE: {
      pointer_event_t* e = (pointer_event_t*)event;
      wbuffer_write_int32(wb, e->x);
      wbuffer_write_int32(wb, e->y);
      break;
    }
    case EVT_KEY_DOWN:
    case EVT_KEY_UP: {
      key_event_t* e = (key_event_t*)event;
      wbuffer_write_uint32(wb, e->key);
      break;
    }
    default: {
      log_debug("not supported event type:%d\n", event->type);
      return RET_NOT_IMPL;
    }
  }

  return tk_client_request(&(ui->client), REMOTE_UI_SEND_EVENT, MSG_DATA_TYPE_BINARY, wb);
}

ret_t remote_ui_open_window(remote_ui_t* ui, const char* name, const char* xml,
                            const char* init_json) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, name);

  if (xml != NULL) {
    wbuffer_write_string(wb, xml);
  } else {
    wbuffer_write_string(wb, "");
  }
  if (init_json != NULL) {
    wbuffer_write_string(wb, init_json);
  } else {
    wbuffer_write_string(wb, "");
  }

  return tk_client_request(&(ui->client), REMOTE_UI_OPEN_WINDOW, MSG_DATA_TYPE_BINARY, wb);
}

#endif /*WITH_FULL_REMOTE_UI*/

static ret_t remote_ui_show_dialog(remote_ui_t* ui, const char* type, const char* title,
                                   const char* content, uint32_t duration) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(type != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, type);
  wbuffer_write_string(wb, title);
  wbuffer_write_string(wb, content);
  wbuffer_write_uint32(wb, duration);

  return tk_client_request(&(ui->client), REMOTE_UI_OPEN_DIALOG, MSG_DATA_TYPE_BINARY, wb);
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
  return remote_ui_request_simple(ui, REMOTE_UI_BACK_TO_PREV);
}

ret_t remote_ui_back_to_home(remote_ui_t* ui) {
  return remote_ui_request_simple(ui, REMOTE_UI_BACK_TO_HOME);
}

ret_t remote_ui_set_prop(remote_ui_t* ui, const char* target, const char* name,
                         const value_t* value) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(value != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, target);
  wbuffer_write_string(wb, name);
  wbuffer_write_value(wb, value);

  return tk_client_request(&(ui->client), REMOTE_UI_SET_PROP, MSG_DATA_TYPE_BINARY, wb);
}

ret_t remote_ui_get_prop(remote_ui_t* ui, const char* target, const char* name, value_t* value) {
  ret_t ret = RET_OK;
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(value != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, target);
  wbuffer_write_string(wb, name);

  ret = tk_client_request(&(ui->client), REMOTE_UI_GET_PROP, MSG_DATA_TYPE_BINARY, wb);
  if (ret == RET_OK) {
    value_t v;
    rbuffer_t rb;
    value_set_int(&v, 0);
    rbuffer_init(&rb, wb->data, wb->cursor);
    rbuffer_read_value(&rb, &v);
    value_deep_copy(value, &v);
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
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, target);
  wbuffer_write_int32(wb, x);
  wbuffer_write_int32(wb, y);

  return tk_client_request(&(ui->client), REMOTE_UI_MOVE_WIDGET, MSG_DATA_TYPE_BINARY, wb);
}

ret_t remote_ui_resize_widget(remote_ui_t* ui, const char* target, uint32_t w, uint32_t h) {
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, target);
  wbuffer_write_uint32(wb, w);
  wbuffer_write_uint32(wb, h);

  return tk_client_request(&(ui->client), REMOTE_UI_RESIZE_WIDGET, MSG_DATA_TYPE_BINARY, wb);
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
  wbuffer_t* wb = NULL;
  return_value_if_fail(ui != NULL && ui->client.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(target != NULL, RET_BAD_PARAMS);
  return_value_if_fail(xml != NULL, RET_BAD_PARAMS);

  wb = &(ui->client.wb);
  wbuffer_rewind(wb);
  wbuffer_write_string(wb, target);
  wbuffer_write_string(wb, xml);

  return tk_client_request(&(ui->client), REMOTE_UI_CREATE_WIDGET, MSG_DATA_TYPE_BINARY, wb);
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
#endif /*WITH_FULL_REMOTE_UI*/

ret_t remote_ui_dispatch_one(remote_ui_t* ui, rbuffer_t* rb) {
  ret_t ret = RET_FAIL;
  uint32_t type = 0;
  const char* target = NULL;
  return_value_if_fail(ui != NULL && rb != NULL, RET_BAD_PARAMS);

  rbuffer_read_string(rb, &target);
  rbuffer_read_uint32(rb, &type);

  if (ui->fallback_on_event != NULL) {
    ret = ui->fallback_on_event(ui, rb, target, type);
  }

  if (ret != RET_OK && target != NULL && type != 0) {
    emitter_t* emitter = tk_object_get_prop_pointer(ui->event_handlers, target);
    if (emitter != NULL || type == EVT_LOG_MESSAGE) {
      switch (type) {
#ifdef WITH_FULL_REMOTE_UI
        case EVT_KEY_DOWN:
        case EVT_KEY_UP: {
          key_event_t e;
          int key = 0;
          rbuffer_read_int32(rb, &key);
          log_debug("key_event:: type:%d key:%d\n", type, key);
          emitter_dispatch(emitter, key_event_init(&e, type, emitter, key));
          break;
        }
        case EVT_POINTER_DOWN:
        case EVT_POINTER_UP:
        case EVT_POINTER_MOVE:
        case EVT_CLICK: {
          pointer_event_t e;
          int x = 0;
          int y = 0;
          rbuffer_read_int32(rb, &x);
          rbuffer_read_int32(rb, &y);
          log_debug("pointer_event:: type:%d x:%d y:%d\n", type, x, y);
          emitter_dispatch(emitter, pointer_event_init(&e, type, emitter, x, y));
          break;
        }
#endif /*WITH_FULL_REMOTE_UI*/
        case EVT_VALUE_CHANGED: {
          value_change_event_t e;
          value_change_event_init(&e, type, emitter);
          rbuffer_read_value(rb, &(e.new_value));
          emitter_dispatch(emitter, (event_t*)&e);
          break;
        }
        case EVT_PROP_CHANGED: {
          value_t value;
          prop_change_event_t e;
          const char* name = NULL;
          rbuffer_read_string(rb, &name);
          value_set_int(&value, 0);
          rbuffer_read_value(rb, &value);
          prop_change_event_init(&e, type, name, &value);
          emitter_dispatch(emitter, (event_t*)&e);
          break;
        }
        case EVT_LOG_MESSAGE: {
          const char* message = NULL;
          int8_t level = LOG_LEVEL_DEBUG;
          rbuffer_read_int8(rb, &level);
          rbuffer_read_string(rb, &message);

          if (ui->log_hook != NULL) {
            ui->log_hook(ui->log_hook_ctx, level, message);
          }
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

ret_t remote_ui_hook_log(remote_ui_t* ui, remote_ui_on_log_message_t log, void* ctx) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);
  ui->log_hook = log;
  ui->log_hook_ctx = ctx;

  return remote_ui_request_simple(ui, REMOTE_UI_HOOK_LOG);
}

ret_t remote_ui_unhook_log(remote_ui_t* ui) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);
  ui->log_hook = NULL;
  ui->log_hook_ctx = NULL;

  return remote_ui_request_simple(ui, REMOTE_UI_UNHOOK_LOG);
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
      rbuffer_t* iter = (rbuffer_t*)darray_get(dest, i);
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

emitter_t* remote_ui_get_event_hander(remote_ui_t* ui, const char* target) {
  return_value_if_fail(ui != NULL && ui->event_handlers != NULL && target != NULL, NULL);

  return tk_object_get_prop_pointer(ui->event_handlers, target);
}

ret_t remote_ui_set_fallback_on_event(remote_ui_t* ui,
                                      remote_ui_on_event_func_t fallback_on_event) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);

  ui->fallback_on_event = fallback_on_event;

  return RET_OK;
}

ret_t remote_ui_set_on_notify(remote_ui_t* ui, remote_ui_on_notify_t on_notify, void* ctx) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);

  ui->on_notify = on_notify;
  ui->on_notify_ctx = ctx;

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
