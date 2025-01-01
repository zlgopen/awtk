/**
 * File:   remote_ui_service.c
 * Author: AWTK Develop Team
 * Brief:  remote ui service
 *
 * Copyright (c) 2018 - 2025  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
static ret_t remote_ui_service_on_event_func(void* ctx, event_t* e);
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

    if (service_args->logout != NULL) {
      ui->logout = service_args->logout;
    }
  }

  return (tk_service_t*)ui;
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

  if (ui->logout != NULL) {
    ui->logout(&(ui->service));
  }

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

  if (widget == NULL) {
    widget = widget_lookup_by_type(win, target, TRUE);
  }

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
    ret = bitmap_save_png(image, filename) ? RET_OK : RET_FAIL;
  } else {
    fs_remove_file(os_fs(), filename);
  }
  bitmap_destroy(image);

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
  ret_t ret = RET_OK;
  wbuffer_t* wb = NULL;
  char target[32] = {0};
  remote_ui_service_t* ui = (remote_ui_service_t*)ctx;
  return_value_if_fail(ui != NULL, RET_REMOVE);
  return_value_if_fail(ui->service.destroy == (tk_service_destroy_t)remote_ui_service_destroy,
                       RET_REMOVE);

  if (ui->dispatching) {
    log_debug("ignore event %d because busy\n", e->type);
    return RET_OK;
  }

  wb = &(ui->service.wb);
  log_debug("remote_ui_service_on_event_func type=%d\n", e->type);

  tk_snprintf(target, sizeof(target) - 1, "%p", e->target);

  wbuffer_rewind(wb);
  wbuffer_write_string(wb, target);
  wbuffer_write_int32(wb, e->type);

  switch (e->type) {
    case EVT_VALUE_CHANGED: {
      value_change_event_t* event = value_change_event_cast(e);
      value_t* v = &(event->new_value);
      wbuffer_write_value(wb, v);
      break;
    }
    case EVT_PROP_CHANGED: {
      value_t vv;
      value_t* v = NULL;
      prop_change_event_t* event = prop_change_event_cast(e);
      /*保持类型不变*/
      if (tk_object_get_prop(OBJECT(e->target), event->name, &vv) == RET_OK) {
        v = &vv;
      } else {
        v = (value_t*)(event->value);
      }
      wbuffer_write_string(wb, event->name);
      wbuffer_write_value(wb, v);
      break;
    }
    case EVT_KEY_DOWN:
    case EVT_KEY_UP: {
      key_event_t* event = key_event_cast(e);
      wbuffer_write_int32(wb, event->key);
      break;
    }
    case EVT_POINTER_DOWN:
    case EVT_POINTER_MOVE:
    case EVT_POINTER_UP: {
      pointer_event_t* event = pointer_event_cast(e);
      wbuffer_write_int32(wb, event->x);
      wbuffer_write_int32(wb, event->y);
      break;
    }
    default:
      break;
  }

  ret = tk_service_send_resp(&(ui->service), MSG_CODE_NOTIFY, MSG_DATA_TYPE_BINARY, RET_OK, wb);
  if (ret != RET_OK) {
    log_debug("send event %d failed\n", e->type);
    ret = RET_REMOVE;
  }

  return ret;
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
      emitter_off_by_func(EMITTER(obj), event, remote_ui_service_on_event_func, ui);
      id = emitter_on(EMITTER(obj), event, remote_ui_service_on_event_func, ui);
      target_obj = obj;
    }
  }

  if (target_obj != NULL) {
    /*do nothing*/
  } else if (tk_str_eq(target, REMOTE_UI_TARGET_GLOBAL)) {
    window_manager_t* wm = WINDOW_MANAGER(window_manager());
    emitter_off_by_func(wm->global_emitter, event, remote_ui_service_on_event_func, ui);
    id = emitter_on(wm->global_emitter, event, remote_ui_service_on_event_func, ui);
    target_obj = wm->global_emitter;
  } else {
    widget_t* widget = remote_ui_service_get_target_widget(ui, target);
    widget_off_by_func(widget, event, remote_ui_service_on_event_func, ui);
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
      ret = emitter_off_by_func(EMITTER(obj), event, remote_ui_service_on_event_func, ui);
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
  if (tk_str_eq(target, "global") && widget == NULL) {
    window_manager_dispatch_input_event(window_manager(), event);
    return RET_OK;
  }

  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);
  event->target = widget;
  if (!widget_is_window_manager(widget) && !widget_is_window(widget)) {
    widget_dispatch_async(widget, event);

    return RET_OK;
  }

  switch (event->type) {
    case EVT_CLICK: {
      widget_dispatch_async(widget, event);
      break;
    }
    case EVT_POINTER_DOWN: {
      widget_on_pointer_down(widget, pointer_event_cast(event));
      break;
    }
    case EVT_POINTER_MOVE: {
      widget_on_pointer_move(widget, pointer_event_cast(event));
      break;
    }
    case EVT_POINTER_UP: {
      widget_on_pointer_up(widget, pointer_event_cast(event));
      break;
    }
    case EVT_KEY_DOWN: {
      widget_on_keydown(widget, key_event_cast(event));
      break;
    }
    case EVT_KEY_UP: {
      widget_on_keyup(widget, key_event_cast(event));
      break;
    }
    default: {
      return RET_FAIL;
      break;
    }
  }

  return RET_OK;
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
}

