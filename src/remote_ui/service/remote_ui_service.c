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
#include "conf_io/conf_json.h"
#include "tkc/object_default.h"
#include "ui_loader/ui_serializer.h"
#include "remote_ui/shared/remote_ui_types_def.h"
#include "remote_ui/service/remote_ui_service.h"

#include "base/events.h"
#include "base/keys.h"

static ret_t remote_ui_service_dispatch(remote_ui_service_t* ui);
static ret_t remote_ui_service_destroy(remote_ui_service_t* ui);
static ret_t remote_ui_service_set_language(remote_ui_service_t* ui, const char* language);

tk_service_t* remote_ui_service_create(tk_iostream_t* io, void* args) {
  remote_ui_service_t* ui = NULL;
  remote_ui_service_args_t* service_args = (remote_ui_service_args_t*)args;

  return_value_if_fail(io != NULL, NULL);

  ui = (remote_ui_service_t*)TKMEM_ZALLOC(remote_ui_service_t);
  return_value_if_fail(ui != NULL, NULL);

  tk_service_init(&(ui->service), io);
  ui->service.dispatch = (tk_service_dispatch_t)remote_ui_service_dispatch;
  ui->service.destroy = (tk_service_destroy_t)remote_ui_service_destroy;

  if (service_args != NULL) {
    if (service_args->auth != NULL) {
      ui->auth = service_args->auth;
    }

    if (service_args->find_target != NULL) {
      ui->find_target = service_args->find_target;
    }
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

static widget_t* remote_ui_service_get_app_window(widget_t* widget) {
  return_value_if_fail(widget != NULL, NULL);

  WIDGET_FOR_EACH_CHILD_BEGIN_R(widget, iter, i)
  if (iter->visible && !widget_is_keyboard(iter) && !widget_is_always_on_top(iter)) {
    return iter;
  }
  WIDGET_FOR_EACH_CHILD_END();

  return NULL;
}

static widget_t* remote_ui_service_get_target_widget(remote_ui_service_t* ui, const char* target) {
  widget_t* win = remote_ui_service_get_app_window(window_manager());
  widget_t* widget = TK_STR_IS_EMPTY(target) ? win : widget_find_by_path(win, target, TRUE);

  return widget;
}

static ret_t remote_ui_service_take_snapshot(remote_ui_service_t* ui, const char* target,
                                             const char* filename) {
  ret_t ret = RET_OK;
  bitmap_t* image = NULL;
  widget_t* widget = remote_ui_service_get_target_widget(ui, target);

  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(filename != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  image = widget_take_snapshot(widget);
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

static ret_t remote_ui_service_prepare_loaded_images_info(remote_ui_service_t* ui,
                                                          const char* target,
                                                          const char* filename) {
  str_t result;
  str_init(&result, 10000);
  image_manager_dump(image_manager(), &result);
  file_write(filename, result.str, result.size);
  str_reset(&result);

  return RET_OK;
}

static ret_t remote_ui_service_prepare_loaded_assets_info(remote_ui_service_t* ui,
                                                          const char* target,
                                                          const char* filename) {
  str_t result;
  str_init(&result, 10000);
  assets_manager_dump(assets_manager(), &result);
  file_write(filename, result.str, result.size);
  str_reset(&result);

  return RET_OK;
}

static ret_t remote_ui_service_prepare_xml_source(remote_ui_service_t* ui, const char* target,
                                                  const char* filename) {
  str_t str;
  ret_t ret = RET_OK;
  widget_t* widget = remote_ui_service_get_target_widget(ui, target);

  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(filename != NULL, RET_BAD_PARAMS);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  str_init(&str, 10000);
  widget_to_xml(widget, &str);
  ret = file_write(filename, str.str, str.size);
  str_reset(&str);

  return ret;
}

static ret_t remote_ui_service_on_event_func(void* ctx, event_t* e) {
  wbuffer_t* wb = NULL;
  char target[32] = {0};
  ubjson_writer_t* writer = NULL;
  remote_ui_service_t* ui = (remote_ui_service_t*)ctx;
  return_value_if_fail(ui != NULL, RET_REMOVE);
  return_value_if_fail(ui->service.destroy == (tk_service_destroy_t)remote_ui_service_destroy,
                       RET_REMOVE);

  wb = &(ui->service.wb);
  writer = remote_ui_service_get_writer(ui);
  log_debug("remote_ui_service_on_event_func type=%d\n", e->type);

  tk_snprintf(target, sizeof(target) - 1, "%p", e->target);
  ubjson_writer_write_object_begin(writer);
  ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_TARGET, target);
  ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_EVENT, e->type);

  switch (e->type) {
    case EVT_VALUE_CHANGED: {
      char value[64] = {0};
      value_change_event_t* event = value_change_event_cast(e);
      value_t* v = &(event->new_value);
      ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_VALUE_TYPE, v->type);
      ubjson_writer_write_kv_str(writer, REMOTE_UI_KEY_VALUE,
                                 value_str_ex(v, value, sizeof(value)));
      break;
    }
    case EVT_KEY_DOWN:
    case EVT_KEY_UP: {
      key_event_t* event = key_event_cast(e);
      ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_VALUE_TYPE, event->key);
      break;
    }
    case EVT_POINTER_DOWN:
    case EVT_POINTER_MOVE:
    case EVT_POINTER_UP: {
      pointer_event_t* event = pointer_event_cast(e);
      ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_X, event->x);
      ubjson_writer_write_kv_int(writer, REMOTE_UI_KEY_Y, event->y);
      break;
    }
    default:
      break;
  }

  ubjson_writer_write_object_end(writer);

  return tk_service_send_resp(&(ui->service), MSG_CODE_NOTIFY, MSG_DATA_TYPE_UBJSON, RET_OK, wb);
}

