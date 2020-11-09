/**
 * File:   dialog_toast.inc
 * Author: AWTK Develop Team
 * Brief:  dialog toast
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
 * 2020-08-28 Luo ZhiMing <luozhiming@zlg.cn> created
 *
 */

/*
 *  该文件的功能是服务于 dialog_toast 函数，dialog_toast 函数为仿照 android 的 showToast 函数的功能开发的。
 *  dialog_toast 函数的功能分别是：
 *  1，按照时间长度显示用户输入文字信息。
 *  2，当同时有多出调用地方调用 dialog_toast 函数的显示原则为：
 *    1) 显示最后一个调用的信息，当最后一个信息显示时间到后，检查倒数第二个信息的时间是否已经到了，直到找到还没有到时间的信息。
 *    2) 如果没有找到可以显示的信息，就退出显示 dialog。
 *  3，当退出 dialog 后，还是没有发现有新的信息加入，那就释放所有的内存和单例对象。
 * 
 */


#include "tkc/darray.h"
#include "base/dialog.h"
#include "base/main_loop.h"
#include "base/window_manager.h"


typedef struct _dialog_toast_message_t {
  uint32_t duration;
  uint32_t start_time;
  char text[4];
}dialog_toast_message_t;

typedef struct _dialog_toast_t {
  widget_t* toast;
  bool_t is_empty;
  bool_t is_opened;
  bool_t is_opening;
  bool_t is_closing;
  darray_t message_list;
  uint32_t curr_timer_id;
}dialog_toast_t;

#define DIALOG_TOAST(v)((dialog_toast_t*)(v))

static dialog_toast_t* s_dialog_toast = NULL;
static ret_t dialog_toast_close(dialog_toast_t* dialog_toast);
static ret_t dialog_toast_set_label_self_layout(widget_t* toast, widget_t* label);

static ret_t dialog_get_size_limits(rect_t* r);
static ret_t dialog_toast_on_destroy(void* ctx, event_t* e);
static ret_t dialog_toast_on_click_to_quit(void* ctx, event_t* e);
static ret_t dialog_toast_set_curr_message(dialog_toast_t* dialog_toast, dialog_toast_message_t* message, uint32_t duration);

static ret_t dialog_toast_on_dialog_will_open(void* ctx, event_t* e) {
  dialog_toast_t* dialog_toast = DIALOG_TOAST(ctx);
  dialog_toast->is_opening = TRUE;
  if (dialog_toast->toast->children->size > 0) {
    rect_t r;
    widget_t* label = WIDGET(dialog_toast->toast->children->elms[0]);
    dialog_get_size_limits(&r);
    
    label_set_line_wrap(label, TRUE);
    widget_update_style(label);
    label_resize_to_content(label, r.x, r.w, r.y, r.h);
    dialog_toast_set_label_self_layout(dialog_toast->toast, label);
  }
  return RET_REMOVE;
}

static ret_t dialog_toast_on_dialog_open(void* ctx, event_t* e) {
  dialog_toast_t* dialog_toast = DIALOG_TOAST(ctx);
  dialog_toast->is_opened = TRUE;
  dialog_toast->is_opening = FALSE;
  /* 播放完打开窗口动画后，重新显示信息，同时解决有可能在播放动画期间加入信息导致播放动画不正确的问题 */
  if (dialog_toast->message_list.size > 0) {
    dialog_toast_message_t* message = (dialog_toast_message_t*)darray_tail(&(dialog_toast->message_list));
    dialog_toast_set_curr_message(dialog_toast, message, message->duration);
  }
  return RET_REMOVE;
}

static ret_t dialog_toast_init(dialog_toast_t* dialog_toast, bool_t init_list) {

  return_value_if_fail(dialog_toast != NULL, RET_BAD_PARAMS);

  dialog_toast->is_opened = FALSE;
  dialog_toast->is_opening = FALSE;
  dialog_toast->is_closing = FALSE;
  dialog_toast->curr_timer_id = TK_INVALID_ID;
  dialog_toast->toast = dialog_create(NULL, 0, 0, 0, 0);
  dialog_toast->is_empty = dialog_toast->message_list.size == 0;

  if (init_list) {
    darray_init(&(dialog_toast->message_list), 10, default_destroy, NULL);
  }

  if (dialog_toast->toast != NULL) {
    widget_set_prop_str(dialog_toast->toast, WIDGET_PROP_THEME, "dialog_toast");
    widget_set_prop_str(dialog_toast->toast, WIDGET_PROP_ANIM_HINT, "fade(duration=500)");

    widget_on(dialog_toast->toast, EVT_DESTROY, dialog_toast_on_destroy, dialog_toast);
    widget_on(dialog_toast->toast, EVT_WINDOW_OPEN, dialog_toast_on_dialog_open, dialog_toast);
    widget_on(dialog_toast->toast, EVT_POINTER_UP, dialog_toast_on_click_to_quit, dialog_toast);
    widget_on(dialog_toast->toast, EVT_WINDOW_WILL_OPEN, dialog_toast_on_dialog_will_open, dialog_toast);

    return RET_OK;
  }
  
  return RET_OOM;
}

