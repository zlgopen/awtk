/**
 * File:   remote_ui_service.c
 * Author: AWTK Develop Team
 * Brief:  remote ui service
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
#include "tkc/path.h"
#include "tkc/utils.h"
#include "tkc/fscript.h"
#include "base/bitmap.h"
#include "base/window.h"
#include "base/window_manager.h"
#include "base/ui_loader.h"
#include "base/dialog.h"
#include "conf_io/conf_ubjson.h"
#include "tkc/object_default.h"
#include "ui_loader/ui_serializer.h"
#include "remote_ui/shared/remote_ui_types_def.h"
#include "remote_ui/service/remote_ui_service.h"

#include "base/events.h"
#include "base/keys.h"

static ret_t remote_ui_service_dispatch(remote_ui_service_t* ui);
static ret_t remote_ui_service_destroy(remote_ui_service_t* ui);

tk_service_t* remote_ui_service_create(tk_iostream_t* io, void* args) {
  remote_ui_service_t* ui = NULL;
  remote_ui_service_args_t* service_args = (remote_ui_service_args_t*)args;

  return_value_if_fail(io != NULL, NULL);

  ui = (remote_ui_service_t*)TKMEM_ZALLOC(remote_ui_service_t);
  return_value_if_fail(ui != NULL, NULL);

  tk_service_init(&(ui->service), io);
  ui->service.dispatch = (tk_service_dispatch_t)remote_ui_service_dispatch;
  ui->service.destroy = (tk_service_destroy_t)remote_ui_service_destroy;

  if (service_args != NULL && service_args->auth != NULL) {
    ui->auth = service_args->auth;
  }

  return (tk_service_t*)ui;
}

static ubjson_writer_t* remote_ui_service_get_writer(remote_ui_service_t* ui) {
  wbuffer_t* wb = &(ui->service.wb);
  ubjson_writer_t* writer = &(ui->writer);

  wb->cursor = 0;
  return ubjson_writer_init(writer, (ubjson_write_callback_t)wbuffer_write_binary, wb);
}

static ret_t remote_ui_service_login(remote_ui_service_t* ui, const char* username,
                                     const char* password) {
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  if (ui->auth != NULL) {
    if (ui->auth((tk_service_t*)ui, username, password) == RET_OK) {
      ui->is_login = TRUE;
      return RET_OK;
    } else {
      return RET_FAIL;
    }
  } else {
    return RET_OK;
  }
}

static ret_t remote_ui_service_logout(remote_ui_service_t* ui) {
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);

  ui->is_login = FALSE;

  return RET_OK;
}

static ret_t remote_ui_service_get_dev_info(remote_ui_service_t* ui, remote_ui_dev_info_t* info) {
  ret_t ret = RET_OK;
  widget_t* wm = window_manager();
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(info != NULL, RET_BAD_PARAMS);

  memset(info, 0x00, sizeof(*info));
  info->screen_width = wm->w;
  info->screen_height = wm->h;
  /*TODO*/
  return ret;
}

static ret_t remote_ui_service_reboot(remote_ui_service_t* ui,
                                      remote_ui_reboot_type_t reboot_type) {
  /*TODO*/
  return RET_OK;
}

static ret_t remote_ui_service_create_dir(remote_ui_service_t* ui, const char* path) {
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(path != NULL, RET_BAD_PARAMS);

  return fs_create_dir_r(os_fs(), path);
}

static ret_t remote_ui_service_remove_dir(remote_ui_service_t* ui, const char* path) {
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(path != NULL, RET_BAD_PARAMS);

  return fs_remove_dir_r(os_fs(), path);
}

static ret_t remote_ui_service_remove_file(remote_ui_service_t* ui, const char* filename) {
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(filename != NULL, RET_BAD_PARAMS);

  return fs_remove_file(os_fs(), filename);
}

static ret_t remote_ui_service_get_manifest(remote_ui_service_t* ui, str_t* result) {
  ret_t ret = RET_OK;
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(result != NULL, RET_BAD_PARAMS);

  str_set(result, "todo");

  /*TODO*/
  return ret;
}

