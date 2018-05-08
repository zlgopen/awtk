/**
 * File:   demo_main.c
 * Author: Li XianJing <xianjimli@hotmail.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "resource.h"
#include "tk.h"
#include "base/mem.h"
#include "ui_loader/ui_loader_xml.h"
#include "ui_loader/ui_builder_default.h"

static uint32_t s_heap_mem[2048];

char* read_file(const char* file_name, uint32_t* length) {
  struct stat st = {0};
  if (stat(file_name, &st)) {
    return NULL;
  } else {
    char* buffer = (char*)TKMEM_ALLOC(st.st_size + 1);
    FILE* fp = fopen(file_name, "rb");
    fread(buffer, 1, st.st_size, fp);
    fclose(fp);
    buffer[st.st_size] = '\0';
    *length = st.st_size;

    return buffer;
  }
}

widget_t* prefix_xml_ui(const char* filename) {
  uint32_t size = 0;
  uint8_t* content = (uint8_t*)read_file(filename, &size);
  ui_loader_t* loader = xml_ui_loader();
  ui_builder_t* builder = ui_builder_default();
  printf("preview %s\n", filename);
  return_value_if_fail(content != NULL, NULL);
  ui_loader_load(loader, content, size, builder);
  free(content);

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

  tk_init(320, 480, s_heap_mem, sizeof(s_heap_mem));
  resource_init();
  win = prefix_xml_ui(filename);
  tk_run();
  widget_destroy(win);

  return 0;
}