static ret_t dialog_toast_on_idle_create_dialog(const idle_info_t* info) {
  ret_t ret = RET_OK;
  dialog_toast_t* dialog_toast = DIALOG_TOAST(info->ctx);

  ret = dialog_toast_init(dialog_toast, FALSE);
  return_value_if_fail(ret == RET_OK, ret);
  /* 检查是否在关闭窗口动画的过程中加入新的信息 */
  if (dialog_toast->message_list.size > 0) {
    dialog_toast_message_t* message = (dialog_toast_message_t*)darray_tail(&(dialog_toast->message_list));
    dialog_toast_set_curr_message(dialog_toast, message, message->duration);
  } else {
    dialog_toast_close(dialog_toast);
  }

  return RET_REMOVE;
}

static ret_t dialog_toast_on_destroy(void* ctx, event_t* e) {
  dialog_toast_t* dialog_toast = DIALOG_TOAST(ctx);
  /* 在播放关闭 dialog 动画的期间新插入显示的消息就不是否 dialog_toast 对象 */
  if (dialog_toast->message_list.size == 0) {
    /* 没有需要显示的消息后，彻底清除所有数据，把所有的内存释放出去。 */
    darray_deinit(&(dialog_toast->message_list));
    TKMEM_FREE(dialog_toast);
    s_dialog_toast = NULL;
  } else {
    /* 重新创建 dialog 对象，并把在关闭动画期间插入的信息显示出来 */
    idle_add(dialog_toast_on_idle_create_dialog, dialog_toast);
  }

  return RET_OK;
}

static ret_t dialog_toast_quit(dialog_toast_t* dialog_toast) {
  bool_t first = TRUE;
  uint32_t now = timer_manager()->get_time();
  darray_t* message_list = &(dialog_toast->message_list);
  return_value_if_fail(message_list->size > 0, RET_FAIL);
  if (!dialog_toast->is_opened) {
    return RET_BUSY;
  }
  do {
    if (message_list->size > 0) {
      dialog_toast_message_t* message = (dialog_toast_message_t*)darray_tail(message_list);
      int32_t diff_time = message->duration + message->start_time - now;
      /* 
       * dialog_toast_quit 函数中一定会释放正在显示的信息。
       * 释放正在显示的信息后，检查队列中是否有没有过期的信息。
       */
      if (diff_time > 0 && !first) {
        /* 在宏观时间轴上，之前加入的信息的时间也是在跑的，所以只要显示剩余的时间 */
        dialog_toast_set_curr_message(dialog_toast, message, diff_time);
        dialog_toast->is_empty = FALSE;
        break;
      } else {
        /* 清除过期的消息和释放其占用的内存 */
        void* data = darray_pop(message_list);
        if (data != NULL) {
          TKMEM_FREE(data);
        }
        if (diff_time > 0 && first) {
          timer_remove(dialog_toast->curr_timer_id);
          dialog_toast->curr_timer_id = TK_INVALID_ID;
        }
#ifndef AWTK_WEB
        main_loop_quit(main_loop());
#endif
      }
      first = FALSE;
    }
    dialog_toast->is_empty = dialog_toast->message_list.size == 0;
  }while (message_list->size > 0);

#ifdef AWTK_WEB
  /* 因为 web 状态下，不支持模态，所以当队列中没有信息了，就马上退出 dialog */
  if (dialog_toast->is_empty) {
    dialog_toast_close(dialog_toast);
  }
#endif
  return RET_OK;
}

static ret_t dialog_toast_on_timer_to_quit(const timer_info_t* info) {
  dialog_toast_quit(DIALOG_TOAST(info->ctx));

  return RET_REMOVE;
}

static ret_t dialog_toast_on_click_to_quit(void* ctx, event_t* e) {
  dialog_toast_quit(DIALOG_TOAST(ctx));

  return RET_REMOVE;
}

static dialog_toast_t* dialog_toast_manager() {
  
  if (s_dialog_toast != NULL) {
    return s_dialog_toast;
  }

  s_dialog_toast = TKMEM_ZALLOC(dialog_toast_t);

  if (dialog_toast_init(s_dialog_toast, TRUE) == RET_OK) {
    return s_dialog_toast;
  } else {
    if (s_dialog_toast != NULL) {
      TKMEM_FREE(s_dialog_toast);
    }
    return NULL;
  }
}

static ret_t dialog_toast_on_idle_close(const idle_info_t* info) {
  widget_t* widget = WIDGET(info->ctx);
  return window_manager_close_window(widget->parent, widget);
}

static ret_t dialog_toast_close(dialog_toast_t* dialog_toast) {
  return_value_if_fail(dialog_toast != NULL, RET_BAD_PARAMS);
  if (dialog_toast != NULL && dialog_toast->is_empty && !dialog_toast->is_closing) {
    dialog_toast->is_closing = TRUE;
    idle_add(dialog_toast_on_idle_close, dialog_toast->toast);
  }
  return RET_OK;
}

