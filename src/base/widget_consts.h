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

/**
 * @enum widget_prop_t
 * @annotation ["scriptable", "string"]
 * @prefix WIDGET_PROP_
 * 控件的属性。
 */

/**
 * @const WIDGET_PROP_X
 * X坐标。
 */
#define WIDGET_PROP_X "x"

/**
 * @const WIDGET_PROP_Y
 * Y坐标。
 */
#define WIDGET_PROP_Y "y"

/**
 * @const WIDGET_PROP_W
 * 宽度。
 */
#define WIDGET_PROP_W "w"

/**
 * @const WIDGET_PROP_H
 * 高度。
 */
#define WIDGET_PROP_H "h"

/**
 * @const WIDGET_PROP_MIN_W
 * 最小宽度。
 */
#define WIDGET_PROP_MIN_W "min_w"

/**
 * @const WIDGET_PROP_MAX_W
 * 最大宽度。
 */
#define WIDGET_PROP_MAX_W "max_w"

/**
 * @const WIDGET_PROP_LAYOUT_W
 * layout宽度。
 */
#define WIDGET_PROP_LAYOUT_W "layout_w"

/**
 * @const WIDGET_PROP_LAYOUT_H
 * layout高度。
 */
#define WIDGET_PROP_LAYOUT_H "layout_h"

/**
 * @const WIDGET_PROP_VIRTUAL_W
 * 虚拟宽度。
 */
#define WIDGET_PROP_VIRTUAL_W "virtual_w"

/**
 * @const WIDGET_PROP_VIRTUAL_H
 * 虚拟高度。
 */
#define WIDGET_PROP_VIRTUAL_H "virtual_h"

/**
 * @const WIDGET_PROP_NAME
 * 名称。
 */
#define WIDGET_PROP_NAME "name"

/**
 * @const WIDGET_PROP_CURSOR
 * 鼠标指针。
 */
#define WIDGET_PROP_CURSOR "cursor"

/**
 * @const WIDGET_PROP_VALUE
 * 值。
 */
#define WIDGET_PROP_VALUE "value"

/**
 * @const WIDGET_PROP_TEXT
 * 文本。
 */
#define WIDGET_PROP_TEXT "text"

/**
 * @const WIDGET_PROP_TR_TEXT
 * 待翻译文本。
 */
#define WIDGET_PROP_TR_TEXT "tr_text"

/**
 * @const WIDGET_PROP_STYLE
 * style。
 */
#define WIDGET_PROP_STYLE "style"

/**
 * @const WIDGET_PROP_ENABLE
 * 是否启用。
 */
#define WIDGET_PROP_ENABLE "enable"

/**
 * @const WIDGET_PROP_MARGIN
 * 边距。
 */
#define WIDGET_PROP_MARGIN "margin"

/**
 * @const WIDGET_PROP_SPACING
 * 间距。
 */
#define WIDGET_PROP_SPACING "spacing"

/**
 * @const WIDGET_PROP_LEFT_MARGIN
 * 左边距。
 */
#define WIDGET_PROP_LEFT_MARGIN "left_margin"

/**
 * @const WIDGET_PROP_RIGHT_MARGIN
 * 右边距。
 */
#define WIDGET_PROP_RIGHT_MARGIN "right_margin"

/**
 * @const WIDGET_PROP_TOP_MARGIN
 * 顶边距。
 */
#define WIDGET_PROP_TOP_MARGIN "top_margin"

/**
 * @const WIDGET_PROP_BOTTOM_MARGIN
 * 底边距。
 */
#define WIDGET_PROP_BOTTOM_MARGIN "bottom_margin"

/**
 * @const WIDGET_PROP_STEP
 * 步长。
 */
#define WIDGET_PROP_STEP "step"

/**
 * @const WIDGET_PROP_VISIBLE
 * 是否可见。
 */
#define WIDGET_PROP_VISIBLE "visible"