static ret_t remote_ui_service_on_event(remote_ui_service_t* ui, const char* target,
                                        uint32_t event) {
  uint32_t id = 0;
  char buff[32] = {0};
  void* target_obj = NULL;
  ret_t ret = RET_NOT_FOUND;
  wbuffer_t* wb = &(ui->service.wb);

  if (ui->find_target != NULL) {
    tk_object_t* obj = ui->find_target(&(ui->service), target);
    if (obj != NULL) {
      id = emitter_on(obj, event, remote_ui_service_on_event_func, ui);
      target_obj = obj;
    }
  }

  if (target_obj != NULL) {
    /*do nothing*/
  } else if (tk_str_eq(target, REMOTE_UI_TARGET_GLOBAL)) {
    window_manager_t* wm = WINDOW_MANAGER(window_manager());
    id = emitter_on(wm->global_emitter, event, remote_ui_service_on_event_func, ui);
    target_obj = wm->global_emitter;
  } else {
    widget_t* widget = remote_ui_service_get_target_widget(ui, target);
    id = widget_on(widget, event, remote_ui_service_on_event_func, ui);
    target_obj = widget;
  }
  ret = id > 0 ? RET_OK : RET_FAIL;

  wbuffer_rewind(wb);
  if (target_obj != NULL) {
    tk_snprintf(buff, sizeof(buff) - 1, "%p", target_obj);
    wbuffer_write_string(wb, buff);
  }

  return tk_service_send_resp(&(ui->service), REMOTE_UI_ON_EVENT, MSG_DATA_TYPE_STRING, ret, wb);
}

