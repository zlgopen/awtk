/**
 * File:   preview_ui.c
 * Author: AWTK Develop Team
 * Brief:  preview ui
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

#include "awtk.h"
#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "assets.h"
#include "tkc/utf8.h"

#include "ui_loader/ui_loader_xml.h"
#include "ui_loader/ui_loader_default.h"
#include "ui_loader/ui_builder_default.h"

#if defined(WITH_SDL) && !defined(MINGW)
#include <SDL.h>
#endif

static ret_t refresh_in_timer(const timer_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);

  widget_invalidate(widget, NULL);

  return RET_REPEAT;
}

widget_t* preview_ui(const char* filename) {
  str_t s;
  uint32_t size = 0;
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
  builder = ui_builder_default(name);
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

  return builder->root;
}

#include "tkc/path.h"

#define DEFAULT_UI "./demos/assets/raw/ui/main.xml"
#if defined(WIN32) && !defined(MINGW)
#include <windows.h>

#define MAX_ARGV 7
void command_line_to_argv(char* lpcmdline, const char* argv[MAX_ARGV], int32_t* argc) {
  int32_t i = 1;
  char* p = lpcmdline;

  argv[0] = "preview.exe";
  for (i = 1; i < MAX_ARGV; i++) {
    argv[i] = p;
    if (*p == '\"') {
      argv[i] = p + 1;
      p = strchr(p + 1, '\"');
      if (p == NULL) break;
      *p++ = '\0';
      if (*p == 0) break;
    } else {
      p = strchr(p, ' ');
    }
    if (p == NULL) {
      break;
    }

    while (*p == ' ') {
      *p++ = '\0';
    }
  }
  *argc = i + 1;

  return;
}

int WINAPI wWinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPWSTR lpcmdline, int ncmdshow) {
  str_t str;
  int argc = 1;
  int32_t len = wcslen(lpcmdline) * 6;
  char* utf8_line = (char*)TKMEM_ALLOC(len);
  tk_utf8_from_utf16(lpcmdline, utf8_line, len);
  char* argv[MAX_ARGV];
  str_init(&str, 1024);
  str_set(&str, utf8_line);
  command_line_to_argv(str.str, argv, &argc);
#if defined(WIN32)
  TK_ENABLE_CONSOLE();
#endif /*WIN32*/
#else
#include "tkc/mem.h"
int main(int argc, char* argv[]) {
#endif
  int32_t w = 320;
  int32_t h = 480;
  bool_t have_lang = FALSE;
  const char* filename = DEFAULT_UI;
  const char* theme = "default";
  char res_root[MAX_PATH + 1];
  char country[3];
  char language[3];
  memset(res_root, 0x00, sizeof(res_root));

  if (argc > 1) {
    filename = argv[1];
  } else {
    log_debug("%s ui_file [w] [h] [res root] [language] [theme]\n", argv[0]);
#ifdef WIN32
    assert(!"no ui file provided");
#endif /*WIN32*/
    exit(0);
  }

  if (argc > 2) {
    int32_t ww = atoi(argv[2]);
    if (ww > 0) {
      w = ww;
    }
  }

  if (argc > 3) {
    int32_t hh = atoi(argv[3]);
    if (h > 0) {
      h = hh;
    }
  }

  if (argc > 4) {
    memcpy(res_root, argv[4], strlen(argv[4]));
  } else {
    char* p = NULL;
    memcpy(res_root, filename, strlen(filename));
    p = strstr(res_root, "assets");
    if (p != NULL) {
      *p = '\0';
    }
  }
  log_debug("%s %s %d %d %s\n", argv[0], argv[1], w, h, res_root);

  if (argc > 5) {
    char str[MAX_PATH + 1];
    memset(str, 0x00, sizeof(str));
    memcpy(str, argv[5], strlen(argv[5]));
    tk_strncpy(language, str, 2);
    tk_strncpy(country, str + 3, 2);
    have_lang = TRUE;
  }

  if (argc > 6) {
    theme = argv[6];
  }

  tk_init(w, h, APP_SIMULATOR, NULL, res_root);
  assets_manager_set_res_root(assets_manager(), system_info()->app_root);
#ifdef WITH_FS_RES
  system_info_set_default_font(system_info(), "default_full");
#endif /*WITH_FS_RES*/

  assets_init();
  tk_ext_widgets_init();

  if (!tk_str_eq(theme, "default")) {
    widget_set_theme(window_manager(), theme);
  }

  preview_ui(filename);
  if (have_lang) {
    locale_info_change(locale_info(), language, country);
  }
  widget_set_style_str(window_manager(), "bg_color", "white");
  tk_run();

#if defined(WIN32) && !defined(MINGW)
  TKMEM_FREE(utf8_line);
#endif

  return 0;
}