/**
 * @const WIDGET_PROP_ANIMATION
 * 控件动画。
 */
#define WIDGET_PROP_ANIMATION "animation"

/**
 * @const WIDGET_PROP_ANIM_HINT
 * 窗口动画。
 */
#define WIDGET_PROP_ANIM_HINT "anim_hint"

/**
 * @const WIDGET_PROP_OPEN_ANIM_HINT
 * 打开窗口动画。
 */
#define WIDGET_PROP_OPEN_ANIM_HINT "open_anim_hint"

/**
 * @const WIDGET_PROP_CLOSE_ANIM_HINT
 * 关闭窗口动画。
 */
#define WIDGET_PROP_CLOSE_ANIM_HINT "close_anim_hint"

/**
 * @const WIDGET_PROP_MIN
 *
 */
#define WIDGET_PROP_MIN "min"

/**
 * @const WIDGET_PROP_TIPS
 *
 */
#define WIDGET_PROP_TIPS "tips"

/**
 * @const WIDGET_PROP_INPUT_TYPE
 *
 */
#define WIDGET_PROP_INPUT_TYPE "input_type"

/**
 * @const WIDGET_PROP_READONLY
 *
 */
#define WIDGET_PROP_READONLY "readonly"

/**
 * @const WIDGET_PROP_PASSWORD_VISIBLE
 *
 */
#define WIDGET_PROP_PASSWORD_VISIBLE "password_visible"

/**
 * @const WIDGET_PROP_ACTIVE
 *
 */
#define WIDGET_PROP_ACTIVE "active"

/**
 * @const WIDGET_PROP_VERTICAL
 *
 */
#define WIDGET_PROP_VERTICAL "vertical"

/**
 * @const WIDGET_PROP_SHOW_TEXT
 *
 */
#define WIDGET_PROP_SHOW_TEXT "show_text"

/**
 * @const WIDGET_PROP_XOFFSET
 *
 */
#define WIDGET_PROP_XOFFSET "xoffset"

/**
 * @const WIDGET_PROP_YOFFSET
 *
 */
#define WIDGET_PROP_YOFFSET "yoffset"

/**
 * @const WIDGET_PROP_AUTO_PLAY
 *
 */
#define WIDGET_PROP_AUTO_PLAY "auto_play"

/**
 * @const WIDGET_PROP_AUTO_FIX
 *
 */
#define WIDGET_PROP_AUTO_FIX "auto_fix"

/**
 * @const WIDGET_PROP_X_MIN
 *
 */
#define WIDGET_PROP_X_MIN "x_min"

/**
 * @const WIDGET_PROP_X_MAX
 *
 */
#define WIDGET_PROP_X_MAX "x_max"

/**
 * @const WIDGET_PROP_Y_MIN
 *
 */
#define WIDGET_PROP_Y_MIN "y_min"

/**
 * @const WIDGET_PROP_Y_MAX
 *
 */
#define WIDGET_PROP_Y_MAX "y_max"

/**
 * @const WIDGET_PROP_MAX
 *
 */
#define WIDGET_PROP_MAX "max"

/**
 * @const WIDGET_PROP_ROW
 *
 */
#define WIDGET_PROP_ROW "row"

/**
 * @const WIDGET_PROP_STATE_FOR_STYLE
 * 控件状态。
 */
#define WIDGET_PROP_STATE_FOR_STYLE "state_for_style"

/**
 * @const WIDGET_PROP_THEME
 * 窗口主题名称。
 */
#define WIDGET_PROP_THEME "theme"

/**
 * @const WIDGET_PROP_STAGE
 * window stage
 */
#define WIDGET_PROP_STAGE "stage"

/**
 * @const WIDGET_PROP_IMAGE_MANAGER
 * 图片管理器。
 */
#define WIDGET_PROP_IMAGE_MANAGER "image_manager"

/**
 * @const WIDGET_PROP_ASSETS_MANAGER
 * 资源管理器。
 */