static ret_t remote_ui_service_off_event(remote_ui_service_t* ui, const char* target,
                                         uint32_t event) {
  char buff[32] = {0};
  void* target_obj = NULL;
  ret_t ret = RET_NOT_FOUND;
  wbuffer_t* wb = &(ui->service.wb);

  if (ui->find_target != NULL) {
    tk_object_t* obj = ui->find_target(&(ui->service), target);
    if (obj != NULL) {
      ret = emitter_off_by_func(obj, event, remote_ui_service_on_event_func, ui);
      target_obj = obj;
    }
  }

  if (target_obj != NULL) {
    /*do nothing*/
  } else if (tk_str_eq(target, REMOTE_UI_TARGET_GLOBAL)) {
    window_manager_t* wm = WINDOW_MANAGER(window_manager());
    ret = emitter_off_by_func(wm->global_emitter, event, remote_ui_service_on_event_func, ui);
    target_obj = wm->global_emitter;
  } else {
    widget_t* widget = remote_ui_service_get_target_widget(ui, target);
    ret = widget_off_by_func(widget, event, remote_ui_service_on_event_func, ui);
    target_obj = widget;
  }

  wbuffer_rewind(wb);
  if (target_obj != NULL) {
    tk_snprintf(buff, sizeof(buff) - 1, "%p", target_obj);
    wbuffer_write_string(wb, buff);
  }

  return tk_service_send_resp(&(ui->service), REMOTE_UI_OFF_EVENT, MSG_DATA_TYPE_STRING, ret, wb);
}

