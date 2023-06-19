/**
 * File:   preview_ui.c
 * Author: AWTK Develop Team
 * Brief:  preview_ui
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2018-02-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifdef WITH_VLD
#include "vld.h"
#endif

#include "awtk.h"
#include "tkc/dl.h"
#include "tkc/log.h"
#include "base/timer.h"

#include "ui_loader/ui_loader_xml.h"
#include "ui_loader/ui_loader_default.h"
#include "ui_loader/ui_builder_default.h"

#define DEFAULT_UI "./demos/assets/raw/ui/main.xml"

#define MAX_ARGV 12

#ifndef APP_RES_ROOT_DEFAULT_VALUE
#define APP_RES_ROOT_DEFAULT_VALUE NULL
#endif /*APP_RES_ROOT_DEFAULT_VALUE*/

static const char* s_ui = DEFAULT_UI;
static const char* s_system_bar = NULL;
static const char* s_system_bar_bottom = NULL;
static const char* s_res_root = APP_RES_ROOT_DEFAULT_VALUE;
static const char* s_plugins_path = NULL;
static const char* s_render_mode = "OpenGL";
static const char* s_language = NULL;
static const char* s_theme = NULL;
static const char* s_log_level = NULL;
static const char* s_fps = NULL;

#undef APP_RES_ROOT  // 以便可以通过命令行参数指定res目录

#if defined(WIN32) && !defined(NDEBUG)
#define SET_ENABLE_CONSOLE()                     \
  if (tk_str_icmp(key, "enable_console") == 0) { \
    enable_console = tk_atob(val + 1);           \
  } else {                                       \
    log_debug(usage, argv[0]);                   \
  }
#else
#define SET_ENABLE_CONSOLE() \
  { log_debug(usage, argv[0]); }
#endif /*WIN32 && !NDEBUG*/

#define ON_CMD_LINE(argc, argv)                                                   \
  {                                                                               \
    const char* usage =                                                           \
        "Usage: %s ui=xxx [lcd_w=800] [lcd_h=480] [res_root=xxx] "                \
        "[language=xxx] [theme=xxx] [system_bar=xxx] [bottom_system_bar=xxx] "    \
        "[plugins_path=xxx] [render_mode=xxx] [enable_console=xxx]\n";            \
    if (argc >= 2) {                                                              \
      char key[TK_NAME_LEN + 1];                                                  \
      int i = 1;                                                                  \
                                                                                  \
      for (i = 1; i < argc; i++) {                                                \
        const char* p = argv[i];                                                  \
        const char* val = strstr(p, "=");                                         \
        uint32_t len = (uint32_t)(val - p);                                       \
                                                                                  \
        if (*(argv[i]) == '\0') {                                                 \
          continue;                                                               \
        }                                                                         \
                                                                                  \
        if (val == NULL || val == p || *(val + 1) == '\0' || len > TK_NAME_LEN) { \
          log_debug(usage, argv[0]);                                              \
          exit(0);                                                                \
        }                                                                         \
                                                                                  \
        tk_strncpy(key, p, len);                                                  \
        key[len] = '\0';                                                          \
                                                                                  \
        if (tk_str_icmp(key, "res_root") == 0) {                                  \
          s_res_root = val + 1;                                                   \
        } else if (tk_str_icmp(key, "ui") == 0) {                                 \
          s_ui = val + 1;                                                         \
        } else if (tk_str_icmp(key, "system_bar") == 0) {                         \
          s_system_bar = val + 1;                                                 \
        } else if (tk_str_icmp(key, "bottom_system_bar") == 0) {                  \
          s_system_bar_bottom = val + 1;                                          \
        } else if (tk_str_icmp(key, "lcd_w") == 0) {                              \
          lcd_w = tk_atoi(val + 1);                                               \
        } else if (tk_str_icmp(key, "lcd_h") == 0) {                              \
          lcd_h = tk_atoi(val + 1);                                               \
        } else if (tk_str_icmp(key, "plugins_path") == 0) {                       \
          s_plugins_path = val + 1;                                               \
        } else if (tk_str_icmp(key, "render_mode") == 0) {                        \
          s_render_mode = val + 1;                                                \
        } else if (tk_str_icmp(key, "language") == 0) {                           \
          s_language = val + 1;                                                   \
        } else if (tk_str_icmp(key, "theme") == 0) {                              \
          s_theme = val + 1;                                                      \
        } else if (tk_str_icmp(key, "log_level") == 0) {                          \
          s_log_level = val + 1;                                                  \
        } else if (tk_str_icmp(key, "fps") == 0) {                                \
          s_fps = val + 1;                                                        \
        } else {                                                                  \
          SET_ENABLE_CONSOLE()                                                    \
        }                                                                         \
      }                                                                           \
                                                                                  \
    } else {                                                                      \
      log_debug(usage, argv[0]);                                                  \
      exit(0);                                                                    \
    }                                                                             \
  }