#define WIDGET_PROP_ASSETS_MANAGER "assets_manager"

/**
 * @const WIDGET_PROP_FONT_MANAGER
 * 字体管理器。
 */
#define WIDGET_PROP_FONT_MANAGER "font_manager"

/**
 * @const WIDGET_PROP_THEME_OBJ
 * 窗口的主题对象。
 */
#define WIDGET_PROP_THEME_OBJ "theme_obj"

/**
 * @const WIDGET_PROP_DEFAULT_THEME_OBJ
 * 缺省的主题对象。
 */
#define WIDGET_PROP_DEFAULT_THEME_OBJ "default_theme_obj"

/**
 * @const WIDGET_PROP_SCRIPT
 *
 */
#define WIDGET_PROP_SCRIPT "script"

/**
 * @const WIDGET_PROP_ITEM_WIDTH
 *
 */
#define WIDGET_PROP_ITEM_WIDTH "item_width"

/**
 * @const WIDGET_PROP_ITEM_HEIGHT
 *
 */
#define WIDGET_PROP_ITEM_HEIGHT "item_height"

/**
 * @const WIDGET_PROP_DEFAULT_ITEM_HEIGHT
 *
 */
#define WIDGET_PROP_DEFAULT_ITEM_HEIGHT "default_item_height"

/**
 * @const WIDGET_PROP_XSLIDABLE
 *
 */
#define WIDGET_PROP_XSLIDABLE "xslidable"

/**
 * @const WIDGET_PROP_YSLIDABLE
 *
 */
#define WIDGET_PROP_YSLIDABLE "yslidable"

/**
 * @const WIDGET_PROP_REPEAT
 *
 */
#define WIDGET_PROP_REPEAT "repeat"

/**
 * @const WIDGET_PROP_ANIMATABLE
 *
 */
#define WIDGET_PROP_ANIMATABLE "animatable"

/**
 * @const WIDGET_PROP_AUTO_HIDE_SCROLL_BAR
 *
 */
#define WIDGET_PROP_AUTO_HIDE_SCROLL_BAR "auto_hide_scroll_bar"

/**
 * @const WIDGET_PROP_IMAGE
 *
 */
#define WIDGET_PROP_IMAGE "image"

/**
 * @const WIDGET_PROP_DRAW_TYPE
 *
 */
#define WIDGET_PROP_DRAW_TYPE "draw_type"

/**
 * @const WIDGET_PROP_SELECTABLE
 *
 */
#define WIDGET_PROP_SELECTABLE "selectable"

/**
 * @const WIDGET_PROP_CLICKABLE
 *
 */
#define WIDGET_PROP_CLICKABLE "clickable"

/**
 * @const WIDGET_PROP_SCALE_X
 *
 */
#define WIDGET_PROP_SCALE_X "scale_x"

/**
 * @const WIDGET_PROP_SCALE_Y
 *
 */
#define WIDGET_PROP_SCALE_Y "scale_y"

/**
 * @const WIDGET_PROP_ANCHOR_X
 * x锚点。
 */
#define WIDGET_PROP_ANCHOR_X "anchor_x"

/**
 * @const WIDGET_PROP_ANCHOR_Y
 * y锚点。
 */
#define WIDGET_PROP_ANCHOR_Y "anchor_y"

/**
 * @const WIDGET_PROP_ROTATION
 * 选中角度(幅度)
 */
#define WIDGET_PROP_ROTATION "rotation"

/**
 * @const WIDGET_PROP_COMPACT
 * 紧凑模式。
 */
#define WIDGET_PROP_COMPACT "compact"

/**
 * @const WIDGET_PROP_ICON
 * 图标名称。
 */
#define WIDGET_PROP_ICON "icon"

/**
 * @const WIDGET_PROP_OPTIONS
 * 选项集合。
 */
#define WIDGET_PROP_OPTIONS "options"

/**
 * @const WIDGET_PROP_SELECTED
 * 是否被选中。
 */
