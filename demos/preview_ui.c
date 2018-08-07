/**
 * File:   demo_main.c
 * Author: AWTK Develop Team
 * Brief:  demo main
 *
 * Copyright (c) 2018 - 2018  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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

#include "base/fs.h"
#include "base/mem.h"
#include "base/utils.h"
#include "resource.h"

#include "ui_loader/ui_loader_xml.h"
#include "ui_loader/ui_loader_default.h"
#include "ui_loader/ui_builder_default.h"

widget_t* preview_ui(const char* filename) {
  uint32_t size = 0;
  char name[NAME_LEN + 1];
  ui_builder_t* builder = NULL;
  uint8_t* content = (uint8_t*)fs_read_file(filename, &size);
  ui_loader_t* loader = strstr(filename, ".bin") != NULL ? default_ui_loader() : xml_ui_loader();

  filename_to_name(filename, name, NAME_LEN);
  builder = ui_builder_default(name);
  printf("preview %s\n", filename);
  return_value_if_fail(content != NULL, NULL);
  ui_loader_load(loader, content, size, builder);
  TKMEM_FREE(content);

  return builder->root;
}

#if defined(WIN32)
#include <windows.h>
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinstance, LPSTR lpcmdline, int ncmdshow) {
  int argc = 1;
  const char* filename = (lpcmdline && *lpcmdline) ? lpcmdline : "../tools/ui_gen/window1.xml";
#else
#include "base/mem.h"
int main(int argc, char* argv[]) {
  const char* filename = argc == 1 ? "../tools/ui_gen/window1.xml" : argv[1];
#endif
  widget_t* win = NULL;

  tk_init(320, 480);
  resource_init();
  tk_ext_widgets_init();

  win = preview_ui(filename);
  tk_run();

  return 0;
}