static ret_t remote_ui_service_send_event(remote_ui_service_t* ui, const char* target,
                                          event_t* event) {
  widget_t* widget = remote_ui_service_get_target_widget(ui, target);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  event->target = widget;
  if (!widget_is_window_manager(widget) && !widget_is_window(widget)) {
    return widget_dispatch_async(widget, event);
  }

  switch (event->type) {
    case EVT_CLICK: {
      return widget_dispatch_async(widget, event);
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

static ret_t widget_init_with_conf(widget_t* win, const char* widget_name, conf_node_t* node) {
  char buff[64] = {0};
  conf_node_t* iter = conf_node_get_first_child(node);
  widget_t* widget = widget_find_by_path(win, widget_name, TRUE);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    value_t v;
    const char* name = conf_node_get_name(iter);
    if (conf_node_get_value(iter, &v) == RET_OK) {
      log_debug("%s.%s=%s\n", widget_name, name, value_str_ex(&v, buff, sizeof(buff)));
      widget_set_prop(widget, name, &v);
    }

    iter = iter->next;
  }
  return RET_OK;
}

static ret_t window_init_with_conf(widget_t* widget, conf_doc_t* doc) {
  conf_node_t* iter = conf_node_get_first_child(doc->root);
  while (iter != NULL) {
    const char* name = conf_node_get_name(iter);
    widget_init_with_conf(widget, name, iter);
    iter = iter->next;
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
    conf_doc_t* doc = conf_doc_load_json(init_json, strlen(init_json));
    if (doc != NULL) {
      window_init_with_conf(win, doc);
      conf_doc_destroy(doc);
    }
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
  if (ui->find_target != NULL) {
    tk_object_t* obj = ui->find_target(&(ui->service), target);
    if (obj != NULL) {
      return tk_object_set_prop(obj, name, value);
    }
  }

  if (tk_str_eq(target, REMOTE_UI_TARGET_GLOBAL)) {
    if (tk_str_eq(name, REMOTE_UI_PROP_THEME)) {
      return widget_set_theme(window_manager(), value_str(value));
    } else if (tk_str_eq(name, REMOTE_UI_PROP_LANGUAGE)) {
      return remote_ui_service_set_language(ui, value_str(value));
    } else {
      return RET_NOT_FOUND;
    }
  } else {
    widget_t* widget = remote_ui_service_get_target_widget(ui, target);
    return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

    return widget_set_prop(widget, name, value);
  }
}

static ret_t remote_ui_service_get_prop(remote_ui_service_t* ui, const char* target,
                                        const char* name, value_t* value) {
  char buff[128] = {0};
  if (ui->find_target != NULL) {
    tk_object_t* obj = ui->find_target(&(ui->service), target);
    if (obj != NULL) {
      return tk_object_get_prop(obj, name, value);
    }
  }

  if (tk_str_eq(target, REMOTE_UI_TARGET_GLOBAL)) {
    if (tk_str_eq(name, REMOTE_UI_PROP_THEME)) {
      value_set_str(value, widget_get_theme_name(window_manager()));
      return RET_OK;
    } else if (tk_str_eq(name, REMOTE_UI_PROP_LANGUAGE)) {
      locale_info_t* info = widget_get_locale_info(window_manager());
      tk_snprintf(buff, sizeof(buff) - 1, "%s_%s", info->language, info->country);
      value_dup_str(value, buff);
      return RET_OK;
    } else {
      return RET_NOT_FOUND;
    }
  } else {
    widget_t* widget = remote_ui_service_get_target_widget(ui, target);
    return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

    return widget_get_prop(widget, name, value);
  }
}

static ret_t remote_ui_service_exec_script(remote_ui_service_t* ui, const char* script,
                                           value_t* v) {
  ret_t ret = RET_FAIL;
  tk_object_t* obj = object_default_create();

  if (obj != NULL) {
    widget_t* wm = window_manager();
    widget_t* win = remote_ui_service_get_app_window(wm);

    tk_object_set_prop_pointer(obj, STR_PROP_SELF, win);
    tk_object_set_prop_pointer(obj, STR_PROP_WINDOW, win);
    tk_object_set_prop_pointer(obj, STR_PROP_WINDOW_MANAGER, wm);

    ret = fscript_eval(obj, script, v);
    TK_OBJECT_UNREF(obj);
  }

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

  resp.type = req->type;
  switch (req->type) {
    case MSG_CODE_LOGIN: {
      const char* username = tk_object_get_prop_str(obj, "username");
      const char* password = tk_object_get_prop_str(obj, "password");
      resp.resp_code = remote_ui_service_login(ui, username, password);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case MSG_CODE_LOGOUT: {
      resp.resp_code = remote_ui_service_logout(ui);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_GET_DEV_INFO: {
      remote_ui_dev_info_t info;
      memset(&info, 0x00, sizeof(info));
      resp.resp_code = remote_ui_service_get_dev_info(ui, &info);
      resp.data_type = MSG_DATA_TYPE_UBJSON;
      writer = remote_ui_service_get_writer(ui);
      remote_ui_dev_info_write(writer, &info);
      break;
    }
    case REMOTE_UI_REBOOT: {
      rbuffer_t rb;
      uint32_t reboot_type = REMOTE_UI_REBOOT_DEFAULT;
      rbuffer_init(&rb, wb->data, wb->cursor);

      rbuffer_read_uint32(&rb, &reboot_type);
      resp.resp_code = remote_ui_service_reboot(ui, reboot_type);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case MSG_CODE_UPLOAD_FILE_BEGIN: {
      const char* filename = (const char*)(wb->data);
      filename = path_prepend_app_root(local_file, filename);
      resp.resp_code = tk_service_upload_file(&(ui->service), filename);
      return RET_OK;
      break;
    }
    case MSG_CODE_DOWNLOAD_FILE_BEGIN: {
      const char* filename = (const char*)(wb->data);

      if (tk_str_start_with(filename, REMOTE_UI_FILE_SNAPSHOT)) {
        const char* target = filename + strlen(REMOTE_UI_FILE_SNAPSHOT) + 1;
        filename = path_prepend_temp_path(local_file, "shot.png");
        resp.resp_code = remote_ui_service_take_snapshot(ui, target, filename);
      } else if (tk_str_eq(filename, REMOTE_UI_FILE_MANIFEST)) {
        filename = path_prepend_temp_path(local_file, "manifest.txt");
        resp.resp_code = remote_ui_service_prepare_manifest(ui, filename);
      } else if (tk_str_start_with(filename, REMOTE_UI_FILE_XML_SOURCE)) {
        const char* target = filename + strlen(REMOTE_UI_FILE_XML_SOURCE) + 1;
        filename = path_prepend_temp_path(local_file, "source.xml");
        resp.resp_code = remote_ui_service_prepare_xml_source(ui, target, filename);
      } else if (tk_str_start_with(filename, REMOTE_UI_FILE_LOADED_IMAGES_INFO)) {
        const char* target = filename + strlen(REMOTE_UI_FILE_LOADED_IMAGES_INFO) + 1;
        filename = path_prepend_temp_path(local_file, "loaded_images_info.txt");
        resp.resp_code = remote_ui_service_prepare_loaded_images_info(ui, target, filename);
      } else if (tk_str_start_with(filename, REMOTE_UI_FILE_LOADED_ASSETS_INFO)) {
        const char* target = filename + strlen(REMOTE_UI_FILE_LOADED_ASSETS_INFO) + 1;
        filename = path_prepend_temp_path(local_file, "loaded_images_info.txt");
        resp.resp_code = remote_ui_service_prepare_loaded_assets_info(ui, target, filename);
      } else {
        filename = path_prepend_app_root(local_file, filename);
      }

      resp.resp_code = tk_service_download_file(&(ui->service), filename);
      return RET_OK;
    }
    case REMOTE_UI_CREATE_DIR: {
      const char* filename = (const char*)(wb->data);
      filename = path_prepend_app_root(local_file, filename);

      resp.resp_code = remote_ui_service_create_dir(ui, filename);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REMOVE_DIR: {
      const char* filename = (const char*)(wb->data);
      filename = path_prepend_app_root(local_file, filename);

      resp.resp_code = remote_ui_service_remove_dir(ui, filename);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_REMOVE_FILE: {
      const char* filename = (const char*)(wb->data);
      filename = path_prepend_app_root(local_file, filename);
      resp.resp_code = remote_ui_service_remove_file(ui, filename);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_OPEN_WINDOW: {
      const char* name = tk_object_get_prop_str(obj, REMOTE_UI_KEY_NAME);
      const char* xml = tk_object_get_prop_str(obj, REMOTE_UI_KEY_XML);
      const char* init_json = tk_object_get_prop_str(obj, REMOTE_UI_KEY_INIT);
      resp.resp_code = remote_ui_service_open_window(ui, name, xml, init_json);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_OPEN_DIALOG: {
      const char* type = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TYPE);
      const char* title = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TITLE);
      const char* content = tk_object_get_prop_str(obj, REMOTE_UI_KEY_CONTENT);
      uint32_t duration = tk_object_get_prop_uint32(obj, REMOTE_UI_KEY_DURATION, 3000);

      resp.resp_code = remote_ui_service_open_dialog(ui, type, title, content, duration);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_BACK_TO_PREV: {
      resp.resp_code = remote_ui_service_back_to_prev(ui);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_BACK_TO_HOME: {
      resp.resp_code = remote_ui_service_back_to_home(ui);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_CLOSE_WINDOW: {
      const char* name = (const char*)(wb->data);
      resp.resp_code = remote_ui_service_close_window(ui, name);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_SET_PROP: {
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
    case REMOTE_UI_GET_PROP: {
      const char* str = NULL;
      const char* target = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TARGET);
      const char* name = tk_object_get_prop_str(obj, REMOTE_UI_KEY_NAME);

      value_set_int(&v, 0);
      resp.resp_code = remote_ui_service_get_prop(ui, target, name, &v);
      resp.data_type = MSG_DATA_TYPE_STRING;
      str = value_str_ex(&v, buff, sizeof(buff));
      wbuffer_rewind(wb);
      wbuffer_write_string(wb, str);
      value_reset(&v);
      break;
    }
    case REMOTE_UI_EXEC_FSCRIPT: {
      const char* script = (const char*)(wb->data);

      value_set_int(&v, 0);
      resp.resp_code = remote_ui_service_exec_script(ui, script, &v);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      wbuffer_write_string(wb, value_str_ex(&v, buff, sizeof(buff)));

      break;
    }
    case REMOTE_UI_ON_EVENT: {
      const char* target = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TARGET);
      uint32_t event_type = tk_object_get_prop_int(obj, REMOTE_UI_KEY_EVENT, 0);
      return remote_ui_service_on_event(ui, target, event_type);
    }
    case REMOTE_UI_OFF_EVENT: {
      const char* target = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TARGET);
      uint32_t event_type = tk_object_get_prop_int(obj, REMOTE_UI_KEY_EVENT, 0);
      return remote_ui_service_off_event(ui, target, event_type);
    }
    case REMOTE_UI_SEND_EVENT: {
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
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_MOVE_WIDGET: {
      const char* target = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TARGET);
      int x = tk_object_get_prop_int(obj, REMOTE_UI_KEY_X, 0);
      int y = tk_object_get_prop_int(obj, REMOTE_UI_KEY_Y, 0);
      widget_t* widget = remote_ui_service_get_target_widget(ui, target);
      if (widget != NULL) {
        resp.resp_code = widget_move(widget, x, y);
      } else {
        resp.resp_code = RET_NOT_FOUND;
      }

      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_RESIZE_WIDGET: {
      const char* target = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TARGET);
      int w = tk_object_get_prop_int(obj, REMOTE_UI_KEY_W, 0);
      int h = tk_object_get_prop_int(obj, REMOTE_UI_KEY_H, 0);
      widget_t* widget = remote_ui_service_get_target_widget(ui, target);
      if (widget != NULL) {
        resp.resp_code = widget_resize(widget, w, h);
      } else {
        resp.resp_code = RET_NOT_FOUND;
      }
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_DESTROY_WIDGET: {
      const char* target = (const char*)(wb->data);
      widget_t* widget = remote_ui_service_get_target_widget(ui, target);
      if (widget != NULL) {
        resp.resp_code = widget_destroy(widget);
      } else {
        resp.resp_code = RET_NOT_FOUND;
      }
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_CREATE_WIDGET: {
      const char* target = tk_object_get_prop_str(obj, REMOTE_UI_KEY_TARGET);
      const char* xml = tk_object_get_prop_str(obj, REMOTE_UI_KEY_XML);
      widget_t* widget = remote_ui_service_get_target_widget(ui, target);
      if (widget != NULL) {
        widget_t* new_widget = ui_loader_load_widget_from_xml(widget, xml, strlen(xml));
        if (new_widget != NULL) {
          resp.resp_code = RET_OK;
        } else {
          resp.resp_code = RET_FAIL;
        }
      } else {
        resp.resp_code = RET_NOT_FOUND;
      }
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    default: {
      resp.resp_code = RET_NOT_IMPL;
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

  memset(ui, 0x00, sizeof(*ui));
  TKMEM_FREE(ui);

  return RET_OK;
}

static ret_t remote_ui_dispatch_timer(const timer_info_t* info) {
  tk_istream_t* in = NULL;
  tk_service_t* service = (tk_service_t*)(info->ctx);
  return_value_if_fail(service != NULL && service->io != NULL, RET_REMOVE);

  in = tk_iostream_get_istream(service->io);
  return_value_if_fail(in != NULL, RET_REMOVE);

  if (tk_istream_wait_for_data(in, 20) == RET_OK) {
    tk_service_dispatch(service);
  }

  return RET_REPEAT;
}

tk_service_t* remote_ui_service_start_with_uart(tk_iostream_t* io, void* args) {
  tk_service_t* service = NULL;
  return_value_if_fail(io != NULL, RET_BAD_PARAMS);

  service = remote_ui_service_create(io, args);
  return_value_if_fail(service != NULL, NULL);

  timer_add(remote_ui_dispatch_timer, service, 20);

  return service;
}