static ret_t get_default_res_root(char path[MAX_PATH + 1]) {
#ifdef APP_RES_DEFAULT_ROOT
  tk_strncpy(path, APP_RES_DEFAULT_ROOT, strlen(APP_RES_DEFAULT_ROOT));
  return RET_OK;
#else
  char app_root[MAX_PATH + 1];
  if (path_app_root(app_root) == RET_OK) {
    return path_build(path, MAX_PATH, app_root, "res", NULL);
  }
  return RET_FAIL;
#endif
}

typedef ret_t (*plugin_register_t)(void);
typedef const char* (*plugin_supported_render_mode_t)(void);
static ret_t plugins_dll_register(const char* dirname, const char* render_mode) {
  str_t funcname;
  fs_item_t item;
  char path[MAX_PATH + 1] = {0};
  fs_dir_t* dir = fs_open_dir(os_fs(), dirname);
  return_value_if_fail(dir != NULL, RET_FAIL);

  str_init(&funcname, 0);

  while (fs_dir_read(dir, &item) != RET_FAIL) {
    if (item.is_dir) {
      if (!tk_str_eq(item.name, ".") && !tk_str_eq(item.name, "..")) {
        path_build(path, MAX_PATH, dirname, item.name, NULL);
        plugins_dll_register(path, render_mode);
      }
    } else {
      bool_t should_try_register = TRUE;
      tk_dl_t* dl = NULL;
      plugin_register_t register_func;
      plugin_supported_render_mode_t render_mode_func;

#ifdef WIN32
      if (item.is_reg_file && tk_str_end_with(item.name, ".dll")) {
        str_set(&funcname, item.name);
        str_replace(&funcname, ".dll", "");
      } else {
        continue;
      }
#elif defined LINUX
      if (item.is_reg_file && tk_str_end_with(item.name, ".so") &&
          tk_str_start_with(item.name, "lib")) {
        str_set(&funcname, item.name);
        str_replace(&funcname, "lib", "");
        str_replace(&funcname, ".so", "");
      } else {
        continue;
      }
#elif defined MACOS
      if (item.is_reg_file && tk_str_end_with(item.name, ".dylib") &&
          tk_str_start_with(item.name, "lib")) {
        str_set(&funcname, item.name);
        str_replace(&funcname, "lib", "");
        str_replace(&funcname, ".dylib", "");
      } else {
        continue;
      }
#else
      log_debug("Not supported platform.\n");
      return RET_FAIL;
#endif

      path_build(path, MAX_PATH, dirname, item.name, NULL);
      dl = tk_dl_open(path);
      if (dl == NULL) {
#ifdef WIN32
        log_debug("Open dynamic library faild: %s, error code: %d!\n", path, GetLastError());
#else
        log_debug("Open dynamic library faild: %s!\n", path);
#endif
        continue;
      }

      str_append(&funcname, "_supported_render_mode");
      render_mode_func = (plugin_supported_render_mode_t)(tk_dl_sym(dl, funcname.str));
      if (render_mode_func != NULL) {
        const char* modes = render_mode_func();
        should_try_register = strstr(modes, render_mode) != NULL;
        if (!should_try_register) {
          log_debug("Not supported render mode: %s %s!\n", path, render_mode);
        }
      }
      str_replace(&funcname, "_supported_render_mode", "");

      if (should_try_register) {
        str_append(&funcname, "_register");
        register_func = (plugin_register_t)tk_dl_sym(dl, funcname.str);
        if (register_func != NULL) {
          register_func();
        } else {
          log_debug("Not found function: %s %s!\n", path, funcname.str);
        }
      }
    }
  }

  str_reset(&funcname);
  return RET_OK;
}

static ret_t plugins_register(void) {
  const char* path = s_plugins_path;
  if (path != NULL && *path != '\0') {
    return_value_if_fail(dir_exist(path), RET_BAD_PARAMS);
    return plugins_dll_register(path, s_render_mode);
  }
  return RET_OK;
}

static ret_t refresh_in_timer(const timer_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);

  widget_invalidate(widget, NULL);

  return RET_REPEAT;
}