static ret_t remote_ui_service_take_screen_shot(remote_ui_service_t* ui, const char* filename) {
  ret_t ret = RET_OK;
  bitmap_t* image = NULL;
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(filename != NULL, RET_BAD_PARAMS);

  image = widget_take_snapshot(window_manager());
  if (image != NULL) {
    ret = bitmap_save_png(image, filename);
  } else {
    fs_remove_file(os_fs(), filename);
  }

  return ret;
}

static ret_t remote_ui_service_prepare_manifest(remote_ui_service_t* ui, const char* filename) {
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(filename != NULL, RET_BAD_PARAMS);

  return file_write(filename, "TODO", 5);
}

static ret_t remote_ui_service_prepare_xml_source(remote_ui_service_t* ui, const char* filename) {
  str_t str;
  ret_t ret = RET_OK;
  widget_t* win = window_manager_get_top_window(window_manager());
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(filename != NULL, RET_BAD_PARAMS);

  str_init(&str, 10000);
  widget_to_xml(win, &str);
  ret = file_write(filename, str.str, str.size);
  str_reset(&str);

  return ret;
}

static ret_t remote_ui_service_on_event_func(void* ctx, event_t* e) {
  remote_ui_service_t* ui = (remote_ui_service_t*)ctx;
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);
  /*TODO*/
  return RET_OK;
}

static ret_t remote_ui_service_on_event(remote_ui_service_t* ui, const char* target,
                                        uint32_t event) {
  widget_t* win = window_manager_get_top_window(window_manager());
  widget_t* widget = widget_find_by_path(win, target, TRUE);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget_on(widget, event, remote_ui_service_on_event_func, ui);

  return RET_OK;
}

static ret_t remote_ui_service_off_event(remote_ui_service_t* ui, const char* target,
                                         uint32_t event) {
  widget_t* win = window_manager_get_top_window(window_manager());
  widget_t* widget = widget_find_by_path(win, target, TRUE);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  widget_off_by_func(widget, event, remote_ui_service_on_event_func, ui);

  return RET_OK;
}

static ret_t remote_ui_service_send_event(remote_ui_service_t* ui, const char* target,
                                          event_t* event) {
  widget_t* win = window_manager_get_top_window(window_manager());
  widget_t* widget = widget_find_by_path(win, target, TRUE);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  switch (event->type) {
    case EVT_CLICK: {
      event->type = EVT_POINTER_DOWN;
      widget_on_pointer_down(widget, pointer_event_cast(event));

      event->type = EVT_POINTER_UP;
      return widget_on_pointer_up(widget, pointer_event_cast(event));
    }
    case EVT_POINTER_DOWN: {
      return widget_on_pointer_down(widget, pointer_event_cast(event));
    }
    case EVT_POINTER_MOVE: {
      return widget_on_pointer_move(widget, pointer_event_cast(event));
    }
    case EVT_POINTER_UP: {
      return widget_on_pointer_up(widget, pointer_event_cast(event));
    }
    case EVT_KEY_DOWN: {
      return widget_on_keydown(widget, key_event_cast(event));
    }
    case EVT_KEY_UP: {
      return widget_on_keyup(widget, key_event_cast(event));
    }
    default: {
      break;
    }
  }

  return RET_FAIL;
}

static ret_t remote_ui_service_open_dialog(remote_ui_service_t* ui, const char* type,
                                           const char* title, const char* content,
                                           uint32_t duration) {
  if (tk_str_eq(type, REMOTE_UI_DIALOG_TYPE_CONFIRM)) {
    return dialog_confirm(title, content);
  } else if (tk_str_eq(type, REMOTE_UI_DIALOG_TYPE_INFO)) {
    return dialog_info(title, content);
  } else if (tk_str_eq(type, REMOTE_UI_DIALOG_TYPE_WARN)) {
    return dialog_warn(title, content);
  } else {
    return dialog_toast(content, duration);
  }

  return RET_OK;
}

static ret_t remote_ui_service_open_window(remote_ui_service_t* ui, const char* name,
                                           const char* xml, const char* init_json) {
  widget_t* win = NULL;
  if (TK_STR_IS_NOT_EMPTY(xml)) {
    win = ui_loader_load_widget_from_xml(NULL, xml, strlen(xml));
  } else {
    win = window_open(name);
  }

  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  if (init_json != NULL) {
    /*TODO*/
  }

  return RET_OK;
}

