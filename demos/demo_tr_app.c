/**
 * File:   demo_tr_app.c
 * Author: AWTK Develop Team
 * Brief:  text translation
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
 * 2018-05-03 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/timer.h"
#include "base/enums.h"
#include "base/button.h"
#include "base/locale_info.h"
#include "base/check_button.h"
#include "base/dialog.h"
#include "base/image.h"
#include "base/image_manager.h"
#include "base/label.h"
#include "base/mem.h"
#include "base/progress_bar.h"
#include "base/utils.h"
#include "base/utf8.h"
#include "base/window.h"
#include "base/slider.h"
#include "base/group_box.h"

static ret_t change_locale(void* ctx, event_t* e) {
  char country[3];
  char language[3];
  const char* str = (const char*)ctx;

  widget_t* widget = WIDGET(e->target);
  if (widget_get_value(widget)) {
    strncpy(language, str, 2);
    strncpy(country, str + 3, 2);
    locale_info_change(locale_info(), language, country);
  }

  return RET_OK;
}

static ret_t on_locale_changed(void* ctx, event_t* e) {
  (void)ctx;
  (void)e;
  log_debug("locale_infod change: %s_%s\n", locale_info()->language, locale_info()->country);

  return RET_OK;
}

ret_t application_init() {
  widget_t* ok = NULL;
  widget_t* cancel = NULL;
  widget_t* radio_button = NULL;
  widget_t* win = window_create(NULL, 0, 0, 0, 0);

  widget_on(win, EVT_LOCALE_CHANGED, on_locale_changed, win);

  ok = button_create(win, 10, 5, 80, 30);
  widget_set_tr_text(ok, "ok");

  cancel = button_create(win, 100, 5, 80, 30);
  widget_set_tr_text(cancel, "cancel");

  radio_button = check_button_create_radio(win, 10, 200, 80, 30);
  widget_set_tr_text(radio_button, "English");
  widget_on(radio_button, EVT_VALUE_CHANGED, change_locale, "en_US");
  widget_set_value(radio_button, 1);

  radio_button = check_button_create_radio(win, 100, 200, 80, 30);
  widget_set_tr_text(radio_button, "Chinese");
  widget_on(radio_button, EVT_VALUE_CHANGED, change_locale, "zh_CN");

  return RET_OK;
}
