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
#define WIDGET_PROP_MIN_W "min_w"
#define WIDGET_PROP_MAX_W "max_w"
#define WIDGET_PROP_LAYOUT_W "layout_w"
#define WIDGET_PROP_LAYOUT_H "layout_h"
#define WIDGET_PROP_VIRTUAL_W "virtual_w"
#define WIDGET_PROP_VIRTUAL_H "virtual_h"
#define WIDGET_PROP_NAME "name"
#define WIDGET_PROP_VALUE "value"
#define WIDGET_PROP_TEXT "text"
#define WIDGET_PROP_TR_TEXT "tr_text"
#define WIDGET_PROP_STYLE "style"
#define WIDGET_PROP_ENABLE "enable"
#define WIDGET_PROP_MARGIN "margin"
#define WIDGET_PROP_SPACING "spacing"
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
#define WIDGET_PROP_ITEM_WIDTH "item_width"
#define WIDGET_PROP_ITEM_HEIGHT "item_height"
#define WIDGET_PROP_DEFAULT_ITEM_HEIGHT "default_item_height"
#define WIDGET_PROP_XSLIDABLE "xslidable"
#define WIDGET_PROP_YSLIDABLE "yslidable"
#define WIDGET_PROP_REPEAT "repeat"
#define WIDGET_PROP_ANIMATABLE "animatable"
#define WIDGET_PROP_AUTO_HIDE_SCROLL_BAR "auto_hide_scroll_bar"
#define WIDGET_PROP_IMAGE "image"
#define WIDGET_PROP_DRAW_TYPE "draw_type"
#define WIDGET_PROP_SELECTABLE "selectable"
#define WIDGET_PROP_CLICKABLE "clickable"
#define WIDGET_PROP_SCALE_X "scale_x"
#define WIDGET_PROP_SCALE_Y "scale_y"
#define WIDGET_PROP_ANCHOR_X "anchor_x"
#define WIDGET_PROP_ANCHOR_Y "anchor_y"
#define WIDGET_PROP_ROTATION "rotation"
#define WIDGET_PROP_SUB_THEME "sub_theme"
#define WIDGET_PROP_COMPACT "compact"
#define WIDGET_PROP_ICON "icon"
#define WIDGET_PROP_OPTIONS "options"
#define WIDGET_PROP_SELECTED "selected"
#define WIDGET_PROP_ACTIVE_ICON "active_icon"
#define WIDGET_PROP_OPEN_WINDOW "open_window"
#define WIDGET_PROP_SELECTED_INDEX "selected_index"
#define WIDGET_PROP_CLOSE_WHEN_CLICK "close_when_click"
#define WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE "close_when_click_outside"
#define WIDGET_PROP_LINE_GAP "line_gap"

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
#define WIDGET_TYPE_COMBO_BOX "combo_box"
#define WIDGET_TYPE_COMBO_BOX_ITEM "combo_box_item"
#define WIDGET_TYPE_SLIDE_VIEW "slide_view"
#define WIDGET_TYPE_PAGES "pages"
#define WIDGET_TYPE_TAB_BUTTON "tab_button"
#define WIDGET_TYPE_TAB_CONTROL "tab_control"
#define WIDGET_TYPE_TAB_BUTTON_GROUP "tab_button_group"
#define WIDGET_TYPE_BUTTON_GROUP "button_group"
#define WIDGET_TYPE_CANDIDATES "candidates"
#define WIDGET_TYPE_SPIN_BOX "spin_box"
#define WIDGET_TYPE_DRAGGER "dragger"
#define WIDGET_TYPE_SCROLL_BAR "scroll_bar"
#define WIDGET_TYPE_SCROLL_BAR_DESKTOP "scroll_bar_d"
#define WIDGET_TYPE_SCROLL_BAR_MOBILE "scroll_bar_m"
#define WIDGET_TYPE_SCROLL_VIEW "scroll_view"
#define WIDGET_TYPE_LIST_VIEW "list_view"
#define WIDGET_TYPE_LIST_VIEW_H "list_view_h"
#define WIDGET_TYPE_LIST_ITEM "list_item"

/**
 * @enum widget_state_t
 * @scriptable
 * @prefix WIDGET_
 * 控件状态常量定义。
 */
typedef enum _widget_state_t {
  /**
   * @const WIDGET_STATE_NONE
   * 无效状态。
   */
  WIDGET_STATE_NONE,
  /**
   * @const WIDGET_STATE_NORMAL
   * 正常状态。
   */
  WIDGET_STATE_NORMAL = 1,
  /**
   * @const WIDGET_STATE_PRESSED
   * 指针按下状态。
   */
  WIDGET_STATE_PRESSED = 2,
  /**
   * @const WIDGET_STATE_OVER
   * 指针悬浮状态。
   */
  WIDGET_STATE_OVER = 3,
  /**
   * @const WIDGET_STATE_DISABLE
   * 禁用状态。
   */
  WIDGET_STATE_DISABLE = 4,
  /**
   * @const WIDGET_STATE_FOCUSED
   * 聚焦状态。
   */
  WIDGET_STATE_FOCUSED = 5,
  /**
   * @const WIDGET_STATE_CHECKED
   * 勾选状态。
   */
  WIDGET_STATE_CHECKED = 6,
  /**
   * @const WIDGET_STATE_UNCHECKED
   * 没勾选状态。
   */
  WIDGET_STATE_UNCHECKED = 7,
  /**
   * @const WIDGET_STATE_EMPTY
   * 编辑器无内容状态。
   */
  WIDGET_STATE_EMPTY = 8,
  /**
   * @const WIDGET_STATE_ERROR
   * 输入错误状态。
   */
  WIDGET_STATE_ERROR = 9,
  /**
   * @const WIDGET_STATE_ERROR
   * 输入错误状态。
   */
  WIDGET_STATE_SELECTED = 10
} widget_state_t;
END_C_DECLS

#endif /*TK_WIDGET_CONSTS_H*/