#define WIDGET_PROP_SELECTED "selected"

/**
 * @const WIDGET_PROP_CHECKED
 * 是否被勾选。
 */
#define WIDGET_PROP_CHECKED "checked"

/**
 * @const WIDGET_PROP_ACTIVE_ICON
 * active状态下的图标。
 */
#define WIDGET_PROP_ACTIVE_ICON "active_icon"

/**
 * @const WIDGET_PROP_OPEN_WINDOW
 * 要打开窗口的名称。
 */
#define WIDGET_PROP_OPEN_WINDOW "open_window"

/**
 * @const WIDGET_PROP_SELECTED_INDEX
 * 被选中项的索引。
 */
#define WIDGET_PROP_SELECTED_INDEX "selected_index"

/**
 * @const WIDGET_PROP_CLOSE_WHEN_CLICK
 * 点击窗口时关闭窗口。
 */
#define WIDGET_PROP_CLOSE_WHEN_CLICK "close_when_click"

/**
 * @const WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE
 * 点击窗口外部时关闭窗口。
 */
#define WIDGET_PROP_CLOSE_WHEN_CLICK_OUTSIDE "close_when_click_outside"

/**
 * @const WIDGET_PROP_LINE_GAP
 * 行间距。
 */
#define WIDGET_PROP_LINE_GAP "line_gap"

/**
 * @const WIDGET_PROP_BG_COLOR
 * 背景颜色(仅仅使用于color tile)。
 */
#define WIDGET_PROP_BG_COLOR "bg_color"

/**
 * @const WIDGET_PROP_BORDER_COLOR
 * 边框颜色(仅仅使用于color tile)。
 */
#define WIDGET_PROP_BORDER_COLOR "border_color"

/**
 * @const WIDGET_PROP_DELAY
 * 延迟时间(毫秒)
 */
#define WIDGET_PROP_DELAY "delay"

/**
 * @enum widget_type_t
 * @annotation ["scriptable", "string"]
 * @prefix WIDGET_TYPE_
 * 控件的类型。
 */

/**
 * @const WIDGET_TYPE_NONE
 *
 */
#define WIDGET_TYPE_NONE "widget"

/**
 * @const WIDGET_TYPE_WINDOW_MANAGER
 *
 */
#define WIDGET_TYPE_WINDOW_MANAGER "window_manager"

/**
 * @const WIDGET_TYPE_NORMAL_WINDOW
 *
 */
#define WIDGET_TYPE_NORMAL_WINDOW "window"

/**
 * @const WIDGET_TYPE_TOOL_BAR
 *
 */
#define WIDGET_TYPE_TOOL_BAR "tool_bar"

/**
 * @const WIDGET_TYPE_DIALOG
 *
 */
#define WIDGET_TYPE_DIALOG "dialog"

/**
 * @const WIDGET_TYPE_POPUP
 *
 */
#define WIDGET_TYPE_POPUP "popup"

/**
 * @const WIDGET_TYPE_SPRITE
 *
 */
#define WIDGET_TYPE_SPRITE "sprite"

/**
 * @const WIDGET_TYPE_KEYBOARD
 *
 */
#define WIDGET_TYPE_KEYBOARD "keyboard"

/**
 * @const WIDGET_TYPE_DND
 *
 */
#define WIDGET_TYPE_DND "dnd"

/**
 * @const WIDGET_TYPE_LABEL
 *
 */
#define WIDGET_TYPE_LABEL "label"

/**
 * @const WIDGET_TYPE_BUTTON
 *
 */
#define WIDGET_TYPE_BUTTON "button"

/**
 * @const WIDGET_TYPE_IMAGE
 *
 */
#define WIDGET_TYPE_IMAGE "image"

/**
 * @const WIDGET_TYPE_EDIT
 *
 */
#define WIDGET_TYPE_EDIT "edit"

/**
 * @const WIDGET_TYPE_PROGRESS_BAR
 *
 */