static ret_t remote_ui_service_close_window(remote_ui_service_t* ui, const char* name) {
  widget_t* win = widget_child(window_manager(), name);
  return_value_if_fail(win != NULL, RET_BAD_PARAMS);

  return window_manager_close_window(window_manager(), win);
}

static ret_t remote_ui_service_back_to_prev(remote_ui_service_t* ui) {
  window_manager_back(window_manager());

  return RET_OK;
}

static ret_t remote_ui_service_back_to_home(remote_ui_service_t* ui) {
  window_manager_back_to_home(window_manager());

  return RET_OK;
}

static ret_t remote_ui_service_set_prop(remote_ui_service_t* ui, const char* target,
                                        const char* name, const value_t* value) {
  widget_t* win = window_manager_get_top_window(window_manager());
  widget_t* widget = widget_find_by_path(win, target, TRUE);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_set_prop(widget, name, value);
}

static ret_t remote_ui_service_get_prop(remote_ui_service_t* ui, const char* target,
                                        const char* name, value_t* value) {
  widget_t* win = window_manager_get_top_window(window_manager());
  widget_t* widget = widget_find_by_path(win, target, TRUE);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  return widget_get_prop(widget, name, value);
}

static ret_t remote_ui_service_set_theme(remote_ui_service_t* ui, const char* theme) {
  return widget_set_theme(window_manager(), theme);
}

static ret_t remote_ui_service_exec_script(remote_ui_service_t* ui, const char* script,
                                           value_t* v) {
  tk_object_t* obj = object_default_create();
  ret_t ret = fscript_eval(obj, script, v);
  TK_OBJECT_UNREF(obj);

  return ret;
}

static ret_t remote_ui_service_set_language(remote_ui_service_t* ui, const char* language) {
  const char* p = NULL;
  char lang[TK_NAME_LEN + 1] = {0};
  char country[TK_NAME_LEN + 1] = {0};
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(language != NULL, RET_BAD_PARAMS);

  p = strchr(language, '_');
  if (p != NULL) {
    tk_strncpy(lang, language, p - language);
    tk_strcpy(country, p + 1);
  } else {
    tk_strcpy(lang, language);
  }

  return locale_info_change(locale_info(), lang, country);
}

static ret_t remote_ui_dev_info_write(ubjson_writer_t* writer, remote_ui_dev_info_t* info) {
  return_value_if_fail(writer != NULL && info != NULL, RET_BAD_PARAMS);

  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_DEV_NAME, info->name);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_DEV_OS, info->os);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_DEV_VERSION, info->version);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_DEV_ARCH, info->arch);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_DEV_SCREEN_WIDTH, info->screen_width);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_DEV_SCREEN_HEIGHT, info->screen_height);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_DEV_DPI, info->dpi);
  ubjson_writer_write_object_end(writer);

  return RET_OK;
}

