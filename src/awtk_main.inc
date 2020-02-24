/**
 * File:   awtk_main.c
 * Author: AWTK Develop Team
 * Brief:  awtk main
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

BEGIN_C_DECLS
extern ret_t assets_init();
END_C_DECLS

extern ret_t application_init(void);
extern ret_t application_exit(void);

#ifndef LCD_WIDTH
#define LCD_WIDTH 320
#endif/*LCD_WIDTH*/

#ifndef LCD_HEIGHT
#define LCD_HEIGHT 480
#endif/*LCD_HEIGHT*/

#ifndef AWTK_APP_TYPE
#define AWTK_APP_TYPE APP_SIMULATOR
#endif/*AWTK_APP_TYPE*/

#ifndef GLOBAL_INIT
#define GLOBAL_INIT()
#endif/*GLOBAL_INIT*/

#ifndef GLOBAL_EXIT
#define GLOBAL_EXIT()
#endif/*GLOBAL_EXIT*/

#ifdef USE_GUI_MAIN
int gui_app_start(int lcd_w, int lcd_h) {
  tk_init(lcd_w, lcd_h, APP_MOBILE, NULL, NULL);
#elif defined(IOS)
#include <SDL.h>
int main(int argc, char* argv[]) {
  int32_t lcd_w = 0;
  int32_t lcd_h = 0;

  tk_init(lcd_w, lcd_h, APP_MOBILE, NULL, NULL);
  system_info_set_default_font(system_info(), "default_full");
#elif defined(MOBILE_APP) || defined(ANDROID)
int SDL_main(int argc, char* argv[]) {
  int32_t lcd_w = LCD_WIDTH;
  int32_t lcd_h = LCD_HEIGHT;

  tk_init(lcd_w, lcd_h, APP_MOBILE, NULL, NULL);
  system_info_set_default_font(system_info(), "default_full");
#elif defined(WIN32)
#include <windows.h>
#define MAX_ARGV 7

static void command_line_to_argv(char* lpcmdline, const char* argv[MAX_ARGV], int32_t* argc) {
  int32_t i = 1;
  char* p = lpcmdline;

  argv[0] = "awtk.exe";

  if (!*p) {
    argv[1] = NULL;
    return;
  }

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
  char* argv[MAX_ARGV];
  int32_t lcd_w = LCD_WIDTH;
  int32_t lcd_h = LCD_HEIGHT;

  str_init(&str, 1024);
  str_from_wstr(&str, lpcmdline);
  command_line_to_argv(str.str, argv, &argc);

  TK_ENABLE_CONSOLE();
  
#ifdef ON_CMD_LINE
  ON_CMD_LINE(argc, argv);
#else
  if (argc >= 2) {
    lcd_w = tk_atoi(argv[1]);
  }
  if (argc >= 3) {
    lcd_h = tk_atoi(argv[2]);
  }
#endif/*ON_CMD_LINE*/

  tk_init(lcd_w, lcd_h, APP_SIMULATOR, NULL, NULL);
#else
int main(int argc, char* argv[]) {
  int32_t lcd_w = LCD_WIDTH;
  int32_t lcd_h = LCD_HEIGHT;
#ifdef ON_CMD_LINE
  ON_CMD_LINE(argc, argv);
#else
  if (argc >= 2) {
    lcd_w = tk_atoi(argv[1]);
  }
  if (argc >= 3) {
    lcd_h = tk_atoi(argv[2]);
  }
#endif/*ON_CMD_LINE*/

  tk_init(lcd_w, lcd_h, APP_SIMULATOR, NULL, NULL);
#endif

#if defined(WITH_LCD_PORTRAIT)
  if (lcd_w > lcd_h) {
    tk_set_lcd_orientation(LCD_ORIENTATION_90);
  }
#endif /*WITH_LCD_PORTRAIT*/

#ifdef WITH_LCD_LANDSCAPE
  if (lcd_w < lcd_h) {
    tk_set_lcd_orientation(LCD_ORIENTATION_90);
  }
#endif /*WITH_LCD_PORTRAIT*/

#ifdef WITH_FS_RES
  system_info_set_default_font(system_info(), "default_full");
#endif /*WITH_FS_RES*/

  assets_init();
  tk_ext_widgets_init();
  log_set_log_level(LOG_LEVEL_INFO);
  log_info("Build at: %s %s\n", __DATE__, __TIME__);

#ifdef ENABLE_CURSOR
  window_manager_set_cursor(window_manager(), "cursor");
#endif /*ENABLE_CURSOR*/

  GLOBAL_INIT();
  application_init();
  tk_run();
  application_exit();
  GLOBAL_EXIT();

#ifdef WIN32
  str_reset(&str);
#endif/*WIN32*/

  return 0;
}

