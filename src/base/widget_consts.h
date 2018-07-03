/**
 * File:   widget_consts.h
 * Author: AWTK Develop Team
 * Brief:  widget property names
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
 * 2018-04-19 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_WIDGET_CONSTS_H
#define TK_WIDGET_CONSTS_H

#include "base/types_def.h"

BEGIN_C_DECLS

/*widget props*/
#define WIDGET_PROP_X "x"
#define WIDGET_PROP_Y "y"
#define WIDGET_PROP_W "w"
#define WIDGET_PROP_H "h"
#define WIDGET_PROP_NAME "name"
#define WIDGET_PROP_VALUE "value"
#define WIDGET_PROP_TEXT "text"
#define WIDGET_PROP_TR_TEXT "tr_text"
#define WIDGET_PROP_STYLE "style"
#define WIDGET_PROP_ENABLE "enable"
#define WIDGET_PROP_MARGIN "margin"
#define WIDGET_PROP_LEFT_MARGIN "left_margin"
#define WIDGET_PROP_RIGHT_MARGIN "right_margin"
#define WIDGET_PROP_TOP_MARGIN "top_margin"
#define WIDGET_PROP_BOTTOM_MARGIN "bottom_margin"
#define WIDGET_PROP_STEP "step"
#define WIDGET_PROP_VISIBLE "visible"
#define WIDGET_PROP_ANIM_HINT "anim_hint"
#define WIDGET_PROP_OPEN_ANIM_HINT "open_anim_hint"
#define WIDGET_PROP_CLOSE_ANIM_HINT "opne_anim_hint"
#define WIDGET_PROP_MIN "min"
#define WIDGET_PROP_MAX "max"
#define WIDGET_PROP_TIPS "tips"
#define WIDGET_PROP_INPUT_TYPE "input_type"
#define WIDGET_PROP_READONLY "readonly"
#define WIDGET_PROP_ACTIVE "active"
#define WIDGET_PROP_VERTICAL "vertical"
#define WIDGET_PROP_SHOW_TEXT "show_text"
#define WIDGET_PROP_XOFFSET "xoffset"
#define WIDGET_PROP_YOFFSET "yoffset"
#define WIDGET_PROP_AUTO_PLAY "auto_play"
#define WIDGET_PROP_AUTO_FIX "auto_fix"
#define WIDGET_PROP_X_MIN "x_min"
#define WIDGET_PROP_X_MAX "x_max"
#define WIDGET_PROP_Y_MIN "y_min"
#define WIDGET_PROP_Y_MAX "y_max"
#define WIDGET_PROP_MAX "max"
#define WIDGET_PROP_ROW "row"
#define WIDGET_PROP_THEME "theme"
#define WIDGET_PROP_SCRIPT "script"

/*widget type name*/
#define WIDGET_TYPE_NONE "widget"
#define WIDGET_TYPE_WINDOW_MANAGER "window_manager"
#define WIDGET_TYPE_NORMAL_WINDOW "window"
#define WIDGET_TYPE_TOOL_BAR "tool_bar"
#define WIDGET_TYPE_DIALOG "dialog"
#define WIDGET_TYPE_POPUP "popup"
#define WIDGET_TYPE_SPRITE "sprite"
#define WIDGET_TYPE_KEYBOARD "keyboard"
#define WIDGET_TYPE_DND "dnd"
#define WIDGET_TYPE_LABEL "label"
#define WIDGET_TYPE_BUTTON "button"
#define WIDGET_TYPE_IMAGE "image"
#define WIDGET_TYPE_EDIT "edit"
#define WIDGET_TYPE_PROGRESS_BAR "progress_bar"
#define WIDGET_TYPE_GROUP_BOX "group_box"
#define WIDGET_TYPE_CHECK_BUTTON "check_button"
#define WIDGET_TYPE_RADIO_BUTTON "radio_button"
#define WIDGET_TYPE_DIALOG_TITLE "dialog_title"
#define WIDGET_TYPE_DIALOG_CLIENT "dialog_client"
#define WIDGET_TYPE_SLIDER "slider"
#define WIDGET_TYPE_VIEW "view"
#define WIDGET_TYPE_SLIDE_VIEW "slide_view"
#define WIDGET_TYPE_PAGES "pages"
#define WIDGET_TYPE_CANDIDATES "candidates"
#define WIDGET_TYPE_SPIN_BOX "spin_box"
#define WIDGET_TYPE_DRAGGER "dragger"
#define WIDGET_TYPE_SCROLL_BAR "scroll_bar"
#define WIDGET_TYPE_SCROLL_BAR_DESKTOP "scroll_bar_d"
#define WIDGET_TYPE_SCROLL_BAR_MOBILE "scroll_bar_m"