static ret_t widget_init_with_conf(widget_t* win, const char* widget_name, conf_node_t* node) {
  conf_node_t* iter = conf_node_get_first_child(node);
  widget_t* widget = widget_find_by_path(win, widget_name, TRUE);
  return_value_if_fail(widget != NULL, RET_BAD_PARAMS);

  while (iter != NULL) {
    value_t v;
    const char* name = conf_node_get_name(iter);
    if (conf_node_get_value(iter, &v) == RET_OK) {
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

  if (TK_STR_IS_NOT_EMPTY(init_json)) {
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

static ret_t remote_ui_dev_info_write(wbuffer_t* wb, remote_ui_dev_info_t* info) {
  return_value_if_fail(wb != NULL && info != NULL, RET_BAD_PARAMS);

  wbuffer_write_string(wb, info->name);
  wbuffer_write_string(wb, info->os);
  wbuffer_write_string(wb, info->version);
  wbuffer_write_string(wb, info->arch);
  wbuffer_write_uint32(wb, info->screen_width);
  wbuffer_write_uint32(wb, info->screen_height);
  wbuffer_write_uint32(wb, info->dpi);

  return RET_OK;
}

static ret_t remote_ui_on_log(void* ctx, tk_log_level_t level, const char* format, va_list ap) {
  remote_ui_service_t* ui = (remote_ui_service_t*)ctx;

  if (ui != NULL && ui->service.destroy == (tk_service_destroy_t)remote_ui_service_destroy) {
    char msg[1024] = {0};
    wbuffer_t* wb = &(ui->service.wb);

    tk_vsnprintf(msg, sizeof(msg) - 1, format, ap);

    wbuffer_rewind(wb);
    wbuffer_write_string(wb, "<log>");
    wbuffer_write_int32(wb, EVT_LOG_MESSAGE);
    wbuffer_write_int8(wb, level);
    wbuffer_write_string(wb, msg);

    if (ui->dispatching) {
      log_debug("ignore log message because busy\n");
    } else {
      tk_service_send_resp(&(ui->service), MSG_CODE_NOTIFY, MSG_DATA_TYPE_BINARY, RET_OK, wb);
    }
  } else {
    remote_ui_service_hook_log(ui, FALSE);
  }

  return RET_OK;
}

static ret_t remote_ui_service_dispatch_impl(remote_ui_service_t* ui, tk_msg_header_t* req,
                                             wbuffer_t* wb) {
  value_t v;
  char buff[1024] = {0};
  tk_msg_header_t resp;
  rbuffer_t rb;
  char local_file[MAX_PATH + 1] = {0};
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);
  return_value_if_fail(req != NULL && wb != NULL, RET_BAD_PARAMS);

  memset(&resp, 0x00, sizeof(resp));
  rbuffer_init(&rb, wb->data, wb->cursor);

  resp.type = req->type;
  switch (req->type) {
    case MSG_CODE_LOGIN: {
      const char* username = NULL;
      const char* password = NULL;

      rbuffer_read_string(&rb, &username);
      rbuffer_read_string(&rb, &password);
      resp.resp_code = remote_ui_service_login(ui, username, password);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case MSG_CODE_LOGOUT: {
      resp.resp_code = remote_ui_service_logout(ui);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      remote_ui_service_hook_log(ui, FALSE);
      break;
    }
    case REMOTE_UI_GET_DEV_INFO: {
      remote_ui_dev_info_t info;
      memset(&info, 0x00, sizeof(info));
      resp.resp_code = remote_ui_service_get_dev_info(ui, &info);
      resp.data_type = MSG_DATA_TYPE_BINARY;
      wbuffer_rewind(wb);
      remote_ui_dev_info_write(wb, &info);
      break;
    }
    case REMOTE_UI_REBOOT: {
      rbuffer_t rb;
      uint32_t reboot_type = REMOTE_UI_REBOOT_DEFAULT;
      rbuffer_init(&rb, wb->data, wb->cursor);

      rbuffer_read_uint32(&rb, &reboot_type);
      resp.resp_code = remote_ui_service_reboot(ui, (remote_ui_reboot_type_t)reboot_type);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case MSG_CODE_UPLOAD_FILE_BEGIN: {
      const char* filename = (const char*)(wb->data);
      filename = path_prepend_app_root(local_file, filename);
      resp.resp_code = tk_service_upload_file(&(ui->service), filename);
      return RET_OK;
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
      const char* name = NULL;
      const char* xml = NULL;
      const char* init_json = NULL;

      rbuffer_read_string(&rb, &name);
      rbuffer_read_string(&rb, &xml);
      rbuffer_read_string(&rb, &init_json);

      resp.resp_code = remote_ui_service_open_window(ui, name, xml, init_json);
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_OPEN_DIALOG: {
      const char* type = NULL;
      const char* title = NULL;
      const char* content = NULL;
      uint32_t duration = 0;

      rbuffer_read_string(&rb, &type);
      rbuffer_read_string(&rb, &title);
      rbuffer_read_string(&rb, &content);
      rbuffer_read_uint32(&rb, &duration);

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
      const char* target = NULL;
      const char* name = NULL;

      value_set_int(&v, 0);
      rbuffer_read_string(&rb, &target);
      rbuffer_read_string(&rb, &name);
      if (rbuffer_read_value(&rb, &v) == RET_OK) {
        resp.resp_code = remote_ui_service_set_prop(ui, target, name, &v);
      } else {
        resp.resp_code = RET_FAIL;
      }
      resp.data_type = MSG_DATA_TYPE_NONE;
      wbuffer_rewind(wb);
      break;
    }
    case REMOTE_UI_GET_PROP: {
      const char* target = NULL;
      const char* name = NULL;

      rbuffer_read_string(&rb, &target);
      rbuffer_read_string(&rb, &name);

      value_set_int(&v, 0);
      resp.resp_code = remote_ui_service_get_prop(ui, target, name, &v);
      resp.data_type = MSG_DATA_TYPE_BINARY;
      wbuffer_rewind(wb);
      wbuffer_write_value(wb, &v);
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
      value_reset(&v);

      break;
    }
    case REMOTE_UI_ON_EVENT: {
      const char* target = NULL;
      uint32_t event_type = 0;

      rbuffer_read_string(&rb, &target);
      rbuffer_read_uint32(&rb, &event_type);

      return remote_ui_service_on_event(ui, target, event_type);
    }
    case REMOTE_UI_OFF_EVENT: {
      const char* target = NULL;
      uint32_t event_type = 0;

      rbuffer_read_string(&rb, &target);
      rbuffer_read_uint32(&rb, &event_type);
      return remote_ui_service_off_event(ui, target, event_type);
    }
    case REMOTE_UI_SEND_EVENT: {
      event_t* e = NULL;
      const char* target = NULL;
      uint32_t event_type = 0;

      rbuffer_read_string(&rb, &target);
      rbuffer_read_uint32(&rb, &event_type);
      switch (event_type) {
        case EVT_CLICK:
        case EVT_POINTER_DOWN:
        case EVT_POINTER_UP:
        case EVT_POINTER_MOVE: {
          pointer_event_t evt;
          int32_t x = 0;
          int32_t y = 0;

          rbuffer_read_int32(&rb, &x);
          rbuffer_read_int32(&rb, &y);
          e = pointer_event_init(&evt, event_type, NULL, x, y);
          break;
        }
        case EVT_KEY_DOWN:
        case EVT_KEY_UP: {
          key_event_t evt;
          uint32_t key = 0;
          rbuffer_read_uint32(&rb, &key);
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
      int32_t x = 0;
      int32_t y = 0;
      widget_t* widget = NULL;
      const char* target = NULL;

      rbuffer_read_string(&rb, &target);
      rbuffer_read_int32(&rb, &x);
      rbuffer_read_int32(&rb, &y);

      widget = remote_ui_service_get_target_widget(ui, target);
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
      uint32_t w = 0;
      uint32_t h = 0;
      widget_t* widget = NULL;
      const char* target = NULL;

      rbuffer_read_string(&rb, &target);
      rbuffer_read_uint32(&rb, &w);
      rbuffer_read_uint32(&rb, &h);

      widget = remote_ui_service_get_target_widget(ui, target);
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
      widget_t* widget = NULL;
      const char* target = NULL;
      const char* xml = NULL;

      rbuffer_read_string(&rb, &target);
      rbuffer_read_string(&rb, &xml);

      widget = remote_ui_service_get_target_widget(ui, target);
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
    case REMOTE_UI_HOOK_LOG: {
      remote_ui_service_hook_log(ui, TRUE);
      resp.resp_code = RET_OK;
      resp.data_type = MSG_DATA_TYPE_NONE;
      break;
    }
    case REMOTE_UI_UNHOOK_LOG: {
      remote_ui_service_hook_log(ui, FALSE);
      resp.resp_code = RET_OK;
      resp.data_type = MSG_DATA_TYPE_NONE;
      break;
    }
    default: {
      resp.resp_code = RET_NOT_IMPL;
      break;
    }
  }

  return tk_service_send_resp(&(ui->service), resp.type, resp.data_type, resp.resp_code, wb);
}

static ret_t remote_ui_service_dispatch(remote_ui_service_t* ui) {
  ret_t ret = RET_OK;
  tk_msg_header_t header;
  return_value_if_fail(ui != NULL && ui->service.io != NULL, RET_BAD_PARAMS);

  memset(&header, 0x00, sizeof(header));
  ret = tk_service_read_req(&(ui->service), &header, &(ui->service.wb));
  return_value_if_fail(ret == RET_OK, ret);

  ui->dispatching = TRUE;
  ret = remote_ui_service_dispatch_impl(ui, &header, &(ui->service.wb));
  ui->dispatching = FALSE;

  return ret;
}

static ret_t remote_ui_service_destroy(remote_ui_service_t* ui) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);

  remote_ui_service_hook_log(ui, FALSE);
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
  return_value_if_fail(io != NULL, NULL);

  service = remote_ui_service_create(io, args);
  return_value_if_fail(service != NULL, NULL);

  timer_add(remote_ui_dispatch_timer, service, 20);

  return service;
}

ret_t remote_ui_service_hook_log(remote_ui_service_t* ui, bool_t hook) {
  return_value_if_fail(ui != NULL, RET_BAD_PARAMS);

  if (hook) {
    log_set_hook(remote_ui_on_log, ui);
  } else {
    log_set_hook(NULL, NULL);
  }

  return RET_OK;
}