#define WIDGET_TYPE_PROGRESS_BAR "progress_bar"

/**
 * @const WIDGET_TYPE_GROUP_BOX
 *
 */
#define WIDGET_TYPE_GROUP_BOX "group_box"

/**
 * @const WIDGET_TYPE_CHECK_BUTTON
 *
 */
#define WIDGET_TYPE_CHECK_BUTTON "check_button"

/**
 * @const WIDGET_TYPE_RADIO_BUTTON
 *
 */
#define WIDGET_TYPE_RADIO_BUTTON "radio_button"

/**
 * @const WIDGET_TYPE_DIALOG_TITLE
 *
 */
#define WIDGET_TYPE_DIALOG_TITLE "dialog_title"

/**
 * @const WIDGET_TYPE_DIALOG_CLIENT
 *
 */
#define WIDGET_TYPE_DIALOG_CLIENT "dialog_client"

/**
 * @const WIDGET_TYPE_SLIDER
 *
 */
#define WIDGET_TYPE_SLIDER "slider"

/**
 * @const WIDGET_TYPE_VIEW
 *
 */
#define WIDGET_TYPE_VIEW "view"

/**
 * @const WIDGET_TYPE_COMBO_BOX
 *
 */
#define WIDGET_TYPE_COMBO_BOX "combo_box"

/**
 * @const WIDGET_TYPE_COMBO_BOX_ITEM
 *
 */
#define WIDGET_TYPE_COMBO_BOX_ITEM "combo_box_item"

/**
 * @const WIDGET_TYPE_SLIDE_VIEW
 *
 */
#define WIDGET_TYPE_SLIDE_VIEW "slide_view"

/**
 * @const WIDGET_TYPE_PAGES
 *
 */
#define WIDGET_TYPE_PAGES "pages"

/**
 * @const WIDGET_TYPE_TAB_BUTTON
 *
 */
#define WIDGET_TYPE_TAB_BUTTON "tab_button"

/**
 * @const WIDGET_TYPE_TAB_CONTROL
 *
 */
#define WIDGET_TYPE_TAB_CONTROL "tab_control"

/**
 * @const WIDGET_TYPE_TAB_BUTTON_GROUP
 *
 */
#define WIDGET_TYPE_TAB_BUTTON_GROUP "tab_button_group"

/**
 * @const WIDGET_TYPE_BUTTON_GROUP
 *
 */
#define WIDGET_TYPE_BUTTON_GROUP "button_group"

/**
 * @const WIDGET_TYPE_CANDIDATES
 *
 */
#define WIDGET_TYPE_CANDIDATES "candidates"

/**
 * @const WIDGET_TYPE_SPIN_BOX
 *
 */
#define WIDGET_TYPE_SPIN_BOX "spin_box"

/**
 * @const WIDGET_TYPE_DRAGGER
 *
 */
#define WIDGET_TYPE_DRAGGER "dragger"

/**
 * @const WIDGET_TYPE_SCROLL_BAR
 *
 */
#define WIDGET_TYPE_SCROLL_BAR "scroll_bar"

/**
 * @const WIDGET_TYPE_SCROLL_BAR_DESKTOP
 *
 */
#define WIDGET_TYPE_SCROLL_BAR_DESKTOP "scroll_bar_d"

/**
 * @const WIDGET_TYPE_SCROLL_BAR_MOBILE
 *
 */
#define WIDGET_TYPE_SCROLL_BAR_MOBILE "scroll_bar_m"

/**
 * @const WIDGET_TYPE_SCROLL_VIEW
 *
 */
#define WIDGET_TYPE_SCROLL_VIEW "scroll_view"

/**
 * @const WIDGET_TYPE_LIST_VIEW
 *
 */
#define WIDGET_TYPE_LIST_VIEW "list_view"

/**
 * @const WIDGET_TYPE_LIST_VIEW_H
 *
 */
#define WIDGET_TYPE_LIST_VIEW_H "list_view_h"