/**
 * @enum widget_type_t
 * @scriptable
 * 控件类型常量定义。
 */
typedef enum _widget_type_t {
  /**
   * @const WIDGET_NONE
   * 无效类型。
   */
  WIDGET_NONE = 0,
  /**
   * @const WIDGET_WINDOW_MANAGER
   * 窗口管理器。
   */
  WIDGET_WINDOW_MANAGER,
  /**
   * @const WIDGET_NORMAL_WINDOW
   * 窗口。
   */
  WIDGET_NORMAL_WINDOW,
  /**
   * @const WIDGET_TOOL_BAR
   * 工具条。
   */
  WIDGET_TOOL_BAR,
  /**
   * @const WIDGET_DIALOG
   * 对话框。
   */
  WIDGET_DIALOG,
  /**
   * @const WIDGET_POPUP
   * 弹出框。
   */
  WIDGET_POPUP,
  /**
   * @const WIDGET_SPRITE
   * 精灵窗口如鼠标指针。
   */
  WIDGET_SPRITE,
  /**
   * @const WIDGET_KEYBOARD
   * 软键盘。
   */
  WIDGET_KEYBOARD,
  /**
   * @const WIDGET_DND
   * drag & drop icon。
   */
  WIDGET_DND,
  /**
   * @const WIDGET_LABEL
   * 简单文本。
   */
  WIDGET_LABEL,
  /**
   * @const WIDGET_BUTTON
   * 按钮。
   */
  WIDGET_BUTTON,
  /**
   * @const WIDGET_IMAGE
   * 图片。
   */
  WIDGET_IMAGE,
  /**
   * @const WIDGET_EDIT
   * 文本编辑器。
   */
  WIDGET_EDIT,
  /**
   * @const WIDGET_PROGRESS_BAR
   * 进度条。
   */
  WIDGET_PROGRESS_BAR,
  /**
   * @const WIDGET_GROUP_BOX
   * 分组框。
   */
  WIDGET_GROUP_BOX,
  /**
   * @const WIDGET_CHECK_BUTTON
   * 多选按钮。
   */
  WIDGET_CHECK_BUTTON,
  /**
   * @const WIDGET_RADIO_BUTTON
   * 单选按钮。
   */
  WIDGET_RADIO_BUTTON,
  /**
   * @const WIDGET_DIALOG_TITLE
   * 对话框的标题。
   */
  WIDGET_DIALOG_TITLE,
  /**
   * @const WIDGET_DIALOG_CLIENT
   * 对话框的客户区。
   */
  WIDGET_DIALOG_CLIENT,
  /**
   * @const WIDGET_SLIDER
   * 滑块。
   */
  WIDGET_SLIDER,
  /**
   * @const WIDGET_VIEW
   * 通用容器和自绘控件。
   */
  WIDGET_VIEW,
  /**
   * @const WIDGET_SLIDE_VIEW
   * Slide View。
   */
  WIDGET_SLIDE_VIEW,
  /**
   * @const WIDGET_PAGES
   * Slide View。
   */
  WIDGET_PAGES,
  /**
   * @const WIDGET_CANDIDATES
   * Input method words candidates。
   */
  WIDGET_CANDIDATES,
  /**
   * @const WIDGET_SPIN_BOX
   * Spin Box。
   */
  WIDGET_SPIN_BOX,
  /**
   * @const WIDGET_DRAGGER
   * Dragger。
   */
  WIDGET_DRAGGER,
  /**
   * @const WIDGET_SCROLL_BAR
   * Dragger。
   */
  WIDGET_SCROLL_BAR,

  WIDGET_NR,
  WIDGET_USER_START = 100

} widget_type_t;

END_C_DECLS

#endif /*TK_WIDGET_CONSTS_H*/