static ret_t remote_ui_service_dispatch_impl(remote_ui_service_t* ui, tk_msg_header_t* req,
                                             wbuffer_t* wb) {
  value_t v;
  char buff[1024] = {0};
  ret_t ret = RET_FAIL;
  tk_object_t* obj = NULL;
  ubjson_writer_t* writer = NULL;
  tk_msg_header_t resp;
  char local_file[MAX_PATH + 1] = {0};
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(req != NULL && wb != NULL, RET_BAD_PARAMS);

  memset(&resp, 0x00, sizeof(resp));
  if (req->data_type == MSG_DATA_TYPE_UBJSON) {
    obj = conf_ubjson_load_from_buff(wb->data, wb->cursor, FALSE);
  }

  switch (req->type) {
    case MSG_REQ_LOGIN: {
      const char* username = tk_object_get_prop_str(obj, "username");
      const char* password = tk_object_get_prop_str(obj, "password");
      resp.resp_code = remote_ui_service_login(ui, username, password);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = MSG_RESP_LOGIN;
      wbuffer_rewind(wb);
      break;
    }
    case MSG_REQ_LOGOUT: {
      resp.resp_code = remote_ui_service_logout(ui);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = MSG_RESP_LOGOUT;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_GET_DEV_INFO: {
      remote_ui_dev_info_t info;
      memset(&info, 0x00, sizeof(info));
      resp.resp_code = remote_ui_service_get_dev_info(ui, &info);
      resp.data_type = MSG_DATA_TYPE_UBJSON;
      resp.type = REMOTE_UI_RESP_GET_DEV_INFO;
      writer = remote_ui_service_get_writer(ui);
      ret = remote_ui_dev_info_write(writer, &info);
      break;
    }
    case REMOTE_UI_REQ_REBOOT: {
      rbuffer_t rb;
      uint32_t reboot_type = REMOTE_UI_REBOOT_DEFAULT;
      rbuffer_init(&rb, wb->data, wb->cursor);

      rbuffer_read_uint32(&rb, &reboot_type);
      resp.resp_code = remote_ui_service_reboot(ui, reboot_type);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_REBOOT;
      wbuffer_rewind(wb);
      break;
    }
    case MSG_REQ_UPLOAD_FILE_BEGIN: {
      const char* filename = (const char*)(wb->data);
      filename = path_prepend_app_root(local_file, filename);
      resp.resp_code = tk_service_upload_file(&(ui->service), filename);
      return RET_OK;
      break;
    }
    case MSG_REQ_DOWNLOAD_FILE_BEGIN: {
      const char* filename = (const char*)(wb->data);

      if (tk_str_eq(filename, REMOTE_UI_FILE_SCREEN_SHOT)) {
        filename = path_prepend_temp_path(local_file, filename);
        resp.resp_code = remote_ui_service_take_screen_shot(ui, filename);
      } else if (tk_str_eq(filename, REMOTE_UI_FILE_MANIFEST)) {
        filename = path_prepend_temp_path(local_file, filename);
        resp.resp_code = remote_ui_service_prepare_manifest(ui, filename);
      } else if (tk_str_eq(filename, REMOTE_UI_FILE_XML_SOURCE)) {
        filename = path_prepend_temp_path(local_file, filename);
        resp.resp_code = remote_ui_service_prepare_xml_source(ui, filename);
      } else {
        filename = path_prepend_app_root(local_file, filename);
      }

      resp.resp_code = tk_service_download_file(&(ui->service), filename);
      return RET_OK;
    }
    case REMOTE_UI_REQ_CREATE_DIR: {
      const char* filename = (const char*)(wb->data);
      filename = path_prepend_app_root(local_file, filename);

      resp.resp_code = remote_ui_service_create_dir(ui, filename);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_CREATE_DIR;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_REMOVE_DIR: {
      const char* filename = (const char*)(wb->data);
      filename = path_prepend_app_root(local_file, filename);

      resp.resp_code = remote_ui_service_remove_dir(ui, filename);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_REMOVE_DIR;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_REMOVE_FILE: {
      const char* filename = (const char*)(wb->data);
      filename = path_prepend_app_root(local_file, filename);
      resp.resp_code = remote_ui_service_remove_file(ui, filename);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_REMOVE_FILE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_OPEN_WINDOW: {
      const char* name = tk_object_get_prop_str(obj, REMOTE_UI_KEY_NAME);
      const char* xml = tk_object_get_prop_str(obj, REMOTE_UI_KEY_XML);
      const char* init_json = tk_object_get_prop_str(obj, REMOTE_UI_KEY_INIT);
      resp.resp_code = remote_ui_service_open_window(ui, name, xml, init_json);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_OPEN_WINDOW;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_OPEN_DIALOG: {
      const char* type = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TYPE);
      const char* title = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TITLE);
      const char* content = tk_object_get_prop_str(obj, REMOTE_UI_KEY_CONTENT);
      uint32_t duration = tk_object_get_prop_uint32(obj, REMOTE_UI_KEY_DURATION, 3000);

      resp.resp_code = remote_ui_service_open_dialog(ui, type, title, content, duration);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_OPEN_WINDOW;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_BACK_TO_PREV: {
      resp.resp_code = remote_ui_service_back_to_prev(ui);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_BACK_TO_PREV;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_BACK_TO_HOME: {
      resp.resp_code = remote_ui_service_back_to_home(ui);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_BACK_TO_HOME;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_CLOSE_WINDOW: {
      const char* name = (const char*)(wb->data);
      resp.resp_code = remote_ui_service_close_window(ui, name);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_CLOSE_WINDOW;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_SET_PROP: {
      value_t v;
      const char* target = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TARGET);
      const char* name = tk_object_get_prop_str(obj, REMOTE_UI_KEY_NAME);

      if (tk_object_get_prop(obj, REMOTE_UI_KEY_VALUE, &v) == RET_OK) {
        resp.resp_code = remote_ui_service_set_prop(ui, target, name, &v);
      } else {
        resp.resp_code = RET_FAIL;
      }
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_GET_PROP: {
      const char* str = NULL;
      const char* target = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TARGET);
      const char* name = tk_object_get_prop_str(obj, REMOTE_UI_KEY_NAME);

      value_set_int(&v, 0);
      resp.resp_code = remote_ui_service_get_prop(ui, target, name, &v);
      resp.data_type = MSG_DATA_TYPE_STRING;
      str = value_str_ex(&v, buff, sizeof(buff));
      wbuffer_rewind(wb);
      wbuffer_write_string(wb, str);
      break;
    }
    case REMOTE_UI_REQ_SET_LANGUAGE: {
      const char* language = (const char*)(wb->data);
      resp.resp_code = remote_ui_service_set_language(ui, language);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_SET_LANGUAGE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_SET_THEME: {
      const char* theme = (const char*)(wb->data);
      resp.resp_code = remote_ui_service_set_theme(ui, theme);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_SET_THEME;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_EXEC_FSCRIPT: {
      const char* script = (const char*)(wb->data);

      value_set_int(&v, 0);
      resp.resp_code = remote_ui_service_exec_script(ui, script, &v);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_EXEC_FSCRIPT;
      wbuffer_rewind(wb);
      wbuffer_write_string(wb, value_str_ex(&v, buff, sizeof(buff)));

      break;
    }
    case REMOTE_UI_REQ_ON_EVENT: {
      const char* target = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TARGET);
      uint32_t event_type = tk_object_get_prop_int(obj, REMOTE_UI_KEY_EVENT, 0);
      resp.resp_code = remote_ui_service_on_event(ui, target, event_type);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_ON_EVENT;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_OFF_EVENT: {
      const char* target = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TARGET);
      uint32_t event_type = tk_object_get_prop_int(obj, REMOTE_UI_KEY_EVENT, 0);
      resp.resp_code = remote_ui_service_off_event(ui, target, event_type);
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_OFF_EVENT;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REQ_SEND_EVENT: {
      event_t* e = NULL;
      const char* target = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TARGET);
      uint32_t event_type = tk_object_get_prop_int(obj, REMOTE_UI_KEY_EVENT, 0);

      switch (event_type) {
        case EVT_CLICK:
        case EVT_POINTER_DOWN:
        case EVT_POINTER_UP:
        case EVT_POINTER_MOVE: {
          pointer_event_t evt;
          int x = tk_object_get_prop_int(obj, REMOTE_UI_KEY_X, 0);
          int y = tk_object_get_prop_int(obj, REMOTE_UI_KEY_Y, 0);
          e = pointer_event_init(&evt, event_type, NULL, x, y);
          break;
        }
        case EVT_KEY_DOWN:
        case EVT_KEY_UP: {
          key_event_t evt;
          uint32_t key = tk_object_get_prop_int(obj, REMOTE_UI_KEY_CODE, 0);
          e = key_event_init(&evt, event_type, NULL, key);
          break;
        }
        default:
          break;
      }
      if (e != NULL) {
        resp.resp_code = remote_ui_service_send_event(ui, target, e);
      } else {
        resp.resp_code = RET_FAIL;
      }
      resp.data_type = MSG_DATA_TYPE_NONE;
      resp.type = REMOTE_UI_RESP_SEND_EVENT;
      wbuffer_rewind(wb);
      break;
    }
    default: {
      ret = RET_NOT_IMPL;
      break;
    }
  }
  TK_OBJECT_UNREF(obj);

  return tk_service_send_resp(&(ui->service), resp.type, resp.data_type, resp.resp_code, wb);
}

static ret_t remote_ui_service_dispatch(remote_ui_service_t* ui) {
  ret_t ret = RET_OK;
  tk_msg_header_t header;
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);

  memset(&header, 0x00, sizeof(header));
  ret = tk_service_read_req(&(ui->service), &header, &(ui->service.wb));
  return_value_if_fail(ret == RET_OK, ret);

  return remote_ui_service_dispatch_impl(ui, &header, &(ui->service.wb));
}

static ret_t remote_ui_service_destroy(remote_ui_service_t* ui) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);

  TKMEM_FREE(ui);

  return RET_OK;
}
