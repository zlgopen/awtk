/**
 * File:   demo_main.c
 * Author: AWTK Develop Team
 * Brief:  demo main
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
#include "ext_widgets.h"
#include "tkc/fs.h"
#include "tkc/mem.h"
#include "tkc/utils.h"
#include "base/timer.h"
#include "assets.h"

#include "ui_loader/ui_loader_xml.h"
#include "ui_loader/ui_loader_default.h"
#include "ui_loader/ui_builder_default.h"

static ret_t on_timer(const timer_info_t* info) {
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

  if (builder->root != NULL && !(builder->root->vt->is_window)) {
    widget_t* win = window_create(NULL, 0, 0, 0, 0);
    widget_add_child(win, builder->root);
    widget_layout(win);
  }

  if (is_bin) {
    TKMEM_FREE(content);
  } else {
    str_reset(&s);
  }

  timer_add(on_timer, builder->root, 1000);

  return builder->root;
}

#include "tkc/path.h"

#define DEFAULT_UI "./demos/assets/raw/ui/main.xml"
#if defined(WIN32)
#include <windows.h>

#define MAX_ARGV 5
void command_line_to_argv(char* lpcmdline, const char* argv[MAX_ARGV], int32_t* argc) {
  int32_t i = 1;
  char* p = lpcmdline;

  argv[0] = "preview.exe";
  for (i = 1; i < MAX_ARGV; i++) {
    argv[i] = p;
    p = strchr(p, ' ');
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

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
  str_t str;
  int argc = 1;
  char* argv[MAX_ARGV];
  str_init(&str, 1024);
  str_set(&str, lpcmdline);
  command_line_to_argv(str.str, argv, &argc);
#else
#include "tkc/mem.h"
int main(int argc, char* argv[]) {
#endif
  int32_t w = 320;
  int32_t h = 480;
  const char* filename = DEFAULT_UI;

  if (argc > 1) {
    filename = argv[1];
  } else {
    log_debug("%s ui_file [w] [h]\n", argv[0]);
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

  log_debug("%s %s %d %d\n", argv[0], argv[1], w, h);
  if (path_exist("./demos/assets/raw")) {
    tk_init(w, h, APP_SIMULATOR, NULL, "./demos");
  } else if (path_exist("./assets/raw")) {
    tk_init(w, h, APP_SIMULATOR, NULL, "./");
  } else {
    assert(!"not found assets!");
  }

  assets_init();
  tk_ext_widgets_init();

  preview_ui(filename);
  tk_run();

  return 0;
}