/**
 * @const WIDGET_TYPE_LIST_ITEM
 *
 */
#define WIDGET_TYPE_LIST_ITEM "list_item"

/**
 * @const WIDGET_TYPE_COLOR_PICKER
 *
 */
#define WIDGET_TYPE_COLOR_PICKER "color_picker"

/**
 * @const WIDGET_TYPE_COLOR_COMPONENT
 *
 */
#define WIDGET_TYPE_COLOR_COMPONENT "color_component"

/**
 * @const WIDGET_TYPE_COLOR_TILE
 *
 */
#define WIDGET_TYPE_COLOR_TILE "color_tile"

/**
 * @const WIDGET_TYPE_RICH_TEXT
 *
 */
#define WIDGET_TYPE_RICH_TEXT "rich_text"

/**
 * @const WIDGET_TYPE_APP_BAR
 *
 */
#define WIDGET_TYPE_APP_BAR "app_bar"

/**
 * @const WIDGET_TYPE_GRID
 *
 */
#define WIDGET_TYPE_GRID "grid"

/**
 * @const WIDGET_TYPE_ROW
 *
 */
#define WIDGET_TYPE_ROW "row"

/**
 * @const WIDGET_TYPE_COLUMN
 *
 */
#define WIDGET_TYPE_COLUMN "column"

/**
 * @const WIDGET_TYPE_CALIBRATION_WIN
 *
 */
#define WIDGET_TYPE_CALIBRATION_WIN "calibration_win"

/**
 * @enum widget_state_t
 * @annotation ["scriptable"]
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
   * @const WIDGET_STATE_SELECTED
   * 选中状态。
   */
  WIDGET_STATE_SELECTED = 10,

  /*for active/selected/current */
  /**
   * @const WIDGET_STATE_NORMAL_OF_CHECKED
   * 正常状态(选中项)。
   */
  WIDGET_STATE_NORMAL_OF_CHECKED = 101,
  /**
   * @const WIDGET_STATE_PRESSED_OF_CHECKED
   * 指针按下状态(选中项)。
   */
  WIDGET_STATE_PRESSED_OF_CHECKED = 102,
  /**
   * @const WIDGET_STATE_OVER_OF_CHECKED
   * 指针悬浮状态(选中项)。
   */
  WIDGET_STATE_OVER_OF_CHECKED = 103,
  /**
   * @const WIDGET_STATE_NORMAL_OF_ACTIVE
   * 正常状态(当前项)。
   */
  WIDGET_STATE_NORMAL_OF_ACTIVE = 101,
  /**
   * @const WIDGET_STATE_PRESSED_OF_ACTIVE
   * 指针按下状态(当前项)。
   */
  WIDGET_STATE_PRESSED_OF_ACTIVE = 102,
  /**
   * @const WIDGET_STATE_OVER_OF_ACTIVE
   * 指针悬浮状态(当前项)。
   */
  WIDGET_STATE_OVER_OF_ACTIVE = 103
} widget_state_t;

/**
 * @enum window_stage_t
 * @annotation ["scriptable"]
 * @prefix WINDOW_STAGE_
 * 窗口的生命周期常量定义。
 */
typedef enum _window_stage_t {
  /**
   * @const WINDOW_STAGE_NONE
   * 初始状态。
   */
  WINDOW_STAGE_NONE = 0,
  /**
   * @const WINDOW_STAGE_CREATED
   * 创建完成。
   */
  WINDOW_STAGE_CREATED,
  /**
   * @const WINDOW_STAGE_OPENED
   * 窗口已经打开(窗口打开动画完成后，处于该状态，直到窗口被关闭)
   */
  WINDOW_STAGE_OPENED,
  /**
   * @const WINDOW_STAGE_CLOSED
   * 窗口已关闭。
   */
  WINDOW_STAGE_CLOSED
} window_stage_t;

END_C_DECLS

#endif /*TK_WIDGET_CONSTS_H*/