static ret_t application_on_launch(void) {
  // 当程序初始化完成时调用，全局只触发一次。

  if (s_language != NULL && *s_language != '\0') {
    const char* p = strstr(s_language, "_");
    if (p != NULL) {
      char language[TK_NAME_LEN + 1] = {0};
      tk_strncpy(language, s_language, tk_min((size_t)(p - s_language), sizeof(language)));
      locale_info_change(locale_info(), language, p + 1);
    } else {
      locale_info_change(locale_info(), s_language, "");
    }
  }

  widget_set_style_str(window_manager(), "bg_color", "white");

  return RET_OK;
}

static ret_t application_on_exit(void) {
  // 当程序退出时调用，全局只触发一次。

  return RET_OK;
}

static widget_t* preview_ui(const char* filename) {
  str_t s;
  uint32_t size = 0;
  widget_t* root = NULL;
  char name[TK_NAME_LEN + 1];
  ui_builder_t* builder = NULL;
  uint8_t* content = NULL;
  bool_t is_bin = strstr(filename, ".bin") != NULL;
  ui_loader_t* loader = is_bin ? default_ui_loader() : xml_ui_loader();

  str_init(&s, 0);
  if (is_bin) {
    content = (uint8_t*)file_read(filename, &size);
  } else {
    xml_file_expand_read(filename, &s);
    content = (uint8_t*)s.str;
    size = s.size;
  }

  filename_to_name(filename, name, TK_NAME_LEN);
  builder = ui_builder_default_create(name);
  printf("preview %s\n", filename);
  return_value_if_fail(content != NULL, NULL);
  ui_loader_load(loader, content, size, builder);

  if (builder->root == NULL) {
    builder->root = label_create(NULL, 10, 10, 100, 30);
    widget_set_text_utf8(builder->root, "not found ui file!");
  }

  if (builder->root != NULL && !(builder->root->vt->is_window)) {
    widget_t* win = window_create(NULL, 0, 0, 0, 0);
    widget_add_child(win, builder->root);
    widget_layout(win);

    timer_add(refresh_in_timer, builder->root, 1000);
  }

  if (is_bin) {
    TKMEM_FREE(content);
  } else {
    str_reset(&s);
  }

  root = builder->root;
  ui_builder_destroy(builder);

  return root;
}

static ret_t application_init(void) {
  plugins_register();
  application_on_launch();

  if (s_system_bar != NULL && *s_system_bar != '\0') {
    window_open(s_system_bar);
  }

  if (s_system_bar_bottom != NULL && *s_system_bar_bottom != '\0') {
    window_open(s_system_bar_bottom);
  }
  preview_ui(s_ui);

  return RET_OK;
}

static ret_t application_exit() {
  application_on_exit();
  log_debug("application_exit\n");

  return RET_OK;
}

static ret_t build_asset_dir_custom(void* ctx, char* path, uint32_t size, const char* theme,
                                    const char* ratio, const char* subpath) {
  return_value_if_fail(path_build(path, size, s_res_root, theme, subpath, ratio, NULL) == RET_OK,
                       RET_FAIL);
  if (path != NULL && *path != '\0') {
    return RET_OK;
  } else {
    return RET_FAIL;
  }
}

ret_t assets_init(void) {
#ifdef WITH_FS_RES
  assets_manager_t* am = assets_manager();
  const char* res_root = NULL;
  bool_t run_default = FALSE;

  if (s_theme != NULL) {
    assets_manager_set_theme(am, s_theme);
  }

  TKMEM_FREE(am->res_root);
  am->res_root = NULL;

  assets_manager_set_custom_build_asset_dir(am, build_asset_dir_custom, NULL);

  char path[MAX_PATH + 1];

  if (s_res_root != NULL && *s_res_root != '\0') {
    assets_manager_set_res_root(am, s_res_root);
  }

  res_root = assets_manager_get_res_root(am);
  tk_snprintf(path, sizeof(path), "%s/assets/default/raw/ui", res_root);
  if (!dir_exist(path)) {
    run_default = TRUE;
  }

  if (run_default) {
    char path[MAX_PATH + 1];
    if (get_default_res_root(path) == RET_OK) {
      assets_manager_set_res_root(am, path);
    }
  }

  assets_manager_preload(am, ASSET_TYPE_FONT, "default");
  assets_manager_preload(am, ASSET_TYPE_STYLE, "default");
#endif
  tk_init_assets();
  return RET_OK;
}

static ret_t global_init(void) {
  log_level_t log_level =
      s_log_level != NULL && *s_log_level != '\0' ? tk_atoi(s_log_level) : LOG_LEVEL_DEBUG;

  log_set_log_level(log_level);

  return RET_OK;
}

static ret_t global_exit(void) {
  return RET_OK;
}

#define GLOBAL_INIT() global_init()
#define GLOBAL_EXIT() global_exit()
#define APP_NAME "awtk_preview"

#include "awtk_main.inc"
