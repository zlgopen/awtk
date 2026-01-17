/**
 * File:   demo_progress_circle.c
 * Author: AWTK Develop Team
 * Brief:  演示环形进度条控件的脏区优化
 *
 * Copyright (c) 2018 - 2025 Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2025-01-17 Li XianJing <xianjimli@hotmail.com> created 
 *
 */

#include "awtk.h"
#include <stdlib.h>
#include "ext_widgets/progress_circle/progress_circle.h"

static widget_t* s_progress_circle1 = NULL; /* 使用 progress_circle_set_value */
static widget_t* s_progress_circle2 = NULL; /* 使用 animate:value */
static float_t s_value1 = 0;
static float_t s_value2 = 0;

static ret_t on_timer_update(const timer_info_t* timer) {
  /* 更新第一个进度条：使用 progress_circle_set_value */
  s_value1 = (s_value1 + 2.0f);
  if (s_value1 > 100) {
    s_value1 = 0;
  }
  if (s_progress_circle1 != NULL) {
    progress_circle_set_value(s_progress_circle1, s_value1);
  }

  /* 更新第二个进度条：使用 animate:value */
  s_value2 = (s_value2 + 2.0f);
  if (s_value2 > 100) {
    s_value2 = 0;
  }
  if (s_progress_circle2 != NULL) {
    widget_set_prop_float(s_progress_circle2, "animate:value", s_value2);
  }

  return RET_REPEAT;
}

static ret_t on_button_click(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  const char* name = widget_get_prop_str(widget, WIDGET_PROP_NAME, NULL);

  if (name == NULL) {
    return RET_OK;
  }

  if (tk_str_eq(name, "btn_set_value1")) {
    /* 直接设置第一个进度条的值 */
    if (s_progress_circle1 != NULL) {
      float_t value = (float_t)(random() % 100);
      log_info("Button clicked: Set progress_circle1 value to %.2f using progress_circle_set_value\n",
               value);
      progress_circle_set_value(s_progress_circle1, value);
      s_value1 = value;
    }
  } else if (tk_str_eq(name, "btn_set_value2")) {
    /* 使用动画设置第二个进度条的值 */
    if (s_progress_circle2 != NULL) {
      float_t value = (float_t)(random() % 100);
      log_info("Button clicked: Set progress_circle2 value to %.2f using animate:value\n", value);
      widget_set_prop_float(s_progress_circle2, "animate:value", value);
      s_value2 = value;
    }
  } else if (tk_str_eq(name, "btn_reset")) {
    /* 重置两个进度条 */
    if (s_progress_circle1 != NULL) {
      progress_circle_set_value(s_progress_circle1, 0);
      s_value1 = 0;
    }
    if (s_progress_circle2 != NULL) {
      widget_set_prop_float(s_progress_circle2, "animate:value", 0);
      s_value2 = 0;
    }
    log_info("Button clicked: Reset both progress circles\n");
  }

  return RET_OK;
}

#define LCD_WIDTH 480
#define LCD_HEIGHT 360