static widget_t* dialog_toast_create_label(widget_t* parent, const char* text) {
  rect_t r;
  widget_t* label = NULL;

  dialog_get_size_limits(&r);

  label = label_create(parent, 0, 0, 0, 0);
  return_value_if_fail(label != NULL, NULL);
  
  label_set_line_wrap(label, TRUE);
  widget_update_style(label);

  if (text != NULL) {
    widget_set_tr_text(label, text);
  }
  label_resize_to_content(label, r.x, r.w, r.y, r.h);

  return label;
}

static ret_t dialog_toast_set_label_self_layout(widget_t* toast, widget_t* label) {
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = 0;
  char params[128];
  int32_t margin = 10;
  int32_t margin_top = 0;
  int32_t margin_left = 0;
  int32_t margin_right = 0;
  int32_t margin_bottom = 0;
  style_t* style = toast->astyle;
  return_value_if_fail(toast != NULL && label != NULL, RET_BAD_PARAMS);

  memset(params, 0x0, sizeof(params));

  margin = style_get_int(style, STYLE_ID_MARGIN, margin);
  margin_top = style_get_int(style, STYLE_ID_MARGIN_TOP, margin);
  margin_left = style_get_int(style, STYLE_ID_MARGIN_LEFT, margin);
  margin_right = style_get_int(style, STYLE_ID_MARGIN_RIGHT, margin);
  margin_bottom = style_get_int(style, STYLE_ID_MARGIN_BOTTOM, margin);

  w = label->w + margin_left + margin_right;
  h = label->h + margin_top + margin_bottom;
  x = (toast->parent->w - w) >> 1;
  y = (toast->parent->h - h) >> 1;
  
  widget_move_resize(toast, x, y, w, h);

  tk_snprintf(params, sizeof(params) - 1, "default(x=%d, y=%d, w=%d, h=%d)", margin_left, margin_top,
              label->w, label->h);
  
  return widget_set_self_layout(label, params);
}

static ret_t dialog_toast_set_curr_message(dialog_toast_t* dialog_toast, dialog_toast_message_t* message, uint32_t duration) {

  widget_t* label = NULL;
 
  return_value_if_fail(dialog_toast != NULL && message != NULL && duration != 0, RET_BAD_PARAMS);

  /* 移除前一个显示信息的计时器 */
  if (dialog_toast->curr_timer_id != TK_INVALID_ID) {
    ENSURE(timer_remove(dialog_toast->curr_timer_id) == RET_OK);
  }

  /* 添加新一个信息的计时器 */
  dialog_toast->curr_timer_id = timer_add(dialog_toast_on_timer_to_quit, dialog_toast, duration);
  ENSURE(dialog_toast->curr_timer_id != TK_INVALID_ID);

  widget_destroy_children(dialog_toast->toast);

  if (dialog_toast->is_opened) {
    label = dialog_toast_create_label(dialog_toast->toast, message->text);
    return_value_if_fail(label != NULL, RET_OOM);

    return dialog_toast_set_label_self_layout(dialog_toast->toast, label);
  } else {
    label = label_create(dialog_toast->toast, 0, 0, 0, 0);
    return_value_if_fail(label != NULL, RET_OOM);
    widget_set_tr_text(label, message->text);
    return RET_OK;
  }
 
}

static dialog_toast_message_t* dialog_toast_push_message(dialog_toast_t* dialog_toast, const char* text, uint32_t duration) {
  uint32_t size = 0;
  ret_t ret = RET_FAIL;
  darray_t* list = NULL;
  dialog_toast_message_t* message = NULL;
  return_value_if_fail(dialog_toast != NULL && text != NULL && duration != 0, NULL);

  list = &(dialog_toast->message_list);
  size = list->size;
  message = TKMEM_CALLOC(sizeof(uint8_t), sizeof(dialog_toast_message_t) + strlen(text));
  if (message != NULL) {
    strcpy(message->text, text);
    message->duration = duration;
    message->start_time = timer_manager()->get_time();
    ret = darray_push(list, message);
    goto_error_if_fail(ret == RET_OK);

    /* 在关闭 dialog 动画期间不显示消息，等完成关闭动画后重新创建 dialog 再显示消息 */
    if (!dialog_toast->is_closing && !dialog_toast->is_opening) {
      ret = dialog_toast_set_curr_message(dialog_toast, message, duration);
      goto_error_if_fail(ret == RET_OK);
    }

    return message;
  } else {
    return NULL;
  }
error:
  if (size < list->size) {
    darray_pop(list);
  }
  if (message != NULL) {
    TKMEM_FREE(message);
  }
  return NULL;
}

static ret_t dialog_toast_model(dialog_toast_t* dialog_toast) {
  return_value_if_fail(dialog_toast != NULL && dialog_toast->toast != NULL, RET_BAD_PARAMS);
#ifdef AWTK_WEB
  log_debug("awtk web not support dialog_modal\n");
  return RET_FAIL;
#else
  log_debug("%s run\n", __FUNCTION__);
  main_loop_run(main_loop());
  log_debug("%s quit\n", __FUNCTION__);
  dialog_toast_close(dialog_toast);
  return RET_OK;
#endif /*AWTK_WEB*/
}