ret_t application_init() {
  widget_t* win = NULL;
  widget_t* label1 = NULL;
  widget_t* label2 = NULL;
  widget_t* btn1 = NULL;
  widget_t* btn2 = NULL;
  widget_t* btn_reset = NULL;

  /* 创建窗口 */
  win = window_create(NULL, 0, 0, LCD_WIDTH, LCD_HEIGHT);
  widget_set_name(win, "main");
  widget_set_style_str(win, "bg_color", "#F0F0F0");

  /* 创建标题标签 */
  label1 = label_create(win, 20, 20, 400, 30);
  widget_set_text(label1, L"环形进度条脏区优化演示");
  widget_set_style_str(label1, "font_size", "20");
  widget_set_style_str(label1, "text_color", "#000000");
  widget_set_style_str(label1, "text_align_h", "center");

  /* 创建说明标签 */
  label2 = label_create(win, 20, 50, 400, 30);
  widget_set_text(label2, L"查看日志输出，观察两种方式的脏区差异");
  widget_set_style_str(label2, "font_size", "14");
  widget_set_style_str(label2, "text_color", "#666666");
  widget_set_style_str(label2, "text_align_h", "center");

  /* 创建第一个进度条：使用 progress_circle_set_value */
  s_progress_circle1 = progress_circle_create(win, 80, 100, 120, 120);
  widget_set_name(s_progress_circle1, "progress_circle1");
  progress_circle_set_max(s_progress_circle1, 100);
  progress_circle_set_value(s_progress_circle1, 0);
  progress_circle_set_line_width(s_progress_circle1, 8);
  progress_circle_set_start_angle(s_progress_circle1, -90);
  progress_circle_set_show_text(s_progress_circle1, TRUE);
  widget_set_style_str(s_progress_circle1, "fg_color", "#4A90E2");
  widget_set_style_str(s_progress_circle1, "text_color", "#333333");

  /* 创建第一个进度条的标签 */
  label1 = label_create(win, 80, 230, 120, 30);
  widget_set_text(label1, L"progress_circle_set_value");
  widget_set_style_str(label1, "font_size", "12");
  widget_set_style_str(label1, "text_color", "#000000");
  widget_set_style_str(label1, "text_align_h", "center");

  /* 创建第一个进度条的按钮 */
  btn1 = button_create(win, 80, 260, 120, 30);
  widget_set_name(btn1, "btn_set_value1");
  widget_set_text(btn1, L"Rand Value");
  widget_on(btn1, EVT_CLICK, on_button_click, NULL);

  /* 创建第二个进度条：使用 animate:value */
  s_progress_circle2 = progress_circle_create(win, 240, 100, 120, 120);
  widget_set_name(s_progress_circle2, "progress_circle2");
  progress_circle_set_max(s_progress_circle2, 100);
  widget_set_prop_float(s_progress_circle2, WIDGET_PROP_VALUE, 0);
  progress_circle_set_line_width(s_progress_circle2, 8);
  progress_circle_set_start_angle(s_progress_circle2, -90);
  progress_circle_set_show_text(s_progress_circle2, TRUE);
  widget_set_style_str(s_progress_circle2, "fg_color", "#50C878");
  widget_set_style_str(s_progress_circle2, "text_color", "#333333");

  /* 创建第二个进度条的标签 */
  label2 = label_create(win, 240, 230, 120, 30);
  widget_set_text(label2, L"animate:value");
  widget_set_style_str(label2, "font_size", "12");
  widget_set_style_str(label2, "text_color", "#000000");
  widget_set_style_str(label2, "text_align_h", "center");

  /* 创建第二个进度条的按钮 */
  btn2 = button_create(win, 240, 260, 120, 30);
  widget_set_name(btn2, "btn_set_value2");
  widget_set_text(btn2, L"Rand Value");
  widget_on(btn2, EVT_CLICK, on_button_click, NULL);

  /* 创建重置按钮 */
  btn_reset = button_create(win, 160, 310, 120, 35);
  widget_set_name(btn_reset, "btn_reset");
  widget_set_text(btn_reset, L"Reset");
  widget_set_style_str(btn_reset, "font_size", "16");
  widget_on(btn_reset, EVT_CLICK, on_button_click, NULL);

  /* 设置日志级别为 DEBUG，以便查看脏区日志 */
  log_set_log_level(LOG_LEVEL_DEBUG);
  log_info("========================================\n");
  log_info("环形进度条脏区优化演示程序启动\n");
  log_info("========================================\n");
  log_info("观察日志输出，可以看到两种方式的脏区都是值变化区域\n");
  log_info("progress_circle_set_value: 直接设置值\n");
  log_info("animate:value: 使用动画属性设置值\n");
  log_info("========================================\n");

  /* 启动定时器，定期更新进度条 */
  widget_add_timer(win, on_timer_update, 100);

  return RET_OK;
}

ret_t application_exit() {
  log_info("application_exit\n");
  return RET_OK;
}

#include "awtk_main.inc"
